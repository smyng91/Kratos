//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Michael Andre, https://github.com/msandre
//

#if !defined(KRATOS_RESPONSE_FUNCTION_SENSITIVITY_BUILDER_UTILITY_H_INCLUDED)
#define KRATOS_RESPONSE_FUNCTION_SENSITIVITY_BUILDER_UTILITY_H_INCLUDED

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "includes/model_part.h"
#include "utilities/openmp_utils.h"
#include "response_functions/adjoint_response_function.h"

namespace Kratos
{
///@name Kratos Classes
///@{

/// Utility for assembling sensitivities from a model part and a response function.
class ResponseFunctionSensitivityBuilderUtility
{
public:
    ///@name Type Definitions
    ///@{

    typedef Geometry<Node<3>> GeometryType;

    typedef ModelPart::NodesContainerType NodesContainerType;

    typedef ModelPart::ElementsContainerType ElementsContainerType;

    typedef ModelPart::ConditionsContainerType ConditionsContainerType;

    ///@}
    ///@name Life Cycle
    ///@{

    ResponseFunctionSensitivityBuilderUtility(ModelPart& rModelPart,
                                              AdjointResponseFunction::Pointer pResponseFunction)
        : mrModelPart(rModelPart), mpResponseFunction(pResponseFunction)
    {
    }

    ///@}
    ///@name Operations
    ///@{

    void BuildNodalSolutionStepSensitivities(std::string const& rVariable,
                                             double ScalingFactor = 1.0)
    {
        KRATOS_TRY;

        if (KratosComponents<Variable<double>>::Has(rVariable) == true)
        {
            const Variable<double>& r_variable =
                KratosComponents<Variable<double>>::Get(rVariable);
            BuildNodalSolutionStepSensitivities(r_variable, ScalingFactor);
        }
        else if (KratosComponents<Variable<array_1d<double, 3>>>::Has(rVariable) == true)
        {
            const Variable<array_1d<double, 3>>& r_variable =
                KratosComponents<Variable<array_1d<double, 3>>>::Get(rVariable);
            BuildNodalSolutionStepSensitivities(r_variable, ScalingFactor);
        }
        else
            KRATOS_ERROR << "Unsupported variable: " << rVariable << "." << std::endl;

        KRATOS_CATCH("");
    }

    ///@}

private:

    ///@name Private Operations
    ///@{

    template <class TDataType>
    void BuildNodalSolutionStepSensitivities(Variable<TDataType> const& rVariable,
                                             double ScalingFactor)
    {
        KRATOS_TRY;

        Communicator& r_comm = mrModelPart.GetCommunicator();
        if (r_comm.TotalProcesses() > 1)
        {
// Make sure we only add the old sensitivity once when we assemble.
#pragma omp parallel
            {
                ModelPart::NodeIterator nodes_begin;
                ModelPart::NodeIterator nodes_end;
                OpenMPUtils::PartitionedIterators(mrModelPart.Nodes(), nodes_begin, nodes_end);
                for (auto it = nodes_begin; it != nodes_end; ++it)
                    if (it->FastGetSolutionStepValue(PARTITION_INDEX) != r_comm.MyPID())
                        it->FastGetSolutionStepValue(rVariable) = rVariable.Zero();
            }
        }

        BuildNodalSolutionStepElementContributions(rVariable, ScalingFactor);

        BuildNodalSolutionStepConditionContributions(rVariable, ScalingFactor);

        r_comm.AssembleCurrentData(rVariable);

        KRATOS_CATCH("");
    }

    ///@}

private:
    ///@name Member Variables
    ///@{

    ModelPart& mrModelPart;
    AdjointResponseFunction::Pointer mpResponseFunction;

    ///@}
    ///@name Private Operations
    ///@{

    template <class TDataType>
    void BuildNodalSolutionStepElementContributions(Variable<TDataType> const& rVariable,
                                                    double ScalingFactor)
    {
        KRATOS_TRY;

        auto& r_elements = mrModelPart.Elements();
        const auto& r_process_info = mrModelPart.GetProcessInfo();

        const int num_threads = OpenMPUtils::GetNumThreads();
        std::vector<Vector> local_sensitivity(num_threads);
        std::vector<Vector> partial_sensitivity(num_threads);
        std::vector<Vector> adjoint_vector(num_threads);
        std::vector<Matrix> sensitivity_matrix(num_threads);

#pragma omp parallel
        {
            ModelPart::ElementIterator elements_begin;
            ModelPart::ElementIterator elements_end;
            OpenMPUtils::PartitionedIterators(r_elements, elements_begin, elements_end);
            int k = OpenMPUtils::ThisThread();

            for (auto it = elements_begin; it != elements_end; ++it)
            {
                Element::GeometryType& r_geom = it->GetGeometry();

                if (HasActiveNodes(r_geom) == false)
                    continue;

                it->CalculateSensitivityMatrix(rVariable, sensitivity_matrix[k], r_process_info);

                mpResponseFunction->CalculatePartialSensitivity(
                    *it, rVariable, sensitivity_matrix[k],
                    partial_sensitivity[k], r_process_info);

                it->GetValuesVector(adjoint_vector[k]);

                if (local_sensitivity[k].size() != sensitivity_matrix[k].size1())
                    local_sensitivity[k].resize(sensitivity_matrix[k].size1(), false);

                noalias(local_sensitivity[k]) =
                    ScalingFactor * (prod(sensitivity_matrix[k], adjoint_vector[k]) +
                                     partial_sensitivity[k]);

                AssembleNodalSolutionStepSensitivityContribution(
                    rVariable, local_sensitivity[k], r_geom);
            }
        }

        KRATOS_CATCH("");
    }

    template <class TDataType>
    void BuildNodalSolutionStepConditionContributions(Variable<TDataType> const& rVariable,
                                                      double ScalingFactor)
    {
        KRATOS_TRY;

        auto& r_conditions = mrModelPart.Conditions();
        const auto& r_process_info = mrModelPart.GetProcessInfo();

        const int num_threads = OpenMPUtils::GetNumThreads();
        std::vector<Vector> local_sensitivity(num_threads);
        std::vector<Vector> partial_sensitivity(num_threads);
        std::vector<Vector> adjoint_vector(num_threads);
        std::vector<Matrix> sensitivity_matrix(num_threads);

#pragma omp parallel
        {
            ModelPart::ConditionIterator conditions_begin;
            ModelPart::ConditionIterator conditions_end;
            OpenMPUtils::PartitionedIterators(r_conditions, conditions_begin, conditions_end);
            int k = OpenMPUtils::ThisThread();

            for (auto it = conditions_begin; it != conditions_end; ++it)
            {
                Condition::GeometryType& r_geom = it->GetGeometry();

                it->CalculateSensitivityMatrix(rVariable, sensitivity_matrix[k], r_process_info);

                if (sensitivity_matrix[k].size1() == 0 || HasActiveNodes(r_geom) == false)
                    continue;

                mpResponseFunction->CalculatePartialSensitivity(
                    *it, rVariable, sensitivity_matrix[k],
                    partial_sensitivity[k], r_process_info);

                it->GetValuesVector(adjoint_vector[k]);

                if (local_sensitivity[k].size() != sensitivity_matrix[k].size1())
                    local_sensitivity[k].resize(sensitivity_matrix[k].size1(), false);

                noalias(local_sensitivity[k]) =
                    ScalingFactor * (prod(sensitivity_matrix[k], adjoint_vector[k]) +
                                     partial_sensitivity[k]);

                AssembleNodalSolutionStepSensitivityContribution(
                    rVariable, local_sensitivity[k], r_geom);
            }
        }

        KRATOS_CATCH("");
    }

    bool HasActiveNodes(GeometryType const& rGeom)
    {
        bool result = false;
        for (unsigned i_node = 0; i_node < rGeom.PointsNumber(); ++i_node)
            if (rGeom[i_node].GetValue(UPDATE_SENSITIVITIES))
            {
                result = true;
                break;
            }
        return result;
    }

    void AssembleNodalSolutionStepSensitivityContribution(Variable<double> const& rVariable,
                                                          Vector const& rSensitivityVector,
                                                          GeometryType& rGeom)
    {
        unsigned int index = 0;
        for (unsigned int i_node = 0; i_node < rGeom.PointsNumber(); ++i_node)
        {
            if (rGeom[i_node].GetValue(UPDATE_SENSITIVITIES))
            {
                double& r_sensitivity = rGeom[i_node].FastGetSolutionStepValue(rVariable);
                rGeom[i_node].SetLock();
                r_sensitivity += rSensitivityVector[index++];
                rGeom[i_node].UnSetLock();
            }
            else
                ++index;
        }
    }

    void AssembleNodalSolutionStepSensitivityContribution(Variable<array_1d<double, 3>> const& rVariable,
                                                          Vector const& rSensitivityVector,
                                                          GeometryType& rGeom)
    {
        unsigned int index = 0;
        for (unsigned int i_node = 0; i_node < rGeom.PointsNumber(); ++i_node)
        {
            if (rGeom[i_node].GetValue(UPDATE_SENSITIVITIES))
            {
                array_1d<double, 3>& r_sensitivity =
                    rGeom[i_node].FastGetSolutionStepValue(rVariable);
                rGeom[i_node].SetLock();
                for (unsigned int d = 0; d < rGeom.WorkingSpaceDimension(); ++d)
                    r_sensitivity[d] += rSensitivityVector[index++];
                rGeom[i_node].UnSetLock();
            }
            else
                index += rGeom.WorkingSpaceDimension();
        }
    }

    ///@}

};

///@} // Kratos Classes
} /* namespace Kratos.*/

#endif /* KRATOS_RESPONSE_FUNCTION_SENSITIVITY_BUILDER_UTILITY_H_INCLUDED defined */

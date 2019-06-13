//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Suneth Warnakulasuriya (https://github.com/sunethwarna)
//

#if !defined(KRATOS_RANS_K_WALL_FRICTION_VELOCITY_PROCESS_H_INCLUDED)
#define KRATOS_RANS_K_WALL_FRICTION_VELOCITY_PROCESS_H_INCLUDED

// System includes
#include <cmath>
#include <string>

// External includes

// Project includes
#include "containers/model.h"
#include "custom_utilities/rans_variable_utils.h"
#include "includes/cfd_variables.h"
#include "includes/checks.h"
#include "includes/define.h"
#include "includes/model_part.h"
#include "processes/process.h"
#include "rans_modelling_application_variables.h"

namespace Kratos
{
///@addtogroup RANSModellingApplication
///@{

///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

/// Auxiliary process to set Boussinesq buoyancy forces in variable temperature flows.
/** This process modifies the BODY_FORCE variable according to the Boussinesq hypothesis
    so that the fluid element can take natural convection into account.

    This process makes use of the following data:
    - TEMPERATURE from the nodal solution step data: current temperature for the node (mandatory).
    - AMBIENT_TEMPERATURE from ProcessInfo: The reference temperature for the simulation (mandatory).
    - gravity from the Parameters passed in the constructor: an array that defines the gravity vector (mandatory).
    - thermal_expansion_coefficient from the Parameters: a double defining the thermal expansion coefficient for the fluid (optional).

    With this, the process calculates the Boussinesq force and assings it to the BODY_FORCE solution step variable of each node.
    The force is set to (1 + thermal_expansion_coefficient*(temperature - ambient_temperature) ) * g

    If the thermal expansion coefficient is not provided, it is assumed to be (1/ambient_temperature).
    This is the usual value for perfect gases (if the temperature is given in Kelvin).
 */

class RansKWallFrictionVelocityProcess : public Process
{
public:
    ///@name Type Definitions
    ///@{

    typedef ModelPart::NodeType NodeType;

    typedef ModelPart::NodesContainerType NodesContainerType;

    /// Pointer definition of RansKWallFrictionVelocityProcess
    KRATOS_CLASS_POINTER_DEFINITION(RansKWallFrictionVelocityProcess);

    ///@}
    ///@name Life Cycle
    ///@{

    /// Constructor
    RansKWallFrictionVelocityProcess(Model& rModel, Parameters& rParameters)
        : mrModel(rModel), mrParameters(rParameters)
    {
        KRATOS_TRY

        Parameters default_parameters = Parameters(R"(
        {
            "model_part_name"     : "PLEASE_SPECIFY_MODEL_PART_NAME",
            "c_mu"                : 0.09,
            "echo_level"          : 0,
            "is_fixed"            : true,
            "min_k_value"         : 1e-18
        })");

        mrParameters.ValidateAndAssignDefaults(default_parameters);

        mCmu = mrParameters["c_mu"].GetDouble();
        mIsConstrained = mrParameters["is_fixed"].GetBool();
        mEchoLevel = mrParameters["echo_level"].GetInt();
        mModelPartName = mrParameters["model_part_name"].GetString();
        mMinValue = mrParameters["min_k_value"].GetDouble();

        KRATOS_CATCH("");
    }
    /// Destructor.
    ~RansKWallFrictionVelocityProcess() override
    {
        // delete mpDistanceCalculator;
    }

    ///@}
    ///@name Operators
    ///@{

    ///@}
    ///@name Operations
    ///@{

    void ExecuteInitialize() override
    {
        if (mIsConstrained)
        {
            ModelPart::NodesContainerType& r_nodes =
                mrModel.GetModelPart(mModelPartName).Nodes();
            int number_of_nodes = r_nodes.size();
#pragma omp parallel for
            for (int i_node = 0; i_node < number_of_nodes; ++i_node)
            {
                NodeType& r_node = *(r_nodes.begin() + i_node);
                r_node.Fix(TURBULENT_KINETIC_ENERGY);
            }

            KRATOS_INFO_IF(this->Info(), mEchoLevel > 0)
                << "Fixed TURBULENT_KINETIC_ENERGY dofs in " << mModelPartName << ".\n";
        }
    }

    void Execute() override
    {
        KRATOS_TRY

        ModelPart::NodesContainerType& r_nodes =
            mrModel.GetModelPart(mModelPartName).Nodes();
        int number_of_nodes = r_nodes.size();

#pragma omp parallel for
        for (int i_node = 0; i_node < number_of_nodes; ++i_node)
        {
            NodeType& r_node = *(r_nodes.begin() + i_node);
            CalculateTurbulentValues(r_node);
        }

        KRATOS_INFO_IF(this->Info(), mEchoLevel > 0)
            << "Applied k values to " << mModelPartName << ".\n";

        KRATOS_CATCH("");
    }

    int Check() override
    {
        // Checking variable definitions
        KRATOS_CHECK_VARIABLE_KEY(VELOCITY);
        KRATOS_CHECK_VARIABLE_KEY(TURBULENT_KINETIC_ENERGY);

        ModelPart::NodesContainerType& r_nodes =
            mrModel.GetModelPart(mModelPartName).Nodes();
        int number_of_nodes = r_nodes.size();

#pragma omp parallel for
        for (int i_node = 0; i_node < number_of_nodes; ++i_node)
        {
            NodeType& r_node = *(r_nodes.begin() + i_node);
            KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(TURBULENT_KINETIC_ENERGY, r_node);
            KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(VELOCITY, r_node);
        }

        return 0;
    }

    ///@}
    ///@name Access
    ///@{

    ///@}
    ///@name Inquiry
    ///@{

    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    std::string Info() const override
    {
        return std::string("RansKWallFrictionVelocityProcess");
    }

    /// Print information about this object.
    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << this->Info();
    }

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const override
    {
    }

    ///@}
    ///@name Friends
    ///@{

    ///@}

protected:
    ///@name Protected static Member Variables
    ///@{

    ///@}
    ///@name Protected member Variables
    ///@{

    ///@}
    ///@name Protected Operators
    ///@{

    ///@}
    ///@name Protected Operations
    ///@{

    ///@}
    ///@name Protected  Access
    ///@{

    ///@}
    ///@name Protected Inquiry
    ///@{

    ///@}
    ///@name Protected LifeCycle
    ///@{

    ///@}

private:
    ///@name Static Member Variables
    ///@{

    ///@}
    ///@name Member Variables
    ///@{

    Model& mrModel;
    Parameters& mrParameters;
    std::string mModelPartName;

    double mCmu;
    double mMinValue;
    int mEchoLevel;

    bool mIsConstrained;

    ///@}
    ///@name Private Operators
    ///@{

    ///@}
    ///@name Private Operations
    ///@{

    void CalculateTurbulentValues(NodeType& rNode)
    {
        const double y_plus = rNode.FastGetSolutionStepValue(RANS_Y_PLUS);
        const double nu = rNode.FastGetSolutionStepValue(KINEMATIC_VISCOSITY);
        const double wall_distance = rNode.FastGetSolutionStepValue(DISTANCE);
        const double u_tau = y_plus * nu / wall_distance;

        rNode.FastGetSolutionStepValue(TURBULENT_KINETIC_ENERGY) =
            std::min(mMinValue, std::pow(u_tau, 2) / std::sqrt(mCmu));
    }

    ///@}
    ///@name Private  Access
    ///@{

    ///@}
    ///@name Private Inquiry
    ///@{

    ///@}
    ///@name Un accessible methods
    ///@{

    /// Assignment operator.
    RansKWallFrictionVelocityProcess& operator=(RansKWallFrictionVelocityProcess const& rOther);

    /// Copy constructor.
    RansKWallFrictionVelocityProcess(RansKWallFrictionVelocityProcess const& rOther);

    ///@}

}; // Class RansKWallFrictionVelocityProcess

///@}

///@name Type Definitions
///@{

///@}
///@name Input and output
///@{

/// output stream function
inline std::ostream& operator<<(std::ostream& rOStream,
                                const RansKWallFrictionVelocityProcess& rThis);

///@}

///@} addtogroup block

} // namespace Kratos.

#endif // KRATOS_RANS_K_WALL_FRICTION_VELOCITY_PROCESS_H_INCLUDED defined

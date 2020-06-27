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

// System includes
#include <functional>
#include <limits>

// External includes

// Project includes
#include "utilities/geometry_utilities.h"
#include "utilities/math_utils.h"
#include "utilities/parallel_utilities.h"
#include "utilities/variable_utils.h"

// Application includes
#include "fluid_dynamics_application_variables.h"

// Include base h
#include "cfd_utilities.h"

namespace Kratos
{
namespace CFDUtilities
{
void CalculateConditionGeometryData(
    const GeometryType& rGeometry,
    const GeometryData::IntegrationMethod& rIntegrationMethod,
    Vector& rGaussWeights,
    Matrix& rNContainer)
{
    const GeometryType::IntegrationPointsArrayType& integration_points =
        rGeometry.IntegrationPoints(rIntegrationMethod);

    const std::size_t number_of_integration_points = integration_points.size();
    const int dimension = rGeometry.WorkingSpaceDimension();
    const double domain_size = rGeometry.DomainSize();

    if (rGaussWeights.size() != number_of_integration_points)
    {
        rGaussWeights.resize(number_of_integration_points, false);
    }

    rNContainer = rGeometry.ShapeFunctionsValues(rIntegrationMethod);

    // CAUTION: "Jacobian" is 2.0*A for triangles but 0.5*A for lines
    double det_J = (dimension == 2) ? 0.5 * domain_size : 2.0 * domain_size;

    for (unsigned int g = 0; g < number_of_integration_points; g++)
    {
        rGaussWeights[g] = det_J * integration_points[g].Weight();
    }
}

double CalculateConditionWallHeight(const ConditionType& rCondition, const array_1d<double, 3>& rNormal)
{
    KRATOS_TRY

    array_1d<double, 3> normal = rNormal / norm_2(rNormal);

    const ElementType& r_parent_element = rCondition.GetValue(NEIGHBOUR_ELEMENTS)[0];

    const GeometryType& r_parent_geometry = r_parent_element.GetGeometry();
    const GeometryType& r_condition_geometry = rCondition.GetGeometry();

    const array_1d<double, 3>& parent_center = r_parent_geometry.Center();
    const array_1d<double, 3>& condition_center = r_condition_geometry.Center();

    return inner_prod(condition_center - parent_center, normal);

    KRATOS_CATCH("");
}

void CalculateNumberOfNeighbourConditions(ModelPart& rModelPart)
{
    KRATOS_TRY

    ModelPart::NodesContainerType& r_nodes = rModelPart.Nodes();

    // reseting all nodal variables which needs to be calculated
    VariableUtils().SetNonHistoricalVariableToZero(NUMBER_OF_NEIGHBOUR_CONDITIONS, r_nodes);

    // updating
    block_for_each(rModelPart.Conditions(), [](ConditionType& r_cond) {
        ConditionType::GeometryType& r_geometry = r_cond.GetGeometry();
        for (IndexType i_node = 0; i_node < r_geometry.PointsNumber(); ++i_node)
        {
            NodeType& r_node = r_geometry[i_node];
            r_node.SetLock();
            r_node.GetValue(NUMBER_OF_NEIGHBOUR_CONDITIONS) += 1.0;
            r_node.UnSetLock();
        }
    });

    rModelPart.GetCommunicator().AssembleNonHistoricalData(NUMBER_OF_NEIGHBOUR_CONDITIONS);

    KRATOS_CATCH("");
}

double CalculateReactionBasedYPlusUTau(
    array_1d<double, 3>& rFrictionVelocity,
    const array_1d<double, 3>& rReaction,
    const array_1d<double, 3>& rNormal,
    const double Density,
    const double KinematicViscosity,
    const double WallHeight)
{
    KRATOS_TRY

    // calculating unit normal since rNormal contains surface area of the condition
    const double surface_area = norm_2(rNormal);
    const array_1d<double, 3>& unit_normal = rNormal / surface_area;

    // calculate tangential stress
    noalias(rFrictionVelocity) =
        (rReaction - unit_normal * inner_prod(rReaction, unit_normal)) / surface_area;
    const double shear_stress = norm_2(rFrictionVelocity);

    // calculate y_plus
    const double y_plus = std::sqrt(shear_stress / Density) * WallHeight / KinematicViscosity;

    // calculate u_tau
    noalias(rFrictionVelocity) =
        rFrictionVelocity /
        std::sqrt((shear_stress <= std::numeric_limits<double>::epsilon() ? 1.0 : shear_stress) * Density);

    return y_plus;

    KRATOS_CATCH("");
}

double CalculateLinearLogarithmicWallFunctionBasedYPlusLimit(
    const double VonKarman, const double WallSmoothness, const int MaxIterations, const double Tolerance)
{
    double y_plus = 11.06;
    const double inv_kappa = 1.0 / VonKarman;
    double dx = 0.0;
    for (int i = 0; i < MaxIterations; ++i)
    {
        const double value = inv_kappa * std::log(y_plus) + WallSmoothness;
        dx = value - y_plus;

        if (std::abs(dx) < Tolerance)
            return y_plus;

        y_plus = value;
    }

    KRATOS_WARNING("CalculateLinearLogarithmicWallFunctionBasedYPlusLimit")
        << "Logarithmic y_plus limit reached max iterations with dx > "
           "Tolerance [ "
        << dx << " > " << Tolerance << ", MaxIterations = " << MaxIterations << " ].\n";
    return y_plus;
}

double CalculateLinearLogarithmicWallFunctionBasedYPlusAndUtau(
    array_1d<double, 3>& rFrictionVelocity,
    const array_1d<double, 3>& rWallVelocity,
    const array_1d<double, 3>& rNormal,
    const double KinematicViscosity,
    const double WallHeight,
    const double VonKarman,
    const double WallSmoothness,
    const int MaxIterations,
    const double Tolerance)
{
    KRATOS_TRY

    // calculating unit normal since rNormal contains surface area of the condition
    const double surface_area = norm_2(rNormal);
    const array_1d<double, 3>& unit_normal = rNormal / surface_area;

    // calculate tangential velocity
    noalias(rFrictionVelocity) =
        rWallVelocity - unit_normal * inner_prod(rWallVelocity, unit_normal);
    const double wall_velocity = norm_2(rFrictionVelocity);

    // calculate linear log region limit
    const double limit_y_plus = CalculateLinearLogarithmicWallFunctionBasedYPlusLimit(
        VonKarman, WallSmoothness, MaxIterations, Tolerance);

    // linear region
    double u_tau = std::sqrt(wall_velocity * KinematicViscosity / WallHeight);
    double y_plus = u_tau * WallHeight / KinematicViscosity;
    const double inv_kappa = 1.0 / VonKarman;

    // log region
    if (y_plus > limit_y_plus)
    {
        int iter = 0;
        double dx = 1e10;
        double u_plus = inv_kappa * std::log(y_plus) + WallSmoothness;

        while (iter < MaxIterations && std::abs(dx) > Tolerance * u_tau)
        {
            // Newton-Raphson iteration
            double f = u_tau * u_plus - wall_velocity;
            double df = u_plus + inv_kappa;
            dx = f / df;

            // Update variables
            u_tau -= dx;
            y_plus = WallHeight * u_tau / KinematicViscosity;
            u_plus = inv_kappa * std::log(y_plus) + WallSmoothness;
            ++iter;
        }

        KRATOS_WARNING_IF("CFDUtilities", iter >= MaxIterations)
            << "CalculateLinearLogarithmicWallFunctionBasedYPlusAndUtau "
               "couldn't converge Newton-Raphson. Residual is "
            << dx << std::endl;
    }

    noalias(rFrictionVelocity) =
        rFrictionVelocity *
        (-1.0 * u_tau /
         (wall_velocity <= std::numeric_limits<double>::epsilon() ? 1.0 : wall_velocity));

    return y_plus;

    KRATOS_CATCH("");
}

void CalculateYPlusAndUTauForConditions(
    ModelPart& rModelPart,
    const Variable<double>& rKinematicViscosityVariable,
    const std::function<double(
        array_1d<double, 3>&,
        const GeometryType&,
        const array_1d<double, 3>&,
        const Vector&,
        const double,
        const double,
        const double
        )>& rYPlusAndUTauCalculationMethod)
{
    KRATOS_TRY

    const array_1d<double, 3> local_point = ZeroVector(3);

    KRATOS_ERROR_IF(!rModelPart.HasNodalSolutionStepVariable(rKinematicViscosityVariable))
        << rModelPart.Name() << " doesn't have " << rKinematicViscosityVariable.Name()
        << " in nodal solution step variable list. Please add it first.\n";
    KRATOS_ERROR_IF(!rModelPart.HasNodalSolutionStepVariable(DENSITY))
        << rModelPart.Name() << " doesn't have " << DENSITY.Name()
        << " in nodal solution step variable list. Please add it first.\n";

    block_for_each(
        rModelPart.Conditions(), [local_point, rKinematicViscosityVariable,
                                  rYPlusAndUTauCalculationMethod](ConditionType& r_condition) {
            GeometryType& r_geometry = r_condition.GetGeometry();

            Vector gauss_weights;
            Matrix shape_functions;
            CalculateConditionGeometryData(
                r_condition.GetGeometry(), GeometryData::GI_GAUSS_1,
                gauss_weights, shape_functions);

            const Vector& gauss_shape_functions = row(shape_functions, 0);

            // calculate normal for the condition
            const array_1d<double, 3> normal = r_geometry.Normal(local_point);
            const double y_wall = CalculateConditionWallHeight(r_condition, normal);
            double nu;
            GeometryUtils::EvaluateHistoricalVariableValueAtGaussPoint(
                nu, r_geometry, rKinematicViscosityVariable, gauss_shape_functions);
            double rho;
            GeometryUtils::EvaluateHistoricalVariableValueAtGaussPoint(
                rho, r_geometry, DENSITY, gauss_shape_functions);

            array_1d<double, 3> u_tau;
            const double y_plus = rYPlusAndUTauCalculationMethod(
                u_tau, r_geometry, normal, gauss_shape_functions, rho, nu, y_wall);

            r_condition.SetValue(FRICTION_VELOCITY, u_tau);
            r_condition.SetValue(Y_PLUS, y_plus);
        });

    KRATOS_CATCH("");
}

void CalculateYPlusAndUTauForConditionsBasedOnReaction(
    ModelPart& rModelPart,
    const Variable<double>& rKinematicViscosityVariable,
    const Variable<array_1d<double, 3>>& rReactionVariable)
{
    KRATOS_TRY

    KRATOS_ERROR_IF(!rModelPart.HasNodalSolutionStepVariable(rReactionVariable))
        << rModelPart.Name() << " doesn't have " << rReactionVariable.Name()
        << " in nodal solution step variable list. Please add it first.\n";

    CalculateNumberOfNeighbourConditions(rModelPart);

    const std::function<double(
        array_1d<double, 3>&, const GeometryType&, const array_1d<double, 3>&,
        const Vector&, const double, const double, const double)>
        method = [rReactionVariable](
                     array_1d<double, 3>& rFrictionVelocity,
                     const GeometryType& rGeometry, const array_1d<double, 3>& rNormal,
                     const Vector& rGaussShapeFunctions, const double Density,
                     const double KinematicViscosity, const double WallHeight) {
            array_1d<double, 3> reaction =
                rGeometry[0].FastGetSolutionStepValue(rReactionVariable) /
                rGeometry[0].GetValue(NUMBER_OF_NEIGHBOUR_CONDITIONS);
            for (unsigned int i_node = 1; i_node < rGeometry.PointsNumber(); ++i_node)
            {
                const NodeType& r_node = rGeometry[i_node];
                noalias(reaction) += r_node.FastGetSolutionStepValue(rReactionVariable) /
                                     r_node.GetValue(NUMBER_OF_NEIGHBOUR_CONDITIONS);
            }

            return CalculateReactionBasedYPlusUTau(
                rFrictionVelocity, reaction, rNormal, Density, KinematicViscosity, WallHeight);
        };

    CalculateYPlusAndUTauForConditions(rModelPart, rKinematicViscosityVariable, method);

    KRATOS_CATCH("");
}

void CalculateYPlusAndUTauForConditionsBasedOnLinearLogarithmicWallFunction(
    ModelPart& rModelPart,
    const Variable<double>& rKinematicViscosityVariable,
    const double VonKarman,
    const double WallSmoothness,
    const int MaxIterations,
    const double Tolerance)
{
    KRATOS_TRY

    KRATOS_ERROR_IF(VonKarman <= 0.0)
        << "Von Karman constant needs to be greater than zero. [ " << VonKarman
        << " !> 0.0 ].\n";
    KRATOS_ERROR_IF(WallSmoothness <= 0.0)
        << "Wall smoothness parameter needs to be greater than zero. [ "
        << WallSmoothness << " !> 0.0 ].\n";
    KRATOS_ERROR_IF(MaxIterations < 0)
        << "Max iterations needs to be greater than zero. [ " << MaxIterations
        << " !> 0 ].\n";
    KRATOS_ERROR_IF(Tolerance < 0.0)
        << "Tolerance needs to be greater than zero. [ " << Tolerance << " !> 0.0 ].\n";
    KRATOS_ERROR_IF(!rModelPart.HasNodalSolutionStepVariable(VELOCITY))
        << rModelPart.Name() << " doesn't have " << VELOCITY.Name()
        << " in nodal solution step variable list. Please add it first.\n";
    KRATOS_ERROR_IF(!rModelPart.HasNodalSolutionStepVariable(MESH_VELOCITY))
        << rModelPart.Name() << " doesn't have " << MESH_VELOCITY.Name()
        << " in nodal solution step variable list. Please add it first.\n";

    const std::function<double(
        array_1d<double, 3>&, const GeometryType&, const array_1d<double, 3>&,
        const Vector&, const double, const double, const double)>
        method = [VonKarman, WallSmoothness, MaxIterations, Tolerance](
                     array_1d<double, 3>& rFrictionVelocity,
                     const GeometryType& rGeometry, const array_1d<double, 3>& rNormal,
                     const Vector& rGaussShapeFunctions, const double Density,
                     const double KinematicViscosity, const double WallHeight) {
            array_1d<double, 3> r_wall_velocity;
            GeometryUtils::EvaluateHistoricalVariableValueAtGaussPoint(
                r_wall_velocity, rGeometry, VELOCITY, rGaussShapeFunctions);
            array_1d<double, 3> r_mesh_velocity;
            GeometryUtils::EvaluateHistoricalVariableValueAtGaussPoint(
                r_mesh_velocity, rGeometry, MESH_VELOCITY, rGaussShapeFunctions);

            return CalculateLinearLogarithmicWallFunctionBasedYPlusAndUtau(
                rFrictionVelocity, r_wall_velocity - r_mesh_velocity, rNormal, KinematicViscosity,
                WallHeight, VonKarman, WallSmoothness, MaxIterations, Tolerance);
        };

    CalculateYPlusAndUTauForConditions(rModelPart, rKinematicViscosityVariable, method);

    KRATOS_CATCH("");
}

} // namespace CFDUtilities

} // namespace Kratos.

/*
//  KRATOS  _____________
//         /  _/ ____/   |
//         / // / __/ /| |
//       _/ // /_/ / ___ |
//      /___/\____/_/  |_| Application
//
//  Main authors:   Thomas Oberbichler
*/

#if !defined(KRATOS_IGA_APPLICATION_VARIABLES_H_INCLUDED)
#define  KRATOS_IGA_APPLICATION_VARIABLES_H_INCLUDED

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "includes/variables.h"

namespace Kratos
{


KRATOS_DEFINE_VARIABLE(int, BREP_ID)

KRATOS_DEFINE_VARIABLE(double, NURBS_CONTROL_POINT_WEIGHT)

KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(COORDINATES)
KRATOS_DEFINE_VARIABLE(Vector, LOCAL_COORDINATES)
KRATOS_DEFINE_VARIABLE(Vector, TANGENTS)
KRATOS_DEFINE_VARIABLE(Vector, TANGENTS_SLAVE)

KRATOS_DEFINE_VARIABLE(double, CROSS_AREA)
KRATOS_DEFINE_VARIABLE(double, PRESTRESS_CAUCHY)

KRATOS_DEFINE_VARIABLE(Vector, SHAPE_FUNCTION_VALUES)
KRATOS_DEFINE_VARIABLE(Matrix, SHAPE_FUNCTION_LOCAL_DERIVATIVES)
KRATOS_DEFINE_VARIABLE(Matrix, SHAPE_FUNCTION_LOCAL_SECOND_DERIVATIVES)
KRATOS_DEFINE_VARIABLE(Matrix, SHAPE_FUNCTION_LOCAL_THIRD_DERIVATIVES)

KRATOS_DEFINE_VARIABLE(Vector, SHAPE_FUNCTION_VALUES_SLAVE)
KRATOS_DEFINE_VARIABLE(Matrix, SHAPE_FUNCTION_LOCAL_DERIVATIVES_SLAVE)
KRATOS_DEFINE_VARIABLE(Matrix, SHAPE_FUNCTION_LOCAL_SECOND_DERIVATIVES_SLAVE)

KRATOS_DEFINE_VARIABLE(double, RAYLEIGH_ALPHA)
KRATOS_DEFINE_VARIABLE(double, RAYLEIGH_BETA)

//Condition Variables
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(POINT_LOAD)
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(LINE_LOAD)
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_LOAD)

KRATOS_DEFINE_VARIABLE(double, PENALTY_FACTOR)

// Stress recovery
KRATOS_DEFINE_VARIABLE(double, STRESS_CAUCHY_11)
KRATOS_DEFINE_VARIABLE(double, STRESS_CAUCHY_12)
KRATOS_DEFINE_VARIABLE(double, STRESS_CAUCHY_22)
KRATOS_DEFINE_VARIABLE(double, STRESS_CAUCHY_23)
KRATOS_DEFINE_VARIABLE(double, STRESS_CAUCHY_13)
KRATOS_DEFINE_VARIABLE(double, STRESS_CAUCHY_TOP_11)
KRATOS_DEFINE_VARIABLE(double, STRESS_CAUCHY_TOP_12)
KRATOS_DEFINE_VARIABLE(double, STRESS_CAUCHY_TOP_22)
KRATOS_DEFINE_VARIABLE(double, STRESS_CAUCHY_BOTTOM_11)
KRATOS_DEFINE_VARIABLE(double, STRESS_CAUCHY_BOTTOM_12)
KRATOS_DEFINE_VARIABLE(double, STRESS_CAUCHY_BOTTOM_22)
KRATOS_DEFINE_VARIABLE(double, INTERNAL_FORCE_11)
KRATOS_DEFINE_VARIABLE(double, INTERNAL_FORCE_12)
KRATOS_DEFINE_VARIABLE(double, INTERNAL_FORCE_22)
KRATOS_DEFINE_VARIABLE(double, INTERNAL_MOMENT_11)
KRATOS_DEFINE_VARIABLE(double, INTERNAL_MOMENT_12)
KRATOS_DEFINE_VARIABLE(double, INTERNAL_MOMENT_22)
KRATOS_DEFINE_VARIABLE(double, SHEAR_FORCE_1)
KRATOS_DEFINE_VARIABLE(double, SHEAR_FORCE_2)

// DOF
KRATOS_DEFINE_VARIABLE(double, W_BAR)
KRATOS_DEFINE_VARIABLE(double, W_BAR_REACTION)

} // namespace Kratos

#endif // !defined(KRATOS_IGA_APPLICATION_VARIABLES_H_INCLUDED)
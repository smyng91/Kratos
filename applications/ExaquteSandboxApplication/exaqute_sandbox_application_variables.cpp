//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Riccardo Tosi
//

#include "exaqute_sandbox_application_variables.h"

namespace Kratos
{
KRATOS_CREATE_VARIABLE( double, DOF_1 )
KRATOS_CREATE_VARIABLE( double, DOF_2 )
KRATOS_CREATE_VARIABLE( double, ScalarVariable )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( VectorVariable )

}

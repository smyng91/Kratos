/*
==============================================================================
Kratos
A General Purpose Software for Multi-Physics Finite Element Analysis
Version 1.0 (Released on march 05, 2007).

Copyright 2007
Pooyan Dadvand, Riccardo Rossi
pooyan@cimne.upc.edu
rrossi@cimne.upc.edu
CIMNE (International Center for Numerical Methods in Engineering),
Gran Capita' s/n, 08034 Barcelona, Spain

Permission is hereby granted, free  of charge, to any person obtaining
a  copy  of this  software  and  associated  documentation files  (the
"Software"), to  deal in  the Software without  restriction, including
without limitation  the rights to  use, copy, modify,  merge, publish,
distribute,  sublicense and/or  sell copies  of the  Software,  and to
permit persons to whom the Software  is furnished to do so, subject to
the following condition:

Distribution of this code for  any  commercial purpose  is permissible
ONLY BY DIRECT ARRANGEMENT WITH THE COPYRIGHT OWNER.

The  above  copyright  notice  and  this permission  notice  shall  be
included in all copies or substantial portions of the Software.

THE  SOFTWARE IS  PROVIDED  "AS  IS", WITHOUT  WARRANTY  OF ANY  KIND,
EXPRESS OR  IMPLIED, INCLUDING  BUT NOT LIMITED  TO THE  WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT  SHALL THE AUTHORS OR COPYRIGHT HOLDERS  BE LIABLE FOR ANY
CLAIM, DAMAGES OR  OTHER LIABILITY, WHETHER IN AN  ACTION OF CONTRACT,
TORT  OR OTHERWISE, ARISING  FROM, OUT  OF OR  IN CONNECTION  WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

==============================================================================
*/
 
//   
//   Project Name:        Kratos       
//   Last Modified by:    $Author: mengmeng $
//   Date:                $Date: 2009-02-26 14:28:57 $
//   Revision:            $Revision: 1.16 $
//
//



// System includes
#include <string>
#include <iostream> 

// External includes 


// Project includes
#include "includes/define.h"
#include "includes/variables.h"
#include "includes/kernel.h"
#include "includes/node.h"
#include "includes/element.h"
#include "includes/condition.h"
#include "includes/constitutive_law.h"

//#include "geometries/triangle_2d_3.h"
#include "geometries/triangle_3d_3.h"
//#include "geometries/tetrahedra_3d_4.h"

#include "includes/convection_diffusion_settings.h"
#include "includes/radiation_settings.h"

namespace Kratos
{
 	
    KRATOS_CREATE_VARIABLE(ConvectionDiffusionSettings::Pointer,  CONVECTION_DIFFUSION_SETTINGS)	
    KRATOS_CREATE_VARIABLE(RadiationSettings::Pointer,  RADIATION_SETTINGS)

    KRATOS_CREATE_VARIABLE(bool, IS_INACTIVE)
    KRATOS_CREATE_VARIABLE(int, OSS_SWITCH)
    KRATOS_CREATE_VARIABLE(double, DYNAMIC_TAU)
    
    KRATOS_CREATE_VARIABLE(ConstitutiveLaw::Pointer, CONSTITUTIVE_LAW)

    KRATOS_CREATE_VARIABLE(vector<int>, NEIGHBOURS_INDICES)
    KRATOS_CREATE_VARIABLE(double, PARTITION_INDEX)
    KRATOS_CREATE_VARIABLE(double, ERASE_FLAG)
    KRATOS_CREATE_VARIABLE(int, NL_ITERATION_NUMBER)
    KRATOS_CREATE_VARIABLE(int, FRACTIONAL_STEP)
    KRATOS_CREATE_VARIABLE(double, TIME)
    KRATOS_CREATE_VARIABLE(int, TIME_STEPS)
    KRATOS_CREATE_VARIABLE(double, START_TIME)
    KRATOS_CREATE_VARIABLE(double, END_TIME)
    KRATOS_CREATE_VARIABLE(double, DELTA_TIME)
    KRATOS_CREATE_VARIABLE(double, TEMPERATURE)
    KRATOS_CREATE_VARIABLE(double, PRESSURE)
    KRATOS_CREATE_VARIABLE(double, DENSITY)
    KRATOS_CREATE_VARIABLE(double, VISCOSITY)
    KRATOS_CREATE_VARIABLE(double, ERROR_RATIO)

    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_ACCELERATION)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_VELOCITY)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(VELOCITY)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ROTATION)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MOMENTUM)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(REACTION)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(BODY_FORCE)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SEEPAGE_DRAG)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NORMAL)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(FORCE)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MOMENT)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MASS)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(RHS)
    KRATOS_CREATE_VARIABLE( double, MU )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(FORCE_CM)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MOMENTUM_CM)
    KRATOS_CREATE_VARIABLE(double, TAU)
    KRATOS_CREATE_VARIABLE(Matrix, INERTIA )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ADVPROJ)
    KRATOS_CREATE_VARIABLE(double, RHS_WATER)
    KRATOS_CREATE_VARIABLE(double, RHS_AIR)
    //NEIGHBOUR_NODES  is defined in Node.h
    KRATOS_CREATE_VARIABLE(WeakPointerVector<Node<3> >, NEIGHBOUR_NODES)

    //NEIGHBOR_ELEMENTS is defined in element.h
    KRATOS_CREATE_VARIABLE(WeakPointerVector<Element >, NEIGHBOUR_ELEMENTS)

    //NEIGHBOR_CONDITIONS is defined in condition.h
    KRATOS_CREATE_VARIABLE(WeakPointerVector<Condition >, NEIGHBOUR_CONDITIONS)

    //FATHER_NODES  is defined in Node.h
    KRATOS_CREATE_VARIABLE(WeakPointerVector<Node<3> >, FATHER_NODES)

    //for structural application TO BE REMOVED
    KRATOS_CREATE_VARIABLE(Vector, MATERIAL_PARAMETERS)
    KRATOS_CREATE_VARIABLE(int, WRINKLING_APPROACH )
    KRATOS_CREATE_VARIABLE(Matrix, GREEN_LAGRANGE_STRAIN_TENSOR )
    KRATOS_CREATE_VARIABLE(Matrix, PK2_STRESS_TENSOR )
    KRATOS_CREATE_VARIABLE(Matrix, CAUCHY_STRESS_TENSOR )
    KRATOS_CREATE_VARIABLE(Matrix, AUXILIARY_MATRIX_1 )
    KRATOS_CREATE_VARIABLE(double, YOUNG_MODULUS )
    KRATOS_CREATE_VARIABLE(double, POISSON_RATIO )
    KRATOS_CREATE_VARIABLE(double, THICKNESS )
    KRATOS_CREATE_VARIABLE(double, NEGATIVE_FACE_PRESSURE )
    KRATOS_CREATE_VARIABLE(double, POSITIVE_FACE_PRESSURE )
    KRATOS_CREATE_VARIABLE( double, INITIAL_PENALTY )
    KRATOS_CREATE_VARIABLE(Vector, INTERNAL_VARIABLES)
    KRATOS_CREATE_VARIABLE(double, DP_EPSILON)
    KRATOS_CREATE_VARIABLE(Vector, INSITU_STRESS )
    KRATOS_CREATE_VARIABLE(double, DP_ALPHA1 )
    KRATOS_CREATE_VARIABLE(double, DP_K )
    KRATOS_CREATE_VARIABLE(int, CALCULATE_INSITU_STRESS )
    KRATOS_CREATE_VARIABLE( double, INTERNAL_FRICTION_ANGLE )

    //CONTACT_LINK_MASTER is defined in condition.h
    //     KRATOS_CREATE_VARIABLE( Condition::Pointer, CONTACT_LINK_MASTER )
    //CONTACT_LINK_SLAVE is defined in condition.h
    //     KRATOS_CREATE_VARIABLE( Condition::Pointer, CONTACT_LINK_SLAVE )

    KRATOS_CREATE_VARIABLE(int, CONTACT_RAMP )
    KRATOS_CREATE_VARIABLE(Vector, PENALTY )
    KRATOS_CREATE_VARIABLE(Vector, PENALTY_T )
    KRATOS_CREATE_VARIABLE(double, FRICTION_COEFFICIENT )
    KRATOS_CREATE_VARIABLE(double, LAMBDA )
    KRATOS_CREATE_VARIABLE(double, MIU )
    KRATOS_CREATE_VARIABLE(Vector, LAMBDAS )
    KRATOS_CREATE_VARIABLE(Matrix, LAMBDAS_T )
    KRATOS_CREATE_VARIABLE(Vector, GAPS )
    KRATOS_CREATE_VARIABLE(Vector, DELTA_LAMBDAS )
    KRATOS_CREATE_VARIABLE(Matrix, DELTA_LAMBDAS_T )
    KRATOS_CREATE_VARIABLE(int, CONTACT_SLAVE_INTEGRATION_POINT_INDEX )
    KRATOS_CREATE_VARIABLE( Matrix, CONTACT_LINK_M )
    KRATOS_CREATE_VARIABLE( int, CONTACT_DOUBLE_CHECK )
    KRATOS_CREATE_VARIABLE( int, IS_CONTACT_MASTER )
    KRATOS_CREATE_VARIABLE( int, IS_CONTACT_SLAVE )
    KRATOS_CREATE_VARIABLE( Vector, STICK )
    KRATOS_CREATE_VARIABLE( int, FIRST_TIME_STEP )
    KRATOS_CREATE_VARIABLE( int, QUASI_STATIC_ANALYSIS )
    KRATOS_CREATE_VARIABLE( Vector, NORMAL_STRESS )
    KRATOS_CREATE_VARIABLE( Vector, TANGENTIAL_STRESS )
    KRATOS_CREATE_VARIABLE( double, NORMAL_CONTACT_STRESS )
    KRATOS_CREATE_VARIABLE( double, TANGENTIAL_CONTACT_STRESS )
    KRATOS_CREATE_VARIABLE( double, CONTACT_STICK )

    KRATOS_CREATE_VARIABLE( double, WATER_PRESSURE )
    KRATOS_CREATE_VARIABLE( double, REACTION_WATER_PRESSURE )
    KRATOS_CREATE_VARIABLE( double, WATER_PRESSURE_DT )
    KRATOS_CREATE_VARIABLE( double, WATER_PRESSURE_ACCELERATION )
    KRATOS_CREATE_VARIABLE( double, WATER_PRESSURE_NULL )
    KRATOS_CREATE_VARIABLE( double, WATER_PRESSURE_NULL_DT )
    KRATOS_CREATE_VARIABLE( double, WATER_PRESSURE_NULL_ACCELERATION )
    KRATOS_CREATE_VARIABLE( double, WATER_PRESSURE_EINS )
    KRATOS_CREATE_VARIABLE( double, WATER_PRESSURE_EINS_DT )
    KRATOS_CREATE_VARIABLE( double, WATER_PRESSURE_EINS_ACCELERATION )

    KRATOS_CREATE_VARIABLE( double, AIR_PRESSURE )
    KRATOS_CREATE_VARIABLE( double, REACTION_AIR_PRESSURE )
    KRATOS_CREATE_VARIABLE( double, AIR_PRESSURE_DT )
    KRATOS_CREATE_VARIABLE( double, AIR_PRESSURE_ACCELERATION )
    KRATOS_CREATE_VARIABLE( double, AIR_PRESSURE_NULL )
    KRATOS_CREATE_VARIABLE( double, AIR_PRESSURE_NULL_DT )
    KRATOS_CREATE_VARIABLE( double, AIR_PRESSURE_NULL_ACCELERATION )
    KRATOS_CREATE_VARIABLE( double, AIR_PRESSURE_EINS )
    KRATOS_CREATE_VARIABLE( double, AIR_PRESSURE_EINS_DT )
    KRATOS_CREATE_VARIABLE( double, AIR_PRESSURE_EINS_ACCELERATION )
    KRATOS_CREATE_VARIABLE( double, SUCTION )
    KRATOS_CREATE_VARIABLE( double, YIELD_STRESS )
    
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_OLD)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_DT)
    //     KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_NULL)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_NULL_DT)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION_NULL)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_EINS)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_EINS_DT)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION_EINS)
    KRATOS_CREATE_VARIABLE( Matrix, ELASTIC_LEFT_CAUCHY_GREEN_OLD )

    KRATOS_CREATE_VARIABLE(int, ACTIVATION_LEVEL)
    KRATOS_CREATE_VARIABLE(double, FLAG_VARIABLE )
    KRATOS_CREATE_VARIABLE(double, DISTANCE )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( LAGRANGE_DISPLACEMENT )
    KRATOS_CREATE_VARIABLE( double, LAGRANGE_AIR_PRESSURE )
    KRATOS_CREATE_VARIABLE( double, LAGRANGE_WATER_PRESSURE )
    KRATOS_CREATE_VARIABLE( double, LAGRANGE_TEMPERATURE )


    //for ALE application TO BE REMOVED
    KRATOS_CREATE_VARIABLE(double, IS_INTERFACE )

    //for electric_application
    KRATOS_CREATE_VARIABLE(double, ELECTRIC_POTENTIAL )
    KRATOS_CREATE_VARIABLE(double, ELECTRIC_POTENTIAL_DT )
    KRATOS_CREATE_VARIABLE(double, ELECTRIC_POTENTIAL_DT2 )
    KRATOS_CREATE_VARIABLE(double, ELECTRIC_POTENTIAL_NULL )
    KRATOS_CREATE_VARIABLE(double, ELECTRIC_POTENTIAL_EINS )
    KRATOS_CREATE_VARIABLE(double, ELECTRIC_POTENTIAL_NULL_DT )
    KRATOS_CREATE_VARIABLE(double, ELECTRIC_POTENTIAL_EINS_DT )
    KRATOS_CREATE_VARIABLE(double, ELECTRIC_POTENTIAL_NULL_DT2 )
    KRATOS_CREATE_VARIABLE(double, ELECTRIC_POTENTIAL_EINS_DT2 )
    KRATOS_CREATE_VARIABLE(double, LAGRANGE_ELECTRIC_POTENTIAL)
    KRATOS_CREATE_VARIABLE(double, CONCENTRATION )
    KRATOS_CREATE_VARIABLE(double, CONCENTRATION_DT )
    KRATOS_CREATE_VARIABLE(double, CONCENTRATION_DT2 )
    KRATOS_CREATE_VARIABLE(double, CONCENTRATION_NULL )
    KRATOS_CREATE_VARIABLE(double, CONCENTRATION_EINS )
    KRATOS_CREATE_VARIABLE(double, CONCENTRATION_NULL_DT )
    KRATOS_CREATE_VARIABLE(double, CONCENTRATION_EINS_DT )
    KRATOS_CREATE_VARIABLE(double, CONCENTRATION_NULL_DT2 )
    KRATOS_CREATE_VARIABLE(double, CONCENTRATION_EINS_DT2 )
    KRATOS_CREATE_VARIABLE(double, LAGRANGE_CONCENTRATION)



    //for PFEM application TO BE REMOVED
    KRATOS_CREATE_VARIABLE(double,  NODAL_AREA)
    KRATOS_CREATE_VARIABLE(double,  NODAL_H)
    KRATOS_CREATE_VARIABLE(double,  IS_STRUCTURE)
    KRATOS_CREATE_VARIABLE(double,  IS_POROUS)
    KRATOS_CREATE_VARIABLE(double,  IS_WATER)
    KRATOS_CREATE_VARIABLE(double,  IS_FLUID)
    KRATOS_CREATE_VARIABLE(double,  IS_BOUNDARY)
    KRATOS_CREATE_VARIABLE(double,  IS_FREE_SURFACE)
    KRATOS_CREATE_VARIABLE(double,  IS_LAGRANGIAN_INLET)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NORMAL_TO_WALL)
    KRATOS_CREATE_VARIABLE(double,  IS_WATER_ELEMENT)


    KRATOS_CREATE_VARIABLE(double, BULK_MODULUS)
    KRATOS_CREATE_VARIABLE(double, SATURATION )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( GRAVITY )
    KRATOS_CREATE_VARIABLE(double, DENSITY_WATER )
    KRATOS_CREATE_VARIABLE(double, VISCOSITY_WATER)
    KRATOS_CREATE_VARIABLE(double, DENSITY_AIR )
    KRATOS_CREATE_VARIABLE(double, VISCOSITY_AIR)
    KRATOS_CREATE_VARIABLE(double, POROSITY )
    KRATOS_CREATE_VARIABLE(double, DIAMETER )
    KRATOS_CREATE_VARIABLE(double, AIR_ENTRY_VALUE )
    KRATOS_CREATE_VARIABLE(double, FIRST_SATURATION_PARAM )
    KRATOS_CREATE_VARIABLE(double, SECOND_SATURATION_PARAM )
    KRATOS_CREATE_VARIABLE(double, PERMEABILITY_WATER )
    KRATOS_CREATE_VARIABLE(double, PERMEABILITY_AIR )
    KRATOS_CREATE_VARIABLE(double, BULK_AIR )
    KRATOS_CREATE_VARIABLE(int, SCALE )
    KRATOS_CREATE_VARIABLE(double, C_SMAGORINSKY )

    KRATOS_CREATE_VARIABLE(int, IS_JACK_LINK )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( FACE_LOAD )

    KRATOS_CREATE_VARIABLE(double, DIVPROJ)
    KRATOS_CREATE_VARIABLE(double, THAWONE)
    KRATOS_CREATE_VARIABLE(double, THAWTWO)

     KRATOS_CREATE_VARIABLE(double, ARRHENIUS)
    KRATOS_CREATE_VARIABLE(double, ARRHENIUSAUX)
    KRATOS_CREATE_VARIABLE(double, ARRHENIUSAUX_)
    KRATOS_CREATE_VARIABLE(double, PRESSUREAUX)
    KRATOS_CREATE_VARIABLE(double, NODAL_MAUX)
    KRATOS_CREATE_VARIABLE(double, NODAL_PAUX)
    KRATOS_CREATE_VARIABLE(double, FACE_HEAT_FLUX)
    KRATOS_CREATE_VARIABLE(double, HEAT_FLUX)
    KRATOS_CREATE_VARIABLE(double, TC)
    KRATOS_CREATE_VARIABLE(double, IS_BURN )
    KRATOS_CREATE_VARIABLE(double, CONDUCTIVITY)
    KRATOS_CREATE_VARIABLE(double, SPECIFIC_HEAT)
    KRATOS_CREATE_VARIABLE(double, IS_DRIPPING )
    KRATOS_CREATE_VARIABLE(double, IS_PERMANENT)
    KRATOS_CREATE_VARIABLE(double, MATERIAL_VARIABLE)
    KRATOS_CREATE_VARIABLE(double, IS_WALL ) 
    KRATOS_CREATE_VARIABLE(double, FUEL)
    KRATOS_CREATE_VARIABLE(double, YO)
    KRATOS_CREATE_VARIABLE(double, YF)
    KRATOS_CREATE_VARIABLE(double, YI)
    KRATOS_CREATE_VARIABLE(double, M)
    KRATOS_CREATE_VARIABLE(double, Y1)
    KRATOS_CREATE_VARIABLE(double, Y2)
    KRATOS_CREATE_VARIABLE(double, YP)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_1)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_2)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_3)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_4)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_5)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_6)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_7)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_8)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_9)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_10)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_11)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_12)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_13)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_14)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_15)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_16)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_17)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_18)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_19)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_20)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_21)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_22)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_23)
    KRATOS_CREATE_VARIABLE(double, RADIATIVE_INTENSITY_24)
    KRATOS_CREATE_VARIABLE(double, EMISSIVITY)
    KRATOS_CREATE_VARIABLE(double, ENTHALPY)
    KRATOS_CREATE_VARIABLE(double, MIXTURE_FRACTION)
    KRATOS_CREATE_VARIABLE(double, rhoD)
    KRATOS_CREATE_VARIABLE(double,Yox)
    KRATOS_CREATE_VARIABLE(double,Ypr)
    KRATOS_CREATE_VARIABLE(double,Yfuel)
    KRATOS_CREATE_VARIABLE(double,Hfuel)
    KRATOS_CREATE_VARIABLE(double,Hpr)
    KRATOS_CREATE_VARIABLE(double,Hpr1)
    KRATOS_CREATE_VARIABLE(double,Hox)
    KRATOS_CREATE_VARIABLE(double, INCIDENT_RADIATION_FUNCTION)


    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY1) 
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY2)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY3)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY4)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY5)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY6)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY7)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY8)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY9)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY10)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY11)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY12)
 
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY13) 
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY14)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY15)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY16)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY17)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY18)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY19)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY20)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY21)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY22)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY23)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY24)
    KRATOS_CREATE_VARIABLE(double, SWITCH_TEMPERATURE)





    KRATOS_CREATE_VARIABLE(double,ABSORPTION_COEFFICIENT)
    KRATOS_CREATE_VARIABLE(double,STEFAN_BOLTZMANN_CONSTANT)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DIRECTION)
    KRATOS_CREATE_VARIABLE(double, NODAL_SWITCH)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(Y)

    KRATOS_CREATE_VARIABLE(double,SOUND_VELOCITY)    
    KRATOS_CREATE_VARIABLE(double,AIR_SOUND_VELOCITY)
    KRATOS_CREATE_VARIABLE(double,WATER_SOUND_VELOCITY)

    KRATOS_CREATE_VARIABLE(double, IS_VISITED )    
    KRATOS_CREATE_VARIABLE(double, IS_EROSIONABLE)    

    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(FRACT_VEL)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PRESS_PROJ)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(CONV_PROJ)
    KRATOS_CREATE_VARIABLE( double, NODAL_MASS)
    KRATOS_CREATE_VARIABLE( double,  AUX_INDEX)
    KRATOS_CREATE_VARIABLE( double,  EXTERNAL_PRESSURE)
    KRATOS_CREATE_VARIABLE( double, PRESSURE_OLD_IT )
    KRATOS_CREATE_VARIABLE( Vector, BDF_COEFFICIENTS )


    // for mengmeng application
//     KRATOS_CREATE_VARIABLE( double, TEMPERATURE_NULL )
//     KRATOS_CREATE_VARIABLE( double, TEMPERATURE_EINS )
//     KRATOS_CREATE_VARIABLE( double, SURFACE_FLOW_HEAT )
//     KRATOS_CREATE_VARIABLE( double, SURFACE_FLOW_WATER )
//     KRATOS_CREATE_VARIABLE( double, ICE_VOLUME_FRACTION )
//     KRATOS_CREATE_VARIABLE( double, ICE_VOLUME_FRACTION_NULL )
//     KRATOS_CREATE_VARIABLE( double, ICE_VOLUME_FRACTION_EINS )

    //for xfem application
    KRATOS_CREATE_VARIABLE(double, CRACK_OPENING)
    KRATOS_CREATE_VARIABLE(double, CRACK_TRANSLATION)

    //for level set application
    KRATOS_CREATE_VARIABLE(int, REFINEMENT_LEVEL)
    KRATOS_CREATE_VARIABLE(double, IS_DIVIDED)
    KRATOS_CREATE_VARIABLE(bool, IS_DUPLICATED)
    KRATOS_CREATE_VARIABLE(bool, SPLIT_ELEMENT)
    KRATOS_CREATE_VARIABLE(bool, SPLIT_NODAL)
    KRATOS_CREATE_VARIABLE(double, MIN_DT)
    KRATOS_CREATE_VARIABLE(double, MAX_DT)



    KratosApplication::KratosApplication() :
    mCondition3D(0, Element::GeometryType::Pointer(new Triangle3D3<Node<3> >(Element::GeometryType::PointsArrayType(3, Node<3>())))),
    mCondition2D(0, Element::GeometryType::Pointer(new Geometry<Node<3> >(Element::GeometryType::PointsArrayType(2, Node<3>())))),
    mpVariableData(KratosComponents<VariableData>::pGetComponents()),
    mpDoubleVariables(KratosComponents<Variable<double> >::pGetComponents()),
    mpArray1DVariables(KratosComponents<Variable<array_1d<double, 3> > >::pGetComponents()),
    mpVectorVariables(KratosComponents<Variable<Vector> >::pGetComponents()),
    mpMatrixVariables(KratosComponents<Variable<Matrix> >::pGetComponents()),
    mpArray1DVariableComponents(KratosComponents<VariableComponent<VectorComponentAdaptor<array_1d<double, 3> > > >::pGetComponents()),
    mpElements(KratosComponents<Element>::pGetComponents()), 
    mpConditions(KratosComponents<Condition>::pGetComponents())
    {}

    void KratosApplication::RegisterVariables()
    {
	KRATOS_REGISTER_VARIABLE(CONVECTION_DIFFUSION_SETTINGS)
	KRATOS_REGISTER_VARIABLE(RADIATION_SETTINGS)

        KRATOS_REGISTER_VARIABLE(CONSTITUTIVE_LAW)
        
        KRATOS_REGISTER_VARIABLE(IS_INACTIVE)

        KRATOS_REGISTER_VARIABLE(OSS_SWITCH)
        KRATOS_REGISTER_VARIABLE(DYNAMIC_TAU)

        KRATOS_REGISTER_VARIABLE(NEIGHBOURS_INDICES)
        KRATOS_REGISTER_VARIABLE(PARTITION_INDEX)
        KRATOS_REGISTER_VARIABLE(ERASE_FLAG)
        KRATOS_REGISTER_VARIABLE(NL_ITERATION_NUMBER)
        KRATOS_REGISTER_VARIABLE(FRACTIONAL_STEP)

        KRATOS_REGISTER_VARIABLE(TIME)
        KRATOS_REGISTER_VARIABLE(TIME_STEPS)
        KRATOS_REGISTER_VARIABLE(START_TIME)
        KRATOS_REGISTER_VARIABLE(END_TIME)
        KRATOS_REGISTER_VARIABLE(DELTA_TIME)
        KRATOS_REGISTER_VARIABLE(TEMPERATURE)
        KRATOS_REGISTER_VARIABLE(PRESSURE)
        KRATOS_REGISTER_VARIABLE(DENSITY)
        KRATOS_REGISTER_VARIABLE(VISCOSITY)
        KRATOS_REGISTER_VARIABLE(ERROR_RATIO)
        KRATOS_REGISTER_VARIABLE(TAU)

        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_ACCELERATION)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_VELOCITY)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(VELOCITY)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ROTATION)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MOMENTUM)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(REACTION)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(BODY_FORCE)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SEEPAGE_DRAG)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(NORMAL)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(FORCE)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MOMENT)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(FORCE_CM)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MOMENTUM_CM)

        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MASS)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(RHS)

	KRATOS_REGISTER_VARIABLE( RHS_WATER)
	KRATOS_REGISTER_VARIABLE( RHS_AIR)

        KRATOS_REGISTER_VARIABLE(NEIGHBOUR_NODES);
        KRATOS_REGISTER_VARIABLE(NEIGHBOUR_ELEMENTS);
        KRATOS_REGISTER_VARIABLE(NEIGHBOUR_CONDITIONS);

        KRATOS_REGISTER_VARIABLE(FATHER_NODES)

        KRATOS_REGISTER_VARIABLE( INERTIA )

        //for structural application TO BE REMOVED
        KRATOS_REGISTER_VARIABLE(MATERIAL_PARAMETERS)
        KRATOS_REGISTER_VARIABLE(WRINKLING_APPROACH )
        KRATOS_REGISTER_VARIABLE(GREEN_LAGRANGE_STRAIN_TENSOR )

        KRATOS_REGISTER_VARIABLE(PK2_STRESS_TENSOR )
        KRATOS_REGISTER_VARIABLE(CAUCHY_STRESS_TENSOR )
        KRATOS_REGISTER_VARIABLE(AUXILIARY_MATRIX_1 )
        KRATOS_REGISTER_VARIABLE(YOUNG_MODULUS )
        KRATOS_REGISTER_VARIABLE(POISSON_RATIO )
        KRATOS_REGISTER_VARIABLE(THICKNESS )
        KRATOS_REGISTER_VARIABLE(NEGATIVE_FACE_PRESSURE )
        KRATOS_REGISTER_VARIABLE(POSITIVE_FACE_PRESSURE )
        KRATOS_REGISTER_VARIABLE(INITIAL_PENALTY)
        //KRATOS_REGISTER_VARIABLE(CONSTITUTIVE_LAW_NEW)
        KRATOS_REGISTER_VARIABLE(DP_EPSILON)
        KRATOS_REGISTER_VARIABLE(INSITU_STRESS)
        KRATOS_REGISTER_VARIABLE(DP_ALPHA1)
        KRATOS_REGISTER_VARIABLE(DP_K)
        KRATOS_REGISTER_VARIABLE(CALCULATE_INSITU_STRESS)
        KRATOS_REGISTER_VARIABLE( INTERNAL_FRICTION_ANGLE )

        //CONTACT_LINK_MASTER is defined in condition.h
        //     KRATOS_REGISTER_VARIABLE(CONTACT_LINK_MASTER )
        //CONTACT_LINK_SLAVE is defined in condition.h
        KRATOS_REGISTER_VARIABLE(CONTACT_RAMP )
        KRATOS_REGISTER_VARIABLE(PENALTY )
        KRATOS_REGISTER_VARIABLE(PENALTY_T )
        KRATOS_REGISTER_VARIABLE(FRICTION_COEFFICIENT )
        KRATOS_REGISTER_VARIABLE(LAMBDA )
        KRATOS_REGISTER_VARIABLE(MIU )
        KRATOS_REGISTER_VARIABLE(LAMBDAS )
        KRATOS_REGISTER_VARIABLE(LAMBDAS_T )
        KRATOS_REGISTER_VARIABLE(GAPS )
        KRATOS_REGISTER_VARIABLE(DELTA_LAMBDAS )
        KRATOS_REGISTER_VARIABLE(DELTA_LAMBDAS_T )
        KRATOS_REGISTER_VARIABLE(CONTACT_SLAVE_INTEGRATION_POINT_INDEX )
        KRATOS_REGISTER_VARIABLE(CONTACT_LINK_M )
        KRATOS_REGISTER_VARIABLE(CONTACT_DOUBLE_CHECK )
        KRATOS_REGISTER_VARIABLE(IS_CONTACT_MASTER )
        KRATOS_REGISTER_VARIABLE(IS_CONTACT_SLAVE )
        KRATOS_REGISTER_VARIABLE(STICK)
        KRATOS_REGISTER_VARIABLE(FIRST_TIME_STEP)
        KRATOS_REGISTER_VARIABLE(QUASI_STATIC_ANALYSIS )
        KRATOS_REGISTER_VARIABLE( NORMAL_STRESS )
        KRATOS_REGISTER_VARIABLE( TANGENTIAL_STRESS )
        KRATOS_REGISTER_VARIABLE( NORMAL_CONTACT_STRESS )
        KRATOS_REGISTER_VARIABLE( TANGENTIAL_CONTACT_STRESS )
        KRATOS_REGISTER_VARIABLE( CONTACT_STICK )

        KRATOS_REGISTER_VARIABLE(WATER_PRESSURE)
        KRATOS_REGISTER_VARIABLE(REACTION_WATER_PRESSURE)
        KRATOS_REGISTER_VARIABLE(WATER_PRESSURE_DT)
        KRATOS_REGISTER_VARIABLE(WATER_PRESSURE_ACCELERATION)
        KRATOS_REGISTER_VARIABLE(WATER_PRESSURE_NULL)
        KRATOS_REGISTER_VARIABLE(WATER_PRESSURE_NULL_DT)
        KRATOS_REGISTER_VARIABLE(WATER_PRESSURE_NULL_ACCELERATION)
        KRATOS_REGISTER_VARIABLE(WATER_PRESSURE_EINS)
        KRATOS_REGISTER_VARIABLE(WATER_PRESSURE_EINS_DT)
        KRATOS_REGISTER_VARIABLE(WATER_PRESSURE_EINS_ACCELERATION)

        KRATOS_REGISTER_VARIABLE(AIR_PRESSURE)
        KRATOS_REGISTER_VARIABLE(REACTION_AIR_PRESSURE)
        KRATOS_REGISTER_VARIABLE(AIR_PRESSURE_DT)
        KRATOS_REGISTER_VARIABLE(AIR_PRESSURE_ACCELERATION)
        KRATOS_REGISTER_VARIABLE(AIR_PRESSURE_NULL)
        KRATOS_REGISTER_VARIABLE(AIR_PRESSURE_NULL_DT)
        KRATOS_REGISTER_VARIABLE(AIR_PRESSURE_NULL_ACCELERATION)
        KRATOS_REGISTER_VARIABLE(AIR_PRESSURE_EINS)
        KRATOS_REGISTER_VARIABLE(AIR_PRESSURE_EINS_DT)
        KRATOS_REGISTER_VARIABLE(AIR_PRESSURE_EINS_ACCELERATION)
        KRATOS_REGISTER_VARIABLE(SUCTION)
	KRATOS_REGISTER_VARIABLE(YIELD_STRESS)

        KRATOS_REGISTER_VARIABLE(DISPLACEMENT_OLD)
        KRATOS_REGISTER_VARIABLE(DISPLACEMENT_DT)
        //     KRATOS_REGISTER_VARIABLE(ACCELERATION)
        KRATOS_REGISTER_VARIABLE(DISPLACEMENT_NULL)
        KRATOS_REGISTER_VARIABLE(DISPLACEMENT_NULL_DT)
        KRATOS_REGISTER_VARIABLE(ACCELERATION_NULL)
        KRATOS_REGISTER_VARIABLE(DISPLACEMENT_EINS)
        KRATOS_REGISTER_VARIABLE(DISPLACEMENT_EINS_DT)
        KRATOS_REGISTER_VARIABLE(ACCELERATION_EINS)
        KRATOS_REGISTER_VARIABLE(ELASTIC_LEFT_CAUCHY_GREEN_OLD)

        KRATOS_REGISTER_VARIABLE(ACTIVATION_LEVEL)
        KRATOS_REGISTER_VARIABLE(FLAG_VARIABLE )
        KRATOS_REGISTER_VARIABLE(DISTANCE )
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS( LAGRANGE_DISPLACEMENT )
        KRATOS_REGISTER_VARIABLE( LAGRANGE_AIR_PRESSURE )
        KRATOS_REGISTER_VARIABLE( LAGRANGE_WATER_PRESSURE )
        KRATOS_REGISTER_VARIABLE( LAGRANGE_TEMPERATURE )


        //for ALE application TO BE REMOVED
        KRATOS_REGISTER_VARIABLE(IS_INTERFACE )
        KRATOS_REGISTER_VARIABLE(IS_VISITED)
        KRATOS_REGISTER_VARIABLE(IS_EROSIONABLE)


        //for electric_application
        KRATOS_REGISTER_VARIABLE( ELECTRIC_POTENTIAL )
        KRATOS_REGISTER_VARIABLE(ELECTRIC_POTENTIAL_DT )
        KRATOS_REGISTER_VARIABLE(ELECTRIC_POTENTIAL_DT2 )
        KRATOS_REGISTER_VARIABLE(ELECTRIC_POTENTIAL_NULL )
        KRATOS_REGISTER_VARIABLE(ELECTRIC_POTENTIAL_EINS )
        KRATOS_REGISTER_VARIABLE(ELECTRIC_POTENTIAL_NULL_DT )
        KRATOS_REGISTER_VARIABLE(ELECTRIC_POTENTIAL_EINS_DT )
        KRATOS_REGISTER_VARIABLE(ELECTRIC_POTENTIAL_NULL_DT2 )
        KRATOS_REGISTER_VARIABLE(ELECTRIC_POTENTIAL_EINS_DT2 )
        KRATOS_REGISTER_VARIABLE(LAGRANGE_ELECTRIC_POTENTIAL)
        KRATOS_REGISTER_VARIABLE(CONCENTRATION )
        KRATOS_REGISTER_VARIABLE(CONCENTRATION_DT )
        KRATOS_REGISTER_VARIABLE(CONCENTRATION_DT2 )
        KRATOS_REGISTER_VARIABLE(CONCENTRATION_NULL )
        KRATOS_REGISTER_VARIABLE(CONCENTRATION_EINS )
        KRATOS_REGISTER_VARIABLE(CONCENTRATION_NULL_DT )
        KRATOS_REGISTER_VARIABLE(CONCENTRATION_EINS_DT )
        KRATOS_REGISTER_VARIABLE(CONCENTRATION_NULL_DT2 )
        KRATOS_REGISTER_VARIABLE(CONCENTRATION_EINS_DT2 )
        KRATOS_REGISTER_VARIABLE(LAGRANGE_CONCENTRATION)


        //for PFEM application TO BE REMOVED
        KRATOS_REGISTER_VARIABLE( NODAL_AREA)
        KRATOS_REGISTER_VARIABLE( NODAL_H)
        KRATOS_REGISTER_VARIABLE( IS_STRUCTURE)
        KRATOS_REGISTER_VARIABLE( IS_POROUS)
        KRATOS_REGISTER_VARIABLE( IS_WATER)
        KRATOS_REGISTER_VARIABLE( IS_FLUID)
        KRATOS_REGISTER_VARIABLE( IS_BOUNDARY)
        KRATOS_REGISTER_VARIABLE( IS_FREE_SURFACE)
        KRATOS_REGISTER_VARIABLE( IS_LAGRANGIAN_INLET)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(NORMAL_TO_WALL)
        KRATOS_REGISTER_VARIABLE( IS_WATER_ELEMENT)

        KRATOS_REGISTER_VARIABLE( BULK_MODULUS )
        KRATOS_REGISTER_VARIABLE( SATURATION )    
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS( GRAVITY)    
        KRATOS_REGISTER_VARIABLE( DENSITY_WATER )
        KRATOS_REGISTER_VARIABLE(VISCOSITY_WATER)
        KRATOS_REGISTER_VARIABLE( DENSITY_AIR )
        KRATOS_REGISTER_VARIABLE(VISCOSITY_AIR)
        KRATOS_REGISTER_VARIABLE( POROSITY )
        KRATOS_REGISTER_VARIABLE( DIAMETER )
        KRATOS_REGISTER_VARIABLE( AIR_ENTRY_VALUE )
        KRATOS_REGISTER_VARIABLE( FIRST_SATURATION_PARAM )
        KRATOS_REGISTER_VARIABLE( SECOND_SATURATION_PARAM )
        KRATOS_REGISTER_VARIABLE( PERMEABILITY_WATER )
        KRATOS_REGISTER_VARIABLE( PERMEABILITY_AIR )
        KRATOS_REGISTER_VARIABLE( BULK_AIR )
        KRATOS_REGISTER_VARIABLE( SCALE )
        KRATOS_REGISTER_VARIABLE( C_SMAGORINSKY )

        KRATOS_REGISTER_VARIABLE( IS_JACK_LINK )
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS( FACE_LOAD )
        KRATOS_REGISTER_VARIABLE(SOUND_VELOCITY)
        KRATOS_REGISTER_VARIABLE(AIR_SOUND_VELOCITY)
        KRATOS_REGISTER_VARIABLE(WATER_SOUND_VELOCITY)

        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(FRACT_VEL)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PRESS_PROJ)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(CONV_PROJ)
        KRATOS_REGISTER_VARIABLE(NODAL_MASS)
        KRATOS_REGISTER_VARIABLE(AUX_INDEX)
        KRATOS_REGISTER_VARIABLE(EXTERNAL_PRESSURE)
        KRATOS_REGISTER_VARIABLE( PRESSURE_OLD_IT )
        KRATOS_REGISTER_VARIABLE( BDF_COEFFICIENTS )

        KRATOS_REGISTER_VARIABLE( DIVPROJ)
        KRATOS_REGISTER_VARIABLE( THAWONE)
        KRATOS_REGISTER_VARIABLE(THAWTWO)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ADVPROJ)

        //for xfem application
        KRATOS_REGISTER_VARIABLE(CRACK_OPENING)
        KRATOS_REGISTER_VARIABLE(CRACK_TRANSLATION)

        // MINE VM!
        KRATOS_REGISTER_VARIABLE( MU )

	KRATOS_REGISTER_VARIABLE(ARRHENIUS)
	KRATOS_REGISTER_VARIABLE( ARRHENIUSAUX)
	KRATOS_REGISTER_VARIABLE( ARRHENIUSAUX_)
	KRATOS_REGISTER_VARIABLE(PRESSUREAUX)
	KRATOS_REGISTER_VARIABLE(NODAL_MAUX)
	KRATOS_REGISTER_VARIABLE(NODAL_PAUX)
	KRATOS_REGISTER_VARIABLE(FACE_HEAT_FLUX)
	KRATOS_REGISTER_VARIABLE(HEAT_FLUX)
	KRATOS_REGISTER_VARIABLE(TC)
	KRATOS_REGISTER_VARIABLE(IS_BURN )
	KRATOS_REGISTER_VARIABLE(CONDUCTIVITY)
	KRATOS_REGISTER_VARIABLE(SPECIFIC_HEAT)
	KRATOS_REGISTER_VARIABLE(IS_DRIPPING )
	KRATOS_REGISTER_VARIABLE(IS_PERMANENT)
	KRATOS_REGISTER_VARIABLE(MATERIAL_VARIABLE)
	KRATOS_REGISTER_VARIABLE(IS_WALL ) 
	KRATOS_REGISTER_VARIABLE(FUEL)
	KRATOS_REGISTER_VARIABLE(YO)
	KRATOS_REGISTER_VARIABLE(YF)
	KRATOS_REGISTER_VARIABLE(YI)
	KRATOS_REGISTER_VARIABLE(M)
	KRATOS_REGISTER_VARIABLE(M)
        KRATOS_REGISTER_VARIABLE(Y1)
        KRATOS_REGISTER_VARIABLE(Y2)
        KRATOS_REGISTER_VARIABLE(YP)
        KRATOS_REGISTER_VARIABLE(Yfuel)
        KRATOS_REGISTER_VARIABLE(Yox)
        KRATOS_REGISTER_VARIABLE(Ypr)
        KRATOS_REGISTER_VARIABLE(Hfuel)
        KRATOS_REGISTER_VARIABLE(Hpr)
        KRATOS_REGISTER_VARIABLE(Hpr1)
        KRATOS_REGISTER_VARIABLE(Hox)


        KRATOS_REGISTER_VARIABLE(ABSORPTION_COEFFICIENT)
        KRATOS_REGISTER_VARIABLE(STEFAN_BOLTZMANN_CONSTANT)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(DIRECTION)
	KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY)
	KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_1)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_2)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_3)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_4)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_5)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_6)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_7)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_8)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_9)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_10)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_11)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_12)

	KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_13)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_14)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_15)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_16)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_17)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_18)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_19)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_20)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_21)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_22)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_23)
        KRATOS_REGISTER_VARIABLE(RADIATIVE_INTENSITY_24)
	KRATOS_REGISTER_VARIABLE(EMISSIVITY)
	KRATOS_REGISTER_VARIABLE(ENTHALPY)
	KRATOS_REGISTER_VARIABLE(MIXTURE_FRACTION)
	KRATOS_REGISTER_VARIABLE(rhoD)


        KRATOS_REGISTER_VARIABLE(INCIDENT_RADIATION_FUNCTION)


        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY1)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY2)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY3)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY4)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY5)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY6)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY7)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY8)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY9)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY10)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY11)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY12)

 	KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY13)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY14)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY15)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY16)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY17)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY18)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY19)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY20)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY21)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY22)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY23)
        KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY24)


        KRATOS_REGISTER_VARIABLE(SWITCH_TEMPERATURE)

        KRATOS_REGISTER_VARIABLE(NODAL_SWITCH)
	KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(Y)
	// Registering elements and conditions here
	KRATOS_REGISTER_CONDITION("Condition3D", mCondition3D)
	KRATOS_REGISTER_CONDITION("Condition2D", mCondition2D)



    // for mengmeng application
//     KRATOS_REGISTER_VARIABLE(TEMPERATURE_NULL )
//     KRATOS_REGISTER_VARIABLE(TEMPERATURE_EINS )
//     KRATOS_REGISTER_VARIABLE(SURFACE_FLOW_HEAT )
//     KRATOS_REGISTER_VARIABLE(SURFACE_FLOW_WATER )
//     KRATOS_REGISTER_VARIABLE(ICE_VOLUME_FRACTION )
//     KRATOS_REGISTER_VARIABLE(ICE_VOLUME_FRACTION_NULL )
//     KRATOS_REGISTER_VARIABLE(ICE_VOLUME_FRACTION_EINS )

        //for level set application
        KRATOS_REGISTER_VARIABLE(REFINEMENT_LEVEL)
        KRATOS_REGISTER_VARIABLE(IS_DIVIDED )
        KRATOS_REGISTER_VARIABLE(IS_DUPLICATED )
        KRATOS_REGISTER_VARIABLE(SPLIT_NODAL)
        KRATOS_REGISTER_VARIABLE(SPLIT_ELEMENT)
	KRATOS_REGISTER_VARIABLE(MIN_DT)
	KRATOS_REGISTER_VARIABLE(MAX_DT)
    }

    // Specialize array of compenents for VariableData
    KratosComponents<VariableData>::ComponentsContainerType KratosComponents<VariableData>::msComponents;

    // Initializing static members
/*    const Condition  KratosApplication::msCondition3D(0, Element::GeometryType::Pointer(new Triangle3D<Node<3> >(Element::GeometryType::PointsArrayType(3, Node<3>()))));
    const Condition  KratosApplication::msCondition2D(0, Element::GeometryType::Pointer(new Geometry<Node<3> >(Element::GeometryType::PointsArrayType(2, Node<3>()))));
*/

}  // namespace Kratos.




# import Kratos
from KratosMultiphysics import *
from KratosMultiphysics.ExternalSolversApplication import *
from KratosMultiphysics.SolidMechanicsApplication import *
from KratosMultiphysics.StructuralMechanicsApplication import *
from KratosMultiphysics.ContactStructuralMechanicsApplication import *

# Import Kratos "wrapper" for unittests
import KratosMultiphysics.KratosUnittest as KratosUnittest

# Import the tests o test_classes to create the suits
## SMALL TESTS
# Exact integration tests
from TestExactIntegration import TestLineExactIntegration1 as TTestLineExactIntegration1
from TestExactIntegration import TestLineExactIntegration2 as TTestLineExactIntegration2
from TestExactIntegration import TestTriangleExactIntegration1 as TTestTriangleExactIntegration1
from TestExactIntegration import TestTriangleExactIntegration2 as TTestTriangleExactIntegration2
from TestExactIntegration import TestTriangleExactIntegration3 as TTestTriangleExactIntegration3
from TestExactIntegration import TestQuadrilateralExactIntegration1 as TTestQuadrilateralExactIntegration1
from TestExactIntegration import TestQuadrilateralExactIntegration2 as TTestQuadrilateralExactIntegration2

# Mesh tying tests
from SmallTests import SimplePatchTestTwoDMeshTying as TSimplePatchTestTwoDMeshTying
from SmallTests import SimplestPatchTestThreeDMeshTying as TSimplestPatchTestThreeDMeshTying
from SmallTests import SimplePatchTestThreeDMeshTying as TSimplePatchTestThreeDMeshTying

 # Legacy tests 
from SmallTests import SimplePatchTestContact as TSimplePatchTestContact
from SmallTests import SimpleSlopePatchTestContact as TSimpleSlopePatchTestContact
from SmallTests import SimplePatchNotMatchingATestContact as TSimplePatchNotMatchingATestContact
from SmallTests import SimplePatchNotMatchingBTestContact as TSimplePatchNotMatchingBTestContact
from SmallTests import TaylorPatchTestContact as TTaylorPatchTestContact
from SmallTests import TaylorPatchDynamicTestContact as TTaylorPatchDynamicTestContact
from SmallTests import HertzSimpleTestContact as THertzSimpleTestContact
from SmallTests import HertzSimpleSphereTestContact as THertzSimpleSphereTestContact
from SmallTests import HertzSphereTestContact as THertzSphereTestContact
#from SmallTests import HertzCompleteTestContact as THertzCompleteTestContact
from SmallTests import ThreeDSimplestPatchMatchingTestContact as TThreeDSimplestPatchMatchingTestContact
from SmallTests import ThreeDSimplestTrianglePatchMatchingTestContact as TThreeDSimplestTrianglePatchMatchingTestContact
from SmallTests import ThreeDPatchMatchingTestContact as TThreeDPatchMatchingTestContact
from SmallTests import ThreeDPatchNotMatchingTestContact as TThreeDPatchNonMatchingTestContact

# ALM frictionless tests
from SmallTests import ALMHyperSimplePatchTestContact as TALMHyperSimplePatchTestContact
from SmallTests import ALMSimplePatchTestContact as TALMSimplePatchTestContact
from SmallTests import ALMSimpleSlopePatchTestContact as TALMSimpleSlopePatchTestContact
from SmallTests import ALMSimplePatchNotMatchingATestContact as TALMSimplePatchNotMatchingATestContact
from SmallTests import ALMSimplePatchNotMatchingBTestContact as TALMSimplePatchNotMatchingBTestContact
from SmallTests import ALMTaylorPatchTestContact as TALMTaylorPatchTestContact
from SmallTests import ALMTaylorPatchDynamicTestContact as TALMTaylorPatchDynamicTestContact
from SmallTests import ALMHertzSimpleTestContact as TALMHertzSimpleTestContact
from SmallTests import ALMHertzSimpleSphereTestContact as TALMHertzSimpleSphereTestContact
from SmallTests import ALMHertzSphereTestContact as TALMHertzSphereTestContact
#from SmallTests import ALMHertzCompleteTestContact as TALMHertzCompleteTestContact
from SmallTests import ALMThreeDSimplestPatchMatchingTestContact as TALMThreeDSimplestPatchMatchingTestContact
from SmallTests import ALMThreeDPatchMatchingTestContact as TALMTThreeDPatchMatchingTestContact
from SmallTests import ALMThreeDPatchNotMatchingTestContact as TALMThreeDPatchNotMatchingTestContact

## NIGTHLY TESTS
from NightlyTests import IroningTestContact as TIroningTestContact
from NightlyTests import IroningDieTestContact as TIroningDieTestContact
#from NightlyTests import ALMIroningTestContact as TALMIroningTestContact
#from NightlyTests import ALMIroningDieTestContact as TALMIroningDieTestContact

## VALIDATION TESTS

def AssambleTestSuites():
    ''' Populates the test suites to run.

    Populates the test suites to run. At least, it should pupulate the suites:
    "small", "nighlty" and "all"

    Return
    ------

    suites: A dictionary of suites
        The set of suites with its test_cases added.
    '''
    suites = KratosUnittest.KratosSuites

    # Create a test suit with the selected tests (Small tests):
    smallSuite = suites['small']
    # Exact integration tests
    smallSuite.addTest(TTestLineExactIntegration1('test_execution'))
    smallSuite.addTest(TTestLineExactIntegration2('test_execution'))
    smallSuite.addTest(TTestTriangleExactIntegration1('test_execution'))
    smallSuite.addTest(TTestTriangleExactIntegration2('test_execution'))
    smallSuite.addTest(TTestTriangleExactIntegration3('test_execution'))
    smallSuite.addTest(TTestQuadrilateralExactIntegration1('test_execution'))
    smallSuite.addTest(TTestQuadrilateralExactIntegration2('test_execution'))
    
    # Mesh tying tests 
    smallSuite.addTest(TSimplePatchTestTwoDMeshTying('test_execution'))
    smallSuite.addTest(TSimplestPatchTestThreeDMeshTying('test_execution'))
    smallSuite.addTest(TSimplePatchTestThreeDMeshTying('test_execution'))
    
    # Legacy tests 
    smallSuite.addTest(TSimplePatchTestContact('test_execution'))
    smallSuite.addTest(TSimpleSlopePatchTestContact('test_execution'))
    smallSuite.addTest(TSimplePatchNotMatchingATestContact('test_execution'))
    smallSuite.addTest(TSimplePatchNotMatchingBTestContact('test_execution'))
    smallSuite.addTest(TTaylorPatchTestContact('test_execution'))
    smallSuite.addTest(TTaylorPatchDynamicTestContact('test_execution'))
    smallSuite.addTest(THertzSimpleSphereTestContact('test_execution'))
    smallSuite.addTest(THertzSphereTestContact('test_execution'))
    smallSuite.addTest(THertzSimpleTestContact('test_execution'))
    #smallSuite.addTest(THertzCompleteTestContact('test_execution'))
    smallSuite.addTest(TThreeDSimplestPatchMatchingTestContact('test_execution'))
    smallSuite.addTest(TThreeDSimplestTrianglePatchMatchingTestContact('test_execution'))
    smallSuite.addTest(TThreeDPatchMatchingTestContact('test_execution'))
    smallSuite.addTest(TThreeDPatchNonMatchingTestContact('test_execution'))
    
    # ALM frictionless tests
    smallSuite.addTest(TALMHyperSimplePatchTestContact('test_execution'))
    smallSuite.addTest(TALMSimplePatchTestContact('test_execution'))
    smallSuite.addTest(TALMSimpleSlopePatchTestContact('test_execution'))
    smallSuite.addTest(TALMSimplePatchNotMatchingATestContact('test_execution'))
    smallSuite.addTest(TALMSimplePatchNotMatchingBTestContact('test_execution'))
    smallSuite.addTest(TALMTaylorPatchTestContact('test_execution'))
    smallSuite.addTest(TALMTaylorPatchDynamicTestContact('test_execution'))
    #smallSuite.addTest(TALMHertzSimpleSphereTestContact('test_execution'))
    #smallSuite.addTest(TALMHertzSphereTestContact('test_execution'))
    #smallSuite.addTest(TALMHertzSimpleTestContact('test_execution'))
    ##smallSuite.addTest(THertzCompleteTestContact('test_execution'))
    smallSuite.addTest(TALMThreeDSimplestPatchMatchingTestContact('test_execution'))
    smallSuite.addTest(TALMTThreeDPatchMatchingTestContact('test_execution'))
    smallSuite.addTest(TALMThreeDPatchNotMatchingTestContact('test_execution'))

    # Create a test suit with the selected tests plus all small tests
    nightSuite = suites['nightly']
    nightSuite.addTests(smallSuite)
    #nightSuite.addTest(TIroningTestContact('test_execution'))
    #nightSuite.addTest(TIroningDieTestContact('test_execution'))
    
    # For very long tests that should not be in nighly and you can use to validate 
    validationSuite = suites['validation']

    # Create a test suit that contains all the tests:
    allSuite = suites['all']
    allSuite.addTests(
        KratosUnittest.TestLoader().loadTestsFromTestCases([
            TTestLineExactIntegration1,
            TTestLineExactIntegration2,
            TTestTriangleExactIntegration1,
            TTestTriangleExactIntegration2,
            TTestTriangleExactIntegration3,
            TTestQuadrilateralExactIntegration1,
            TTestQuadrilateralExactIntegration2,
            TSimplePatchTestTwoDMeshTying,
            TSimplestPatchTestThreeDMeshTying,
            ##TSimplePatchTestThreeDMeshTying, # FIXME: Some error in the mesh tying condition
            ##TSimplePatchTestContact,
            ##TSimpleSlopePatchTestContact,
            ##TSimplePatchNotMatchingATestContact,
            ##TSimplePatchNotMatchingBTestContact,
            ##TTaylorPatchTestContact,
            ##TTaylorPatchDynamicTestContact,
            THertzSimpleTestContact,
            THertzSimpleSphereTestContact,
            THertzSphereTestContact,
            ##TThreeDSimplestPatchMatchingTestContact,
            ##TThreeDSimplestTrianglePatchMatchingTestContact,
            ##TThreeDPatchMatchingTestContact,
            ##TThreeDPatchNonMatchingTestContact,
            TALMHyperSimplePatchTestContact,
            TALMSimplePatchTestContact,
            TALMSimpleSlopePatchTestContact,
            TALMSimplePatchNotMatchingATestContact,
            TALMSimplePatchNotMatchingBTestContact,
            TALMTaylorPatchTestContact,
            TALMTaylorPatchDynamicTestContact,
            ##TALMHertzSimpleTestContact,
            ##TALMHertzSimpleSphereTestContact,
            ##TALMHertzSphereTestContact,
            TALMThreeDSimplestPatchMatchingTestContact,
            TALMTThreeDPatchMatchingTestContact, # FIXME: Some error, rotation in the solution
            TALMThreeDPatchNotMatchingTestContact,
            ############# JUST TESTING ###########
            ##THertzCompleteTestContact,
            ##TIroningTestContact,
            ##TIroningDieTestContact,
            ##TALMHertzCompleteTestContact,
            ##TALMIroningTestContact,
            ##TALMIroningDieTestContact,
        ])
    )

    return suites

if __name__ == '__main__':
    KratosUnittest.runTests(AssambleTestSuites())

import KratosMultiphysics as km
import KratosMultiphysics.KratosUnittest as UnitTest
import KratosMultiphysics.kratos_utilities as kratos_utilities
from KratosMultiphysics.RANSApplication.test_utilities import RunParametericTestCase


class FlowSolverTestCase(UnitTest.TestCase):
    @classmethod
    def setUpCase(cls, working_folder, parameters_file_name, print_output):
        cls.working_folder = working_folder
        cls.parameters_file_name = parameters_file_name
        cls.print_output = print_output
        cls.parameters = {}

    def testSteady(self):
        self.parameters["<TIME_SCHEME_TYPE>"] = "steady"

        self._runTest()

    def testTransient(self):
        self.parameters["<TIME_SCHEME_TYPE>"] = "transient"

        self._runTest()

    def _runTest(self):
        if (km.IsDistributedRun()):
            self.parameters["<PARALLEL_TYPE>"] = "MPI"
        else:
            self.parameters["<PARALLEL_TYPE>"] = "OpenMP"
        RunParametericTestCase(self.parameters_file_name, self.working_folder,
                               self.parameters, self.print_output)


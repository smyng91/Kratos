from __future__ import print_function, absolute_import, division

# import kratos
import KratosMultiphysics as Kratos

# import RANS
import KratosMultiphysics.RANSApplication as KratosRANS

# import formulation interface
from KratosMultiphysics.RANSApplication.formulations.rans_formulation import RansFormulation

# import utilities
from KratosMultiphysics.RANSApplication import ScalarVariableDifferenceNormCalculationUtility
from KratosMultiphysics.RANSApplication.formulations.utilities import GetConvergenceInfo

class TwoEquationTurbulenceModelRansFormulation(RansFormulation):
    def __init__(self, model_part, settings):
        super().__init__(model_part, settings)

        self.echo_level = settings["echo_level"].GetInt()
        self.nu_t_convergence_utility = ScalarVariableDifferenceNormCalculationUtility(self.GetBaseModelPart(), Kratos.TURBULENT_VISCOSITY)
        self.SetMaxCouplingIterations(settings["coupling_settings"]["max_iterations"].GetInt())

    def GetMinimumBufferSize(self):
        if (self.is_steady_simulation):
            return 1
        else:
            return 2

    def SetTimeSchemeSettings(self, settings):
        if (settings.Has("scheme_type")):
            scheme_type = settings["scheme_type"].GetString()
            if (scheme_type == "steady"):
                self.is_steady_simulation = True
                self.GetBaseModelPart().ProcessInfo.SetValue(Kratos.BOSSAK_ALPHA, 0.0)
            elif (scheme_type == "bdf2" or scheme_type == "bossak"):
                self.is_steady_simulation = False
                default_settings = Kratos.Parameters('''{
                    "scheme_type": "transient",
                    "alpha_bossak": -0.3
                }''')
                settings.ValidateAndAssignDefaults(default_settings)
                self.GetBaseModelPart().ProcessInfo.SetValue(Kratos.BOSSAK_ALPHA, settings["alpha_bossak"].GetDouble())
            else:
                raise Exception("Only \"steady\", \"bdf2\" and \"bossak\" scheme types supported. [ scheme_type = \"" + scheme_type  + "\" ]")
        else:
            raise Exception("\"scheme_type\" is missing in time scheme settings")

        super().SetTimeSchemeSettings(settings)

    def SolveCouplingStep(self):
        settings = self.GetParameters()
        relative_tolerance = settings["coupling_settings"]["relative_tolerance"].GetDouble()
        absolute_tolerance = settings["coupling_settings"]["absolute_tolerance"].GetDouble()
        max_iterations = self.GetMaxCouplingIterations()

        for itration in range(max_iterations):
            self.nu_t_convergence_utility.InitializeCalculation()

            for formulation in self.GetRansFormulationsList():
                if (not formulation.SolveCouplingStep()):
                    return False
            self.ExecuteAfterCouplingSolveStep()

            relative_error, absolute_error = self.nu_t_convergence_utility.CalculateDifferenceNorm()
            info = GetConvergenceInfo(Kratos.TURBULENT_VISCOSITY, relative_error, relative_tolerance, absolute_error, absolute_tolerance)
            Kratos.Logger.PrintInfo(self.GetName() + " CONVERGENCE", info)

            is_converged = relative_error < relative_tolerance or absolute_error < absolute_tolerance
            if (is_converged):
                Kratos.Logger.PrintInfo(self.GetName() + " CONVERGENCE", "TURBULENT_VISCOSITY *** CONVERGENCE ACHIEVED ***")

            Kratos.Logger.PrintInfo(self.GetName(), "Solved coupling itr. " + str(itration + 1) + "/" + str(max_iterations) + ".")
            if (is_converged):
                return True

        return True
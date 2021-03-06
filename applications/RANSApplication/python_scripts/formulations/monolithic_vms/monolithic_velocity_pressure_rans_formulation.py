# Importing the Kratos Library
import KratosMultiphysics as Kratos

# Import applications
import KratosMultiphysics.FluidDynamicsApplication as KratosCFD
import KratosMultiphysics.RANSApplication as KratosRANS

# import formulation interface
from KratosMultiphysics.RANSApplication.formulations.rans_formulation import RansFormulation

# import utilities
from KratosMultiphysics.RANSApplication import RansCalculationUtilities
from KratosMultiphysics.RANSApplication.formulations.utilities import CreateBlockBuilderAndSolver
from KratosMultiphysics.RANSApplication.formulations.utilities import CreateRansFormulationModelPart
from KratosMultiphysics.RANSApplication.formulations.utilities import CalculateNormalsOnConditions
from KratosMultiphysics.RANSApplication.formulations.utilities import InitializePeriodicConditions
from KratosMultiphysics.RANSApplication.formulations.utilities import GetKratosObjectType

class MonolithicVelocityPressureRansFormulation(RansFormulation):
    def __init__(self, model_part, settings):
        """Incompressible Variational-Multi-Scale Navier Stokes formulation

        This RansFormulation solves VELOCITY, and PRESSURE with Variational-Multi-Scale (VMS) formulated
        incompressible Navier-Stokes equation.

        This supports both steady and transient problems, transient with bossak time integration scheme.
        It uses wall functions at walls, therefore SLIP flag must be True on all walls (in both conditions and nodes)

        Args:
            model_part (Kratos.ModelPart): ModelPart to be used in the formulation.
            settings (Kratos.Parameters): Settings to be used in the formulation.
        """
        super().__init__(model_part, settings)

        ##settings string in json format
        default_settings = Kratos.Parameters("""
        {
            "formulation_name": "monolithic",
            "maximum_iterations": 10,
            "echo_level": 0,
            "compute_reactions": false,
            "reform_dofs_at_each_step": false,
            "relative_velocity_tolerance": 1e-5,
            "absolute_velocity_tolerance": 1e-7,
            "relative_pressure_tolerance": 1e-5,
            "absolute_pressure_tolerance": 1e-7,
            "linear_solver_settings": {
                "solver_type": "amgcl"
            },
            "move_mesh_strategy": 0,
            "move_mesh_flag": false,
            "velocity_relaxation":0.9,
            "pressure_relaxation":0.9,
            "oss_switch": 0,
            "dynamic_tau": 0.01
        }""")

        settings.ValidateAndAssignDefaults(default_settings)

        self.min_buffer_size = 2
        self.echo_level = settings["echo_level"].GetInt()

        self.SetMaxCouplingIterations(1)

        Kratos.Logger.PrintInfo(self.GetName(), "Construction of formulation finished.")

    def AddVariables(self):
        base_model_part = self.GetBaseModelPart()
        base_model_part.AddNodalSolutionStepVariable(Kratos.VELOCITY)
        base_model_part.AddNodalSolutionStepVariable(Kratos.ACCELERATION)
        base_model_part.AddNodalSolutionStepVariable(Kratos.MESH_VELOCITY)
        base_model_part.AddNodalSolutionStepVariable(Kratos.PRESSURE)
        base_model_part.AddNodalSolutionStepVariable(Kratos.IS_STRUCTURE)
        base_model_part.AddNodalSolutionStepVariable(Kratos.DISPLACEMENT)
        base_model_part.AddNodalSolutionStepVariable(Kratos.VISCOSITY)
        base_model_part.AddNodalSolutionStepVariable(Kratos.DENSITY)
        base_model_part.AddNodalSolutionStepVariable(Kratos.BODY_FORCE)
        base_model_part.AddNodalSolutionStepVariable(Kratos.NODAL_AREA)
        base_model_part.AddNodalSolutionStepVariable(Kratos.NODAL_H)
        base_model_part.AddNodalSolutionStepVariable(Kratos.ADVPROJ)
        base_model_part.AddNodalSolutionStepVariable(Kratos.DIVPROJ)
        base_model_part.AddNodalSolutionStepVariable(Kratos.REACTION)
        base_model_part.AddNodalSolutionStepVariable(Kratos.REACTION_WATER_PRESSURE)
        base_model_part.AddNodalSolutionStepVariable(Kratos.EXTERNAL_PRESSURE)
        base_model_part.AddNodalSolutionStepVariable(Kratos.NORMAL)
        base_model_part.AddNodalSolutionStepVariable(Kratos.Y_WALL)
        base_model_part.AddNodalSolutionStepVariable(KratosCFD.Q_VALUE)
        base_model_part.AddNodalSolutionStepVariable(Kratos.KINEMATIC_VISCOSITY)
        base_model_part.AddNodalSolutionStepVariable(KratosRANS.TURBULENT_KINETIC_ENERGY)

        Kratos.Logger.PrintInfo(self.GetName(), "Added solution step variables.")

    def AddDofs(self):
        base_model_part = self.GetBaseModelPart()
        Kratos.VariableUtils().AddDof(Kratos.VELOCITY_X, Kratos.REACTION_X, base_model_part)
        Kratos.VariableUtils().AddDof(Kratos.VELOCITY_Y, Kratos.REACTION_Y, base_model_part)
        Kratos.VariableUtils().AddDof(Kratos.VELOCITY_Z, Kratos.REACTION_Z, base_model_part)
        Kratos.VariableUtils().AddDof(Kratos.PRESSURE, Kratos.REACTION_WATER_PRESSURE, base_model_part)

        Kratos.Logger.PrintInfo(self.GetName(), "Added dofs.")

    def PrepareModelPart(self):
        self.monolithic_model_part = CreateRansFormulationModelPart(
            self,
            "VMS",
            self.condition_name)
        Kratos.Logger.PrintInfo(self.GetName(), "Created formulation model part.")

    def Initialize(self):
        model_part = self.GetBaseModelPart()
        CalculateNormalsOnConditions(model_part)

        process_info = model_part.ProcessInfo
        bossak_alpha = process_info[Kratos.BOSSAK_ALPHA]

        settings = self.GetParameters()

        if (self.IsPeriodic()):
            if (self.domain_size == 2):
                periodic_variables_list = [Kratos.VELOCITY_X, Kratos.VELOCITY_Y, Kratos.PRESSURE]
            else:
                periodic_variables_list = [Kratos.VELOCITY_X, Kratos.VELOCITY_Y, Kratos.VELOCITY_Z, Kratos.PRESSURE]
            InitializePeriodicConditions(model_part,
                                         self.monolithic_model_part,
                                         periodic_variables_list)

        conv_criteria = GetKratosObjectType("MixedGenericCriteria")(
            [(Kratos.VELOCITY, settings["relative_velocity_tolerance"].GetDouble(), settings["absolute_velocity_tolerance"].GetDouble()),
             (Kratos.PRESSURE, settings["relative_pressure_tolerance"].GetDouble(), settings["absolute_pressure_tolerance"].GetDouble())])

        if self.is_steady_simulation:
            scheme = GetKratosObjectType("ResidualBasedSimpleSteadyScheme")(
                settings["velocity_relaxation"].GetDouble(),
                settings["pressure_relaxation"].GetDouble(),
                self.domain_size)
        else:
            scheme = GetKratosObjectType("ResidualBasedPredictorCorrectorVelocityBossakSchemeTurbulent")(
                bossak_alpha,
                settings["move_mesh_strategy"].GetInt(),
                self.domain_size)

        linear_solver = GetKratosObjectType("LinearSolverFactory")(
            settings["linear_solver_settings"])

        builder_and_solver = CreateBlockBuilderAndSolver(
            linear_solver,
            self.IsPeriodic(),
            self.GetCommunicator())

        self.solver = GetKratosObjectType("ResidualBasedNewtonRaphsonStrategy")(
            self.monolithic_model_part,
            scheme,
            conv_criteria,
            builder_and_solver,
            settings["maximum_iterations"].GetInt(),
            settings["compute_reactions"].GetBool(),
            settings["reform_dofs_at_each_step"].GetBool(),
            settings["move_mesh_flag"].GetBool())

        self.solver.SetEchoLevel(self.echo_level)
        conv_criteria.SetEchoLevel(self.echo_level)

        process_info.SetValue(Kratos.DYNAMIC_TAU, settings["dynamic_tau"].GetDouble())
        process_info.SetValue(Kratos.OSS_SWITCH, settings["oss_switch"].GetInt())

        super().Initialize()
        self.solver.Initialize()

        Kratos.Logger.PrintInfo(self.GetName(), "Solver initialization finished.")

    def GetMinimumBufferSize(self):
        return self.min_buffer_size

    def Finalize(self):
        self.solver.Clear()
        super().Finalize()

    def SolveCouplingStep(self):
        if (self.IsBufferInitialized()):
            max_iterations = self.GetMaxCouplingIterations()
            for iteration in range(max_iterations):
                self.solver.Predict()
                _ = self.solver.SolveSolutionStep()
                self.ExecuteAfterCouplingSolveStep()
                Kratos.Logger.PrintInfo(self.GetName(), "Solved coupling iteration " + str(iteration + 1) + "/" + str(max_iterations) + ".")
                return True

        return False

    def InitializeSolutionStep(self):
        if (self.IsBufferInitialized()):
            super().InitializeSolutionStep()
            self.solver.InitializeSolutionStep()

    def FinalizeSolutionStep(self):
        if (self.IsBufferInitialized()):
            self.solver.FinalizeSolutionStep()
            super().FinalizeSolutionStep()

    def Check(self):
        super().Check()
        self.solver.Check()

    def Clear(self):
        self.solver.Clear()
        super().Clear()

    def SetTimeSchemeSettings(self, settings):
        if (settings.Has("scheme_type")):
            scheme_type = settings["scheme_type"].GetString()
            if (scheme_type == "steady"):
                self.is_steady_simulation = True
            elif (scheme_type == "bossak"):
                self.is_steady_simulation = False
                default_settings = Kratos.Parameters('''{
                    "scheme_type": "bossak",
                    "alpha_bossak": -0.3
                }''')
                settings.ValidateAndAssignDefaults(default_settings)
                self.GetBaseModelPart().ProcessInfo.SetValue(Kratos.BOSSAK_ALPHA, settings["alpha_bossak"].GetDouble())
            else:
                raise Exception("Only \"steady\" and \"bossak\" scheme types supported. [ scheme_type = \"" + scheme_type  + "\" ]")
        else:
            raise Exception("\"scheme_type\" is missing in time scheme settings")

    def SetConstants(self, settings):
        defaults = Kratos.Parameters('''{
            "von_karman": 0.41,
            "beta"      : 5.2,
            "c_mu"      : 0.09
        }''')
        settings.ValidateAndAssignDefaults(defaults)

        # set constants
        von_karman = settings["von_karman"].GetDouble()
        beta = settings["beta"].GetDouble()
        y_plus_limit = RansCalculationUtilities.CalculateLogarithmicYPlusLimit(
            von_karman, beta)

        process_info = self.GetBaseModelPart().ProcessInfo
        process_info.SetValue(KratosRANS.WALL_VON_KARMAN, von_karman)
        process_info.SetValue(KratosRANS.WALL_SMOOTHNESS_BETA, beta)
        process_info.SetValue(KratosRANS.RANS_LINEAR_LOG_LAW_Y_PLUS_LIMIT, y_plus_limit)
        process_info.SetValue(KratosRANS.TURBULENCE_RANS_C_MU, settings["c_mu"].GetDouble())

    def SetWallFunctionSettings(self, settings):
        wall_function_region_type = "logarithmic_region_only"
        if (settings.Has("wall_function_region_type")):
            wall_function_region_type = settings["wall_function_region_type"].GetString()

        if (wall_function_region_type == "logarithmic_region_only"):
            self.condition_name = "RansVMSMonolithicKBasedWall"
        else:
            msg = "Unsupported wall function region type provided. [ wall_function_region_type = \"" + wall_function_region_type + "\" ]."
            msg += "Supported wall function region types are:\n"
            msg += "\tlogarithmic_region_only\n"
            raise Exception(msg)

    def GetStrategy(self):
        return self.solver

    def GetModelPart(self):
        return self.monolithic_model_part

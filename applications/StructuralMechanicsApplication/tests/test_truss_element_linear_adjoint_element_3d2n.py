from __future__ import print_function, absolute_import, division
import os
os.environ['OMP_NUM_THREADS'] = "1"
import KratosMultiphysics

import KratosMultiphysics.StructuralMechanicsApplication as StructuralMechanicsApplication
import KratosMultiphysics.KratosUnittest as KratosUnittest
import math

def get_displacement_vector(mp,disp):
    index=0
    for node in mp.Nodes:
        disp[index] = node.GetSolutionStepValue(KratosMultiphysics.DISPLACEMENT_X,0)
        index = index + 1
        disp[index] = node.GetSolutionStepValue(KratosMultiphysics.DISPLACEMENT_Y,0)
        index = index + 1
        disp[index] = node.GetSolutionStepValue(KratosMultiphysics.DISPLACEMENT_Z,0)
        index = index + 1
 
def add_variables(mp):
    mp.AddNodalSolutionStepVariable(KratosMultiphysics.DISPLACEMENT)
    mp.AddNodalSolutionStepVariable(KratosMultiphysics.REACTION)
    mp.AddNodalSolutionStepVariable(KratosMultiphysics.VOLUME_ACCELERATION)
    mp.AddNodalSolutionStepVariable(KratosMultiphysics.VELOCITY)
    mp.AddNodalSolutionStepVariable(KratosMultiphysics.ACCELERATION)

def apply_material_properties(mp,dim):
    #define properties
    mp.GetProperties()[1].SetValue(KratosMultiphysics.YOUNG_MODULUS,1000)
    mp.GetProperties()[1].SetValue(KratosMultiphysics.DENSITY,7850)
    mp.GetProperties()[1].SetValue(StructuralMechanicsApplication.CROSS_AREA,10)
    mp.GetProperties()[1].SetValue(StructuralMechanicsApplication.TRUSS_PRESTRESS_PK2,0)
    g = [0,0,0]
    mp.GetProperties()[1].SetValue(KratosMultiphysics.VOLUME_ACCELERATION,g)
    cl = StructuralMechanicsApplication.TrussConstitutiveLaw()
    mp.GetProperties()[1].SetValue(KratosMultiphysics.CONSTITUTIVE_LAW,cl)

def zero_vector(size):
    v = KratosMultiphysics.Vector(size)
    for i in range(size):
        v[i] = 0.0
    return v

class TestTrussLinearAdjointElement(KratosUnittest.TestCase):

    def setUp(self):
        # create test model part
        dim=3
        self.model_part = KratosMultiphysics.ModelPart("test")
        self.model_part.ProcessInfo.SetValue(KratosMultiphysics.DOMAIN_SIZE,dim)
        add_variables(self.model_part)
        self.model_part.CreateNewNode(1, 0.0, 0.0, 0.0)
        self.model_part.CreateNewNode(2, 1.0, 0.1, 0.3)
        apply_material_properties(self.model_part,dim)
        prop = self.model_part.GetProperties()[1]

        self.model_part.CreateNewElement("TrussLinearElement3D2N", 1, [1, 2], prop)
        StructuralMechanicsApplication.ReplaceElementsAndConditionsForAdjointProblemProcess(
            self.model_part).Execute()
        self.adjoint_truss_element = self.model_part.GetElement(1)

        self.model_part.CreateNewElement("TrussLinearElement3D2N", 2, [1, 2], prop)
        self.truss_element = self.model_part.GetElement(2)
      
        self._assign_solution_step_data(0)

        self.truss_element.Initialize()
        self.adjoint_truss_element.Initialize()

    def _create_shape_perturbed_elements(self,mp,delta):
        dim=3
        self.model_part_1 = KratosMultiphysics.ModelPart("Shape_Perturbed_Elements")
        add_variables(self.model_part_1)

        x1 = mp.Nodes[1].X
        y1 = mp.Nodes[1].Y
        z1 = mp.Nodes[1].Z
        x2 = mp.Nodes[2].X
        y2 = mp.Nodes[2].Y
        z2 = mp.Nodes[2].Z
        self.model_part_1.CreateNewNode(1, x1, y1, z1)
        self.model_part_1.CreateNewNode(2, x1+delta, y1, z1)
        self.model_part_1.CreateNewNode(3, x1, y1+delta, z1)
        self.model_part_1.CreateNewNode(4, x1, y1, z1+delta)
        self.model_part_1.CreateNewNode(5, x2, y2, z2)
        self.model_part_1.CreateNewNode(6, x2+delta, y2, z2)
        self.model_part_1.CreateNewNode(7, x2, y2+delta, z2)
        self.model_part_1.CreateNewNode(8, x2, y2, z2+delta)

        apply_material_properties(self.model_part_1,dim)
        prop = self.model_part_1.GetProperties()[1]

        self.model_part_1.CreateNewElement("TrussLinearElement3D2N", 1, [2, 5], prop)
        self.model_part_1.CreateNewElement("TrussLinearElement3D2N", 2, [3, 5], prop)
        self.model_part_1.CreateNewElement("TrussLinearElement3D2N", 3, [4, 5], prop)
        self.model_part_1.CreateNewElement("TrussLinearElement3D2N", 4, [1, 6], prop)
        self.model_part_1.CreateNewElement("TrussLinearElement3D2N", 5, [1, 7], prop)
        self.model_part_1.CreateNewElement("TrussLinearElement3D2N", 6, [1, 8], prop)

        for element in self.model_part_1.Elements:
            element.Initialize()

    def _create_property_perturbed_elements(self,mp,delta):
        dim = 3
        self.model_part_2 = KratosMultiphysics.ModelPart("Property_Perturbed_Elements")
        self.model_part_2.ProcessInfo.SetValue(KratosMultiphysics.DOMAIN_SIZE,dim)
        add_variables(self.model_part_2)
        self.model_part_2.CreateNewNode(1, mp.Nodes[1].X, mp.Nodes[1].Y, mp.Nodes[1].Z)
        self.model_part_2.CreateNewNode(2, mp.Nodes[2].X, mp.Nodes[2].Y, mp.Nodes[2].Z)
        apply_material_properties(self.model_part_2,dim)

        A_initial = mp.GetProperties()[1][StructuralMechanicsApplication.CROSS_AREA]
        self.model_part_2.GetProperties()[1].SetValue(StructuralMechanicsApplication.CROSS_AREA, A_initial + delta)
        prop = self.model_part_2.GetProperties()[1]

        self.model_part_2.CreateNewElement("TrussLinearElement3D2N", 1, [1, 2], prop)
        self.property_perturbed_truss_element = self.model_part_2.GetElement(1)
        self.property_perturbed_truss_element.Initialize()

    def _assign_solution_step_data(self, step=0):
        # generate nodal solution step test data
        self.model_part.Nodes[1].SetSolutionStepValue(KratosMultiphysics.DISPLACEMENT_X,step,0.014725)
        self.model_part.Nodes[1].SetSolutionStepValue(KratosMultiphysics.DISPLACEMENT_Y,step,0.001200)
        self.model_part.Nodes[1].SetSolutionStepValue(KratosMultiphysics.DISPLACEMENT_Z,step,0.0725715)
        self.model_part.Nodes[2].SetSolutionStepValue(KratosMultiphysics.DISPLACEMENT_X,step,0.019735)
        self.model_part.Nodes[2].SetSolutionStepValue(KratosMultiphysics.DISPLACEMENT_Y,step,0.002400)
        self.model_part.Nodes[2].SetSolutionStepValue(KratosMultiphysics.DISPLACEMENT_Z,step,0.377976)

    def _shape_perturbation_correction_factor(self):
        dx = self.model_part.Nodes[1].X - self.model_part.Nodes[2].X
        dy = self.model_part.Nodes[1].Y - self.model_part.Nodes[2].Y
        dz = self.model_part.Nodes[1].Z - self.model_part.Nodes[2].Z
        l = math.sqrt(dx*dx + dy*dy + dz*dz)
        return l

    def _assert_matrix_almost_equal(self, matrix1, matrix2, prec=7):
        self.assertEqual(matrix1.Size1(), matrix2.Size1())
        self.assertEqual(matrix1.Size2(), matrix2.Size2())
        for i in range(matrix1.Size1()):
            for j in range(matrix1.Size2()):
                self.assertAlmostEqual(matrix1[i,j], matrix2[i,j], prec)

    def test_CalculateSensitivityMatrix_Shape(self):
        # unperturbed residual
        LHSUnperturbed = KratosMultiphysics.Matrix(6,6)
        RHSUnperturbed = KratosMultiphysics.Matrix(6,6)
        dummy_RHS = zero_vector(6)
        PrimalDisplacement = zero_vector(6)
        get_displacement_vector(self.model_part,PrimalDisplacement)

        self.truss_element.CalculateLocalSystem(LHSUnperturbed,dummy_RHS,self.model_part.ProcessInfo)
        RHSUnperturbed = LHSUnperturbed * PrimalDisplacement

        # pseudo-load by finite difference approximation
        h = 0.00001
        corr_factor = self._shape_perturbation_correction_factor()
        alpha = corr_factor * h

        FDPseudoLoadMatrix = KratosMultiphysics.Matrix(6,6)
        LHSPerturbed = KratosMultiphysics.Matrix(6,6)
        RHSPerturbed = KratosMultiphysics.Matrix(6,6)

        self._create_shape_perturbed_elements(self.model_part,alpha)

        row_index = 0
        for element in self.model_part_1.Elements:
            element.CalculateLocalSystem(LHSPerturbed,dummy_RHS,self.model_part_1.ProcessInfo)
            RHSPerturbed = LHSPerturbed * PrimalDisplacement
            for j in range(6):
                FDPseudoLoadMatrix[row_index,j] = -(RHSPerturbed[j] - RHSUnperturbed[j]) / alpha
            row_index = row_index + 1

        # pseudo-load computation by adjoint element
        PseudoLoadMatrix = KratosMultiphysics.Matrix(6,6)
        self.adjoint_truss_element.SetValue(StructuralMechanicsApplication.PERTURBATION_SIZE, h)
        self.adjoint_truss_element.CalculateSensitivityMatrix(StructuralMechanicsApplication.SHAPE,PseudoLoadMatrix,self.model_part.ProcessInfo)
        self._assert_matrix_almost_equal(FDPseudoLoadMatrix, PseudoLoadMatrix, 5)

    def test_CalculateSensitivityMatrix_Property(self):
        # unperturbed residual
        LHSUnperturbed = KratosMultiphysics.Matrix(6,6)
        RHSUnperturbed = zero_vector(6)
        dummy_RHS = zero_vector(6)
        PrimalDisplacement = zero_vector(6)
        get_displacement_vector(self.model_part,PrimalDisplacement)
   
        self.truss_element.CalculateLocalSystem(LHSUnperturbed, dummy_RHS, self.model_part.ProcessInfo)
     
        RHSUnperturbed = LHSUnperturbed * PrimalDisplacement

        # pseudo-load by finite difference approximation
        h = 0.00001
        FDPseudoLoadMatrix = KratosMultiphysics.Matrix(1,6)
        LHSPerturbed = KratosMultiphysics.Matrix(6,6)
        RHSPerturbed = zero_vector(6)

        inital_property_value = self.model_part.GetProperties()[1][StructuralMechanicsApplication.CROSS_AREA]
        delta = h * inital_property_value
        self._create_property_perturbed_elements(self.model_part,delta)
        
        self.property_perturbed_truss_element.CalculateLocalSystem(LHSPerturbed, dummy_RHS, self.model_part_2.ProcessInfo)

        RHSPerturbed = LHSPerturbed * PrimalDisplacement

        for j in range(6):
            FDPseudoLoadMatrix[0,j] = -(RHSPerturbed[j] - RHSUnperturbed[j]) / delta

        # pseudo-load computation by adjoint element
        PseudoLoadMatrix = KratosMultiphysics.Matrix(1,6)
        self.adjoint_truss_element.SetValue(StructuralMechanicsApplication.PERTURBATION_SIZE, h)
        self.adjoint_truss_element.CalculateSensitivityMatrix(StructuralMechanicsApplication.CROSS_AREA, PseudoLoadMatrix, self.model_part.ProcessInfo)
        self._assert_matrix_almost_equal(FDPseudoLoadMatrix, PseudoLoadMatrix, 5)

if __name__ == '__main__':
    KratosUnittest.main()

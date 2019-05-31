import KratosMultiphysics
import KratosMultiphysics.CompressiblePotentialFlowApplication as CPFApp
import KratosMultiphysics.MeshingApplication as MeshingApplication
from KratosMultiphysics.CompressiblePotentialFlowApplication.define_wake_process_2d import DefineWakeProcess2D
import math

def Factory(settings, Model):
    if( not isinstance(settings,KratosMultiphysics.Parameters) ):
        raise Exception("expected input shall be a Parameters object, encapsulating a json string")

    return DefineEmbeddedWakeProcess(Model, settings["Parameters"])

class DefineEmbeddedWakeProcess(DefineWakeProcess2D):

    def _SaveTrailingEdgeNode(self):
        # This function finds and saves the trailing edge for further computations
        import time
        ini_time=time.time()
        self.__FindMaximumInactiveNode()
        KratosMultiphysics.Logger.PrintInfo('EmbeddedWake','Find Max. Inactive Node time: ',time.time()-ini_time)
        ini_time=time.time()
        self.__CreateInactiveTrailingEdgeElementBall()
        KratosMultiphysics.Logger.PrintInfo('EmbeddedWake','Creating Inactive Element Ball time: ',time.time()-ini_time)

        self.trailing_edge_node.SetValue(CPFApp.TRAILING_EDGE, True)
    def __FindMaximumInactiveNode(self):
        self.boundary_sub_model_part=self.fluid_model_part.CreateSubModelPart("boundary")
        max_x=-1e10
        boundary_element_id_list=[]
        for element in self.fluid_model_part.Elements:
            if element.Is(KratosMultiphysics.TO_SPLIT):
                boundary_element_id_list.append(element.Id)
            if element.IsNot(KratosMultiphysics.ACTIVE):
                for node in element.GetNodes():
                    if node.X>max_x:
                        max_x=node.X
                        self.max_inactive_node=node
        self.boundary_sub_model_part.AddElements(boundary_element_id_list)

    def __CreateInactiveTrailingEdgeElementBall(self):
        deactivated_model_part=self.fluid_model_part.CreateSubModelPart("deactivated")
        max_x_node=-1e10
        for element in self.boundary_sub_model_part.Elements:
            for elnode in element.GetNodes():
                if elnode.Id == self.max_inactive_node.Id:
                    self.__DeactivateActive(element)
                    deactivated_model_part.Elements.append(element)
        max_x_center=-1e10
        for element in deactivated_model_part.Elements:
            n_center = 0
            center_X=element.GetGeometry().Center().X
            if center_X>max_x_center:
                max_x_center=center_X
                max_elem=element

        for elnode in max_elem.GetNodes():
            if elnode.X>max_x_node:
                max_x_node=elnode.X
                max_node=elnode

        self.trailing_edge_node = max_node

        for element in self.boundary_sub_model_part.Elements:
            center_X=element.GetGeometry().Center().X
            if center_X>self.max_inactive_node.X:
                self.__DeactivateBoundary(element)

    def __DeactivateBoundary(self,elem):
        elem.Set(KratosMultiphysics.TO_SPLIT,False)
    def __DeactivateActive(self,elem):
        elem.Set(KratosMultiphysics.ACTIVE,False)
        elem.Set(KratosMultiphysics.TO_SPLIT,False)


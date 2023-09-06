class BaseComponent:
    def __init__(self, component_id:str):
        self.component_id = component_id

    def getComponentId(self)->str:
        return self.component_id


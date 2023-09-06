import os

class BaseFile:
    def __init__(self, out_file_path:str=None):
        self.out_file_path = out_file_path
        self.file_name = None
        if out_file_path:
            self.file_name = os.path.basename(out_file_path)
        self.components = []

        # Callback for when a component is added
        self.add_component_callback = None

    def __str__(self):
        raise NotImplementedError('BaseFile.__str__ not implemented')

    def getFileName(self):
        return self.file_name

    def addComponents(self, *components):
        for component in components:
            self.components.append(component)
            if self.add_component_callback:
                self.add_component_callback(component)

    def write(self):
        with open(self.out_file_path, 'w') as f:
            f.write(str(self))

    def _getComponents(self):
        return self.components
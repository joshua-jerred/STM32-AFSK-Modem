from ..base_file import BaseFile
from ..output_buffer import OutputBuffer, OutputBufferConfig

# Components
from ...components.enum import Enum as Enum_Component

# Component Generators
from .gen_enum import PythonEnumGenerator


class PythonFile(BaseFile):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.out_config = OutputBufferConfig(
            indentation_str='    ',
            single_line_comment_char='#',
        )
        self.auto_imports = []
        self.user_imports = []
        self.add_component_callback = self.__componentAddedCallback

    def __addAutoImport(self, import_str):
        if import_str not in self.auto_imports:
            self.auto_imports.append(import_str)

    def __componentAddedCallback(self, component):
        if component.getComponentId() == 'enum' and component.isTypedEnum():
            self.__addAutoImport('from enum import IntEnum')
        elif component.getComponentId() == 'enum':
            self.__addAutoImport('from enum import Enum')

    def __addComponentToBuffer(self, buffer:OutputBuffer, component):
        if isinstance(component, Enum_Component):
            PythonEnumGenerator(buffer, component)

    def __str__(self):
        out = OutputBuffer(self.out_config)
        out.addHeaderLines(self.getFileName())
        out.addBlankLine()

        out.addLines(self.auto_imports)
        if len(self.auto_imports) > 0 and len(self.user_imports) > 0:
            out.addBlankLine()
        out.addLines(self.user_imports)
        if len(self.auto_imports) > 0 or len(self.user_imports) > 0:
            out.addBlankLine()

        components = self._getComponents()
        for component in components:
            self.__addComponentToBuffer(out, component)
            out.addBlankLine()

        out.addFooterLines()
        return str(out)
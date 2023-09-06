from ..base_file import BaseFile
from ..output_buffer import OutputBuffer, OutputBufferConfig

# Components
from ...components.enum import Enum as Enum_Component

# Component Generators
from .gen_enum import CppEnumGenerator


class CppFile(BaseFile):
    def __init__(self, include_guard:str=None, **kwargs):
        super().__init__(**kwargs)
        self.out_config = OutputBufferConfig(
            indentation_str='  ',
            single_line_comment_char='//',
        )
        self.include_guard = include_guard
        self.auto_includes = []
        self.user_includes = []
        self.add_component_callback = self.__componentAddedCallback

    def __addAutoInclude(self, include_str):
        if include_str not in self.auto_includes:
            self.auto_includes.append(include_str)

    def __componentAddedCallback(self, component):
        pass

    def __addComponentToBuffer(self, buffer:OutputBuffer, component):
        if isinstance(component, Enum_Component):
            CppEnumGenerator(buffer, component)

    def addIncludes(self, *includes):
        for include in includes:
            if include not in self.user_includes:
                self.user_includes.append(f'#include {include}')

    def __str__(self):
        out = OutputBuffer(self.out_config)
        out.addHeaderLines(self.getFileName())
        out.addBlankLine()

        if self.include_guard:
            out.addLine(f'#ifndef {self.include_guard}')
            out.addLine(f'#define {self.include_guard}')
            out.addBlankLine()

        out.addLines(self.auto_includes)
        if len(self.auto_includes) > 0 and len(self.user_includes) > 0:
            out.addBlankLine()
        out.addLines(self.user_includes)
        if len(self.auto_includes) > 0 or len(self.user_includes) > 0:
            out.addBlankLine()

        components = self._getComponents()
        for component in components:
            self.__addComponentToBuffer(out, component)
            out.addBlankLine()

        if self.include_guard:
            out.addLine(f'#endif // {self.include_guard}')
            out.addBlankLine()

        out.addFooterLines()
        return str(out)
from ..output_buffer import OutputBuffer
from ...components.enum import Enum

def CppEnumGenerator(output_buffer:OutputBuffer, enum:Enum):
    init_line = "enum"
    if enum.isEnumClass():
        init_line += " class"
    init_line += f' {enum.getName()}'
    if enum.isTypedEnum():
        init_line += f' : {enum.getEnumType()}'
    output_buffer.addLine(f'{init_line} {{')

    output_buffer.increaseIndentation()
    tokens = enum.getTokens()
    i = 0
    for (token_name, token_value, token_comment) in tokens:
        out_line = f'{token_name}'
        if token_value:
            out_line += f' = {token_value}'
        if i < len(tokens) - 1:
            out_line += ','
        i += 1
        output_buffer.addLine(out_line)
        if token_comment:
            output_buffer.addCommentOnLastLine(token_comment)
    output_buffer.decreaseIndentation()
    output_buffer.addLine('};')
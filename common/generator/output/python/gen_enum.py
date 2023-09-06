from ..output_buffer import OutputBuffer
from ...components.enum import Enum

def PythonEnumGenerator(output_buffer:OutputBuffer, enum:Enum):
    sub_class = "Enum"
    if enum.isTypedEnum():
        sub_class = "IntEnum"
    output_buffer.addLine(f'class {enum.getName()}({sub_class}):')

    output_buffer.increaseIndentation()
    tokens = enum.getTokens()
    for (token_name, token_value, token_comment) in tokens:
        if token_value:
            output_buffer.addLine(f'{token_name} = {token_value}')
        else:
            output_buffer.addLine(f'{token_name}')
            
        if token_comment:
            output_buffer.addCommentOnLastLine(token_comment)
    output_buffer.decreaseIndentation()
from .base_component import BaseComponent

class Enum(BaseComponent):
    def __init__(self, name:str, enum_class=True, enum_type=None):
        super().__init__("enum")
        self.name = name
        self.enum_class = enum_class
        self.enum_type = enum_type
        self.tokens = []

    def addToken(self, token_name:str = None, token_value:str = None, token_comment:str = None):
        self.tokens.append((token_name, token_value, token_comment))

    def getName(self)->str:
        return self.name

    def isEnumClass(self)->bool:
        return self.enum_class

    def isTypedEnum(self)->bool:
        return self.enum_type is not None

    def getEnumType(self):
        return self.enum_type

    def getTokens(self)->list:
        return self.tokens
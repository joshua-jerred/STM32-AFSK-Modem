from .file_header import getHeaderLines, getFooterLines

class OutputBufferConfig:
    def __init__(self, **kwargs):
        self.indentation_str = kwargs.get('indentation_str', '    ')
        self.single_line_comment_char = kwargs.get('single_line_comment_char', '//')
        self.line_ending = kwargs.get('line_ending', '\n')

class OutputBuffer:
    def __init__(self, config:OutputBufferConfig):
        self.conf:OutputBufferConfig = config
        self.output_lines = []
        self.indentation = 0

    def increaseIndentation(self):
        self.indentation += 1

    def decreaseIndentation(self):
        self.indentation -= 1
        if self.indentation < 0:
            raise Exception('Indentation cannot be less than 0')

    def addLine(self, line:str):
        self.output_lines.append(f'{self.conf.indentation_str * self.indentation}{line}')

    def addLines(self, lines:list):
        for line in lines:
            self.addLine(line)

    def addBlankLine(self):
        self.output_lines.append('')

    def addCommentOnNextLine(self, comment:str):
        self.addLine(self.conf.single_line_comment_char + ' ' + comment)

    def addCommentOnLastLine(self, comment:str):
        if len(self.output_lines) == 0:
            raise Exception('Cannot add comment to last line if there are no lines')
        self.output_lines[-1] += ' ' + self.conf.single_line_comment_char + ' ' + comment

    def addHeaderLines(self, file_name=None):
        self.addLines(getHeaderLines(self.conf.single_line_comment_char, file_name))

    def addFooterLines(self):
        self.addLines(getFooterLines(self.conf.single_line_comment_char))

    def getOutputLines(self):
        return self.output_lines

    def __str__(self):
        return self.conf.line_ending.join(self.output_lines)
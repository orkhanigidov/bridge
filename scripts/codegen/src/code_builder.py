class CodeBuilder:
    def __init__(self):
        self.lines = []
        self.indent_level = 0

    def line(self, text=""):
        if text:
            self.lines.append("    " * self.indent_level + text)
        else:
            self.lines.append("")

        return self

    def indent(self):
        self.indent_level += 1
        return self

    def dedent(self):
        self.indent_level = max(0, self.indent_level - 1)
        return self

    def block(self, start, end="}"):
        self.line(start)
        self.indent()
        return BlockContext(self, end)

    def build(self):
        return "\n".join(self.lines)


class BlockContext:
    def __init__(self, builder, end):
        self.builder = builder
        self.end = end

    def __enter__(self):
        return self.builder

    def __exit__(self, exc_type, exc_value, exc_traceback):
        self.builder.dedent().line(self.end)

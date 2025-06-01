from typing import List


class CodeBuilder:
    def __init__(self, indent_size=4) -> None:
        self._lines: List[str] = []
        self._indent_level: int = 0
        self._indent_size: int = indent_size

    def line(self, text: str = "") -> "CodeBuilder":
        if text:
            indent = " " * (self._indent_level * self._indent_size)
            self._lines.append(indent + text)
        else:
            self._lines.append("")
        return self

    def indent(self) -> "CodeBuilder":
        self._indent_level += 1
        return self

    def dedent(self) -> "CodeBuilder":
        self._indent_level = max(0, self._indent_level - 1)
        return self

    def block(self, start: str, end: str = "}") -> "BlockContext":
        self.line(start)
        self.indent()
        return BlockContext(self, end)

    def build(self) -> str:
        return "\n".join(self._lines)


class BlockContext:
    def __init__(self, builder: CodeBuilder, end: str) -> None:
        self._builder = builder
        self._end = end

    def __enter__(self) -> CodeBuilder:
        return self._builder

    def __exit__(self, exc_type, exc_value, exc_traceback) -> None:
        self._builder.dedent().line(self._end)

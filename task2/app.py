from asyncio import subprocess
from http import HTTPStatus
import io
import os
import re
import tempfile
from quart import Quart
import quart

NAME = "ntc-task2"
TEMP_PREFIX = NAME + "_"
TEMP_SUFFIX = "-temp"


def main():
    app = Quart(NAME)

    regex_text = ""

    with open("./regex-text.txt") as f:
        regex_text = f.readline()

    regex = re.compile(regex_text)

    @app.route("/")
    async def handle_root() -> quart.Response | list[dict[str, str]] | None:
        tempf = tempfile.NamedTemporaryFile(
            dir=".", delete=False, prefix=TEMP_PREFIX, suffix=TEMP_SUFFIX
        )
        len = quart.request.content_length

        if len is None:
            return quart.Response(status=HTTPStatus.UNPROCESSABLE_ENTITY)

        stream = await quart.request.stream
        for byte_line in stream:
            tempf.write(byte_line)

        proc = await subprocess.create_subprocess_shell("./grep-for-coords.sh ")

        outpipe = proc.stdout

        if outpipe is None:
            return

        result: list[dict[str, str]] = []

        str_result = str(outpipe.read())
        for match in regex.finditer(str_result):
            dictionary: dict[str, str] = {}

            print(match)

            dictionary["coord"] = str_result[match.span()[0] : match.span()[1]]

            found = max(
                str_result.find(".", match.span()[1]),
                str_result.find("?", match.span()[1]),
                str_result.find("!", match.span()[1]),
            )

            # if not found, found = -1; then found + 1 = 0
            dictionary["sentence"] = str_result[found + 1].strip()[:200]

            result.append(dictionary)

        return result

    app.run("127.0.0.1", 5153)


if __name__ == "__main__":
    main()

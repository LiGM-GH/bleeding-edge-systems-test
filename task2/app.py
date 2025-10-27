from asyncio import subprocess
from http import HTTPStatus
import os
import re
import tempfile
from quart import Quart
import quart

NAME = "ntc-task2"
TEMP_PREFIX = "userfiles/" + NAME + "_"
TEMP_SUFFIX = "-temp"


def main():
    app = Quart(NAME)

    regex_obj = re.compile(
        r"(-?[0-9]+\.[0-9]+\s+-?[0-9]+\.[0-9]+|[NS][0-9]+\.[0-9]+\s+[WE][0-9]+\.[0-9]+|[0-9]+-[0-9]+[NS]\s+[0-9]+-[0-9]+[WE]|[0-9]+[NS]\s+[0-9]+[WE]|[0-9]+[NS]\s+[0-9]+[WE]|[NS][0-9]+\s+[WE][0-9]+|[NS][0-9]+\s+[WE][0-9]+|[NS][0-9]+\s+[WE][0-9]+|[0-9][0-9]°[0-9][0-9]\.[0-9][0-9]'[NS]\s+[0-9][0-9]°[0-9][0-9]\.[0-9][0-9]'[WE]|[0-9][0-9]°[0-9][0-9]'[NS]\s+[0-9][0-9]°[0-9][0-9]'[WE]|[0-9][0-9]°[0-9][0-9]\.[0-9][0-9]'[NS]\s+[0-9][0-9]°[0-9][0-9]\.[0-9][0-9]'[WE]|[0-9][0-9]°[NS]\s+[0-9][0-9]°[WE]|[0-9][0-9]°[0-9][0-9]'[0-9]+\.[0-9]+''[NS]\s+[0-9][0-9]°[0-9][0-9]'[0-9]+\.[0-9]+''[WE]|[0-9][0-9]°[0-9][0-9]'[0-9]+''[NS]\s+[0-9][0-9]°[0-9][0-9]'[0-9]+''[WE]|[0-9][0-9]°[0-9][0-9]'[NS]\s+[0-9][0-9]°[0-9][0-9]'[WE]|[0-9][0-9]°[0-9][0-9](\.[0-9]+)?'\s*(с\.ш|ю\.ш|С|Ю)\.?\s+[0-9][0-9]°[0-9][0-9](\.[0-9]+)?'\s*(в\.д|з\.д|В|З)|[0-9][0-9]°[0-9][0-9]'[0-9][0-9]\.[0-9]+('')?\s*(с\.ш|ю\.ш|С|Ю)\.?\.?\s+[0-9][0-9]°[0-9][0-9]'[0-9][0-9]\.[0-9]+('')?\s*(в\.д|з\.д|В|З)|[0-9]+\.[0-9]+[NS]\s+[0-9]+\.[0-9]+[WE]|[0-9]+-[0-9][0-9]\.[0-9][0-9][NS]\s+[0-9]+-[0-9][0-9]\.[0-9][0-9][WE]|[0-9]+,[0-9]+°,\s+[0-9]+,[0-9]+°|N[0-9]+\.[0-9]+°,\s+E[0-9]+\.[0-9]+°)"
    )

    @app.route("/")
    async def handle_root() -> quart.Response | list[dict[str, str]] | None:
        tempf = tempfile.NamedTemporaryFile(
            dir=".", delete=False, prefix=TEMP_PREFIX, suffix=TEMP_SUFFIX
        )

        try:
            len = quart.request.content_length

            if len is None:
                return quart.Response(status=HTTPStatus.UNPROCESSABLE_ENTITY)

            async for byte_line in quart.request.body:
                tempf.write(byte_line)
            tempf.close()

            print(f"{tempf.name = }")
            proc = await subprocess.create_subprocess_shell(
                f"grep -Pso {tempf.name} -e $(cat ./regex-text.txt)",
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
            )

            # proc_result = await proc.communicate()
            # print(f"{proc_result = }")
            # print(f"{proc.returncode = }")

            outpipe, outerr = await proc.communicate()

            # if outpipe is None:
            #     print("COULDN'T GET OUTPUT")
            #     return quart.Response(status=HTTPStatus.UNPROCESSABLE_ENTITY)

            result: list[dict[str, str]] = []

            print(f"{outpipe = }")
            print(f"{outerr = }")

            str_result = outpipe.decode().strip()

            finds = regex_obj.finditer(str_result)
            i = 0

            for match in finds:
                i += 1
                dictionary: dict[str, str] = {}

                print(match)

                dictionary["coord"] = str_result[match.span()[0] : match.span()[1]]

                dictionary["sentence"] = str_result[:200]

                result.append(dictionary)

            if i == 0:
                print("NO FINDS")

        finally:
            os.remove(tempf.name)

        return result

    app.run("127.0.0.1", 5153)


if __name__ == "__main__":
    main()

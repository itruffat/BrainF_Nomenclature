"""
This code is *absolutely terrible*, but it was crafted quickly to have an available tool to help with the development of
other programs and give an example of how the format works.
"""


import sys


def str_deserialize(_string):
    answer = {}
    parts = _string.split("~")

    for part in parts:
        subparts = part.split("_")
        header = subparts[0]
        subparts = subparts[1:]
        if len(header) > 2:
            assert (len(subparts) == 0)
            answer["_over"] = header
            continue

        elif header == "v":
            answer["version"] = int(subparts[0])
            continue

        elif header == "cv":
            answer["cell_value"] = {}
            for field in subparts:
                if field == 'uv' or field[-1] == "b":
                    answer["cell-value"]["word-size"] = field
                    continue
                if field in ('po', 'mr', 'sd'):
                    answer["cell-value"]["negative"] = field
                    continue
                if field in ('oe', 'oi', 'or'):
                    answer["cell-value"]["overflow"] = field
                    continue
                if field in ('ue', 'ui', 'ur'):
                    answer["cell-value"]["underflow"] = field
                    continue
                raise Exception
            continue

        elif header == "pt":
            answer["pointer"] = {}
            for field in subparts:
                if field in ('30k', 'uc') or field[-1] == "c":
                    answer["pointer"]["max-cell"] = field
                    continue
                if field in ('ae', 'ai', 'ar'):
                    answer["cell-value"]["after-scope"] = field
                    continue
                if field in ('be', 'bi', 'br'):
                    answer["cell-value"]["before-scope"] = field
                    continue
                raise Exception
            continue

        elif header == "io":
            answer["io"] = {}
            for field in subparts:
                if field in ('dr', 'wi'):
                    answer["io"]["read"] = field
                    continue
                if field in ('ff', 'sw'):
                    answer["io"]["write"] = field
                    continue
                raise Exception
            continue
        raise Exception

    return answer


def is_valid(to_check):
    try:
        str_deserialize(to_check)
    except:
        return False
    else:
        return True


str_des, str_ser = 'strd', 'strs'
json_des, json_ser = 'jsond', 'jsons'


def parse(_mode, _data):
    if _mode == str_des:
        string = _data
        if not is_valid(string):
            print("BAD STRING")
            exit(2)
        return str_deserialize(string)
    elif _mode == str_ser:
        pass


if __name__ == "__main__":
    mode = str_des if len(sys.argv) < 2 or sys.argv[2] not in [str_des,str_ser, json_des, json_ser] else sys.argv[2]
    data = input() if len(sys.argv) < 3 else sys.argv[3]
    if mode == json_des:
        file = data
        with open(file,"r") as file_h:
            data = file_h.read()
    print(parse(mode, data))







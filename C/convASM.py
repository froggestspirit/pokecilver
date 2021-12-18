#!/usr/bin/env python3
import argparse
import time


currentFunc = ""
funcList = []
funcRet = False

def parse_line(string):
    if not string:
        return "", ""
    if string[0] != "\t":
        return parse_line_label(string)
    comment = string.strip("\t")
    if comment:
        comment = f"  // {comment}"
    string = string.split(";")
    asm = string[0][1:].replace("$", "0x").replace("%", "0b")
    if asm:
        return f"\t{parse_asm(asm)}", comment
    return asm, comment


def parse_line_label(string):
    global currentFunc
    global funcList
    global funcRet
    comment = ""
    asm = ""
    string = string.split(";")
    if string[0].find("::") != -1:
        prevFunc = currentFunc
        currentFunc = string[0].split("::")[0]
        funcList.append(currentFunc)
        if prevFunc != "":
            if not funcRet:
                asm = f"\treturn a{currentFunc};\n"
            asm = f"{asm}}}\n\n"
        asm = f"{asm}int {currentFunc}(struct gb_s *gb){{"
    elif len(string[0]) and string[0][0] == ".":
        asm = f"\n_{string[0][1:].replace(':', '').strip()}:"
    else:
        asm = f"{asm}{string[0]}"
    funcRet = False
    if len(string) > 1:
        comment = f"  // {string[1]}"
    return asm, comment


def parse_asm(asm):
    global funcRet
    register = {"a": "_A",
                "b": "_B",
                "c": "_C",
                "d": "_D",
                "e": "_E",
                "f": "_F",
                "h": "_H",
                "l": "_L",
                "af": "_AF",
                "bc": "_BC",
                "de": "_DE",
                "hl": "_HL",
                "sp": "_SP",
                "[bc]": "_bc",
                "[de]": "_de",
                "[hl]": "_hl",
                "[hli]": "_hli",
                "[hld]": "_hld",
                "[sp]": "_sp"}
    condition = {"c": "IF_C",
                "nc": "IF_NC",
                "z": "IF_Z",
                "nz": "IF_NZ"}
    opcode = asm.split(" ", 1)
    asm = "?"
    if len(opcode) > 1:
        asm = opcode[1].split(",")
    opcode = opcode[0].lower()
    asm = list(i.strip(" ") for i in asm)
    if opcode in ("ld", "ldh"):
        op = "LD"
        dest = "_addr"
        source = ""
        if asm[0] in register and asm[1] in register:  # Register into register
            return f"{op}{register[asm[0]]}{register[asm[1]]};"
        elif asm[0] in register:  # Non-register into register
            return f"{op}{register[asm[0]]}({asm[1].strip('[]')});"
        elif asm[1] in register:  # register into address
            return f"{op}{dest}{register[asm[1]]}({asm[0].strip('[]')});"
        else:  # unknown
            return f"huh? {opcode} {asm}"
    elif opcode in ("set", "res", "bit"):
        op = opcode.upper()
        if asm[1] in register:
            return f"{op}{register[asm[1]]}({asm[0].strip('[]')});"
        else:  # unknown
            return f"huh? {opcode} {asm}"
    elif opcode in ("scf", "ccf", "cpl", "daa", "rrca", "rlca", "rra", "rla"):
        return f"{opcode.upper()};"
    elif opcode in ("ei", "di"):
        return ""
    elif opcode in ("ret"):
        cond = ""
        if asm[0] in condition:
            cond = f"{condition[asm[0]]} "
        funcRet = True
        return f"{cond}return -1;"
    elif opcode in ("jp", "jr"):
        cond = ""
        if asm[0] in condition:
            cond = f"{condition[asm[0]]} "
            asm = asm[1:]
        if asm[0][0] == ".":
            return f"{cond}goto _{asm[0][1:]};"
        funcRet = True
        return f"{cond}return a{asm[0]};"
    elif opcode in ("call", "rst"):
        cond = ""
        if asm[0] in condition:
            cond = f"{condition[asm[0]]} "
            asm = asm[1:]
        return f"{cond}CALL(a{asm[0]});"
    elif opcode in ("callfar", "farcall", "homecall", "predef"):
        return f"{opcode.upper()}(a{asm[0]});"
    elif opcode in ("add","adc","sub","sbc","and","or","xor","cp"):
        op = opcode.upper()
        if asm[0] in register:  # Register
            return f"{op}_A{register[asm[0]]};"
        else:
            return f"{op}_A({asm[0].strip('[]')});"
    elif opcode in ("push", "pop", "inc", "dec", "srl", "sla", "sra", "rl", "rlc", "rr", "rrc", "swap"):
        op = opcode.upper()
        if asm[0] in register:
            return f"{op}{register[asm[0]]};"
        else:  # unknown
            return f"huh? {opcode} {asm}"
    else:
        print(f"{opcode} {asm}")
    return asm


def main():
    global currentFunc
    global funcList
    parser = argparse.ArgumentParser()
    parser.add_argument("fileName")
    args = parser.parse_args()

    with open(args.fileName, "r") as inFile:
        asmFile = inFile.read().split("\n")
    asm = []
    comment = []
    for line in asmFile:
        lineASM, lineComment = parse_line(line)
        comment.append(lineComment)
        asm.append(lineASM)
    if currentFunc != "":
        comment.append("")
        asm.append("}")

    for ln, line in enumerate(asm):
        print(f"{line}{comment[ln]}".strip(" "))
    
    for f in funcList:
        print(f"func[a{f}] = {f};")
    return 0


start = time.time()
if not main():
    print(f"Done in: {time.time() - start} seconds")

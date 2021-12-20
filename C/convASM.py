#!/usr/bin/env python3
import argparse
import time


funcMapFile = "funcmap.h"

currentFunc = ""
funcList = []
funcsKnown = []
funcsKnownAddr = []
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
    global funcsKnown
    global funcsKnownAddr
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
        localLabel = string[0][1:].replace(':', '').strip()
        localLabelID = funcsKnown.index(f"a{currentFunc}_{localLabel}")
        asm = f"\n_{localLabel}:\n\tSET_PC({funcsKnownAddr[localLabelID]});"
    else:
        asm = f"{asm}{string[0]}"
    funcRet = False
    if len(string) > 1:
        comment = f"  // {string[1]}"
    return asm, comment


def parse_asm(asm):
    global funcRet
    global currentFunc
    global funcsKnown
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
    condition = {"c": "_C",
                "nc": "_NC",
                "z": "_Z",
                "nz": "_NZ"}
    opcode = asm.split(" ", 1)
    asm = "?"
    if len(opcode) > 1:
        asm = opcode[1].split(",")
    opcode = opcode[0].lower()
    asm = list(i.strip(" ") for i in asm)
    for i, curASM in enumerate(asm):
        func = None
        parts = curASM.split("(")
        for p, part in enumerate(parts):
            if part in ("BANK", "LOW", "HIGH"):
                if f"a{parts[p + 1].split(')')[0]}" in funcsKnown:
                    func = parts[p + 1].split(')')[0]
        if func:
            asm[i] = curASM.replace(func, f"a{func}")

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
    elif opcode in ("ret", "reti"):
        cond = ""
        if asm[0] in condition:
            cond = f"{condition[asm[0]]} "
        funcRet = True
        return f"RET{cond};"
    elif opcode in ("jr"):
        cond = ""
        preCond = ""
        if asm[0] in condition:
            cond = f"{condition[asm[0]]} "
            preCond = "IF"
            asm = asm[1:]
        if asm[0][0] == ".":
            return f"{preCond}{cond} goto _{asm[0][1:]};"
        funcRet = True
        return f"JR{cond}(a{asm[0]});"
    elif opcode in ("jp"):
        cond = ""
        if asm[0] in condition:
            cond = f"{condition[asm[0]]} "
            asm = asm[1:]
        if asm[0][0] == ".":
            return f"JP{cond}(a{currentFunc}_{asm[0][1:]});"
        funcRet = True
        return f"JP{cond}(a{asm[0]});"
    elif opcode in ("call"):
        cond = ""
        if asm[0] in condition:
            cond = f"{condition[asm[0]]} "
            asm = asm[1:]
        return f"CALL{cond}(a{asm[0]});"
    elif opcode in ("rst"):
        if f"a{asm[0]}" in funcsKnown:
            return f"RST(a{asm[0]});"
        return f"RST({asm[0]});"
    elif opcode in ("callfar", "farcall", "homecall"):
        return f"{opcode.upper()}(a{asm[0]});"
    elif opcode in ("lda_predef", "predef", "predef_jump"):
        return f"{opcode.upper()}(p{asm[0]});"
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
    global funcsKnown
    global funcsKnownAddr
    parser = argparse.ArgumentParser()
    parser.add_argument("fileName")
    args = parser.parse_args()

    with open(funcMapFile, "r") as inFile:
        funcsFile = inFile.read().split("\n")
    funcsKnown = list(i.split(" ")[1] for i in funcsFile if i[:7] == "#define")
    funcsKnownAddr = list(i.split(" ")[2] for i in funcsFile if i[:7] == "#define")

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
    

    print(f"\n\n")
    for f in funcList:
        print(f"func[a{f}] = {f};")
    return 0


start = time.time()
if not main():
    print(f"Done in: {time.time() - start} seconds")

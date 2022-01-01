#!/usr/bin/env python3
import argparse
import time
import os

charMapFile = "charmap.asm"
funcMapFile = "funcmap.h"

currentFunc = ""
funcList = []
funcsKnown = []
funcsKnownAddr = []
charMap = []
charMapEqu = []
includes = []
inMacro = False
funcRet = False


def convert_string(string):
    global charMap
    global charMapEqu
    print(string)
    strings = string.split('"')
    if len(strings) < 2:
        return string
    newStrings = []
    for ind, string in enumerate(strings):
        if not (ind & 1):
            newStrings.append(string)
            continue
        i = 0
        parts = []
        string = string.strip('"')
        while i < len(string):
            end = i + 1
            if string[i] == "<":
                end = string[i:].index(">") + i + 1
            elif string[i] == "'" and len(string) > i + 1:
                if string[i:end + 1] in charMap:
                    end += 1
            parts.append(string[i:end])
            i = end
        conv = []
        for part in parts:
            id = charMap.index(part)
            conv.append(charMapEqu[id])
        newStrings.append(", ".join(conv))
    return "".join(newStrings)


def check_if_label(string, prefix):
    global currentFunc
    global funcsKnown
    if string[0] == '"' and string[-1] == '"':
        return convert_string(string)
    parts = string.split(" ")
    ret = ""
    for part in parts:
        if part[0] == ".":
            part = f"{currentFunc}_{part[1:]}"
        if part in funcsKnown:
            ret = f"{ret} {prefix}{part}"
        else:
            ret = f"{ret} {part}"
    return ret[1:]


def parse_line(string):
    global inMacro
    if not string:
        return "", ""
    if string[0] != "\t" or inMacro:
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
    global includes
    global inMacro
    comment = ""
    asm = ""
    string = string.split(";")
    if inMacro:
        if string[0] == "ENDM":
            inMacro = False
        return "", f"// {';'.join(string)}"
    elif string[0].split(" ")[-1] == "MACRO":
        inMacro = True
        return "", f"// {';'.join(string)}"
    if string[0].find(":") != -1 and string[0][0] != ".":
        prevFunc = currentFunc
        parts = string[0].split(" ")
        parts[0] = f"{parts[0].strip(':')}:"  # force one : at the end
        currentFunc = parts[0][:-1]
        funcList.append(currentFunc)
        if prevFunc != "":
            if not funcRet:
                asm = f"\treturn m{currentFunc};\n"
            asm = f"{asm}}}\n\n"
        asm = f"{asm}int {currentFunc}(){{"
        if len(parts) > 1:
            asm = f"{asm}\n//{' '.join(parts[1:])}"
    elif len(string[0]) and string[0][0] == ".":
        parts = string[0][1:].split(" ")
        parts[0] = f"{parts[0].strip(':')}:"  # force one : at the end
        if len(parts) > 1:
            parts[0] = f"{parts[0]}\n//"
        localLabel = " ".join(parts)
        localLabelID = funcsKnown.index(f"{currentFunc}_{localLabel.split(':')[0]}")
        asm = f"\n_{localLabel}\n\tSET_PC({funcsKnownAddr[localLabelID]});"
    else:
        parts = list(i for i in string[0].split(" ") if i != "")
        if not parts:
            parts = [""]
        if parts[0] == "rept":
            asm = f"for(int rept = 0; rept < {' '.join(parts[1:])}; rept++){{"
        elif parts[0] == "endr":
            asm = "}"
        elif parts[0] in ("INCLUDE","INCBIN"):
            asm = f"// {asm}{string[0]}"
            includes.append(" ".join(parts[1:]))
        elif len(parts) > 1 and parts[1] == "EQU":
            asm = f"#define {parts[0]} {' '.join(parts[2:]).replace('$', '0x').replace('%', '0b')}"
        else:
            asm = f"{asm}{string[0]}"
            if asm:
                print(asm)
    funcRet = False
    if len(string) > 1:
        comment = f"  // {string[1]}"
        if asm == "":
            comment = f"// {string[1]}"
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
                "[sp]": "_sp",
                "[c]": "_c"}
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
                if f"{parts[p + 1].split(')')[0]}" in funcsKnown:
                    func = parts[p + 1].split(')')[0]
        if func:
            asm[i] = curASM.replace(func, f"a{func}")

    if opcode in ("ld", "ldh"):
        op = opcode.upper()
        dest = "_addr"
        source = ""
        if asm[0] in register and asm[1] in register:  # Register into register
            return f"{op}{register[asm[0]]}{register[asm[1]]};"
        elif asm[0] in register:  # Non-register into register
            if "[" in asm[1]:
                source = "_addr"
            return f"{op}{register[asm[0]]}{source}({check_if_label(asm[1].strip('[]'), 'm')});"
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
    elif opcode in ("ei", "di", "nop"):
        return "NOP;"
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
            return f"{preCond}{cond}goto _{asm[0][1:]};"
        funcRet = True
        return f"JR{cond}(m{asm[0]});"
    elif opcode in ("jp"):
        cond = ""
        if asm[0] == "hl":
            funcRet = True
            return "JP_hl;"
        if asm[0] in condition:
            cond = f"{condition[asm[0]]} "
            asm = asm[1:]
        if asm[0][0] == ".":
            return f"JP{cond}(m{currentFunc}_{asm[0][1:]});"
        funcRet = True
        return f"JP{cond}(m{asm[0]});"
    elif opcode in ("call"):
        cond = ""
        if asm[0] in condition:
            cond = f"{condition[asm[0]]} "
            asm = asm[1:]
        return f"CALL{cond}({check_if_label(asm[0], 'm')});"
    elif opcode in ("rst"):
        return f"RST({check_if_label(asm[0], 'm')});"
    elif opcode in ("callfar", "farcall", "homecall"):
        return f"{opcode.upper()}(a{asm[0]});"
    elif opcode in ("lda_predef", "predef", "predef_jump"):
        return f"{opcode.upper()}(p{asm[0]});"
    elif opcode in ("add","adc","sub","sbc","and","or","xor","cp"):
        op = opcode.upper()
        if len(asm) == 2:
            return f"{op}_HL{register[asm[1]]};"
        if asm[0] in register:  # Register
            return f"{op}_A{register[asm[0]]};"
        else:
            return f"{op}_A({convert_string(asm[0].strip('[]'))});"
    elif opcode in ("push", "pop", "inc", "dec", "srl", "sla", "sra", "rl", "rlc", "rr", "rrc", "swap"):
        op = opcode.upper()
        if asm[0] in register:
            return f"{op}{register[asm[0]]};"
        else:  # unknown
            return f"huh? {opcode} {asm}"
    elif opcode in ("db", "dw", "dn", "assert", "dwcoord", "menu_coords"):
        return f"//{opcode} {asm};"
    elif opcode in ("maskbits"):
        if len(asm) == 1:
            asm.append(0)
        return f"{opcode}({(asm[0])}, {(asm[1])});"
    elif opcode in ("hlcoord", "bccoord", "decoord", "ldcoord_a", "lda_coord"):
        if len(asm) == 2:
            asm.append("wTilemap")
        return f"{opcode}({(asm[0])}, {(asm[1])}, {asm[2]});"
    elif opcode in ("hlbgcoord", "bcbgcoord", "debgcoord"):
        if len(asm) == 2:
            asm.append("vBGMap0")
        return f"{opcode}({(asm[0])}, {(asm[1])}, {asm[2]});"
    elif opcode in ("lb"):
        return f"LD{register[asm[0]]}(({(asm[1])} << 8) | {(asm[2])});"
    elif opcode in ("ln"):
        return f"LD{register[asm[0]]}(({(asm[1])} << 4) | {(asm[2])});"
    elif opcode in ("print_name"):
        return f"opcode({check_if_label(asm[0])});"
    else:
        print(f"{opcode} {asm}")
    return f"//{opcode} {asm}"


def main():
    global currentFunc
    global funcList
    global funcsKnown
    global funcsKnownAddr
    global charMap
    global charMapEqu
    global includes
    parser = argparse.ArgumentParser()
    parser.add_argument("fileName")
    args = parser.parse_args()

    with open(charMapFile, "r") as inFile:
        charFile = inFile.read().split("\n")
    for i in charFile:
        if i[1:8] == "charmap":
            end = 11
            if i[10] == "<":
                end = i[10:].index(">") + 11
            elif i[10] == "'" and i[11] != '"':
                end += 1
            charMap.append(i[10:end])
            start = i[end:].index("$") + end + 1
            charMapEqu.append(f"0x{i[start:start+2]}")

    with open(funcMapFile, "r") as inFile:
        funcsFile = inFile.read().split("\n")
    funcsKnown = list(i.split(",")[0][8:] for i in funcsFile if i[:7] == "FUNCMAP")
    funcsKnownAddr = list(i.split(",")[1].strip(" )") for i in funcsFile if i[:7] == "FUNCMAP")

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
    if not os.path.exists(args.fileName.replace(".asm", ".c")):
        with open(args.fileName.replace(".asm", ".c"), "w") as cFile:
            cFile.write('#include "../constants.h"\n\n')
            for ln, line in enumerate(asm):
                ln = f"{line}{comment[ln]}".strip(" ")
                cFile.write(f"{ln}\n")

    print(f"\n")
    if not os.path.exists(args.fileName.replace(".asm", ".h")):
        with open(args.fileName.replace(".asm", ".h"), "w") as cFile:
            for f in funcList:
                cFile.write(f"{f}();\n")
            for inc in includes:
                cFile.write(f"//#include {inc}\n")
    print(f"\t// {args.fileName.replace('.asm', '.c')}")
    for f in funcList:
        print(f"\tREDIRECT({f});")
    return 0


start = time.time()
if not main():
    pass
    #print(f"Done in: {time.time() - start} seconds")

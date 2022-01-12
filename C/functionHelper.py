#!/usr/bin/env python3
import argparse
import time
import os

contained = "containedFunctions.txt"
containedFuncs = []
converted = "convertedFunctions.txt"
convertedFuncs = []


def analyze(fileName):
    printFile = f"{fileName}\n"
    with open(fileName, "r") as inFile:
        asmFile = inFile.read().split("\n")
    currentFunc = None
    currentFuncs = []
    fallthrough = False
    for line in asmFile:
        if line:
            if line[:3] == "int":
                currentFunc = line.split("(")[0][4:]
                currentFuncs.append(currentFunc)
                fallthrough = False
            elif line[0] == "}":
                if not fallthrough:
                    for cFunc in currentFuncs:
                        if cFunc not in containedFuncs and cFunc not in convertedFuncs:
                            print(f"{printFile}\t{cFunc}")
                            printFile = ""
                            containedFuncs.append(cFunc)
                    currentFunc = None
                    currentFuncs = []
                    fallthrough = False
            elif line[:7] == "\treturn" and "[" not in line and currentFunc not in convertedFuncs:
                fallthrough = True
            elif line[0] == "\t":
                op = line.strip("\t").split("(")[0].split(" ")[0]
                if op in ("CALL", "CALL_Z", "CALL_NZ", "CALL_C", "CALL_NC",
                                "JP", "JP_Z", "JP_NZ", "JP_C", "JP_NC",
                                "JR", "JR_Z", "JR_NZ", "JR_C", "JR_NC", "RST"):
                    routine = line.split(";")[0].replace(op, "", 1).strip("\t() ")[1:]
                    if routine not in containedFuncs and routine not in convertedFuncs:
                        currentFunc = None
                        currentFuncs = []
                        fallthrough = False
                elif op in ("JP_hl", "JP_hl;", "return", "return;",
                            "FARCALL", "CALLFAR", "HOMECALL", "PREDEF", "PREDEF_JUMP"):
                    currentFunc = None
                    currentFuncs = []
                    fallthrough = False
    with open(contained, "w") as outFile:
        outFile.write("\n".join(containedFuncs))


def update(fileName):
    printFile = f"{fileName}\n"
    with open(fileName, "r") as inFile:
        asmFile = inFile.read().split("\n")
    currentFunc = None
    with open(fileName, "w") as outFile:
        for lineNum, line in enumerate(asmFile):
            skipLine = False
            if line:
                if line[:3] == "int":
                    currentFunc = line.split("(")[0][4:]
                elif line[0] == "}":
                    currentFunc = None
                elif line[0] == "\t":
                    op = line.strip("\t").split("(")[0].split(" ")[0]
                    if op == "SET_PC":
                        if currentFunc in containedFuncs or currentFunc in convertedFuncs:
                            skipLine = True
                    elif op in ("CALL", "CALL_Z", "CALL_NZ", "CALL_C", "CALL_NC",
                                "JP", "JP_Z", "JP_NZ", "JP_C", "JP_NC",
                                "JR", "JR_Z", "JR_NZ", "JR_C", "JR_NC", "RST"):
                        routine = line.split(";")[0].replace(op, "", 1).strip("\t() ")[1:]
                        condition = ""
                        op = op.split("_")
                        if len(op) == 2:
                            condition = f"IF_{op[1]} "
                        op = op[0]
                        comment = f"{';'.join(line.split(';')[1:])}"
                        if routine in convertedFuncs:
                            if currentFunc in containedFuncs or currentFunc in convertedFuncs:
                                if len(op) > 2:  # Op is either call or rst
                                    line = f"\t{condition}{routine}();{comment}"
                                else:
                                    line = f"\t{condition}return {routine}();{comment}"
                                print(f"{printFile}{lineNum + 1}: {line}")
                                printFile = ""
                        elif routine in containedFuncs:
                            if len(op) > 2:  # Op is either call or rst
                                line = f"\t{condition}CCALL(a{routine});{comment}"
                            else:
                                line = f"\t{condition}return {routine}();{comment}"
                            print(f"{printFile}{lineNum + 1}: {line}")
                            printFile = ""
            if not skipLine:
                outFile.write(f"{line}\n")
            skipLine = False


def main():
    global containedFuncs
    global convertedFuncs
    parser = argparse.ArgumentParser()
    parser.add_argument("fileName")
    parser.add_argument("-a", "--analyze", dest="analyze", action="store_true", help="Check file for contained functions, and update list")
    parser.add_argument("-u", "--update", dest="update", action="store_true", help="Update calls/jumps to contained/converted functions in C file")
    args = parser.parse_args()
    
    if os.path.exists(converted):
        with open(converted, "r") as inFile:
            convertedFuncs = list(i for i in inFile.read().split("\n") if i)

    if os.path.exists(contained):
        with open(contained, "r") as inFile:
            containedFuncs = list(i for i in inFile.read().split("\n") if i and i not in convertedFuncs)    

    if args.analyze:
        analyze(args.fileName)
    if args.update:
        update(args.fileName)

    return 0


start = time.time()
if not main():
    pass
    #print(f"Done in: {time.time() - start} seconds")

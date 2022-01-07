#!/usr/bin/env python3
import argparse
import time
import os

existing = "containedFuncs.txt"

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("fileName")
    args = parser.parse_args()

    existingFuncs = []
    if os.path.exists(existing):
        with open(existing, "r") as inFile:
            existingFuncs = list(i for i in inFile.read().split("\n") if i)
            
    with open(args.fileName, "r") as inFile:
        asmFile = inFile.read().split("\n")

    currentFunc = []
    fallthrough = False
    for line in asmFile:
        if line:
            if line[:3] == "int":
                currentFunc.append(line.split("(")[0][4:])
                fallthrough = False
            elif line[0] == "}":
                if not fallthrough:
                    for cFunc in currentFunc:
                        if cFunc not in existingFuncs:
                            print(f"{cFunc}")
                    currentFunc = []
                    fallthrough = False
            elif line[:7] == "\treturn":
                fallthrough = True
            elif line[0] == "\t":
                line = line.strip("\t").split("(")[0].split(" ")[0]
                if line in ("CALL", "CALL_Z", "CALL_NZ", "CALL_C", "CALL_NC",
                            "JP", "JP_Z", "JP_NZ", "JP_C", "JP_NC", "JP_hl;",
                            "JR", "JR_Z", "JR_NZ", "JR_C", "JR_NC", "RST",
                            "FARCALL", "CALLFAR", "HOMECALL", "PREDEF", "PREDEF_JUMP"):
                    currentFunc = []
                    fallthrough = False
    return 0


start = time.time()
if not main():
    pass
    #print(f"Done in: {time.time() - start} seconds")

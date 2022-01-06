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

    currentFunc = None
    for line in asmFile:
        if line:
            if line[:3] == "int":
                currentFunc = line.split("(")[0][4:]
            elif line[0] == "}":
                if currentFunc:
                    if currentFunc not in existingFuncs:
                        print(f"{currentFunc}")
            elif line[0] == "\t":
                line = line.strip("\t").split("(")[0].split(" ")[0]
                if line in ("CALL", "CALL_Z", "CALL_NZ", "CALL_C", "CALL_NC",
                            "JP", "JP_Z", "JP_NZ", "JP_C", "JP_NC", "JP_hl;",
                            "JR", "JR_Z", "JR_NZ", "JR_C", "JR_NC", "RST",
                            "FARCALL", "CALLFAR", "HOMECALL", "PREDEF", "PREDEF_JUMP"):
                    currentFunc = None
    return 0


start = time.time()
if not main():
    pass
    #print(f"Done in: {time.time() - start} seconds")

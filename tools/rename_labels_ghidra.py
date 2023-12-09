#!/usr/bin/env python3

# TODO: Description

import sys
import re
import fileinput

if (len(sys.argv) > 2):
    print("order_labels.py inputfile")
    sys.exit(1)

labels = []
out = ""

with fileinput.input(openhook=fileinput.hook_encoded("shift_jis")) as f:
    for line in f:
        m = re.search(r'^((?:LAB_|LP_GEN_)[a-f0-9]+):', line)
        if m:
            labels.append(m.group(1))

        out += line

for i, l in enumerate(labels):
    nl = "L" + str(i)
    out = re.sub(r'\b' + l, nl, out)

print(out)
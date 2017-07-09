#!/usr/bin/python3

fname = 'shell.bin'

with open(fname, 'rb') as f:
    data = f.read()

code  = ''
count = 0

for ch in data:
    if ch <= 0xf:
        code += '\\x0%x' % ch
    else:
        code += '\\x%x' % ch
    count += 1

print("shellcode:\n" + code + "\n")
print("bytes    : {0}".format(count))

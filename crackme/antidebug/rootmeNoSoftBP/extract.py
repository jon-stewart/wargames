entry0 = 0x80
load1  = 0x123

sz = load1 - entry0

print(": ", hex(sz))

with open("crackme.bin", "rb") as fp:
    elf = fp.read()

_text = elf[entry0 : entry0 + sz]

with open("text_section", "wb") as fp:
    fp.write(_text)

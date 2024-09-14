# Reverse Engineering Basics

```
->


 .--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--.
/ .. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \
\ \/\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ \/ /
 \/ /`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'\/ /
 / /\                                                                                                            / /\
/ /\ \                                                                                                          / /\ \
\ \/ /                                                                                                          \ \/ /
 \/ /    .______       ___________    ____  _______ .______          _______. _______                            \/ /
 / /\    |   _  \     |   ____\   \  /   / |   ____||   _  \        /       ||   ____|                           / /\
/ /\ \   |  |_)  |    |  |__   \   \/   /  |  |__   |  |_)  |      |   (----`|  |__                             / /\ \
\ \/ /   |      /     |   __|   \      /   |   __|  |      /        \   \    |   __|                            \ \/ /
 \/ /    |  |\  \----.|  |____   \    /    |  |____ |  |\  \----.----)   |   |  |____                            \/ /
 / /\    | _| `._____||_______|   \__/     |_______|| _| `._____|_______/    |_______|                           / /\
/ /\ \                                                                                                          / /\ \
\ \/ /    _______ .__   __.   _______  __  .__   __.  _______  _______ .______       __  .__   __.   _______    \ \/ /
 \/ /    |   ____||  \ |  |  /  _____||  | |  \ |  | |   ____||   ____||   _  \     |  | |  \ |  |  /  _____|    \/ /
 / /\    |  |__   |   \|  | |  |  __  |  | |   \|  | |  |__   |  |__   |  |_)  |    |  | |   \|  | |  |  __      / /\
/ /\ \   |   __|  |  . `  | |  | |_ | |  | |  . `  | |   __|  |   __|  |      /     |  | |  . `  | |  | |_ |    / /\ \
\ \/ /   |  |____ |  |\   | |  |__| | |  | |  |\   | |  |____ |  |____ |  |\  \----.|  | |  |\   | |  |__| |    \ \/ /
 \/ /    |_______||__| \__|  \______| |__| |__| \__| |_______||_______|| _| `._____||__| |__| \__|  \______|     \/ /
 / /\                                                                                                            / /\
/ /\ \                                                                                                          / /\ \
\ \/ /                                                                                                          \ \/ /
 \/ /                                                                                                            \/ /
 / /\.--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--./ /\
/ /\ \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \/\ \
\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `' /
 `--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'

->
```

---

## Pre-requisites

- Basic programming concepts
<!--stop-->
- Knowledge check: difference between program and process

<!--stop-->

## Good to know

- Python
<!--stop-->
- Assembly language

---

## What are we reversing?

<!--stop-->

Development life cycle:

- Ideas/Planning
<!--stop-->
- Design
<!--stop-->
- Development
<!--stop-->
- Compilation
<!--stop-->
- Assembly
<!--stop-->
- Stripping
<!--stop-->
- Obfuscation?

---

## Information Loss

- Design to development
<!--stop-->
- development to development
<!--stop-->
- compilation
<!--stop-->
- assembly
<!--stop-->
- Optional obfuscation

---

## Static Analysis

- nm
<!--stop-->
- checksec
<!--stop-->
- strings
<!--stop-->
- objdump
<!--stop-->
- readelf

---

## Dynamic Analysis

- ltrace, strace
<!--stop-->
- GDB
<!--stop-->
- Radare

---

## Disassemblers

- Cutter
<!--stop-->
- Binary Ninja
<!--stop-->
- Ghidra
<!--stop-->
- IDA
<!--stop-->
- Angr

---

## Applications of Reverse Engineering

- Software Analysis
<!--stop-->
- Documentation
<!--stop-->
- API development
<!--stop-->
- Bug hunting
<!--stop-->
- Digital Forensics
<!--stop-->
- Debugging
<!--stop-->
- Game Modding
<!--stop-->
- Cracking

---

## Reversing Intermediate Code

- [Weird Snake - PicoCTF 2024](https://play.picoctf.org/practice?category=3&page=1&search=weird)

<!--stop-->

```python
#

# In python shell

# Disassemble python statement
import dis
dis.dis("print(3)")



# Disassemble python function
def func(x, y):
return x + y - 3
dis.dis(func)


#
```

```bash
#

# Disassemble python file
python -m dis file.py

#
```

---

## Reversing Using Static Tools

- [PicoCTF 2024 - Packer](https://play.picoctf.org/practice/challenge/421?category=3&page=1&search=packer)

<!--stop-->

```sh
-

strings packer.out

-
```

<!--stop-->

```sh
-

upx -d packer.out

-
```

<!--stop-->

```sh
-

strings unpacked.out

-
```

---

## Reversing Using Dynamic Tools

- [PicoCTF 2024 - File Run 1](https://play.picoctf.org/practice/challenge/266?category=3&page=1&search=file-run1)

<!--stop-->

```sh
#

cutter run.out

#
```

---

## References

1. [ASCII Art](https://www.asciiart.eu)
1. [Reverse Engineering - pwn.college](https://www.youtube.com/playlist?list=PL-ymxv0nOtqrGVyPIpJeostmi7zW5JS5l)

---

## Epilogue

```
-

    Questions?
    Feedback?
    Suggestions for Next topic?

-
```

<!--stop-->

```
-

    Thank You

-
```

## Title Banner

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

### Required

- Basic programming concepts
- Knowledge check: difference between program and process
- Tools: Kali Linux or Ghidra/Cutter

### Good to know

- Python
- Assembly language

---

## What is Reverse Engineering

Wikipedia ->

> Reverse engineering (also known as backwards engineering or back engineering) is a process or method through which one attempts to understand through deductive reasoning how a previously made device, process, system, or piece of software accomplishes a task with very little (if any) insight into exactly how it does so.

---

## What are we reversing?

Development life cycle:

- Ideas/Planning
- Design
- Development
- Compilation
- Assembly
- Stripping
- Obfuscation?

---

## Scenario

Objectives of an attacker:

- Create a malware
- Software that's used commonly
- Add a backdoor in the application
- Difficult to traceback

---

## Why are we reversing

Information loss at each stage of the development lifecycle.

Code used: [calculator.c](/assets/talks/reverse/calculator.c)

- Design to development
- Development to development
- Intermediate code

  ```sh

  gcc -E filename.c -o fileout.i

  ```

- Compile to machine code

  ```sh

  # compile without debug symbols
  gcc -o file.out filename.c

  # compile with debug symbols
  gcc -g -o file.out filename.c

  ```

- Optional obfuscation

  ```sh

  strip file.out

  ```

---

## Static Analysis

- Print generic file metadata

  ```sh

  file file.out

  ```

- Display all the printable characters

  ```sh

  strings file.out

  ```

- nm

  - Prints all the symbols in binary

  ```sh

  nm -a file.out

  ```

- Print binary metadata

  ```sh

  # different headers:
  readelf -h a.out

  # symbol table:
  readelf -s a.out

  ```

- Prints compile settings for the binary

  ```sh

  checksec --file=file.out

  ```

- Disassemble a binary

  ```sh

  # single function:
  objdump -M intel --disassemble=<func_name> file.out

  # full binary:
  objdump -M intel -d file.out

  ```

---

## Dynamic Analysis

- ltrace, strace

  ```sh

  ltrace file.out
  strace file.out

  ```

- GDB

  ```sh

  # -q to silence the initial banner
  gdb -q file.out

  ```

- Radare

  ```sh

  r2 file.out

  ```

---

## Disassemblers

- Cutter
- Binary Ninja
- Ghidra
- IDA
- Angr

---

## Applications of Reverse Engineering

- Software Analysis
- Documentation
- API development
- Bug hunting
- Digital Forensics
- Debugging
- Game Modding
- Cracking

---

## Interactive Workshop

```



               Workshop



```

---

### Challenge 1: Intermediate Language Analysis

- [Weird Snake - PicoCTF 2024](https://play.picoctf.org/practice?category=3&page=1&search=weird)
- File: [snake.disasm](/assets/talks/reverse/snake.disasm)

---

### Challenge 2: Static Analysis I

- [PicoCTF 2024 - File Run 1](https://play.picoctf.org/practice/challenge/266?category=3&page=1&search=file-run1)
- File: [run](/assets/talks/reverse/run)

---

### Challenge 3: Static Analysis II

- [PicoCTF 2024 - Packer](https://play.picoctf.org/practice/challenge/421?category=3&page=1&search=packer)
- File: [packer.out](/assets/talks/reverse/packer.out)

---

### Challenge 4: Dynamic Analysis

- [PicoCTF 2024 - Crackme](https://play.picoctf.org/practice?category=3&page=1&search=crackme%20100)
- File: [crackme100](/assets/talks/reverse/crackme100)

---

## Summary

1. Use static analysis tools to gather information about the file without running it.
1. For binary files, most of the disassemblers will be helpful. Decompiled code is usually in C or C++ even if it is compiled from other languages like GoLang.
1. To analyse the dynamic behavior, CLI debuggers are helpful. Eg. gdb, radare. Knowledge of assembly language is required here.
1. If it is some other intermediate code, then it is probably some plain text file with obfuscated variable names and data structure. It requires language specific deobfuscators.

---

## Further Learning

- [Reverse Engineering - pwn.college](https://pwn.college/program-security/reverse-engineering/)
- [Assembly for x86, ARM32, x64, and ARM64](https://github.com/mytechnotalent/Reverse-Engineering?tab=readme-ov-file#x86-course)
- CTFs :
  - [PicoCTF](https://www.picoctf.org) - Beginner friendly
  - [pwnable.kr](https://pwnable.kr/)
  - [Overthewire](https://overthewire.org/wargames/)
- Advance topics:
  - [Binary exploitation - RazviOverflow](https://www.youtube.com/playlist?list=PLchBW5mYosh_F38onTyuhMTt2WGfY-yr7)
  - [How Kernel Works](https://tldp.org/LDP/lkmpg/2.6/lkmpg.pdf)

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

```
-

    Thank You

-
```

## Description

This is the presentation slides for the talk given in October 2024 at DC604 - Vancouver Cybersecurity Community.
Everything below this line is part of the presentation with extra description added explain some slides.

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

There is 'Reverse' in 'Reverse Engineering', that means that was something in the forward direction. That something can be anything from wearing a black shirt to designing Netflix. In this context, we are reverseing the process of developing a software.

- Ideas/Planning
- Design
- Development
- Compilation
- Assembly

---

## Scenario

All the examples in the demo is based on this scenario:

> Someone with malicious intent wants to create a software that is commonly used by everyone and install a backdoor in it.

Objectives of the attacker:

- Create a malware
- Software that's used commonly
- Add a backdoor in the application
- Difficult to traceback

---

## Why are we reversing

We need to reverse because a lot of information is lost at each stage of software development.

> Code used: [calculator.c](/assets/talks/reverse/calculator.c)

### Code Pre-Processing

A lot of operations happen during pre-processing stage. Some of them are:

- All the comments from the code are removed
- All the macros are processed

It means, that any comment explaining the code won't exist after pre-processing.

The main function in `calculator.c` looks like this:

```c
int main() {
  char expression[MAX]; // Array to hold the input expression
  printf("Enter a mathematical expression: ");
  fgets(expression, sizeof(expression), stdin); // Read the input expression
                                                // Remove newline character from input
  expression[strcspn(expression, "\n")] = 0;

  printFile("./calculator.c"); // Print the contents of the file

  // Evaluate the expression and store the result
  double result = evaluate(expression);
  // Print the result with two decimal places
  printf("Result: %.2f\n", result);
  return 0; // Indicate successful completion
}
```

Generating the intermediate file after c pre-processing:

```sh
gcc -E calculator.c -o calculator.i
```

The code after pre-processing:

```c
int main() {
  char expression[100];
  printf("Enter a mathematical expression: ");
  fgets(expression, sizeof(expression),
# 174 "calculator.c" 3 4
      stdin
# 174 "calculator.c"
      );

  expression[strcspn(expression, "\n")] = 0;

  printFile("./calculator.c");


  double result = evaluate(expression);

  printf("Result: %.2f\n", result);
  return 0;
}

```

### Compilation

Compilation is the process of converting human-readable code to machine code. Compilation output can be modified by providing different flags.
A software can be compiled with or without debug symbols. In both the cases, actual code is lost and it can only be translated to actual assembly language accurately.

#### Compiling the code with debug symbols:

```sh
gcc -g -o calc-debug.out calculator.c
```

Information related to function names is retained in debug file:

```sh
objdump --disassemble -j .text calc-debug.out  | grep '>:' | nl | sort -rn
    18  0000000000001942 <main>:
    17  0000000000001582 <evaluate>:
    16  00000000000014b7 <applyOp>:
    15  0000000000001473 <precedence>:
    14  00000000000013c2 <printFile>:
    13  0000000000001369 <pop>:
    12  00000000000012ff <push>:
    11  00000000000012de <isEmpty>:
    10  00000000000012c1 <initStack>:
     9  00000000000012a3 <divide>:
     8  0000000000001285 <multiply>:
     7  0000000000001267 <subtract>:
     6  0000000000001249 <add>:
     # ... <omitted for brevity>
```

Above names matches the function names in `calculator.c`:

```sh
grep -i '(.*) {' calculator.c | grep -v 'while\|if\|switch\|for\|items'
    double add(double a, double b) { return a + b; }
    double subtract(double a, double b) { return a - b; }
    double multiply(double a, double b) { return a * b; }
    double divide(double a, double b) { return a / b; }
    void initStack(Stack *s) {
    int isEmpty(Stack *s) {
    void push(Stack *s, double item) {
    double pop(Stack *s) {
    void printFile(char *filename) {
    int precedence(char op) {
    double applyOp(double a, double b, char op) {
    double evaluate(char *expression) {
    int main() {
```

#### Compiling the code without debug symbols:

```sh
gcc -o calc.out calculator.c
```

`calc.out` contains binary data which can be analysed using tools like `objdump` or `otool`.
The code is displayed in assembly language which needs deep analysis to figure out the actual intention of the code.

Here is the transformed code:

```sh
objdump -M intel --disassemble=main calc.out
    # ... <omitted for brevity>
    0000000000001942 <main>:
        1942:       f3 0f 1e fa             endbr64
        1946:       55                      push   rbp
        1947:       48 89 e5                mov    rbp,rsp
        194a:       48 83 c4 80             add    rsp,0xffffffffffffff80
        194e:       64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
        1955:       00 00
        1957:       48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
        195b:       31 c0                   xor    eax,eax
        195d:       48 8d 05 dc 06 00 00    lea    rax,[rip+0x6dc]        # 2040 <_IO_stdin_used+0x40>
        1964:       48 89 c7                mov    rdi,rax
        1967:       b8 00 00 00 00          mov    eax,0x0
        196c:       e8 9f f7 ff ff          call   1110 <printf@plt>
        1971:       48 8b 15 98 26 00 00    mov    rdx,QWORD PTR [rip+0x2698]        # 4010 <stdin@GLIBC_2.2.5>
        1978:       48 8d 45 90             lea    rax,[rbp-0x70]
        197c:       be 64 00 00 00          mov    esi,0x64
        1981:       48 89 c7                mov    rdi,rax
        1984:       e8 a7 f7 ff ff          call   1130 <fgets@plt>
        1989:       48 8d 45 90             lea    rax,[rbp-0x70]
        198d:       48 8d 15 ce 06 00 00    lea    rdx,[rip+0x6ce]        # 2062 <_IO_stdin_used+0x62>
```

### Obfuscation

Even after the compilation, the binary file can be modifed to reduce the file size by either stripping
all the symbols or packing it to compress its size.

```sh
# Stripping all the symbols,
# same file is replaced with stripped version
strip calc.out

# Compressing the file using `upx`
# Same file is replaced with packed version
upx calc.out
```

Here is the size comparison of different versions of same binary.
Files have been copied to ensure that different versions are available for comparison.

```sh
find . -executable -type f -exec ls -lh {} \;
    -rwxr-xr-x 1 ainz ainz 8.9K Jan  6 09:47 ./packed.out
    -rwxr-xr-x 1 ainz ainz 67K Jan  6 09:47 ./debug-calc
    -rwxr-xr-x 1 ainz ainz 67K Jan  6 09:47 ./stripped
    -rwxr-xr-x 1 ainz ainz 75K Jan  6 09:47 ./full-debug
    -rwxr-xr-x 1 ainz ainz 70K Jan  6 09:47 ./full
    -rwxr-xr-x 1 ainz ainz 8.9K Jan  6 09:47 ./obfuscalculated
    -rwxr-xr-x 1 ainz ainz 75K Jan  6 09:47 ./unstripped
```

---

## Reverse Engineering Demo

The demo file is taken from one of the challenge given in `Huntress CTF 2024` organized during Cybersecurity Awareness month.

File: [gocrackme](/assets/talks/reverse/gocrackme)

Challenge Description:

> Welcome to the Go Dojo, gophers in training!
>
> Go malware is on the rise. So we need you to sharpen up those Go reverse engineering skills.
> We've written three simple CrackMe programs in Go to turn you into Go-binary reverse engineering ninjas!

### Static Analysis

One of the ways to analyse any binary without actually running it. Information like, file type, metadata, symbols and assembly code can be gathered with this approach.

Different tools used for static analysis:

```sh
# 1. Print generic file metadata
file file.out

# 2. Display all the printable characters
strings file.out

# 3. Print all the symbols in binary
nm -a file.out

# 4. Print binary metadata
readelf -h a.out # different headers
readelf -s a.out  # symbol table

# 5. Print compile settings for the binary
checksec --file=file.out

# 6. Disassemble a binary
objdump -M intel --disassemble=<func_name> file.out # single function
objdump -M intel -d file.out # full binary

```

---

### Disassemblers

It is advanced static analysis where different tools try to transform the machine code to high level language like `C` or `C++`.
The process is called `Decompilation`. After decompilation, the code is just another programming language in human readable format.
It makes it easier to get a sense of what's happening in the binary file.

Some of these tools are:

- Cutter
- Binary Ninja
- Ghidra
- IDA
- Angr

---

### Dynamic Analysis

This type of analysis depends on running the binary and interacting with it in real-time. It can be dangerous as any side-effect of running
the binary will actually affect the system.

Tools for dynamic analysis:

```sh
# 1. ltrace
# Intercepts and displays any library function call made by the binary
ltrace file.out

# 2. strace
# Intercepts and displays any system calls made by the binary
strace file.out

# 3. GDB
# For runtime debugging.
# It allows us to manipulate the runtime environment during execution
gdb file.out

# 4. Radare
# Same as gdb
r2 file.out

```

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

## Practice Challenges

In the second half of the presentation, workshop was conducted to apply the concepts into practice.
All the challenges are taken from [PicoCTF](https://play.picoctf.org/) platform

Solution writeup can be find here: [Challenge Writup](/assets/talks/reverse/challenges-writup.txt)

---

### Challenge 1: Intermediate Language Analysis

- [Weird Snake - PicoCTF 2024](https://play.picoctf.org/practice?category=3&page=1&search=weird)
- File: [snake.disasm](/assets/talks/reverse/snake.disasm)
- Flag format: `picoCTF{somestring}`
- Desription:
  > I have a friend that enjoys coding and he hasn't stopped talking about a snake recently.
  >
  > He left this 'snake.disasm' file on my computer and dares me to uncover a secret phrase from it. Can you assist?

Tips

```python
# Disassemble python statement
import dis
dis.dis("print(3)")


# Disassemble python function
def func(x, y):
    return x + y - 3
dis.dis(func)

```

```bash
# Disassemble python file
python -m dis file.py

```

---

### Challenge 2: Static Analysis I

- [PicoCTF 2024 - Packer](https://play.picoctf.org/practice/challenge/421?category=3&page=1&search=packer)
- File: [packer.out](/assets/talks/reverse/packer.out)
- Flag format: `picoCTF{somestring}`
- Description:
  > Reverse this linux executable 'packer.out' and find the flag

Solution:

```sh
# Check static strings
strings packer.out

# unpack using upx after finding out that above binary is packed with `upx`
upx -d packer.out

# Check the static strings again
strings unpacked.out | grep -i 'flag'

# Convert hexstring to ascii
echo <hexstring> | xxd -r -p

```

---

### Challenge 3: Static Analysis II

- [PicoCTF 2024 - File Run 1](https://play.picoctf.org/practice/challenge/266?category=3&page=1&search=file-run1)
- File: [run](/assets/talks/reverse/run)
- Flag format: `picoCTF{somestring}`
- Description:
  > A program has been provided to you, what happens if you try to run it on the command line?
  >
  > Now, try different ways to find the flag.

Approaches:

1. Static Analysis: `strings run.out`
2. Disassembly: Run Ghidra, and it will show that a variable gets printed, checking that address in memory reveals the flag.
3. Dynamic Analysis: `chmod u+x run && ./run`

---

### Challenge 4: Dynamic Analysis

- [PicoCTF 2024 - Crackme](https://play.picoctf.org/practice?category=3&page=1&search=crackme%20100)
- File: [crackme100](/assets/talks/reverse/crackme100)
- Flag format: password asked in the binary
- Description:
  > A classic Crackme. Find the password, get the flag!
  >
  > Crack the Binary file locally and recover the password.
  >
  > Note: Focus is on finding the password.

Approach:

1. Binary can be disassembled using ghidra and try to understand the decompiled code

2. Use gdb to display the expected password by adding breakpoint at the point of comparison

```sh
# Check compilation flags:
checksec --file=./crackme100

# Get the comparison address:
objdump -M intel --disassemble=main crackme100  | grep cmp

# Debug using gdb
# gdb commands:
# Start the program:   run
# Add breakpoint:   break *0x123456
# Continue after breakpoint:   continue
# Update value of a register:   set $rax = 0x123456
# Update value of a register:   set $rax = 0x123456
# Display value at an address:   x/s 0x123456
gdb -q crackme100

```

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
  - Process Management

---

## References

1. [Reverse Engineering - pwn.college](https://www.youtube.com/playlist?list=PL-ymxv0nOtqrGVyPIpJeostmi7zW5JS5l)
1. [GDB Documentation](https://sourceware.org/gdb/current/onlinedocs/gdb.pdf)
1. [PicoCTF](https://play.picoctf.org/)
1. [Huntress CTF](https://www.huntress.com/) - The link points the the team who organized the event.

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

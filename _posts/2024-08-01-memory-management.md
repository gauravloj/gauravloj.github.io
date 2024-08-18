---
title: Memory Management
time: 2024-08-01 18:51:08
categories: [Talks, "2024"]
tags: [linux, memory management, c, optimization]
---

## Description

This is the topic presented in Vancouver Linux User Group, February 2024 meeting. It explains how memory is managed inside as well as outside of a process. All the demo codes are written in `C` language and internals are shown using `GDB debugger` tool.

All the content apart from this description is part of the actual presentation. I have added extra description for each slide for better understanding.

Actual presentation can be found at [Memory Management - GitHub](https://github.com/gauravloj/talks/tree/main/2024/memory-management)

---

## Memory Management Categories

Memory allocation can be seen from two different perspective:

1. How a process manages its own memory

- Static Memory Allocation
- Dynamic memory Allocation

1. How kernel manages process memory i.e. process memory allocation

Note: ultimately it is the kernel that manages everything, but the distinction is made based on where the allocated memory exists. Later, we will know that the whole process state exists inside virtual memory and kernel allocates and maps this virtual memory to physical memory

---

## Memory Management of Running Process

### Process Layout

A process memory on its own is divided into many different segments. Main segments that are useful to know for a normal developer are:

1. Text - Static memory that stores the actual code in machine readable format
1. Data - Static memory that stores the globally initialized variables
1. BSS - Static memory that stores the globally uninitialized variables
1. Heap - Dynamic memory that is ready to be allocated in demand
1. Stack - Dynamic memory that stores local variables and function context

```


          ---------   0x00000000
         |~~~~~~~~~|
         |  TEXT   |
         |         |
          ---------
         |         |
         |  DATA   |
         |         |
          ---------
         |         |
         |   BSS   |
         |         |
          ---------
         |         |
         |  HEAP   |
         |         |
         |    |    |
         |    |    |
         |   \|/   |
         |    V    |
         |         |
         |         |
         |         |
         |         |
         |         |
         |         |
         |         |
          ---------
         |    A    |
         |   /|\   |
         |    |    |
         |    |    |
         |         |
         |  STACK  |
          ---------   0x7ffff000

```

---

### Probing Static Memory

#### Text Segment

We will use this simple program to check the text segment. This program doesn't contain any variables, so the only segment that will occupy memory is text segment.

```c
// filename: 01-text.c

int main(int argc, char **argv) {
  return 0;
}

```

Checking the size of different segments.

Observation: output size on Mac is 64 bytes while on Linux it is 72 bytes. It is probably because of the difference in compilation algorithm for different architecture, ARM for Mac and AMD64 for Linux.

```sh

gcc -c -o text.o 01-text.c


#########################
#                       #
#       MacOS           #
#                       #
#########################

size -m text.o
#  Segment : 64
#  	Section (__TEXT, __text): 32
#  	Section (__LD, __compact_unwind): 32
#  	total 64
#  total 64

otool -s __TEXT __text -v text.o

# text.o:
# (__TEXT,__text) section
# _main:
# 0000000000000000	sub	sp, sp, #0x10
# 0000000000000004	mov	x8, x0
# 0000000000000008	mov	w0, #0x0
# 000000000000000c	str	wzr, [sp, #0xc]
# 0000000000000010	str	w8, [sp, #0x8]
# 0000000000000014	str	x1, [sp]
# 0000000000000018	add	sp, sp, #0x10
# 000000000000001c	ret


#########################
#                       #
#       Linux           #
#                       #
#########################


size text.o
#  text	   data	    bss	    dec	    hex	filename
#    72	      0	      0	     72	     48	text.o

objdump -S -j .text text.o

# text.o:     file format elf64-littleaarch64
#
#
# Disassembly of section .text:
#
# 0000000000000000 <main>:
#    0:	d10043ff 	sub	sp, sp, #0x10
#    4:	b9000fe0 	str	w0, [sp, #12]
#    8:	f90003e1 	str	x1, [sp]
#    c:	52800000 	mov	w0, #0x0                   	// #0
#   10:	910043ff 	add	sp, sp, #0x10
#   14:	d65f03c0 	ret


```

---

#### BSS Segment

Adding some uninitialized variables to the code:

```c
// filename: 02-bss.c

int helper_one;
int helper_two = 0;
int helper_three = 0;

int main(int argc, char **argv) {
  return 0;
}

```

Observation: 8 bytes were allocated on Macos and 12 bytes were allocated on Linux for BSS segment. Mac optimized the allocation by skipping the variable `helper_one` that was not assigned any value, while linux considered all the uninitialised variables and allocated memory for them.

It is beneficial to have less binary size as it will take less time to load it in memory.

```sh

gcc -c -o bss.o 02-bss.c

#########################
#                       #
#       MacOS           #
#                       #
#########################

size -m bss.o
#  Segment : 72
#  	Section (__TEXT, __text): 32
#  	Section (__DATA, __common): 8 (zerofill)
#  	Section (__LD, __compact_unwind): 32
#  	total 72
#  total 72

otool -s __DATA __common bss.o
# bss.o:
# Contents of (__DATA,__common) section
# zerofill section and has no contents in the file

#########################
#                       #
#       Linux           #
#                       #
#########################

size bss.o
#    text	   data	    bss	    dec	    hex	filename
#      72	      0	     12	     84	     54	bss.o

# content of BSS segment
objdump -s -j .bss bss.o
# bss.o:     file format elf64-littleaarch64
#
# Contents of section .bss:
# 0000 00000000 00000000 00000000           ............


```

---

#### Data Segment

Along with uninitialised, there are new variables that are initialised with a `char` and a `double`.

```c

// filename: 03-data-1.c

int helper_one;
int helper_two;
int helper_three = 0;
double company_number = 555.3745;
char company_code = 'N';

int main(int argc, char **argv) {
  return 0;
}

```

Observation:

1. Data segment on both the platforms allocated 9 bytes: 8 bytes for a double and 1 byte for single character
1. Floating point representation of `555.3745` is `f9db22d1 40815afe` which is 8 bytes long
1. Even though the representation is same on both the platforms, `objdump` has displayed it in same arrangement as it is stored in memory i.e. little endian, while MacOS has reformatted it big endian for the convenience of reading.
1. Char is stored as `4e` which is hex for the ASCII value of `N`

```sh

gcc -c -o data1.o 03-data-1.c

#########################
#                       #
#       MacOS           #
#                       #
#########################

size -m data1.o
#  Segment : 84
#  	Section (__TEXT, __text): 32
#  	Section (__DATA, __common): 4 (zerofill)
#  	Section (__DATA, __data): 9
#  	Section (__LD, __compact_unwind): 32
#  	total 77
#  total 84

# content of Data segment
# Notice that '4e' is ASCII of 'N" in hex
otool -s __DATA __data data1.o

#  data1.o:
#  Contents of (__DATA,__data) section
#  0000000000000020	f9db22d1 40815afe 4e

#########################
#                       #
#       Linux           #
#                       #
#########################

size data1.o
#   text	   data	    bss	    dec	    hex	filename
#     72	      9	     12	     93	     5d	data1.o

objdump -s -j .data data1.o

#  data1.o:     file format elf64-littleaarch64
#
#  Contents of section .data:
#   0000 d122dbf9 fe5a8140 4e                 ."...Z.@N

```

Another example for data segment

```c

// filename: 04-data-2.c

int helper_count = 13;  // 0x0000000D
int entry_passcode = 0x12153467;
char head_chef[6] = "ABCDE";
int main(int argc, char**argv) {
  return 0;
}

```

Observation: Difference of little endianness and big endianness is clearly visible in below output.

```sh

gcc -c -o data2.o 04-data-2.c

#########################
#                       #
#       MacOS           #
#                       #
#########################

size -m data2.o
#  Segment : 80
#  	Section (__TEXT, __text): 32
#  	Section (__DATA, __data): 14
#  	Section (__LD, __compact_unwind): 32
#  	total 78
#  total 80

# content of Data segment
otool -d data2.o
#  data2.o:
#  (__DATA,__data) section
#  0000000000000020	0000000d 12153467 44434241 45 00


#########################
#                       #
#       Linux           #
#                       #
#########################

size data2.o

#   text	   data	    bss	    dec	    hex	filename
#     72	     14	      0	     86	     56	data2.o

objdump -s -j .data data2.o
#  data2.o:     file format elf64-littleaarch64
#
#  Contents of section .data:
#  0000 0d000000 67341512 41424344 4500      ....g4..ABCDE.

```

---

### Probing Dynamic Memory

All the demonstration after this slide are done on Linux only because MacOS doesn't have `procfs` which is used to see the current state of running process.

#### Memory Mapping

Below is a simple program that does nothing in a loop.

```c
// filename: 05-mapping-stack.c

#include <unistd.h> // Needed for sleep function
int main(int argc, char** argv) {
  // Infinite loop
  while (1) {
    sleep(1); // Sleep 1 second
  };
  return 0;
}

```

Observations:

1. By checking the `maps` file in the procfs for the respective process, we can see a lot of addresses in the memory.
1. If we check the difference between the first address `aaaac9d00000` and last address `ffffceb88000`, it will come out in the range of TBs of memory. It is because this mapping represents the virtual memory of the process and not the actual memory assigned on the RAM.
1. Initial 3 address ranges are mapped to the actual binary code
1. Other addresses are for shared libraries
1. One of the address range is dedicated for stack: `ffffceb67000-ffffceb88000`. Stack is where all the function context is stored.

Here is the description of each column in the mapping output:

1. Address range - start address to end address of mapped memory
1. Permission - rwxps - read, write, execute, private, shared
1. Offset - offset of mapped memory in file, 0 for heap
1. Device - major:minor if mapped memory is file, 00:00 for heap
1. Inode - inode number if mapped memory is file, 0 for heap
1. Pathname - path of mapped memory if mapped memory is file, 'heap' for heap, 'stack' for stack

```bash

gcc -o stackmap 05-mapping-stack.c
./stackmap &  # returns PID

# Assume PID is 1234

# /proc is a virtual file system
ls -l /proc/1234

# Memory map of process
cat /proc/1234/maps

#  Address range             perm  offset  dev   inode                      Pathname

#  aaaac9d00000-aaaac9d01000 r-xp 00000000 fe:02 709295                     /home/ubuntu/memory-management/code/linux/stackmap
#  aaaac9d1f000-aaaac9d20000 r--p 0000f000 fe:02 709295                     /home/ubuntu/memory-management/code/linux/stackmap
#  aaaac9d20000-aaaac9d21000 rw-p 00010000 fe:02 709295                     /home/ubuntu/memory-management/code/linux/stackmap
#  ffffba310000-ffffba4a0000 r-xp 00000000 fe:02 793036                     /usr/lib/aarch64-linux-gnu/libc.so.6
#  ffffba4a0000-ffffba4ad000 ---p 00190000 fe:02 793036                     /usr/lib/aarch64-linux-gnu/libc.so.6
#  ffffba4ad000-ffffba4b0000 r--p 0019d000 fe:02 793036                     /usr/lib/aarch64-linux-gnu/libc.so.6
#  ffffba4b0000-ffffba4b2000 rw-p 001a0000 fe:02 793036                     /usr/lib/aarch64-linux-gnu/libc.so.6
#  ffffba4b2000-ffffba4be000 rw-p 00000000 00:00 0
#  ffffba4d9000-ffffba4ff000 r-xp 00000000 fe:02 793033                     /usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
#  ffffba512000-ffffba514000 rw-p 00000000 00:00 0
#  ffffba514000-ffffba516000 r--p 00000000 00:00 0                          [vvar]
#  ffffba516000-ffffba517000 r-xp 00000000 00:00 0                          [vdso]
#  ffffba517000-ffffba519000 r--p 0002e000 fe:02 793033                     /usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
#  ffffba519000-ffffba51b000 rw-p 00030000 fe:02 793033                     /usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
#  ffffceb67000-ffffceb88000 rw-p 00000000 00:00 0                          [stack]


```

This code will show the mapping for heap allocation at address range `aaaaec693000-aaaaec6b4000`

```c

// filename: 06-mapping-heap.c

#include <unistd.h> // Needed for sleep function
#include <stdlib.h> // Needed for malloc function
#include <stdio.h> // Needed for printf

int main(int argc, char** argv) {
  void* ptr = malloc(1024); // Allocate 1KB from heap
  printf("Address: %p\n", ptr);
  fflush(stdout); // To force the print
  // Infinite loop
  while (1) {
    sleep(1); // Sleep 1 second
  };
  return 0;
}


```

```bash

gcc -o heapmap 06-mapping-heap.c
./heapmap &  # return PID
# [1] 1234
#
# Address: 0xaaaaec6932a0

# Assume PID is 1234
# Memory map of process
cat /proc/1234/maps

#  aaaab9ff0000-aaaab9ff1000 r-xp 00000000 fe:02 709296                     /home/ubuntu/memory-management/code/linux/heapmap
#  aaaaba00f000-aaaaba010000 r--p 0000f000 fe:02 709296                     /home/ubuntu/memory-management/code/linux/heapmap
#  aaaaba010000-aaaaba011000 rw-p 00010000 fe:02 709296                     /home/ubuntu/memory-management/code/linux/heapmap
#  aaaaec693000-aaaaec6b4000 rw-p 00000000 00:00 0                          [heap]
#  ffffb6bb0000-ffffb6d40000 r-xp 00000000 fe:02 793036                     /usr/lib/aarch64-linux-gnu/libc.so.6
#  ffffb6d40000-ffffb6d4d000 ---p 00190000 fe:02 793036                     /usr/lib/aarch64-linux-gnu/libc.so.6
#  ffffb6d4d000-ffffb6d50000 r--p 0019d000 fe:02 793036                     /usr/lib/aarch64-linux-gnu/libc.so.6
#  ffffb6d50000-ffffb6d52000 rw-p 001a0000 fe:02 793036                     /usr/lib/aarch64-linux-gnu/libc.so.6
#  ffffb6d52000-ffffb6d5e000 rw-p 00000000 00:00 0
#  ffffb6d7a000-ffffb6da0000 r-xp 00000000 fe:02 793033                     /usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
#  ffffb6db3000-ffffb6db5000 rw-p 00000000 00:00 0
#  ffffb6db5000-ffffb6db7000 r--p 00000000 00:00 0                          [vvar]
#  ffffb6db7000-ffffb6db8000 r-xp 00000000 00:00 0                          [vdso]
#  ffffb6db8000-ffffb6dba000 r--p 0002e000 fe:02 793033                     /usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
#  ffffb6dba000-ffffb6dbc000 rw-p 00030000 fe:02 793033                     /usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
#  ffffe0df4000-ffffe0e15000 rw-p 00000000 00:00 0                          [stack]

```

---

#### Probing Stack Segment

Below code is used to display that local variables are stored on stack. And by using `gdb` we will see the raw representation of stack memory. It is a simple demo to display stack memory of the running process.

```c
// filename: 07-stack.c

#include <stdio.h>

int main(int argc, char** argv) {
  char arr[4];
  arr[0] = 'A';
  arr[1] = 'B';
  arr[2] = 'C';
  arr[3] = 'D';
  return 0;
}

```

```bash

gcc -g -o stack 07-stack.c
gdb -q ./stack

# Optionally, connect to a remote process
# gdb -tui -p 1234 /path/to/binary

# inside gdb
break main   # Add a breakpoint to 'main' function
x/4xw $rsp   # display 4 words in hex starting from the address pointed by $rsp register
print arr    # display the current value of address pointed by 'arr'
x/4xw arr    # display 4 words in hex starting from the address pointed by 'arr'
set {char}0x7fffffffe2e0 = 'A' # set memory at address 0x7fffffffe2e0 to 'A'
set arr[1] = 'F' # set arr[1] to 'F'

# Other useful gdb commands
run   # Execute the binary until a breakpoint is reached
next  # Execute next line without going in any function call, works if source code is available
step  # Execute next line, step into the function if current line is a function call, works if source code is available
continue   # Continue program execution until next breakpoint
info registers   # Show current state of registers

```

Below code is to demonstrate what happens when data stored in a variable is bigger than the actual memory allocated to that variable.
When this code is run in `gdb`, it is visible the `strcpy` function not only copies the string to `str` variable, but it also overrides the memory that comes after the 10 bytes of `str` variable including the return pointer from main. And hence the program crashes with segmentation fault.

```c
// filename: 08-stack-overflow.c

#include <string.h>

int main(int argc, char** argv) {
  char str[10];
  strcpy(str, "akjsdhkhqiueryo34928739r27yeiwuyfiusdciuti7twe79ye");
  return 0;
}

```

```bash

gcc -g -o stackoverflow ./code/08-stack-overflow.c
./stackoverflow
#  Segmentation fault (core dumped)

```

---

#### Probing Heap Segment

Below code is used to display that dynamically allocated variables are stored on heap. Since heap is just another memory location, we can easily analyse the program's heap state using `gdb` commands

```c
// filename: 09-heap.c

#include <stdio.h> // For printf function
#include <stdlib.h> // For C library's heap memory functions

void print_mem_maps() {
#ifdef __linux__
  FILE* fd = fopen("/proc/self/maps", "r");
  if (!fd) {
    printf("Could not open maps file.\n");
    exit(1);
  }
  char line[1024];
  while (!feof(fd)) {
    fgets(line, 1024, fd);
    printf("> %s", line);
  }
  fclose(fd);
#endif
}

int main(int argc, char** argv) {
  // Allocate 10 bytes without initialization
  char* ptr1 = (char*)malloc(10 * sizeof(char));
  printf("Address of ptr1: %p\n", (void*)&ptr1);
  printf("Memory allocated by malloc at %p: ", (void*)ptr1);
  for (int i = 0; i < 10; i++) {
    printf("0x%02x ", (unsigned char)ptr1[i]);
  }
  printf("\n");
  // Allocation 10 bytes all initialized to zero
  char* ptr2 = (char*)calloc(10, sizeof(char));
  printf("Address of ptr2: %p\n", (void*)&ptr2);
  printf("Memory allocated by calloc at %p: ", (void*)ptr2);
  for (int i = 0; i < 10; i++) {
    printf("0x%02x ", (unsigned char)ptr2[i]);
  }
  printf("\n");
  print_mem_maps();
  free(ptr1);
  free(ptr2);
  return 0;
}

```

```bash

gcc -g -o heap 09-heap.c
gdb -q ./heap

break main
run
x/10xw ptr1

```

---

#### All at once

Below code is copied from `Extrem C` book from `Packt Publishing`. It displays the address of different variables. By combining the address mapping from `procfs` and the address printed in the output, we can see which variable is allocated to which part of the memory.

```c
// filename: 10-memory_segments.c

#include <stdio.h>
#include <stdlib.h>

int global_var;
int global_initialized_var = 5;

void function() { // This is just a demo function
  int stack_var;  // notice this variable has the same name as the one in main()

  printf("the function's stack_var is at address 0x%08x\n", &stack_var);
}

int main() {
  int stack_var; // same name as the variable in function()
  static int static_initialized_var = 5;
  static int static_var;
  int *heap_var_ptr;

  heap_var_ptr = (int *)malloc(4);

  // These variables are in the data segment
  printf("global_initialized_var is at address 0x%08x\n",
         &global_initialized_var);
  printf("static_initialized_var is at address 0x%08x\n\n",
         &static_initialized_var);

  // These variables are in the bss segment
  printf("static_var is at address 0x%08x\n", &static_var);
  printf("global_var is at address 0x%08x\n\n", &global_var);

  // This variable is in the heap segment
  printf("heap_var is at address 0x%08x\n\n", heap_var_ptr);

  // These variables are in the stack segment
  printf("stack_var is at address 0x%08x\n", &stack_var);
  function();
}

```

```bash

gcc -o segments ./code/10-memory_segments.c
./segments

#  global_initialized_var is at address 0xd9d80040
#  static_initialized_var is at address 0xd9d80044
#
#  static_var is at address 0xd9d80050
#  global_var is at address 0xd9d8004c
#
#  heap_var is at address 0x077a42a0
#
#  stack_var is at address 0xce68b5d4
#  the function's stack_var is at address 0xce68b5bc

```

---

```









        This Slide is intentionally left blank








```

---

## Memory Management Outside a Process

### Basic terminologies

- **Page**: memory blocks. 4k by default
- **Virtual Memory**: Process address space
- **Paging**: fetching memory from secondary to primary storage
- **Swap**: Alternate space for inactive allocated memory
- **Translation Lookaside Buffer**: Cache for page translation from virtual to physical address
- **Cache**: CPU cache
- **Page Cache**: Recently used memory pages

```


          ---------   0x00000000
         |         |
         |  TEXT   |                                  Physical memory
         |         |
          ---------                                    ---------   0x00000000
         |         |                                  |/////////|
         |  DATA   |                                  |---------|
         |         |                                  |/////////|     Reserved for Kernel, Page cache, TLB, Buffer
          ---------                                   |---------|
         |         |                                  |         |        A
         |   BSS   |                                  |---------|       /|\
         |         |                                  |         |        |
          ---------                                   |---------|        |
         |         |                                  |         |        |
         |  HEAP   |                                  |---------|        |
         |         |                                  |         |        |
         |    |    |                                  |---------|        |
         |    |    |                                  |         |        |
         |   \|/   |                                  |---------|        |
         |    V    |                                  |         |        |    Page frames for processes
         |         |                                  |---------|        |
         |         |                                  |         |        |
         |         |                                  |---------|        |
         |         |                                  |         |        |
         |         |                                  |---------|        |
         |         |                                  |         |        |
         |         |                                  |---------|        |
          ---------                                   |         |       \|/
         |    A    |                                   ---------         V
         |   /|\   |
         |    |    |
         |    |    |
         |         |
         |  STACK  |
          ---------   0x7ffff000

```

---

### Allocating memory for a process

1.  Kernel Allocates the process memory inside the RAM along with the Virtual Memory and page cache setup
1.  Process instructions are moved to RAM in blocks called Page from Hard Drive via Page table
1.  To execute the process, CPU reads the machine instructions from RAM
1.  CPU manages the instructions cache for faster instruction execution
1.  During the process, if User interacts and modifies any data, than that data is marked as dirty and will be written if a process ends or the data is re-read
1.  Once the Program is completed, Kernel deallocates the process memory

### Interaction between different components

```

                                    ---------
                                   |  Users  |
                                    ---------
                                      |  A
                                      | /|\
                                      |  |
                                      |  |
                                      |  |
                                     \|/ |
                                      V  |

  -------------                    ---------     Dirty Cache     ---------
 |  Processes  | ---------------> |   RAM   | ---------------->  |   HDD   |
  -------------                    ---------  <-----------------  ---------
                                       |           page cache
                                       |
                                       |
                                       |
                                       |
                                      \|/
                                       V

                                    ---------
                                   |  CPU    |    ----> L1, L2, L3 cache
                                    ---------



```

### How it looks

#### Process information

1. `/proc/meminfo` : It displays the memory information of the system including the current page table size, total virtual memory available for each process, total physical memory available and a lot more.
1. On running `top` command, under `VIRT` column, it shows how much virtual memory has been allocated to the process. Most of the time it is 100 times more than the actual RAM available. But it is possible because not all the allocated memory is being used at the same time.

```bash

# Assuming 1234 is the process id
cat /proc/meminfo  # VmallocTotal, PageTable


top # Virt, Res

# specific process
cat /proc/1234/cmdline # show command line of Process
cat /proc/1234/environ # show environment variables of Process
cat /proc/1234/fd/4 # show file descriptor of Process, 4 is file descriptor
cat /proc/1234/status  # show status of Process

```

This program will show the changes in virtual memory of the process. In case when kernel can't allocate more memory, it will kill the process with `out of memory kill`.

---

#### Memory eater

```c
// filename: 12-memory-eater.c

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char* argv[]) {
    int *p;
    unsigned long int total_size = 0;
    int to_allocate = 1024 * 1024;
    int multiplier = 1;

    if (argc == 2) {
      to_allocate *= atoi(argv[1]);
    }

    while(1) {
        p = malloc(to_allocate);
        if (p == NULL) {
            printf("Failed to allocate %d bytes\n", to_allocate);
            break;
        }
        total_size += to_allocate;
        if (total_size < 1024*1024*1024) {
            printf("Allocated %lu MB\n", total_size/(1024*1024));
        } else {
            printf("Allocated %lu GB\n", total_size/(1024*1024*1024));
        }
        sleep(1);
    }
    return 0;
}



```

```bash

gcc -o memeater ./code/12-memory-eater.c
./memeater &
top # observe memeater

```

---

#### Page Cache

Below command will show how the CPU cache affects the program runtime.

```bash

top # memory and cache uses
free -m  # Displays the available buffer/cache and swap space
echo 3 > /proc/sys/vm/drop_caches  # it does what it says


```

#### Active/Inactive Memory

This is important part to understand the performance of the system. If anonymous memory is high and swap space is low, the system will most likely get slow.

- Anon - memory used by processes. Gets swaps out
- file - buffers and caches. Gets removed

```bash

    grep -i active /proc/meminfo
    vmstat 2 5  # show virtual memory stat 5 times with 2 second interval. See si, so column
    swapon -s  # list devices used for swap
    echo 90 > /proc/sys/vm/swapiness  # tune the swapping activity

```

---

#### Cache Friendly code

This code shows the internal arrangement of an array and how the runtime of the process is affected by the way we access the array locations.

1. Arrays are stored in row major order, so it is efficient to access the array with row elements together.
1. When a memory is accessed, kernel not only copies the requested address but also a fixed block of memory location that includes the requested address. It means, that if a row element is accessed, kernel will most likely copy the whole row, but it is not the same for columns.

```c
// filename: 11-cache-friendly-code.c

#include <stdio.h> // For printf function
#include <stdlib.h> // For heap memory functions
#include <string.h> // For strcmp function

void fill(int* matrix, int rows, int columns) {
  int counter = 1;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      *(matrix + i * columns + j) = counter;
    }
    counter++;
  }
}

void print_matrix(int* matrix, int rows, int columns) {
  int counter = 1;
  printf("Matrix:\n");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      printf("%d ", *(matrix + i * columns + j));
    }
    printf("\n");
  }
}

void print_flat(int* matrix, int rows, int columns) {
  printf("Flat matrix: ");
  for (int i = 0; i < (rows * columns); i++) {
    printf("%d ", *(matrix + i));
  }
  printf("\n");
}

int friendly_sum(int* matrix, int rows, int columns) {
  int sum = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      sum += *(matrix + i * columns + j);
    }
  }
  return sum;
}

int not_friendly_sum(int* matrix, int rows, int columns) {
  int sum = 0;
  for (int j = 0; j < columns; j++) {
    for (int i = 0; i < rows; i++) {
      sum += *(matrix + i * columns + j);
    }
  }
  return sum;
}

int main(int argc, char** argv) {
  if (argc < 4) {
    printf("Usage: %s [number-of-rows] [number-of-columns] [print|friendly-sum|not-friendly-sum]\n", argv[0]);
    exit(1);
  }
  int rows = atol(argv[1]);
  int columns = atol(argv[2]);
  char* operation = argv[3];
  int* matrix = (int*)malloc(rows * columns * sizeof(int));
  fill(matrix, rows, columns);
  if (strcmp(operation, "print") == 0) {
    print_matrix(matrix, rows, columns);
    print_flat(matrix, rows, columns);
  }
  else if (strcmp(operation, "friendly-sum") == 0) {
    int sum = friendly_sum(matrix, rows, columns);
    printf("Friendly sum: %d\n", sum);
  }
  else if (strcmp(operation, "not-friendly-sum") == 0) {
    int sum = not_friendly_sum(matrix, rows, columns);
    printf("Not friendly sum: %d\n", sum);
  }
  else {
    printf("FATAL: Not supported operation!\n");
    exit(1);
  }
  free(matrix);
  return 0;
}



```

On running the below scenarios, we can see that `friendly-sum` will take less time than `not-friendly-sum`.

```bash

gcc -O0 -o cache-code ./code/11-cache-friendly-code.c
./cache-code 2 3 print
time ./cache-code 2 3 friendly-sum
time ./cache-code 2 3 not-friendly-sum

```

---

### Improve Cache hits

Since each CPU creates their own cache, it is efficient to run a process on the same CPU for its lifetime. Below commands can be used to pin a process to a single CPU. Once done, that process will always run on the assigned CPU if it is online. But if that CPU is disabled, than other CPUs will be used just like in normal scenario

```bash

dd if=/dev/zero of=/dev/null & # run in background
top # Select field 'last used CPU' with f and press j to sort by it

taskset -p 3 $(pidof dd) # pin to CPU 0
echo 0 > /sys/bus/cpu/devices/cpu3/online # disable CPU 3
lscpu # check CPU 3 is offline
top # check if dd is still running on CPU 3

```

---

## References

1. [Linux under the hood, Pearson](https://www.oreilly.com/library/view/linux-under-the/9780134663500/)
1. [Extreme C, Packt Publishing](https://www.oreilly.com/library/view/extreme-c/9781789343625/)
1. [Hacking: The Art of Exploitation, 2nd Edition, No Starch Press](https://learning.oreilly.com/library/view/hacking-the-art/9781593271442/)
1. [Procfs documentation](https://docs.kernel.org/filesystems/proc.html)

---

## Epilogue

```

    Questions?
    Feedback?
    Suggestions for Next topic?

```

```

    Thank You

```

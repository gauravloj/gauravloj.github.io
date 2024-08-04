---
title: Memory Management
time: 2024-08-01 18:51:08
categories: [Talks, year-2024]
tags: [linux, memory management, c, optimization]
---

```



░  ░░░░  ░░        ░░  ░░░░  ░░░      ░░░       ░░░  ░░░░ ░░
▒   ▒▒   ▒▒  ▒▒▒▒▒▒▒▒   ▒▒   ▒▒  ▒▒▒▒  ▒▒  ▒▒▒▒  ▒▒▒  ▒▒  ▒▒
▓        ▓▓      ▓▓▓▓        ▓▓  ▓▓▓▓  ▓▓       ▓▓▓▓▓    ▓▓▓
█  █  █  ██  ████████  █  █  ██  ████  ██  ███  ██████  ████
█  ████  ██        ██  ████  ███      ███  ████  █████  ████



░░  ░░░░  ░░░      ░░░   ░░░  ░░░      ░░░░      ░░░        ░░  ░░░░  ░░        ░░   ░░░  ░░        ░
▒▒   ▒▒   ▒▒  ▒▒▒▒  ▒▒    ▒▒  ▒▒  ▒▒▒▒  ▒▒  ▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒▒▒   ▒▒   ▒▒  ▒▒▒▒▒▒▒▒    ▒▒  ▒▒▒▒▒  ▒▒▒▒
▓▓        ▓▓  ▓▓▓▓  ▓▓  ▓  ▓  ▓▓  ▓▓▓▓  ▓▓  ▓▓▓   ▓▓      ▓▓▓▓        ▓▓      ▓▓▓▓  ▓  ▓  ▓▓▓▓▓  ▓▓▓▓
██  █  █  ██        ██  ██    ██        ██  ████  ██  ████████  █  █  ██  ████████  ██    █████  ████
██  ████  ██  ████  ██  ███   ██  ████  ███      ███        ██  ████  ██        ██  ███   █████  ████



```

---

## Memory Management Categories

```
1. Static Memory Allocation
2. Dynamic memory Allocation
3. Process Memory Allocation
```

---

## How a process manages its memory

### Memory Layout of Running process

```
->

    Basic segments of a process
    1. Text
    2. Data
    3. BSS
    4. Heap
    5. Stack


->
```

<!--stop-->

```
->


          ---------   0x00000000
         |         |
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

->
```

---

## Probing Static Memory

### Text Segment

Code for 01-text.c

```file
path: ./code/01-text.c
lang: c
```

Checking the size of different segments

```sh
-

gcc -c -o text.o ./code/01-text.c
size -m text.o  # macos
size text.o  # linux

otool -s __TEXT __text -v text.o   # macos
objdump -S -j .text text.o  # linux

-
```

<!--stop-->

### BSS Segment

Code for 02-bss.c

```file
path: ./code/02-bss.c
lang: c
```

Checking the size of different segments

```sh
-

gcc -c -o bss.o ./code/02-bss.c
size -m bss.o

# content of BSS segment
objdump -s -j .bss bss.o # Linux
otool -s __DATA __common bss.o # macos

-
```

---

## Probing Static Memory

### Data Segment

Code for 03-data-1.c

```file
path: ./code/03-data-1.c
lang: c
```

Checking the size of different segments

```sh
-

gcc -c -o data1.o ./code/03-data-1.c
size -m data1.o

# content of Data segment
otool -s __DATA __data data1.o   # macos
objdump -s -j .data data1.o      # Linux

-
```

<!--stop-->

Code for 04-data-2.c

```file
path: ./code/04-data-2.c
lang: c
```

Checking the size of different segments

```sh
-

gcc -c -o data2.o ./code/04-data-2.c
size -m data2.o

# content of Data segment
otool -d data2.o   # macos
objdump -s -j .data data2.o      # Linux
objdump -d data2
objdump -t data2

-
```

---

# How a process manages its memory

## Memory Layout of Running process

```
->

    Basic segments of a process
    1. Text
    2. Data
    3. BSS
    4. Heap
    5. Stack


->
```

```
->


          ---------   0x00000000
         |         |
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

->
```

---

## Probing Dynamic Memory

### Memory Mapping

Output of memory mapping in procfs

1. Address range - start address to end address of mapped memory
1. Permission - rwxps - read, write, execute, private, shared
1. Offset - offset of mapped memory in file, 0 for heap
1. Device - major:minor if mapped memory is file, 00:00 for heap
1. Inode - inode number if mapped memory is file, 0 for heap
1. Pathname - path of mapped memory if mapped memory is file, 'heap' for heap, 'stack' for stack

Demo code 05-mapping-stack.c

```file
path: ./code/05-mapping-stack.c
lang: c
```

```bash
-

gcc -o stackmap 05-mapping-stack.c
./stackmap &  # return PID

# Assume PID is 1234

# /proc is a virtual file system
ls -l /proc/1234

# Memory map of process
cat /proc/1234/maps

-
```

<!--stop-->

Demo code 06-mapping-heap.c

```file
path: ./code/06-mapping-heap.c
lang: c
```

```bash
-

gcc -o heapmap 06-mapping-heap.c
./heapmap &  # return PID

# Assume PID is 1234

# Memory map of process
cat /proc/1234/maps

-
```

---

## Probing Dynamic Memory

### Probing Stack Segment

Demo code ./code/07-stack.c

```file
path: ./code/07-stack.c
lang: c
```

```bash
-

gcc -g -o stack 07-stack.c
gdb -q ./stack

# Optionally, connect to a remote process
# gdb -tui -p 1234 /path/to/binary

# inside gdb
break main
run
next
step
continue
info registers
x/4xw $rsp
print arr
x/4xw arr
set {char}0x7fffffffe2e0 = 'A' # set memory at address 0x7fffffffe2e0 to 'A'
set arr[1] = 'F' # set arr[1] to 'F'

-
```

<!--stop-->

Demo code ./code/08-stack-overflow.c

```file
path: ./code/08-stack-overflow.c
lang: c
```

```bash
-

gcc -g -o stackoverflow ./code/08-stack-overflow.c
./stackoverflow

-
```

---

## Probing Dynamic Memory

### Probing Heap Segment

Demo code ./code/09-heap.c

```file
path: ./code/09-heap.c
lang: c
```

```bash
-

gcc -g -o heap 09-heap.c
gdb -q ./heap

-
```

---

## All at once

```file
path: ./code/10-memory_segments.c
lang: c
```

```bash
-

gcc -o segments ./code/10-memory_segments.c
./segments

-
```

---

```
-









        This Slide is intentionally left blank








-
```

---

# Outside a Process

## Basic terminologies

- Page: memory blocks. 4k by default
- Virtual Memory: Process address space
- Paging: fetching memory from secondary to primary storage
- Swap: Alternate space for inactive allocated memory
- Translation Lookaside Buffer: Cache for page translation from virtual to physical address
- Cache: CPU cache
- Page Cache: Recently used memory pages

<!--stop-->

```
->


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

->
```

---

## Allocating memory for a process

1.  Kernel Allocates the process memory inside the RAM along with the Virtual Memory and page cache setup
1.  Process instructions are moved to RAM in blocks called Page from Hard Drive via Page table
1.  To execute the process, CPU reads the machine instructions from RAM
1.  CPU manages the instructions cache for faster instruction execution
1.  During the process, if User interacts and modifies any data, than that data is marked as dirty and will be written if a process ends or the data is re-read
1.  Once the Program is completed, Kernel deallocates the process memory

## Interaction between different components

```
-
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



-
```

<!--stop-->

## How it looks

```bash
-

    # Assuming 1234 is the process id
    cat /proc/meminfo  # VmallocTotal, PageTable
    top # Virt, Res

    # specific process
    cat /proc/1234/cmdline # show command line of Process
    cat /proc/1234/environ # show environment variables of Process
    cat /proc/1234/fd/4 # show file descriptor of Process, 4 is file descriptor
    cat /proc/1234/status  # show status of Process

-
```

---

## Memory eater

```file
path: ./code/12-memory-eater.c
lang: c
```

```bash
-

gcc -o memeater ./code/12-memory-eater.c
./memeater
top # observe memeater

-
```

---

## Page Cache

```bash
-

    top # memory ancd cache uses
    free -m
    echo 3 > /proc/sys/vm/drop_caches  # it does what it says


-
```

<!--stop-->

## Active/Inactive Memory

- Anon - memory used by processes. Gets swaps out
- file - buffers and caches. Gets removed

```bash
-
    grep -i active /proc/meminfo
    vmstat 2 5  # show virtual memory stat 5 times with 2 second interval. See si, so column
    swapon -s  # list devices used for swap
    echo 90 > /proc/sys/vm/swapiness  # tune the swapping activity
-
```

---

## Cache Friendly code

```file
path: ./code/11-cache-friendly-code.c
lang: c
```

```bash
-
gcc -O0 -o cache-code ./code/11-cache-friendly-code.c
./cache-code 2 3 print
time ./cache-code 2 3 friendly-sum
time ./cache-code 2 3 not-friendly-sum

-
```

---

## Improve Cache hits

Process pinning

```bash
-

    dd if=/dev/zero of=/dev/null & - run in background
    top - Select field 'last used CPU' with f and press j to sort by it

    taskset -p 3 $(pidof dd) - pin to CPU 0
    echo 0 > /sys/bus/cpu/devices/cpu3/online - disable CPU 3
    lscpu - check CPU 3 is offline
    top - check dd is still running on CPU 3

-
```

---

## References

1. Linux under the hood, Pearson - https://www.oreilly.com/library/view/linux-under-the/9780134663500/
1. Extreme C, Packt Publishing - https://www.oreilly.com/library/view/extreme-c/9781789343625/
1. Hacking: The Art of Exploitation, 2nd Edition, No Starch Press - https://learning.oreilly.com/library/view/hacking-the-art/9781593271442/
1. Procfs documentation - https://docs.kernel.org/filesystems/proc.html

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

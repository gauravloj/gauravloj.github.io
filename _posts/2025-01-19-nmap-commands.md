---
title: Nmap Commands
time: 2025-01-19 21:55:16
categories: [Cyber Security, CTFs]
tags: [reconnaissance, nmap, enumeration, host discovery]
---


## Syntax

```sh

# basic syntax
nmap [options] <iplist>

# Ways to provide IP List:

# 1. list
nmap ip1 ip2 ip2

# 2. range
nmap 10.10.10.10-30
nmap 10.10.0-255.101-125

# 3. CIDR
nmap 10.10.10.0/24
```

## Host Discovery

```sh
# List scan targets:
nmap -sL <iplist>

# Only discover host, no port scan :
nmap -sn <iplist>

# Reverse DNS lookup for offline hosts, (providing dns server is optional) - 
nmap -R --dns-servers DNS_SERVER <iplist>

# no DNS lookup
nmap -n <IP list>

# Scan host that appears to be down: 
nmap -Pn <iplist>

# ARP scan (same subnet)
nmap -PR <iplist>

# ICMP echo (ping) scan
nmap -PE <iplist>

 # ICMP timestamp scan (ping scan might be blocked)
nmap -PP <iplist>

 # ICMP address mask scan (ping scan might be blocked)
nmap -PM <iplist>

# TCP SYN scan
nmap -PS[portlist] <iplist>

# TCP ACK scan
nmap -PA[portlist] <iplist>

# UDP scan
nmap -PU[portlist] <iplist>
```

## Port Scan

### Specifying ports
By default nmap scans top 1000 ports.

```sh
# specify `-F` to scan top 100 ports instead of 1000.
nmap -F <iplist>

# Define custom number for top ports to scan
nmap --top-ports 10 <iplist>

# scan port 22
nmap -p22 <iplist>

# scan ports 22, 80 nd 443
nmap -p22,80,443 <iplist>

# scan ports from 10 to 100
nmap -p10-100 <iplist>

# scan all the ports
nmap -p- <iplist>

```

### Scan Timing
Time delay between each scan request

| Timing | Total | Duration |
| ------ | ----- | -------- |
| T0 | (paranoid)  | 9.8 hours |
| T1 | (sneaky) | 27.53 minutes |
| T2 | (polite) | 40.56 seconds |
| T3 | (normal) | 0.15 seconds |
| T4 | (aggressive) | 0.13 seconds |
| T5 | (insane) |   |

```sh
# Example:
nmap -T0 -sn <iplist>

# or 
nmap -T paranoid  <iplist>

```

### Packet rate
Setting the number of packet sent per second:

```sh
# Setting minimum rate:
nmap --min-rate <number> <iplist>

# Setting maximum rate:
nmap --max-rate <number> <iplist>
```

### Setting parallel requests
Setting number of scan requests (host discovery/port scan) in parallel:

```sh
# Setting minimum open probes:
nmap --min-parallelism <numprobes> <iplist>

# Setting maximum open probes:
nmap --max-parallelism <numprobes> <iplist>
```

### Setting host timeout

Setting Maximum amount of time to wait for a target host

```sh
nmap —host-timeout <time in seconds> <iplist>
nmap —host-timeout 900ms <iplist>
```


## Basic Scan types

```sh
# TCP connect scan, complete TCP 3-way handshake (SYN,SYN/ACK, ACK)
nmap -sT <iplist>

# TCP SYN scan, send the SYN flag and reset the connection without 
# completing the 3-way handshake:
nmap -sS <iplist>

# UDP scan:
nmap -sU <iplist>

```

## Advanced Scan types

```sh
# null scan, no flags are set in TCP header,
# slow because no response indicates an open port
nmap -sN <iplist>

# FIN scan, set FIN flag in TCP header, 
# slow because no response indicates an open port
nmap -sF <iplist>

# XMAS scan, set FIN, PSH, URGENT flag in TCP header, 
# slow because no response indicates an open port
nmap -sX <iplist>

# ACK scan, set ACK flag in TCP header.
# It will return RST flag for both open and closed ports,
# but won’t respond if the port is blocked by firewall.
# Useful to know firewall rules
nmap -sA <iplist>

# Window scan, set ACK flag in TCP header.
# It will return RST flag for both open and closed ports,
# but might respond differently when the port is blocked by firewall.
# Useful to know firewall rules
nmap -sW <iplist>

# custom combination of TCP flags,
# TCP flags can be a concatenated string of 
# these flags: URG, ACK, PSH, RST, SYN, FIN
# Eg. --scanflags ACKPSH
nmap --scanflags <TCP flags> <iplist>

```


## Manipulating Request

```sh
# Spoofing IP, response will go to the spoofed IP instead of our own IP.
nmap -S <spoofed IP> <iplist>

# Supporting flags along with spoofing IP.
# Useful only when we can monitor the network data
nmap -e NET_INTERFACE -Pn -S SPOOFED_IP <iplist>

# Spoof MAC address if the target is on the same subnet.
nmap --spoof-mac SPOOFED_MAC <iplist>

# Send request with decoy IPs to avoid our IP getting flagged as malicious IP.
nmap -D 10.10.0.1,10.10.0.2,RND,RND,ME <iplist>

# fragment the request in 8 bytes
nmap -sS -p80 -f 10.20.30.144

# fragment the request in 16 bytes
nmap -sS -p80 -ff 10.20.30.144

# fragment the request in 32 bytes, custom value.
nmap -sS -p80 --mtu 32 10.20.30.144

# Custom data size
nmap -sS -p80 --data-length NUM 10.20.30.144

# Instead of spoofing IP where we need to monitor the network traffic,
# we can spoof IP of an idle host and monitor the traffic (IP identification number) on that host.
nmap -sI ZOMBIE_IP <iplist>

```

## Getting extra information

```sh
# enable log verbosity
nmap -v <iplist>

# increase the verbosity level
nmap -vv <iplist>

# enable debug mode
nmap -d <iplist>

# increase debug level
nmap -dd <iplist>

# Display a reason for concluding the state of a port
nmap --reason <iplist>

```

## Post scan activities
### Version detection:
Low version intensity has lower chances of identifying the version

```sh
# Identify service version
nmap -sV <iplist>

# Version intensity 2
nmap -sV --version-light <iplist>

# version intensity 9
nmap -sV --version-all <iplist>

# specify version intensity between 0 to 9
nmap -sV --version-intensity LEVEL <iplist>

```
### OS Detection
```sh
# detect OS and other relevant details like version, kernel
sudo nmap -sS -O <iplist>

```

### Traceroute
```sh
# find routers between source and target machine
sudo nmap -sS --traceroute <iplist>

```

### Output formats

```sh
# Normal output
nmap -oN <filename> <iplist>

# - XML output
nmap -oX <filename> <iplist>

# - grep-able output (useful for grep and awk)
nmap -oG <filename> <iplist>

# - Output in all major formats
nmap -oA <basename> <iplist>

```

### Nmap Scripts
  - All the scripts are located at `/usr/share/nmap/scripts`
  - Other categories include auth, broadcast, brute, default, discovery, dos, exploit, external, fuzzer, intrusive, malware, safe, version, and vuln

```sh
# run scripts from default category
# https://nmap.org/nsedoc/categories/default.html
nmap -sS -sC <iplist>

# Run script from other categories
nmap -sS --script=version <iplist>

# run the nmap script by script name
nmap -sS --script <script name> <iplist>

```


## Useful examples:

```sh
# find versions and vulnerabilities
nmap -sV --script vuln  10.10.198.61 

# Enumerate Samba shares and usernames
nmap -p 445 --script=smb-enum-shares.nse,smb-enum-users.nse 10.10.165.133

# Mount information for NFS filesystem
nmap -p 111 --script=nfs-ls,nfs-statfs,nfs-showmount 10.10.165.133
```


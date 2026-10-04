
# Architecture Diagram

This document describes the hardware and software subsystem interaction layers for the Linux System Monitor and Process Viewer project.

---

## System Architectural Diagram

```mermaid
graph TD
    User([User / Operator]) -->|Interacts with CLI Menu| CLI[C++ CLI Application main.cpp]
    
    subgraph User Space
        CLI -->|Reads /proc/stat, /proc/meminfo| ProcFS[/proc Pseudo Filesystem/]
        CLI -->|Reads /proc/PID/comm| ProcFS
        CLI -->|Executes statvfs| DiskAPI[POSIX statvfs System Call]
        CLI -->|Reads File /dev/sysmon| DevSysmon[/dev/sysmon Node/]
    end
    
    subgraph Kernel Space Boundary
        ProcFS -->|Kernel Info Provider| KernelProc[Kernel /proc Subsystem]
        DiskAPI -->|Queries VFS| KernelVFS[Virtual File System]
        DevSysmon -->|Character Device Read| Driver[Linux Device Driver sysmon_driver.c]
    end

    subgraph Linux Kernel Core
        Driver -->|Calls num_online_cpus| KernelCPU[CPU Subsystem]
        Driver -->|Calls si_meminfo| KernelMem[Memory Management]
        Driver -->|Queries Boot Time| KernelTime[Timer Subsystem]
    end
```

---

## ASCII Architecture Overview

```
                   +-------------------+
                   |   User / Operator |
                   +---------+---------+
                             |
                             v
               +---------------------------+
               |    C++ CLI Application    |
               |        (main.cpp)         |
               +-------------+-------------+
                             |
       +---------------------+---------------------+
       |                     |                     |
       v                     v                     v
+--------------+     +---------------+     +---------------+
| /proc Filesystem   |  statvfs("/")  |     |  /dev/sysmon  |
| (CPU, Memory,      | (Disk Space   |     | (Misc Device  |
|  Processes)        |  Information) |     |  Node)        |
+--------------+     +---------------+     +-------+-------+
                                                   |
===================================================|====================
                                KERNEL BOUNDARY    |
===================================================|====================
                                                   v
                                           +---------------+
                                           | sysmon_driver |
                                           | (Kernel Mod)  |
                                           +-------+-------+
                                                   |
                                                   v
                                           +---------------+
                                           | Linux Kernel  |
                                           |  Subsystems   |
                                           +---------------+
```

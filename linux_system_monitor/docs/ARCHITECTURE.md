# Architecture

## 1. Overview

The project follows a simple layered architecture.

```text
+--------------------------------+
|          User / Terminal       |
+---------------+----------------+
                |
                v
+--------------------------------+
|       C++ User Application     |
|                                |
| CPU | Memory | Disk | Process  |
+---------+---------------+------+
          |               |
          v               v
      /proc filesystem   /dev/sysmon
          |               |
          |               v
          |       +---------------+
          |       | Linux Kernel  |
          |       | Device Driver |
          |       +---------------+
          |
          v
    Linux Kernel Data
```

## 2. User Space

`src/main.cpp` runs in user space.

It provides the menu and collects system information.

## 3. Linux /proc

The application reads:

- `/proc/stat` for CPU information
- `/proc/meminfo` for memory information
- `/proc/<PID>/comm` for process names

`/proc` is a virtual filesystem provided by Linux.

## 4. Disk Information

The application uses the Linux `statvfs()` system call to obtain disk space information.

## 5. Kernel Space

`driver/sysmon_driver.c` is a Linux kernel module.

The module registers a miscellaneous device called:

```text
sysmon
```

Linux creates:

```text
/dev/sysmon
```

## 6. Communication

The user application opens:

```text
/dev/sysmon
```

and reads information returned by the driver's `read` function.

This demonstrates basic user-space and kernel-space communication.

## 7. Software Architecture Concepts Demonstrated

- Layered architecture
- Modular design
- User-space / kernel-space separation
- Interface through a device file
- Separation of monitoring functions

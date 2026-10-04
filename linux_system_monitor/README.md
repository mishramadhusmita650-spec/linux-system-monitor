# Linux System Monitor and Process Viewer

## 1. Project Overview

This project is a Linux-based system monitoring tool developed using **C++ and a Linux Kernel Module written in C**.

The application displays basic system information:

- CPU usage
- Memory usage
- Disk usage
- Running processes
- Information received from a Linux device driver

The project demonstrates:

- C++ programming
- Linux `/proc` virtual filesystem
- Linux system calls/APIs
- Process information
- Linux kernel module/device driver concepts
- User-space and kernel-space communication

> **Important:** This project must be built and executed on a Linux system. The kernel module requires Linux kernel headers matching the running kernel.

---

## 2. Technologies Used

- C++17
- C
- Linux
- Linux `/proc` filesystem
- Linux Kernel Module
- Make
- GCC/G++

No Python or Java is used.

---

## 3. Project Structure

```text
linux_system_monitor/
│
├── README.md
│
├── src/
│   ├── main.cpp
│   └── Makefile
│
├── driver/
│   ├── sysmon_driver.c
│   └── Makefile
│
└── docs/
    └── ARCHITECTURE.md
```

---

## 4. System Architecture

```text
+-------------------------------+
|        User / Terminal        |
+---------------+---------------+
                |
                v
+-------------------------------+
|       C++ Monitor App         |
|                               |
| CPU | Memory | Disk | Process |
+---------------+---------------+
                |
        +-------+-------+
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
 Linux Kernel Information
```

---

# 5. Requirements

Use a Linux system such as Ubuntu.

Install the required packages:

```bash
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r)
```

Check:

```bash
g++ --version
gcc --version
make --version
uname -r
```

---

# 6. Build the C++ Application

Open the project directory:

```bash
cd linux_system_monitor/src
```

Compile:

```bash
make
```

Run:

```bash
./sysmon
```

---

# 7. Build the Linux Device Driver

Open another terminal or go to the driver directory:

```bash
cd linux_system_monitor/driver
```

Build:

```bash
make
```

This creates:

```text
sysmon_driver.ko
```

---

# 8. Load the Driver

Run:

```bash
sudo insmod sysmon_driver.ko
```

Check:

```bash
lsmod | grep sysmon
```

The device should be available as:

```text
/dev/sysmon
```

Check:

```bash
ls -l /dev/sysmon
```

---

# 9. Run the Application

From the `src` directory:

```bash
./sysmon
```

Select:

```text
5. Read Device Driver
```

The program reads information from `/dev/sysmon`.

---

# 10. Remove the Driver

When finished:

```bash
sudo rmmod sysmon_driver
```

Check:

```bash
lsmod | grep sysmon
```

---

# 11. Program Menu

```text
================================
       LINUX SYSTEM MONITOR
================================

1. CPU Usage
2. Memory Usage
3. Disk Usage
4. List Processes
5. Read Device Driver
0. Exit
```

---

# 12. Feature Explanation

## CPU Usage

The program reads `/proc/stat`.

Linux provides CPU time information through this file.

The program reads CPU values twice with a short delay and calculates approximate CPU usage.

## Memory Usage

The program reads `/proc/meminfo`.

It obtains:

- Total memory
- Available memory

Then it calculates used memory.

## Disk Usage

The program uses the Linux `statvfs()` system call to check disk space for `/`.

## Process List

The program reads the `/proc` directory.

Numeric directory names represent process IDs.

For example:

```text
/proc/1
/proc/100
/proc/500
```

The program reads the process name from:

```text
/proc/<PID>/comm
```

## Device Driver

The kernel module creates:

```text
/dev/sysmon
```

The C++ application opens this device and reads system information supplied by the driver.

The driver gets basic information from the Linux kernel:

- CPU count
- Total memory
- Available memory
- System uptime

---

# 13. Testing

Test CPU:

```text
Select 1
```

Test memory:

```text
Select 2
```

Test disk:

```text
Select 3
```

Test processes:

```text
Select 4
```

Load the driver:

```bash
sudo insmod driver/sysmon_driver.ko
```

Then test:

```text
Select 5
```

---

# 14. GitHub Submission

Recommended repository name:

```text
linux-system-monitor
```

Upload:

```text
README.md
src/main.cpp
src/Makefile
driver/sysmon_driver.c
driver/Makefile
docs/ARCHITECTURE.md
```

Do not upload compiled files such as:

```text
sysmon
sysmon_driver.ko
*.o
```

The `.gitignore` file can be added if required.

---

# 15. 5–10 Minute Demonstration

### Step 1 – Introduction

Say:

> "My project is a Linux System Monitor and Process Viewer developed using C++ and a Linux kernel module written in C."

### Step 2 – Explain architecture

Say:

> "The project has a user-space C++ application and a kernel-space device driver. The application gets system information from the Linux `/proc` filesystem and communicates with the device driver through `/dev/sysmon`."

### Step 3 – Demonstrate CPU

Select:

```text
1
```

Explain:

> "CPU information is obtained from `/proc/stat`."

### Step 4 – Demonstrate memory

Select:

```text
2
```

Explain:

> "Memory information is obtained from `/proc/meminfo`."

### Step 5 – Demonstrate disk

Select:

```text
3
```

Explain:

> "Disk information is obtained using the Linux statvfs system call."

### Step 6 – Demonstrate processes

Select:

```text
4
```

Explain:

> "Linux represents processes using directories inside `/proc`. I identify numeric directories as process IDs and read their names."

### Step 7 – Demonstrate driver

Show:

```bash
sudo insmod driver/sysmon_driver.ko
```

Then:

```bash
ls -l /dev/sysmon
```

Run the application and select:

```text
5
```

Explain:

> "This option reads information from the Linux device driver through `/dev/sysmon`."

---

# 16. Important Concepts to Know for Evaluation

Be prepared to explain:

### What is `/proc`?

`/proc` is a virtual filesystem provided by Linux. It exposes information about processes and the running system.

### What is a process?

A process is a running instance of a program.

### What is a PID?

PID means Process ID. Linux assigns a unique ID to a running process.

### What is a device driver?

A device driver is software that allows the operating system and applications to interact with hardware or kernel-level functionality.

### What is a kernel module?

A kernel module is code that can be loaded into and removed from the Linux kernel while the system is running.

### What is `/dev/sysmon`?

It is the device file created by the project driver. The user-space application reads information from this device.

### User space vs kernel space

The C++ application runs in user space.

The device driver runs in kernel space.

### Why Linux?

Linux provides `/proc`, system calls, device files and kernel module support, making it suitable for demonstrating system-level programming.

---

# 17. Future Enhancements

Possible future improvements:

- Real-time monitoring
- Network monitoring
- CPU temperature monitoring
- Process termination
- Graphical interface
- Logging system statistics
- More detailed driver functionality

---

# 18. Learning Outcomes

Through this project, the following concepts are demonstrated:

- C++ programming
- File handling
- Linux filesystem
- Process management concepts
- System calls
- Linux kernel modules
- Device files
- User-space/kernel-space communication
- Makefiles
- GitHub project organization

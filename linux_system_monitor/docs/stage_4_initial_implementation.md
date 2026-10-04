# Stage 4: Initial Implementation

## Application Implementation (`src/main.cpp`)

The user-space application is written in C++ using a simple function-based procedural design.

### Key Functions in `main.cpp`

1. **`void showCPU()`**
   - Opens `/proc/stat`.
   - Reads CPU state lines: `user`, `nice`, `system`, `idle`, `iowait`, `irq`, `softirq`.
   - Sums all fields to calculate `Total Jiffies` and isolates `Idle Jiffies`.
   - Pauses execution (`usleep` or `std::this_thread::sleep_for`).
   - Reads `/proc/stat` a second time.
   - Calculates differential usage:
     $$\Delta \text{Total} = \text{Total}_2 - \text{Total}_1$$
     $$\Delta \text{Idle} = \text{Idle}_2 - \text{Idle}_1$$
     $$\text{CPU Usage \%} = \frac{\Delta \text{Total} - \Delta \text{Idle}}{\Delta \text{Total}} \times 100$$

2. **`void showMemory()`**
   - Opens `/proc/meminfo`.
   - Parses text line-by-line using `std::ifstream`.
   - Extracts `MemTotal`, `MemFree`, and `MemAvailable`.
   - Computes Used RAM ($\text{Total} - \text{Available}$) and formats values into Megabytes.

3. **`void showDisk()`**
   - Includes `<sys/statvfs.h>`.
   - Invokes `statvfs("/", &stat)`.
   - Multiplies `f_blocks` and `f_bfree` by `f_frsize` to calculate Total and Used Gigabytes.

4. **`void showProcesses()`**
   - Opens `/proc` using `opendir("/proc")`.
   - Reads directory entries using `readdir()`.
   - Checks if entry name is numeric using `isdigit()`.
   - Constructs path `/proc/[PID]/comm`.
   - Reads process name string and prints PID + Process Name.
   - Truncates output to 15 processes.

5. **`void readDriver()`**
   - Attempts to open `/dev/sysmon`.
   - If device file cannot be opened, prints error message advising user to load module (`sudo insmod driver/sysmon_driver.ko`).
   - If successful, reads contents and displays system telemetry provided by kernel driver.

6. **`int main()`**
   - Contains main execution loop presenting interactive CLI selection menu:
     - 1: View CPU Usage
     - 2: View Memory Usage
     - 3: View Disk Usage
     - 4: View Active Processes
     - 5: View Kernel Driver Info (`/dev/sysmon`)
     - 6: Exit

---

## Kernel Driver Implementation (`driver/sysmon_driver.c`)

The kernel module is implemented in C using standard Linux kernel headers (`<linux/init.h>`, `<linux/module.h>`, `<linux/fs.h>`, `<linux/miscdevice.h>`, `<linux/uaccess.h>`).

### Structure and Logic
- **Device Details:** Registered as a `miscdevice` structure with name `"sysmon"`, minor number `MISC_DYNAMIC_MINOR`.
- **Read Operation Handler (`sysmon_read`):**
  - Invoked when user reads `/dev/sysmon`.
  - Ensures single-read behavior per open file handle using read offset checks (`*ppos > 0`).
  - Fetches core count using `num_online_cpus()`.
  - Fetches RAM metrics using `si_meminfo(&si)`.
  - Fetches uptime seconds using kernel uptime helpers.
  - Formats string into internal kernel buffer via `snprintf()`.
  - Transfers string to user-space buffer safely using `copy_to_user()`.
- **Initialization & Cleanup (`sysmon_init`, `sysmon_exit`):**
  - `misc_register(&sysmon_device)` registers character device on insertion.
  - `misc_deregister(&sysmon_device)` cleans up device node on removal.

---

## Makefiles

### User Space Makefile (`src/Makefile`)
```makefile
CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = sysmon_cli

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
```

### Driver Makefile (`driver/Makefile`)
```makefile
obj-m += sysmon_driver.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
```
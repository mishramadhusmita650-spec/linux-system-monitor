# Stage 3: System Design and Architecture

## Overall Architecture
The system architecture spans two primary execution spaces: **User Space** and **Kernel Space**. 

1. **User Space Application (`main.cpp`):** Executes with standard user permissions. Interacts directly with virtual filesystem entries (`/proc/stat`, `/proc/meminfo`, `/proc/[PID]/comm`), issues POSIX standard functions (`statvfs`), and opens character device nodes (`/dev/sysmon`).
2. **Kernel Space Module (`sysmon_driver.c`):** Executes in privileged kernel mode. Interacts directly with internal kernel subsystems (`sysinfo`, kernel uptime, core counter) and exposes these metrics through a `/dev/sysmon` node registered as a Linux miscellaneous character device.

---

## Subsystem Architecture & Interaction Data Flow

```
+-----------------------------------------------------------------------+
|                              USER SPACE                               |
|                                                                       |
|                     +---------------------------+                     |
|                     |    C++ CLI Application    |                     |
|                     |        (main.cpp)         |                     |
|                     +-------------+-------------+                     |
|                                   |                                   |
|       +---------------------------+---------------------------+       |
|       |                           |                           |       |
|       v                           v                           v       |
|  +---------+                 +---------+                 +---------+  |
|  |  show   |                 |  show   |                 |  read   |  |
|  | CPU/Mem |                 |  Disk   |                 | Driver  |  |
|  +----+----+                 +----+----+                 +----+----+  |
|       |                           |                           |       |
+-------|---------------------------|---------------------------|-------+
        |                           |                           |        
========|===========================|===========================|========
        |      KERNEL BOUNDARY      |                           |        
+-------|---------------------------|---------------------------|-------+
|       v                           v                           v       |
|  +---------+                 +---------+                 +---------+  |
|  |  /proc  |                 | statvfs |                 |  /dev/  |  |
|  | Filesys |                 | ("/")   |                 | sysmon  |  |
|  +---------+                 +---------+                 +----+----+  |
|                                                               |       |
|                                                               v       |
|                                                    +------------------+
|                                                    | sysmon_driver.c  |
|                                                    | (Misc Device)    |
|                                                    +--------+---------+
|                                                             |         |
|                                                             v         |
|                                                    +------------------+
|                                                    |  Linux Kernel    |
|                                                    |   Subsystems     |
|                                                    +------------------+
|                              KERNEL SPACE                             |
+-----------------------------------------------------------------------+
```

---

## Detailed Module Interactions

### 1. User-space Application & `/proc` Interface
- **CPU Monitoring (`showCPU()`):** Reads `/proc/stat`. Parses the first line (`cpu ...`). Reads user, nice, system, idle, iowait, irq, softirq times. Sleeps for 500ms and re-reads `/proc/stat`. Computes the delta of idle time against total time:
  $$\text{CPU Usage \%} = \left(1 - \frac{\Delta \text{Idle}}{\Delta \text{Total}}\right) \times 100$$
- **Memory Monitoring (`showMemory()`):** Parses `/proc/meminfo` line-by-line looking for key tags (`MemTotal:`, `MemFree:`, `MemAvailable:`). Formats values into Megabytes (MB).
- **Process Listing (`showProcesses()`):** Uses `<dirent.h>` to open directory `/proc`. Filters directories whose names consist purely of digits. For each PID, opens `/proc/[PID]/comm` to read the executable process name. Limits display to 15 entries.

### 2. User-space Application & `statvfs()`
- **Disk Monitoring (`showDisk()`):** Calls standard library function `statvfs("/", &statbuf)`. Computes:
  $$\text{Total Storage} = \text{f\_blocks} \times \text{f\_frsize}$$
  $$\text{Free Storage} = \text{f\_bfree} \times \text{f\_frsize}$$
  $$\text{Used Storage} = \text{Total Storage} - \text{Free Storage}$$

### 3. User-space Application & `/dev/sysmon` Interaction
- **Driver Query (`readDriver()`):** Opens `/dev/sysmon` using standard file streams or file descriptor calls (`std::ifstream` / `open`). Reads buffer generated by driver and outputs text directly to console.

### 4. Linux Device Driver (`sysmon_driver.c`)
- Registers a miscellaneous character device via `misc_register()`.
- Device Name: `sysmon` (creates `/dev/sysmon`).
- Implements `file_operations` callback `.read = sysmon_read`.
- When read by user-space:
  1. Queries kernel structure `struct sysinfo` via `si_meminfo()`.
  2. Queries kernel function `ktime_get_boottime_ts64()` or `get_jiffies_64()` for system uptime.
  3. Uses `num_online_cpus()` to get active core count.
  4. Formats data into a character buffer and copies to user-space memory via `copy_to_user()`.

---

## Structural UML Diagram References
Visual representations for architecture, sequence, logical components, and state transitions are maintained in the project's `diagrams/` folder:
- `diagrams/architecture.md`
- `diagrams/class-diagram.md`
- `diagrams/sequence-diagram.md`
- `diagrams/state-machine.md`
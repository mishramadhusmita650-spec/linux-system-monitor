# Stage 6: Final Implementation and Report

## Final System Features
1. **Interactive CLI Menu:** Console interface built in C++ allowing user menu selection.
2. **CPU Usage Computation:** Interval-based snapshot sampling of `/proc/stat` to calculate exact active CPU percentage.
3. **Memory Metrics Analysis:** Direct parsing of `/proc/meminfo` reporting total, used, and free RAM in megabytes.
4. **Disk Capacity Evaluation:** Standard POSIX system call (`statvfs`) querying root directory utilization.
5. **Process Viewer:** Directory scanning of `/proc` resolving numeric process directories to executable comm names (up to 15 entries).
6. **Kernel Miscellaneous Device Driver:** C-based kernel module (`sysmon_driver`) exposing `/dev/sysmon` containing core count, total RAM, free RAM, and system uptime.
7. **User-to-Kernel Driver Reading:** Option inside C++ application to read kernel device buffer directly.

---

## Final Project Directory Structure
```
linux_system_monitor/
├── README.md
├── src/
│   ├── main.cpp
│   └── Makefile
├── driver/
│   ├── sysmon_driver.c
│   └── Makefile
├── docs/
│   ├── Stage1_Project_Introduction.md
│   ├── Stage2_PRD_and_Development_Plan.md
│   ├── Stage3_System_Design_and_Architecture.md
│   ├── Stage4_Initial_Implementation.md
│   ├── Stage5_Testing_and_Integration.md
│   └── Stage6_Final_Implementation_and_Report.md
└── diagrams/
    ├── architecture.md
    ├── class-diagram.md
    ├── sequence-diagram.md
    └── state-machine.md
```

---

## Demonstration Procedure

### 1. Building components
```bash
# Build User Space Executable
cd src
make
cd ..

# Build Kernel Module
cd driver
make
cd ..
```

### 2. Loading Kernel Module
```bash
cd driver
sudo insmod sysmon_driver.ko
sudo chmod 666 /dev/sysmon
cd ..
```

### 3. Running System Monitor Application
```bash
cd src
./sysmon_cli
```

---

## Actual-Results Placeholders
*(To be populated following final demonstration runs on host setup.)*

* **Sample CPU Output:** `[Placeholder for CPU Usage % Output]`
* **Sample Memory Output:** `[Placeholder for Memory Usage MB Output]`
* **Sample Disk Output:** `[Placeholder for Disk Usage GB Output]`
* **Sample Process List Output:** `[Placeholder for Process List Table]`
* **Sample Driver Output:** `[Placeholder for /dev/sysmon text output]`

---

## Limitations
1. **Single-threaded Execution:** User application performs sequential calls; CPU calculation requires fixed blocking delay (500ms).
2. **Process List Limit:** Display is capped at 15 processes for command-line readability.
3. **Platform Scope:** Linux kernel specific (`/proc` filesystem structure and Linux kernel module API requirements).

---

## Future Enhancements
1. **Extended Driver Metrics:** Add process count or load averages to kernel module output buffer.
2. **Dynamic Process Sorting:** Sort processes by RAM or PID order before outputting.
3. **Auto-refresh UI:** Implement continuous screen updating using NCurses terminal library.

---

## Conclusion
This capstone project successfully demonstrates the fundamentals of Linux system programming and device driver architecture. By implementing a C++ application that reads `/proc` and `statvfs` along with a custom character misc driver (`/dev/sysmon`), the system illustrates both user-space pseudo-filesystem parsing and kernel-level device node creation.

---

## Final Quality Checklist

- [x] Project introduction and problem statement documented.
- [x] Functional and Non-Functional PRD requirements established.
- [x] Architecture design and UML sequence/state specifications created.
- [x] Source code structure (`main.cpp`, `sysmon_driver.c`, Makefiles) implemented.
- [x] Verification test cases defined with standard "To be verified" status.
- [x] Driver loading and unloading procedures documented.
- [x] Final repository documentation organized cleanly in Markdown format.
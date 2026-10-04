# Stage 2: PRD and Development Plan

## Product Requirement Document (PRD)

### 1. Functional Requirements
* **FR-01 (CPU Usage Calculation):** The application shall read `/proc/stat`, pause briefly to take a second snapshot, and compute active CPU percentage using the delta of total time vs. idle time.
* **FR-02 (Memory Statistics):** The application shall parse `/proc/meminfo` to determine Total RAM, Free RAM, and Available RAM, presenting values in readable megabytes/gigabytes.
* **FR-03 (Disk Space Statistics):** The application shall query `statvfs("/")` to extract total and used disk capacity for the root filesystem.
* **FR-04 (Process Enumeration):** The application shall scan the `/proc` directory, filter numeric directory names (representing PIDs), read `/proc/[PID]/comm`, and output a table listing up to 15 active processes.
* **FR-05 (Kernel Driver Information Service):** The custom kernel driver (`sysmon_driver.c`) shall expose CPU core count, total RAM, free RAM, and system uptime when `/dev/sysmon` is read.
* **FR-06 (Driver Integration in Application):** The C++ application shall provide an option to open `/dev/sysmon`, read its formatted string output, and render it to the console.
* **FR-07 (Interactive Console Menu):** The C++ application shall present an interactive terminal menu allowing the user to view individual metrics or exit the utility.

---

### 2. Non-Functional Requirements
* **NFR-01 (Portability):** The software shall compile on standard Linux distributions running modern 6.x or 5.x Linux kernels.
* **NFR-02 (Maintainability):** Source code shall follow structured function separation (`showCPU`, `showMemory`, `showDisk`, `showProcesses`, `readDriver`) and clear C kernel driver idioms.
* **NFR-03 (Build Simplicity):** Single-command build execution shall be provided through modular Makefiles in both `src/` and `driver/` directories.
* **NFR-04 (Robustness):** The C++ application shall handle missing `/dev/sysmon` node gracefully if the kernel module has not been inserted via `insmod`.

---

### 3. Software Requirements
* **Operating System:** Linux (Ubuntu/Debian, Fedora, or Arch-based distributions)
* **Compiler:** `g++` (supporting C++11 or later), `gcc` (for kernel module compilation)
* **Kernel Headers:** Installed Linux kernel headers matching the running kernel version (`linux-headers-$(shell uname -r)`)
* **Build Utility:** GNU Make (`make`)
* **Version Control:** Git

---

## Project Modules
The repository is divided into two primary logical modules:

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
└── diagrams/
```

1. **User-Space CLI Application (`src/`):** Contains `main.cpp` providing file parsing logic, system call interfaces, and the interactive menu interface.
2. **Kernel Space Device Driver (`driver/`):** Contains `sysmon_driver.c` implementing kernel structures (`file_operations`, `miscdevice`), module initialization, and file reading logic.

---

## 6-Stage Development Plan

| Stage | Title | Focus Area | Deliverables |
|---|---|---|---|
| **Stage 1** | Project Introduction | Definition of problem, objectives, and tech stack | `Stage1_Project_Introduction.md` |
| **Stage 2** | PRD & Development Plan | Detailed requirements and scheduling | `Stage2_PRD_and_Development_Plan.md` |
| **Stage 3** | System Design & Architecture | Structural and control diagrams, interface design | `Stage3_System_Design_and_Architecture.md` |
| **Stage 4** | Initial Implementation | Source code development for CLI & Driver | `Stage4_Initial_Implementation.md`, source files |
| **Stage 5** | Testing & Integration | Unit tests, driver load verification, test logs | `Stage5_Testing_and_Integration.md` |
| **Stage 6** | Final Implementation & Report | Documentation wrap-up, checklists, future scope | `Stage6_Final_Implementation_and_Report.md` |

---

## Project Deliverables
1. Source file `src/main.cpp` with executable build rule in `src/Makefile`.
2. Source file `driver/sysmon_driver.c` with Kbuild configuration in `driver/Makefile`.
3. Complete documentation suite inside `docs/`.
4. Visual design specifications inside `diagrams/`.
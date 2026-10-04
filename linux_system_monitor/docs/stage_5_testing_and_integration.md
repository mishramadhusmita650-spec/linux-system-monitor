# Stage 5: Testing and Integration

## Verification Test Matrix

All software components undergo verification across build, execution, and integration stages.

| Test ID | Test Case Description | Expected Result | Actual Result |
|---|---|---|---|
| **T01** | CPU Usage Calculation | Reads `/proc/stat`, calculates non-zero CPU usage percentage. | To be verified |
| **T02** | Memory Usage Parsing | Correctly parses `MemTotal` and `MemFree` from `/proc/meminfo`. | To be verified |
| **T03** | Disk Usage Query | Successfully queries `statvfs("/")` and displays non-zero disk capacity. | To be verified |
| **T04** | Process Listing | Iterates `/proc`, identifies numeric PIDs, displays up to 15 process names. | To be verified |
| **T05** | Application Build | Compiles `src/main.cpp` using `make` without compiler errors. | To be verified |
| **T06** | Driver Build | Compiles `driver/sysmon_driver.c` using Kernel Makefiles into `sysmon_driver.ko`. | To be verified |
| **T07** | Driver Loading | Inserts module using `insmod`, verifies entry in `dmesg` and `/dev/sysmon`. | To be verified |
| **T08** | `/dev/sysmon` Access | Directly reads `/dev/sysmon` using `cat /dev/sysmon` to confirm string output. | To be verified |
| **T09** | Application + Driver Integration | C++ application successfully executes option 5 and displays driver output. | To be verified |

---

## Driver Management Procedures

### Driver Loading Procedure
1. Navigate to driver directory:
   ```bash
   cd driver
   ```
2. Build driver kernel object:
   ```bash
   make
   ```
3. Insert kernel module using elevated permissions:
   ```bash
   sudo insmod sysmon_driver.ko
   ```
4. Adjust device node permissions to allow reading:
   ```bash
   sudo chmod 666 /dev/sysmon
   ```
5. Confirm device node creation:
   ```bash
   ls -l /dev/sysmon
   ```

### Driver Unloading Procedure
1. Remove kernel module:
   ```bash
   sudo rmmod sysmon_driver
   ```
2. Verify removal via kernel log:
   ```bash
   dmesg | tail -n 5
   ```

---

## Integration Testing Procedure
1. Build user-space application:
   ```bash
   cd src && make
   ```
2. Execute CLI Application without loading driver:
   ```bash
   ./sysmon_cli
   ```
3. Select option `5` (Kernel Driver Info). Confirm application displays proper error message without crashing.
4. Insert driver following **Driver Loading Procedure**.
5. Re-run `./sysmon_cli` and select option `5`. Confirm core count, RAM stats, and uptime are properly rendered.

---

## Error Handling Analysis
- **Missing `/proc` entries:** Application checks standard file streams (`std::ifstream::is_open()`) before parsing.
- **Unprivileged `/dev/sysmon` Access:** If user attempts to access `/dev/sysmon` without reading privileges, application outputs permission error notice.
- **Unloaded Kernel Module:** If `/dev/sysmon` is absent, `readDriver()` outputs a user warning indicating module is uninserted.

---

## Test Execution Screenshots
*(This section is reserved for appending operational screenshots following manual test execution on host systems.)*

- **Screenshot 1:** CLI Application Main Menu & CPU / Memory / Disk output.
- **Screenshot 2:** Process Listing Output (15 processes).
- **Screenshot 3:** Kernel module build and `insmod` insertion log (`dmesg`).
- **Screenshot 4:** Integrated `/dev/sysmon` display output inside C++ CLI application.
# Sequence Diagram

This diagram details the control and data sequence when a user queries kernel telemetry via option 5 in the C++ application.

---

## Integration Sequence (Mermaid)

```mermaid
sequenceDiagram
    autonumber
    actor User
    participant App as C++ CLI App (main.cpp)
    participant DevNode as Device Node (/dev/sysmon)
    participant Driver as Sysmon Driver (sysmon_driver.c)
    participant Kernel as Linux Kernel

    User->>App: Select Menu Option 5 (Driver Info)
    App->>DevNode: open("/dev/sysmon", O_RDONLY)
    alt Device file open success
        DevNode->>Driver: sysmon_read() file operation
        Driver->>Kernel: num_online_cpus()
        Kernel-->>Driver: Return CPU core count
        Driver->>Kernel: si_meminfo(&si)
        Kernel-->>Driver: Return RAM statistics
        Driver->>Kernel: Query Uptime Jiffies
        Kernel-->>Driver: Return uptime value
        Driver->>Driver: Format string buffer
        Driver->>DevNode: copy_to_user() text buffer
        DevNode-->>App: Read buffer stream
        App->>User: Render Kernel Metrics output
    else Device open failure (/dev/sysmon missing)
        App-->>User: Display Driver Not Loaded Error Message
    end
```

---

## Procedural Text Flow
1. User selects `5` on CLI menu.
2. `main.cpp` invokes `readDriver()`.
3. `readDriver()` opens stream `/dev/sysmon`.
4. Kernel routes file read call to `sysmon_read()` inside `sysmon_driver.c`.
5. `sysmon_read()` queries active CPU cores, memory status (`si_meminfo`), and uptime.
6. Driver copies formatted string to user space memory via `copy_to_user()`.
7. `readDriver()` prints text to stdout.
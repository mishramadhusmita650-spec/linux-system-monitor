# State Machine Diagram

This diagram represents the operational state transition lifecycle of the C++ CLI system monitoring application.

---

## CLI Application State Machine (Mermaid)

```mermaid
stateDiagram-v8
    [*] --> Start
    Start --> DisplayMenu : Application Launch
    
    state DisplayMenu {
        [*] --> AwaitingInput
    }

    DisplayMenu --> CPU_State : Option 1
    DisplayMenu --> Memory_State : Option 2
    DisplayMenu --> Disk_State : Option 3
    DisplayMenu --> Process_State : Option 4
    DisplayMenu --> Driver_State : Option 5
    DisplayMenu --> ExitState : Option 6

    CPU_State --> DisplayResult : Compute Delta
    Memory_State --> DisplayResult : Parse /proc/meminfo
    Disk_State --> DisplayResult : Execute statvfs
    Process_State --> DisplayResult : Scan /proc Directories
    Driver_State --> DisplayResult : Read /dev/sysmon

    DisplayResult --> DisplayMenu : Return on Keypress / Loop
    ExitState --> [*] : Program Terminated
```

---

## State Descriptions
* **Start:** Program entry point initialization.
* **DisplayMenu:** Interactive prompt presenting choices 1-6.
* **CPU_State:** Takes two snapshots of `/proc/stat` separated by a delay to calculate usage %.
* **Memory_State:** Reads total, free, and available memory from `/proc/meminfo`.
* **Disk_State:** Invokes `statvfs("/")` to compute used and total disk capacity.
* **Process_State:** Iterates `/proc`, identifies numeric PIDs, and displays up to 15 process names from `/proc/[PID]/comm`.
* **Driver_State:** Reads custom driver output from `/dev/sysmon`.
* **DisplayResult:** Outputs formatted diagnostic information to stdout.
* **ExitState:** Cleans up standard streams and exits process with return code 0.
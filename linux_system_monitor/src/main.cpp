#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sys/statvfs.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;
namespace fs = std::filesystem;

// Show CPU usage using /proc/stat
void showCPU()
{
    ifstream file("/proc/stat");

    string cpu;
    long long user1, nice1, system1, idle1, iowait1, irq1, softirq1, steal1;

    file >> cpu >> user1 >> nice1 >> system1 >> idle1
         >> iowait1 >> irq1 >> softirq1 >> steal1;

    long long total1 = user1 + nice1 + system1 + idle1 +
                       iowait1 + irq1 + softirq1 + steal1;

    long long idleTotal1 = idle1 + iowait1;

    this_thread::sleep_for(chrono::milliseconds(500));

    file.close();
    file.open("/proc/stat");

    file >> cpu >> user1 >> nice1 >> system1 >> idle1
         >> iowait1 >> irq1 >> softirq1 >> steal1;

    long long total2 = user1 + nice1 + system1 + idle1 +
                       iowait1 + irq1 + softirq1 + steal1;

    long long idleTotal2 = idle1 + iowait1;

    long long totalDiff = total2 - total1;
    long long idleDiff = idleTotal2 - idleTotal1;

    double usage = 100.0 * (totalDiff - idleDiff) / totalDiff;

    cout << fixed << setprecision(2);
    cout << "CPU Usage: " << usage << "%\n";
}

// Show memory usage using /proc/meminfo
void showMemory()
{
    ifstream file("/proc/meminfo");

    string name;
    long long value;
    string unit;

    long long total = 0;
    long long available = 0;

    while (file >> name >> value >> unit)
    {
        if (name == "MemTotal:")
            total = value;

        if (name == "MemAvailable:")
            available = value;
    }

    long long used = total - available;

    cout << "Total Memory: " << total / 1024 << " MB\n";
    cout << "Used Memory : " << used / 1024 << " MB\n";
    cout << "Free Memory : " << available / 1024 << " MB\n";
}

// Show disk usage of root filesystem
void showDisk()
{
    struct statvfs info;

    if (statvfs("/", &info) != 0)
    {
        cout << "Unable to read disk information.\n";
        return;
    }

    unsigned long long total =
        (unsigned long long)info.f_blocks * info.f_frsize;

    unsigned long long freeSpace =
        (unsigned long long)info.f_bavail * info.f_frsize;

    unsigned long long used = total - freeSpace;

    double usedGB = (double)used / (1024 * 1024 * 1024);
    double totalGB = (double)total / (1024 * 1024 * 1024);

    cout << fixed << setprecision(2);
    cout << "Disk Used : " << usedGB << " GB\n";
    cout << "Disk Total: " << totalGB << " GB\n";
}

// List a few running processes
void showProcesses()
{
    int count = 0;

    cout << left << setw(10) << "PID" << "PROCESS\n";
    cout << "-------------------------\n";

    for (const auto& entry : fs::directory_iterator("/proc"))
    {
        if (!entry.is_directory())
            continue;

        string pid = entry.path().filename().string();

        bool number = true;

        for (char c : pid)
        {
            if (!isdigit(c))
            {
                number = false;
                break;
            }
        }

        if (!number)
            continue;

        ifstream processFile(entry.path() / "comm");

        string processName;

        if (processFile >> processName)
        {
            cout << left << setw(10) << pid << processName << "\n";
            count++;
        }

        if (count >= 15)
            break;
    }
}

// Read information from the Linux device driver
void readDriver()
{
    ifstream device("/dev/sysmon");

    if (!device)
    {
        cout << "Could not open /dev/sysmon.\n";
        cout << "Load the driver first using:\n";
        cout << "sudo insmod ../driver/sysmon_driver.ko\n";
        return;
    }

    string line;

    while (getline(device, line))
        cout << line << '\n';

    device.close();
}

int main()
{
    int choice;

    while (true)
    {
        cout << "\n=================================\n";
        cout << "       LINUX SYSTEM MONITOR\n";
        cout << "=================================\n";
        cout << "1. CPU Usage\n";
        cout << "2. Memory Usage\n";
        cout << "3. Disk Usage\n";
        cout << "4. List Processes\n";
        cout << "5. Read Device Driver\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        cout << '\n';

        switch (choice)
        {
            case 1:
                showCPU();
                break;

            case 2:
                showMemory();
                break;

            case 3:
                showDisk();
                break;

            case 4:
                showProcesses();
                break;

            case 5:
                readDriver();
                break;

            case 0:
                cout << "Exiting program...\n";
                return 0;

            default:
                cout << "Invalid choice.\n";
        }
    }
}

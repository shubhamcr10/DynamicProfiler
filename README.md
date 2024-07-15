# Dynamic Profiling

This project demonstrates how to dynamically start and stop profiling a process with signals in Unix-like environments.

## Project Overview

The project includes:
- `start_profiling.cpp`: C++ program to send `SIGUSR1` signal to start profiling.
- `stop_profiling.cpp`: C++ program to send `SIGUSR2` signal to stop profiling.
- `testcode.cpp`: Example executable that can be profiled using the provided signals.

## Prerequisites

- C++ compiler (GCC recommended)
- Unix-like operating system (Linux, macOS)
- Basic understanding of signal handling in C/C++
- Installed libraries if using specific profiling tools (`callgrind`, `pthread`)

## Usage

### Automated Option

#### Terminal 1: Run the Executable

Ensure that the executable you want to profile (`testcode`) is running with .

```bash
./testcode
```

#### Terminal 2: Build and Execute Profiling Scripts

1. Build the Profiling Programs:

```bash
g++ -o start_profiling start_profiling.cpp
g++ -o stop_profiling stop_profiling.cpp
```

2. Start Profiling:
```bash
./start_profiling
```

3.Stop Profiling:
```bash
./stop_profiling
```

### Manual Option

#### Terminal 1: Compile and Run the Executable with Profiling

1. Compile the Executable:

Ensure you include necessary libraries (pthread and callgrind if needed):
```bash
g++ -g -pthread -o testcode testcode.cpp -lpthread -lcallgrind
```

2. Run the Executable:

```bash
./testcode
```

#### Terminal 2: Manually Send Signals to Control Profiling
1. Find the Process ID (PID) of the Executable:

```bash
pidof testcode
```

2. Start Profiling:

```bash
kill -SIGUSR1 <pid>
```

3. Stop Profiling:

```bash
kill -SIGUSR2 <pid>
```

Replace <pid> with the actual PID obtained from the pidof command.

## Additional Notes

- Ensure that the testcode executable has the necessary signal handlers to handle SIGUSR1 and SIGUSR2 for starting and stopping profiling respectively.
- If you encounter any issues with the kill command, verify the syntax and options compatible with your shell environment.
- Use this guide to automate the profiling process or to manually control profiling as per your requirement.


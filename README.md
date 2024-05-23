# CPU Scheduler Project

## Overview
This project implements a CPU scheduler that reads a list of processes from an input file and schedules them using different scheduling algorithms. It supports First-Come, First-Served (FCFS), Shortest Job First (SJF), and Round Robin (RR) with different time quantums.

## Files
- `cpuscheduler.c`: The main source code file containing the implementation of the CPU scheduler.
- `cpu_scheduler_input.txt`: The input file containing the list of processes to be scheduled. Each process is defined by its ID, arrival time, priority, burst time, RAM requirement, and CPU rate requirement.
- `output.txt`: The output file containing the result of cpu scheduling.

## Compilation and Execution
### Compilation
To compile the `cpuscheduler` program, use the following command:
```sh
make
```
or if you are using MINGW
```sh
mingw32-make
```
## Execution
To run the scheduler with the input file cpu_scheduler_input.txt, use the following command:
This will compile the cpuscheduler.c file and create an executable named cpuscheduler.

## Input File Format
The input file should contain the process information in the following format:
<process_id>,<arrival_time>,<priority>,<burst_time>,<ram>,<cpu_rate>
```
Each process should be on a new line. For example:
P1,0,1,2,574,4
P2,1,0,1,50,6
P3,1,3,2,515,8
...
```
# Scheduling Algorithms

The CPU scheduler uses the following algorithms:

## CPU-1 (FCFS)
- Queue: Processes with priority 0.
- Algorithm: First-Come, First-Served (FCFS).
- Resources: Reserved RAM for priority 0 processes (512 units).

## CPU-2 (SJF, RR-q8, RR-q16)
- Queue 1: Processes with priority 1.
  - Algorithm: Shortest Job First (SJF).
- Queue 2: Processes with priority 2.
  - Algorithm: Round Robin with a time quantum of 8 units (RR-q8).
- Queue 3: Processes with priority 3.
  - Algorithm: Round Robin with a time quantum of 16 units (RR-q16).
- Resources: Shared RAM for priority 1, 2, and 3 processes (1536 units).

## Log Output
The scheduler logs the state of each process as it is scheduled. The log is printed to the standard output and also saved to output.txt. The log includes the following events:
- Process is queued.
- Process is assigned to a CPU.
- Process is completed and terminated.
- Insufficient resources for a process (if applicable).

## Functions
The main functions in `cpuscheduler.c` are:
- `read_input_file`: Reads the process information from the input file and populates the input queue.
- `allocate_processes`: Allocates processes to CPU queues based on their priority.
- `simulate_fcfs`: Simulates the FCFS scheduling algorithm.
- `simulate_sjf`: Simulates the SJF scheduling algorithm.
- `simulate_round_robin`: Simulates the Round Robin scheduling algorithm.
- `log_process_state`: Logs the state of a process.
- `print_process_logs`: Prints the process logs.

# Usage Example

Here's an example of how to compile and run the scheduler:

1. Create the input file `cpu_scheduler_input.txt` with the provided content.
2. Compile the scheduler:
   ```sh
   make
   ```
3. Run the scheduler
```sh
make run
```
or
```sh
mingw32-make
```
5. Check the output in output.txt.

Note: Ensure that the input file path and format are correct. If the scheduler encounters insufficient resources for any process, it will log the issue and terminate the application.

Ensure that you have a output.txt file (it can be empty)

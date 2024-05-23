#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Define constants for maximum number of processes, RAM size, reserved RAM, and time quantums
#define MAX_PROCESSES 100
#define RAM_SIZE 2048
#define RESERVED_RAM_FOR_PRIORITY_0 512
#define TIME_QUANTUM_8 8
#define TIME_QUANTUM_16 16

// Process struct definition
typedef struct {
    char id[10];                 // Process ID
    int arrival_time;            // Arrival time of the process
    int priority;                // Priority of the process
    int burst_time;              // Burst time of the process
    int ram;                     // RAM required by the process
    int cpu_rate;                // CPU rate required by the process
} Process;

// Queue struct definition
typedef struct {
    Process processes[MAX_PROCESSES];   // Array of processes in the queue
    int count;                          // Number of processes in the queue
} Queue;

// ProcessLog struct definition
typedef struct {
    char ids[MAX_PROCESSES][10];    // Array of process IDs for logging
    int count;                      // Number of logged processes
} ProcessLog;

// Function prototypes
void read_input_file(char *filename, Queue *queue);
void allocate_processes(Queue *input_queue, Queue *cpu1_queue, Queue *cpu2_queues[]);
void simulate_fcfs(Queue *cpu_queue, char *cpu_name, int *cpu_ram, int *cpu_rate, ProcessLog *log);
void simulate_sjf(Queue *cpu_queue, char *cpu_name, int *cpu_ram, int *cpu_rate, ProcessLog *log);
void simulate_round_robin(Queue *cpu_queue, char *cpu_name, int time_quantum, int *cpu_ram, int *cpu_rate, ProcessLog *log);
void log_process_state(const char *format, ...);
void print_process_logs(ProcessLog cpu1_log, ProcessLog cpu2_logs[]);
int check_resources(int required_ram, int available_ram, int required_cpu_rate, int available_cpu_rate);

// Main function
int main(int argc, char *argv[]) {
    // Check if input file is provided
    if (argc < 2) {
        printf("Usage: %s <input file>\n", argv[0]);
        return 1;
    }

    // Define input queue and CPU queues
    Queue input_queue = {.count = 0};
    Queue cpu1_queue = {.count = 0};
    Queue *cpu2_queues[3];
    
    // Define process logs for CPU1 and CPU2
    ProcessLog cpu1_log = {.count = 0};
    ProcessLog cpu2_logs[3] = {{.count = 0}, {.count = 0}, {.count = 0}};

    // Allocate memory for CPU2 queues
    for (int i = 0; i < 3; i++) {
        cpu2_queues[i] = (Queue *)malloc(sizeof(Queue));
        cpu2_queues[i]->count = 0;
    }

    // Define CPU RAM and CPU rate variables
    int cpu1_ram = RESERVED_RAM_FOR_PRIORITY_0;
    int cpu2_ram = RAM_SIZE - RESERVED_RAM_FOR_PRIORITY_0;
    int cpu1_rate = 100;
    int cpu2_rate = 100;

    // Get input filename from command line arguments
    char *input_filename = argv[1];

    // Read input file and allocate processes to CPU queues
    read_input_file(input_filename, &input_queue);
    allocate_processes(&input_queue, &cpu1_queue, cpu2_queues);

    // Simulate FCFS scheduling for CPU1
    simulate_fcfs(&cpu1_queue, "CPU-1", &cpu1_ram, &cpu1_rate, &cpu1_log);

    // Simulate SJF, RR-q8, and RR-q16 scheduling for CPU2
    simulate_sjf(cpu2_queues[0], "CPU-2 (SJF)", &cpu2_ram, &cpu2_rate, &cpu2_logs[0]);
    simulate_round_robin(cpu2_queues[1], "CPU-2 (RR-8)", TIME_QUANTUM_8, &cpu2_ram, &cpu2_rate, &cpu2_logs[1]);
    simulate_round_robin(cpu2_queues[2], "CPU-2 (RR-16)", TIME_QUANTUM_16, &cpu2_ram, &cpu2_rate, &cpu2_logs[2]);

    // Print process logs
    print_process_logs(cpu1_log, cpu2_logs);

    // Free memory allocated for CPU2 queues
    for (int i = 0; i < 3; i++) {
        free(cpu2_queues[i]);
    }

    return 0;
}

// Function to read input file and populate the input queue
void read_input_file(char *filename, Queue *queue) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open input file");
        exit(1);
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        Process process;
        sscanf(line, "%[^,],%d,%d,%d,%d,%d",
               process.id, &process.arrival_time, &process.priority,
               &process.burst_time, &process.ram, &process.cpu_rate);
        queue->processes[queue->count++] = process;
    }

    fclose(file);
}

// Function to allocate processes to CPU1 and CPU2 queues based on priority
void allocate_processes(Queue *input_queue, Queue *cpu1_queue, Queue *cpu2_queues[]) {
    // Sort processes by arrival time
    for (int i = 0; i < input_queue->count - 1; i++) {
        for (int j = i + 1; j < input_queue->count; j++) {
            if (input_queue->processes[i].arrival_time > input_queue->processes[j].arrival_time) {
                Process temp = input_queue->processes[i];
                input_queue->processes[i] = input_queue->processes[j];
                input_queue->processes[j] = temp;
            }
        }
    }

    // Iterate over all processes in the input queue
    for (int i = 0; i < input_queue->count; i++) {
        Process process = input_queue->processes[i];  // Get the current process

        // Check the priority of the process
        if (process.priority == 0) {
            // Assign processes with priority 0 to CPU1 queue
            cpu1_queue->processes[cpu1_queue->count++] = process;
        } else {
            // Assign processes with priority 1, 2, or 3 to corresponding CPU2 queues
            int queue_index = process.priority - 1;
            cpu2_queues[queue_index]->processes[cpu2_queues[queue_index]->count++] = process;
        }
    }
}

// Function to check if required resources are available (RAM, CPU rate)
int check_resources(int required_ram, int available_ram, int required_cpu_rate, int available_cpu_rate) {
    // Return true if both required RAM and CPU rate are less than or equal to available resources
    return required_ram <= available_ram && required_cpu_rate <= available_cpu_rate;
}

// Function to simulate FCFS (First-Come, First-Served) scheduling
void simulate_fcfs(Queue *cpu_queue, char *cpu_name, int *cpu_ram, int *cpu_rate, ProcessLog *log) {
    // Iterate over all processes in the CPU queue
    for (int i = 0; i < cpu_queue->count; i++) {
        Process process = cpu_queue->processes[i];  // Get the current process

       // Check the resources
       while (!check_resources(process.ram, *cpu_ram, process.cpu_rate, *cpu_rate)) {
            // The application will terminate here if resources are insufficient for any process
          log_process_state("Insufficient resources for process %s", process.id);
         exit(0); // Terminate the application if resources are insufficient
        }
        // Allocate resources to the process
        *cpu_ram -= process.ram;
        *cpu_rate -= process.cpu_rate;

        // Log the state of the process as it is queued, assigned, and completed
        log_process_state("Process %s is queued to be assigned to %s.", process.id, cpu_name);
        log_process_state("Process %s is assigned to %s.", process.id, cpu_name);
        log_process_state("Process %s is completed and terminated.", process.id);

        // Add the process ID to the log
        strcpy(log->ids[log->count++], process.id);

        // Release the resources allocated to the process
        *cpu_ram += process.ram;
        *cpu_rate += process.cpu_rate;
    }
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_PROCESSES 100
#define RAM_SIZE 2048
#define RESERVED_RAM_FOR_PRIORITY_0 512
#define TIME_QUANTUM_8 8
#define TIME_QUANTUM_16 16

typedef struct {
    char id[10];
    int arrival_time;
    int priority;
    int burst_time;
    int ram;
    int cpu_rate;
} Process;

typedef struct {
    Process processes[MAX_PROCESSES];
    int count;
} Queue;

typedef struct {
    char ids[MAX_PROCESSES][10];
    int count;
} ProcessLog;

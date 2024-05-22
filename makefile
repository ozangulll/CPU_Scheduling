all:cpuscheduler run

cpuscheduler: cpuscheduler.c
	gcc cpuscheduler.c -o cpuscheduler

run: cpuscheduler
	.\cpuscheduler cpu_scheduler_input.txt > output.txt

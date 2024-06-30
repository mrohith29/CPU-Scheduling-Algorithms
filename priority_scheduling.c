#include <stdio.h>
#include <limits.h>

#define MAX_PROCESSES 100

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

void calculateFCFS(Process processes[], int n) {
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    processes[0].completion_time = processes[0].arrival_time + processes[0].burst_time;
    processes[0].turnaround_time = processes[0].completion_time - processes[0].arrival_time;
    processes[0].waiting_time = processes[0].turnaround_time - processes[0].burst_time;

    total_waiting_time += processes[0].waiting_time;
    total_turnaround_time += processes[0].turnaround_time;

    for (int i = 1; i < n; i++) {
        if (processes[i].arrival_time > processes[i - 1].completion_time) {
            processes[i].completion_time = processes[i].arrival_time + processes[i].burst_time;
        } else {
            processes[i].completion_time = processes[i - 1].completion_time + processes[i].burst_time;
        }

        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;

        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    printf("\nFCFS Scheduling:\n");
    printf("Process ID\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].completion_time, processes[i].turnaround_time, processes[i].waiting_time);
    }

    printf("Average Waiting Time: %.2f\n", total_waiting_time / (float)n);
}

void calculateSJF(Process processes[], int n) {
    int total_waiting_time = 0, total_turnaround_time = 0, completed = 0, current_time = 0;
    int is_completed[MAX_PROCESSES] = {0};

    while (completed != n) {
        int min_index = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && !is_completed[i] && processes[i].burst_time < min_burst) {
                min_burst = processes[i].burst_time;
                min_index = i;
            }
        }

        if (min_index == -1) {
            current_time++;
            continue;
        }

        processes[min_index].completion_time = current_time + processes[min_index].burst_time;
        processes[min_index].turnaround_time = processes[min_index].completion_time - processes[min_index].arrival_time;
        processes[min_index].waiting_time = processes[min_index].turnaround_time - processes[min_index].burst_time;

        total_waiting_time += processes[min_index].waiting_time;
        total_turnaround_time += processes[min_index].turnaround_time;
        current_time = processes[min_index].completion_time;
        is_completed[min_index] = 1;
        completed++;
    }

    printf("\nSJF Non-Preemptive Scheduling:\n");
    printf("Process ID\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].completion_time, processes[i].turnaround_time, processes[i].waiting_time);
    }

    printf("Average Waiting Time: %.2f\n", total_waiting_time / (float)n);
}

void calculateSJFPreemptive(Process processes[], int n) {
    int total_waiting_time = 0, total_turnaround_time = 0, current_time = 0, completed = 0;
    int min_index = -1, min_remaining = INT_MAX;

    while (completed != n) {
        min_index = -1;
        min_remaining = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0 && processes[i].remaining_time < min_remaining) {
                min_remaining = processes[i].remaining_time;
                min_index = i;
            }
        }

        if (min_index == -1) {
            current_time++;
            continue;
        }

        processes[min_index].remaining_time--;
        current_time++;

        if (processes[min_index].remaining_time == 0) {
            processes[min_index].completion_time = current_time;
            processes[min_index].turnaround_time = processes[min_index].completion_time - processes[min_index].arrival_time;
            processes[min_index].waiting_time = processes[min_index].turnaround_time - processes[min_index].burst_time;

            total_waiting_time += processes[min_index].waiting_time;
            total_turnaround_time += processes[min_index].turnaround_time;
            completed++;
        }
    }

    printf("\nSJF Preemptive Scheduling:\n");
    printf("Process ID\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].completion_time, processes[i].turnaround_time, processes[i].waiting_time);
    }

    printf("Average Waiting Time: %.2f\n", total_waiting_time / (float)n);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process processes[n];
    for (int i = 0; i < n; i++) {
        printf("Enter process ID, arrival time, and burst time for process %d: ", i + 1);
        scanf("%d %d %d", &processes[i].id, &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }

    calculateFCFS(processes, n);

    // Resetting data for SJF non-preemptive calculation
    for (int i = 0; i < n; i++) {
        processes[i].completion_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].remaining_time = processes[i].burst_time;
    }

    calculateSJF(processes, n);

    // Resetting data for SJF preemptive calculation
    for (int i = 0; i < n; i++) {
        processes[i].completion_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].remaining_time = processes[i].burst_time;
    }

    calculateSJFPreemptive(processes, n);

    return 0;
}

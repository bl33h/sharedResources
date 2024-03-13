/* ----------------------------------------------------------------------------
Copyright (C), 2024-2025, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: banker.c
@version: I
Creation: 11/03/2024
Last modification: 12/03/2024
References
[GfG. (2023). Banker s Algorithm in Operating System. GeeksforGeeks. https://www.geeksforgeeks.org/bankers-algorithm-in-operating-system-2/]
------------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>

// resources and processes number
#define resourcesNumber 3
#define processesNumber 5

// initial available resources
int maxClaim[processesNumber][resourcesNumber] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};
int available[resourcesNumber] = {3, 3, 2};  

// initial allocations
int allocation[processesNumber][resourcesNumber] = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}}; 
bool finish[processesNumber] = {false, false, false, false, false};
int need[processesNumber][resourcesNumber];

// system safe state checker
bool checkSafety() {
    int work[resourcesNumber];
    for (int i = 0; i < resourcesNumber; ++i) {
        work[i] = available[i];
    }

    bool finishLocal[processesNumber];
    for (int i = 0; i < processesNumber; ++i) {
        finishLocal[i] = finish[i];
    }

    for (int count = 0; count < processesNumber; ++count) {
        bool found = false;
        for (int p = 0; p < processesNumber; ++p) {
            if (!finishLocal[p]) {
                int j;
                for (j = 0; j < resourcesNumber; ++j)
                    if (need[p][j] > work[j])
                        break;

                if (j == resourcesNumber) {
                    for (int k = 0; k < resourcesNumber; ++k)
                        work[k] += allocation[p][k];

                    finishLocal[p] = true;
                    found = true;
                }
            }
        }
        // no process can proceed
        if (!found) break; 
    }

    for (int i = 0; i < processesNumber; ++i)
        if (!finishLocal[i])
        // system is not safe if any process is left
            return false; 

    // system is safe if all processes are finished
    return true; 
}

// resources request for a process
bool requestResources(int process_id, int request[resourcesNumber]) {

    // request is greater than need checker
    for (int i = 0; i < resourcesNumber; ++i) {
        if (request[i] > need[process_id][i]) {
            printf("!error: process has exceeded its maximum claim.\n");
            return false;
        }
    }

    // request is greater than available checker
    for (int i = 0; i < resourcesNumber; ++i) {
        if (request[i] > available[i]) {
            printf("resources not available.\n");
            return false;
        }
    }

    // allocare resources
    for (int i = 0; i < resourcesNumber; ++i) {
        available[i] -= request[i];
        allocation[process_id][i] += request[i];
        need[process_id][i] -= request[i];
    }

    // system safety checker
    if (checkSafety()) {
        printf("- resources allocated to process no [%d] successfully.\n", process_id);
        return true;
    } else {

        // allocation revocation if system is unsafe
        for (int i = 0; i < resourcesNumber; ++i) {
            available[i] += request[i];
            allocation[process_id][i] -= request[i];
            need[process_id][i] += request[i];
        }
        printf("!request denied to avoid unsafe state.\n");
        return false;
    }
}

// release resources from a process
void releaseResources(int process_id, int release[resourcesNumber]) {
    for (int i = 0; i < resourcesNumber; ++i) {
        available[i] += release[i];
        allocation[process_id][i] -= release[i];
        need[process_id][i] += release[i];
    }
    printf("|resources released from process [%d].\n", process_id);
}

int main() {
    // calculate need matrix
    for (int i = 0; i < processesNumber; ++i) {
        for (int j = 0; j < resourcesNumber; ++j) {
            need[i][j] = maxClaim[i][j] - allocation[i][j];
        }
    }

    while (true) {
        int process_id;
        int request[resourcesNumber];
        int release[resourcesNumber];

        printf("\n• enter process ID (or -1 to exit): ");
        scanf("%d", &process_id);

        if (process_id == -1) {
            break;
        }

        if (process_id < 0 || process_id >= processesNumber) {
            printf("?invalid process ID, please try again.\n");

            // skip the rest of the loop iteration if the process ID is invalid
            continue; 
        }

        printf("• enter resource request (format: x y z): ");
        for (int i = 0; i < resourcesNumber; ++i) {
            scanf("%d", &request[i]);
        }
        
        // check and request resources only if the input request is not 0
        bool isRequestNonZero = false;
        for (int i = 0; i < resourcesNumber; ++i) {
            if (request[i] > 0) {
                isRequestNonZero = true;
                break;
            }
        }

        if (isRequestNonZero && !requestResources(process_id, request)) {
            printf("!!!failed to allocate resources due to safety constraints or invalid request.\n");
        }

        printf("• enter resource release (format: x y z): ");
        for (int i = 0; i < resourcesNumber; ++i) {
            scanf("%d", &release[i]);
        }
        
        // perform release operation
        releaseResources(process_id, release);
    }

    return 0;
}
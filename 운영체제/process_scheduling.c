#include <stdio.h>
#include <stdlib.h>

// 스케줄 출력 함수
void printSchedule(int **throughput, int process_count, int total_time) {
    printf("        ");
    for (int l = 0; l < total_time; l++) {
        printf("%2d  ", l);
    }
    printf("\n    ");
    for (int l = 0; l < total_time; l++) {
        printf("------");
    }
    printf("\n");

    for (int i = 1; i < process_count; i++) {
        printf(" [%c] >> ", i + 64); // ASCII 'A'는 65
        for (int k = 0; k < total_time; k++) {
            if (throughput[i][k] == 1) {
                printf("■  "); // 실행된 경우
            } else {
                printf("□  "); // 실행되지 않은 경우
            }
        }
        printf("\n");
    }
}

// FCFS (First-Come, First-Served)
void FCFS(int **arr, int process_count) {
    int total_time = 0;
    int **throughput;
    int deadline = 0;

    // 총 실행 시간 계산
    for (int i = 1; i <= process_count; i++) {
        total_time += arr[i][1];
    }

    // 스케줄 배열 할당
    throughput = (int **)malloc(sizeof(int *) * (process_count + 1));
    for (int i = 1; i <= process_count; i++) {
        throughput[i] = (int *)malloc(sizeof(int) * total_time);
        for (int j = 0; j < total_time; j++) {
            throughput[i][j] = 0; // 초기화
        }
    }

    // FCFS 스케줄링
    int current_time = 0;
    for (int i = 1; i <= process_count; i++) {
        while (current_time < arr[i][0]) {
            current_time++;
        }
        for (int j = 0; j < arr[i][1]; j++) {
            throughput[i][current_time++] = 1;
        }
    }

    // 스케줄 출력
    printSchedule(throughput, process_count, total_time);

    // 메모리 해제
    for (int i = 1; i <= process_count; i++) {
        free(throughput[i]);
    }
    free(throughput);
}

// SPN (Shortest Process Next)
void SPN(int **arr, int process_count) {
    int total_time = 0;
    int **throughput;

    // 총 실행 시간 계산
    for (int i = 1; i <= process_count; i++) {
        total_time += arr[i][1];
    }

    // 스케줄 배열 할당
    throughput = (int **)malloc(sizeof(int *) * (process_count + 1));
    for (int i = 1; i <= process_count; i++) {
        throughput[i] = (int *)malloc(sizeof(int) * total_time);
        for (int j = 0; j < total_time; j++) {
            throughput[i][j] = 0; // 초기화
        }
    }

    // SPN 스케줄링
    int current_time = 0;
    while (1) {
        int shortest_time = 99999;
        int selected_process = -1;

        // 가장 짧은 실행 시간의 프로세스 찾기
        for (int i = 1; i <= process_count; i++) {
            if (arr[i][1] > 0 && arr[i][0] <= current_time && arr[i][1] < shortest_time) {
                shortest_time = arr[i][1];
                selected_process = i;
            }
        }

        if (selected_process == -1) {
            break; // 실행할 프로세스가 없으면 종료
        }

        // 프로세스 실행
        throughput[selected_process][current_time++] = 1;
        arr[selected_process][1]--;

        // 모든 프로세스 완료 여부 확인
        int all_completed = 1;
        for (int i = 1; i <= process_count; i++) {
            if (arr[i][1] > 0) {
                all_completed = 0;
                break;
            }
        }

        if (all_completed) {
            break;
        }
    }

    // 스케줄 출력
    printSchedule(throughput, process_count, current_time);

    // 메모리 해제
    for (int i = 1; i <= process_count; i++) {
        free(throughput[i]);
    }
    free(throughput);
}

// HRRN (Highest Response Ratio Next)
void HRRN(int **arr, int process_count) {
    int total_time = 0;
    int **throughput;

    // 총 실행 시간 계산
    for (int i = 1; i <= process_count; i++) {
        total_time += arr[i][1];
    }

    // 스케줄 배열 할당
    throughput = (int **)malloc(sizeof(int *) * (process_count + 1));
    for (int i = 1; i <= process_count; i++) {
        throughput[i] = (int *)malloc(sizeof(int) * total_time);
        for (int j = 0; j < total_time; j++) {
            throughput[i][j] = 0; // 초기화
        }
    }

    // HRRN 스케줄링
    int current_time = 0;
    while (1) {
        float highest_response_ratio = -1.0;
        int selected_process = -1;

        // 응답 비율 계산 후 가장 높은 비율의 프로세스 선택
        for (int i = 1; i <= process_count; i++) {
            if (arr[i][1] > 0 && arr[i][0] <= current_time) {
                float response_ratio = (float)(current_time - arr[i][0] + arr[i][1]) / arr[i][1];
                if (response_ratio > highest_response_ratio) {
                    highest_response_ratio = response_ratio;
                    selected_process = i;
                }
            }
        }

        if (selected_process == -1) {
            break; // 실행할 프로세스가 없으면 종료
        }

        // 프로세스 실행
        throughput[selected_process][current_time++] = 1;
        arr[selected_process][1]--;

        // 모든 프로세스 완료 여부 확인
        int all_completed = 1;
        for (int i = 1; i <= process_count; i++) {
            if (arr[i][1] > 0) {
                all_completed = 0;
                break;
            }
        }

        if (all_completed) {
            break;
        }
    }

    // 스케줄 출력
    printSchedule(throughput, process_count, current_time);

    // 메모리 해제
    for (int i = 1; i <= process_count; i++) {
        free(throughput[i]);
    }
    free(throughput);
}

// RR (Round Robin)
void RR(int **arr, int process_count, int quantum) {
    int total_time = 0;
    int **throughput;

    // 총 실행 시간 계산
    for (int i = 1; i <= process_count; i++) {
        total_time += arr[i][1];
    }

    // 스케줄 배열 할당
    throughput = (int **)malloc(sizeof(int *) * (process_count + 1));
    for (int i = 1; i <= process_count; i++) {
        throughput[i] = (int *)malloc(sizeof(int) * total_time);
        for (int j = 0; j < total_time; j++) {
            throughput[i][j] = 0; // 초기화
        }
    }

    // RR 스케줄링
    int current_time = 0;
    while (1) {
        int completed_processes = 0;
        for (int i = 1; i <= process_count; i++) {
            if (arr[i][1] > 0) {
                int remaining_time = arr[i][1];
                for (int j = 0; j < quantum && remaining_time > 0; j++) {
                    throughput[i][current_time++] = 1;
                    arr[i][1]--;
                    remaining_time--;
                }
            } else {
                completed_processes++;
            }
        }

        if (completed_processes == process_count) {
            break;
        }
    }

    // 스케줄 출력
    printSchedule(throughput, process_count, current_time);

    // 메모리 해제
    for (int i = 1; i <= process_count; i++) {
        free(throughput[i]);
    }
    free(throughput);
}

// Feedback with Quantum 2^i
void Feedback(int **arr, int process_count) {
    int total_time = 0;
    int **throughput;

    // 총 실행 시간 계산
    for (int i = 1; i <= process_count; i++) {
        total_time += arr[i][1];
    }

    // 스케줄 배열 할당
    throughput = (int **)malloc(sizeof(int *) * (process_count + 1));
    for (int i = 1; i <= process_count; i++) {
        throughput[i] = (int *)malloc(sizeof(int) * total_time);
        for (int j = 0; j < total_time; j++) {
            throughput[i][j] = 0; // 초기화
        }
    }

    // Feedback 스케줄링
    int current_time = 0;
    int quantum = 1;

    while (1) {
        int all_completed = 1;

        // 각 프로세스 실행
        for (int i = 1; i <= process_count; i++) {
            if (arr[i][1] > 0) {
                all_completed = 0; // 실행할 프로세스가 있으면

                // 프로세스의 인덱스에 따라 시간 할당량 결정
                quantum = 1 << (i - 1); // 2^i

                // 프로세스 실행
                int remaining_time = arr[i][1];
                for (int j = 0; j < quantum && remaining_time > 0; j++) {
                    throughput[i][current_time++] = 1;
                    arr[i][1]--;
                    remaining_time--;
                }
            }
        }

        // 모든 프로세스 완료 여부 확인
        if (all_completed) {
            break;
        }
    }

    // 스케줄 출력
    printSchedule(throughput, process_count, current_time);

    // 메모리 해제
    for (int i = 1; i <= process_count; i++) {
        free(throughput[i]);
    }
    free(throughput);
}

int main() {
    int process_count;
    printf("프로세스 수 입력: ");
    scanf("%d", &process_count);

    int **arr = (int **)malloc(sizeof(int *) * (process_count + 1));

    // 시작 프로세스
    arr[0] = (int *)malloc(sizeof(int) * 2);
    arr[0][0] = 0;
    arr[0][1] = 0;
    
    // 입력되는 프로세스 정보
    for (int i = 1; i <= process_count; i++) {
        arr[i] = (int *)malloc(sizeof(int) * 2); // {도착 시간, 실행 시간}
        printf("프로세스 %d : ", i);
        scanf("%d %d", &arr[i][0], &arr[i][1]);
    }

    int choice = 1;
    while (1) {
        printf("\n< 스케줄링 알고리즘 선택 >\n");
        printf("1. FIFO\n");
        printf("2. SPN\n");
        printf("3. HRRN\n");
        printf("4. RR (quantum = 1)\n");
        printf("5. RR (quantum = 4)\n");
        printf("6. Feedback (quantum = 2^i)\n");
        printf("0. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 6) {
            printf("종료합니다...\n");
            break;
        }

        switch (choice) {
            case 1:
                FCFS(arr, process_count);
                break;
            case 2:
                SPN(arr, process_count);
                break;
            case 3:
                HRRN(arr, process_count);
                break;
            case 4:
                RR(arr, process_count, 1);
                break;
            case 5:
                RR(arr, process_count, 4);
                break;
            case 6:
                Feedback(arr, process_count);
                break;
            default:
                printf("잘못된 선택입니다. 종료합니다...\n");
                return 1;
        }
    }

    // 메모리 해제
    for (int i = 1; i <= process_count; i++) {
        free(arr[i]);
    }
    free(arr);

    return 0;
}

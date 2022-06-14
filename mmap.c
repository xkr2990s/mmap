#include <stdio.h>
#include <sys/mman.h>
#include <time.h>

int main() {
    //프로그램 실행시간 측정 변수 & 행렬 크기, 반복 횟수, 행렬곱 원소 변수 선언
    clock_t start, end;
    float run_time;
    int N = 0, state = 0, sum;
    //행렬 크기와 행렬곱 연산 반복 횟수 입력
    printf("행렬의 크기 : ");
    scanf("%d", &N);
    printf("반복 횟수 : ");
    scanf("%d", &state);
    //결과 행렬 선언 & 실행시간 측정 시작
    int mul[N][N];
    start = clock();
    //mmap()를 사용하여 N*N 크기의 행렬 두 개 생성
    int *ptr1 = mmap(NULL, N*N*sizeof(int),
            PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
    int *ptr2 = mmap(NULL, N*N*sizeof(int),
            PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
    //정상적 맵핑 확인
    if(ptr1==MAP_FAILED || ptr2==MAP_FAILED) {
        printf("Mapping Failed\n");
        return 1;
    }
    //행렬 초기화
    for(int i=0;i<N*N;i++)
        ptr1[i] = 1;
    for(int i=0;i<N*N;i++)
        ptr2[i] = 1;
    //행렬 곱 알고리즘
    while(state) {
        for(int row=0;row<N;row++) {
            for (int col=0;col<N;col++) {
                sum = 0;
                for (int i=0;i<N;i++)
                    sum += ptr1[row*N+i] * ptr2[i*N+col];
                mul[row][col] = sum;
            }
        }
        state--;
    }
    //측정 종료 & 실행시간 출력
    end = clock();
    run_time = end-start;
    printf("실행시간 : %.0f\n", run_time);
    //메모리 해제
    int err1 = munmap(ptr1, N*N*sizeof(int));
    int err2 = munmap(ptr2, N*N*sizeof(int));
    if (err1!=0 || err2!=0) {
        printf("UnMapping Failed\n");
        return 1;
    }

    return 0;
}
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void filterPrimes(int inputPipe[2]) {
    int currentPrime;

    close(inputPipe[1]);

    if (read(inputPipe[0], &currentPrime, sizeof(currentPrime)) == 0) {
        close(inputPipe[0]);
        exit(0);
    }

    printf("prime %d\n", currentPrime);

    // Tạo ống dẫn cho các số nguyên tố tiếp theo
    int nextPipe[2];
    pipe(nextPipe);

    // Tạo quy trình con
    if (fork() == 0) {
        close(inputPipe[0]); 
        close(nextPipe[1]);
        filterPrimes(nextPipe);
    } else {
        close(nextPipe[0]); 

        int number;
        while (read(inputPipe[0], &number, sizeof(number)) > 0) {
            if (number % currentPrime != 0) {
                write(nextPipe[1], &number, sizeof(number));
            }
        }
        close(inputPipe[0]);
        close(nextPipe[1]);
        wait(0); 
    }
}

int main() {
    int initialPipe[2];
    pipe(initialPipe); 

    if (fork() == 0) {
        filterPrimes(initialPipe);
    } else {
        close(initialPipe[0]);

        for (int i = 2; i <= 280; i++) {
            write(initialPipe[1], &i, sizeof(i)); 
        }

        close(initialPipe[1]); 
        wait(0); 
    }

    exit(0); 
}

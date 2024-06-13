#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

pid_t transmitter_pid;
volatile sig_atomic_t bit_received = 0;
volatile sig_atomic_t received_bit;
volatile sig_atomic_t bit_count = 0;
int received_number = 0;

void sigusr_handler(int signo) {
    if (signo == SIGUSR1) {
        received_bit = 0;
    } else if (signo == SIGUSR2) {
        received_bit = 1;
    }
    bit_received = 1;
    bit_count++;
    if (bit_count <= 32) {
        received_number |= (received_bit << (bit_count - 1));
    }
    kill(transmitter_pid, SIGUSR1);
}

int main() {
    printf("Приемник PID: %d\n", getpid());

    struct sigaction sa;
    sa.sa_handler = sigusr_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("Ошибка установки обработчика сигнала");
        exit(EXIT_FAILURE);
    }

    printf("Введите PID передатчика: ");
    scanf("%d", &transmitter_pid);

    printf("Ожидание сигнала...\n");

    while (bit_count <= 32) {
        pause();  // Ожидание сигнала
    }

    printf("Принятое число: %d\n", received_number);
    return 0;
}

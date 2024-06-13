#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

pid_t receiver_pid;
volatile sig_atomic_t ready_to_send = 1;

void sigusr_handler(int signo) {
    ready_to_send = 1;
}

void send_bit(int bit) {
    int signal = (bit == 0) ? SIGUSR1 : SIGUSR2;
    kill(receiver_pid, signal);
}

void send_number(int number) {
    for (int i = 0; i < 32; i++) {
        while (!ready_to_send) {
            usleep(100);
        }
        ready_to_send = 0;
        send_bit((number >> i) & 1);
    }

    while (!ready_to_send) {
        usleep(100);
    }
    ready_to_send = 0;
    kill(receiver_pid, SIGUSR1);
}

int main() {
    printf("Передатчик PID: %d\n", getpid());

    struct sigaction sa;
    sa.sa_handler = sigusr_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("Ошибка установки обработчика сигнала");
        exit(EXIT_FAILURE);
    }

    printf("Введите PID приемника: ");
    scanf("%d", &receiver_pid);

    int number;
    printf("Введите число для отправки: ");
    scanf("%d", &number);

    send_number(number);

    printf("Число отправлено.\n");
    return 0;
}

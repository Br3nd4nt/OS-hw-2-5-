#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFFER_SIZE 4096

void copy_file(const char *input_file, const char *output_file) {
    int input_fd, output_fd;
    ssize_t read_bytes, write_bytes;
    char buffer[BUFFER_SIZE];
    struct stat file_stat;

    input_fd = open(input_file, O_RDONLY);
    if (input_fd == -1) {
        perror("open");
        exit(1);
    }

    if (fstat(input_fd, &file_stat) == -1) {
        perror("fstat");
        exit(1);
    }

    output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, file_stat.st_mode & 0777);
    if (output_fd == -1) {
        perror("open");
        exit(1);
    }

    while ((read_bytes = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        char *out_ptr = buffer;
        ssize_t bytes_to_write = read_bytes;
        while (bytes_to_write > 0) {
            ssize_t bytes_written = write(output_fd, out_ptr, bytes_to_write);
            if (bytes_written < 0) {
                close(input_fd);
                close(output_fd);
                perror("write");
                exit(1);
            }
            bytes_to_write -= bytes_written;
            out_ptr += bytes_written;
        }
    }

    if (read_bytes < 0) {
            close(input_fd);
            close(output_fd);
            perror("read");
        }

        // Закрываем файлы
        close(input_fd);
        close(output_fd);

        // Сохраняем режимы доступа исходного файла
        if (chmod(output_file, file_stat.st_mode) < 0) {
            perror("chmod");
            exit(1);
        }

        printf("Копирование завершено успешно.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }
    copy_file(argv[1], argv[2]);
}

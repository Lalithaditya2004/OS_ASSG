#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/mgdev"

int main() {
    int fd;
    char *message = "KAJA_B220937CS";
    char buffer[100];

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    write(fd, message, strlen(message));

    read(fd, buffer, sizeof(buffer));

    printf("Read from device: %s\n", buffer);

    close(fd);

    return 0;
}

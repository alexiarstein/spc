// SPC - a File listing tool that lists files based on their size in descending order
// Author: Alexia Steinberg <alexia@rockylinux.org>
// Usage: spc <directory> <N. of rows> (e.g: spc /tmp 4)
// License: GNU GPL 3.0 (See LICENSE for more information)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <directory> <number>\n", argv[0]);
        return 1;
    }

    char *directory = argv[1];
    int num = atoi(argv[2]);

    if (num <= 0) {
        fprintf(stderr, "Number must be greater than 0.\n");
        return 1;
    }

    char command[512];
    printf("Size    File\n");
    snprintf(command, sizeof(command), "du -h \"%s\" | sort -hr | head -n %d", directory, num);

    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }

    pclose(fp);
    return 0;
}

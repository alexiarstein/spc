#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define COMMAND_SIZE 1024
#define MAX_NUM_LENGTH 20 // Maximum length for the number string

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s <directory> <number>\n", program_name);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }

    char *directory = argv[1];
    int num;
    if (sscanf(argv[2], "%d", &num) != 1 || num <= 0) {
        fprintf(stderr, "Number must be a positive integer.\n");
        return 1;
    }

    // Create pipes for communication
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) { // Child process
        // Close read end of pipe
        close(pipefd[0]);

        // Redirect stdout to the pipe write end
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            close(pipefd[1]);
            return 1;
        }
        close(pipefd[1]);

        // Convert num to string
        size_t num_str_len = snprintf(NULL, 0, "%d", num) + 1; // +1 for null terminator
        char *num_str = malloc(num_str_len);
        if (num_str == NULL) {
            perror("malloc");
            return 1; // Exit if malloc fails
        }
        snprintf(num_str, num_str_len, "%d", num);

        // Execute the command in the child process
        char *args[] = {"du", "-h", directory, NULL};
        char *envp[] = {NULL}; // No environment variables needed
        
        execve("/usr/bin/du", args, envp); // Use execve with specified arguments and environment

        // If execve fails, print error and exit
        perror("execve");
        free(num_str);
        return 1;
    } else { // Parent process
        // Close write end of pipe
        close(pipefd[1]);

        // Read from the pipe
        char buffer[COMMAND_SIZE];
        ssize_t total_bytes_read = 0;
        ssize_t bytes_read;
        
        while ((bytes_read = read(pipefd[0], buffer + total_bytes_read, COMMAND_SIZE - total_bytes_read - 1)) > 0) {
            total_bytes_read += bytes_read;
            
            // Check if buffer is full (no space for null terminator)
            if (total_bytes_read >= COMMAND_SIZE - 1) {
                fprintf(stderr, "Command output exceeded buffer size. Aborting.\n");
                break;
            }
        }
        
        if (bytes_read == -1) {
            perror("read");
            close(pipefd[0]);
            return 1;
        }

        // Null-terminate the buffer
        buffer[total_bytes_read] = '\0';

        // Print the entire output
        printf("%s", buffer);

        // Wait for the child process to terminate
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            close(pipefd[0]);
            return 1;
        }

        // Close read end of pipe
        close(pipefd[0]);

        // Check if the child process terminated normally
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            fprintf(stderr, "Child process did not terminate normally.\n");
            return 1;
        }
    }

    return 0;
}

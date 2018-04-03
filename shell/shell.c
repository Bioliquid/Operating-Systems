#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define DELIMS " \t\n\a\r"

char** split_args(char *input_line) {
	int buffer_size = 16;
	int pos = 0;
	char **tokens = malloc(buffer_size * sizeof(char*));
	char *token;

	token = strtok(input_line, DELIMS);
	while(token) {
		tokens[pos] = token;
		pos++;
		if(pos >= buffer_size) {
			buffer_size *= 2;
			tokens = realloc(tokens, buffer_size * sizeof(char*));
			if(!tokens) {
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, DELIMS);
	}
	tokens[pos] = NULL;
	return tokens;
}

int launch(char **arg) {
	extern char **environ;
	pid_t pid = fork();
	if(pid == 0) {
		if(execve(arg[0], arg, environ) == -1) {
			perror("Error");
		}
		exit(EXIT_FAILURE);
	} else if(pid < 0) {
		printf("\n Failed to process \n");
	} else {
		int status;
		do {
			if(wait(&status) == -1) {
				perror("Error");
			}
		} while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

void read_args(char **input_line) {
	size_t buffer_size = 0;

	if (getline(&(*input_line), &buffer_size, stdin) == -1) {	
		free(*input_line);
		if (errno == 0) {
			printf("\nProcess stopped with exit code 0\n");
			exit(EXIT_FAILURE);
		}
		perror("Reading failed with error");
	}
}

int main(int argc, char ** argv) {
	char *input_line = NULL;
	char **tokens = NULL;
	int status = 1;
	while(status) {
		printf("client@linux-echo:~$ ");
		read_args(&input_line);
		tokens = split_args(input_line);
		if (tokens[0] != NULL) {
			if(strcmp("exit", tokens[0]) != 0) {
				status = launch(tokens);
			} else {
				status = 0;
			}
		}
		free(input_line);
		free(tokens);
	}
	return 0;
}
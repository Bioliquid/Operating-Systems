#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int execute(char **args) {
	if (args[0] == NULL) {
		return 1;
	}
	
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		if (execvp(args[0], args) == -1) {
			perror("Executing process failed with error");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("Fork failed with error");
	} else {
		do {
			if ((wpid = waitpid(pid, &status, WUNTRACED) == -1)) {
				perror("I have no time for games");
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

char **split_args(char *args) {
	size_t token_capacity = 8;
	size_t position = 0;
	char **tokens = (char**)malloc(token_capacity * sizeof(char*));
	char *token;
	
	if (!tokens) {
		perror("Memory allocation failed with error");
	}

	token = strtok(args, " \t\n");
	while (token != NULL) {
		tokens[position] = token;
		++position;

		if (position >= token_capacity) {
			token_capacity *= 2;
			tokens = realloc(tokens, token_capacity * sizeof(char*));
			
			if (!tokens) {
				perror("Memory allocation failed with error");
			}
		}

		token = strtok(NULL, " \t\n");
	}
	tokens = realloc(tokens, (position + 1) * sizeof(char*));
	tokens[position] = NULL;
	return tokens;
}

char *read_args() {
	char *line = NULL;
	size_t buffer_size = 0;

	if (getline(&line, &buffer_size, stdin) == -1) {	
		perror("Reading failed with error");
	}

	return line;
}

int main() {
	char *line = 0;
	char **args;
	enum status { FAILURE, SUCCESS };
	enum status current_status = SUCCESS;
	
	do {
		printf("> ");
		line = read_args();
		args = split_args(line);
		current_status = execute(args);
	} while (current_status == SUCCESS);

	free(line);
	free(args);
	return EXIT_SUCCESS;
}
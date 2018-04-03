#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <ctype.h>
#include <sys/types.h>

int launch(char *const* arg) {
	extern char **environ;
	pid_t pid = fork();
	if(pid == 0) {
		if(execve(arg[0], arg, environ) == -1) {
			perror("Error");
		}
		exit(EXIT_FAILURE);
	} else if(pid < 0) {
		printf("\nFailed to process\n");
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

bool is_number(char *st) {
	for (; *st; ++st) {
		if (*st != 0 && !isdigit(*st)) {
			return false;
		}
	}
	return true;
}

int parse(char ** argv) {
	argv++;
	DIR *directory;
	struct dirent *dir;

	if ((directory = opendir(*argv)) == NULL) {
		perror("Failed to open directory");
		exit(EXIT_FAILURE);
	}
	char directory_path[1024];
	
	if (strcmp(*argv, ".") != 0) {
		strcpy(directory_path, *argv);
		strcat(directory_path, "/");
	} else {
		directory_path[0] = '\0';
	}
	argv++;

	bool has_name = false;
	char *name = "";

	bool has_exec_file = false;
	char *exec_file = "";

	bool has_size = false;
	char sign = '\0';
	off_t size = 0;

	bool has_nlink = false;
	nlink_t nlink = 0;

	bool has_inode = false;
	ino_t inode = 0;
	while (*argv) {
		if (strcmp(*argv, "-name") == 0) {
			argv++;
			has_name = true;
			name = *argv;	
		} else if(strcmp(*argv, "-size") == 0) {
			argv++;
			char * pointer = *argv + 1;
			if (is_number(pointer) == false) {
				printf("Invalid \'size\' argument\n");
				exit(EXIT_FAILURE);
			}
			has_size = true;
			sign = **argv;
			size = atoll(pointer);
		} else if(strcmp(*argv, "-inum") == 0) {
			argv++;
			if (is_number(*argv) == false) {
				printf("Invalid \'inode\' argument\n");
				exit(EXIT_FAILURE);
			}
			has_inode = true;
			inode = strtoul(*argv, 0L, 10);
		} else if(strcmp(*argv, "-nlinks") == 0) {
			argv++;
			if (is_number(*argv) == false) {
				printf("Invalid \'number of hardlinks\' argument\n");
				exit(EXIT_FAILURE);
			}
			has_nlink = true;
			nlink = strtoul(*argv, 0L, 10);
		} else if(strcmp(*argv, "-exec") == 0) {
			argv++;
			has_exec_file = true;
			exec_file = *argv;
		}
		argv++;
	}
	while ((dir = readdir(directory)) != NULL) {
		struct stat file_stat;
		char file_path[1024];
		strcpy(file_path, directory_path);
		strcat(file_path, dir->d_name);
		if (stat(file_path, &file_stat) == -1) {
			perror("Failed to get files stats");
			exit(EXIT_FAILURE);
		}
		if (S_ISREG(file_stat.st_mode)) {
			if (has_name == true && strcmp(dir->d_name, name) != 0) {
				continue;
			}
			if (has_inode == true && inode != file_stat.st_ino) {
				continue;
			}
			if(has_nlink == true  && nlink != file_stat.st_nlink) {
				continue;
			}

			if(has_size == true) {
				if(sign == '=' && size != file_stat.st_size) {
					continue;
				}
				if(sign == '+' && size >= file_stat.st_size) {
					continue;
				}
				if(sign == '-' && size <= file_stat.st_size) {
					continue;
				}
			}

			if(has_exec_file == true) {
				char *const a[3] = {exec_file, file_path, (char*)0};
				launch(a);
			} else {
				printf("%s\n", file_path);	
			}	
		}
	}

	if (closedir(directory) == -1) {
		perror("Failed to close directory");
	}
	return 1;
} 

int main(int argc, char ** argv) {
	
	parse(argv);

	return EXIT_SUCCESS;
}
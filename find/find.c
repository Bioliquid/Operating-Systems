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

//find conditions declaration
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


int filter(struct stat file_stat, char *filename) {
    if (has_name == true && strcmp(filename, name) != 0) {
		return 0;
	}
	if (has_inode == true && inode != file_stat.st_ino) {
		return 0;
	}
	if(has_nlink == true  && nlink != file_stat.st_nlink) {
		return 0;
	}
	if(has_size == true) {
		if(sign == '=' && size != file_stat.st_size) {
			return 0;
		}
		if(sign == '+' && size >= file_stat.st_size) {
			return 0;
		}
		if(sign == '-' && size <= file_stat.st_size) {
			return 0;
		}
	}
    return 1;
}

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

void help() {
	printf("Function \'find\' takes odd number of arguments:\n");
	printf("First argument is a path to search place\n");
	printf("\'find\' can take several options:\n");
	printf("\t\'-name\' takes a string which describes file name\n");
	printf("\t\'-size\' takes a char followed by a positive integer which describes size of file\n");
	printf("\t\t(+) - greater than size\n");
	printf("\t\t(-) - less than size\n");
	printf("\t\t(=) - equal to size\n");
	printf("\t\'-inum\' takes a positive integer which describes inode of file\n");
	printf("\t\'-nlinks\' takes a positive integer which describes number of hardlinks\n");
	printf("\t\'-exec\' takes a string which describes a path to executable file\n");
	exit(EXIT_FAILURE);
}

void listdir(char *dirpath) {
	DIR *directory;
	if ((directory = opendir(dirpath)) == NULL) {
		printf("Failed to open directory %s: %d\n", dirpath, errno);
		if (closedir(directory) == -1) {
			printf("Failed to close directory %s: %d\n", dirpath, errno);
		}
		return;
	}
	char directory_path[1024];
	
	strcpy(directory_path, dirpath);
	strcat(directory_path, "/");

	struct dirent *dir;
	while ((dir = readdir(directory)) != NULL) {
		struct stat file_stat;
		char file_path[1024];
		strcpy(file_path, directory_path);
		strcat(file_path, dir->d_name);
		if (stat(file_path, &file_stat) == -1) {
			printf("Failed to get stat of file %s: %d\n", file_path, errno);
			exit(EXIT_FAILURE);
		}
		if (dir->d_type == DT_DIR) {
			if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
				listdir(file_path);
			}
		} else if (filter(file_stat, dir->d_name)) {
			if(has_exec_file == true) {
				char *const a[3] = {exec_file, file_path, NULL};
				launch(a);
			} else {
				printf("%s\n", file_path);	
			}	
		}
	}

	if (closedir(directory) == -1) {
		printf("Failed to close directory %s: %d\n", dirpath, errno);
	}
}

void parse(char **argv) {
	char *dirpath = *argv;
	++argv;

	while (*argv) {
		if (strcmp(*argv, "-name") == 0) {
			++argv;
			has_name = true;
			name = *argv;	
		} else if(strcmp(*argv, "-size") == 0) {
			++argv;
			char * pointer = *argv + 1;
			has_size = true;
			sign = **argv;
			size = atoll(pointer);
		} else if(strcmp(*argv, "-inum") == 0) {
			++argv;
			has_inode = true;
			inode = strtoul(*argv, 0L, 10);
		} else if(strcmp(*argv, "-nlinks") == 0) {
			++argv;
			has_nlink = true;
			nlink = strtoul(*argv, 0L, 10);
		} else if(strcmp(*argv, "-exec") == 0) {
			++argv;
			has_exec_file = true;
			exec_file = *argv;
		} else {
			printf("Invalid argument %s\n", *argv);
			help();
		}
		++argv;
	}

	listdir(dirpath);
} 

int main(int argc, char **argv) {
	if (argc % 2 == 1) {
		help();
	}
	parse(++argv);

	return EXIT_SUCCESS;
}
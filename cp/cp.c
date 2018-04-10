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
#include <fcntl.h>

char *get_folder(const char *path) {
	int j = -1;
	for (int i = 0; i < strlen(path); ++i) {
		if (path[i] == '/') {
			j = i;
		}
	} 
	if (j == -1) {
		return "";
	}
	char *temp = malloc((j + 1) * sizeof(char));
	for (int i = 0; i < j; ++i) {
		temp[i] = path[i];
	}
	temp[j] = '\0';
	return temp;
}

int read_and_write(const char *from, const char *to) {
	int fd_from, fd_to;

	if ((fd_from = open(from, O_RDONLY)) == -1) {
		fprintf(stderr, "Failed to open file: %s\nerror code: %d\n", from, errno);
		perror("");
		return 1;
	}
	char *folder = get_folder(to);
	if (strcmp(folder, "") != 0 && strcmp(folder, ".") != 0 && strcmp(folder, "..") != 0 && mkdir(folder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
		if (errno != EEXIST) {
			fprintf(stderr, "Failed to create directory %s: %d\n", folder, errno);
			perror("");
			free(folder);
			return 1;
		}
	}
	free(folder);

	if ((fd_to = open(to, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == -1) {
		fprintf(stderr, "Failed to open file: %s\nerror code: %d\n", to, errno);perror("");
		return 1;	
	}

	char buffer[4096];
	ssize_t buffered;
	for (; (buffered = read(fd_from, buffer, sizeof(buffer))) > 0;) {
		if (buffered == -1) {
			if (errno != EINTR) {
				fprintf(stderr, "Failed to read full file content: %s\nerror code: %d\n", from, errno);
				perror("");
				return 1;	
			} else {
				continue;
			}
		}
		char *to_write = buffer;
		ssize_t written;

		do {
			if ((written = write(fd_to, to_write, buffered)) >= 0) {
				buffered -= written;
				to_write += written;
			} else if (errno == EINTR) {
				continue;
			} else {
				fprintf(stderr, "Failed to write full file content: %s\nerror code: %d\n", to, errno);
				perror("");
				return 1;
			}
		} while (buffered > 0);
	}
	if (close(fd_from) == -1) {
		fprintf(stderr, "Failed to close file: %s\nerror code: %d\n", from, errno);
		perror("");
		return 1;
	}
	if (close(fd_to) == -1) {
		fprintf(stderr, "Failed to close file: %s\nerror code: %d\n", to, errno);
		perror("");
		return 1;
	}
	return 0;
}

int file_type(const char *path) {
	struct stat statbuf;
	if (stat(path, &statbuf) == -1) {
		return -1;
	}
	if (S_ISREG(statbuf.st_mode)) {
		return 0;
	}
	if (S_ISDIR(statbuf.st_mode)) {
		return 1;
	}
	return 0;
}



int cp(const char *from, const char *to) {
	int type = file_type(from);
	int to_type = file_type(to);
	if (type == 0) {
		if (to_type == 0 || to_type == -1) {
			read_and_write(from, to);
		} else if (to_type == 1) {
			fprintf(stderr, "Can't write file \'%s\' to directory \'%s\'\n", from, to);
			return 1;
		}
	} else if (type == 1) {
		if (to_type == 0) {
			fprintf(stderr, "Can't copy directory \'%s\' to file \'%s\'\n", from, to);
			return 1;
		}
		DIR *directory;
		if ((directory = opendir(from)) != NULL) {
			struct dirent *dir;
			while ((dir = readdir(directory)) != NULL) {
				char new_from[1024];
				snprintf(new_from, sizeof(new_from), "%s/%s", from, dir->d_name);
				char new_to[1024];
				snprintf(new_to, sizeof(new_to), "%s/%s", to, dir->d_name);
				if (dir->d_type == DT_DIR) {
					if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
						cp(new_from, new_to);
					}
				} else {
					read_and_write(new_from, new_to);
				}
			}
		} else {
			fprintf(stderr, "Failed to open directory %s: %d\n", from, errno);
			perror("");
			return 1;
		}
	} else {
		fprintf(stderr, "Failed to access the file: %s\nerror code: %d\n", to, errno);
		perror("");
		return 1;
	}
	return 0;
}

void help() {
	printf("Usage of \'cp\' programm:\n");
	printf("./cp [name1] [name2]\n");
	printf("where\n");
	printf("\t-name1 is a file or a folder you want to copy from\n");
	printf("\t-name2 is a file or a folder you want to copy to\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	if (argc != 3) {
		help();
	}
	cp(argv[1], argv[2]);
	return EXIT_SUCCESS;
}
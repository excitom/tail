/**
 * A demonstration implementation of the Linux tail command.
 *
 * Tom Lang 3/2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>

int watch = 0;
int lines = 0;
char param[256] = "";
char path[256];
char ln[2];

void readContinuously(int f);
void readLastLines(int f, int lines);

int main (int argc, char **argv) {
	ln[1] = '\0';
	int c;
	while ((c = getopt(argc, argv, "0123456789fhn:")) != EOF)
		switch(c) {
			case 'f':
				watch = 1;
				break;
			case 'n':
				lines = atoi(optarg);
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				ln[0] = c;
				strcat(param, ln);
				break;
			case 'h':
				printf("Tom's tail command\n");
				printf("Options are:\n");
				printf("	-f = watch the file continuously\n");
				printf("	-n = display the last N lines\n");
				printf("\n");
				exit(0);
			default:
				printf("Unrecognized option %c ignored\n", (char)c);
				break;

		}
	if ((lines == 0) && (strlen(param) > 0)) {
		lines = atoi(param);
	}

	int f;
	if (optind < argc) {
		if (strlen(argv[optind]) > 255) {
			printf("File path too long\n");
			exit(1);
		}
		strcpy(path, argv[optind]);
		f = open(path, O_RDONLY);
		if (f == -1) {
			fprintf(stderr, "x %s", path);
			perror("Can't open file ");
			exit(1);
		}
	} else {
		// no path name, assume STDIN
		f = STDIN_FILENO;
	}
	
	if (watch) {
		readContinuously(f);
	}
	else if (lines > 0) {
		readLastLines(f, lines);
	}
}

void
readContinuously(int f) {
	int maxLen = 255;
	char *p = malloc(256);
	while (1) {
		ssize_t s = read(f, p, maxLen);
		if (s != -1) {
			fputs(p, stdout);
		}
		sleep(2);
	}
}

void
readLastLines(int f, int lines) {
	int endpos = lseek(f, 0, SEEK_END);
	if (endpos == -1) {
		perror("fseek: ");
		exit(1);
	}
	int count = 0;
	unsigned char c;
	while( count <= lines ) {
		if(lseek(f, -1, SEEK_CUR) == -1) {
			break;
		}
		if (read(f, &c, 1) == -1) {
			perror("No input");
			exit(1);
		}
		if (c == '\n') {
			count++;
		}
		if(lseek(f, -1, SEEK_CUR) == -1) {
			break;
		}
	}
	// if the last character read was not a newline,
	// then the -n parameter is more than the number of lines
	// in the file.
	int pos;
	if (c == '\n') {
		pos = lseek(f, 1, SEEK_CUR);		// skip leading newline
	} else {
		pos = lseek(f, 0, SEEK_CUR);
	}
	int size = endpos-pos;
	char *buffer = malloc(size);
	if (buffer == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}
	read(f, buffer, size);
	fwrite(buffer, size, 1, stdout);
}

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
char path[256];

void
readContinuously(FILE *f) {
	int maxLen = 255;
	char *p = malloc(256);
	while (1) {
		char *s = fgets(p, maxLen, f);
		if (feof(f)) {
			// last get returned < maxLen ?
			if (s != NULL) {
				fputs(p, stdout);
			}
			sleep(2);
			clearerr(f);		// reset EOF
		} else {
			fputs(p, stdout);
		}
	}
}

void
readLastLines(FILE *f, int lines) {
	if (fseek(f, 0, SEEK_END) == -1) {
		perror("fseek: ");
		exit(1);
	}
	int endpos = ftell(f);
	int count = 0;
	int c = 0;
	while( count <= lines ) {
		if(fseek(f, -1, SEEK_CUR) == -1) {
			break;
		}
		c = fgetc(f);
		if (c == EOF) {
			perror("No input");
			exit(1);
		}
		if (c == '\n') {
			count++;
		}
		if(fseek(f, -1, SEEK_CUR) == -1) {
			break;
		}
	}
	// if the last character read was not a newline,
	// then the -n parameter is more than the number of lines
	// in the file.
	if (c == '\n') {
		fseek(f, 1, SEEK_CUR);		// skip leading newline
	}
	int pos = ftell(f);
	int size = endpos-pos;
	char *buffer = malloc(size);
	fread(buffer, size, 1, f);
	fwrite(buffer, size, 1, stdout);
}

int main (int argc, char **argv) {
	int c;
	while ((c = getopt(argc, argv, "fhn:")) != EOF)
		switch(c) {
			case 'f':
				watch = 1;
				break;
			case 'n':
				lines = atoi(optarg);
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

	FILE *f;
	if (optind < argc) {
		if (strlen(argv[optind]) > 255) {
			printf("File path too long\n");
			exit(1);
		}
		strcpy(path, argv[optind]);
		f = fopen(path, "r");
		if (f == NULL) {
			fprintf(stderr, "x %s", path);
			perror("Can't open file ");
			exit(1);
		}
	} else {
		// no path name, assume STDIN
		f = stdin;
	}
	
	if (watch) {
		readContinuously(f);
	}
	else if (lines > 0) {
		readLastLines(f, lines);
	}
}

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
	if (optind < argc) {
		if (strlen(argv[optind]) > 255) {
			printf("File path too long\n");
			exit(1);
		}
		strcpy(path, argv[optind]);
	} else {
		path[0] = '\0';		// missing path name
	}
	
	FILE *f = fopen(path, "r");
	if (f == NULL) {
		fprintf(stderr, "x %s", path);
		perror("Can't open file ");
		exit(1);
	}
	if (watch) {
		printf("-f not yet implemented\n");
	}
	else if (lines > 0) {
		if (fseek(f, 0, SEEK_END) == -1) {
			perror("fseek: ");
			exit(1);
		}
		int endpos = ftell(f);
		int count = 0;
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
}

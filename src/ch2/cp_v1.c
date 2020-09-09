/*
 * Author:ELio Yang
 * Date  :2020/09/07
 * version : 0.1
 * feature : copy file
 * this is a program to achieve simple instruction : cp
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 4096
#define LESSARGEXIT 1
#define FAILEXIT 2
#define CREATEMODE 0644

/*
used to print error messages
*/
void oops(const char *s1, const char *s2);

int main(int argc, char const *argv[])
{
	/* decl file descriptor and buffer */
	__label__ ok;
	__label__ done;
	int src_fd;
	int dest_fd;
	char *buf[BUFSIZ];
	int num_char;

	/* arguments check */
	if (argc < 3) {
		fprintf(stderr, "usage: %s source destination \n", argv[0] + 2);
		exit(LESSARGEXIT);
	}

	if (argc == 4 || strcmp(argv[3],"-i")) {
		printf("Modify an existed file do you want to continue(y/n)");
		int c;
		while ((c = getc(stdin))) {
			if(c=='Y'||c=='y'){
				goto ok;
			}
			else if(c=='N'||c=='n'){
				goto done;
			}
			else{
				printf("\n please enter y/n\n");
			}
		}
	}
      ok:
	/* open files */
	if ((src_fd = open(argv[1], O_RDONLY)) == -1) {
		oops("Can't open ", argv[1]);
	}
	if ((dest_fd = creat(argv[2], CREATEMODE)) == -1) {
		oops("Can't creat", argv[2]);
	}

	/* write to buffer */
	while ((num_char = read(src_fd, buf, BUFSIZE)) > 0) {
		if (write(dest_fd, buf, num_char) != num_char) {
			/* reading num_char and write num_char */
			oops("Write error to ", argv[2]);
		}
	}
	if (num_char == -1) {
		oops("Read error from ", argv[1]);
	}

	/* check close */
	if (close(src_fd) == -1 || close(dest_fd) == -1) {
		oops("Error closing file", "");
	}
      done:
	return 0;
}

void oops(const char *s1, const char *s2)
{
	fprintf(stderr, "Error : %s ", s1);
	perror(s2);
	exit(FAILEXIT);
}

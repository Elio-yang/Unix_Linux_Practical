/*
 * Author:ELio Yang
 * Date  :2020/09/07
 * version : 0.1
 * feature : copy file
 * this is a program to achieve simple instruction : cp
 */

/*
 * How does it work?
 * through man cp :
 * cp src-file dest-file
 * open two file and read from src write to dest
 */

/*
 * Process:
 * Read two files --->get context from src ---> write to dest ---> close file
 * 
 * Function needed:
 * 
 *   ssize_t read(int fd,void *buf,size_t count)
 *   #include<unistd.h>  (means unix standard header) 
 *   fd: file descriptor(can be get from open)
 *   buf: write into this buffer
 *   count: how many bytes to read to 
 *   
 *   int open(char *name,int how)--->return a file descriptor
 *   #include<fcntl.h> (means file control)
 *   name:file path
 *   how: O_RDONLY O_WRONLY O_RDWT 
 *     
 *   int close(int fd)
 *   #include<unistd.h>
 *   fd: file descriptor
 *   return 0 successfuly closed
 *         -1 failed
 *   
 *   int creat(const char *pathname, mode_t mode);
 *   #include<fcntl.h>
 *   pathname:where is the file
 *   mode:default 0644 (-wr-r--r--)
 * 
 *   ssize_t num_written=write(int fd,void *buf,size_t amt)
 *   #include<unistd.h>
 *   
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

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

	int src_fd;
	int dest_fd;
	char *buf[BUFSIZ];
	int num_char;

	/* arguments check */
	if (argc != 3) {
		fprintf(stderr, "usage: %s source destination \n", argv[0] + 2);
		exit(LESSARGEXIT);
	}

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
	return 0;
}

void oops(const char *s1, const char *s2)
{
	fprintf(stderr, "Error : %s ", s1);
	perror(s2);
	/* The perror() function produces a message on standard error
	 * describing the last error encoun‐ tered during a call to a system
	 * or library function. 
	 */
	exit(FAILEXIT);
}

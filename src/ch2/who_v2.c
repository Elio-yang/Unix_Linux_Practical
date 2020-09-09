/*
 * Author:ELio Yang
 * Date  :2020/09/06
 * version : 0.2
 * feature : show correct user-document and time
 * this is a program to achieve simple instruction : who
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <utmp.h>
#include <time.h>
#define _NO_UT_TIME


#define OPEN_ERROR 1

/*
* get information from the structure utmp_buf
* which is defined in utmp.h to save imformation
* of the system user
*/
void show_info(struct utmp *utmp_buf);
/*
 * show time in a human read way 
 */
void show_time(time_t time);
int main(int argc, char const *argv[])
{
	struct utmp current_utmp;
	int utmp_fd;
	size_t size = sizeof(current_utmp);
	if ((utmp_fd = open(UTMP_FILE, O_RDONLY)) == -1) {
		perror(UTMP_FILE);
		exit(OPEN_ERROR);
	}
	while ((read(utmp_fd, &current_utmp, size)) == size) {
		show_info(&current_utmp);
	}
	close(utmp_fd);
	return 0;
}



void show_info(struct utmp *utmp_buf)
{
	__label__ done;
	if (utmp_buf->ut_type != USER_PROCESS) {
		goto done;
	}
	printf("%-8.8s", utmp_buf->ut_user);	/* log name */
	printf(" ");
	printf("%-8.8s", utmp_buf->ut_line);	/* device name */
	printf(" ");
	show_time((time_t) utmp_buf->ut_time);
	printf(" ");
      done:
	return;

#ifdef HOST
	printf("%s", utmp_buf->ut_host);	/* the host */
#endif
}

void show_time(time_t time)
{
	char *t = ctime(&time);
	printf("%20s", t + 4);
}

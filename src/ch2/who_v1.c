/*
 * Author:ELio Yang
 * Date  :2020/09/06
 * version : 0.1
 * feature : show who is using the system
 * this is a program to achieve simple instruction : who
 * */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <utmp.h>

#define OPEN_ERROR 1

void show_info(struct utmp *utmp_buf);
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
	if (utmp_buf->ut_type != USER_PROCESS) {
		return;
	}
	printf("%-8.8s", utmp_buf->ut_user);	/* log name */
	printf(" ");
	printf("%-8.8s", utmp_buf->ut_line);	/* device name */
	printf(" ");
	printf("%20d", utmp_buf->ut_tv.tv_sec);	/* login time */
	printf(" ");
#ifdef HOST
	printf("%s", utmp_buf->ut_host);	/* the host */
#endif
	printf("\n");
}

/*
 * Author:ELio Yang
 * Date  :2020/09/09
 * version : 0.4
 * feature : add logout function
 * this is a program to achieve simple instruction : who
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <utmp.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>

#define _NO_UT_TIME
#define OPEN_ERROR 1
#define FAILEXIT 2
#define BUFSIZE 16
#define UTSIZE (sizeof(struct utmp))
#define UTNULL ((struct utmp*)NULL)
#define SIZE BUFSIZE*UTSIZE

static char utmp_buffer[SIZE];
static int cnt_read;
static int cnt_cur;
static int utmp_fd = -1;

int open_utmp();

void close_utmp();

int load_utmp();

struct utmp *next_utmp();

void show_info(struct utmp *utmp_recd);

void show_time(time_t time);

void oops(const char *s1, const char *s2);

int log_tty(char *line);

int main(int argc, char const *argv[])
{
	struct utmp *utmp_recd;
	open_utmp();
	while ((utmp_recd = next_utmp()) != UTNULL) {
		show_info(utmp_recd);
	}
	close_utmp();
}

int open_utmp()
{
	utmp_fd = open(UTMP_FILE, O_RDONLY);
	if (utmp_fd == -1) {
		oops("Can't open :", UTMP_FILE);
	}
	cnt_cur = 0;
	cnt_read = 0;
	return utmp_fd;
}

void close_utmp()
{
	if (utmp_fd != -1) {
		close(utmp_fd);
	}
}

int load_utmp()
{
	int total_read = read(utmp_fd, utmp_buffer, UTSIZE);
	cnt_read = total_read / UTSIZE;
	cnt_cur = 0;
	return cnt_read;
}

struct utmp *next_utmp()
{
	struct utmp *next;
	if (utmp_fd == -1) {
		return UTNULL;
	}
	if (cnt_cur == cnt_read && load_utmp() == 0) {
		return UTNULL;
	}
	next = (struct utmp *)&utmp_buffer[cnt_cur++ * UTSIZE];
	return next;
}

void show_info(struct utmp *utmp_recd)
{
	__label__ done;
	if (utmp_recd->ut_type != USER_PROCESS) {
		goto done;
	}
	printf("%-8.8s", utmp_recd->ut_user);	/* log name */
	printf(" ");
	printf("%-8.8s", utmp_recd->ut_line);	/* device name */
	printf(" ");
	show_time((time_t) utmp_recd->ut_time);
	printf(" ");
      done:
	return;

#ifdef HOST
	printf("%s", utmp_recd->ut_host);	/* the host */
#endif
}

void show_time(time_t time)
{
	char *t = ctime(&time);
	printf("%20s", t + 4);
}

void oops(const char *s1, const char *s2)
{
	fprintf(stderr, "Error : %s ", s1);
	perror(s2);
	exit(FAILEXIT);
}

int log_tty(char *line)
{
	int fd;
	struct utmp recd;
	off_t len = (off_t) sizeof(struct utmp);
	int flag = -1;

	if ((fd = open(UTMP_FILE, O_RDONLY)) == -1) {
		return flag;
	}
	while (read(fd, &recd, len) == len) {
		recd.ut_type = DEAD_PROCESS;
		/* set type */
		if ((strncmp(recd.ut_line, line, sizeof(recd.ut_line))) == 0) {
			/* check name */
			if (time((time_t *) & recd.ut_time) != (-1)) {
				/* set time */
				if (lseek(fd, -len, SEEK_CUR) != (-1)) {
					/* set back */
					if (write(fd, &recd, len) == len) {
						/* update */
						flag = 0;
					}
				}
			}
		}
	}
	return flag;
}

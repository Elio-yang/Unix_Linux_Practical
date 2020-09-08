/*
 * Author:ELio Yang
 * Date  :2020/09/08
 * version : 0.3
 * feature : show correct user-document and time with buffer
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
#define FAILEXIT 2
#define BUFSIZE 16
#define UTSIZE (sizeof(struct utmp))
#define UTNULL ((struct utmp*)NULL)
#define SIZE BUFSIZE*UTSIZE

/*
 * utmp_buffer store 16 record at one time
 * cnt_read     how many structures have been read
 * cnt_cur      current position(which structure)
 * utmp_fd      file descriptor
 */
static char utmp_buffer[SIZE];
static int cnt_read;
static int cnt_cur;
static int utmp_fd = -1;

/*
 * open utmp file
 */
int 
open_utmp();

/*
 * close utmp file
 */
void
close_utmp();

/*
 * load to buffer
 */
int 
load_utmp();

/*
 * get next utmp record
 */
struct utmp * 
next_utmp();

/*
 * get information
 */
void 
show_info(struct utmp *utmp_recd);
/*
 * show time in a human read way 
 */
void 
show_time(time_t time);

void oops(const char *s1, const char *s2);

int 
main(int argc, char const *argv[])
{
        struct utmp * utmp_recd;
	open_utmp();
        while((utmp_recd=next_utmp())!=UTNULL){
                show_info(utmp_recd);
        }
        close_utmp();
}

int 
open_utmp()
{
        utmp_fd=open(UTMP_FILE,O_RDONLY);
        if(utmp_fd==-1){
               oops("Can't open :",UTMP_FILE);
        }
        cnt_cur=0;
        cnt_read=0;
        return utmp_fd;
}

void 
close_utmp()
{
        if(utmp_fd!=-1){
                close(utmp_fd);
        }
}

int 
load_utmp()
{
        int total_read=read(utmp_fd,utmp_buffer,UTSIZE);
        cnt_read=total_read/UTSIZE;
        cnt_cur=0;
        return cnt_read;
}

struct utmp * 
next_utmp()
{
        struct utmp *next;
        if(utmp_fd==-1){
                return UTNULL;
        }
        if(cnt_cur==cnt_read&&load_utmp()==0){
                return UTNULL;
        }
        next=(struct utmp *)&utmp_buffer[cnt_cur++*UTSIZE];
        return next;
}
void 
show_info(struct utmp * utmp_recd)
{
	__label__ done;
	if (utmp_recd->ut_type != USER_PROCESS) {
		goto done;
	}
	printf("%-8.8s", utmp_recd->ut_user);	/* log name */
	printf(" ");
	printf("%-8.8s", utmp_recd->ut_line);	/* device name */
	printf(" ");
	/* 
	 * Return a string of the form "Day Mon dd hh:mm:ss yyyy\n"
	 * that is the representation of TP in this format. 
	 * extern char *asctime (const struct tm *__tp) __THROW;
	 * Equivalent to `asctime (localtime (timer))'.  
	 * extern char *ctime (const time_t *__timer) __THROW;
	 */
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

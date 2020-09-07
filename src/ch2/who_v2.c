/*
 * Author:ELio Yang
 * Date  :2020/09/06
 * version : 0.2
 * feature : show correct user-document and time
 * this is a program to achieve simple instruction : who
 */

/*
 * How does it work?
 * through man who :
 * If FILE is not specified, use /var/run/utmp. /var/log/wtmp as FILE is common.
 * who read the file to get information of users.
 */

/*
 * Process:
 * Read the file --->get information ---> show information ---> close file
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
 */
/*
 * how to format this file?
 * -indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -fca -sc who_v2.c
 * indent can be get from: apt-get install indent
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <utmp.h>
#include <time.h>
#define _NO_UT_TIME

// utmp.h
//  #ifndef _NO_UT_TIME
//  /* We have a problem here: `ut_time' is also used otherwise.  Define
//     _NO_UT_TIME if the compiler complains.  */
//  # define ut_time    ut_tv.tv_sec

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

// defined in        /usr/include/x86_64-linux-gnu/bits/utmp.h
// refered from      /usr/inlcude/utmp.h
// struct utmp
// {
//   short int ut_type;                 /* Type of login.  */
//   pid_t ut_pid;                      /* Process ID of login process.  */
//   char ut_line[UT_LINESIZE]          /* Devicename.  */
//   char ut_id[4];                     /* Inittab ID.  */
//   char ut_user[UT_NAMESIZE]          /* Username.  */
//   char ut_host[UT_HOSTSIZE]          /* Hostname for remote login.  */
//   struct exit_status ut_exit;        /* Exit status of a process marked as DEAD_PROCESS.  */
// #if __WORDSIZE_TIME64_COMPAT32
//   int32_t ut_session;                /* Session ID, used for windowing.  */
//   struct
//   {
//     int32_t tv_sec;          /* Seconds.  */
//     int32_t tv_usec;         /* Microseconds.  */
//   } ut_tv;                   /* Time entry was made.  */
// #else
//   long int ut_session;               /* Session ID, used for windowing.  */
//   struct timeval ut_tv;              /* Time entry was made.  */
// #endif
//   int32_t ut_addr_v6[4];             /* Internet address of remote host.  */
//   char __glibc_reserved[20];         /* Reserved for future use.  */
// };

void show_info(struct utmp *utmp_buf)
{
	__label__ done;
	// in utmp.h
	// /* Values for the `ut_type' field of a `struct utmp'.  */
	// #define EMPTY 0 /* No valid user accounting information.  */

	// #define RUN_LVL 1 /* The system's runlevel.  */
	// #define BOOT_TIME 2 /* Time of system boot.  */
	// #define NEW_TIME 3 /* Time after system clock changed.  */
	// #define OLD_TIME 4 /* Time when system clock changed.  */

	// #define INIT_PROCESS 5 /* Process spawned by the init process.
	// */
	// #define LOGIN_PROCESS 6 /* Session leader of a logged in user.
	// */
	// #define USER_PROCESS 7 /* Normal process.  */
	// #define DEAD_PROCESS 8 /* Terminated process.  */
	if (utmp_buf->ut_type != USER_PROCESS) {
		goto done;
	}
	printf("%-8.8s", utmp_buf->ut_user);	/* log name */
	printf(" ");
	printf("%-8.8s", utmp_buf->ut_line);	/* device name */
	printf(" ");
	/* 
	 * Return a string of the form "Day Mon dd hh:mm:ss yyyy\n"
	 * that is the representation of TP in this format. 
	 * extern char *asctime (const struct tm *__tp) __THROW;
	 * Equivalent to `asctime (localtime (timer))'.  
	 * extern char *ctime (const time_t *__timer) __THROW;
	 */
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

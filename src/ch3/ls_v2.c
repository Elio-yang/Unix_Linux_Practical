/*
 * Author:ELio Yang
 * Date  :2020/09/11
 * version : 0.2
 * feature : list file/directory
 * this is a program to achieve simple instruction : ls
 */

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

/*-----------------------------------------------------------------------------
                                     File mask
-----------------------------------------------------------------------------*/
#define _S_IFTM 0170000

#define S_DIR 0040000
#define S_CHR 0020000
#define S_BLK 0060000
#define S_REG 0100000
#define S_FIFO 0010000
#define S_LINK 0120000
#define S_SOCK 0140000

/*-----------------------------------------------------------------------------
                       check the properties of file
-----------------------------------------------------------------------------*/
#define _S_ISTYPE(mode, mask) (((mode)&_S_IFTM) == (mask))
#define is_dir(mode) _S_ISTYPE((mode), S_DIR)
#define is_chr(mode) _S_ISTYPE((mode), S_CHR)
#define is_blk(mode) _S_ISTYPE((mode), S_BLK)
#define is_reg(mode) _S_ISTYPE((mode), S_REG)
#define is_fifo(mode) _S_ISTYPE((mode), S_FIFO)
#define is_link(mode) _S_ISTYPE((mode), S_LINK)
#define is_sock(mode) _S_ISTYPE((mode), S_SOCK)

/*-----------------------------------------------------------------------------
                       set user/group/other properties
-----------------------------------------------------------------------------*/
#define S_ID(mode, mask) (((mode)&mask))

#define S_IDRUSR 0400
#define S_IDWUSR 0200
#define S_IDXUSR 0100
#define S_WRXUSR (S_IDRUSR | S_IDWUSR | S_IDXUSR)

#define S_IDRGRP (S_IDRUSR >> 3)
#define S_IDWGRP (S_IDWUSR >> 3)
#define S_IDXGRP (S_IDXUSR >> 3)
#define S_RWXGRP (S_WRXUSR >> 3)

#define S_IDROTH (S_IDRGRP >> 3)
#define S_IDWOTH (S_IDWGRP >> 3)
#define S_IDXOTH (S_IDXGRP >> 3)
#define S_RWXOTH (S_RWXGRP >> 3)

#define ERROR 1
#define NAME_PTR 16
#define FILE_MAX 255

/*-----------------------------------------------------------------------------
                        structure to store information
-----------------------------------------------------------------------------*/
struct FILE_STAT {
	int id;
	struct stat f_stat;
	char *filename;
};
static struct FILE_STAT info[FILE_MAX];

/*-----------------------------------------------------------------------------
                        functions
-----------------------------------------------------------------------------*/
void do_ls(char direct[]);
void oops(const char *s1, const char *s2);
void print_info(int cnt);
void reload();
int mycomp(const void *a, const void *b);
void mode_to_str(mode_t mode, char str[]);
char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);
/*-----------------------------------------------------------------------------
                        main function
-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	/* argument check */
	if (argc == 1) {
		do_ls(".");
	} else {
		while (--argc) {
			printf("%s\n", *++argv);
			do_ls(*argv);
		}
	}
	return 0;
}

/*-----------------------------------------------------------------------------
                        list the directory(main part)
-----------------------------------------------------------------------------*/
void do_ls(char direct[])
{
	DIR *dir;
	/* pointer to a directory */
	struct dirent *dir_cur;
	int cnt = 0;
	/* pointer to a structure which store informations */
	if ((dir = opendir(direct)) == NULL) {
		oops("Can't view directory", direct);
	} else {
		while ((dir_cur = readdir(dir)) != NULL) {

			stat(dir_cur->d_name, &(info[cnt].f_stat));
			info[cnt].id = cnt;
			info[cnt].filename = dir_cur->d_name;
			cnt++;
			if (cnt == (FILE_MAX + 1)) {
				print_info(cnt);
				cnt = 0;
				reload();
			}
		}
		print_info(cnt);
		closedir(dir);
	}
}

/*-----------------------------------------------------------------------------
                              print error messages
-----------------------------------------------------------------------------*/
void oops(const char *s1, const char *s2)
{
	fprintf(stderr, "Error : %s ", s1);
	perror(s2);
	exit(ERROR);
}

/*-----------------------------------------------------------------------------
                       core of ls:sroting and printing 
-----------------------------------------------------------------------------*/
void print_info(int cnt)
{
	char mode_str[11];
	qsort(info, cnt, sizeof(struct FILE_STAT), mycomp);
	for (int i = 0; i < cnt; i++) {
		if (info[i].id == -1) {
			break;
		}

		mode_to_str(info[i].f_stat.st_mode, mode_str);
		printf("%s", mode_str);
		printf("%4d ", (int)info[i].f_stat.st_nlink);
		printf("%-8s", uid_to_name(info[i].f_stat.st_uid));
		printf("%-8s", gid_to_name(info[i].f_stat.st_gid));
		printf("%8ld ", (long)info[i].f_stat.st_size);
		printf("%.14s ",
		       4 + ctime((time_t *) & (info[i].f_stat.st_mtim)));
		printf("%s", info[i].filename);
		if (i < cnt - 1) {
			printf("\n");
		}
	}
	printf("\n");
}

/*-----------------------------------------------------------------------------
                reload the array:-1 means has been printed
-----------------------------------------------------------------------------*/
void reload()
{
	memset(info, -1, FILE_MAX * (sizeof(struct stat)));
}

/*-----------------------------------------------------------------------------
		mycomp for qsort: using struct-->filename
-----------------------------------------------------------------------------*/
int mycomp(const void *a, const void *b)
{
	return (strcmp
		(((struct FILE_STAT *)a)->filename,
		 ((struct FILE_STAT *)b)->filename));
}

/*-----------------------------------------------------------------------------
                set mode descriptor
-----------------------------------------------------------------------------*/
void mode_to_str(mode_t mode, char str[])
{
	strcpy(str, "----------");
	if (is_blk(mode))
		str[0] = 'b';
	if (is_chr(mode))
		str[0] = 'c';
	if (is_dir(mode))
		str[0] = 'd';
	if (is_fifo(mode))
		str[0] = 'p';
	if (is_link(mode))
		str[0] = 'l';
	if (is_reg(mode))
		str[0] = '-';
	if (is_sock(mode))
		str[0] = 's';

	if (S_ID(mode, S_IDRUSR))
		str[1] = 'r';
	if (S_ID(mode, S_IDWUSR))
		str[2] = 'w';
	if (S_ID(mode, S_IDXUSR))
		str[3] = 'x';
	if (S_ID(mode, S_IDRGRP))
		str[4] = 'r';
	if (S_ID(mode, S_IDRGRP))
		str[5] = 'w';
	if (S_ID(mode, S_IDXGRP))
		str[6] = 'x';
	if (S_ID(mode, S_IDROTH))
		str[7] = 'r';
	if (S_ID(mode, S_IDWOTH))
		str[8] = 'w';
	if (S_ID(mode, S_IDXOTH))
		str[9] = 'x';
}

/*-----------------------------------------------------------------------------
                uid to string if name==NULL then id
-----------------------------------------------------------------------------*/
char *uid_to_name(uid_t uid)
{
	struct passwd *pwd;
	static char p_name[17];
	if ((pwd = getpwuid(uid)) == NULL) {
		sprintf(p_name, "%d", uid);
		return p_name;
	} else {
		return pwd->pw_name;
	}
}

/*-----------------------------------------------------------------------------
                gid to string if name==NULL then id
-----------------------------------------------------------------------------*/
char *gid_to_name(gid_t gid)
{
	struct group *grp;
	static char g_name[17];
	if ((grp = getgrgid(gid)) == NULL) {
		sprintf(g_name, "%d", gid);
		return g_name;
	} else {
		return grp->gr_name;
	}
}

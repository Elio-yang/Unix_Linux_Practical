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

/*File mask*/
#define _S_IFTM 0170000

#define S_DIR 0040000
#define S_CHR 0020000
#define S_BLK 0060000
#define S_REG 0100000
#define S_FIFO 0010000
#define S_LINK 0120000
#define S_SOCK 0140000

/*check the properties of file*/
#define _S_ISTYPE(mode, mask) (((mode)&_S_IFTM) == (mask))
#define is_dir(mode) _S_ISTYPE((mode), S_DIR)
#define is_chr(mode) _S_ISTYPE((mode), S_CHR)
#define is_blk(mode) _S_ISTYPE((mode), S_BLK)
#define is_reg(mode) _S_ISTYPE((mode), S_REG)
#define is_fifo(mode) _S_ISTYPE((mode), S_FIFO)
#define is_link(mode) _S_ISTYPE((mode), S_LINK)
#define is_sock(mode) _S_ISTYPE((mode), S_SOCK)

/*set user/group/other properties*/
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

struct FILE_STAT {
	uid_t uid;
	gid_t gid;
	nlink_t links;
	size_t size;
	time_t mtime;
	mode_t mode;
	char *filename;
};

struct FILE_STAT *info[FILE_MAX];

void do_ls(char direct[]);
void oops(const char *s1, const char *s2);
void print_info(int cnt, struct stat *f_stat);
void reload();
int mycomp(const void *a, const void *b);
void mode_to_str(mode_t mode, char str[]);
void stat_file(char *filename);

char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);

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

			Name[cnt] = dir_cur->d_name;
			cnt++;
			if (cnt == (FILE_MAX + 1)) {
				print_info(cnt);
				cnt = 0;
			}
		}
		print_info(cnt);
		closedir(dir);
	}
}

void oops(const char *s1, const char *s2)
{
	fprintf(stderr, "Error : %s ", s1);
	perror(s2);
	exit(ERROR);
}

void print_info(int cnt, struct stat *f_stat)
{
	qsort(Name, cnt, sizeof(char *), mycomp);
	for (int i = 0; i < cnt; i++) {
		if (Name[i] == 0) {
			break;
		}

		printf("%20s     ", Name[i]);
		if ((i + 1) % 5 == 0) {
			printf("\n");
		}
	}
	printf("\n");
}

void reload()
{
	memset(Name, 0, FILE_MAX * (sizeof(char *)));
}

int mycomp(const void *a, const void *b)
{
	return (-strcmp
		(((struct FILE_STAT *)a)->filename,
		 ((struct FILE_STAT *)b)->filename));
}

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

void stat_file(char *filename)
{
	struct stat f_stat;
	if (stat(filename, &f_stat) == -1) {
		perror(filename);
	} else {

	}
}

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

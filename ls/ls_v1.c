/*
 * Author:ELio Yang
 * Date  :2020/09/10
 * version : 0.1
 * feature : list file/directory
 * this is a program to achieve simple instruction : ls
 */

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#define ERROR 1
#define NAME_PTR 16
#define FILE_MAX 255

static char* Name[FILE_MAX];
void do_ls(char direct[]);
void oops(const char *s1, const char *s2);
void print_info(int cnt);
void reload();
int mycomp(const void* a,const void* b);

int main(int argc,char *argv[])
{
        /*argument check*/
        if(argc==1){
                do_ls(".");
        }else{
                while(--argc){
                        printf("%s\n",*++argv);
                        do_ls(*argv);
                }
        }
        return 0;

}
void do_ls(char direct[])
{
        DIR * dir;
        /*pointer to a directory*/
        struct dirent * dir_cur;
        int cnt=0;
        /*pointer to a structure which store informations*/
        if((dir=opendir(direct))==NULL){
                oops("Can't view directory",direct);
        }else{
                while((dir_cur=readdir(dir))!=NULL){
                        Name[cnt]=dir_cur->d_name;
                        cnt++;
                        if(cnt==(FILE_MAX+1)){
                                print_info(cnt);
                                cnt=0;
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
void print_info(int cnt)
{
        qsort(Name,cnt,sizeof(char*),mycomp);
        for(int i=0;i<cnt;i++){
                if(Name[i]==0){
                        break;
                }
                printf("%20s     ",Name[i]);
                if((i+1)%5==0){
                        printf("\n");
                }
                
        }
        printf("\n");
}
void reload(){
        memset(Name,0,FILE_MAX*(sizeof(char*)));
}
int mycomp(const void* a,const void* b)
{
    return (-strcmp(*(char **)a,*(char**)b));
}
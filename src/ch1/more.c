/*
 * Author:ELio Yang
 * Date  :2020/09/02
 * this is a program to achieve simple order : more
 * */
#include <stdio.h>
#include <stdlib.h>
#define LINE 512
#define PANE 24

/*
 * get instructions:how many lines to return
 * Enter:one more line
 * Space:one more pane
 * q:quit
 * */
int get_instr();

/*
 * show the message delivered by get_instr
 * Each time one pane of message: 24 lines with max line_length 512
 * */
void show_more(FILE *);

/*
 * how to use more?
 * more filename(argv[1])
 * command | more
 * more < filename
 * */
int main(int argc, char *argv[])
{
        FILE *fp;
        if (argc==1){
                /*no filename we need to get from stdin*/
                show_more(stdin);
        } else{
                while (--argc){
                        if ((fp=fopen(*++argv."r"))!=NULL){
                                show_more(fp);
                                fclose(fp);
                        }else{
                                exit(1);
                        }
                }
        }
        return 0;
}

int get_instr()
{
        int c;
        /*need more show?*/
        printf("\033[7m more? \033[m");
        while ((c=getchar())!EOF){
                if (c=='q'){
                        exit(0);
                }
                if (c=='\n'){
                        return 1;
                }
                if (c==' '){
                        return PANE;
                }
        }
        return 0;
}

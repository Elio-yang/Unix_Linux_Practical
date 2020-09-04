/*
 * Author:ELio Yang
 * Date  :2020/09/02
 * this is a program to achieve simple instruction : more
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
 * 
 * this time an argument is added to make sure
 * the input will be get from users but not stdin
 * */
int
get_instr(FILE *cmd);

/*
 * show the message delivered by get_instr
 * Each time one pane of message: 24 lines with max line_length 512
 * */
void
show_more(FILE *);

/*
 * how to use more?
 * more filename(argv[1])
 * command | more
 * more < filename
 * */
int 
main(int argc, char *argv[])
{
        FILE *fp;
        if (argc==1){
         /*no filename we need to get from stdin*/
               show_more(stdin);
        /*for example ls /bin | ./more_v2*/
        } else{
                while (--argc){
                        if ((fp=fopen(*++argv,"r"))!=NULL){
                                show_more(fp);
                                fclose(fp);
                        }else{
                                exit(1);
                        }
                }
        }
        return 0;
}

int 
get_instr(FILE *cmd)
{
        int c;
        /*need more show?*/
        printf("\033[7m more? \033[0m");
        /*
        * here \033[7m is kind of control method
        * \033[7m means reverse the color
        * \033[0m means cancel the operation
        **/
        while ((c=getc(cmd))){
                if (c=='q'){
                        return 0;       /*q-->exit*/
                }
                if (c=='\n'){
                        return 1;       /*\n-->one more line*/
                }
                if (c==' '){
                        return PANE;    /*space-->one more pane(24lines)*/
                }
        }
        return 0;
}
void 
show_more(FILE * fp)
{
        char line[LINE];        /*buffer*/
        int num_of_line=0;      /*counting*/
        int reply=0;           /*from get_instr()*/
        FILE *fp_tty;           /*get from keyboard*/
        fp_tty=fopen("/dev/tty","r");
        if(fp_tty==NULL){
                exit(1);
        }

        while (fgets(line,LINE,fp)){
                if (num_of_line==PANE){
                /*full screen with 24 lines*/
                        reply=get_instr(fp_tty);
                        if (reply==0){
                                break;
                        }
                        num_of_line-=reply;     
                        /*reset the line*/
                }
                if ((fputs(line,stdout))==EOF){
                        exit(1);        
                /*output to stdout if EOF then exit*/
                }
                num_of_line++;                  
                /*count the line*/
        }
}

#include <stdio.h>
#include <string.h>

#define MAXLINE 256

char* make_command(char *id,char *device,char *data)
{
    static char str[100];
    static char *test;
    char *cmd="../can-utils/cansend can0 ";
    
    strcpy(str,cmd);
    strcat(str,id);
    strcat(str,"#");
    strcat(str,"0");
    strcat(str,device);
    strcat(str,data);
    test=str;
    return test;
    
}




int main()
{
    FILE *fp;
    int state;
    int i=0;
    char buff[MAXLINE];
    char *test;
   
    test=make_command("7df","1","22");
    printf("%s",test);

    fp = popen(test, "r");
    if (fp == NULL)
    {
        perror("erro : ");
        exit(0);
    }

}

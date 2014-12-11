#include <stdio.h>
#include <string.h>

#define MAXLINE 256
int musicNumber=1;
int status=0;


void audioPlayer(char CMD)
{

    static char str[100];
    char *aplay ="sudo aplay ";
    char *listOfMusic[]={"bro.wav","inno.wav","park.wav","you.wav"};
    strcpy(str,aplay);
    strcat(str,listOfMusic[musicNumber]);
     
        switch (CMD)
        {
            case '6':   if(status==0)
                        {popen(str,"w");status=1;}
                        break;
            
            case '7' :  if(status==1)
                        {popen("sudo killall aplay","w");status=0;}
                        break;
            
            case '8' :  musicNumber--;
                        if(musicNumber>=0){}
                        else if(musicNumber<0){musicNumber=0;}

                        break;
            
            case '9' :  musicNumber++;
                        if((sizeof(listOfMusic)/sizeof(char*))<musicNumber)
                        {musicNumber=0;}
                        break;
            
            default  :  break;
        }




}


int main()
{
    FILE *fp;
    int state;
    int i=0;
    char buff[MAXLINE];
    char ID[3];
    char bytelength=0;
    char *data;    

    fp = popen("candump can0", "r");
    
    if (fp == NULL)
    {
        perror("erro : ");
        exit(0);
    }

    while(fgets(buff, MAXLINE, fp) != NULL)
    {
        data=buff;
        audioPlayer(data[19]);
        //if(data[19]=='8'||data[19]=='9'){audioPlayer('7');audioPlayer('6');}

    }

    state = pclose(fp);
    printf("state is %d\n", state);
}

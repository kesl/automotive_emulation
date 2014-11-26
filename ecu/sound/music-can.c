#include <stdio.h>
#include <string.h>

#define MAXLINE 256
unsigned int musicNumber=1;

void audioPlayer(char CMD)
{

    static char str[100];
    char *aplay ="sudo aplay ";
    char *listOfMusic[]={"alam.wav","reme.wav"};
    strcpy(str,aplay);
    strcat(str,listOfMusic[musicNumber]);
     
        switch (CMD)
        {
            case '6':   popen(str,"w");
                        break;
            
            case '7' :  popen("sudo killall aplay","w");
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

    fp = popen("./can-utils/candump can0", "r");
    
    if (fp == NULL)
    {
        perror("erro : ");
        exit(0);
    }

    while(fgets(buff, MAXLINE, fp) != NULL)
    {
        fgets(buff, MAXLINE, fp);
        data=buff;
        audioPlayer(data[19]);
    }

    state = pclose(fp);
    printf("state is %d\n", state);
}

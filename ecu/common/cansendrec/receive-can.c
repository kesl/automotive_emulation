#include <stdio.h>

#define MAXLINE 256


int main()
{
    FILE *fp;
    int state;
    int i=0;
    char buff[MAXLINE];
    char ID[3];
    char bytelength=0;
    char *data;    
    
    fp = popen("../can-utils/candump can0", "r");
    if (fp == NULL)
    {
        perror("erro : ");
        exit(0);
    }

    while(fgets(buff, MAXLINE, fp) != NULL)
    {
        data=buff;

        printf("test : %s\r\n",data);
        /*
        printf("test : %s", buff);
    
        for(i=0;i<40;i++)
        {
            printf("buffer : %c,%d \r\n",buff[i],i);
        }
         */

        printf("can : %c, id : %c%c%c, length: %c\r\n",data[5],data[8],data[9],data[10],data[15]);


        if(data[5]=='0')
        {
        //ID[0]=data[8]; ID[1]=data[9]; ID[2]=data[10];
        //bytelength=(int)(data[15]-48);

        //printf("length : %d id : %s ",bytelength,ID);
        
        for(i=0;i<8*3;i++)
        {
            //if((i+1)%3!=0)
            //{
                //data[i]=buff[19+i];printf("%c",data[i]);
                printf("%c",data[19+i]);
            //}
        }
        }
        //printf("data : %s",data);



        
    }

    state = pclose(fp);
    printf("state is %d\n", state);
}

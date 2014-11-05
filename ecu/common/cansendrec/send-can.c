#include <stdio.h>
#include <string.h>

#define MAXLINE 256

int Get_wheel_value(void)
{
    unsigned int wheel=0;
    int i=0;
    FILE *f;
    char Line[256];
    unsigned char value[3];
    f=fopen("save.txt","r");
    //fgets(Line,256,f);
    unsigned char test[100] ;
  
    fgets(test,100,f);
    //printf("%s\n",test);
   
    if(test[0]=='a')
    {

        if(test[6]=='.'){wheel=(test[5]-48);}
        else if(test[7]=='.'){wheel=(test[5]-48)*10+(test[6]-48);}
        else if(test[8]=='.'){wheel=(test[5]-48)*100+(test[6]-48)*10+(test[7]-48);}
        else ;

        if(test[4]=='1')
        {
            wheel = wheel + 1000;
        }
      
    }

    else if(test[0]=='b')
    {}
    else {;}


    //printf("%d\n",wheel);
    fclose(f);    
    return wheel;
}


char* make_command(char *id,char *device,char *data)
{
    static char str[100];
    static char *test;
    char *cmd="~/Desktop/sdcard1/cansource/can-utils/cansend can0 ";// 이건 나중에 수정 cansend 위치
    
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
    char *canmessage;
    int wheeldata=0; 
    char *id,*dev;
    int data=0;
    char hex[2];
    int i=0;
    char *newdata;
    int state;


    while(1){
    wheeldata = Get_wheel_value();
    id="7df";
    if(wheeldata<=1000)
    {
        dev="0";data=wheeldata;
    }
    else if(wheeldata>=1000)
    {
        dev="1";data=wheeldata-1000;
    }

    hex[0]=(data/16);hex[1]=(data%16);
    
    if(hex[0]<=9){hex[0]=hex[0]+48;}
    else if(hex[0]>9){hex[0]=hex[0]+87;}
    if(hex[1]<=9){hex[1]=hex[1]+48;}
    else if(hex[1]>9){hex[1]=hex[1]+87;}
    newdata=hex;
    canmessage=make_command(id,dev,newdata);

    fp = popen(canmessage, "r");
    if (fp == NULL)
    {
        perror("erro : ");
        exit(0);
    }
    state=fclose(fp);
    
    }

}

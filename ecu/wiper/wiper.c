#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>




#define MAXLINE     256
#define MSG_MemSet   0x01
#define MSG_DegSet   0x10
#define MSG_DegMem   0x11
#define MSG_DegMemX   0x12
#define MSG_DegMemT   0x14

#define MSG_SpdSet   0x20
#define MSG_SpdMem   0x21
#define MSG_SpdMemX   0x22
#define MSG_DegSpdSet  0x30
#define MSG_DegSpdMem  0x31
#define MSG_TickSet   0x40
#define MSG_Report   0x50
#define MSG_ReportTimeSet 0x51
#define MSG_ReportTimeSetX 0x52
#define MSG_AccelDivSet  0x60
#define MSG_AccelDivSetX 0x61
#define MSG_PwmStart  0x82
#define MSG_PwmStop   0x83

typedef unsigned char         	BOOLEAN;
typedef unsigned char           INT8U;
typedef char                    INT8S;
typedef unsigned short          INT16U;
typedef short                   INT16S;
typedef unsigned long           INT32U;
typedef long                    INT32S;
typedef unsigned long long      INT64U;
typedef long long               INT64S;
typedef float                   FP32;
typedef double                  FP64;



void Ack(void);
void WaitReport(void);
void DegSet(INT32U did, INT32U sid, INT32U acc, INT32U degree);
void MemSet(INT32U did);
void Report(INT32U did);
void SpeedSet(INT32U did, INT32U, INT32U spd);

enum comport_number { COM1, COM2, COM3, COM4 };

#define BAUDRATE B38400 //baudrate setting
//#define BAUDRATE B9600 //baudrate setting
#define _POSIX_SOURCE 1 // POSIX compliant source

struct termios oldtio, newtio; // for terminal attribute save & setting
int serial_fd;                 // serial port file descriptor

void gprintf(char *fmt,...)
{
   char buf[512];
   int ret;
   va_list ap;

   va_start(ap,fmt);
   vsprintf(buf, fmt, ap);
   va_end(ap);

   ret = write(serial_fd, (void *)buf, strlen(buf));
   if( ret < 0 )
   {
      printf("serial write error: %s\n", strerror(errno));
   }
}

int getch(void)
{
   int read_bytes;
   unsigned char buf;

   read_bytes = read(serial_fd, &buf, 1);   // 1 문자를 받으면 리턴
   if ( read_bytes > 0 )
      return (int)buf;
   if ( (read_bytes < 0) && (errno != EINTR) && (errno != EAGAIN) ) {
        printf("getch error\n");
       return -1;        
   }

}


INT8U UART0_getc(void) 
{
    return (INT8U)getch();
}
void UART0_putc(INT8U data)
{
    int ret;

    ret = write(serial_fd, (void *)&data, 1);
    if (ret < 0) {
        printf("putc error\n");
    }
        
}

void Ack(void)
{
    while(UART0_getc() != 0xAC);  
}

void WaitReport(void)
{
    while(UART0_getc() != 0xED);  
}

void DegSet(INT32U did, INT32U sid, INT32U acc, INT32U degree)
{
    UART0_putc((char)0x5);
    UART0_putc((char)0);
    UART0_putc((char)MSG_DegSet);
    UART0_putc((char)sid);
    UART0_putc((char)acc);
    UART0_putc((char)degree);
    Ack();
}
void SpeedSet(INT32U did, INT32U sid, INT32U spd)
{
    UART0_putc((char)0x4);
    UART0_putc((char)0);
    UART0_putc((char)MSG_SpdSet);
    UART0_putc((char)sid);
    UART0_putc((char)spd);
    Ack();
}

void MemSet(INT32U did)
{
    UART0_putc((char)0x2);
    UART0_putc((char)0);
    UART0_putc((char)MSG_MemSet);
    Ack();  
}

void Report(INT32U did)
{
    UART0_putc((char)0x2);
    UART0_putc((char)0);
    UART0_putc((char)MSG_Report);
    Ack();  
}


int serial_open(int which_port)
{
   char *port_str;

   switch(which_port)
   {
      case COM1:
         port_str = "/dev/ttyUSB0";
         break;
      case COM2:
         port_str = "/dev/ttyS1";
         break;
      case COM3:
         port_str = "/dev/ttyS2";
         break;
      case COM4:
         port_str = "/dev/ttyS3";
         break;
      default:
         printf("input serial port error\n");
         exit(EXIT_FAILURE);
   }

   serial_fd = open(port_str, O_RDWR | O_NOCTTY );
   if( serial_fd < 0 )
   {
      printf("serial_port open error: %s\n", port_str);
      exit(-1);
   }

   tcgetattr(serial_fd,&oldtio); // 현재 설정을 oldtio에 저장

   bzero(&newtio, sizeof(newtio));
   //newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
   newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD; // 흐름제어 없음
   newtio.c_iflag = IGNPAR;

   #if 0  // raw output
   newtio.c_oflag &= ~OPOST;
   #else
   // preprocessing output: 즉 output하기 전에 어떤 처리를 해서 ouput을 한다.
   newtio.c_oflag |= OPOST; // post processing enable
   newtio.c_oflag |= ONLCR; // 유닉스 용의 newline(NL:'\n')을 dos용의 newline인
                            // CR-NL('\r'\'n')으로 자동 전환 옵션
   #endif

   // set input mode (non-canonical, no echo,...)
   newtio.c_lflag = 0;

   newtio.c_cc[VTIME]    = 0;   // 문자 사이의 timer를 disable
   newtio.c_cc[VMIN]     = 1;   // 최소 5 문자 받을 때까진 blocking

   tcflush(serial_fd, TCIFLUSH);
   tcsetattr(serial_fd,TCSANOW,&newtio);

   return 0;
}

void serial_close(void)
{
   // 원래의 attribute로 돌려 놓는다.
   tcsetattr(serial_fd, TCSANOW, &oldtio);
   close(serial_fd);
}

int thread_arr[8];
int thread_id;




void test (void)
{

    int i=0;
    int did, sid, stage;
    int deg=260;
    
    did = 0;
    sid = 0;
    stage = 0;
    
    usleep(300000); 
    DegSet(did, sid, 3, deg);
    MemSet(did);
    Report(did);
    WaitReport();
    usleep(300000);
    deg=330;
    DegSet(did, sid, 3, deg);
    MemSet(did);
    Report(did);
    WaitReport();
    usleep(500000);
}






int main(void)
{
    int result = 0;
    int i = 0;
    FILE *fp;
    int state;
    char buff[MAXLINE];
    char ID[3];
    char bytelength=0;
    char *data;    

    serial_open(COM1);
 
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
        if(data[19]=='1')test();
    }

    state = pclose(fp);

    while (1)
        sleep(5);

    serial_close();
}

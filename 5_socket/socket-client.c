#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <dirent.h> 
#include <list>
#include <string>
#include <iostream>
#define BUFFER_SIZE 1024
using namespace std;

list<string> files;

// code for client for FIS
int sfd,wsfd;
char buffer[1024];
sockaddr_in serverAddr;
socklen_t addr_size;

void listDir() {
  DIR           *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (dir->d_type == DT_REG) {
       // printf("%s\n", dir->d_name);
       files.push_back(dir->d_name);
     }
   }
   closedir(d);
 }
}

char sendBuff[1025];  
int sockfd;
char recvBuff[1024];
struct sockaddr_in serv_addr;

int connect_to_fis() {
  sockfd = 0;

  memset(recvBuff, '0' ,sizeof(recvBuff));
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))< 0) {
    printf("\n Error : Could not create socket \n");
    return 1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(12000);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) {
    printf("\n Error : Connect Failed \n");
    return 1;
  }
}

void send_file_list() {
  strcpy(sendBuff, "File list sent by client");
  // write(sockfd, sendBuff, strlen(sendBuff));
struct sockaddr_storage sender;
socklen_t sendsize = sizeof(sender);
bzero(&sender, sizeof(sender));
sendto(sockfd, sendBuff, sizeof(sendBuff), 0, (struct sockaddr *)&sender, sendsize);
}


void receive_file_list () {
  int n = 0;
  while((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0) {
    recvBuff[n] = 0;
    if(fputs(recvBuff, stdout) == EOF) {
      printf("\n Error : Fputs error");
    }
    printf("\n");
  }
  // printf("<= %s\n", recvBuff);
}

void view_file_list () {
  for(std::list<string>::iterator list_iter = files.begin(); 
    list_iter != files.end(); list_iter++)
  {
    std::cout<<*list_iter<<endl;
  }
}

void request_file(string ip,string file,string saveas)
{
  int filesocket;   
  struct sockaddr_in fileserAddr;
  socklen_t addr_size;

  filesocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(12001);
  serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  addr_size = sizeof serverAddr;
  connect(filesocket, (struct sockaddr *) &serverAddr, addr_size);

  FILE *f=fopen(saveas.c_str(),"w");
  char buffer[BUFFER_SIZE];
  int t;
  while((t=recv(filesocket, buffer, BUFFER_SIZE, 0))>0)
  {
    // cout << t << endl;
    // for(int i=0;i<t;i++)
    //   putc(buffer[i],f);

  }
  fclose(f);
  close(filesocket);
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int sock, n;
unsigned int length;
struct sockaddr_in server, from;
struct hostent *hp;

void connectfis() {
  sock= socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) error("socket");

  server.sin_family = AF_INET;
  hp = gethostbyname("localhost");
  if (hp==0) error("Unknown host");

  bcopy((char *)hp->h_addr, 
       (char *)&server.sin_addr,
        hp->h_length);
  server.sin_port = htons(12000);
  length=sizeof(struct sockaddr_in);
}

string append_filenams() {
  string out;
  view_file_list();
  // for(std::list<string>::iterator list_iter = files.begin(); 
  //   list_iter != files.end(); list_iter++)
  // {
  //   std::cout<<*list_iter<<endl;
  // }
}
void init () {
  connectfis();
  printf("Please enter the message: ");
  string f = append_filenams();
  cout <<f;
  bzero(buffer,BUFFER_SIZE);
  fgets(buffer,BUFFER_SIZE,stdin);
  n=sendto(sock,buffer,
           strlen(buffer),0,(const struct sockaddr *)&server,length);
  if (n < 0) error("Sendto");
  n = recvfrom(sock,buffer,BUFFER_SIZE,0,(struct sockaddr *)&from, &length);
  if (n < 0) error("recvfrom");
  write(1,"Got an ack: ",12);
  write(1,buffer,n);
  close(sock);
}

int getDetails (string file) {
  file = "REQ"+file;
  connectfis();
  strcpy(buffer,file.c_str());
  n=sendto(sock,buffer,
           strlen(buffer),0,(const struct sockaddr *)&server,length);
  if (n < 0) error("Sendto");
  n = recvfrom(sock,buffer,BUFFER_SIZE,0,(struct sockaddr *)&from, &length);
  if (n < 0) error("recvfrom");
  write(1,"Got an ack: ",12);
  write(1,buffer,n);
  close(sock);
  if (buffer[0]=='-') return -1;
  return 0;
}

void update_file_list() {

}

int main(int argc, char *argv[])
{
  listDir();
  init();
  // connect_to_fis();
  // send_file_list();
  // receive_file_list();

  int input;
  string fname, save_as;
  while(1) {
    printf("What would you like to do?\n");
    printf("1. Update and view file list.\n");
    printf("2. Download a file.\n");
    printf("3. Shut down.\n");
    scanf("%d",&input);
    switch(input) {
      case 1:
        update_file_list();
        view_file_list();
        break;
      case 2: {
        printf("File name: ");
        cin >> fname;
        if (getDetails(fname) ==-1) {
          cout << "File not found." << endl;
          break;
        }
        printf("\nSave as: ");
        cin >> save_as;
        string ip(buffer,n);
        request_file(ip,fname,save_as);
      }
        break;
      case 3:
        break;
    }
    if(input==3) break;
  }
  
  return 0;
}

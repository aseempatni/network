#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <stdlib.h>
#define BUFFER_SIZE 1024

using namespace std;

int sfd,wsfd;
char buffer[1024];
sockaddr_in serverAddr;
socklen_t addr_size;  
void init_server()
{
  wsfd = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(12004);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  /*int iSetOption = 1;
  setsockopt(wsfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,
        sizeof(iSetOption));*/
  if(bind(wsfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)))
  {
    cout << "Error: Could not bind\n";
    exit(1);
  }

  if(listen(wsfd,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  struct sockaddr_storage serverStorage;
  addr_size = sizeof(serverStorage);
  sfd = accept(wsfd, (struct sockaddr *) &serverStorage, &addr_size);
}

string get_fname()
{
  char buffer[BUFFER_SIZE];
  int t = recv(sfd, buffer, BUFFER_SIZE, 0);
  string temp(buffer,t);
  return temp;
}

void send_file(string file)
{
  FILE *f=fopen(file.c_str(),"rb");
  char buffer[BUFFER_SIZE];

  while(!feof(f))
  {
    int sz=fread (buffer,1,BUFFER_SIZE,f);
    // printf("%s",buffer);
    send(sfd,buffer,sz,0);
  }

  fclose(f);
  close(wsfd);
  close(sfd);
}

int main(){
  while(1)
  {
    init_server();
    string fname=get_fname();
    send_file(fname);
    printf("sent.\n");
  }
  return 0;
}
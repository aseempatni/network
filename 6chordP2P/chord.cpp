#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#define BUFFER_SIZE 1024

using namespace std;

typedef unsigned long long int llu;

class node {
public:
	llu id;

	string ip;
	int port;
	string succ_add;
	int succ_port;
	string pred_add;
	int pred_port;
};

llu hash(string p, int len) { // consistent hashing
	llu h = 0;
	int i;
	for (i = 0; i < len; i++) {
		h += p[i];
		h += (h << 10);
		h ^= (h >> 6);
	}
	h += (h << 3);
	h ^= (h >> 11);
	h += (h << 15);
	return h;
}

node search(llu file_id);
llu map_file_to_node (llu file_id);

bool download (string filename, string saveas, node server)
{
	int filesocket;
	socklen_t addr_size;
	sockaddr_in serverAddr;

	filesocket = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(server.port);
	serverAddr.sin_addr.s_addr = inet_addr(server.ip.c_str());
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
	addr_size = sizeof serverAddr;
	if(connect(filesocket, (struct sockaddr *) &serverAddr, addr_size)<0)
	{
		cout << "Could nt connect.\n";
		return -1;
	}

	// Send file name
	char buffer[BUFFER_SIZE];
	strcpy(buffer,filename.c_str());
	send(filesocket,buffer,strlen(buffer),0);

	// Download start
	FILE *f=fopen(saveas.c_str(),"w");
	int t;
	while((t=recv(filesocket, buffer, BUFFER_SIZE, MSG_WAITALL))>40) {
		for(int i=0;i<t;i++)
			putc(buffer[i],f);
	}
	fclose(f);
	close(filesocket);

	cout << "File saved.\n";
	return 1;
}

void client()
{
	
}

//**********server code****************/
fd_set readset, tempset;
int maxfd, flags;
int srvsock, peersock, j, result, result1, sent, len;
timeval tv;
char buffer[BUFFER_SIZE];
sockaddr_in addr;
struct sockaddr_storage serverStorage;
socklen_t addr_size = sizeof(serverStorage);

int server(){
	srvsock = socket(PF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12002);
	addr.sin_addr.s_addr = INADDR_ANY;
	memset(addr.sin_zero, '\0', sizeof addr.sin_zero);
	int iSetOption = 1;
	setsockopt(srvsock, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,
		sizeof(iSetOption));
	if(bind(srvsock, (struct sockaddr *) &addr, sizeof(addr)))
	{
		cout << "Error: Could not bind\n";
		exit(1);
	}

	if(listen(srvsock,5)==0)
		printf("== Listening\n");
	else
		printf("Error\n");
	FD_ZERO(&readset);
	FD_SET(srvsock, &readset);
	maxfd = srvsock;

	do {
		memcpy(&tempset, &readset, sizeof(tempset));
		tv.tv_sec = 1000;
		tv.tv_usec = 0;
		result = select(maxfd + 1, &tempset, NULL, NULL, &tv);

		if (result == 0) {
			printf("select() timed out!\n");
		}
		else if (result < 0 && errno != EINTR) {
			printf("Error in select(): %s\n", strerror(errno));
		}
		else if (result > 0) {

			if (FD_ISSET(srvsock, &tempset)) {
				len = sizeof(addr);
				peersock = accept(srvsock, (struct sockaddr *) &serverStorage, &addr_size);
				if (peersock < 0) {
					printf("Error in accept(): %s\n", strerror(errno));
				}
				else {
					FD_SET(peersock, &readset);
					maxfd = (maxfd < peersock)?peersock:maxfd;
				}
				FD_CLR(srvsock, &tempset);
			}

			for (j=0; j<maxfd+1; j++) {
				if (FD_ISSET(j, &tempset)) {
					result = recv(j, buffer, BUFFER_SIZE, 0);
					string fname(buffer,result);

					if(result > 0) {
						buffer[result] = 0;
						printf("Opening: %s\n", buffer);
						sent = 0;

						FILE *f=fopen(fname.c_str(),"rb");
						struct stat st;
						int size;
						stat(fname.c_str(), &st);
						sprintf(buffer,"%d",(int)st.st_size);
						//	send(j,buffer,strlen(buffer),0);

						while(!feof(f))
						{
							result1=fread (buffer,1,BUFFER_SIZE,f);
							int sb = send(j,buffer,result1,0);
							cout << result1 << endl;
						}
						fclose(f);
						cout << "Close File\n" << endl;
						if (result1 > 0)
							sent += result1;
						else if (result1 < 0 && errno != EINTR)
							break;
						cout << "File sent: " << result1 << endl;
						close(j);
						FD_CLR(j, &readset);
					}
					else {
						printf("Error in recv(): %s\n", strerror(errno));
					}
           }      // end if (FD_ISSET(j, &tempset))
        }      // end for (j=0;...)
     }      // end else if (result > 0)
 } while (1);
}


int main () {
	if(fork())
	{
		client();
	}
	else
	{
		server();
	}
}


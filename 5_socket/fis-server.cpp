#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <map>
#include <string>
#include <iostream>
#include <vector>
#define NEW 1
#define REQ 2
#define UPD 3
#define BUFFER_SIZE 1024

using namespace std;

// Mapping
std::map<string, string> filemap;
void showFileMap() {
  // showing contents:
	std::cout << "File Mapping:\n";
	std::map<string,string>::iterator it;
	for (it=filemap.begin(); it!=filemap.end(); ++it)
		std::cout << it->second << " => " << it->first << '\n';
}
int addfile (string filename, string ip) {
	filemap.insert ( std::pair<string,string>(filename,ip) );
}
void error(const char *msg)
{
	perror(msg);
	exit(0);
}
string buftostr(char* buf, int size) {
	string s(buf,size);
	s = s.substr(3);
	// cout << "After substr " << s;
	return s;
}

void get_ip(char* buf, int size) {
	string ip = "127.0.0.1";
	string file = buftostr(buf,size);
	cout << "Received request for " << file << endl;
	if ( filemap.find(file) == filemap.end() ) {
		cout << "File not found." << endl;
		sprintf (buf, "-");
	} else {
		strcpy(buf, filemap[file].c_str());
	}

	// strcpy(buf, ip.c_str());
  // bzero(buf,BUFFER_SIZE);
  // fgets(buf,BUFFER_SIZE,"127.0.0.1");
}
string getType(string a) {
	return a.substr(0,3);
	a = a.substr(4);
}
std::vector<std::string> split(std::string str, const char* delim) {
	std::vector<std::string> v;
	std::string tmp;

	for(std::string::const_iterator i = str.begin(); i <= str.end(); ++i) {
		if(*i != *delim && i != str.end()) {
			tmp += *i;
		} else {
			if (tmp.length() > 0) {
				v.push_back(tmp);
				cout << tmp;
			}
			tmp = "";
		}
	}
	return v;
}
void Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters = ":")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
string append_filenams() {
  string out;
  // view_file_list();
	std::map<string,string>::iterator it;
	for (it=filemap.begin(); it!=filemap.end(); ++it)
		out += it->first + ":";
  return out;
}
int main () {
	// addfile("a.mp3","127.0.0.1");
	showFileMap();

	int sock, length, n;
	socklen_t fromlen;
	struct sockaddr_in server;
	struct sockaddr_in from;
	char buf[1024];


	sock=socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) error("Opening socket");
	length = sizeof(server);
	bzero(&server,length);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(12000);
	if (bind(sock,(struct sockaddr *)&server,length)<0) 
		error("binding");
	fromlen = sizeof(struct sockaddr_in);
	string type;
	while (1) {
		n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
		printf("Datagram's IP address is: %s\n", inet_ntoa(from.sin_addr));
		printf("Datagram's port is: %d\n", (int) ntohs(from.sin_port));
		if (n < 0) error("recvfrom");

		type = getType(buf);
		// cout << type << endl;
		if (strcmp(type.c_str(),"REQ")==0) {
			// A request for file
			printf("This is a file request.\n");
			get_ip(buf, n);
		}
		else if (strcmp(type.c_str(),"ADD")==0) {
			// A list of files
			printf("This is file list\n");
			string flist = buftostr(buf, n);
			vector<string> tokens;
			Tokenize(flist, tokens);
			std::vector<std::string> vv = tokens;
			for (std::vector<string>::iterator it = vv.begin() ; it != vv.end(); ++it){
				// std::cout << ' ' << *it << endl;
				filemap[*it] = inet_ntoa(from.sin_addr);
			}
			showFileMap();
			strcpy(buf,"Files added\n");
		}
		else if (strcmp(type.c_str(),"UPD")==0) {
			// A list of files
			printf("File list requested.\n");
			string f = append_filenams();
			strcpy(buf,f.c_str());
		}
		// write(1,"Received request for: ",21);
		// write(1,buf,n);
       // printf("%s", buf);
		n = sendto(sock,buf,strlen(buf),
			0,(struct sockaddr *)&from,fromlen);
		if (n  < 0) error("sendto");
	}

/*
  int listenfd = 0,connfd = 0;
  
  struct sockaddr_in serv_addr;
 
  char sendBuff[1025];  
  int numrv;  
  char recvBuff[1024];
 
  listenfd = socket(AF_INET, SOCK_DGRAM, 0);
  printf("socket retrieve success\n");
  
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));
      
  serv_addr.sin_family = AF_INET;    
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  serv_addr.sin_port = htons(10000);    
 
  int status = bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
  int n=0;

  */


  // if(listen(listenfd, 10) == -1){
  //     printf("Failed to listen\n");
  //     return -1;
  // }


/*
  int type;
  while(1) {
  	printf("LIstening\n");
    // connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request
		printf("Here\n");
		struct sockaddr_storage sender;
		socklen_t sendsize = sizeof(sender);
		bzero(&sender, sizeof(sender));
		recvfrom(listenfd, recvBuff, sizeof(recvBuff), 0, (struct sockaddr*)&sender, &sendsize);
		printf("Here\n");
		
		type = NEW;
    switch(type) {
    	case REQ: // Request for ip
		  	break;
    	case NEW: 
    		// A new client is added
	    	// Get its file list
    		// printf("Receiving file list.\n");
 				// while((n = read(connfd, recvBuff, sizeof(recvBuff)-1)) > 0) {
		   //  	recvBuff[n] = 0;
		   //  	if(fputs(recvBuff, stdout) == EOF) {
		   //  		printf("\n Error : Fputs error");
		   //  	}

		   //  	printf("%d\n", n);
		   //  	printf("\n");
		   //  }
			  // printf("<= %s\n", recvBuff);
		   //  printf("Received file list.\n");



		    // strcpy(sendBuff, "This is updated file list sent by fis");
		    // write(connfd, sendBuff, strlen(sendBuff));
  	  	break;
	  	case UPD: // Send updated list of files
  	  	strcpy(sendBuff, "This is a file list");
		    write(connfd, sendBuff, strlen(sendBuff));
	    	break;	
    }
    close(connfd);
    sleep(1);
  }

*/
}
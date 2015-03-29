#ifndef NODE_H
#define NODE_H
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/stat.h>

#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include "message.h"
#include "utils.h"

#include <list>
#include <map>
#include <dirent.h> 
#include <netdb.h>

#define BUFFER_SIZE 1024
#define INFO 100
#define REAL 101

#define MAX_BUFFER_SIZE 1024
#define MAX_CLIENTS 2
using namespace std;

class node {
	llu id;
	string ip;
	string hash_in ();
	int port;
	int sock, length, n;
	socklen_t fromlen;

	struct sockaddr_in my_sock; // bound to node
	struct sockaddr_in other_sock; // used to send and receive msg
	char buf[MAX_BUFFER_SIZE];
	int count; // count of other nodes, this will be removed later.
public:
	node(int port);
	node(struct sockaddr_in me);
	node* successor();
	node* predecessor();
	void recv_msg();
	void init_udp();
	void run_file_server();
	void send_msg(string ip, int port, string type, string msg);
	void send_msg(struct sockaddr_in to, string type, string msg);
	void update_neighbors();
	void showFileMap();
	int addfile (string filename, string ip);
	string get_addr(string filename);
	int download(string filename, string saveas);
	bool download_file (string filename, string savaas, string ip, int port);
	void search(string filename);
	void forward_msg(node* to, message m);
	void print();
	void printreq();

	void share_files();
	void req_share_files();

	void handle_neighbor(message msg);
	string getaddr();
	void printIndex();
private:
	node* successor_node;
	node* predecessor_node;
	void process_msg(message msg);
	std::map<string, string> filemap;
	// std::map<string, string> download_queue;
};

#endif
#ifndef NODE_H
#define NODE_H
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/stat.h>

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "message.h"
#include "location.h"
#include "finger_table.h"

#include <list>
#include <map>
#include <dirent.h> 
#include <netdb.h>

#define BUFFER_SIZE 1024
#define INFO 100
#define REAL 101

#define SHARE 1
#define SEARCH 2
#define DOWNLOAD 3
#define STATS 4


#define MAX_BUFFER_SIZE 1024
#define MAX_CLIENTS 2
using namespace std;

class node : public location {
	int sock, length, n;
	socklen_t fromlen;

	struct sockaddr_in other_sock; // used to send and receive msg
	char buf[MAX_BUFFER_SIZE];
public:
	node(int port);
	location* successor();
	location* predecessor();
	void recv_msg();
	void init_udp();
	void run_file_server();
	void run();
	void send_msg(string ip, int port, string type, string msg);
	void send_msg(struct sockaddr_in to, string type, string msg);
	void send_msg(location* loc, string type, string msg);
	void update_neighbors();
	void showFileMap();
	int addfile (string filename, string ip);
	string get_addr(string filename);
	int download(string filename, string saveas);
	bool download_file (string filename, string savaas, string ip, int port);
	void search(string filename);
	void forward_msg(location* to, message m);
	void print();
	void printreq();
	void print_ft ();

	void share_files();
	void req_share_files();
	void update_fingers();
	void handle_neighbor(message msg);
	void printIndex();
	void stabalize();
	// void insert_before (string addr_str);
	void insert_after (string addr_str);

	location* finger[MBIT];
	void update_ft(location *loc);
	location* closest_successor(llu k);
private:
	location* successor_node;
	location* predecessor_node;
	void process_msg(message msg);
	std::map<string, string> filemap;
	// std::map<string, string> download_queue;
};

#endif
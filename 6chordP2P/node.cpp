#include "utils.h"
#include "node.h"

location* node::successor() {
	return this-> successor_node;
}

location* node::predecessor() {
	return this-> predecessor_node;
}

void node::run() {
	init_udp();
	if(port!=9003) {
		update_neighbors();
		update_fingers();
	}
	if (fork()) {
		run_file_server();
		exit(0);
	}
	else {
		if(fork()) {
			// server
			while(1) {
				recv_msg();
			}
			exit(0);
		}
		else {
			// client
			while (1) {
				cout << "What would you like to do?" << endl;
				// cout << "0  Nothing ()" << endl;
				cout << "1  Share my files." << endl;
				cout << "2  Search a file." << endl;
				cout << "3  Download a file." << endl;
				cout << "4  Get Stats" << endl;
				cout << "5  Leave" << endl;
                int input;
				cin >> input;
				string filename, saveas;
				switch(input) {
					case SHARE:
					req_share_files();
					break;
					case SEARCH:
					cout << "Enter filename: ";
					cin >> filename;
					search(filename);
					break;
					case DOWNLOAD:
					cout << "Enter filename: ";
					cin >> filename;
					cout << "Save as: ";
					cin >> saveas;
					download(filename, saveas);
					break;
					case STATS:
					printreq();
					break;
                    case LEAVE:
                    leave_req();
                    break;
                    case 0:
					break;
				}
			}	
			exit(0);
		}
	}
}

node::node(int port = 9003) :location("127.0.0.1",port){
	ip = "127.0.0.1";
	my_sock.sin_family=AF_INET;
	my_sock.sin_addr.s_addr=inet_addr(ip.c_str());
	my_sock.sin_port=htons(port);
	this->port = port;
	this->ip = ip;
	string pstr = getaddr();
	id = hash(pstr);

	successor_node = NULL;
	predecessor_node = NULL;
	cout << "### New node created at port " << port << endl;
	for(int i=0; i<MBIT; i++) {
		finger[i] = new location(ip, port);
	}
}

void node::update_neighbors() {
	cout << "Querying neighbors" << endl;
	send_msg("127.0.0.1",9003,"NBR",getaddr());
	// Ask node 0 for the neighbors
}

void node::update_ft (location* loc) {
	llu x;
	// update self finger table
	for(int i=0; i<MBIT; i++) {
		x = (id + (llu)pow(2,i)) % MAX_ID;
		// need to handle boundary case here
		if(finger[i]->id >= x) {
			// Normal case
			if(loc->id >= x && loc->id < finger[i]->id) {
				finger[i] = loc;
				printf("Finger[%d] = ",i+1);
				cout << finger[i]->getaddr() << " ";
				cout << "(" << finger[i]->id << ")" << endl;
			}
		}
		else {
			// boundary case
			if(loc->id >= x || loc->id < finger[i]->id) {
				finger[i] = loc;
				printf("Finger[%d] = ",i+1);
				cout << finger[i]->getaddr() << " ";
				cout << "(" << finger[i]->id << ")" << endl;
			}

		}
	}
	// signal the joining node to update its finger table
	for(int i=0; i<MBIT; i++) {
		x = (loc->id + (llu)pow(2,i))%MAX_ID;
		// need to handle boundary case here
		if(predecessor()->id < id) {
			// Normal case
			if(x<=id && x>predecessor()->id) {
				cout << "1" << endl;
				send_msg(loc,"ADD_FINGER", tostr(i) + ":" + getaddr());
				// break;
			}
		}
		else {
			// boundary case
			if(id >= x || predecessor()->id < x) {
				cout << "2" << endl;
				send_msg(loc,"ADD_FINGER", tostr(i) + ":" + getaddr());
				// break;
			}
		}
	}
}

void node::update_fingers() {
	sleep(1);
	cout << "Updating finger table" << endl;
	send_msg(my_sock,"CLI_UPD_FINGER",getaddr());
	// share_files();
	// Ask node 0 for the neighbors
}

void node::recv_msg() {
	n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&other_sock,&fromlen);
	printf("%s:%d", inet_ntoa(other_sock.sin_addr), (int) ntohs(other_sock.sin_port));
	cout << " ==> " << buf << endl;
	if (n < 0) error("recvfrom");
	message msg(buf, n);
	msg.from = other_sock;
	process_msg(msg);
}

void node::leave_req() {
	send_msg(my_sock, "LEAVE", tostr(port)); 
} 

void node::init_udp() {
	sock=socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) error("Opening socket");
	length = sizeof(my_sock);
	bzero(&my_sock,length);

	my_sock.sin_family=AF_INET;
	my_sock.sin_addr.s_addr=INADDR_ANY;
	my_sock.sin_port=htons(this->port);

	int yes = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	if (bind(sock,(struct sockaddr *)&my_sock,length)<0) 
		error("binding");
	fromlen = sizeof(struct sockaddr_in);
    // getownip();
    // ip = inet_ntoa (my_sock.sin_addr);
	cout << "### Server initiated at " << ip << ":" << port << " (" << id << ") " << endl;
}

void node::send_msg(string ip, int port, string type, string text) {
	other_sock.sin_family = AF_INET;
	// other_sock.sin_addr.s_addr = inet_addr(ip.c_str());
	other_sock.sin_addr.s_addr = inet_addr("127.0.0.1");

	other_sock.sin_port = htons(port);
	length=sizeof(struct sockaddr_in);

	message msg(type, text);
	msg.get_buffer(buf);

	n=sendto(sock,buf,
		strlen(buf),0,(const struct sockaddr *)&other_sock,length);
	cout << ip << ":" << port <<  " <== " << buf << endl;
	if (n < 0) error("Sendto");
}

void node::send_msg(struct sockaddr_in other_sock, string type, string text) {
	length=sizeof(struct sockaddr_in);

	message msg(type, text);
	msg.get_buffer(buf);

	n=sendto(sock,buf,
		strlen(buf),0,(const struct sockaddr *)&other_sock,length);
	cout <<  inet_ntoa(other_sock.sin_addr) << ":" << (int) ntohs(other_sock.sin_port) << " <== " << buf << endl;
	if (n < 0) error("Sendto");
}

void node::send_msg(location *loc, string type, string text) {
	send_msg(loc->my_sock, type, text);
}

list<string> node::listDir() {
	list<string> files;
	DIR *d;
	struct dirent *dir;
	string path = "./"+folder;
	d = opendir(path.c_str());
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_type == DT_REG) {
                // printf("%s\n", dir->d_name);
				files.push_back(dir->d_name);
			}
		}
		closedir(d);
	}
	return files;
}

string append_filenams(list<string> files) {
	string out;
	for(std::list<string>::iterator list_iter = files.begin(); 
		list_iter != files.end(); list_iter++)
	{
		out += *list_iter + ":";
	}
	return out;
}

// void node::insert_before (string addr_str) {
// 	struct sockaddr_in requester; 
// 	requester.sin_family=AF_INET;
// 	requester.sin_addr.s_addr=inet_addr(decode_ip(addr_str).c_str()); // needs to be changed
// 	requester.sin_port=htons(decode_port(addr_str));

// 	send_msg(predecessor()->my_sock, "NBS", addr_str);
// 	send_msg(requester, "NBS", getaddr());
// 	send_msg(requester, "NBP", predecessor()->getaddr());
// 	this-> predecessor_node = new node(requester);
// }

void node::insert_after (string addr_str) {
	struct sockaddr_in requester; 
	requester.sin_family=AF_INET;
	requester.sin_addr.s_addr=inet_addr(decode_ip(addr_str).c_str()); // needs to be changed
	requester.sin_port=htons(decode_port(addr_str));

	send_msg(successor(), "NBP", addr_str);
	send_msg(requester, "NBP", getaddr());
	send_msg(requester, "NBS", this-> successor()->getaddr());
	this-> successor_node = new location(requester);
}

void node::handle_neighbor(message msg) {
	int inport = atoi(msg.tokens[2].c_str());
	string inip = msg.tokens[1];
	string req_addr = encode_addr(inip, inport);
	llu req_id = hash(req_addr);
	
	// Request for neighbor
	if (this-> predecessor()==NULL && this->successor()==NULL) {
		// beginning
		this-> predecessor_node = new location(msg.from);
		this-> successor_node = new location(msg.from);
		send_msg(msg.from, "NBP", getaddr());
		send_msg(msg.from, "NBS", getaddr());
	}
	else {
		// simplified logic here
		if(id < this-> successor()->id) {
			// Normal case
			if(req_id > this->id && req_id<this->successor()->id) 
				insert_after(req_addr);
			else forward_msg(successor(),msg);
		}
		else {
			// Boundary case
			if(req_id > id || req_id < this->successor()->id) 
				insert_after(req_addr);
			else forward_msg(successor(),msg);
		}
	}
}

void node::process_msg(message msg) {
	if (strcmp(msg.type.c_str(), "NBR")==0) {
		handle_neighbor(msg);
	}
	else if (strcmp(msg.type.c_str(), "NBP")==0) { 
		// Change successor
		sockaddr_in new_sock;
		new_sock.sin_family=AF_INET;
		new_sock.sin_addr.s_addr=inet_addr(msg.tokens[1].c_str());
		new_sock.sin_port=htons(atoi(msg.tokens[2].c_str()));
		this->predecessor_node = new location(new_sock);
		stabalize();
	}
	else if (strcmp(msg.type.c_str(), "NBS")==0) { 
		// Change predecessor
		sockaddr_in new_sock;
		new_sock.sin_family=AF_INET;
		new_sock.sin_addr.s_addr=inet_addr(msg.tokens[1].c_str());
		new_sock.sin_port=htons(atoi(msg.tokens[2].c_str()));
		this->successor_node = new location(new_sock);	
	}
	else if (strcmp(msg.type.c_str(), "ADD_FILE")==0) { 
		// Add file to hash
		llu file_id = hash(msg.text);
		if(id>predecessor()->id) {
			// Normal case
			if(file_id>predecessor()->id && file_id <= id) 
				addfile(msg.tokens[1],msg.tokens[2]+":"+msg.tokens[3]);
			else {
				if (closest_successor(file_id)==this) 
					forward_msg(successor(), msg);
				else
					forward_msg(closest_successor(file_id), msg);
			}
		}
		else {
			// Boundary condition
			if(file_id>predecessor()->id || file_id <= id) 
				addfile(msg.tokens[1],msg.tokens[2]+":"+msg.tokens[3]);
			else {
				if (closest_successor(file_id)==this) 
					forward_msg(successor(), msg);
				else
					forward_msg(closest_successor(file_id), msg);
			}
		}
	}
	else if (strcmp(msg.type.c_str(), "SEARCH")==0) {
		// Search for a file
		llu file_id = hash(msg.text);
		cout << file_id << endl;
		string addr;
		if(id>predecessor()->id) {
			// Normal case
			if(file_id>predecessor()->id && file_id <= id) {
				addr = get_addr(msg.text);
				send_msg(msg.tokens[2],atoi(msg.tokens[3].c_str()), "FOUND",msg.tokens[1] + ":"+ addr);
			}
			else {
				if (closest_successor(file_id)==this) 
					forward_msg(successor(), msg);
				else
					forward_msg(closest_successor(file_id), msg);
			}
		}
		else {
			// Boundary condition
			if(file_id>predecessor()->id || file_id <= id) {
				addr = get_addr(msg.text);
				send_msg(msg.tokens[2],atoi(msg.tokens[3].c_str()), "FOUND",msg.tokens[1] + ":" + addr);
			}
			else {
				if (closest_successor(file_id)==this) 
					forward_msg(successor(), msg);
				else
					forward_msg(closest_successor(file_id), msg);
			}
		}
	}
	else if (strcmp(msg.type.c_str(), "DWLD")==0) {
		// Download a file
		llu file_id = hash(msg.text);
		cout << file_id << endl;
		string addr;
		if(id>predecessor()->id) {
			// Normal case
			if(file_id>predecessor()->id && file_id < id) {
				addr = get_addr(msg.text);
				send_msg(msg.tokens[2],atoi(msg.tokens[3].c_str()), "DWLD_FROM",msg.tokens[1] + ":"+ addr +":"+ msg.tokens[4]);
			}
			else {
				if (closest_successor(file_id)==this) 
					forward_msg(successor(), msg);
				else
					forward_msg(closest_successor(file_id), msg);
			}
		}
		else {
			// Boundary condition
			if(file_id>predecessor()->id || file_id < id) {
				addr = get_addr(msg.text);
				send_msg(msg.tokens[2],atoi(msg.tokens[3].c_str()), "DWLD_FROM",msg.tokens[1] + ":" + addr +":"+ msg.tokens[4]);
			}
			else {
				if (closest_successor(file_id)==this) 
					forward_msg(successor(), msg);
				else
					forward_msg(closest_successor(file_id), msg);
			}
		}
	}
	else if (strcmp(msg.type.c_str(), "FOUND")==0) {
		// Found a file location
		string filename = msg.tokens[1];
		string fip = msg.tokens[2];
		string fport = msg.tokens[3];
		cout << "File " << filename << " found at " << fip << ":" << fport << endl;
	}
	else if (strcmp(msg.type.c_str(), "DWLD_FROM")==0) {
		// Found location for file to download from
		string filename = msg.tokens[1];
		string fip = msg.tokens[2];
		string fport = msg.tokens[3];
		cout << "File " << filename << " found at " << fip << ":" << fport << endl;
		string saveas = msg.tokens[4];
		cout << "Saving as: " << saveas << endl;
		download_file(filename, saveas, fip, atoi(fport.c_str()));
	}
	else if (strcmp(msg.type.c_str(), "PRINT")==0) {
		// print information about self
		print();
	}
	else if (strcmp(msg.type.c_str(), "SHARE")==0) {
		// Share files, add them to respective hash
		share_files();
	}
	else if (strcmp(msg.type.c_str(), "LEAVE")==0) {
		// Leave
		leave();
	}
	else if (strcmp(msg.type.c_str(), "UPD_FINGER")==0) {
		// construct finger table
		int inport = atoi(msg.tokens[2].c_str());
		string inip = msg.tokens[1];
		string req_addr = encode_addr(inip, inport);
		llu req_id = hash(req_addr);
		location* req_loc = new location(inip, inport);
		if(req_loc->id == id) {
			cout << "finger table updation complete" << endl;
		}
		else {
			update_ft(req_loc);
			forward_msg(successor(),msg);
		}
	}
	else if (strcmp(msg.type.c_str(), "ADD_FINGER")==0) {
		// add to finger table
		int i = atoi(msg.tokens[1].c_str());
		int inport = atoi(msg.tokens[3].c_str());
		string inip = msg.tokens[2];
		string req_addr = encode_addr(inip, inport);
		llu req_id = hash(req_addr);
		location* req_loc = new location(inip, inport);
		if(req_id < finger[i]->id) {
			finger[i] = req_loc;
			// cout << "here" << endl;
			printf("Finger[%d] = ",i+1);
			cout << finger[i]->getaddr() << " ";
			cout << "(" << finger[i]->id << ")" << endl;
		}
	}
	else if (strcmp(msg.type.c_str(), "CLI_UPD_FINGER")==0) {
		send_msg(successor(),"UPD_FINGER",getaddr());
	}
	else if (strcmp(msg.type.c_str(), "REPLACE_FT")==0) {
		int inport = atoi(msg.tokens[2].c_str());
		string inip = msg.tokens[1];
		string req_addr = encode_addr(inip, inport);
		llu req_id = hash(req_addr);
		int inport_new = atoi(msg.tokens[4].c_str());
		string inip_new = msg.tokens[3];
		string req_addr_new = encode_addr(inip_new, inport_new);
		llu req_id_new = hash(req_addr_new);
		if (req_id==id ) {
			std::map<string,string>::iterator it;
			for (it=filemap.begin(); it!=filemap.end(); ++it)  {
				if (hash(it->second)==req_id) {
					cout << "Removing " << it->first << " from index" << endl;
					filemap.erase(it);
					it = filemap.begin();
				}
			}
			cout << "ALl done. Bye" << endl;
			exit(0);
		}
		else {
			for(int i=0; i<MBIT; i++) {
				if (finger[i]->id == req_id) {
					finger[i] = new location(inip_new,inport_new);
				}
			}
			std::map<string,string>::iterator it;
			for (it=filemap.begin(); it!=filemap.end(); ++it)  {
				if (hash(it->second)==req_id) {
					cout << "Removing " << it->first << " from index" << endl;
					filemap.erase(it);
					it = filemap.begin();
				}
			}
			forward_msg(successor(),msg);
		}
	}
	else {
		// Anything else
	}
}

void node::leave() {
	std::map<string,string>::iterator it;
	// send files to successor
	for (it=filemap.begin(); it!=filemap.end(); ++it) {
		send_msg(successor() ,"ADD_FILE", it->first + ":" + it->second);
	}
	// update finger table of others
	send_msg(successor() ,"REPLACE_FT", getaddr() + ":" + successor()->getaddr());
}

location* node::closest_successor(llu k) {
	location * loc = this;
	for(int i=0; i<MBIT; i++) {
		if (finger[i]->id > loc->id) {
			if (k> loc->id && k <= finger[i]->id) break;
		}
		else {
			if (k <= finger[i]->id || k> loc->id) break;
		}
		loc = finger[i];
	}
	return loc;
}


void node::req_share_files() {
	send_msg(my_sock, "SHARE", tostr(port));
}

void node::share_files() {
	list <string> files = listDir();
	for(std::list<string>::iterator list_iter = files.begin(); 
		list_iter != files.end(); list_iter++)
	{
		// Send the filenames to respective nodes
		llu file_id = hash(*list_iter);
		// cout << file_id << endl;
		string temp = *list_iter ;
		temp += ":" + getaddr();
		send_msg(ip,port ,"ADD_FILE", temp); // sending to self
	}
}

void node::stabalize() {
	std::map<string,string>::iterator it;
	for (it=filemap.begin(); it!=filemap.end(); ++it) {
		if (hash(it->first) <= predecessor()->id) {
			send_msg(predecessor() ,"ADD_FILE", it->first + ":" + it->second);
			filemap.erase (it->first);
			it = filemap.begin();
			if(filemap.empty()) break;
		}
	}
}

void node::printIndex() {
	std::cout << "File Mapping:";
	if (filemap.size()==0) {
		cout << " Empty" << endl;
		return;
	}
	std::map<string,string>::iterator it;
	for (it=filemap.begin(); it!=filemap.end(); ++it)  {
		std::cout << "  " << it->first << " => " << it->second;
		cout << " (" << hash(it->first) << ")" << endl;
	}
}

void node::print() {
	cout << endl;
	cout << "=========================" << endl;
	cout << "P: " << predecessor()->getaddr() << endl;
	cout << "ID: " << predecessor()->id << endl;
	cout << "\t^" << endl;
	cout << "\t|" << endl;
	cout << "Self: " << getaddr() << endl;
	cout << "ID: " << id << endl;
	cout << "\t|" << endl;
	cout << "\tv" << endl;
	cout << "S: " << successor()->getaddr() << endl;
	cout << "ID: " << successor()->id << endl;
	cout << "=========================" << endl;
	printIndex();
	cout << "=========================" << endl;
	print_ft();
	cout << "=========================" << endl;
}

void node::print_ft () {
	cout << "Finger table" << endl;
	for(int i=0; i<MBIT; i++) {
		cout << i+1 << " " << finger[i]->getaddr() << " ";
		cout << "(" << finger[i]->id << ")" << endl;
	}
}

void node::printreq () {
	send_msg(my_sock, "PRINT", tostr(port));
}

void node::forward_msg(location* to, message m) {
	string body = m.msg.substr(m.tokens[0].size()+1);
	cout << body << endl;
	send_msg(to->my_sock,m.type, body);
}

void node::showFileMap() {
	std::cout << "File Mapping:\n";
	std::map<string,string>::iterator it;
	for (it=filemap.begin(); it!=filemap.end(); ++it)
		std::cout << it->second << " => " << it->first << '\n';
}

int node::addfile (string filename, string addr) {
	// cout << "adding to index" << endl;
	filemap.insert ( std::pair<string,string>(filename,addr) );
}

string node::get_addr(string filename) {
	string ip;
	cout << "Received request for " << filename << endl;
	if ( filemap.find(filename) == filemap.end() ) {
		cout << "File not found." << endl;
		return "-";
	} else {
		return filemap[filename];
	}
}

int node::download(string filename, string saveas) {
	send_msg(my_sock, "DWLD", filename + ":" + getaddr() + ":" + saveas);
	// download_queue.insert ( std::pair<string,string>(filename,saveas) );
}

void node::search(string filename) {
	send_msg(my_sock, "SEARCH", filename + ":" + getaddr());
}

bool node::download_file (string filename, string saveas, string ip, int port) {
	int filesocket;
	socklen_t addr_size;
	sockaddr_in serverAddr;

	saveas = folder + "/" + saveas;
	filesocket = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
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
void node::run_file_server() {
	// file server local variables
	fd_set readset, tempset;
	int maxfd, flags;
	int srvsock, peersock, j, result, result1, sent, len;
	timeval tv;
	char buffer[BUFFER_SIZE];
	sockaddr_in addr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size = sizeof(serverStorage);

	// file server start
	srvsock = socket(PF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	memset(addr.sin_zero, '\0', sizeof addr.sin_zero);
	int iSetOption = 1;
	setsockopt(srvsock, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,
		sizeof(iSetOption));
	if(bind(srvsock, (struct sockaddr *) &addr, sizeof(addr))) {
		cout << "Error: Could not bind\n";
		exit(1);
	}
	if(listen(srvsock,5)==0) printf("### File server listening\n");
	else printf("Error\n");
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
					fname = folder + "/" + fname;

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
							// cout << result1 << endl;
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

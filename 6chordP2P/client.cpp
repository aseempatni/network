#include "node.h"
#include "utils.h"
#include <stdlib.h>

#define SHARE 1
#define SEARCH 2
#define DOWNLOAD 3
#define STATS 4

// Declarators

node* search(llu file_id);
llu map_file_to_node (llu file_id);
bool download (string filename, node* server);

int main (int argc, char* argv[]) {

	if (argc!=2) {
		cout << "Usage: ./client <port>" << endl;
		exit(1);
	}
	int port = atoi(argv[1]);
	node self(port);
    string type;

    if (fork()) {
    	self.run_file_server();
    	exit(0);
    }
    else {
		if(fork()) {
			while(1) {
				self.recv_msg();
				// self.print();
			}
		}
		while (1) {
			cout << "What would you like to do?" << endl;
			// cout << "0  Nothing ()" << endl;
			cout << "1  Share my files." << endl;
			cout << "2  Search a file." << endl;
			cout << "3  Download a file." << endl;
			cout << "4  Get self Stats" << endl;
			int input;
			cin >> input;
			string filename, saveas;
			switch(input) {
				case SHARE:
					self.req_share_files();
					break;
				case SEARCH:
					cout << "Enter filename: ";
					cin >> filename;
					self.search(filename);
					break;
				case DOWNLOAD:
					cout << "Enter filename: ";
					cin >> filename;
					cout << "Save as: ";
					cin >> saveas;
					self.download(filename, saveas);
					break;
				case STATS:
					self.printreq();
					break;
				case 0:
				break;
			}
			// self.recv_msg();
	    }
	}
}

// Implementation


#include "node.h"
#include "utils.h"
#include <stdlib.h>

#define DOWNLOAD 3

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
		// self.recv_msg();
		// self.send_msg("127.0.0.1",12000,"NBR","who's my neighbor?");
		if(fork()) {
			while(1) {
				self.recv_msg();
			}
		}
		while (1) {
			cout << "What would you like to do?" << endl;
			// cout << "0  Nothing ()" << endl;
			cout << "1  Update my neighbors." << endl;
			cout << "2  Share my files." << endl;
			cout << "3  Download a file." << endl;
			int input;
			cin >> input;
			string filename, saveas;
			switch(input) {
				case DOWNLOAD:
					cout << "Enter filename: ";
					cin >> filename;
					cout << "Save as: ";
					cin >> saveas;
					self.download(filename, saveas);
					break;
				case 1:
					self.send_msg("127.0.0.1",12000,"NBR","who's my neighbor?");
					break;
				case 0:
				break;
			}
			// self.recv_msg();
	    }
	}
}

// Implementation


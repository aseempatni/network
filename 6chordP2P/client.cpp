#include "node.h"
#include "utils.h"
#include <stdlib.h>


int main (int argc, char* argv[]) {

	if (argc!=2) {
		cout << "Usage: ./client <port>" << endl;
		exit(1);
	}
	int port = atoi(argv[1]);

	node self(port);
	self.run();
}
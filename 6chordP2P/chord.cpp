#include <iostream>
#include "utils.h"
using namespace std;

class node {
	string ip;
	int port;
	public:
		node();
		node* successor();
		node* predecessor();
	private:
		node* successor_node;
		node* predecessor_node;
		void query_neighbors();

};

node* node::successor() {
	return this-> successor_node;
}
node* node::predecessor() {
	return this-> predecessor_node;
}
node::node() {
	cout << "New node created." << endl;
	this->query_neighbors();
}

void node::query_neighbors() {
	cout << "Querying neighbors" << endl;
	// Ask node 0 for the neighbors

}

node* search(llu file_id);
llu map_file_to_node (llu file_id);
bool download (string filename, node* server);




int main () {
	node self;
}




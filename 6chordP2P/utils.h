#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

#include <string>
#include <vector>

#include <climits>
#include <math.h>

#define MAX_ID 8
// #define MAX_ID ULLONG_MAX
#define MBIT 3 // log2 (MAX_ID) 

using namespace std;

typedef unsigned long long int llu;

// llu hash(string p, int len);
llu hash(string p);

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

void Tokenize(const string& str,
	vector<string>& tokens,
	const string& delimiters = ":");

string encode_addr(string ip, int port);
// string encode_addr(struct sockaddr_in addr);
int decode_port(string encoded_addr);

string decode_ip(string encoded_addr);

void error(const char *msg);
void getownip();
string tostr(int x);
#endif
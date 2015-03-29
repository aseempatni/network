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

using namespace std;

typedef unsigned long long int llu;

llu hash(string p, int len);
llu hash(string p);

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

void Tokenize(const string& str,
	vector<string>& tokens,
	const string& delimiters = ":");

string encode_addr(string ip, int port);
int decode_port(string encoded_addr);

string decode_ip(string encoded_addr);

void error(const char *msg);
void getownip();
string tostr(int x);
#endif
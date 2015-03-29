#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
using namespace std;

typedef unsigned long long int llu;

llu hash(string p, int len);

void error(const char *msg);
void getownip();
string tostr(int x);
#endif
//
// Created by Ziqian Zhang on 2019-03-08.
//
#include "server.h"
#include "client.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    cout << argv[0];
    cout << argv[1];
    if (strcmp(argv[1], "server") == 0)
        enterServer();
    else
        enterClient(argv[1]);
}
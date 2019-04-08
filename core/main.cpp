/*---------------------------------------------------------------------------------------
--	SOURCE FILE:    main.cpp - Main program/Application starting point
--
--	PROGRAM:        Linux Chat
--
--	FUNCTIONS:      int main (int argc, char **argv)
--
--	DATE:			March 8, 2019
--
--	REVISIONS:	    (Date and Description)
--
--	DESIGNER:		Ziqian Zhang, Viktor Alvar
--
--	PROGRAMMER:		Ziqian Zhang, Viktor Alvar
--
--	NOTES:
--  The starting points for the Linux Chat program. The program has two modes, server
--  and client. You can run the server application by adding the following command
--  line argument: "./linuxchat server". If you would like to run the client application
--  add the following command line argument: "./linux 127.0.1.1" or your desired host
--  address.
---------------------------------------------------------------------------------------*/

#include "server.h"
#include "client.h"
#include <string.h>
#include <iostream>

using namespace std;

/*---------------------------------------------------------------------------------------
--	FUNCTION:		main
--
--	DATE:			March 8, 2019
--
--	REVISIONS:	    (Date and Description)
--
--	DESIGNER:		Ziqian Zhang, Viktor Alvar
--
--	PROGRAMMER:		Ziqian Zhang, Viktor Alvar
--
--	INTERFACE:      int main(int argc, char **argv)
--                      int argc: Number of command line arguments
--                      char **argv: Command line arguments
--
--	RETURNS:		int.
--
--	NOTES:
--  Starting point of the Linux Chat application. This function is called first when
--  executing the application. Runs the server or client application depending on the
--  command line argument that was passed.
---------------------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
    cout << argv[0];
    cout << argv[1];

    if (strcmp(argv[1], "server") == 0)
        enterServer();
    else
        enterClient(argv[1]);
}
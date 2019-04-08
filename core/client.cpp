/*---------------------------------------------------------------------------------------
--	SOURCE FILE:	client.cpp - The client mode for the Linux Chat program
--
--	PROGRAM:		Linux Chat
--
--	FUNCTIONS:		int enterClient(char *hostName)
--                  void *myThreadFun(void *vargp)
--
--	DATE:			March 8, 2019
--
--	REVISIONS:		(Date and Description)
--
--	DESIGNERS:		Ziqian Zhang, Viktor Alvar
--
--	PROGRAMMERS:	Ziqian Zhang
--
--	NOTES:
--	The program will establish a TCP connection to a user specifed server. The server
--  can be specified using a fully qualified domain name or and IP address. After the
--  connection has been established the user will be able to send strings to the server
--  and also receive strings from the server and display them onto the UI.
---------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_TCP_PORT 7000    // Default port
#define BUFLEN          200     // Buffer length

// Global Variables
int n, bytes_to_read;
int sd, port;
struct hostent *hp;
struct sockaddr_in server;
char *host, *bp, rbuf[BUFLEN], sbuf[BUFLEN], **pptr;
char str[16];

// Function Prototypes
void *myThreadFun(void *vargp);

/*---------------------------------------------------------------------------------------
--	FUNCTION:		enterClient
--
--	DATE:			March 8, 2019
--
--	REVISIONS:	    (Date and Description)
--
--	DESIGNER:		Ziqian Zhang, Viktor Alvar
--
--	PROGRAMMER:		Ziqian Zhang
--
--	INTERFACE:      int enterClient(char *hostName)
--                      char *hostName: Server Host Name
--
--	RETURNS:		int.
--
--	NOTES:
--  Enters the client mode in the application. The client mode is triggered if the user
--  enters a host name as a command line argument. This function establishes makes a
--  connection to the server and sends user input as a string. The receving thread is
--  also created in this funciton.
---------------------------------------------------------------------------------------*/
int enterClient(char *hostName) {
    pthread_t pthread;
    port = SERVER_TCP_PORT;
    host = hostName;

    // Create the socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Cannot create socket");
        exit(1);
    }

    bzero((char *) &server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if ((hp = gethostbyname(host)) == NULL) {
        fprintf(stderr, "Unknown server address\n");
        exit(1);
    }

    bcopy(hp->h_addr, (char *) &server.sin_addr, hp->h_length);

    // Connecting to the server
    if (connect(sd, (struct sockaddr *) &server, sizeof(server)) == -1) {
        fprintf(stderr, "Can't connect to server\n");
        perror("connect");
        exit(1);
    }

    pthread_create(&pthread, NULL, myThreadFun, NULL);
    printf("Connected:    Server Name: %s\n", hp->h_name);
    fflush(stdout);
    pptr = hp->h_addr_list;
    printf("\t\tIP Address: %s\n", inet_ntop(hp->h_addrtype, *pptr, str, sizeof(str)));
    fflush(stdout);

    while (1) {
        printf("Transmit:\n");
        fflush(stdout);
        //gets(sbuf); // get user's text
        fgets(sbuf, BUFLEN, stdin);

        // Transmit data through the socket
        send(sd, sbuf, BUFLEN, 0);

        if (sbuf[0] == '~' && sbuf[1] == '!')
            break;
    }

    close(sd);
    return (0);
}

/*---------------------------------------------------------------------------------------
--	FUNCTION:		myThreadFun
--
--	DATE:			March 8, 2019
--
--	REVISIONS:	    (Date and Description)
--
--	DESIGNER:		Ziqian Zhang, Viktor Alvar
--
--	PROGRAMMER:		Ziqian Zhang
--
--	INTERFACE:      void *myThreadFun(void *vargp)
--                      void *vargp: Thread function parameters
--
--	RETURNS:		void.
--
--	NOTES:
--  Continously reads the connected socket to receive string messages from the server.
--  When a message is receivied it is printed and displayed on the UI. This function is
--  called when the read thread is created in the enterClient function.
---------------------------------------------------------------------------------------*/
void *myThreadFun(void *vargp) {
    while (1) {
        int n = 0;
        bp = rbuf;
        bytes_to_read = BUFLEN;

        // client makes repeated calls to recv until no more data is expected to arrive.
        while ((n = recv(sd, bp, bytes_to_read, 0)) < BUFLEN) {
            bp += n;

            bytes_to_read -= n;
        }

        printf("%s\n", rbuf);
        fflush(stdout);
    }

    close(sd);
    return NULL;
}
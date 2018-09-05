/*********************************************************************
** Author: James Hippler (HipplerJ)
** Oregon State University
** CS 372-400 (Winter 2018)
** Introduction to Computer Networks
**
** Description: Programming Assignment #1 (Project 1)
** Due: Sunday, February 11, 2018
**
** Filename: chatclient.c
**
** Objectives:
** 1.) Implement a client-server network application
** 2.) Learn to use sockets API
** 3.) Use the TCP protocol
** 4.) Refresh programming skills
**
** EXTERNAL RESOURCES
** Learned how to program some of the C/C++ Client side code from the below website
** https://beej.us/guide/bgnet/html/multi/index.html
** http://www.theinsanetechie.in/2014/01/a-simple-chat-program-in-c-tcp.html
** https://codereview.stackexchange.com/questions/154969/c-multithreaded-chat-client-and-server
*********************************************************************/

// Include the necessary libraries to operate the program and to include the Socket API
// The include list was gathered directly from the Beej Guide
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXHANDLE 11                                                            // Define maximum characters for User Handle
#define CHARBUFFER 501                                                          // Define constant global variable for the maximum number of character in buffer string


// Create function prototypes for main to call later in the program
void confirmExecution(int);
void getHandle(char *);
struct addrinfo * getAddrResults(char *, char *);
int createSock(struct addrinfo *);
void sockIgnition(int, struct addrinfo *, char *);
void initiateChat(int, char *, char *);

/************************************************************************
* Description: main function
* Creates the character arrays to store the client and server
* handle information.  Calls the necessary function to orchestrate
* the program and connect with the TCP chat server.
*************************************************************************/

int main(int argc, char *argv[]){

  char clientHandle[MAXHANDLE],                                                 // Create a character array to store the client side handle
       serverHandle[MAXHANDLE];                                                 // Create a character array to store the server side handle.

  confirmExecution(argc);                                                       // Call the function to confirm the appropriate script arguments were provided.
	getHandle(clientHandle);                                                      // Call the function to get the user's handle.  Pass the handle array.
	struct addrinfo * results = getAddrResults(argv[1], argv[2]);                 // This structure is used to prep the socket address structures for subsequent use
	int sockfd = createSock(results);                                             // Create the socket and assigned return to in variable.
	sockIgnition(sockfd, results, clientHandle);                                  // Create the socket and prepare for TCPconnnection
	initiateChat(sockfd, clientHandle, serverHandle);                             // Kick-off the chat functionality between the server and the client
	freeaddrinfo(results);                                                        // Free the linked-list (Grabbed directly from Beej)

  return 0;                                                                     // Return 0 and end the main function (and program)
}

/************************************************************************
* Description: confirmExecution
* Function verifies that the user input the correct amount of
* script arguments when executing the chat server program.
* It takes the argc array and confirms that there are three elements.
*************************************************************************/

void confirmExecution(int argc){
  if(argc != 3){                                                                // If the array does not equal 3 then print and error messages to the screen
    fprintf(stderr, "Invalid script execution!\n");
    printf("Expected Input:\n");
    printf("./<file_name> <host_name> <port_number>\n");
    printf("\nExample:\n");                                                     // Provide user with a proper usage example
    printf("./chatclient flip3.engr.oregonstate.edu 3737\n");
    printf("\nExiting the Program\n");                                          // Prompt user that the connection is being closed.
    printf("Please re-run with the correct parameters\n\n");
    exit(1);                                                                    // Exit the program and return the user to the command line
  } else {                                                                      // Else print a message saying the correct number of commands were issued.
    printf("Correct amount of arguments provided on command line!\n\n");
  }
}

/************************************************************************
* Description: getHandle
* Function prompts the user to input a username (handle) for their side
* of the connection.  The function receives a pointer to the handle
* character array from main and assigns the user submission.
* The function contains exception handling to obligate the user from
* inputting more than 10 characters.
*************************************************************************/

 void getHandle(char *clientHandle){
   do{
     printf("Please enter a user handle (10 Characters or less)...\n");
     printf("Client Handle: ");                                                 // Prompt the user to input their chat handle
     scanf("%s", clientHandle);                                                 // Assign user's input to the handle array
     if(strlen(clientHandle) > (MAXHANDLE - 1) || strlen(clientHandle) < 1){    // If the length of array is more than 10
       printf("\nUsername must be less than 10 characters)");                   // Print error messages and tell the user to re-enter
       printf("\nPlease re-enter username\n\n");
     } else {                                                                   // Else welcome the user to the chatroom
       printf("\n%s, Welcome to the chatroom\n", clientHandle);
       printf("Establishing your connection with the server\n\n");
     }
   } while (strlen(clientHandle) > (MAXHANDLE-1) || strlen(clientHandle) < 1);  // Continue prompting the user until they input the correct amount of characters
 }

/************************************************************************
* Description: struct addrinfo *getAddrResults
* Functions creates a linked list with the address and the port number.
* Function receives the address and the port number and return a linked
* list.
*************************************************************************/

struct addrinfo *getAddrResults(char *input_addr, char *portnum){
  // printf("IN THE getAddrResults FUNCTION\n");                                // Used for testing.  DELETE once complete.
 	struct  addrinfo *results,
          hints;
  int     rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;                                                    // AF_INET or AF_INET6 to force version
  hints.ai_socktype = SOCK_STREAM;

  rv = getaddrinfo(input_addr, portnum, &hints, &results);
 	if(rv != 0){
 		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rv));               // If an error occurred.  Print the received error message by calling the gai_strerror function
 		exit(1);                                                                    // Exit the program and return the user to the command prompt
 	}
 	return results;                                                               // Return the results to the main function
}

/************************************************************************
* Description: createSocket
* Program calls the socket function and assigns the return value
* to an integer variable.  This is function is used to create the
* initial socket using the Socket API.  Function has error handling
* to confirm that the appropriate return message is received from the
* socket function.  Once complete, the sockfd variable with the socket
* return information is handed back to the main function.
*************************************************************************/

int createSock(struct addrinfo *results){
  // printf("IN THE createSock FUNCTION\n");                                    // Used for testing.  DELETE once complete.
  int sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	if (sockfd == -1){                                                            // If return value from the socket function is -1
    fprintf(stderr, "ERROR: Failed CREATING Socket!\n");                        // Notify the user that the porgram was unable to create the socket
		exit(1);                                                                    // Exit the program
  } else {
    printf("SUCCESS: Socket has been Created!\n");
  }
	return sockfd;                                                                // Return the sockfd information to main function for later use.
}

/************************************************************************
* Description: sockIgnition
* Function receives the socket information and the linked list
* in order to create a connection on the socket.  There's error handling
* incase the connection functionreturns an status code of -1.  In the
* event that this happens, the function will produce and error and
* terminate the program's execution.
*************************************************************************/

void sockIgnition(int sockfd, struct addrinfo *res, char *clientHandle){
  // printf("IN THE sockIgnition FUNCTION\n");                                  // Used for testing.  DELETE once complete.
	int statusCode = connect(sockfd, res->ai_addr, res->ai_addrlen);
	if (statusCode == -1){                                                        // If the connection function returns a value of -1
		fprintf(stderr, "ERROR: Failed CONNECTING Socket!\n");                      // Display an error message to the user saying the connection failed
		exit(1);                                                                    // Terminate the program
	} else {
    printf("SUCCESS: Socket Connection Established!\n\n");
    printf("Client Handle: %s\n", clientHandle);                                // Print the username for the client side of the chat client
    printf("You may now begin sending messages to the server\n");               // Let the user know that they can now start sending messages
  }
}

/************************************************************************
* Description: initiateChat
* function takes the socket connection information and the
* client / server account handles.  The function is one handles
* the sending and receiving of messages between the client and the
* server.  Function will return to main when either the client
* or the server send the \quit function
*************************************************************************/

void initiateChat(int sockfd, char * clientHandle, char * serverHandle){
  // printf("IN THE chat FUNCTION\n");                                          // Used for testing.  DELETE when complete
  char  sent[CHARBUFFER],                                                       // Create an array to store the sending messages to the server
        received[CHARBUFFER];                                                   // Create an array to store the received messages from the server
  int   verify;
  send(sockfd, clientHandle, strlen(clientHandle), 0);                          // Send chat client connection information to establish chat
  recv(sockfd, serverHandle, 10, 0);                                            // Receive chat server connection information and establish chat
  fgets(sent, CHARBUFFER, stdin);                                               // get the message created by the client chat

  do {                                                                          // Begin a loop that continues the chat application until the user sends a quit message
    memset(sent,0,sizeof(sent));                                                // Clear the array so that the new received message can be stored
    memset(received,0,sizeof(received));                                        // Clear the array so that the new send message can be stored
    printf("%s -> ", clientHandle);                                             // print the clients handle user name followed by an arrow
    fgets(sent, CHARBUFFER, stdin);                                             // get the message created by the client chat

    if (strcmp(sent, "\\quit\n") == 0){                                         // If the message is quit
      printf("You have ended the TCP Connection\n\n");                          // Notify the user that they have successfully ended the connection
      send(sockfd, sent, strlen(sent) ,0);                                      // Send  message to the server. Even if \quit so it's aware that you've ended the connection
      break;                                                                    // End the while loop and terminate the program
    }
    send(sockfd, sent, strlen(sent) ,0);                                        // Send  message to the server. Even if \quit so it's aware that you've ended the connection

    verify = recv(sockfd, received, CHARBUFFER, 0);                             // Receive incoming message from the server. and store in a character array
    if (strcmp(received, "\\quit") == 0){                                       // If the server sends a message to quit (\quit)
      printf("%s -> %s\n", serverHandle, received);                             // print the \quit message to the console
      printf("TCP Connection Closed by the Server\n");                          // Let the user know that the conneciton was closed
      break;                                                                    // Break out of the loop and return to main to terminate the program
    } else if (verify == 0){                                                    // Server issued the Ctrl+C command to end it's operation
  		printf("TCP Connection Closed by the Server\n");                          // Let the user know that the conneciton was closed
  		break;                                                                    // Break out of the loop and return to main to terminate the program
    } else {
      printf("%s -> %s\n", serverHandle, received);                             // Print the server handle and their message
    }

  } while(strcmp(sent, "\\quit\n") != 0);                                       // Loop until the chat client sends the QUIT message

  printf("TCP Socket Connection Is Now Closed!\n");                             // Print final message that the connection has beemn closed
  close(sockfd);                                                                // Close the TCP Socket connection and return to main
}

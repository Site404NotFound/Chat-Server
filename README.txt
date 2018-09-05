Author: James Hippler (HipplerJ)
Oregon State University
CS 372-400 (Winter 2018)
Introduction to Computer Networks

Description: Programming Assignment #1 (Project 1)
Due: Sunday, February 11, 2018

Filenames:
- chatserve.py
- chatclient.c
- README.txt

Objectives:
1.) Implement a client-server network application
2.) Learn to use sockets API
3.) Use the TCP protocol
4.) Refresh programming skills

EXTRAS (MAYBE?)
- Added functionality to input server name manually.
- Did some work to get the python script to output the hostname, port number, and IP Address Info
  of the server where it was executed.
- Added functionality to end the chat on the client side if the server user inputs a
  Keyboard interrupt (Ctrl + C)
- Exception handling incase user inputs a Keyboard Interrupt from the Server Side
  application.

EXTERNAL RESOURCES
Learned how to program some of the Python Server side code from the below websites
https://docs.python.org/release/2.6.5/library/socketserver.html
https://www.geeksforgeeks.org/simple-chat-room-using-python/
https://wiki.python.org/moin/TcpCommunication
http://www.bogotobogo.com/python/python_network_programming_tcp_server_client_chat_server_chat_client_select.php
http://www.binarytides.com/code-chat-application-server-client-sockets-python/
https://beej.us/guide/bgnet/html/multi/index.html
http://www.theinsanetechie.in/2014/01/a-simple-chat-program-in-c-tcp.html
https://codereview.stackexchange.com/questions/154969/c-multithreaded-chat-client-and-server

TESTING
Tested the chat server application on flip1 (flip1.engr.oregonstate.edu) with the chat client application running
initially on flip1 as well.  Chat application also works on flip3 (flip3.engr.oregonstate.edu) and can communicate
to with the chat server on flip1.

INSTRUCTIONS
1.) Compile the chatclient.c file to create an executable.  You can use the following command which
    will name the output chatclient

      gcc chatclient.c -o chatclient

2.) Start the chat server application with the command below.  Specify a port number as an argument (I used 3737 with
    regular success).

      python chatserve.py <port_number>

    Example:
      python chatserve.py 3737

    Alternatively, you could make the python script executable with the below command

      chmod +x chatserve.py

    Then the program could be executed as follows.

      ./chatserve.py <port_number>

    Example:
      ./chatserve.py 3737

3.) Give the chat server application a user name (server handle).  Once this is complete the application will
    sit idle until it receives a message from the client.

4.) Start the chat client application with the command below.  Make sure to specify the hostname and the port
    number used when initializing the chat server (It will be output by the chatserve.py program just before it
    begins waiting)

    ./chatclient <host_name> <port_number>

    Example:
    ./chatclient flip1.engr.oregonstate.edu 3737

3.) Give the chat client application a user name (client handle).  Once this is complete the application will
    prompt you to begin sending messages with the server.

6.) The chat application forces the users to take turns sending and receiving their messages.

7.) When satisfied, type '\quit' from either server or client terminal to end the chat.  The server will return
    to waiting for another client message.

8.) To end the server application, use the keyboard interrupt command (Crtl + C)

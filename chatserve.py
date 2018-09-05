#!/usr/bin/python

"""
/*********************************************************************
** Author: James Hippler (HipplerJ)
** Oregon State University
** CS 372-400 (Winter 2018)
** Introduction to Computer Networks
**
** Description: Programming Assignment #1 (Project 1)
** Due: Sunday, February 11, 2018
**
** Filename: chatserve.py
**
** Objectives:
** 1.) Implement a client-server network application
** 2.) Learn to use sockets API
** 3.) Use the TCP protocol
** 4.) Refresh programming skills
**
** EXTERNAL RESOURCES
** Learned how to program some of the Python Server side code from the below websites
** https://docs.python.org/release/2.6.5/library/socketserver.html
** https://www.geeksforgeeks.org/simple-chat-room-using-python/
** https://wiki.python.org/moin/TcpCommunication
** http://www.bogotobogo.com/python/python_network_programming_tcp_server_client_chat_server_chat_client_select.php
** http://www.binarytides.com/code-chat-application-server-client-sockets-python/
*********************************************************************/
"""

import socket  		   												 			# Import the python socket module
import select          	 														# Import the python select module
import sys              														# Import the python sys module
import os               														# Import the pythonj os module
from thread import *    														# Imported to implement threading

"""
/************************************************************************
* Description: initiateChat(var, var, var, var)
* Starts the actual conversation between the server and the client.
* Handles chat in turn order.  Connection buffer limited to 500
* characters.  Receives the connection information and
* the client/server handles from main.  No returned variables to main
*************************************************************************/
"""

def initiateChat(connect, clienthandle, handle, addr):
	while True:
		received = connect.recv(500).rstrip()
		print "{} -> {}".format(clienthandle,received) 							# print client's handle and their message to the console

		if received == "\quit":													# if the client sends \quit then the host will end the connection
			print("TCP Connection was closed by Client")						# print a message letting the user know the connection is closing
			print("Waiting for connection...")                                  # print statement to the screen that you are again waiting for a connection
			connect.close()                                                     # Close the socket connection with the client
			break																# End the infinite while loop and return to main

		sent = raw_input("{} -> ".format(handle))                        		# Take input on server command line and assign to variable

		if sent == '\quit':                                              		# If the server sends the \quit command
			print("Server Application has terminated communication")		    # Notify the user that the application is terminating
			print("Waiting for client connection...")                           # print statement to the screen that you are again waiting for a connection
			connect.send(sent) 													# Send server message to the client to be displayed
			connect.close()                                                     # Close the socket connection with the client
			break   	                                                       	# Exit Inner Loop and return to waiting for client input
		else:
			connect.send(sent)                                               	# Send server message to the client to be displayed

"""
/************************************************************************
* Description: getHandle()
* Prompts the user to enter a handle name for the server side
* application.  Some exceptions handling involved for user input.
* Returns handle input to the main function
*************************************************************************/
"""

def getHandle():
	handle = ""                                                                 # Create variable called handle and set to empty string
	while len(handle) < 1 or len(handle) > 10:                                  # Ensure that user inputs handle that is between 1 and 10 characters long
		print("Please enter a server handle (10 Characters or less)...")
		handle = raw_input("Server Handle: ")                            		# Prompt user to input handle name and assign to variable

		if len(handle) < 1 or len(handle) > 10:                                 # If character length requirement violated
			print("Server handle must be between 1 and 10 characters.")         # Display error message and restart while loop

	return handle                                                               # Return handle variable to the calling function (main)

"""
/************************************************************************
* Description: displayConDetails(var, var)
* Receives the port number and server from the main function.
* Gathers the hostname and the IP address of the server where it
* was executed.  Displays connection information to the user
*************************************************************************/
"""

def displayConDetails(portnumber, handle):

    hostname = socket.gethostname()                                             # Assigns server hostname to variable
    netaddr = socket.gethostbyname(hostname)                                    # Assigns server IP Address to variable

    print("\nSocket successfully created")
    print("Connecting with the following parameters...")
    print("\nHostname: {}".format(hostname))                                   	# Print Server hostname
    print ("IP Address: {}".format(netaddr))                                    # Print Server IP Address
    print("Port Number: {}".format(portnumber))									# Print designated Port Number
    print("Server Handle: {}".format(handle))									# Print the username for the server side of the chat client
    print("\nWaiting for client connection...")									# Show waiting message to the command line

"""
/************************************************************************
* Description: establishConnection(var, var, var)
* Establishes the connection with the client.
* Receives several variables from main including portnumber, handle,
* and server.  No returned variables to main.
* Calls initiateChate function and passes it the connection information,
* the client & server handles
*************************************************************************/
"""

def establishConnection(portnumber, handle, server):
	netaddr = socket.gethostbyname(socket.gethostname())
	connect,addr = server.accept()                                              # call the server.accept function and assign returns to two different variables
	print("Connection Established ({}:{})".format(netaddr,portnumber)) 			# Display message when connection with the client application has been established
	print("Waiting for client to begin the chat...")
	clienthandle = connect.recv(500)										    # assign the first message from the client as their handle
	connect.send(handle)                                                        # Send the server's handle name to the client for use

	initiateChat(connect, clienthandle, handle, addr)                           # Call the initiateChat function and send necessary variables
	connect.close()                                                             # Close connection if/when the initiateChat function returns

"""
/************************************************************************
* Description: createSocket()
* Creates the socket through the socket API
* Returns socket information to main function
*************************************************************************/
"""

def createSocket():
	server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)                  # Call the necessary socket API to create the program socket
	server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

	server.bind(('',portnumber))											    # Bind the hostname and the port number to the socket
	server.listen(1)															# listen on socket for 1 connection maximum (additional connections rejected)
	return server																# return the server object that was created.

"""
/************************************************************************
* Description: main function
* Program starts here and calls functions as necessary
* Ensures that the user input the correct CLI arguments
* Assigns CLI Arguments to variables to pass to called functions
*************************************************************************/
"""

if __name__ == "__main__":
	if len(sys.argv) != 2:														# User MUST provide the appropriate TWO arguments (script_name, host_name, and port_number)
		print("Invalid script execution!")
		print("Expected Input:")												# Display example of how program should be executed
		print("python <script_name> <port_number>")
		print("\nExample:")
		print("python chatserve.py 3737")
		print("\nExiting the Program.")											# Notify User that the program is terminating and that they well need to re-run
		print("Please re-run with the correct parameters\n")
		exit()																	# Exited the program and return user to the command line
	else:
		print("Correct amount of arguments provided on command line!\n");

	portnumber = int(sys.argv[1])												# assign the portnumber (casts the third user argument to an integer)

	server = createSocket()														# Call the createSocket function and assign the return variable to server
	handle = getHandle()														# Call the gethandle function and assign the return variable to handle
	displayConDetails(portnumber, handle)								        # Call the displayConDetails function to show user the connect parameters

	while True:																	# Run forever loop until an administrator performs a Ctrl-C command to exit.
		establishConnection(portnumber, handle, server)

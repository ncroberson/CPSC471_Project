# CPSC471_Project
Repository for CPSC 471 group project in Fall 2018


Group Members:
	Noah Roberson ncroberson@csu.fullerton.edu
	Burke Masles burkemasles@csu.fullerton.edu
	Mohamed Samatar msamatar@csu.fullerton.edu

Programming language: C++

How to Execute Program:
	
	This program only runs on Windows computers as it uses 	Windows network APIs.

Server:
	1. cd to folder containing FTP_Server.exe
	2. Invoke server as "FTP_Server.exe <port>" with port being 	a port number from 0-65535
	3. server is now listening for connections on <port>.
Client: 
	1. cd to folder containiner FTP_Client.exe
	2. Invoke client as "FTP_Client.exe <host> <port>" with host 	being the hostname of the server, and port being the 	corresponding server listening port. 
	3. client will now attempt to make a connection with the 	server, and either fail with error or be ready for commands.
	4. an ftp> prompt will be displayed and commands are able to 	be sent to the server.
		Commands:
		- "ftp> help" - displays command help information
		
		- "ftp> get <filename>" - sends a request to server to 			retrieve file with name <filename>    
		
		- "ftp> put <filename>" - sends a request to server to 			send file with name <filename>
		
		- "ftp> quit" - destroys ftp session and exits client

Notes:

Our program implementation uses and builds upon portions of Winsocks example code found at URLs: 
<https://docs.microsoft.com/en-us/windows/desktop/winsock/complete-server-code>
and :
<https://docs.microsoft.com/en-us/windows/desktop/winsock/complete-client-code>
As well as low level file transfer code from StackOverflow page:
<https://stackoverflow.com/questions/25634483/send-binary-file-over-tcp-ip-connection>
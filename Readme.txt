CIS3210 Assignment 1
Authors: Daniel Deen(0896283), Haiqing Gong(0821883)

Compiling:
	A Makefile that compiles the server and the client
	 	make all creates executables server and client
	 	make clean deletes all executables and intermediate files
Run the program:
./server port 
./client ip:port [bufflen] bufflens max value is 4096 (Inputting the value is not necessary )
For example:
./server 12024   (12024 is my port number in the server)
./client 127.0.0.1:12024

Script
The file of script.sh is for spawning and running multiple clients simultaneously. Type ./script.sh to run it. If it does not workk, try chmod +x script.sh.

This assignment transfer the text from smallTest.txt file from client to the server. You can also use your own file to test my program.

UI commands: 
works by getchar so type in a single character and then hit enter
'o' - kills the program
's' - stops the program from accepting new connections
'c' - continues to accept new connections 


File Collision functionality:
We chose to just replace the file if a file name collision occurs. We did this because all of our files are made after we have collected all the data so each file is complete. Since each file is complete we decided that the newer one is probably the most up to date file available. 
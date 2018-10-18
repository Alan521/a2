CIS3210 Assignment 1
Author: Haiqing Gong

Compiling:
	A Makefile that compiles the server and the client
	 	make all creates executables server and client
	 	make clean deletes all executables and intermediate files
Run the program:
./server port 
./client ip:port [bufflen] bufflen¡¯s max value is 4096 (Inputting the value is not necessary )
For example:
./server 12024   (12024 is my port number in the server)
./client 127.0.0.1:12024

Script£º
The file of script.sh is for spawning and running multiple clients simultaneously. Type ./script.sh to run it. If it does not workk, try chmod +x script.sh.

This assignment transfer the text from data.pdf file from client to the server. You can also use own file to test my program.


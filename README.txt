OPERATING SYSTEMS: LS PROGRAM
Joseph Pang 

Goal:

Made a program that emulates the same functions as the ls command. 
First you run the Makefile and then you operate the program by typing
"./myls [argument]", for example: ./myls -l

I made a serparate function that prints all the results/files+directories, 
a separate function to print the total disk blocks used by the files (for the -l option: "Total: ##"),
and a separate function to handle the -R option where it will recursively find directories and their subdirectories and print them out.

Working functionalities:

./myls 
./myls -i
./myls -l
./myls -R

These mimic: ls, ls -i, ls -l, ls -R command.

-i, -l, and -R were the only goals given from the assignment.
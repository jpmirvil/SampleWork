// James Mirvil
// COP 3402: System Software
// Project 4 - PL/0 Compiler
// 7/27/2015

To compile project use the following command:
	gcc -o <runfile> <sourcefile.c>
	example: gcc -o compile driver.c scanner.c parser.c vm.c

To Run the project use the following command:
	./<runfile>
	with the following options:
	-l Print the list of lexemes/tokens (scanner output) to the screen
	-a Print the generated assembly code (parser/codegen output) to the screen
	-v Print virtual machine execution trace (virtual machine output) to the screen
	
	example: ./compile -l -a -v
	Prints all the types of output to the console

Input

	Scanner:
	When runnng the project, you must include text file name input.txt.
	This must be in the same directory as the running program.
	If not included the program will terminate indicating this reason.

	Virtual Machine:
	Console prompt will appear that accepts a single integer value.
	This will appear if the input file uses the "9 0 1" command.


Output

	Scanner:
	This program writes text files named lexemetable.txt and lexemelist.txt
	which will contains the interpreted lexeme in both table and list form.

	Parser/Code Generator:
	This program writes a text file named mcode.txt which will contain the 
	generated code for the virtual machine.

	
	Virtual Machine:
	This program writes a text file named stacktrace.txt which will contain
	the results of the run: stack and register values per instruction.
	
	Program will print out the current stack to the console.
	This will appear if the input file uses the "9 0 0" command.



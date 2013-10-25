//Author   : Mohit Jain
//Roll No. : 201202164
//Course   : Operating Systems
//         : Assignment-2

//How to use : Provide as command line argument the file name(s).
//           : Hit space to print the next block of file on the terminal screen.
//Ouput      : Prints the file contents on the terminal screen like "more" command.

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>

//code for setting terminal settings taken from : http://www.delorie.com/gnu/docs/glibc/libc_375.html

struct termios saved_attributes;

/*void ClearScreen()
{
	  const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
	  write(STDOUT_FILENO,CLEAR_SCREE_ANSI,12);
}*/

void reset_input_mode (void)
{
	tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}
void set_input_mode (void)
{
	struct termios tattr;
	char *name;
	if (!isatty (STDIN_FILENO))
	{
		fprintf(stderr,"Not a terminal.\n");
		exit (EXIT_FAILURE);
	}

	tcgetattr(STDIN_FILENO, &saved_attributes);
	atexit (reset_input_mode);

	tcgetattr (STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON|ECHO);
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 0;
	tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}

int main(int argc,char* argv[])
{
//	printf ("argc -> %d\n argv -> %c\n",argc,argv[1][0]);
	
	if (argc<2)            //if no input is given through command line, throw an error.
	{
		printf("\n###Error! Please provide file name(s) as Command Line arguments!\n\n");
		return 0;
	}
	int i,j,src,size,newlinectr=0;
	char ch,space;
	for (i=1;i<argc;i++)
	{
		if (access(argv[i],R_OK)==0)       //cheack for access to given file.
		{
			src=open(argv[i],O_RDONLY);
			size=lseek(src,0L,2);
			newlinectr=0;
			printf ("\n<----------------File Name : "); //printing the file of the current file being displayed.
			for	(j=0;argv[i][j]!='\0';j++)
			{
				printf("%c",argv[i][j]);
			}
			printf(" ----------------->\n\n");
			for (j=0;j<size;j++)
			{
				if (newlinectr==20) //if newline counter is 20, wait for space input.
				{
					newlinectr=0;
					while (1)
					{
						set_input_mode(); // set terminal settings for non-cannonical input.
						read (STDIN_FILENO,&space,1);
						if (space == '\040')
						{
							reset_input_mode();   //reset terminal to normal settings once space input is recieved.
							break;
						}
					}
				}
				lseek(src,j,0);
				read(src,&ch,1); //read and write the file character by character.
				if (ch=='\n')
				{
					newlinectr++;
				}
				write(STDOUT_FILENO,&ch,1);
			}
		}
		else                 //if user doesnt have access to the file, throw this error.
		{
			printf ("\n###ERROR!! The file doesn't exist or the User doesnt have access to read the file : ");
			for (j=0;argv[i][j]!='\0';j++)
			{
				printf("%c",argv[i][j]);
			}
			printf("!! \n\n");
		}
	}
//	ClearScreen();             //clearing the screen
	system("clear");
	return 0;
}

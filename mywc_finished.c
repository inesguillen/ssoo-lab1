//P1-SSOO-23/24

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define FALSE 0
#define TRUE !FALSE

/* ERROR DETECTOR FUNCTION */
int gerror(int nParameter, char *Parameter[])
{
	char format[1000] = {0};
	int size = 0;
	
	// If there is an error
	if (errno > 0 || nParameter < 0)
	{
		// If the number of parameters is negative, we multiply by -1 to make it positive
		if (nParameter < 0) nParameter *= -1;

		// In case that we have enough arguments, but still there is an error
		if (nParameter > 0)
                {
			// We read each parameter and check which is the error
                        for (int i = 0; i < nParameter; i++)
                        {
        	                if (i > 0) size += sprintf(format + size, " %s", Parameter[i]);
	                        else size = sprintf(format,"%s", Parameter[i]);
                	        if (i+1 < nParameter) size += sprintf(format + size, "%c", ':');
                        }
                }
		
		// Print the error and return -1
                if (nParameter > 0) printf("%s: %s\n", format, strerror(errno));
                else printf("%s\n", strerror(errno));
                return -1;
        }

	// If there is no error, finish
        return 0;
}

/* MAIN FUNCTION */
int main(int argc, char *argv[])
{
	char *pfile;
	int fp;

	// If less than two arguments (argv[0] -> program, argv[1] -> file to process) print an error y return -1
	if(argc < 2)
	{
		char *error[]={"Too few arguments"};			
		return gerror(-1, (char **)error);

	}
	
	pfile=argv[1];
	
	// If we cannot open the file we return an error using the function 'gerror'
	if ((fp = open(pfile, O_RDONLY)) == -1){
		return gerror(argc, argv);
	}		
		
	int nlines, nwords, nbytes;
	char *pline;

	// Save memory space in variable 'pline'
	pline = malloc(1000 * sizeof(char));

	// If there is memory overflow return -1, error
	if (!pline) return -1;

	memset(pline,'\0', 1000);
	nlines = nwords = nbytes = 0;
	int siz = 0;

	// While we haven't read all the content in the file
	siz = read(fp, pline, 1000);
	int bword = FALSE;

	// Read byte by byte
	for(int i = 0; i < 1000 && i < siz; i++)
	{
		char c = *(pline+i);
		
		// Check the char we are reading
		if (c != ' ' && c != '\t' && c != '\n')
		{
			// We add a word to the words' counter and make boolean variable TRUE
			if (!bword)
			{	
				nwords++;
				bword = TRUE;
			}
		}	
		else
		{
			// Add a line in the lines' counter when we find a '\n'
			if (c == '\n')	nlines++;
			bword = FALSE;
		}

		// Always add a byte when we read a new char
		nbytes++;
	}
	
	free(pline); // Free space
	close(fp); // Close file
	
	// Print result
	printf(" %d %d %d %s\n", nlines, nwords, nbytes, argv[1]);
	return 0;
}

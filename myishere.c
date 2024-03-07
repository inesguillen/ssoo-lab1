//P1-SSOO-23/24

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <linux/limits.h>

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
            
			// If we have a system error, print error
            if (errno > 0)
                printf("%s: %s\n", format, strerror(errno));
            else
                printf("%s\n", format);
        }
    	else
        	printf("%s\n",  strerror(errno));
		
		// Return-1 and finish
		return -1;
    }

	// If there is no error, finish
    return 0;
}

int main(int argc, char *argv[]) {
	DIR *dir;

	// Data structure returned by function readdir
	struct dirent *entry;
	char actual_path[PATH_MAX] = {0};
	char filename[FILENAME_MAX] = {0};

	// If there are not enough arguments, return -1
	if (argc < 3)
	{
		const char *error[]={argv[0], "Not enough arguments"};
		return gerror(-2, (char **)error);
	}

	// Copy the path and name of file to their respective variables
	strcpy(actual_path, argv[1]);
	strcpy(filename, argv[2]);

	// If we cannot open the directory
	if ((dir = opendir(actual_path)) == NULL)
	{
		// Return error using gerror function
		return gerror(argc, (char **)argv);
	}

    int bfound=FALSE;

    // Read directory entries and try to find the asked file.
    while ((entry = readdir(dir)) != NULL && bfound == FALSE)  // Iterate until no more files exist
    {
		// If the name file matches with the one we are currently reading
        if (!strcmp(entry->d_name, filename)) bfound = TRUE;
    }

	// Print correspondent messages
    if (!bfound)
        printf("File %s is not in directory %s\n", filename, actual_path);
    else
        printf("File %s is in directory %s\n", filename, actual_path);

    // If we cannot close the directory, return error
    if (closedir(dir) == -1)
    {
		const char *error[]={argv[0], "closedir"};
		return gerror(-1, (char **) error);
    }

	return 0;
}

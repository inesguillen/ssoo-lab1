//P1-SSOO-23/24

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <linux/limits.h>





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
				else size = sprintf(format, "%s", Parameter[i]);
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

int main(int argc, char *argv[]) {
	DIR *dir;

	// Data structure returned by function readdir
	struct dirent *entry;
	char actual_path[PATH_MAX] = {0};
	char filename[FILENAME_MAX] = {0};

	// If there are not enough arguments, return -1
	if (argc < 3)
	{
		printf("Not enough arguments\n");
		return -1;

	}
	
	// Copy the path and name of file to their respective variables
	strcpy(actual_path, argv[1]);
	strcpy(filename, argv[2]);

	// Open the directory
	if ((dir = opendir(actual_path)) == NULL)
    {
    	// We format the error to return the same messages as ls command
		char erro[PATH_MAX + 25] = {0};
		sprintf(erro,"cannot access '%s'", argv[1]);
	
		// assign in the name file passed as argument the message we want to return
		argv[1] = erro;
	
		// Use function gerror and finish
        return gerror(argc, (char **)argv);
    }

	// Read directory entries and try to find the asked file.
    while ((entry = readdir(dir)) != NULL)  // Iterate until no more files exist
	{
        if (!strcmp(entry->d_name, filename))
		{
			printf("File %s is in directory %s\n", filename, dir);
	    // If we cannot close the directory, return error
    	if (closedir(dir) == -1) 
		{
			return gerror(-1, (char **)"closedir");
		}		
			return 0;
		}
    }
	printf("File %s is not in directory %s\n", filename, dir);
	// If we cannot close the directory, return error
    if (closedir(dir) == -1) 
	{
		return gerror(-1, (char **)"closedir");
	}		
	return 0;
}

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <errno.h>

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

int main(int argc, char *argv[]) {
    DIR *dir;

    // data structure returned by readdir
    struct dirent *entry;
    char actual_path[PATH_MAX] = {0};

    // There has been no argument passed
    if (argc < 2 ) 
    {
        if (getcwd(actual_path, sizeof(actual_path)) == NULL) {
            return gerror(argc, argv);
        }
	
    }
    // Copy the argument passed to the local variable 
    else  strcpy(actual_path, argv[1]);
    

    // Open the directory
    if ((dir = opendir(actual_path)) == NULL) 
    {
    	// we format the error to return the same messages as ls command
	char erro[PATH_MAX+25]={0};
	sprintf(erro,"cannot access '%s'", argv[1]);
	
	// assign in the name file passed as argument the message we want to return
	argv[1]=erro;
	
	// Use function gerror and finish
        return gerror(argc, (char **)argv);
    }

    // Read and print directory entries
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // If we cannot close the directory, return error
    if ( closedir(dir) == -1)
     {
        return gerror(-1, (char **)"closedir");
     }
	
    return 0;
}


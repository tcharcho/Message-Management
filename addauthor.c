#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stream.h"


int main(int argc, char *argv[])
{
	/* check command line parameters */
	if (argc == 1)
	{
		printf("Incorrect Usage: Please enter author ID and stream(s)\n");
		return 0;
	}

	char* authorID;
	char* streams;
	int i;


	if (strcmp(argv[1], "-r") == 0)
	{
		i = 2;
	}
	else
	{
		i = 1;
	}

	/* store author ID */
	authorID = malloc(strlen(argv[i]) + 2);
	strcpy(authorID, argv[i]);
	i++;

	while (i < argc && strcmp(argv[i], "-s") != 0)
	{
		strcat(authorID, " ");
		authorID = realloc(authorID, strlen(argv[i]) + strlen(authorID) + 2);
		strcat(authorID, argv[i]);
		i++;
	}
	if (i == argc || i+1 == argc)
	{
		printf("Incorrect Usage: Please enter author ID and stream(s)\n");
		free(authorID);
		return 0;
	}
	i++;

	streams = malloc(strlen(argv[i]) + 2);
	strcpy(streams, argv[i]);
	i++;

	while(i < argc)
	{
		strcat(streams, " ");
		streams = realloc(streams, strlen(argv[i]) + strlen(streams) + 2);
		strcat(streams, argv[i]);
		i++;
	}
	

	/* call library functions to add/remove user */
	if (strcmp(argv[1], "-r") == 0)
	{
		removeUser(authorID, streams);
	}
	else
	{
		addUser(authorID, streams);
	}

	free(authorID);
	free(streams);

	return 0;
}






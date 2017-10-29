#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"stream.h"

class PostEntry {
	char* readInput(char* input, int size, FILE* fh)
	{
		char ch = 0;
		int len = 0;

		strcpy(input, "");

		ch = getc(fh);

		while( ch != EOF )
		{
			if(ch != 13)
			{
				strncat(input,&ch,1 );
				len++;
			}


			if (len == size)
			{
				size = size*2;
				input = realloc(input, sizeof(char) * size);
			}

			ch = getc(fh);

		}
		if(input[len-1] != '\n')
		{
			strcat(input, "\n");
			len++;
		}
		return input;
	}

	void getTimeDate(char* dateTime)
	{
		struct tm* tm;
		time_t rawTime;

		/* get current time and date */
		time(&rawTime);
		tm = localtime(&rawTime);

		/* write and format date and time to string */
		sprintf(dateTime, "%02d-%02d-%d %02d:%02d:%02d", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	}

	void formatEntry(struct userPost* myUserPost, char* authorID, char* stream, char* input, char* dateTime)
	{	
		/* copy post information into struct */
		myUserPost->username = malloc(strlen(authorID) + 1);
		myUserPost->streamname = malloc(strlen(stream) + 1);
		myUserPost->text = malloc(strlen(input) + 1);
		myUserPost->date = malloc(strlen(dateTime) + 1);

		strcpy(myUserPost->username, authorID);
		strcpy(myUserPost->streamname, stream);
		strcpy(myUserPost->text, input);
		strcpy(myUserPost->date, dateTime);
	}

	void submitPost(struct userPost* myUserPost)
	{
		updateStream(myUserPost);

		/* free struct elements */
		free(myUserPost->username);
		free(myUserPost->streamname);
		free(myUserPost->text);
		free(myUserPost->date);
	}
};


int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		printf("Incorrect Usage: Please enter an author ID and stream.\n");
		return 1;
	}

	class PostEntry post;
	struct userPost myUserPost;
	FILE* fh;
	char* stream;
	char* authorID;
	char* input;
	char* dateTime;
	int i = 1;
	int size = 80;
	


	/* store author ID */
	authorID = malloc(strlen(argv[i]) + 2);
	strcpy(authorID, argv[i]);
	i++;

	while (i < argc && strcmp(argv[i], "-s") != 0)
	{
		strcat(authorID, " ");
		authorID = realloc(authorID, sizeof(argv[i]) + sizeof(authorID) + sizeof(char)*2);
		strcat(authorID, argv[i]);
		i++;
	}
	i++;
	if (i >= argc)
	{
		printf("Incorrect Usage: Please enter an author ID and stream.\n");
		return 1;
	}

	/* store stream name */
	stream = malloc(strlen(argv[i]) + 2);
	strcpy(stream, argv[i]);
	i++;

	while(i < argc)
	{
		strcat(stream, " ");
		stream = realloc(stream, sizeof(argv[i]) + sizeof(stream) + sizeof(char)*2);
		strcat(stream, argv[i]);
		i++;
	}



	input = malloc(sizeof(char) * size);
	dateTime = malloc(sizeof(char) * size);

	fh = fopen("messages/post.txt", "r");
	if (fh == NULL)
	{
		printf("Error: Please submit a post.\n");
		free(stream);
		free(authorID);
		free(input);
		free(dateTime);
		return 1;
	}


	/* get post input */
	input = post.readInput(input, size, fh);

	/* get current date and time */
	getTimeDate(dateTime);

	/* create user post struct */
	post.formatEntry(&myUserPost, authorID, stream, input, dateTime);

	/* post to stream */
	post.submitPost(&myUserPost);


	free(stream);
	free(authorID);
	free(input);
	free(dateTime);
	fclose(fh);

	return 0;
}





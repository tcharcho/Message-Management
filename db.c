#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX_QUERY 5120
#define HOSTNAME  "dursley.socs.uoguelph.ca"
#define USERNAME  "tcharcho"
#define PASSWORD  "0886239"
#define DATABASE  "tcharcho"

void printHelp()
{
	printf("Usage:\n");
	printf("\t./db -clear\tremoves all of the posts, users, streams and any other information from the tables in the database.\n");
	printf("\t./db -reset\tdeletes the tables from the database.\n");
	printf("\t./db -posts\tprints out all posts stored in the database\n");
	printf("\t./db -users\tprints out all user names stored in the database.\n");
	printf("\t./db -streams\tprints out all stream names stored in the database.\n");
}

void getLast(int numargs, char const *args[])
{
	MYSQL_RES *res;
	MYSQL mysql;
	MYSQL_ROW row;
	char* authorID;
	char* stream;
	char query[MAX_QUERY];
	char* id;
	int i = 3;


	/* initialize connection */
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.\n");
	   return;
	}


	/********************** parse command (i.e. ./db -getLast authorID -s stream) **********************/

	authorID = malloc(strlen(args[2]) + 2);
	strcpy(authorID, args[2]);

	/* store authorID */
	while(i < numargs && strcmp(args[i], "-s") != 0)
	{
		strcat(authorID, " ");
		authorID = realloc(authorID, strlen(args[i]) + strlen(authorID) + 2);
		strcat(authorID, args[i]);
		i++;
	}

	i++;
	if (i >= numargs)
	{
		printf("Incorrect Usage: Please enter author ID and stream\n");
		return;
	}

	stream = malloc(strlen(args[i]) + 2);
	strcpy(stream, args[i]);
	i++;

	/* store stream name*/
	while(i < numargs)
	{
		strcat(stream, " ");
		stream = realloc(stream, strlen(args[i]) + strlen(stream) + 2);
		strcat(stream, args[i]);
		i++;
	}


	/********************** Get number of posts read from sql database **********************/

	/* store id of user (to search in users table) */
	id = malloc(strlen(authorID) + strlen(stream) + 1);
	sprintf(id, "%s%s", authorID, stream);

	query[0] = '\0';
	sprintf(query, "select postsRead from users where id=\'%s\'", id);

	mysql_query(&mysql, query);
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	/* print last UNREAD post (i.e. number of posts read + 1) */
	printf("%d\n", atoi(row[0]) + 1);


	mysql_free_result(res);
	free(authorID);
	free(stream);
	free(id);

	/* close connection */
	mysql_close(&mysql);
	mysql_library_end();
}

void getTotal(int numargs, char const *args[])
{
	MYSQL_RES *res;
	MYSQL_RES *res2;
	MYSQL mysql;
	MYSQL_ROW row;
	char* authorID;
	char* stream;
	char query[MAX_QUERY];
	int i = 3;
	unsigned long alltotal = 0;


	/* initialize connection */
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.\n");
	   return;
	}


	/********************** parse command (i.e. ./db -getTotal authorID -s stream) **********************/

	authorID = malloc(strlen(args[2]) + 2);
	strcpy(authorID, args[2]);

	/* store authorID */
	while(i < numargs && strcmp(args[i], "-s") != 0)
	{
		strcat(authorID, " ");
		authorID = realloc(authorID, strlen(args[i]) + strlen(authorID) + 2);
		strcat(authorID, args[i]);
		i++;
	}

	i++;
	if (i >= numargs)
	{
		printf("Incorrect Usage: Please enter author ID and stream\n");
		return;
	}

	stream = malloc(strlen(args[i]) + 2);
	strcpy(stream, args[i]);
	i++;

	/* store stream name*/
	while(i < numargs)
	{
		strcat(stream, " ");
		stream = realloc(stream, strlen(args[i]) + strlen(stream) + 2);
		strcat(stream, args[i]);
		i++;
	}


	/********************** find number of total posts in stream **********************/

	if (strcmp(stream, "all") == 0)
	{
		query[0] = '\0';
		sprintf(query, "select * from users where authorID=\'%s\'", authorID);

		mysql_query(&mysql, query);
		res = mysql_store_result(&mysql);

		while ((row = mysql_fetch_row(res)))
		{
			query[0] = '\0';
			sprintf(query, "select * from posts where stream=\'%s\'", row[2]);
			mysql_query(&mysql, query);
			res2 = mysql_store_result(&mysql);
			alltotal = alltotal + (unsigned long)mysql_num_rows(res2);
		}

		printf("%lu\n", alltotal);

		mysql_free_result(res2);
	}
	else
	{
		query[0] = '\0';
		sprintf(query, "select * from posts where stream=\'%s\'", stream);

		mysql_query(&mysql, query);
		res = mysql_store_result(&mysql);

		printf("%lu\n", (unsigned long)mysql_num_rows(res));
	}

	


	mysql_free_result(res);
	free(stream);
	free(authorID);

	/* close connection */
	mysql_close(&mysql);
	mysql_library_end();
}

void allRead(int numargs, char const *args[])
{
	MYSQL_RES *res;
	MYSQL_RES *res2;
	MYSQL_ROW row;
	MYSQL mysql;
	char* authorID;
	char* stream;
	char* id;
	char query[MAX_QUERY];
	int i = 3;
	unsigned long total = 0;


	/* initialize connection */
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.\n");
	   return;
	}


	/********************** parse command (i.e. ./db -allRead authorID -s stream) **********************/

	authorID = malloc(strlen(args[2]) + 2);
	strcpy(authorID, args[2]);

	/* store authorID */
	while(i < numargs && strcmp(args[i], "-s") != 0)
	{
		strcat(authorID, " ");
		authorID = realloc(authorID, strlen(args[i]) + strlen(authorID) + 2);
		strcat(authorID, args[i]);
		i++;
	}

	i++;
	if (i >= numargs)
	{
		printf("Incorrect Usage: Please enter author ID and stream\n");
		return;
	}

	stream = malloc(strlen(args[i]) + 2);
	strcpy(stream, args[i]);
	i++;

	/* store stream name*/
	while(i < numargs)
	{
		strcat(stream, " ");
		stream = realloc(stream, strlen(args[i]) + strlen(stream) + 2);
		strcat(stream, args[i]);
		i++;
	}

	/********************** find number of total posts in stream **********************/

	if (strcmp(stream, "all") == 0)
	{
		query[0] = '\0';
		sprintf(query, "select * from users where authorID=\'%s\'", authorID);

		mysql_query(&mysql, query);
		res = mysql_store_result(&mysql);

		while ((row = mysql_fetch_row(res)))
		{
			query[0] = '\0';
			sprintf(query, "select * from posts where stream=\'%s\'", row[2]);
			mysql_query(&mysql, query);
			res2 = mysql_store_result(&mysql);
			total = total + (unsigned long)mysql_num_rows(res2);
		}

		mysql_free_result(res2);
	}
	else
	{
		query[0] = '\0';
		sprintf(query, "select * from posts where stream=\'%s\'", stream);

		if(mysql_query(&mysql, query))
		{
			printf("select * from posts failed\n");
		}

		res = mysql_store_result(&mysql);

		total = (unsigned long)mysql_num_rows(res);
	}
	

	mysql_free_result(res);

	/********************** Mark all posts as read by user in that stream **********************/

	id = malloc(strlen(authorID) + strlen(stream) + 1);
	sprintf(id, "%s%s", authorID, stream);
	query[0] = '\0';
	sprintf(query, "update users set postsRead=%lu where id=\'%s\'", total, id);
	if(mysql_query(&mysql, query))
	{
		printf("update users set postsRead failed\n");
	}

	

	free(stream);
	free(authorID);

	/* close connection */
	mysql_close(&mysql);
	mysql_library_end();

}

void markRead(int numargs, char const *args[])
{
	MYSQL mysql;
	char* authorID;
	char* stream;
	char* id;
	char query[MAX_QUERY];
	int i = 3;


	/* initialize connection */
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.\n");
	   return;
	}


	/********************** parse command (i.e. ./db -markRead authorID -s stream) **********************/

	authorID = malloc(strlen(args[2]) + 2);
	strcpy(authorID, args[2]);

	/* store authorID */
	while(i < numargs && strcmp(args[i], "-s") != 0)
	{
		strcat(authorID, " ");
		authorID = realloc(authorID, strlen(args[i]) + strlen(authorID) + 2);
		strcat(authorID, args[i]);
		i++;
	}

	i++;
	if (i >= numargs)
	{
		printf("Incorrect Usage: Please enter author ID and stream\n");
		return;
	}

	stream = malloc(strlen(args[i]) + 2);
	strcpy(stream, args[i]);
	i++;

	/* store stream name*/
	while(i < numargs)
	{
		strcat(stream, " ");
		stream = realloc(stream, strlen(args[i]) + strlen(stream) + 2);
		strcat(stream, args[i]);
		i++;
	}


	/********************** Mark next post as read by user in that stream **********************/

	id = malloc(strlen(authorID) + strlen(stream) + 1);
	sprintf(id, "%s%s", authorID, stream);
	query[0] = '\0';
	sprintf(query, "update users set postsRead=postsRead +1 where id=\'%s\'", id);
	if(mysql_query(&mysql, query))
	{
		printf("update users set postsRead failed\n");
	}

	

	free(stream);
	free(authorID);
	free(id);

	/* close connection */
	mysql_close(&mysql);
	mysql_library_end();
}

void getPost(int numargs, char const *args[])
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_RES *res2;
	MYSQL_ROW row;
	char* authorID;
	char* stream;
	char query[MAX_QUERY];
	char temp[MAX_QUERY];
	int i = 3;
	unsigned long total = 0;
	int currPos = 0;
	int x;


	/* initialize connection */
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.\n");
	   return;
	}


	/********************** parse command (i.e. ./db -getPost authorID -s stream currPos) **********************/

	authorID = malloc(strlen(args[2]) + 2);
	strcpy(authorID, args[2]);

	/* store authorID */
	while(i < numargs && strcmp(args[i], "-s") != 0)
	{
		strcat(authorID, " ");
		authorID = realloc(authorID, strlen(args[i]) + strlen(authorID) + 2);
		strcat(authorID, args[i]);
		i++;
	}

	i++;
	if (i >= numargs)
	{
		printf("Incorrect Usage: Please enter author ID, stream and current position\n");
		free(authorID);
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}

	stream = malloc(strlen(args[i]) + 1);
	strcpy(stream, args[i]);
	i++;

	if (i >= numargs)
	{
		printf("Incorrect Usage: Please enter author ID, stream and current position\n");
		free(authorID);
		free(stream);
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}

	currPos = atoi(args[i]);



	/********************** find number of total posts in stream **********************/

	if (strcmp(stream, "all") == 0)
	{
		query[0] = '\0';
		sprintf(query, "select * from users where authorID=\'%s\'", authorID);

		mysql_query(&mysql, query);
		res = mysql_store_result(&mysql);

		while ((row = mysql_fetch_row(res)))
		{
			query[0] = '\0';
			sprintf(query, "select * from posts where stream=\'%s\'", row[2]);
			mysql_query(&mysql, query);
			res2 = mysql_store_result(&mysql);
			total = total + (unsigned long)mysql_num_rows(res2);
		}

		mysql_free_result(res2);
	}
	else
	{
		query[0] = '\0';
		sprintf(query, "select * from posts where stream=\'%s\'", stream);

		if(mysql_query(&mysql, query))
		{
			printf("select * from posts 1 failed\n");
		}

		res = mysql_store_result(&mysql);

		total = (unsigned long)mysql_num_rows(res);
	}

	

	mysql_free_result(res);

	if (total == 0 || currPos == 0)
	{
		printf("No posts to see. Check back later or make a new post!\n");
		free(authorID);
		free(stream);
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}


	/********************** Get all posts from stream and print post at current position **********************/

	if (strcmp(stream, "all") == 0)
	{
		query[0] = '\0';
		sprintf(query, "select * from users where authorID=\'%s\'", authorID);
		
		mysql_query(&mysql, query);
		res = mysql_store_result(&mysql);
		row = mysql_fetch_row(res);
		
		temp[0] = '\0';
		sprintf(temp, "select * from posts where stream=\'%s\'", row[2]);

		while((row = mysql_fetch_row(res)))
		{
			strcat(temp, " or stream=\'");
			strcat(temp, row[2]);
			strcat(temp, "\'");
		}

		strcat(temp, " order by date");

		mysql_free_result(res);

		mysql_query(&mysql, temp);
		res = mysql_store_result(&mysql);

		for (x = 0; x < currPos; x++)
		{
			row = mysql_fetch_row(res);
		}

		printf("Sender: %s\nDate: %s\nStream: %s\n%s", row[1], row[3], row[2], row[4]);

	}
	else
	{
		query[0] = '\0';
		sprintf(query, "select * from posts where stream=\'%s\' order by date", stream);

		if(mysql_query(&mysql, query))
		{
			printf("select * from posts 2 failed\n");
		}

		res = mysql_store_result(&mysql);

		for (x = 0; x < currPos; x++)
		{
			row = mysql_fetch_row(res);
		}

		printf("Sender: %s\nDate: %s\nStream: %s\n%s", row[1], row[3], row[2], row[4]);
	}

	



	mysql_free_result(res);



	free(stream);
	free(authorID);

	/* close connection */
	mysql_close(&mysql);
	mysql_library_end();

}

int getStreams(int numargs, char const *args[])
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* authorID;
	char query[MAX_QUERY];
	int i = 2;
	unsigned long total;

	/******************** initialize connection ********************/
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.\n");
	   return 0;
	}


	/********************** parse command (i.e. ./db -getStreams authorID) **********************/

	authorID = malloc(strlen(args[i]) + 2);
	strcpy(authorID, args[i]);
	i++;

	while(i < numargs)
	{
		strcat(authorID, " ");
		authorID = realloc(authorID, strlen(args[i]) + strlen(authorID) + 2);
		strcat(authorID, args[i]);
		i++;
	}


	/********************** get all streams that the user belongs to **********************/

	query[0] = '\0';
	sprintf(query, "select * from users where authorID=\'%s\'", authorID);

	if(mysql_query(&mysql, query))
	{
		printf("select * from users failed\n");
	}

	res = mysql_store_result(&mysql);

	total = (unsigned long)mysql_num_rows(res);

	if (total == 0)
	{
		printf("Error: Author does not have access to any streams. Try: adding the author to the stream first.\n");
		mysql_free_result(res);
		free(authorID);

		/* close connection */
		mysql_close(&mysql);
		mysql_library_end();
		return 0;
	}

	while ((row = mysql_fetch_row(res))) 
	{
		if (strcmp(row[2], "all") != 0)
		{
			printf("%s\n", row[2]);
		}
	}


	mysql_free_result(res);
	free(authorID);

	/* close connection */
	mysql_close(&mysql);
	mysql_library_end();

	return 1;

}

void create()
{
	MYSQL mysql;
	char query[MAX_QUERY];


	/******************** initialize connection ********************/
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.\n");
	   return;
	}


	/******************** create 3 static tables for program ********************/

	/* create streams table */
	query[0] = '\0';
	sprintf(query, "create table streams (stream varchar(25) binary, primary key(stream))");
	mysql_query(&mysql, query);

	/* create users table */
	query[0] = '\0';
	sprintf(query, "create table users (id varchar(50) binary, authorID varchar(25) binary, stream varchar(25) binary, postsRead int, primary key(id))");
	mysql_query(&mysql, query);

	/* create posts table */
	query[0] = '\0';
	sprintf(query, "create table posts (id varchar(75) binary, authorID varchar(25) binary, stream varchar(25) binary, date datetime, post text, primary key(id))");
	mysql_query(&mysql, query);


	/* close connection */
	mysql_close(&mysql);
	mysql_library_end();

}

void clear()
{
	MYSQL mysql;
	char query[MAX_QUERY];


	/******************** initialize connection ********************/
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.\n");
	   return;
	}


	/******************** clear posts, users, and streams table ********************/
	query[0] = '\0';
	sprintf(query, "delete from posts");
	if (mysql_query(&mysql, query))
	{
		printf("Unable to clear posts table.\n");
	}
	else
	{
		printf("Posts table successfully cleared.\n");
	}

	query[0] = '\0';
	sprintf(query, "delete from users");
	if (mysql_query(&mysql, query))
	{
		printf("Unable to clear users table.\n");
	}
	else
	{
		printf("Users table successfully cleared.\n");
	}

	query[0] = '\0';
	sprintf(query, "delete from streams");
	if (mysql_query(&mysql, query))
	{
		printf("Unable to clear streams table.\n");
	}
	else
	{
		printf("Streams table successfully cleared.\n");
	}

	/******************** close connection ********************/
	mysql_close(&mysql);
	mysql_library_end();
}

void reset()
{
	MYSQL mysql;
	char query[MAX_QUERY];


	/******************** initialize connection ********************/
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.\n");
	   return;
	}

	/******************** delete posts, users, and streams table ********************/
	query[0] = '\0';
	sprintf(query, "drop table posts");
	if (mysql_query(&mysql, query))
	{
		printf("Unable to drop posts table.\n");
	}
	else
	{
		printf("Posts table successfully dropped.\n");
	}

	query[0] = '\0';
	sprintf(query, "drop table users");
	if (mysql_query(&mysql, query))
	{
		printf("Unable to drop users table.\n");
	}
	else
	{
		printf("Users table successfully dropped.\n");
	}


	query[0] = '\0';
	sprintf(query, "drop table streams");
	if (mysql_query(&mysql, query))
	{
		printf("Unable to drop streams table.\n");
	}
	else
	{
		printf("Streams table successfully dropped.\n");
	}


	/******************** close connection ********************/
	mysql_close(&mysql);
	mysql_library_end();
}

void posts()
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];


	/******************** initialize connection ********************/
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.\n");
	   return;
	}

	/********************** Get all posts from streams table and print formatted posts **********************/

	query[0] = '\0';
	sprintf(query, "select * from posts");

	if(mysql_query(&mysql, query))
	{
		printf("Unable to select all posts.\n");
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}

	if (!(res = mysql_store_result(&mysql)))
	{
		printf("Unable to store all posts\n");
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}

	if ((unsigned long)mysql_num_rows(res) == 0)
	{
		printf("All steams are empty. No posts to see.\n");
	}

	while ((row = mysql_fetch_row(res)))
	{
		printf("Sender: %s\nDate: %s\nStream: %s\n%s", row[1], row[3], row[2], row[4]);
	}



	mysql_free_result(res);

	/******************** close connection ********************/
	mysql_close(&mysql);
	mysql_library_end();
}

void users()
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];


	/******************** initialize connection ********************/
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.\n");
	   return;
	}

	/********************** Get all unique users from the users table **********************/

	query[0] = '\0';
	sprintf(query, "select distinct(authorID) from users");

	if(mysql_query(&mysql, query))
	{
		printf("Unable to select all users.\n");
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}

	if (!(res = mysql_store_result(&mysql)))
	{
		printf("Unable to store all users\n");
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}

	if ((unsigned long)mysql_num_rows(res) == 0)
	{
		printf("No users in database.\n");
	}

	while ((row = mysql_fetch_row(res)))
	{
		printf("%s\n", row[0]);
	}



	mysql_free_result(res);

	/******************** close connection ********************/
	mysql_close(&mysql);
	mysql_library_end();
}

void streams()
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];


	/******************** initialize connection ********************/
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.\n");
	   return;
	}

	/********************** Get all stream names from the streams table **********************/

	query[0] = '\0';
	sprintf(query, "select * from streams");

	if(mysql_query(&mysql, query))
	{
		printf("Unable to select all streams.\n");
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}

	if (!(res = mysql_store_result(&mysql)))
	{
		printf("Unable to store all streams\n");
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}

	if ((unsigned long)mysql_num_rows(res) == 0)
	{
		printf("No streams in the database.\n");
	}

	while ((row = mysql_fetch_row(res)))
	{
		printf("%s\n", row[0]);
	}



	mysql_free_result(res);

	/******************** close connection ********************/
	mysql_close(&mysql);
	mysql_library_end();
}

int main(int argc, char const *argv[])
{
	if (argc == 1)
	{
		printf("ERROR: Invalid Syntax\n");	
		printf("./db -help\tto print out usage message.\n");
		return 1;
	}	

	if (strcmp(argv[1], "-clear") == 0)
	{
		clear();
	}
	else if (strcmp(argv[1], "-reset") == 0)
	{
		reset();
	}
	else if (strcmp(argv[1], "-posts") == 0)
	{
		posts();
	}
	else if (strcmp(argv[1], "-users") == 0)
	{
		users();
	}
	else if (strcmp(argv[1], "-streams") == 0)
	{
		streams();
	}
	else if (strcmp(argv[1], "-help") == 0)
	{
		printHelp();
	}
	else if (strcmp(argv[1], "-getLast") == 0)
	{
		getLast(argc, argv);
	}
	else if (strcmp(argv[1], "-getTotal") == 0)
	{
		getTotal(argc, argv);
	}
	else if (strcmp(argv[1], "-allRead") == 0)
	{
		allRead(argc, argv);
	}
	else if (strcmp(argv[1], "-markRead") == 0)
	{
		markRead(argc, argv);
	}
	else if (strcmp(argv[1], "-getPost") == 0)
	{
		getPost(argc, argv);
	}
	else if (strcmp(argv[1], "-getStreams") == 0)
	{
		return getStreams(argc, argv);
	}
	else if (strcmp(argv[1], "-create") == 0)
	{
		create();
	}
	else
	{
		printf("ERROR: Invalid Option\n");
		printf("./db -help\tto print out usage message.\n");
	}
	

	return 0;
}

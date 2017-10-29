#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "stream.h"

#define MAX_QUERY 51200
#define HOSTNAME  "dursley.socs.uoguelph.ca"
#define USERNAME  "tcharcho"
#define PASSWORD  "0886239"
#define DATABASE  "tcharcho"

void updateStream(struct userPost* st)
{
	MYSQL mysql;
	MYSQL_RES *res;
	char query[MAX_QUERY];
	char* id;
	char* temp;

	/* connect to database server */
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.");
	   mysql_close(&mysql);
	   mysql_library_end();
	   return;
	}

	/* create posts table */
	query[0] = '\0';
	sprintf(query, "create table posts (id varchar(75) binary, authorID varchar(25) binary, stream varchar(25) binary, date datetime, post text, primary key(id))");
	mysql_query(&mysql, query);


	/* check if stream exists */
	query[0] = '\0';
	sprintf(query, "select * from streams where stream=\'%s\'", st->streamname);
	if(mysql_query(&mysql, query))
	{
		/* stream table doesn't exist */
		printf("Error: The stream \"%s\" does not exist. Cannot post to stream. Try adding the user to create the stream\n", st->streamname);
		res = mysql_store_result(&mysql);
		mysql_free_result(res);
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}

	res = mysql_store_result(&mysql);
	if ((unsigned long)mysql_num_rows(res) == 0)
	{
		/* stream deosn't exist */
		printf("Error: The stream \"%s\" does not exist. Cannot post to stream. Try adding the user to create the stream\n", st->streamname);
		mysql_free_result(res);
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}

	mysql_free_result(res);

	/* check if user has access to stream */
	id = malloc(strlen(st->username) + strlen(st->streamname) + strlen(st->date) + 1);
	sprintf(id, "%s%s", st->username, st->streamname);
	query[0] = '\0';
	sprintf(query, "select * from users where id=\'%s\'", id);

	if(mysql_query(&mysql, query))
	{
		/* users table doesn't exist */
		printf("Error: The stream \"%s\" does not exist. Cannot post to stream. Try adding the user to create the stream\n", st->streamname);
		free(id);
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}

	res = mysql_store_result(&mysql);

	if ((unsigned long)mysql_num_rows(res) == 0)
	{
		/* user doesn't have access to the stream */
		printf("Error: the user %s does not have access to the stream \"%s\". Try adding the user to the stream first.\n", st->username, st->streamname);
		free(id);
		mysql_free_result(res);
		mysql_close(&mysql);
		mysql_library_end();
		return;
	}

	mysql_free_result(res);

	/* post to stream */
	query[0] = '\0';
	strcat(id, st->date);
	temp = malloc(strlen(st->text)*2 + 1);
	mysql_real_escape_string(&mysql, temp, st->text,(unsigned long) strlen(st->text));
	sprintf(query, "insert into posts values (\'%s\', \'%s\', \'%s\', \'%s\', \'%s\')", id, st->username, st->streamname, st->date, temp);
	if(mysql_query(&mysql, query))
	{
		printf("Error: insert into posts failed. Could not post.\n");
	}
	else
	{
		printf("%s has successfully posted to the %s stream.\n", st->username, st->streamname);
	}	
	

	/* close connection to server */
	mysql_close(&mysql);
	mysql_library_end();
	free(id);
	free(temp);
}

void addUser(char* username, char* list)
{
	MYSQL mysql;
	char query[MAX_QUERY];
	char* toke;
	char* id;
	int rows = 0;


	/* connect to database server */
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.");
	   mysql_close(&mysql);
	   mysql_library_end();
	   return;
	}


	/* create streams table */
	query[0] = '\0';
	sprintf(query, "create table streams (stream varchar(25) binary, primary key(stream))");
	mysql_query(&mysql, query);

	/* create users table */
	query[0] = '\0';
	sprintf(query, "create table users (id varchar(50) binary, authorID varchar(25) binary, stream varchar(25) binary, postsRead int, primary key(id))");
	mysql_query(&mysql, query);

	/* tokenize list of streams and add user to streams */
	toke = strtok(list, ", ");
	while(toke != NULL)
	{
		/* create stream */
		query[0] = '\0';
		sprintf(query, "insert into streams values (\'%s\')", toke);
		mysql_query(&mysql, query);

		/* add user */
		id = malloc(strlen(toke) + strlen(username) + 1);
		memset(id, 0, strlen(id));
		strcpy(id, "");
		query[0] = '\0';
		sprintf(id, "%s%s", username, toke);
		sprintf(query, "insert into users values(\'%s\', \'%s\', \'%s\', 0)", id, username, toke);
		mysql_query(&mysql, query);


		/* check if user was added */
		rows = mysql_affected_rows(&mysql);	
		if (rows == -1)
		{
			printf("Error: User %s already exists in stream \"%s\". Cannot add user.\n", username, toke);	
		}
		else
		{
			/* add a record for the all stream */
			free(id);
			id = malloc(strlen(username) + 4);
			memset(id, 0, strlen(id));
			strcpy(id, "");
			sprintf(id, "%sall", username);
			query[0] = '\0';
			sprintf(query, "insert into users values(\'%s\', \'%s\', \'all\', 0)", id, username);
			mysql_query(&mysql, query);

			printf("%s was successfully added to the %s stream.\n", username, toke);
		}


		toke = strtok(NULL, " , ");
		free(id);
	}

	/* close connection to server */
	mysql_close(&mysql);
	mysql_library_end();
}

void removeUser(char* username, char* list)
{
	MYSQL mysql;
	MYSQL_RES *res;
	char query[MAX_QUERY];
	char* toke;
	char* id;
	int rows = 0;

	/* connect to database server */
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) 
	{
	   printf("Could not connect to host.");
	   return;
	}


	/* tokenize list of streams and remove user from streams */
	toke = strtok(list, ", ");
	while(toke != NULL)
	{		
		/* try to remove the user from the stream */
		id = malloc(strlen(toke) + strlen(username) + 1);
		memset(id, 0, strlen(id));
		strcpy(id, "");
		query[0] = '\0';
		sprintf(id, "%s%s", username, toke);
		sprintf(query, "delete from users where id=\'%s\'", id);
		
		if(mysql_query(&mysql, query))
		{
			printf("Error: delete from users failed. Cannot remove user %s from stream \"%s\"\n", username, toke);
			free(id);
			toke = strtok(NULL, " , ");
			continue;
		}

		rows = mysql_affected_rows(&mysql);	

		/* was not able to remove user (user doesn't exist in stream or stream doesn't exist) */
		if (rows == 0)
		{
			/* check if stream exists */
			query[0] = '\0';
			sprintf(query, "select * from streams where stream=\'%s\'", toke);
			mysql_query(&mysql, query);

			res = mysql_store_result(&mysql);

			if ((unsigned long)mysql_num_rows(res) == 0)
			{
				printf("Error: The stream \"%s\" does not exist. Cannot remove user.\n", toke);
			}
			else
			{
				printf("Error: the user %s is not currently in stream \"%s\". Cannot remove user\n", username, toke);
			}

			mysql_free_result(res);
		}
		else
		{
			printf("%s was successfully removed from the %s stream.\n", username, toke);
		}


		toke = strtok(NULL, " , ");
		free(id);
	}


	/* close connection to server */
	mysql_close(&mysql);
	mysql_library_end();
}




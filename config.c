#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parseHeading(char* heading)
{
	int len = strlen(heading);
	char* temp = malloc(sizeof(char) * len);
	char* text = malloc(sizeof(char) * len);
	char size = '3';
	int i;

	strcpy(temp, "");
	strcpy(text, "HEADING");


	for (i = 0; i < len; i++)
	{
		/* ignore white space and commas */
		if (heading[i] != ' ' && heading[i] != '\n' && heading[i] != '\r' && heading[i] != '\t' && heading[i] != ',')
		{
			strncat(temp, &heading[i], 1);
		}


		if (strncmp(temp, "text=\"", 6) == 0)
		{
			memset(text, 0, strlen(text));
			strcpy(text, "");
			i++;
			
			while(heading[i] != '\"')
			{
				strncat(text, &heading[i], 1);
				i++;
			}

			/* clear temp variable */
			memset(temp, 0, strlen(temp));
			strcpy(temp, "");
		}
		else if (strncmp(temp, "size=", 5) == 0)
		{
			i++;
			/* ignore white space */
			while(heading[i] == ' ' || heading[i] == '\n' || heading[i] == '\r' || heading[i] == '\t')
			{
				i++;
			}
			size = heading[i];

			/* clear temp variable */
			memset(temp, 0, strlen(temp));
			strcpy(temp, "");
		}
	}

	printf("<H%c> %s</H%c>\n", size, text, size);

	free(temp);
	free(text);
}

void parseText(char* text)
{	
	FILE* fp;
	int len = strlen(text);
	char* theText = malloc(sizeof(char) * len);
	char* copy = malloc(sizeof(char) * len);
	char ch = 0;
	int i,j;

	strcpy(theText, "");
	strcpy(copy, "");

	/* copy the element config line without any unnecessary white spaces and trailing closing bracket */
	for (i = 0; i < len; i++)
	{
		if (text[i] != ' ' && text[i] != '\n' && text[i] != '\r' && text[i] != '\t' && text[i] != ')')
		{
			strncat(copy, &text[i], 1);
		}

		/* copy everything between quotation marks */
		if (text[i] == '\"')
		{
			i++;
			while(text[i] != '\"')
			{
				strncat(copy, &text[i], 1);
				i++;
			}
			strncat(copy, &text[i], 1);
		}
	}
	
	/* if there were no parameters specified, do nothing */
	if (strlen(copy) == 0)
	{
		printf("Default text\n");
		free(copy);
		free(theText);
		exit(1);
	}

	j = sscanf(copy,"text=\"%[^\"]\"", theText);

	/* print file contents */
	if (j != 1)
	{
		sscanf(copy,"file=\"%[^\"]\"", theText);
		fp = fopen(theText, "r");
		if (fp != NULL)
		{
			while((ch = getc(fp)) != EOF)
			{
				printf("%c", ch);
			}
			fclose(fp);
			printf("<BR>\n");
		}
	}
	else
	{
		/* print specified text */
		printf("%s<BR>\n", theText);
	}


	free(copy);
	free(theText);
}

void parseButton(char* button)
{
	int len = strlen(button);
	char* copy = malloc(sizeof(char)*len);
	char* name = malloc(sizeof(char)*len);
	char* link = malloc(sizeof(char)*len);
	int i,j;

	strcpy(copy, "");
	strcpy(name, "");
	strcpy(link, "");

	/* copy the element config line without any unnecessary white spaces and trailing closing bracket */
	for (i = 0; i < len; i++)
	{
		if (button[i] != ' ' && button[i] != '\n' && button[i] != '\r' && button[i] != '\t' && button[i] != ')')
		{
			strncat(copy, &button[i], 1);
		}

		/* copy everything between quotation marks */
		if (button[i] == '\"')
		{
			i++;
			while(button[i] != '\"')
			{
				strncat(copy, &button[i], 1);
				i++;
			}
			strncat(copy, &button[i], 1);
		}
	}

	/* match config line to correct format and print HTML code */
	j = sscanf(copy,"name=\"%[^\"]\",link=\"%[^\"]\"", name, link);
	if (j != 2)
	{
		j = sscanf(copy,"link=\"%[^\"]\",name=\"%[^\"]\"", link, name);
	}
	if ( j != 0)
	{
		printf("<form method=\"post\" action=\"%s\">\n", link);
		printf("<input type=\"submit\" value=\"%s\">\n", name);
		printf("</form>\n");
	}


	free(copy);
	free(name);
	free(link);
}
/* TO-DO: double check if this is what we're supposed to do */
void parseExec(char* exec)
{
	int len = strlen(exec);
	char* copy = malloc(sizeof(char) * len);
	char* ex = malloc(sizeof(char) * len);
	char* temp = malloc(sizeof(char) * len);
	FILE* fp;
	int i;

	strcpy(copy, "");
	strcpy(temp, "");
	strcpy(ex, "");

	/* copy the element config line without any unnecessary white spaces and trailing closing bracket */
	for (i = 0; i < len; i++)
	{
		if (exec[i] != ' ' && exec[i] != '\n' && exec[i] != '\r' && exec[i] != '\t' && exec[i] != ')')
		{
			strncat(copy, &exec[i], 1);
		}

		/* copy everything between quotation marks */
		if (exec[i] == '\"')
		{
			i++;
			while(exec[i] != '\"')
			{
				strncat(copy, &exec[i], 1);
				i++;
			}
			strncat(copy, &exec[i], 1);
		}
	}

	sscanf(copy, "exe=\"%[^\"]\"", ex);


	fp = fopen(ex, "r");
	if (fp == NULL)
	{
		strcpy(temp, "/bin/");
		strncat(temp, ex, strlen(ex));
		fp = fopen(temp, "r");
	}

	if (fp == NULL)
	{
		memset(temp, 0, strlen(temp));
		strcpy(temp, "");

		strcpy(temp, "/sbin/");
		strncat(temp, ex, strlen(ex));
		fp = fopen(temp, "r");
	}

	if (fp != NULL)
	{
		printf("exec(\"%s\");\n", ex);
		fclose(fp);
	}

	
	free(ex);
	free(copy);
	free(temp);
}

void parseInput(char* input)
{
	int len = strlen(input);
	char* copy = malloc(sizeof(char) * len);
	char* inputs = malloc(sizeof(char) * len);
	char* temp = malloc(sizeof(char) * len);
	char* action = malloc(sizeof(char) * len);
	char* text = malloc(sizeof(char) * len);
	char* name = malloc(sizeof(char) * len);
	char* value = malloc(sizeof(char) * len);
	char* buttons = malloc(sizeof(char) * len);
	char* buttonName = malloc(sizeof(char) * len);
	char* buttonVal = malloc(sizeof(char) * len);
	int numB = 0;
	int i,j,k,x;

	strcpy(copy, "");
	strcpy(inputs, "");
	strcpy(action, "");
	strcpy(text, "");
	strcpy(name, "");
	strcpy(value, "");
	strcpy(temp, "");
	strcpy(buttons, "");
	strcpy(buttonName, "");
	strcpy(buttonVal, "");

	/* copy the element config line without any unnecessary white spaces and trailing closing bracket */
	for (i = 0; i < len; i++)
	{
		if (input[i] != ' ' && input[i] != '\n' && input[i] != '\r' && input[i] != '\t' && input[i] != ')')
		{
			strncat(copy, &input[i], 1);
		}

		/* copy everything between quotation marks */
		if (input[i] == '\"')
		{
			i++;
			while(input[i] != '\"')
			{
				strncat(copy, &input[i], 1);
				i++;
			}
			strncat(copy, &input[i], 1);
		}
	}

	/* find pattern */
	j = sscanf(copy, "action=\"%[^\"]\",%[^\n]", action, inputs);

	if (j != 2)
	{
		j = sscanf(copy, "buttons=%d,%[^|]|action=\"%[^\"]\",%[^\n]", &numB, buttons, action, inputs);
	}

	
	if (j == 2 || j == 4) 
	{
		printf("<form action=\"%s\" method=\"post\">\n", action);

		for (k = 0; k < strlen(inputs); k++)
		{
			if (inputs[k] != ',')
			{
				strncat(temp, &inputs[k], 1);
			}

			if (strncmp(temp, "text=\"", 6) == 0)
			{
				k++;
				while(inputs[k] != '\"')
				{
					strncat(text, &inputs[k], 1);
					k++;
				}

				/* clear variables */
				memset(temp, 0, strlen(temp));
				strcpy(temp, "");
			}
			else if (strncmp(temp, "name=\"", 6) == 0)
			{
				k++;
				while(inputs[k] != '\"')
				{
					strncat(name, &inputs[k], 1);
					k++;
				}

				/* clear variables */
				memset(temp, 0, strlen(temp));
				strcpy(temp, "");
			}
			else if (strncmp(temp, "value=\"", 7) == 0)
			{
				k++;
				while(inputs[k] != '\"')
				{
					strncat(value, &inputs[k], 1);
					k++;
				}

				/* once all t he information has been stored, print the html tag */
				printf("%s<input type=\"text\" name=\"%s\" placeholder=\"%s\"><br>\n", text, name, value);

				/* clear variables */
				memset(temp, 0, strlen(temp));
				strcpy(temp, "");
				memset(text, 0, strlen(text));
				strcpy(text, "");
				memset(name, 0, strlen(name));
				strcpy(name, "");
				memset(value, 0, strlen(value));
				strcpy(value, "");
			}
		}
	}

	if (numB != 0)
	{
		memset(temp, 0, strlen(temp));
		strcpy(temp, "");


		for (x = 0; x < strlen(buttons); x++)
		{
			if (buttons[x] != ',')
			{
				strncat(temp, &buttons[x], 1);
			}

			if (strncmp(temp, "name=\"", 6) == 0)
			{
				x++;
				while(buttons[x] != '\"')
				{
					strncat(buttonName, &buttons[x],1);
					x++;
				}

				memset(temp, 0, strlen(temp));
				strcpy(temp, "");
			}
			else if (strncmp(temp, "value=\"", 7) == 0)
			{
				
				x++;
				while(buttons[x] != '\"')
				{
					strncat(buttonVal, &buttons[x], 1);
					x++;
				}

				printf("<input type=\"submit\" name=\"%s\" value=\"%s\">\n", buttonName, buttonVal);

				memset(temp, 0, strlen(temp));
				strcpy(temp, "");
				memset(buttonName, 0, strlen(buttonName));
				strcpy(buttonName, "");
				memset(buttonVal, 0, strlen(buttonVal));
				strcpy(buttonVal, "");

			}
		}
		printf("</form>\n");
	}
	else
	{
		printf("<input type=\"submit\" name=\"inputButton\" value=\"Submit\">\n");
		printf("</form>\n");
	}
	


	free(copy);
	free(inputs);
	free(action);
	free(text);
	free(name);
	free(value);	
	free(temp);
	free(buttons);
	free(buttonName);
	free(buttonVal);
}

void parseLink(char* link)
{
	int len = strlen(link);
	char* copy = malloc(sizeof(char) * len);
	char* text = malloc(sizeof(char) * len);
	char* theLink = malloc(sizeof(char) * len);
	int i,j;

	strcpy(copy, "");
	strcpy(text, "link");
	strcpy(theLink, "");

	/* copy the element config line without any unnecessary white spaces and trailing closing bracket */
	for (i = 0; i < len; i++)
	{
		if (link[i] != ' ' && link[i] != '\n' && link[i] != '\r' && link[i] != '\t' && link[i] != ')')
		{
			strncat(copy, &link[i], 1);
		}

		/* copy everything between quotation marks */
		if (link[i] == '\"')
		{
			i++;
			while(link[i] != '\"')
			{
				strncat(copy, &link[i], 1);
				i++;
			}
			strncat(copy, &link[i], 1);
		}
	}


	j = sscanf(copy,"text=\"%[^\"]\",link=\"%[^\"]\"", text, theLink);

	if (j != 2)
	{
		j = sscanf(copy,"link=\"%[^\"]\",text=\"%[^\"]\"", theLink, text);
	}

	if (j == 2)
	{
		printf("<a href=\"%s\">%s</a>\n", theLink, text);
	}



	free(theLink);
	free(copy);
	free(text);
}

void parsePicture(char* picture)
{
	int len = strlen(picture);
	char* copy = malloc(sizeof(char)*len);
	char* name = malloc(sizeof(char)*len);
	int h = 100;
	int w = 100;
	int i,j;

	strcpy(copy, "");

	/* copy the element config line without any unnecessary white spaces and trailing closing bracket */
	for (i = 0; i < len; i++)
	{
		if (picture[i] != ' ' && picture[i] != '\n' && picture[i] != '\r' && picture[i] != '\t' && picture[i] != ')')
		{
			strncat(copy, &picture[i], 1);
		}

		/* copy everything between quotation marks */
		if (picture[i] == '\"')
		{
			i++;
			while(picture[i] != '\"')
			{
				strncat(copy, &picture[i], 1);
				i++;
			}
			strncat(copy, &picture[i], 1);
		}
	}


	if (strlen(copy) == 0)
	{
		free(copy);
		free(name);
		exit(1);
	}


	/* match config line with parameters and print html code */
	j = sscanf(copy,"image=\"%[^\"]\",size=%dx%d", name, &w, &h);

	if (j != 3)
	{
		j = sscanf(copy,"size=%dx%d,image=\"%[^\"]\"", &w, &h, name);
	}

	if (j != 3)
	{
		j = sscanf(copy,"image=\"%[^\"]\"", name);
	}

	if (j == 3 || j == 1)
	{
		printf("<img src=\"%s\" style=\"width:%dpx;height:%dpx;\">\n", name, w, h);
	}


	free(copy);
	free(name);
}

void parseRadioButton(char* radioButton)
{
	int len = strlen(radioButton);
	char* copy = malloc(sizeof(char)*len);
	char* action = malloc(sizeof(char)*len);
	char* name = malloc(sizeof(char)*len);
	char* values = malloc(sizeof(char)*len);
	char* val = malloc(sizeof(char)*len);
	char* temp = malloc(sizeof(char)*len);
	int numVals = 0;
	int i,j,k;

	strcpy(copy, "");
	strcpy(action, "");
	strcpy(name, "");
	strcpy(values, "");
	strcpy(val, "");
	strcpy(temp, "");

	/* copy the element config line without any unnecessary white spaces and trailing closing bracket */
	for (i = 0; i < len; i++)
	{
		if (radioButton[i] != ' ' && radioButton[i] != '\n' && radioButton[i] != '\r' && radioButton[i] != '\t' && radioButton[i] != ')')
		{
			strncat(copy, &radioButton[i], 1);
		}

		/* copy everything between quotation marks */
		if (radioButton[i] == '\"')
		{
			i++;
			while(radioButton[i] != '\"')
			{
				strncat(copy, &radioButton[i], 1);
				i++;
			}
			strncat(copy, &radioButton[i], 1);
		}
	}

	if (strlen(copy) == 0)
	{
		free(copy);
		free(action);
		free(name);
		free(values);
		free(val);
		free(temp);
		exit(1);
	}

	j = sscanf(copy,"action=\"%[^\"]\",name=\"%[^\"]\",%s", action, name, values);

	if (j != 3)
	{
		j = sscanf(copy,"name=\"%[^\"]\",action=\"%[^\"]\",%s", name, action, values);
	}

	if (j == 3)
	{
		printf("<form action=\"%s\" method=\"post\">\n", action);

		for (k = 0; k < strlen(values); k++)
		{
			if (values[k] != ',')
			{
				strncat(temp, &values[k], 1);
			}

			if (strncmp(temp, "value=\"", 7) == 0)
			{
				k++;
				while(values[k] != '\"')
				{
					strncat(val, &values[k], 1);
					k++;
				}
				numVals++;

				/* set first value as default */
				if (numVals == 1)
				{
					printf("<input type=\"radio\" name=\"%s\" value=\"%s\" checked>%s<br>\n", name, val, val);
				}
				else
				{
					printf("<input type=\"radio\" name=\"%s\" value=\"%s\">%s<br>\n", name, val, val);
				}
				

				/* clear variables */
				memset(temp, 0, strlen(temp));
				strcpy(temp, "");
				memset(val, 0, strlen(val));
				strcpy(val, "");

			}
		}

		printf("<input type=\"submit\" name=\"submitButton\" value=\"Submit\">\n");
		printf("</form>\n");
	}


	free(copy);
	free(action);
	free(name);
	free(values);
	free(val);
	free(temp);
}

void parseTextArea(char* textArea)
{
	int len = strlen(textArea);
	char* copy = malloc(sizeof(char) * len);
	char* action = malloc(sizeof(char) * len);
	char* text = malloc(sizeof(char) * len);
	char* name = malloc(sizeof(char) * len);
	char* value = malloc(sizeof(char) * len);
	char* hVals = malloc(sizeof(char) * len);
	char* temp = malloc(sizeof(char) * len);
	char* hName = malloc(sizeof(char) * len);
	char* hValue = malloc(sizeof(char) * len);
	int rows = 0;
	int cols = 0;
	int numH = 0;
	int i,j,k;

	strcpy(copy, "");
	strcpy(action, "");
	strcpy(text, "");
	strcpy(name, "");
	strcpy(value, "");
	strcpy(temp, "");
	strcpy(hName, "");
	strcpy(hValue, "");

	/* copy the element config line without any unnecessary white spaces and trailing closing bracket */
	for (i = 0; i < len; i++)
	{
		if (textArea[i] != ' ' && textArea[i] != '\n' && textArea[i] != '\r' && textArea[i] != '\t' && textArea[i] != ')')
		{
			strncat(copy, &textArea[i], 1);
		}

		/* copy everything between quotation marks */
		if (textArea[i] == '\"')
		{
			i++;
			while(textArea[i] != '\"')
			{
				strncat(copy, &textArea[i], 1);
				i++;
			}
			strncat(copy, &textArea[i], 1);
		}
	}


	j = sscanf(copy, "action=\"%[^\"]\",rows=%d,cols=%d,text=\"%[^\"]\",name=\"%[^\"]\",value=\"%[^\"]\"", action, &rows, &cols,text,name,value);

	if (j != 6)
	{
		j = sscanf(copy, "hidden=%d,%[^|]|action=\"%[^\"]\",rows=%d,cols=%d,text=\"%[^\"]\",name=\"%[^\"]\",value=\"%[^\"]\"", &numH, hVals, action, &rows, &cols,text,name,value);
	}

	if (j == 6)
	{
		printf("<form action=\"%s\" method=\"post\">\n", action);
		printf("%s\n", text);
		if (strcmp(name, "view") == 0)
		{
			printf("<textarea name=\"%s\" placeholder=\"%s\" rows=\"%d\" cols=\"%d\" readonly>\n", name, value, rows, cols);
			printf("</textarea>\n");
		}
		else
		{
			printf("<textarea name=\"%s\" placeholder=\"%s\" rows=\"%d\" cols=\"%d\"></textarea>\n", name, value, rows, cols);
			printf("<input type=\"submit\" value=\"Submit\">\n");
		}
		
		printf("</form>\n");
	}
	else if (j == 8)
	{
		printf("<form action=\"%s\" method=\"post\">\n", action);
		printf("%s\n", text);
		printf("<textarea name=\"%s\" placeholder=\"%s\" rows=\"%d\" cols=\"%d\"></textarea>\n", name, value, rows, cols);
		printf("<input type=\"submit\" value=\"Submit\">\n");

		memset(temp, 0, strlen(temp));
		strcpy(temp, "");

		for (k = 0; k < strlen(hVals); k++)
		{
			if (hVals[k] != ',')
			{
				strncat(temp, &hVals[k], 1);
			}

			if (strncmp(temp, "name=\"", 6) == 0)
			{
				k++;
				while(hVals[k] != '\"')
				{
					strncat(hName, &hVals[k], 1);
					k++;
				}

				memset(temp, 0, strlen(temp));
				strcpy(temp, "");
			}
			else if (strncmp(temp, "value=\"", 7) == 0)
			{
				k++;
				while(hVals[k] != '\"')
				{
					strncat(hValue, &hVals[k], 1);
					k++;
				}

				printf("<input type=\"hidden\" name=\"%s\" value=%s>\n", hName, hValue);

				memset(temp, 0, strlen(temp));
				strcpy(temp, "");
				memset(hName, 0, strlen(hName));
				strcpy(hName, "");
				memset(hValue, 0, strlen(hValue));
				strcpy(hValue, "");
			}
		}

		printf("</form>\n");
	}



	free(copy);
	free(action);
	free(text);
	free(name);
	free(value);
	free(hVals);
	free(temp);
	free(hName);
	free(hValue);
}

void parseFile(FILE* fp)
{
	int size1 = 100;
	int size2 = 100;
	int len1 = 0;
	int len2 = 0;
	int level = 0;
	char* temp = malloc(sizeof(char) * size1);
	char* element = malloc(sizeof(char) * size2);
	char ch = 0;

	if (temp == NULL || element == NULL)
	{
		exit(1);
	}

	printf("<HTML>\n");
	
	strcpy(temp, "");
	strcpy(element, "");


	/* read one character at a time */
	while((ch = getc(fp)) != EOF)
	{
		/* ignore white space */
		if (ch != ' ' && ch != '\n' && ch != '\r' && ch != '\t')
		{
			strncat(temp, &ch, 1);
			len1++;
		}

		if (len1 == size1)
		{
			size1 = size1 * 2;
			temp = realloc(temp, sizeof(char) * size1);
		}

		/* if we're at the beginning of a new element, read everything between the brackets */
		if (ch == '(')
		{
			level = 1;

			while(ch != ')' || level != 0)
			{
				ch = getc(fp);
				strncat(element, &ch, 1);
				len2++;
				
				if (len2 == size2)
				{
					size2 = size2 * 2;
					element = realloc(element, sizeof(char) * size2);
				}

				if (ch == '(')
				{
					level++;
				}
				else if (ch == ')')
				{
					level--;
				}
			}
		}


		/* check for start of element and call functions to parse element */
		if (strncmp(temp, ".h(", 3) == 0)
		{
			parseHeading(element);

			/* clear variables */
			memset(temp, 0, strlen(temp));
			memset(element, 0, strlen(element));
			strcpy(temp, "");
			strcpy(element, "");
			len2 = 0;
			len1 = 0;

		}
		else if (strncmp(temp, ".t(", 3) == 0)
		{
			parseText(element);

			/* clear variables */
			memset(temp, 0, strlen(temp));
			memset(element, 0, strlen(element));
			strcpy(temp, "");
			strcpy(element, "");
			len2 = 0;
			len1 = 0;
		}
		else if (strncmp(temp, ".b(", 3) == 0)
		{
			parseButton(element);

			/* clear variables */
			memset(temp, 0, strlen(temp));
			memset(element, 0, strlen(element));
			strcpy(temp, "");
			strcpy(element, "");
			len2 = 0;
			len1 = 0;
		}
		else if (strncmp(temp, ".e(", 3) == 0)
		{
			parseExec(element);

			/* clear variables */
			memset(temp, 0, strlen(temp));
			memset(element, 0, strlen(element));
			strcpy(temp, "");
			strcpy(element, "");
			len2 = 0;
			len1 = 0;
		}
		else if (strncmp(temp, ".i(", 3) == 0)
		{
			parseInput(element);

			/* clear variables */
			memset(temp, 0, strlen(temp));
			memset(element, 0, strlen(element));
			strcpy(temp, "");
			strcpy(element, "");
			len2 = 0;
			len1 = 0;
		}
		else if (strncmp(temp, ".l(", 3) == 0)
		{
			parseLink(element);

			/* clear variables */
			memset(temp, 0, strlen(temp));
			memset(element, 0, strlen(element));
			strcpy(temp, "");
			strcpy(element, "");
			len2 = 0;
			len1 = 0;
		}
		else if (strncmp(temp, ".p(", 3) == 0)
		{
			parsePicture(element);

			/* clear variables */
			memset(temp, 0, strlen(temp));
			memset(element, 0, strlen(element));
			strcpy(temp, "");
			strcpy(element, "");
			len2 = 0;
			len1 = 0;
		}
		else if (strncmp(temp, ".r(", 3) == 0)
		{
			parseRadioButton(element);

			/* clear variables */
			memset(temp, 0, strlen(temp));
			memset(element, 0, strlen(element));
			strcpy(temp, "");
			strcpy(element, "");
			len2 = 0;
			len1 = 0;
		}
		else if (strncmp(temp, ".a(", 3) == 0)
		{
			parseTextArea(element);

			/* clear variables */
			memset(temp, 0, strlen(temp));
			memset(element, 0, strlen(element));
			strcpy(temp, "");
			strcpy(element, "");
			len2 = 0;
			len1 = 0;
		}
		else if (strncmp(temp, ".d(", 3) == 0)
		{
			printf("<HR>\n");

			/* clear variables */
			memset(temp, 0, strlen(temp));
			memset(element, 0, strlen(element));
			strcpy(temp, "");
			strcpy(element, "");
			len2 = 0;
			len1 = 0;
		}


	}

	printf("</HTML>\n");

	free(temp);
	free(element);
}



int main(int argc, char const *argv[])
{
	/***************************************************** ERROR CHECKING *****************************************************/

	if (argc != 2)
	{
		printf("Usage: ./config <filename.wpml>\n");
		exit(1);
	}

	FILE* fp = fopen(argv[1], "r");

	if (fp == NULL)
	{
		/* write to error log??? */
		exit(1);
	}

	/***************************************************** START OF PROGRAM *****************************************************/
	parseFile(fp);


	/***************************************************** FREE MEMORY *****************************************************/
	fclose(fp);	

	return 0;
}

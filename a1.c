#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "a1.h"

int checkArgc(int argNum);
int checkExtention(char *args[]);
void parseToArray (FILE* infile, char** a, char** ws);

int main(int argc, char *argv[])
{
    /******************************** check the command line arguments ********************************/
    if (checkArgc(argc) != 1)
    {
            printf("Usage: %s filename.cc\n", argv[0]);
            return 0;
    }
    if (checkExtention(argv) != 1)
    {
            printf("Bad file extention\n");
            printf("Usage: %s filename.cc\n", argv[0]);
            return 0;
    }


    /***************************************** start program *****************************************/
    FILE* infile;
    char* fileName;
    char* outFileName;
    char* token;
    char** arr;
    char** ws;
    int classPos = 0;
    int fnPos = 0;
    int temp = 0;

    /* open C++ file and make sure it exists */
    infile = fopen(argv[1], "r+");
    if (infile == NULL)
    {
        printf("Could not open file. Exiting program...\n\n");
        return 0;
    }
    /* set the name of the output file for the C program */
    fileName = malloc(strlen(argv[1]) + 1);
    outFileName = malloc(strlen(argv[1]));

    strcpy(fileName, argv[1]);
    token = strtok(fileName, ".");
    strncpy(outFileName, token, strlen(token));
    strcat(outFileName, ".c");

    /* allocate memory for the array to hold the parsed information */
    arr = malloc(sizeof(char*) * 5000);
    ws = malloc(sizeof(char*) * 5000);
    int x;
    for(x = 0; x < 5000; x++)
    {
        arr[x] = malloc(sizeof(char) * 400);
        ws[x] = malloc(sizeof(char) * 400);
        strcpy(arr[x], "");
        strcpy(ws[x], "");
    }

    /* parse the input file */
    parseToArray(infile, arr, ws);


    /* find all classes */
    while ( (classPos = findClass(classPos, arr)) != -1)
    {
        temp = classPos;

        /* for each function: add function pointer and write to constructorClass function (function that assigns function pointers to pointers) */
        while( (fnPos = ifFunctions(temp, arr)) != -1 )
        {
            addFnPtr(classPos, fnPos, arr, ws);
            makeConstructor(fnPos, classPos, arr, 0, ws);
            temp = findFnEnd(fnPos, arr);
        }

        /* move the "};" to match the decleration of a struct (as opposed to a class) */
        closeStruct(classPos, arr, ws);

        /* print the class/struct constructor to array */
        makeConstructor(0, 0, arr, 1, ws);

        /* find all instances of the class and write the constructor call to the array (i.e. constructorA(&myA);) to assign function pointers */
        callConstructor(classPos, arr, ws);

    }
        
        /* find all words regarding to a class and change to struct */
        searchAndReplace(arr, "class", "struct");

        /* Print the converted C program to file */
        printToFile(arr, ws, outFileName);

        /* close all files */
        fclose(infile);

        /* free all allocated memory */
        free(fileName);
        free(outFileName);
        free2D(arr);
        free2D(ws);

        return 0;
}

int checkArgc(int argNum)
{
    if (argNum != 2)
    {
        return 0;
    }
    return 1;
}

int checkExtention(char *args[])
{
    int i = 0;
    int c;

    /* ensure that the user entered the name of a file (and not, for example, a folder)*/
    while (args[1][i] != '.')
    {
        i++;
        if (i >= strlen(args[1]))
        {
            return 0;
        }
    }

    /* ensure that the file extention is appropriate (i.e. a C++ file) */
    char *fileName = malloc(strlen(args[1]) + 1);
    strcpy(fileName, args[1]);

    char *token = strtok(fileName, ".");
    token = strtok(NULL, ".");


    c = strcmp("cc", token);

    free(fileName);

    if (c != 0)
    {
        return 0;
    }
        
    return 1;
}

void parseToArray (FILE* infile, char** a, char** ws)
{
    char ch = 0;
    char lastCh = '\n';
    int x = 0;
    int y = -1;

    while ((ch = getc(infile)) != EOF)
    {
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
        {
            /* whitespace: start new line (aka. increment counter for token array) */
            strncat(ws[y], &ch, 1);
            if (lastCh != ' ' && lastCh != '\t' && lastCh != '\n' && lastCh != '\r' && 
                lastCh != ';'  && lastCh != ',' && lastCh != '(' && lastCh != ')'  && lastCh != '{'  && lastCh != '}')
            {
                strcat(a[x], "\0");
                x++;
            }
        }
        else if (ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '{' || ch == '}')
        {
            /* punctuation: store char and start new line (aka. increment counter for token array) */
            if (lastCh != ' ' && lastCh != '\t' && lastCh != '\n' && lastCh != '\r' && 
                lastCh != ';'  && lastCh != ',' && lastCh != '(' && lastCh != ')'  && lastCh != '{'  && lastCh != '}')
            {
                strcat(a[x], "\0");
                x++;
            }
            strncat(a[x], &ch, 1);
            strcat(a[x], "\0");
            x++;
            y++;
        }
        else if (ch == '"')
        {
            /*if this is the start of a new token, increment the counter for the whitespace array*/
            if (lastCh == ' ' || lastCh == '\t' || lastCh == '\n' || lastCh =='\r' || 
                lastCh ==';'  || lastCh ==',' || lastCh == '(' || lastCh ==')'  || lastCh== '{'  || lastCh == '}')
            {
                y++;
            }

            /* read entire string */
            do
            {
                strncat(a[x], &ch, 1);
                lastCh = ch;
                ch = getc(infile);

                if (ch == '"' && lastCh == '\\')
                {
                    strncat(a[x], &ch, 1);
                    ch = getc(infile);
                }
            } while (ch != '"');

            strncat(a[x], &ch, 1);
        }
        else if (ch == '#' && (lastCh == '\n' || lastCh == '\r'))
        {
            y++;
            /* read entire line */
            do
            {
                strncat(a[x], &ch, 1);
                lastCh = ch;
                ch = getc(infile);
                } while (ch != '\n' && ch != '\r');

                strncat(ws[y], &ch, 1);
                strcat(a[x], "\0");
                x++;
                 
        }
        else if(ch == '/' && lastCh == '/')
        {
            /*if this is the start of a new token, increment the counter for the whitespace array*/
            if (lastCh == ' ' || lastCh == '\t' || lastCh == '\n' || lastCh =='\r' || 
                lastCh ==';'  || lastCh ==',' || lastCh == '(' || lastCh ==')'  || lastCh== '{'  || lastCh == '}')
            {
                y++;
            }

            /* read entire line */
            do
            {
                strncat(a[x], &ch, 1);
                lastCh = ch;
                ch = getc(infile);
            } while (ch != '\n' && ch != '\r');

            strncat(ws[y], &ch, 1);
            strcat(a[x], "\0");
            x++;
        }
         else
        {
            /* store char */
            strncat(a[x], &ch, 1);
            /*if this is the start of a new token, increment the counter for the whitespace array*/
            if (lastCh == ' ' || lastCh == '\t' || lastCh == '\n' || lastCh =='\r' || 
                lastCh ==';'  || lastCh ==',' || lastCh == '(' || lastCh ==')'  || lastCh== '{'  || lastCh == '}')
            {
                y++;
            }

        }
        lastCh = ch;
    }
        
}





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "a1.h"

void free2D (char** toFree)
{
    int i;
    for (i = 0; i < 5000; i++)
    {
        free(toFree[i]);
    }
    free(toFree);
}

void printToFile (char** tokens, char** ws, char* fileName)
{
    FILE* outfile;
    int z = 0;

    outfile = fopen(fileName, "w");

    while (strcmp(tokens[z], "") != 0)
    {
        fprintf(outfile, "%s", tokens[z]);
        fprintf(outfile, "%s", ws[z]);
        z++;
    }

    fclose(outfile);

}

void searchAndReplace (char** tokens, char* a, char* b)
{
    int x = 0;

    while (strcmp(tokens[x], "") != 0)
    {
        if (strcmp(tokens[x], a) == 0)
        {
            strcpy(tokens[x], b);
        }
        x++;
    }
}


int findClass (int pos, char** tokens)
{
    /* check if we are at the end of the array */
    if (pos >= 5000)
    {
        return -1;
    }

    /* search the array for class decleration patter i.e. class name{... */
    while (strcmp(tokens[pos], "") != 0)
    {
        if (strcmp(tokens[pos], "class") == 0 && strncmp(tokens[pos+2], "{", 1) == 0)
        {
            return pos+1;
        }
        pos++;

        /* check if we're at the end of the array */
        if (pos >= 5000)
        {
            return -1;
        }
    }

    return -1;
}

int ifFunctions (int pos, char** tokens)
{
    while(strncmp(tokens[pos], "(", 1) != 0)
    {
        if (strncmp(tokens[pos], "}", 1) == 0 && strncmp(tokens[pos+1], ";", 1) == 0)
        {
            return -1;
        }
        pos++;
    }

    return pos-1;
}

int findFnEnd(int fnPos, char** tokens)
{
    int level = -1;

    while(strcmp(tokens[fnPos], "}") != 0 || level != 0)
    {
        if (strcmp(tokens[fnPos], "{") == 0)
        {
            level++;
        }
        else if (strcmp(tokens[fnPos], "}") == 0)
        {
            level--;
        }

        fnPos++;
        
    }
    return fnPos;
}

int findStructEnd(int classPos, char** tokens)
{
    int temp;
    int structEnd;

    temp = classPos+1;
    structEnd = temp;

    while(strcmp(tokens[temp], "(") != 0)
    {
        if (strncmp(tokens[temp], ";", 1) == 0)
        {
            structEnd = temp;
        }
        temp++;
    }

    return structEnd;
}

void addFnPtr(int classPos, int fnPos, char** tokens, char** ws)
{
    char* params;
    char* fnPtr;
    char* fnName;
    int temp;
    int structEnd;


    /* store the function name */
    fnName = malloc(strlen(tokens[fnPos]) + 1);
    strcpy(fnName, tokens[fnPos]);

    /* initialize params */
    params = malloc(sizeof(char) * 300);
    strcpy(params, "");

    /* store the types of the parameters (if any) */
    if (strcmp(tokens[fnPos+2], ")") != 0)
    {
        temp = fnPos + 1;
        while(strcmp(tokens[temp+1], ")") != 0)
        {
            if (strcmp(tokens[temp+1], ",") != 0)
            {
                strcat(params, tokens[temp]);
                strcat(params, " ");
                temp++;
            }
            else if (strcmp(tokens[temp+1], ",") == 0)
            {
                strcat(params, ", ");
                temp = temp+2;
            }
        }  
    }
    strcat(params, ");");

    /* store the line declaring the function pointer: type \n\t(*fnName)(params) */
    fnPtr = malloc(sizeof(char) * 509);
    strcpy(fnPtr, "\n\t");
    strcat(fnPtr, tokens[fnPos-1]);
    strcat(fnPtr, " (*");
    strcat(fnPtr, fnName);
    strcat(fnPtr, ")");
    strcat(fnPtr, params);



    /* find where the end of the struct should be and add the function pointer */
    structEnd = findStructEnd(classPos, tokens);
    strcat(tokens[structEnd], fnPtr);


    free(params);
    free(fnPtr);
    free(fnName);
}


void closeStruct(int classPos, char** tokens, char** ws)
{
    int fnPos;
    int classEnd;
    int structEnd;

    /* find the position of the first function in the class, if any */
    fnPos = ifFunctions(classPos, tokens);

    /* if the class has functions */
    if (fnPos != -1)
    {
        /* find the end of the class (i.e. last token position in the array for the class) */
        classEnd = classPos;
        while (strcmp(tokens[classEnd], "") != 0)
        {
            if (strncmp(tokens[classEnd], "}", 1) == 0 && strncmp(tokens[classEnd+1], ";", 1) == 0)
            {
                break;
            }
            classEnd++;
        }
        classEnd++;

        /* find where the end of the struct should be */
        structEnd = findStructEnd(classPos, tokens);


        /* close the struct (i.e. concatenate "};" at the end) */
        strcat(tokens[structEnd], "\n};");

        /* delete the class end */
        strcpy(tokens[classEnd], " ");
        strcpy(tokens[classEnd-1], " ");

    }
}

void makeConstructor(int fnPos, int classPos, char** tokens, int print, char** ws)
{
    static char** fnDef;
    static int pos = 0;
    char* className;
    char* fnName;

    className = malloc(strlen(tokens[classPos]) + 1);
    strcpy(className, tokens[classPos]);
    fnName = malloc(strlen(tokens[fnPos]) + 1);
    strcpy(fnName, tokens[fnPos]);

    /* Once all the function pointers in the struct have been assigned the appropriate function, 
    the main calls makeConstructor with print=1 to write the constructor to file */
    if (print == 1)
    {
        int z = 0;
        int y = 0;

        /* close constuctor function */
        strcpy(fnDef[pos], "}\n\n");

        /* find the main function in the tokens array */
        while(strcmp(tokens[z], "") != 0)
        {
            if (strcmp(tokens[z], "int") == 0 && strcmp(tokens[z+1], "main") == 0)
            {
                break;
            }
            z++;
        }
        z--;

        /* add constructor function before main */
        while(strcmp(fnDef[y], "") != 0)
        {
            strcat(tokens[z], fnDef[y]);
            y++;
        }
        

        free2D(fnDef);
        fnDef = NULL;
        free(className);
        free(fnName);
        pos = 0;
        return;
    }

    if (fnDef == NULL)
    {
        int x;
        fnDef = malloc(sizeof(char*) * 5000);
        for (x = 0; x < 5000; x++)
        {
            fnDef[x] = malloc(sizeof(char) * 400);
            strcpy(fnDef[x], "");
        }

        strcpy(fnDef[0], "\n\nvoid constructor");
        strcat(fnDef[0], className);
        strcat(fnDef[0], "(struct ");
        strcat(fnDef[0], className);
        strcat(fnDef[0], "* obj){\n");

        pos = 1;
    }

    /* write constrcutor function, assigning function pointer to appropriate function */
    strcpy(fnDef[pos], "\tobj->");
    strcat(fnDef[pos], fnName);
    strcat(fnDef[pos], " = ");
    strcat(fnDef[pos], fnName);
    strcat(fnDef[pos], ";\n");
    pos++;

    free(className);
    free(fnName);

}

void callConstructor(int classPos, char** tokens, char** ws)
{
    char* className;
    char* fnCall;
    char* objName;
    int i = 0;

    className = malloc(strlen(tokens[classPos]) + 1);
    strcpy(className, tokens[classPos]);

    while(strcmp(tokens[i], "") != 0)
    {
        if (strcmp(tokens[i], "class") == 0 && strcmp(tokens[i+1], className) == 0 && strcmp(tokens[i+3], ";") == 0)
        {
            /* store object name */
            objName = malloc(strlen(tokens[i+2]) + 1);
            strcpy(objName, tokens[i+2]);

            /* store function call (to the constructor function for this class) */
            fnCall = malloc(strlen(objName) + strlen(className) + 18);
            strcpy(fnCall, "\n\tconstructor");
            strcat(fnCall, className);
            strcat(fnCall, "(&");
            strcat(fnCall, objName);
            strcat(fnCall, ");");
            /* write to array */
            strcat(tokens[i+3], fnCall);

            free(objName);
            free(fnCall);

        }
        i++;
    }

    free(className);
}







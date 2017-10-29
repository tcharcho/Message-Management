/*******************************************************
NAME: free2D
PURPOSE: frees allocated memory in a 2D array
RETURN: nothing
PARAMETERS: toFree- the 2D array to free
*******************************************************/
void free2D (char** toFree);

/*******************************************************
NAME: printToFile
PURPOSE: prints given 2D arrays to file, one element of
		 each array at a time (ensuring that the 
		 whitespace is similar to what was read in)
RETURN: nothing
PARAMETERS: tokens- tokens to print
			ws- whitespace to print
			fileName- file name to print to
*******************************************************/
void printToFile (char** tokens, char** ws, char* fileName);

/*******************************************************
NAME: searchAndReplace
PURPOSE: searches for a string in a 2D array and if found,
		 replaces with another string
RETURN: nothing
PARAMETERS: tokens- array to search in 
			a- string to look for
			b- string to replace a with
*******************************************************/
void searchAndReplace (char** tokens, char* a, char* b);

/*******************************************************
NAME: findClass
PURPOSE: find the position of the next class in a 2D 
		 array of tokens
RETURN: position in 2D array (where the class name is)
PARAMETERS: pos- current position to continue searching 
			from
			tokens- 2D array to look in
*******************************************************/
int findClass (int pos, char** tokens);

/*******************************************************
NAME: ifFunctions
PURPOSE: determines whether a class has functions or not
RETURN: if function is found, reaturns function position
		if function is not found, return -1
PARAMETERS: pos- current position to continue searching
			from
			tokens- 2D array to search in
*******************************************************/
int ifFunctions (int pos, char** tokens);


/*******************************************************
NAME: findFnEnd
PURPOSE: looks for the end of the function
RETURN: the index of the end of the function
PARAMETERS: fnPos- index of start of function (where
			function name is located)
			tokens- 2D array to search in
*******************************************************/
int findFnEnd(int fnPos, char** tokens);

/*******************************************************
NAME: findStructEnd
PURPOSE: looks for the end of the struct (i.e. where the
		 struct shoud end in the C program)
RETURN: position of teh end of struct in array
PARAMETERS: classPos- index of start of class (where
			class name is located)
			tokens- 2D array to search in
*******************************************************/
int findStructEnd(int classPos, char** tokens);

/*******************************************************
NAME: addFnPtr
PURPOSE: adds the function pointer to the given class
RETURN: nothing
PARAMETERS: classPos- index of start of class (where
			class name is located)
			fnPos- index of start of function (where
			function name is located)
			fnName- name of current function
			tokens- 2D array to search in
*******************************************************/
void addFnPtr(int classPos, int fnPos, char** tokens, char** ws);

/*******************************************************
NAME: closeStruct
PURPOSE: adds '};' at the appropriate point in C program
		 to indicate the end of a struct decleration
RETURN: nothing
PARAMETERS: classPos- index of start of class (where
			class name is located)
			tokens- 2D array to search in
*******************************************************/
void closeStruct(int classPos, char** tokens, char** ws);

/*******************************************************
NAME: makeConstructor
PURPOSE: Add functions called "Constructor<className>"
		 to the C file (1 for every class that has functions).
		 These constructor classes will be responsible for
		 assigning the function pointer in the struct to 
		 the appropriate functions. 
RETURN: nothing
PARAMETERS: newFnName- the new name of the function
			className- the class that the function belongs to
			tokens- array containing tokens to search 
			thourgh/ appeand to
			print- switch to indicate whether the constructor 
			function is ready to be appended to the array
*******************************************************/
void makeConstructor(int fnPos, int classPos, char** tokens, int print, char** ws);

/*******************************************************
NAME: callConstructor
PURPOSE: to write the code to the array that is responsible
		 for calling the constructor functions
		 i.e. constructorA(&myA);
RETURN: nothing
PARAMETERS: classPos- the position in the array where the
			class name is
			tokens- array containing tokens to search 
			thourgh/ appeand to
*******************************************************/
void callConstructor(int classPos, char** tokens, char** ws);



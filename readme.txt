This is the final project from a university course, Software System Development and Integration. It was built in 3 stages. It allows the user to add “message streams”, to write to “message streams” and to view “message streams” (i.e. a stream could be called “cat” where users post about their love of cats). Users must first be added to the stream before they can post or view.

Previous versions allowed the user access through the terminal and stored all the files remotely. Layers were later added to allow for a web interface and stored the data in a database on a server. 

C, C++, Python, HTML, PHP, and MySQL were used in the development of this program. To run, it is currently required that the files are all on the University of Guelph servers. This can be modified in the code to allow the system to run where there is access to a MySQL database.

To Run:
- copy files into appropriate directory (limitations mentioned above)
- cd into directory
- type make
- navigate to appropriate web page

________________________________________________________________________________
________________________________________________________________________________


Tamara Charchoghlyan
0886239
CIS*2750
Assignment 4

Limitations:
- cannot enter a stream named "all"
- the view does not toggle sort
- max size for author ID = 25 characters
- max size for stream name = 25 characters
- for all: the program treats all like a different stream. So if you've read the first 3 posts in streamA and the first 2 posts in streamB, choosing the all option will display all the posts in streamA and streamB (regardless of the fact that you've read some of them in their respective streams). If you were to return to the all stream, it would then display the posts you haven't read in the all stream (specifically).


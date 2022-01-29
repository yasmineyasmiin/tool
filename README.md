# code_source
From the description of Petri net this code is able to generate the observable paths then the abstract paths.

---------------------------------------------------------------------
--- REQUIREMENTS ----------------------------------------------------
---------------------------------------------------------------------

* A (not to old) C++ compiler
* A machine that supports 32 bit integers


---------------------------------------------------------------------
--- USE -----------------------------------------------------------
---------------------------------------------------------------------

To compile this collection of code you have to: 
-First,run the "make" command inside of the directory "buddy22" this will create/update the library libbdd.a
this directory includes the library of Binary Decision Diagrams.

-Second, run  "make" in the "parser" diractory \\ this directory is responsible for the creation of the  Petri net
from its description file  "file.net".

-Then, you run the "make" command in the principal diretory  to generate the executable file  "StructAnalysis".

-To run this executable on any exemple we hava to access the main directory and execute the command "./StructAnalysis ./Samples/file.net" 
In the paper we used the exemple file Samples/exemple.net.

# code_source
From the description of Petri net this code is able to generate the observable paths then the abstract paths.

--- REQUIREMENTS ----------------------------------------------------
---------------------------------------------------------------------

* A (not to old) C++ compiler..
* A machine that supports 32 bit integers.


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

--- Exemple ------------------------------------------
------------------------------------------------------
We treat the exemple of 2 dining philosophers.


----experimental result ----------------------------
-----------------------------------------------------

we have applied our approach to the Dining philosophers problem by varying each time the number of philosophers (2,3...100 phiilosophers).
we evaluate the size of the reachability graph, the number of observed transitions , the number of aggregates found by applying our tool 
and calculate the execution time for each example.
We notice that the size of the reachability graph is increasing exponentially.
The results are summarised in the attached table. 
![comparative table](https://github.com/yasmineyasmiin/tool/blob/aa652032d329aee3811e473fe80f6a489c80bd38/comparatif.png)

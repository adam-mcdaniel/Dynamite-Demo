This is Dynamite 0.2.0

THIS IS NOT MEANT TO WORK WITH THE EXAMPLE SCRIPTS!

The example scripts are all compatible with the Golang version of the Dynamite virtual machine,
So they will all run on Dynamite 0.1.6.
There is no compiler from Dynamite code to the C++ virtual machine code yet, so none of the example
scripts can be run using this machine.

If you compare the code for both versions of the virtual machine, youll find that the Golang version is
a much bigger mess and is horribly written. Im not very proud of it.
This, even though its my first big C++ project, is much better. It was a lot easier to write in C++ than in 
Golang because I approached this machine with the builtin types in mind.

The C++ machine has the following types
- Nums
- Strings
- Lists
- Trees
- Objects
All objects are Objects by default, and are modified into the 4 other types.


The Golang virtual machine has the following types
- Nums
- Strings
- Dicts   (Trees)
To add a form of lists to the Golang virtual machine, 
I had to write a Golang library that would use a Dictionary
as a List with a variable that would keep track of the number
of items in the Dictionary, so that they could be used in a for loop,
and be indexed and such.
However, this caused many problems.
I integrated the list library into the standard library, and made a
Dictionary definition for a List. This made using lists very strange,
because some of my programs would use the list library constructor for a list,
and others would use the dictionary definition to build a list with different methods.
Overall, it was a very inefficient and clunky system, and I regret not adding lists to
the builtin types when I first started making the machine.
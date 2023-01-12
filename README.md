# 0D-Tidal-Range (formerly BarraCUDA)

## 0D Modelling of tidal range schemes.

### Compilation with g++
The easiest way to compile the model on Mac/Linux, available on Windows as well (other options are available). All source files are in the src/ directory.
	
	$g++ -std=c++11 main.cpp -o BarraCUDA.out

(Use BarraCUDA.exe on Windows)

### To Do:
	- Improve documentation...
	- Better example models.
	- Develop ancillary services operation.

### Origins
Based off Jingjings rebuild of Reza's 0D model, but in C++ as opposed to Fortran with the aim of making a modern built and stored project.

### Notes:
  1. This model is mainly a learning project for me, testing the varius bits aong the way. 
  2. Try to use the word 'scheme' not 'Barrage' or 'Lagoon'
## Structure:
![0D model structure](https://github.com/NHanousek/0D-Tidal-Range/blob/AncillaryServices/Figures/0D-Model-Structure.png?raw=true)

## Principles:
  1. Flow from upstream to downstream is positive.
  2. Time: flow calcs to use metres and seconds. Main model clock to use hours. Hours and minutes only for user input and output
  3. If you do it more than twice, make it a function (and really if you do it more than once).
  4. Try and use consistent patterns and nomenclature between classes and functions.

                eg. input files always always given as:

                const string& fileName; // when called in the function input
                fileName= Lagoon.txt;   // when declared in the main program.

  5. use longNamesForThings to make it easier to determine what is being used.
  6. No user input after startup.
  7. Function first, performance next.
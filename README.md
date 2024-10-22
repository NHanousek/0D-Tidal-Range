# 0D-Tidal-Range (formerly BarraCUDA)

## 0D Modelling of tidal range schemes.

### References

- N. Hanousek; *Numerical modelling of tidal energy devices and structures as part of net zero*, Ph.D. Thesis, Cardiff University (2023) [URI](https://orca.cardiff.ac.uk/id/eprint/167131)
- N. Hanousek, R. Ahamdian, E. Lesurf; *Providing distributed electrical generation through retrofitting disused docks as tidal range energy schemes*, Renewable Energy (2023) [DOI](https://doi.org/10.1016/j.renene.2023.119149)
- T. Zhang, N. Hanousek, M. Qadrdan, R. Ahmadian; *A Day-Ahead Scheduling Model of Power Systems Incorporating Multiple Tidal Range Power Stations*, IEEE Transactions on Sustainable Energy  (2023) [DOI](https://doi.org/10.1109/tste.2022.3224231)

### Compilation with g++
The easiest way to compile the model on Mac/Linux, available on Windows as well (other options are available). All source files are in the src/ directory.
	
	$g++ -std=c++11 main.cpp -o BarraCUDA.out

### To Do:
	- Improve documentation...
	- Better example models.

### Origins
Based off [Jingjings](https://www.researchgate.net/profile/Jingjing-Xue-4) rebuild of [Reza's](https://www.researchgate.net/profile/Reza-Ahmadian-2) 0D model, but in C++ as opposed to Fortran with the aim of making a modern built and stored project - with maximum flexibility for the user.

### Notes:
  1. Use the word 'scheme' not 'Barrage' or 'Lagoon'
  2. This model was formerly known as BarraCUDA but use of CUDA in the model never occured so it got a more generic name.

## Structure:
![0D model structure](https://github.com/NHanousek/0D-Tidal-Range/blob/master/Figures/0D-Model-Structure.png?raw=true)

## Principles:
  1. Flow from upstream to downstream is positive.
  2. Time: flow calcs to use metres and seconds. Main model clock to use hours. Hours and minutes only for user input and output
  3. If you do it more than twice, make it a function (and really if you do it more than once).
  4. Try and use consistent patterns and nomenclature between classes and functions.

                eg. input files always always given as:

                const string& fileName; // when called in the function input
                fileName= Lagoon.txt;   // when declared in the main program.

  5. Use camelCase for variables and functions
  6. No user input after startup.

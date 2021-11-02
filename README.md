# BarraCUDA

## 0D Modelling of tidal range schemes.

### Compilation with g++
The easiest way to compile the model on Mac/Linux, available on Windows as well (other options are available). All source files are in the src/ directory.
	
	$g++ -std=c++11 main.cpp -o BarraCUDA.out

(Use BarraCUDA.exe on Windows)

### To Do:
	- Improve documentation...

### Origins
Based off Jingjings rebuild of Reza's 0D model, but in C++ as opposed to Fortran with the aim of making a modern built and stored project.

### Notes:
  1. This model is mainly a learning project for me, testing the varius bits aong the way. 
  2. Try to use the word 'scheme' not 'Barrage' or 'Lagoon'

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

## Approximate flow of model
1. Declarations and definitions
2. read a config file that pulls all the individual filenames together

    i. Initialise lagoon

    ii. Initialise mesh or area

    iii. Initialise turbines

    iv. Initialise external water level

3. check input parameters for bad inputs
4. initialise results parameters
5. Initialise modeling
6. if (generation == ebb only) 
  - mode will progress: hold -> fill -> hold -> generate -> hold -> fill


7. if (generation == two way)
  - mode will progress: fill -> hold -> generate -> sluice -> hold -> generate -> fill


8. Different modes will affect the behaviour differently:

   i. Holding:
   - turbineFlow = sluiceFlow = 0
   - internal water level = constant (unless seperate inflow)

   ii. Filling:
   - turbine flow = 0
   - external water level > internal water level
   - lagoon is filling up
   - tide is in flood movement

   iii. Generating
   - turbine flow > 0
   - head difference between levels is within desired range
   - water level in lagoon is changing depending on the ebb/flood state

   iv. Sluicing
   - turbine flow = 0
   - lagoon is emptying
   - tide is in ebb movement
9. Calculation of parameters/results at each step:
  - headDifference = lagoonWaterLevel - externalWaterLevel
  - flowTurbines = function of (headDifference) - turbine function
  - flowSluices = function of (headDifference) - sluices function
  - wettedArea = function of bathymettry/mesh - mesh function
  - lagoonWaterLevel(i+1) = lagoonWaterLevel(i) + (flowTurbines + flowSluices + externalFlows)dt/wettedArea
    - if wetted area drops to zero (the lagoon dries out) the lagoon water level stays at 0 until the tide begins to rise again

# BarraCUDA

## 0D Modelling of tidal range schemes eventually with a good amount of parallelisation and possibly CUDA too (but mainly because it makes for a cooler name).

### Origins
Based off Jingjings rebuild of Reza's 0D model, but in C++ as opposed to Fortran with the aim of making a modern built and stored project.

### Note:
  1. This model is mainly a learning project for me, testing the varius bits aong the way. Probably has at least six months before its anywhere near the existing model.
  2. I mainly work on it when i get frustrated at the existing models...
  3. I use the words Barrage and lagoon interchangeably a lot here, realistically it shouldn't make a huge difference at this stage.
  
## Plan:
### What the model should contain:
- [ ] Class Lagoon, 
   - Containing:
   - [x] All the basic info about the lagoon itself
   - Functions:
   - [x] Constructor that takes filename as input
   - [x] Constructor that takes no inputs, gives error lagoon.
   - [x] Sluice gate flow from orifice equation
   - [ ] (Genetic) functions to modify parameters based on genetic algorithm outputs
   
- [x] Class Mesh, read directly from a telemac .t3s file ideally
   - Containing:
   - [x] string title or info: give some basic info abut it
   - [x] int nPoints: Number of points
   - [x] int nElements: Number of elements
   - [x] array of triple doubles xyz: positions of all the points in 3D
   - [x] array of triple ints neighbourhod: the neighbour list for all the elements
   - Functions:
   - [x] Constructor that takes filename as input
   - [x] Constructor that takes no inputs
 
 - [ ] Class schemeArea, wetted area and water levels for the lagon/barrage
   - Containing:
   - [x] int number of points
   - [x] array of doubles Levels
   - [x] array of doubles Areas
   - Functions:
   - [ ] Constructor that takes in a Lagoon and a Mesh
   - [ ] Function that takes water level and returns wetted area
   
- [x] Class External Water Level, read from a telemac .ts1 file
   - Containing:
  - [x] string title: give some basic title/location
  - [x] string source: Where is the root data from/validated against
  - [x] int nReadings: number of readings
  - [x] double array time: time points for the data
  - [x] double array externalWaterLevel: water levels
  - Functions:
  - [x] Constructor that takes filename as input
  - [x] Constructor that makes a blank error object, from no inputs
  
- [ ] Class Turbines, read from a HQPTurbines file
   - Containing:
  - [x] double originalDiameter: original diameter of turbine
  - [x] double diameterTurbine: diameter of a single turbine
  - [x] double areaTurbine: total area of turbines
  - [x] string title: some kind of title infrmation
  - [x] int nPoints: number of points in the readings, one for head-power and one for head-flow
  - [x] double array headDifference: Head differences, one for head-power and one for head-flow
  - [x] double array powerOutput: Power output at head difference
  - [x] double array flowrate: flow rate at head difference
  - [x] double coeffDischarge: coefficient of discharge of the turbines when filling
  - Functions:
  - [ ] Constructor that takes filename and Lagoon as input
  - [ ] Function that returns flowRate and powerOutput for a given headDifference, filling or generating
  - [ ] (Genetic) functions to modify parameters based on genetic algorithm outputs
  
- [x] Class Sluices, built from lagoon info **DECIDED THIS CLASS IS UNNECESSARY**
   - Containing:
  - [ ] double areaSluices: the total area of the cluices
  - [ ] double coeffDischarge: coefficient of discharge of the sluices
  - Functions:
  - [ ] Constructor that takes Lagoon Data as input
  - [ ] Function that returns flow rate through them from head discharge (orifice equation)
  
 - [ ] Class Results, the working outputs of the model
    - Containing:
   - [ ] double array time: Time of model
   - [ ] double array lagoonWaterLevel
   - [ ] double array externalWaterLevel
   - [ ] double array lagoonWettedArea
   - [ ] double array powerGenerated
   - [ ] double array turbineFlow
   - [ ] double array sluiceFlow
   - [ ] int array lagoonMode
   - [ ] double array cumulativeElectricity
    - Functions:
   - [ ] Default constructor
   - [ ] printing to console
   - [ ] printing to file
   - [ ] functions to add new data as it is calculated
 
## Approximate flow of model
1. Declarations and definitions
2. read a config file that pulls all the individual filenames together
  
    i. Initialise lagoon
  
    ii. Initialise mesh
  
    iii. Initialise turbines
  
    iv. Initialise external water level
  
3. check input parameters for bad inputs
4. initialise results parameters
5. Initialise modeling
6. if (generation == ebb only) {
  - mode will progress: hold -> fill -> hold -> generate -> hold -> fill
  
        }
  
7. if (generation == two way) {
  - mode will progress: fill -> hold -> generate -> sluice -> hold -> generate -> fill
  
        }
  
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

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
   - [ ] All the basic info about the lagoon itself
   - Functions:
   - [ ] Constructor that takes filename as input
   - [ ] (Genetic) functions to modify parameters based on genetic algorithm outputs
   
- [ ] Class Mesh, read directly from a telemac .t3s file ideally
   - Containing:
   - [ ] string title or info: give some basic info abut it
   - [ ] int nPoints: Number of points
   - [ ] int nElements: Number of elements
   - [ ] array of triple doubles xyz: positions of all the points in 3D
   - [ ] array of triple ints neighbourhod: the neighbour list for all the elements
   - Functions:
   - [ ] Constructor that takes filename as input
   - [ ] Function that takes head difference and returns wetted area
   
- [ ] Class External Water Level, read from a telemac .ts1 file
   - Containing:
  - [ ] string title: give some basic title/location
  - [ ] string source: Where is the root data from/validated against
  - [ ] int nReadings: number of readings
  - [ ] double array time: time points for the data
  - [ ] double array externalWaterLevel: water levels
  - Functions:
  - [ ] Constructor that takes filename as input
  
- [ ] Class Turbines, read from a HQPTurbines file
   - Containing:
  - [ ] double originalDiameter: original diameter of turbine
  - [ ] double diameterTurbine: diameter of a single turbine
  - [ ] double areaTurbine: total area of turbines
  - [ ] string title: some kind of title infrmation
  - [ ] int nPoints: number of points in the readings
  - [ ] doube array headDifference: Head differences
  - [ ] doube array powerOutput: Power output at head difference
  - [ ] doube array flowrate: flow rate at head difference
  - [ ] double coeffDischarge: coefficient of discharge of the turbines when filling
  - Functions:
  - [ ] Constructor that takes filename and Lagoon as input
  - [ ] Function that returns flowRate and powerOutput for a given headDifference, filling or generating
  - [ ] (Genetic) functions to modify parameters based on genetic algorithm outputs
  
- [ ] Class Sluices, built from lagoon info
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

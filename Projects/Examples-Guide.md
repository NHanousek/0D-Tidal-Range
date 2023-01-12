# Examples of 0D model operation
These are all based on the Mersey Tidal Barrage scheme, with different options and configurations enabled.
Not all options etc. are included. For a comprehensive list of the types of settings available I would go to the tidalRangeScheme.hpp file and look at the data loader function. Here there is a large if-else tree of options that can be set by the operator, and the default settings for the variables.

## EX1_Fixed_Ebb
Fixed, ebb-only operation, one of the most basic configurations.
    Modelfile - BarraCUDA.dat
    Schemefile - MTL-FX-EB.dat
    Waterlevel - MTL-Long-2018.ts1
    Turbines - HQPTurbines.dat
    Area - Scheme_Area.out

## EX2_Grid_2Way
Flexible - energy based, two-way operation, using a grid search method. Here you can see in the scheme file the settings/parameters given to the model to run a grid-based operation, and the flex info file output that is produced.
    Modelfile - BarraCUDA.dat
    Schemefile - MTL-FL-2W.dat
    Waterlevel - MTL-Long-2018.ts1
    Turbines - HQPTurbines.dat
    Area - Scheme_Area.out

## EX3_Genetic-2Way
Flexibe - profit based (note the inclusion of energy value as file in both model and scheme files, which my default triggers value based optimisation). This example uses genetic algorithm to optimise the flexible operation - this GA is not very advanced and is particularly sensitive to the initial population (seed).
    Modelfile - BarraCUDA.dat
    Schemefile - MTL-FX-EB.dat
    Waterlevel - MTL-Long-2018.ts1
    Turbines - HQPTurbines.dat
    Area - Scheme_Area.out
    Genetic file - Genetics.txt *NEW*
    Energyvalue - SSP_2018.ts1 *NEW*

## EX4_Libaux
Fixed ebb-only operation, based on the Libaux or PEEL/NWDA version of the Mersey barrage, (6.4 - Option 1, from Fig A1 in Aggidis and Benzon 2013 doi:10.1016/j.oceaneng.2013.03.019). Here the energy value is supplied to the model but not the scheme so that we can see how much the energy produced was worth, but the scheme is only considering energy.
    Modelfile - Libaux.dat
    Schemefile - MTL-Libaux.dat
    Waterlevel - MTL-Long-2018.ts1
    Turbines - HQPTurbines.dat
    Area - Scheme_Area.out
    Energyvalue - SSP_2018.ts1
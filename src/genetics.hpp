// genetic algorithm components for 0D modelling of trs...
// genetic algorithm method will be copied fro
#ifndef ZERO_D_GA
#define ZERO_D_GA
// #include <vector>
// https://people.sc.fsu.edu/~jburkardt/cpp_src/simple_ga/simple_ga.html
# include <cmath>
# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <fstream>
// # include <ctime>
# include <string>

#include <vector>
#include <algorithm>
#include <random>

using namespace std;

struct gaConfig {
    int POPSIZE = 50;
    int MAXGENS = 1000;
    int NVARS = 3;
    float PXOVER = 0.8;
    float PMUTATION = 0.15;
};

//  Each GENOTYPE is a member of the population, with
struct genotype
{
    vector<double> gene = vector<double>(3);   //  gene: a string of variables,
    double fitness;       //  fitness: the fitness
    vector<double> upper = vector<double>(3);  //  upper: the variable upper bounds,
    vector<double> lower = vector<double>(3);  //  lower: the variable lower bounds,
    double rfitness;      //  rfitness: the relative fitness,
    double cfitness;      //  cfitness: the cumulative fitness.
};
void Xover(vector<genotype>& population, int one, int two, int& seed, gaConfig cfg);
void selector(vector<genotype>& population, int& seed, gaConfig cfg);
void report(int generation, vector<genotype> population, gaConfig cfg);
double r8_uniform_ab(double a, double b, int& seed);
void mutate(vector<genotype>& population, int& seed, gaConfig cfg);
void keep_the_best(vector<genotype>& population, gaConfig cfg);
vector<genotype> initialize(double hsMax, double hsMin, double heMax, double heMin, double hpMax, double hpMin, int& seed, gaConfig cfg);
int i4_uniform_ab(int a, int b, int& seed);
void elitist(vector<genotype>& population, gaConfig cfg);
void crossover(vector<genotype>& population, int& seed, gaConfig cfg);
void startupGA(const string& fileName, gaConfig& cfg);
void full_report(int generation, double simtime, vector<genotype> population, gaConfig cfg);
vector<double> map_point(vector<double> point, vector<double> min, vector<double> max);
vector<vector<double>> generate_latin_hypercube(int n, int dimensions);

    // startup
    void startupGA(const string &fileName, gaConfig &cfg)
{

    // first = false;
    ifstream inFile(fileName);
    string tmp = "NaN";
    if (inFile.is_open()) {
        getline(inFile, tmp); // reads the header line.
        inFile >> tmp;
        while (tmp != "End" && !inFile.eof()) {
            if (tmp == "End" || tmp == "end" || tmp == "END") {
                break;
            }
            else if (tmp == "POPSIZE" || tmp == "POPSIZE:") {
                inFile >> cfg.POPSIZE;
            }
            else if (tmp == "MAXGENS" || tmp == "MAXGENS:") {
                inFile >> cfg.MAXGENS;
            }
            else if (tmp == "NVARS" || tmp == "NVARS:") {
                inFile >> cfg.NVARS;
            }
            else if (tmp == "PXOVER" || tmp == "PXOVER:") {
                inFile >> cfg.PXOVER;
            }
            else if (tmp == "PMUTATION" || tmp == "PMUTATION:") {
                inFile >> cfg.PMUTATION;
            }
            else {
                cout << "Invalid parameter <" << tmp << "> in [" << fileName << "] ..." << endl;
            }
            inFile >> tmp;
        }
        inFile.close();
        cout << fileName << ": read." << endl;
    } else {
        cout <<"Genetics file [" << fileName 
            << "] not opened. Using default control variables." << endl;
    }
        
    return;
}
//****************************************************************************80
void crossover(vector<genotype>& population, int& seed, gaConfig cfg)
//****************************************************************************80
//  Purpose:
//    CROSSOVER selects two parents for the single point crossover.
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    28 April 2014
//
//  Author:
//    Original version by Dennis Cormier and Sita Raghavan.
//    This C++ version by John Burkardt.
//
//  Local parameters:
//    Local, int FIRST, is a count of the number of members chosen.
//
//  Parameters:
//    Input/output, int &SEED, a seed for the random number generator.
{
    const double a = 0.0;
    const double b = 1.0;
    int mem;
    int one;
    int first = 0;
    double x;

    for (mem = 0; mem < cfg.POPSIZE; ++mem) {
        x = r8_uniform_ab(a, b, seed);

        if (x < cfg.PXOVER) {
            ++first;

            if (first % 2 == 0) {
                Xover(population, one, mem, seed, cfg);
            }
            else {
                one = mem;
            }

        }
    }
    return;
}
//****************************************************************************80
void elitist(vector<genotype>& population, gaConfig cfg)
//****************************************************************************80
//  Purpose:
//    ELITIST stores the best member of the previous generation.
//
//  Discussion:
//    The best member of the previous generation is stored as 
//    the last in the array. If the best member of the current 
//    generation is worse then the best member of the previous 
//    generation, the latter one would replace the worst member 
//    of the current population.
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    29 December 2007
//
//  Author:
//    Original version by Dennis Cormier and Sita Raghavan.
//    This C++ version by John Burkardt.
//
//  Local parameters:
//    Local, double BEST, the best fitness value.
//    Local, double WORST, the worst fitness value.
{
    int i;
    double best;
    int best_mem;
    double worst;
    int worst_mem;

    best = population[0].fitness;
    worst = population[0].fitness;

    for (i = 0; i < cfg.POPSIZE - 1; ++i) {
        if (population[i + 1].fitness < population[i].fitness) {

            if (best <= population[i].fitness) {
                best = population[i].fitness;
                best_mem = i;
            }

            if (population[i + 1].fitness <= worst) {
                worst = population[i + 1].fitness;
                worst_mem = i + 1;
            }

        }
        else {
            if (population[i].fitness <= worst) {
                worst = population[i].fitness;
                worst_mem = i;
            }
            if (best <= population[i + 1].fitness) {
                best = population[i + 1].fitness;
                best_mem = i + 1;
            }
        }
    }
    //  If the best individual from the new population is better than 
    //  the best individual from the previous population, then 
    //  copy the best from the new population; else replace the 
    //  worst individual from the current population with the 
    //  best one from the previous generation                     
    if (population[cfg.POPSIZE].fitness <= best) {
        for (i = 0; i < cfg.NVARS; i++) {
            population[cfg.POPSIZE].gene[i] = population[best_mem].gene[i];
        }
        population[cfg.POPSIZE].fitness = population[best_mem].fitness;
    }
    else {
        for (i = 0; i < cfg.NVARS; i++) {
            population[worst_mem].gene[i] = population[cfg.POPSIZE].gene[i];
        }
        population[worst_mem].fitness = population[cfg.POPSIZE].fitness;
    }
    return;
};
//****************************************************************************80
/*void evaluate(vector<genotype>& population, const double& SimTime, gaConfig cfg)
//****************************************************************************80
//  Purpose:
//    EVALUATE implements the user-defined valuation function
//
//  Discussion:
//    Each time this is changed, the code has to be recompiled.
//    The current function is:  x[1]^2-x[1]*x[2]+x[3]
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    29 December 2007
//
//  Author:
//    Original version by Dennis Cormier and Sita Raghavan.
//    This C++ version by John Burkardt.
//
{
    int member;
    int i;
    vector<double> x = vector<double>(cfg.NVARS + 1);

    // THIS IS WHERE PARALLELISATION WOUULD BE ADDED
    for (member = 0; member < cfg.POPSIZE; member++) {
        for (i = 0; i < cfg.NVARS; i++) {
            x[i + 1] = population[member].gene[i];
        }
        // population[member].fitness = ( x[1] * x[1] ) - ( x[1] * x[2] ) + x[3];
        population[member].fitness = flex( //FLEX FUNCTION TO HERE
            x[1], x[2], x[3], SimTime);
    }
    return;
}*/
//****************************************************************************80
int i4_uniform_ab(int a, int b, int& seed)
//****************************************************************************80
//  Purpose:
//    I4_UNIFORM_AB returns a scaled pseudorandom I4 between A and B.
//
//  Discussion:
//    The pseudorandom number should be uniformly distributed
//    between A and B.
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    02 October 2012
//
//  Author:
//    John Burkardt
//
//  Reference:
//    Paul Bratley, Bennett Fox, Linus Schrage,
//    A Guide to Simulation,
//    Second Edition,
//    Springer, 1987,
//    ISBN: 0387964673,
//    LC: QA76.9.C65.B73.
//
//    Bennett Fox,
//    Algorithm 647:
//    Implementation and Relative Efficiency of Quasirandom
//    Sequence Generators,
//    ACM Transactions on Mathematical Software,
//    Volume 12, Number 4, December 1986, pages 362-376.
//
//    Pierre L'Ecuyer,
//    Random Number Generation,
//    in Handbook of Simulation,
//    edited by Jerry Banks,
//    Wiley, 1998,
//    ISBN: 0471134031,
//    LC: T57.62.H37.
//
//    Peter Lewis, Allen Goodman, James Miller,
//    A Pseudo-Random Number Generator for the System/360,
//    IBM Systems Journal,
//    Volume 8, Number 2, 1969, pages 136-143.
//
//  Parameters:
//    Input, int A, B, the limits of the interval.
//    Input/output, int &SEED, the "seed" value, which should NOT be 0.
//    On output, SEED has been updated.
//    Output, int I4_UNIFORM, a number between A and B.
{
    int c;
    const int i4_huge = 2147483647;
    int k;
    float r;
    int value;

    if (seed == 0)
    {
        cerr << "\n";
        cerr << "I4_UNIFORM_AB - Fatal error!\n";
        cerr << "  Input value of SEED = 0.\n";
        exit(1);
    }
    //  Guarantee A <= B.
    if (b < a) {
        c = a;
        a = b;
        b = c;
    }

    k = seed / 127773;
    seed = 16807 * (seed - k * 127773) - k * 2836;

    if (seed < 0) {
        seed = seed + i4_huge;
    }

    r = (float)(seed) * 4.656612875E-10;
    //  Scale R to lie between A-0.5 and B+0.5.
    r = (1.0 - r) * ((float)a - 0.5)
        + r * ((float)b + 0.5);
    //  Use rounding to convert R to an integer between A and B.
    value = round(r);
    //  Guarantee A <= VALUE <= B.
    if (value < a) {
        value = a;
    }
    if (b < value) {
        value = b;
    }

    return value;
}
//****************************************************************************80
vector<genotype> initialize(
    double hsMax, double hsMin,
    double heMax, double heMin,
    double hpMax, double hpMin,
    int& seed, gaConfig cfg)
    //****************************************************************************80
    //  Purpose:
    //    INITIALIZE initializes the genes within the variables bounds. 
    //
    //  Discussion:
    //    It also initializes (to zero) all fitness values for each
    //    member of the population. It reads upper and lower bounds 
    //    of each variable from the input file `gadata.txt'. It 
    //    randomly generates values between these bounds for each 
    //    gene of each genotype in the population. The format of 
    //    the input file `gadata.txt' is 
    //
    //      var1_lower_bound var1_upper bound
    //      var2_lower_bound var2_upper bound ...
    //
    //  Licensing:
    //    This code is distributed under the GNU LGPL license. 
    //
    //  Modified:
    //    28 April 2014
    //
    //  Author:
    //    Original version by Dennis Cormier and Sita Raghavan.
    //    This C++ version by John Burkardt.
    //
    //  Parameters:
    //    Input, string FILENAME, the name of the input file.
    //    Input/output, int &SEED, a seed for the random number generator.
{
    int i;
    ifstream input;
    int j;
    vector<double> lbound{ hsMin, heMin, hpMin };
    vector<double> ubound{ hsMax, heMax, hpMax };
    vector<genotype> population(cfg.POPSIZE + 1);

    vector<vector<double>> hypercube = generate_latin_hypercube(cfg.POPSIZE, cfg.NVARS);

    for (j = 0; j < cfg.POPSIZE; j++) {
        population[j].fitness = 0;
        population[j].rfitness = 0;
        population[j].cfitness = 0;

        for (i = 0; i < cfg.NVARS; i++) {
            population[j].lower[i] = lbound[i];
            population[j].upper[i] = ubound[i];

            population[j].gene = map_point(hypercube[i], lbound, ubound);
        }
    }

        // //   input.open(filename.c_str());
        // //   if (!input)
        // //   {
        // //     cerr << "\n";
        // //     cerr << "INITIALIZE - Fatal error!\n";
        // //     cerr << "  Cannot open the input file!\n";
        // //     exit ( 1 );
        // //   }
        // //
        // //  Initialize variables within the bounds
        // //
        // for (i = 0; i < cfg.NVARS; i++)
        // {
        // //    input >> lbound >> ubound;

        //     for (j = 0; j < cfg.POPSIZE; j++)
        //     {
        //         population[j].fitness = 0;
        //         population[j].rfitness = 0;
        //         population[j].cfitness = 0;
        //         population[j].lower[i] = lbound[i];
        //         population[j].upper[i] = ubound[i];
        //         population[j].gene[i] = r8_uniform_ab(lbound[i], ubound[i], seed);
        //     }
        // }
        input.close();
    return population;
}
//****************************************************************************80
void keep_the_best(vector<genotype>& population, gaConfig cfg)
//****************************************************************************80
//  Purpose:
//    KEEP_THE_BEST keeps track of the best member of the population. 
//
//  Discussion:
//    Note that the last entry in the array Population holds a 
//    copy of the best individual.
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    29 December 2007
//
//  Author:
//    Original version by Dennis Cormier and Sita Raghavan.
//    This C++ version by John Burkardt.
//
//  Local parameters:
//    Local, int CUR_BEST, the index of the best individual.
//
{
    int cur_best;
    int mem;
    int i;

    cur_best = 0;

    for (mem = 0; mem < cfg.POPSIZE; mem++) {
        if (population[cfg.POPSIZE].fitness < population[mem].fitness) {
            cur_best = mem;
            population[cfg.POPSIZE].fitness = population[mem].fitness;
        }
    }
    //  Once the best member in the population is found, copy the genes.
    for (i = 0; i < cfg.NVARS; i++) {
        population[cfg.POPSIZE].gene[i] = population[cur_best].gene[i];
    }
    return;
}
//****************************************************************************80
void mutate(vector<genotype>& population, int& seed, gaConfig cfg)
//****************************************************************************80
//  Purpose:
//    MUTATE performs a random uniform mutation. 
//
//  Discussion:
//    A variable selected for mutation is replaced by a random value 
//    between the lower and upper bounds of this variable.
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    28 April 2014
//
//  Author:
//    Original version by Dennis Cormier and Sita Raghavan.
//    This C++ version by John Burkardt.
//
//  Parameters:
//    Input/output, int &SEED, a seed for the random number generator.
{
    const double a = 0.0;
    const double b = 1.0;
    int i;
    int j;
    double lbound;
    double ubound;
    double x;

    for (i = 0; i < cfg.POPSIZE; i++) {
        for (j = 0; j < cfg.NVARS; j++) {
            x = r8_uniform_ab(a, b, seed);
            if (x < cfg.PMUTATION)
            {
                lbound = population[i].lower[j];
                ubound = population[i].upper[j];
                population[i].gene[j] = r8_uniform_ab(lbound, ubound, seed);
            }
        }
    }
    return;
}
//****************************************************************************80
double r8_uniform_ab(double a, double b, int& seed)
//****************************************************************************80
//  Purpose:
//    R8_UNIFORM_AB returns a scaled pseudorandom R8.
//
//  Discussion:
//    The pseudorandom number should be uniformly distributed
//    between A and B.
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    09 April 2012
//
//  Author:
//    John Burkardt
//
//  Parameters:
//    Input, double A, B, the limits of the interval.
//    Input/output, int &SEED, the "seed" value, which should NOT be 0.
//    On output, SEED has been updated.
//    Output, double R8_UNIFORM_AB, a number strictly between A and B.
{
    int i4_huge = 2147483647;
    int k;
    double value;

    if (seed == 0) {
        cerr << "\n";
        cerr << "R8_UNIFORM_AB - Fatal error!\n";
        cerr << "  Input value of SEED = 0.\n";
        exit(1);
    }

    k = seed / 127773;
    seed = 16807 * (seed - k * 127773) - k * 2836;

    if (seed < 0) {
        seed = seed + i4_huge;
    }

    value = (double)(seed) * 4.656612875E-10;
    value = a + (b - a) * value;
    return value;
}
void full_report(int generation, double simtime, vector<genotype> population, gaConfig cfg) {
//****************************************************************************80
//  Purpose:
//    full_report writyes the whole population to a csv file. 
//    DO NOT OVERUSE THIS AS IT WILL BE A BIG SOURSE OF LAG I THINK
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    28/02/2022
//
//  Author:
//    Nick Hanousek
//
    ofstream gfile;
    string genfilename = "t-"+to_string(simtime)+"_Gen-" + to_string(generation) + ".csv";
    gfile.open(genfilename);
    gfile << "K,HSTART,HEND,HPUMP,FITNESS" << endl;
    int i;
    for (i = 0; i <= cfg.POPSIZE; i++) {
        gfile << i << "," 
            << population[i].gene[0] << ","
            << population[i].gene[1] << ","
            << population[i].gene[2] << ","
            << population[i].fitness << endl;
    }
    gfile.close();
    return;
}
//****************************************************************************80
void report(int generation, vector<genotype> population, gaConfig cfg)
//****************************************************************************80
//  Purpose:
//    REPORT reports progress of the simulation. 
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    29 December 2007
//
//  Author:
//    Original version by Dennis Cormier and Sita Raghavan.
//    This C++ version by John Burkardt.
//
//  Local parameters:
//    Local, double avg, the average population fitness.
//    Local, best_val, the best population fitness.
//    Local, double square_sum, square of sum for std calc.
//    Local, double stddev, standard deviation of population fitness.
//    Local, double sum, the total population fitness.
//    Local, double sum_square, sum of squares for std calc.
{
    double avg;
    double best_val;
    int i;
    double square_sum;
    double stddev;
    double sum;
    double sum_square;

    if (generation == 0) {
        std::cout << "  Generation       Best            Average       Standard     Best        Best      Best\n";
        std::cout << "  number           value           fitness       deviation    Start       End       Pump\n";
    }

    sum = 0.0;
    sum_square = 0.0;

    for (i = 0; i < cfg.POPSIZE; i++)
    {
        sum = sum + population[i].fitness;
        sum_square = sum_square + population[i].fitness * population[i].fitness;
    }

    avg = sum / (double)cfg.POPSIZE;
    square_sum = avg * avg * cfg.POPSIZE;
    stddev = sqrt((sum_square - square_sum) / (cfg.POPSIZE - 1));
    best_val = population[cfg.POPSIZE].fitness;

    std::cout << "  " << setw(8) << generation
        << "  " << setw(14) << best_val
        << "  " << setw(14) << avg
        << "  " << setw(14) << stddev 
        << "  " << setw(9) << population[cfg.POPSIZE].gene[0]
        << "  " << setw(9) << population[cfg.POPSIZE].gene[1] 
        << "  " << setw(9) << population[cfg.POPSIZE].gene[2] << "\n";

    return;
}
//****************************************************************************80
void selector(vector<genotype>& population, int& seed, gaConfig cfg)
//****************************************************************************80
//  Purpose:
//    SELECTOR is the selection function.
//
//  Discussion:
//    Standard proportional selection for maximization problems incorporating 
//    the elitist model.  This makes sure that the best member always survives.
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    28 April 2014
//
//  Author:
//    Original version by Dennis Cormier and Sita Raghavan.
//    This C++ version by John Burkardt.
//
//  Parameters:
//    Input/output, int &SEED, a seed for the random number generator.
{
    const double a = 0.0;
    const double b = 1.0;
    int i;
    int j;
    int mem;
    double p;
    double sum;
    vector<genotype> newpopulation = population;
    //  Find the total fitness of the population.
    sum = 0.0;
    for (mem = 0; mem < cfg.POPSIZE; mem++)
    {
        sum = sum + population[mem].fitness;
    }
    //  Calculate the relative fitness of each member.
    for (mem = 0; mem < cfg.POPSIZE; mem++) {
        population[mem].rfitness = population[mem].fitness / sum;
    }
    //  Calculate the cumulative fitness.
    population[0].cfitness = population[0].rfitness;
    for (mem = 1; mem < cfg.POPSIZE; mem++) {
        population[mem].cfitness = population[mem - 1].cfitness +
            population[mem].rfitness;
    }
    // 
    //  Select survivors using cumulative fitness. 
    for (i = 0; i < cfg.POPSIZE; i++)
    {
        p = r8_uniform_ab(a, b, seed);
        if (p < population[0].cfitness)
        {
            newpopulation[i] = population[0];
        }
        else
        {
            for (j = 0; j < cfg.POPSIZE; j++)
            {
                if (population[j].cfitness <= p && p < population[j + 1].cfitness)
                {
                    newpopulation[i] = population[j + 1];
                }
            }
        }
    }
    //  Overwrite the old population with the new one.
    for (i = 0; i < cfg.POPSIZE; i++)
    {
        population[i] = newpopulation[i];
    }
    return;
}
//****************************************************************************80
//void timestamp()
//****************************************************************************80
//  Purpose:
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//    May 31 2001 09:45:54 AM
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    04 October 2003
//
//  Author:
//    John Burkardt
/*{
# define TIME_SIZE 40

    static char time_buffer[TIME_SIZE];
    struct tm tm;
    struct tm timeinfo;
    time_t now;

    now = time(NULL);
    tm = localtime_s(&timeinfo,&now);
    strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm);
    cout << time_buffer << "\n";

    return;
# undef TIME_SIZE
}*/
//****************************************************************************80
void Xover(vector<genotype>& population, int one, int two, int& seed, gaConfig cfg)
//****************************************************************************80
//  Purpose:
//    XOVER performs crossover of the two selected parents. 
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    28 April 2014
//
//  Author:
//    Original version by Dennis Cormier and Sita Raghavan.
//    This C++ version by John Burkardt.
//
//  Local parameters:
//    Local, int point, the crossover point.
//
//  Parameters:
//    Input, int ONE, TWO, the indices of the two parents.
//    Input/output, int &SEED, a seed for the random number generator.
{
    int i;
    int point;
    double t;
    //  Select the crossover point.
    point = i4_uniform_ab(0, cfg.NVARS - 1, seed);
    //  Swap genes in positions 0 through POINT-1.
    for (i = 0; i < point; i++) {
        t = population[one].gene[i];
        population[one].gene[i] = population[two].gene[i];
        population[two].gene[i] = t;
    }
    return;
}

// Function to generate a random N-dimensional Latin hypercube
vector<vector<double>> generate_latin_hypercube(int n, int dimensions)
{
    // Initialize the vector of vectors
    vector<vector<double>> hypercube(n, vector<double>(dimensions));

    // Create vectors with the index of each row and column
    vector<int> row_indices(n);
    vector<vector<int>> col_indices(dimensions, vector<int>(n));
    for (int i = 0; i < n; i++)
    {
        row_indices[i] = i;
        for (int j = 0; j < dimensions; j++)
        {
            col_indices[j][i] = j * n + i;
        }
    }

    // Shuffle the row and column indices for each dimension
    random_device rd;
    mt19937 gen(rd());
    for (int j = 0; j < dimensions; j++)
    {
        shuffle(col_indices[j].begin(), col_indices[j].end(), gen);
    }

    // Fill in the hypercube with the shuffled values
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < dimensions; j++)
        {
            hypercube[i][j] = (col_indices[j][i] + 0.5) / (n * dimensions);
        }
    }

    return hypercube;
}

// Function to map a point from a unit hypercube to a specified range in each dimension
vector<double> map_point(vector<double> point, vector<double> min, vector<double> max)
{
    int dimensions = point.size();
    vector<double> mapped_point(dimensions);
    for (int j = 0; j < dimensions; j++)
    {
        mapped_point[j] = min[j] + point[j] * (max[j] - min[j]);
    }
    return mapped_point;
}

//****************************************************************************
/*double ga_optimise(
    double& hs, double& he, 
    double& hpe, const double& SimTime,
    const double& hsMax, const double& hsMin,
    const double& heMax, const double& heMin,
    const double& hpMax, const double& hpMin
)
//****************************************************************************
//  Purpose:
//    MAIN supervises the genetic algorithm.
//
//  Discussion:
//    Each generation involves selecting the best 
//    members, performing crossover & mutation and then 
//    evaluating the resulting population, until the terminating 
//    condition is satisfied   
//
//    This is a simple genetic algorithm implementation where the 
//    evaluation function takes positive values only and the 
//    fitness of an individual is the same as the value of the 
//    objective function.  
//
//  Licensing:
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//    25/02/2022
//
//  Author:
//
//    Original version by Dennis Cormier and Sita Raghavan.
//    This C++ version by John Burkardt.
//    TRS modifications by Nick Hanousek
//
//  Reference:
//    Zbigniew Michalewicz,
//    Genetic Algorithms + Data Structures = Evolution Programs,
//    Third Edition,
//    Springer, 1996,
//    ISBN: 3-540-60676-9,
//    LC: QA76.618.M53.
//
//  Parameters:
//    MAXGENS is the maximum number of generations.
//    NVARS is the number of problem variables.
//    PMUTATION is the probability of mutation.
//    POPSIZE is the population size. 
//    PXOVER is the probability of crossover.                          
//
{
    //  Change any of these parameters to match your needs 
    //bool first = true;
    //int POPSIZE = 50;
    //int MAXGENS = 1000;
    //int NVARS = 3;
    //float PXOVER = 0.8;
    //float PMUTATION = 0.15;
    startupGA("Genetics.dat");

    // OLD MAIN BEGINS HERE
    //   string filename = "input.txt";
    int generation;
    int i;
    int seed;

    //timestamp();
    std::cout << "\n";
    std::cout << "SIMPLE_GA: C++ version\n";
    std::cout << "  A simple example of a genetic algorithm.\n";

    if (NVARS < 2) {
        std::cout << "\n";
        std::cout << "  The crossover modification will not be available,\n";
        std::cout << "  since it requires 2 <= NVARS.\n";
    }

    seed = 123456789;

    // no longer reads the values from an external file
    initialize(hsMax, hsMin, heMax, heMin, hpMax, hpMin, seed);

    evaluate();
    keep_the_best();

    for (generation = 0; generation < MAXGENS; generation++) {
        selector(seed);
        crossover(seed);
        mutate(seed);
        report(generation);
        evaluate();
        elitist();
    }

    std::cout << "\n";
    std::cout << "  Best member after " << MAXGENS << " generations:\n";
    std::cout << "\n";

    //   for ( i = 0; i < NVARS; i++ ) {
    //     cout << "  var(" << i << ") = " << population[POPSIZE].gene[i] << "\n";
    //   }
    hs = population[POPSIZE].gene[1];
    he = population[POPSIZE].gene[2];
    hpe = population[POPSIZE].gene[3];
    double optimum = population[POPSIZE].fitness;

    std::cout << "HS: " << hs
        << ", HE: " << he
        << ", HPE:" << hpe
        << " -> " << optimum << endl;
    //  std::cout << "\n";
    //  cout << "  Best fitness = " << population[POPSIZE].fitness << "\n";
    // 
    //  Terminate.
    //
    std::cout << "\n";
    std::cout << "SIMPLE_GA:\n";
    std::cout << "  Normal end of execution.\n";
    std::cout << "\n";
    //timestamp();

    return optimum;
};*/

#endif

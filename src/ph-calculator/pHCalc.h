/*
version : v3.1.0-alpha

MIT License

Copyright (c) 2019-2020 Lee Kyung-ha <i_am@nulleekh.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//Import libraries required for calculation.

#ifndef PH_CALCULATOR_PHCALC_H
#define PH_CALCULATOR_PHCALC_H

#define MAX_NUMBER_LENGTH 255
#define NUMBER_OF_DB_SOLUTE 255
#define NUMBER_OF_DB_DATA 3
#define LENGTH_OF_DB_DATA 32
#define MAX_SOLUTION_NUMBER 255
#define PH_CALCULATOR_STARTPOINT -3
#define PH_CALCULATOR_ENDPOINT 18
#define PH_CALCULATOR_RESOLUTION 0.000001
#define PH_CALCULATOR_INITIALINTERVAL 1
#define RECIPE_FINDER_RESOLUTION 0.0001
#define IONIZATION_CONSTANT 0.00000000000001
//Specify settings used for calculations and operations

#define Type1HFactor(db, index) strtol(db[index][1], NULL, 10)
#define Type2NumProton(db, index) strtol(db[index][2], NULL, 10)
#define Type2AcidConstant(db, index, time) strtod(db[index][2+time], NULL)
#define Type2OHFactor(db, index) strtol(db[index][1], NULL, 10)
#define Type2Solubility(db, index) strtod(db[index][2], NULL)
//Declare macros approaching the database.

long SpecifySolute(char db[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                   long nSolute, char *name);
//Return index of Type 1 or Type 2 solution.

double Type1(char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                           double concentration_type_1, long index_type_1, double supposed_h, double volume);
double Type1Mono(char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                           double concentration_type_1, long index_type_1, double supposed_h, double volume);
double Type1Di(char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                         double sPolypProtic, long index_type_1, double supposed_h, double volume);
double Type1Tri(char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                          double concentration_type_1, long index_type_1, double supposed_h, double volume);
//Calculate the concentration of Type 1 solution.

double Type2(char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                               long iPoorlySoluble, double supposed_h);
double Type2Mono(char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                      long iPoorlySoluble, double supposed_h);
double Type2Di(char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                    long iPoorlySoluble, double supposed_h);
double Type2Tri(char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                     long iPoorlySoluble, double supposed_h);
//Calculate the concentration of Type 2 powder when solution is supersaturation.

double InitialH(char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                    long num_acid, long num_base, long num_type_1, double *concentration_acid, double *concentration_base, double *concentration_type_1,
                    long *index_type_1, double volume);
//Calculate initial concentration considering strong acid, strong base and Type 1 solution.
double Error(char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                      char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                      double initial_h, long num_type_1, long nPoorlySoluble, double *concentration_type_1, long *index_type_1,
                      long *iPoorlySoluble, double supposed_h, double volume);
//Calculate error considering every elements which influence with result.
double PH(char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                   char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                   long num_acid, long num_base, long num_type_1, long nPoorlySoluble, double *concentration_acid, double *concentration_base,
                   double *concentration_type_1, long *index_type_1, double *sPoorlySoluble, long *iPoorlySoluble, double volume,
                   double search_start, double search_end, double resolution, double interval);
//Calculate pH value of mixed solution.

#endif //PH_CALCULATOR_PHCALC_H

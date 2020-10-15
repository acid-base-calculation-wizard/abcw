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

#include "pHCalc.h"

long SpecifySolute(char db[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                   long num_db_solution, char *name) {
    printf("INFO : SpecifySolute function started\n");
    long i;
    //Declare the variables needed to operate the function.

    for (i = 0; i < num_db_solution; i++) {
        if (strncmp(&db[i][0][0], &name[0], LENGTH_OF_DB_DATA) == 0) {
            printf("INFO : SpecifySolute function ended\n");
            return i;
        }
    }   //Search the database and return the material's index number.

    printf("ERROR : Invalid solution name\n");
    printf("INFO : SpecifySolute function ended\n");
    return -1;
}

double Type1(char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                           double concentration_type_1, long index_type_1, double supposed_h, double volume) {
    double result;
    //Declare the variables needed to operate the function.

    switch (Type2NumProton(type_1_database, index_type_1)) {
        case 1 :
            result = Type1Mono(type_1_database, concentration_type_1, index_type_1, supposed_h, volume);
            break;

        case 2 :
            result = Type1Di(type_1_database, concentration_type_1, index_type_1, supposed_h, volume);
            break;

        case 3 :
            result = Type1Tri(type_1_database, concentration_type_1, index_type_1, supposed_h, volume);
            break;

        default :
            printf("ERROR : Error occured while calculating the %ld polyprotic solution\n", index_type_1);
            result = 0;
    }   //Classify type 1 solution and call up the corresponding calculation function.

    return result;  //Return the result.
}

double Type1Mono(char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                           double concentration_type_1, long index_type_1, double supposed_h, double volume) {
    double cen, x1, k1, result;
    //Declare the variables needed to operate the function.

    cen = concentration_type_1 / volume;
    x1 = supposed_h;
    k1 = pow(10, -Type2AcidConstant(type_1_database, index_type_1, 1));
    result = (cen * k1) / (x1 + k1);
    //Calculate the formula for mono-protic acid.

    return result;  //Return the result.
}

double Type1Di(char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                         double concentration_type_1, long index_type_1, double supposed_h, double volume) {
    double cen, x1, x2, k1, k2, result;
    //Declare the variables needed to operate the function.

    cen = concentration_type_1 / volume;
    x1 = supposed_h;
    x2 = pow(supposed_h, 2);
    k1 = pow(10, -Type2AcidConstant(type_1_database, index_type_1, 1));
    k2 = pow(10, -Type2AcidConstant(type_1_database, index_type_1, 2));
    result = (cen * ((k1 * x1) + (2 * k1 * k2))) / (x2 + (k1 * x1) + (k1 * k2));
    //Calculate the formula for di-protic acid.

    return result;  //Return the result.
}

double Type1Tri(char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                          double concentration_type_1, long index_type_1, double supposed_h, double volume) {
    double cen, x1, x2, x3, k1, k2, k3, result;
    //Declare the variables needed to operate the function.

    cen = concentration_type_1 / volume;
    x1 = supposed_h;
    x2 = pow(supposed_h, 2);
    x3 = pow(supposed_h, 3);
    k1 = pow(10, -Type2AcidConstant(type_1_database, index_type_1, 1));
    k2 = pow(10, -Type2AcidConstant(type_1_database, index_type_1, 2));
    k3 = pow(10, -Type2AcidConstant(type_1_database, index_type_1, 3));
    result = (cen * ((k1 * x2) + (2 * k1 * k2 * x1) + (3 * k1 * k2 * k3))) /
             (x3 + (k1 * x2) + (k1 * k2 * x1) + (k1 * k2 * k3));
    //Calculate the formula for tri-protic acid.

    return result;  //Return the result.
}

double Type2(char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                               long index_type_2, double supposed_h) {
    double result;
    //Declare the variables needed to operate the function.

    switch (Type2OHFactor(type_2_database, index_type_2)) {
        case 1 :
            result = Type2Mono(type_2_database, index_type_2, supposed_h);
            break;

        case 2 :
            result = Type2Di(type_2_database, index_type_2, supposed_h);
            break;

        case 3 :
            result = Type2Tri(type_2_database, index_type_2, supposed_h);
            break;

        default :
            printf("ERROR : Error occured while calculating the %ld hydroxide solution\n", index_type_2);
            result = 0;
    }   //Classify Type 2 powder and call up the corresponding calculation function.

    return result;  //Return the result.

}

double Type2Mono(char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                      long index_type_2, double supposed_h) {
    double result;
    //Declare the variables needed to operate the function.

    result = (pow(10, -Type2Solubility(type_2_database, index_type_2)) / pow(IONIZATION_CONSTANT, 1)) * pow(supposed_h, 1);
    //Calculate the formula for mono-hydroxide base.

    return result;  //Return the result.
}

double Type2Di(char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                    long index_type_2, double supposed_h) {
    double result;
    //Declare the variables needed to operate the function.

    result = ((2*pow(10, -Type2Solubility(type_2_database, index_type_2))) / pow(IONIZATION_CONSTANT, 2)) * pow(supposed_h, 2);
    //Calculate the formula for di-hydroxide base.

    return result;  //Return the result.
}

double Type2Tri(char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                     long index_type_2, double supposed_h) {
    double result;
    //Declare the variables needed to operate the function.

    result = ((3*pow(10, -Type2Solubility(type_2_database, index_type_2))) / pow(IONIZATION_CONSTANT, 3)) * pow(supposed_h, 3);
    //Calculate the formula for tri-hydroxide base.

    return result;  //Return the result.
}

double InitialH(char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
                    long num_acid, long num_base, long num_type_1, double *concentration_acid, double *concentration_base, double *concentration_type_1,
                    long *index_type_1, double volume) {
    printf("INFO : InitialH function started\n");
    long i;
    double h = 0;
    //Declare the variables needed to operate the function.

    for (i = 0; i < num_acid; i++) h += concentration_acid[i];
    for (i = 0; i < num_base; i++) h -= concentration_base[i];
    for (i = 0; i < num_type_1; i++) h -= concentration_type_1[i] * Type1HFactor(type_1_database, index_type_1[i]);
    h = h / volume;
    //Calculate the initial hydrogen ion concentration.

    printf("INFO : InitialH function ended\n");
    return h;
    //Return the result.
}

double Error(
        char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        double initial_h, long num_type_1, long num_type_2, double *concentration_type_1, long *index_type_1,
        long *index_type_2, double supposed_h, double volume) {
    long i;
    double result = initial_h + (IONIZATION_CONSTANT / supposed_h);
    //Declare the variables needed to operate the function.

    for (i = 0; i < num_type_1; i++)
        result += Type1(type_1_database, concentration_type_1[i], index_type_1[i], supposed_h, volume);
    for (i = 0; i < num_type_2; i++)
        result -= Type2(type_2_database, index_type_2[i], supposed_h);
    //Calculate the result using the value received.

    return fabs(supposed_h - result); //Return the error between value received and the result.
}

double
PH(char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
            char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
            long num_acid, long num_base, long num_type_1, long num_type_2, double *concentration_acid, double *concentration_base,
            double *concentration_type_1, long *index_type_1, double *mol_type_2, long *index_type_2, double volume,
            double search_start, double search_end, double resolution, double interval) {
    printf("\nINFO : PH function started\n");
    int flag = 0;
    long i, j;
    double pH = search_start - interval, temp_double, lowest, answer = -1, initial_h;
    //Declare the variables needed to operate the function.

    initial_h = InitialH(type_1_database, num_acid, num_base, num_type_1, concentration_acid, concentration_base, concentration_type_1, index_type_1, volume);

    lowest = Error(type_2_database, type_1_database, initial_h, num_type_1, num_type_2,
                            concentration_type_1, index_type_1, index_type_2, pow(10, -search_start), volume);

    while (1) {
        pH += interval;

        temp_double = Error(type_2_database, type_1_database, initial_h, num_type_1, num_type_2,
                             concentration_type_1, index_type_1, index_type_2, pow(10, -pH), volume);

        if (temp_double == -1) return -1;
        else if (temp_double < lowest) {
            lowest = temp_double;
            answer = pH;
        }

        if (pH >= search_end) break;
    }   //Locate the point where the error is minimized within the search scope.

    printf("INFO : start-point = %.3lf | end-point = %.3lf | interval = %.6lf | pH = %.3lf | error = %.9lf\n", search_start,
           search_end, interval, answer, lowest);

    if (interval > resolution) {
        return PH(type_2_database, type_1_database, num_acid, num_base, num_type_1, num_type_2, concentration_acid,
                           concentration_base, concentration_type_1, index_type_1, mol_type_2, index_type_2, volume, answer - interval,
                           answer + interval, resolution, interval * 0.1);
    }   //If the search interval is greater than the allowable error, narrow down the search interval to re-discover both sides of the point.
    else {
        printf("\nINFO : PH function ended\n\n");

        for (i = 0; i < num_type_2; i++) {
            switch (Type2OHFactor(type_2_database, index_type_2[i])) {
                case 1 :
                    if (mol_type_2[i] <
                        Type2(type_2_database, index_type_2[i], pow(10, -answer))*volume) {
                        flag = 1;

                        concentration_base[num_base] += mol_type_2[i];
                        num_base++;

                        for (j = i; j < num_type_2; j++) {
                            mol_type_2[j] = mol_type_2[j + 1];
                            index_type_2[j] = index_type_2[j + 1];
                        }
                        num_type_2--;

                        i--;
                    }
                    break;

                case 2 :
                    if (mol_type_2[i] * 2 <
                        Type2(type_2_database, index_type_2[i], pow(10, -answer))*volume) {
                        flag = 1;

                        concentration_base[num_base] += mol_type_2[i] * 2;
                        num_base++;

                        for (j = i; j < num_type_2; j++) {
                            mol_type_2[j] = mol_type_2[j + 1];
                            index_type_2[j] = index_type_2[j + 1];
                        }
                        num_type_2--;

                        i--;
                    }
                    break;

                case 3 :
                    if (mol_type_2[i] * 3 <
                        Type2(type_2_database, index_type_2[i], pow(10, -answer))*volume) {
                        flag = 1;

                        concentration_base[num_base] += mol_type_2[i] * 3;
                        num_base++;

                        for (j = i; j < num_type_2; j++) {
                            mol_type_2[j] = mol_type_2[j + 1];
                            index_type_2[j] = index_type_2[j + 1];
                        }
                        num_type_2--;

                        i--;
                    }
                    break;

                default :
                    printf("ERROR : Error occured while checking the %ld hydroxide solution\n", index_type_2[i]);
            }   //Calculate Type 2 powder as strong base when solution is not supersaturated.
        }

        if (flag == 1) {
            return PH(type_2_database, type_1_database, num_acid, num_base, num_type_1, num_type_2,
                               concentration_acid, concentration_base, concentration_type_1, index_type_1, mol_type_2, index_type_2, volume,
                               PH_CALCULATOR_STARTPOINT, PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION,
                               PH_CALCULATOR_INITIALINTERVAL);
        }   //Calculate again when solution is supersaturated.
        else {
            return answer;
        }   //Return the answer after it passed every valuation basis.
    }
}

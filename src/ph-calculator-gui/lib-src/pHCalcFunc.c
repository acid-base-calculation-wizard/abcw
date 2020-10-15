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
#include "pHCalc.h"
#include "pHCalcFunc.h"
//Import libraries required for operation.

#define RECIPE_FINDER_STARTPOINT 100000000
#define RECIPE_FINDER_ACCURACY 0.000000000001
//Declare constants for recipe finder.

long SpecifyType1Solute(char *name, long name_length) {
    printf("INFO : SpecifyType1Solute function started\n");
    long i, j, k, num_type_1_database;
    char tmp = 0x00, type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA] = {0,};
    //Declare the variables needed to operate the function.

    printf("INFO - Loading type_1 database.\n");

    FILE *type_1_database_file = fopen("type_1.pcd", "r");

    if (type_1_database_file == NULL) {
        printf("ERROR - Failed to open type_1 database.\n");
        return -1;
    }

    for (i = 0; i < NUMBER_OF_DB_SOLUTE; i++) {
        for (j = 0; j < NUMBER_OF_DB_DATA; j++) {
            for (k = 0; k < LENGTH_OF_DB_DATA; k++) {
                tmp = fgetc(type_1_database_file);
                if (tmp == 0x7C) break;
                else if (tmp == 0x20) break;
                else if (tmp == EOF) break;
                else type_1_database[i][j][k] = tmp;
            }
            if (tmp == 0x20) break;
            else if (tmp == EOF) break;
        }
        if (tmp == EOF) break;
    }
    num_type_1_database = i;
    fclose(type_1_database_file);

    printf("INFO - type_1 database loaded successfully.\n");
    //Get the database needed to operate the program.

    for (i = 0; i < num_type_1_database; i++) {
        if (strncmp(&type_1_database[i][0][0], &name[0], name_length) == 0) {
            printf("INFO : SpecifyType1Solute function ended\n");
            return i;
        }
    }   //Search the database and return the material's unique number.

    printf("ERROR : Invalid solution name\n");
    printf("INFO : SpecifyType1Solute function ended\n");
    return -1;
}

long SpecifyType2Solute(char *name, long name_length) {
    printf("INFO : SpecifyType2Solute function started\n");
    long i, j, k, num_type_2_database;
    char tmp = 0x00, type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA] = {0,};
    //Declare the variables needed to operate the function.

    printf("INFO - Loading type_2 database.\n");

    FILE *type_2_database_file = fopen("type_2.pcd", "r");

    if (type_2_database_file == NULL) {
        printf("ERROR - Failed to open type_2 database.\n");
        return -1;
    }

    for (i = 0; i < NUMBER_OF_DB_SOLUTE; i++) {
        for (j = 0; j < NUMBER_OF_DB_DATA; j++) {
            for (k = 0; k < LENGTH_OF_DB_DATA; k++) {
                tmp = fgetc(type_2_database_file);
                if (tmp == 0x7C) break;
                else if (tmp == 0x20) break;
                else if (tmp == EOF) break;
                else type_2_database[i][j][k] = tmp;
            }
            if (tmp == 0x20) break;
            else if (tmp == EOF) break;
        }
        if (tmp == EOF) break;
    }
    num_type_2_database = i;
    fclose(type_2_database_file);

    printf("INFO - type_2 database loaded successfully.\n");
    //Get the database needed to operate the program.

    for (i = 0; i < num_type_2_database; i++) {
        if (strncmp(&type_2_database[i][0][0], &name[0], name_length) == 0) {
            printf("INFO : SpecifyType2Solute function ended\n");
            return i;
        }
    }   //Search the database and return the material's unique number.

    printf("ERROR : Invalid solution name\n");
    printf("INFO : SpecifyType2Solute function ended\n");
    return -1;
}

double PhCalculator(long num_acid, long num_base, long num_type_1, long num_type_2, double *concentration_acid,
                    double *concentration_base, double *concentration_type_1, double *mol_type_2, long *index_type_1,
                    long *index_type_2, double volume_all) {
    printf("INFO - PhCalculator function started.\n");

    long i, j, k, num_type_1_database, num_type_2_database;
    char temp_char = 0x00, type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA] = {
            0,}, type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA] = {0,};
    double result;
    //Declare the variables needed to operate the function.

    printf("INFO - Loading type_1 database.\n");

    FILE *type_1_database_file = fopen("type_1.pcd", "r");

    if (type_1_database_file == NULL) {
        printf("ERROR - Failed to open type_1 database.\n");
        return -1;
    }

    for (i = 0; i < NUMBER_OF_DB_SOLUTE; i++) {
        for (j = 0; j < NUMBER_OF_DB_DATA; j++) {
            for (k = 0; k < LENGTH_OF_DB_DATA; k++) {
                temp_char = (char) fgetc(type_1_database_file);
                if (temp_char == 0x7C) break;
                else if (temp_char == 0x20) break;
                else if (temp_char == EOF) break;
                else type_1_database[i][j][k] = temp_char;
            }
            if (temp_char == 0x20) break;
            else if (temp_char == EOF) break;
        }
        if (temp_char == EOF) break;
    }
    num_type_1_database = i;
    fclose(type_1_database_file);
    printf("INFO - type_1 database loaded successfully.\n");


    printf("INFO - Loading type_2 database.\n");

    FILE *type_2_database_file = fopen("type_2.pcd", "r");

    if (type_2_database_file == NULL) {
        printf("ERROR - Failed to open type_2 database.\n");
        return -1;
    }

    for (i = 0; i < NUMBER_OF_DB_SOLUTE; i++) {
        for (j = 0; j < NUMBER_OF_DB_DATA; j++) {
            for (k = 0; k < LENGTH_OF_DB_DATA; k++) {
                temp_char = (char) fgetc(type_2_database_file);
                if (temp_char == 0x7C) break;
                else if (temp_char == 0x20) break;
                else if (temp_char == EOF) break;
                else type_2_database[i][j][k] = temp_char;
            }
            if (temp_char == 0x20) break;
            else if (temp_char == EOF) break;
        }
        if (temp_char == EOF) break;
    }
    num_type_2_database = i;
    fclose(type_2_database_file);

    printf("INFO - type_2 database loaded successfully.\n");
    //Get the database needed to operate the program.


    result = PH(type_2_database, type_1_database, num_acid, num_base, num_type_1, num_type_2, concentration_acid,
                concentration_base, concentration_type_1, index_type_1, mol_type_2, index_type_2, volume_all,
                PH_CALCULATOR_STARTPOINT,
                PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION, 1);
    //Calculate pH value.

    printf("INFO - PhCalculator function ended.\n");

    return result;
    //Return pH value.
}

int GraphGenerator(long num_acid, long num_base, long num_type_1, long num_type_2, double *concentration_acid,
                   double *concentration_base, double *concentration_type_1, double *mol_type_2, long *index_type_1,
                   long *index_type_2, double volume_all, long titrant_type, long time_titrant, double volume_titrant,
                   double concentration_titrant, long index_titrant) {
    printf("INFO - GraphGenerator function started.\n");

    long i, j, k, num_type_1_database, num_type_2_database, num_type_1_copy, num_type_2_copy, index_type_1_copy[MAX_SOLUTION_NUMBER], index_type_2_copy[MAX_SOLUTION_NUMBER];
    double concentration_type_1_copy[MAX_SOLUTION_NUMBER], mol_type_2_copy[MAX_SOLUTION_NUMBER];
    char temp_char = 0x00, type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA] = {
            0,}, type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA] = {0,}, result[6];
    //Declare the variables needed to operate the function.

    printf("INFO - Loading type_1 database.\n");

    FILE *type_1_database_file = fopen("type_1.pcd", "r");

    if (type_1_database_file == NULL) {
        printf("ERROR - Failed to open type_1 database.\n");
        return -1;
    }

    for (i = 0; i < NUMBER_OF_DB_SOLUTE; i++) {
        for (j = 0; j < NUMBER_OF_DB_DATA; j++) {
            for (k = 0; k < LENGTH_OF_DB_DATA; k++) {
                temp_char = (char) fgetc(type_1_database_file);
                if (temp_char == 0x7C) break;
                else if (temp_char == 0x20) break;
                else if (temp_char == EOF) break;
                else type_1_database[i][j][k] = temp_char;
            }
            if (temp_char == 0x20) break;
            else if (temp_char == EOF) break;
        }
        if (temp_char == EOF) break;
    }
    num_type_1_database = i;
    fclose(type_1_database_file);
    printf("INFO - type_1 database loaded successfully.\n");


    printf("INFO - Loading type_2 database.\n");

    FILE *type_2_database_file = fopen("type_2.pcd", "r");

    if (type_2_database_file == NULL) {
        printf("ERROR - Failed to open type_2 database.\n");
        return -1;
    }

    for (i = 0; i < NUMBER_OF_DB_SOLUTE; i++) {
        for (j = 0; j < NUMBER_OF_DB_DATA; j++) {
            for (k = 0; k < LENGTH_OF_DB_DATA; k++) {
                temp_char = (char) fgetc(type_2_database_file);
                if (temp_char == 0x7C) break;
                else if (temp_char == 0x20) break;
                else if (temp_char == EOF) break;
                else type_2_database[i][j][k] = temp_char;
            }
            if (temp_char == 0x20) break;
            else if (temp_char == EOF) break;
        }
        if (temp_char == EOF) break;
    }
    num_type_2_database = i;
    fclose(type_2_database_file);

    printf("INFO - type_2 database loaded successfully.\n");
    //Get the database needed to operate the program.


    FILE *resultGraphFile;
    resultGraphFile = fopen("result.pcd", "w");

    if (resultGraphFile == NULL) {
        printf("ERROR - Failed to open result database.\n");
        printf("INFO - GraphGenerator function ended.\n");
        return -1;
    }

    i = 0;
    switch (titrant_type) {
        case 0 :
            num_acid++;

            while (1) {
                result[0] = 0x00;

                concentration_acid[num_acid - 1] = concentration_titrant * volume_titrant * (double) i;

                gcvt(PH(type_2_database, type_1_database, num_acid, num_base, num_type_1, num_type_2,
                                 concentration_acid, concentration_base, concentration_type_1, index_type_1, mol_type_2, index_type_2,
                                 volume_all + volume_titrant * (double)i, PH_CALCULATOR_STARTPOINT, PH_CALCULATOR_ENDPOINT,
                                 PH_CALCULATOR_RESOLUTION, 1), 6, result);

                fputs(result, resultGraphFile);
                fputs("\n", resultGraphFile);

                i++;

                if (i > time_titrant) break;
            }
            //Perform titration experimental simulation as instructed and store results in result.pcd.

            break;

        case 1 :
            num_base++;

            while (1) {
                result[0] = 0x00;

                concentration_base[num_base - 1] = concentration_titrant * volume_titrant * (double) i;

                gcvt(PH(type_2_database, type_1_database, num_acid, num_base, num_type_1, num_type_2,
                                 concentration_acid, concentration_base, concentration_type_1, index_type_1, mol_type_2, index_type_2,
                                 volume_all + volume_titrant * (double)i, PH_CALCULATOR_STARTPOINT, PH_CALCULATOR_ENDPOINT,
                                 PH_CALCULATOR_RESOLUTION, 1), 6, result);

                fputs(result, resultGraphFile);
                fputs("\n", resultGraphFile);

                i++;

                if (i > time_titrant) break;
            }
            //Perform titration experimental simulation as instructed and store results in result.pcd.

            break;

        case 2 :
            num_type_1++;

            index_type_1[num_type_1 - 1] = index_titrant;
            if (index_type_1[num_type_1 - 1] == -1) {
                printf("INFO - GraphGenerator function ended.\n");
                return -1;
            }

            while (1) {
                result[0] = 0x00;

                concentration_type_1[num_type_1 - 1] = concentration_titrant * volume_titrant * (double) i;

                gcvt(PH(type_2_database, type_1_database, num_acid, num_base, num_type_1, num_type_2,
                                 concentration_acid, concentration_base, concentration_type_1, index_type_1, mol_type_2, index_type_2,
                                 volume_all + volume_titrant * (double)i, PH_CALCULATOR_STARTPOINT, PH_CALCULATOR_ENDPOINT,
                                 PH_CALCULATOR_RESOLUTION, 1), 6, result);

                fputs(result, resultGraphFile);
                fputs("\n", resultGraphFile);

                i++;

                if (i > time_titrant) break;
            }
            //Perform titration experimental simulation as instructed and store results in result.pcd.

            break;

        case 3 :
            num_type_2++;

            index_type_2[num_type_2 - 1] = index_titrant;
            if (index_type_2[num_type_2 - 1] == -1) {
                printf("INFO - GraphGenerator function ended.\n");
                return -1;
            }

            while (1) {
                result[0] = 0x00;

                mol_type_2[num_type_2 - 1] = concentration_titrant * (double) i;

                gcvt(PH(type_2_database, type_1_database, num_acid, num_base, num_type_1, num_type_2,
                                 concentration_acid, concentration_base, concentration_type_1, index_type_1, mol_type_2, index_type_2, volume_all,
                                 PH_CALCULATOR_STARTPOINT, PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION, 1), 6,
                     result);

                fputs(result, resultGraphFile);
                fputs("\n", resultGraphFile);

                i++;

                if (i > time_titrant) break;
            }
            //Perform titration experimental simulation as instructed and store results in result.pcd.

            break;

        default :
            printf("ERROR - Invalid solution type.\n");
            printf("INFO - GraphGenerator function ended.\n");
            return -1;
            //Return -1 when titrant type is invalid.
    }
    fclose(resultGraphFile);

    printf("\n\n");

    printf("INFO - Graph has been generated.\n\n");

    printf("INFO - GraphGenerator function ended.\n");
    return 0;
    //Return 0 after operation.
}

double RecipeFinder(long num_acid, long num_base, long num_type_1, long num_type_2, double *concentration_acid,
                 double *concentration_base, double *concentration_type_1, double *mol_type_2, long *index_type_1,
                 long *index_type_2, double volume_all, long titrant_type, double concentration_titrant,
                 long index_titrant, double target_ph) {
    printf("INFO - RecipeFinder function started.\n");

    long i, j, k, num_type_1_database, num_type_2_database;
    double titrand_ph, titrant_ph, accuracy, error, answer, lowest_error, suitable_answer, suitable_volume, volume_titrant;
    char temp_char = 0x00, type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA] = {
            0,}, type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA] = {0,};
    //Declare the variables needed to operate the function.

    printf("INFO - Loading type_1 database.\n");

    FILE *type_1_database_file = fopen("type_1.pcd", "r");

    if (type_1_database_file == NULL) {
        printf("ERROR - Failed to open type_1 database.\n");
        return -1;
    }

    for (i = 0; i < NUMBER_OF_DB_SOLUTE; i++) {
        for (j = 0; j < NUMBER_OF_DB_DATA; j++) {
            for (k = 0; k < LENGTH_OF_DB_DATA; k++) {
                temp_char = (char) fgetc(type_1_database_file);
                if (temp_char == 0x7C) break;
                else if (temp_char == 0x20) break;
                else if (temp_char == EOF) break;
                else type_1_database[i][j][k] = temp_char;
            }
            if (temp_char == 0x20) break;
            else if (temp_char == EOF) break;
        }
        if (temp_char == EOF) break;
    }
    num_type_1_database = i;
    fclose(type_1_database_file);
    printf("INFO - type_1 database loaded successfully.\n");


    printf("INFO - Loading type_2 database.\n");

    FILE *type_2_database_file = fopen("type_2.pcd", "r");

    if (type_2_database_file == NULL) {
        printf("ERROR - Failed to open type_2 database.\n");
        return -1;
    }

    for (i = 0; i < NUMBER_OF_DB_SOLUTE; i++) {
        for (j = 0; j < NUMBER_OF_DB_DATA; j++) {
            for (k = 0; k < LENGTH_OF_DB_DATA; k++) {
                temp_char = (char) fgetc(type_2_database_file);
                if (temp_char == 0x7C) break;
                else if (temp_char == 0x20) break;
                else if (temp_char == EOF) break;
                else type_2_database[i][j][k] = temp_char;
            }
            if (temp_char == 0x20) break;
            else if (temp_char == EOF) break;
        }
        if (temp_char == EOF) break;
    }
    num_type_2_database = i;
    fclose(type_2_database_file);

    printf("INFO - type_2 database loaded successfully.\n");
    //Get the database needed to operate the program.


    titrand_ph = PH(type_2_database, type_1_database, num_acid, num_base, num_type_1, num_type_2,
                             concentration_acid, concentration_base, concentration_type_1, index_type_1, mol_type_2, index_type_2, volume_all,
                             PH_CALCULATOR_STARTPOINT, PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION,
                             PH_CALCULATOR_INITIALINTERVAL);

    i = 0;
    suitable_volume = 0;
    suitable_answer = 0;
    switch (titrant_type) {
        case 0 :
            num_acid++;

            titrant_ph = PH(type_2_database, type_1_database, 1, 0, 0, 0, &concentration_titrant, 0, 0, 0, 0, 0,
                                     1, PH_CALCULATOR_STARTPOINT, PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION,
                                     PH_CALCULATOR_INITIALINTERVAL);
            //calculate titrandt's pH.

            if (titrand_ph < target_ph) {
                if (titrant_ph < target_ph) {
                    printf("ERROR - Invalid titrant entered.\n");
                    printf("INFO - RecipeFinder function ended.\n");
                    return -1;
                }
            } else if (titrand_ph > target_ph) {
                if (titrant_ph > target_ph) {
                    printf("ERROR - Invalid titrant entered.\n");
                    printf("INFO - RecipeFinder function ended.\n");
                    return -1;
                }
            }
            //Check the validity of execution.

            volume_titrant = 0;
            for (i = RECIPE_FINDER_STARTPOINT; i > 1; i /= 10) {
                accuracy = RECIPE_FINDER_ACCURACY * (double) i;
                lowest_error = 1024;
                while (1) {
                    concentration_acid[num_acid - 1] = concentration_titrant * volume_titrant;
                    answer = PH(type_2_database, type_1_database, num_acid, num_base, num_type_1,
                                num_type_2, concentration_acid, concentration_base, concentration_type_1, index_type_1,
                                mol_type_2,
                                index_type_2, volume_all + volume_titrant, PH_CALCULATOR_STARTPOINT,
                                PH_CALCULATOR_ENDPOINT,
                                PH_CALCULATOR_RESOLUTION, PH_CALCULATOR_INITIALINTERVAL);

                    error = fabs(answer - target_ph);

                    if (error <= lowest_error) {
                        lowest_error = error;
                        suitable_answer = answer;
                        suitable_volume = volume_titrant;
                        volume_titrant += accuracy;
                    } else {
                        printf("RESULT - result: %.6lf | pH: %.3lf | error: %.3lf\n\n", suitable_volume,
                               suitable_answer, lowest_error);
                        //Print out the founded recipe.

                        volume_titrant -= accuracy * 2;

                        break;
                    }
                }
            }
            //Perform titration experimental simulation execute according to the prescribed rule until reach target pH value.

            break;

        case 1 :
            num_base++;

            titrant_ph = PH(type_2_database, type_1_database, 0, 1, 0, 0, 0, &concentration_titrant, 0, 0, 0, 0,
                                     1, PH_CALCULATOR_STARTPOINT, PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION,
                                     PH_CALCULATOR_INITIALINTERVAL);
            //calculate titrandt's pH.

            if (titrand_ph < target_ph) {
                if (titrant_ph < target_ph) {
                    printf("ERROR - Invalid titrant entered.\n");
                    printf("INFO - RecipeFinder function ended.\n");
                    return -1;
                }
            } else if (titrand_ph > target_ph) {
                if (titrant_ph > target_ph) {
                    printf("ERROR - Invalid titrant entered.\n");
                    printf("INFO - RecipeFinder function ended.\n");
                    return -1;
                }
            }
            //Check the validity of execution.

            volume_titrant = 0;
            for (i = RECIPE_FINDER_STARTPOINT; i > 1; i /= 10) {
                accuracy = RECIPE_FINDER_ACCURACY * (double) i;
                lowest_error = 1024;
                while (1) {
                    concentration_base[num_base - 1] = concentration_titrant * volume_titrant;
                    answer = PH(type_2_database, type_1_database, num_acid, num_base, num_type_1,
                                num_type_2, concentration_acid, concentration_base, concentration_type_1, index_type_1,
                                mol_type_2,
                                index_type_2, volume_all + volume_titrant, PH_CALCULATOR_STARTPOINT,
                                PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION,
                                PH_CALCULATOR_INITIALINTERVAL);

                    error = fabs(answer - target_ph);

                    if (error <= lowest_error) {
                        lowest_error = error;
                        suitable_answer = answer;
                        suitable_volume = volume_titrant;
                        volume_titrant += accuracy;
                    } else {
                        printf("RESULT - result: %.6lf | pH: %.3lf | error: %.3lf\n\n", suitable_volume,
                               suitable_answer, lowest_error);
                        //Print out the founded recipe.

                        volume_titrant -= accuracy * 2;

                        break;
                    }
                }
            }
            //Perform titration experimental simulation execute according to the prescribed rule until reach target pH value.

            break;

        case 2 :
            num_type_1++;

            index_type_1[num_type_1 - 1] = index_titrant;
            if (index_type_1[i] == -1) {
                printf("INFO - RecipeFinder function ended.\n");
                return -1;
            }

            titrant_ph = PH(type_2_database, type_1_database, 0, 0, 1, 0, 0, 0, &concentration_titrant,
                                     &index_type_1[num_type_1 - 1], 0, 0, 1, PH_CALCULATOR_STARTPOINT,
                                     PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION, PH_CALCULATOR_INITIALINTERVAL);
            //calculate titrandt's pH.

            if (titrand_ph < target_ph) {
                if (titrant_ph < target_ph) {
                    printf("ERROR - Invalid titrant entered.\n");
                    printf("INFO - RecipeFinder function ended.\n");
                    return -1;
                }
            } else if (titrand_ph > target_ph) {
                if (titrant_ph > target_ph) {
                    printf("ERROR - Invalid titrant entered.\n");
                    printf("INFO - RecipeFinder function ended.\n");
                    return -1;
                }
            }
            //Check the validity of execution.

            volume_titrant = 0;
            for (i = RECIPE_FINDER_STARTPOINT; i > 1; i /= 10) {
                accuracy = RECIPE_FINDER_ACCURACY * (double) i;
                lowest_error = 1024;
                while (1) {
                    concentration_type_1[num_type_1 - 1] = concentration_titrant * volume_titrant;
                    answer = PH(type_2_database, type_1_database, num_acid, num_base, num_type_1,
                                num_type_2, concentration_acid, concentration_base, concentration_type_1, index_type_1,
                                mol_type_2,
                                index_type_2, volume_all, PH_CALCULATOR_STARTPOINT,
                                PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION,
                                PH_CALCULATOR_INITIALINTERVAL);

                    error = fabs(answer - target_ph);

                    if (error <= lowest_error) {
                        lowest_error = error;
                        suitable_answer = answer;
                        suitable_volume = volume_titrant;
                        volume_titrant += accuracy;
                        if (lowest_error < RECIPE_FINDER_RESOLUTION) break;
                    } else {
                        printf("RESULT - result: %.6lf | pH: %.3lf | error: %.3lf\n\n", suitable_volume,
                               suitable_answer, lowest_error);
                        //Print out the founded recipe.

                        volume_titrant -= accuracy * 2;

                        break;
                    }
                }
            }
            //Perform titration experimental simulation execute according to the prescribed rule until reach target pH value.

            break;

        case 3 :
            num_type_2++;

            index_type_2[num_type_2 - 1] = index_titrant;
            if (index_type_2[i] == -1) {
                printf("INFO - RecipeFinder function ended.\n");
                return -1;
            }
            //Get data of the Type 2 titrant.

            concentration_titrant = 100000000;
            //Set big number for calculating.

            titrant_ph = PH(type_2_database, type_1_database, 0, 0, 0, 1, 0, 0, 0, 0, &concentration_titrant,
                                     &index_type_2[num_type_2 - 1], 1, PH_CALCULATOR_STARTPOINT,
                                     PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION, PH_CALCULATOR_INITIALINTERVAL);
            //calculate titrandt's pH in saturated water.

            if (titrand_ph < target_ph) {
                if (titrant_ph < target_ph) {
                    printf("ERROR - Invalid titrant entered.\n");
                    printf("INFO - RecipeFinder function ended.\n");
                    return -1;
                }
            } else if (titrand_ph > target_ph) {
                if (titrant_ph > target_ph) {
                    printf("ERROR - Invalid titrant entered.\n");
                    printf("INFO - RecipeFinder function ended.\n");
                    return -1;
                }
            }
            //Check the validity of execution.

            volume_titrant = 0;
            for (i = RECIPE_FINDER_STARTPOINT; i > 1; i /= 10) {
                accuracy = RECIPE_FINDER_ACCURACY * (double) i;
                lowest_error = 1024;
                while (1) {
                    mol_type_2[num_type_2 - 1] = volume_titrant;
                    answer = PH(type_2_database, type_1_database, num_acid, num_base, num_type_1,
                                      num_type_2, concentration_acid, concentration_base, concentration_type_1, index_type_1, mol_type_2,
                                      index_type_2, volume_all, PH_CALCULATOR_STARTPOINT,
                                      PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION,
                                      PH_CALCULATOR_INITIALINTERVAL);

                    error = fabs(answer - target_ph);

                    if (fabs(lowest_error-error) < 0.001 || error <= lowest_error) {
                        lowest_error = error;
                        suitable_answer = answer;
                        suitable_volume = volume_titrant;
                        volume_titrant += accuracy;
                    } else {
                        printf("RESULT - result: %.21lf | pH: %.3lf | error: %.3lf\n\n", suitable_volume, suitable_answer, lowest_error);
                        //Print out the founded recipe.

                        volume_titrant = suitable_volume - accuracy * 2;

                        break;
                    }
                }
            }
            //Perform titration experimental simulation execute according to the prescribed rule until reach target pH value.

            break;

        default :
            printf("ERROR - Invalid solution type.\n");
            printf("INFO - RecipeFinder function ended.\n");
            return -1;
            //Return -1 when titrant type is invalid.
    }

    printf("\n\n");

    printf("INFO - RecipeFinder function ended.\n");
    return suitable_volume;
    //Return the result after operation.
}
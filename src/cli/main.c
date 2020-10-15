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
//Import libraries required for operation.

#define RECIPE_FINDER_STARTPOINT 100000000
#define RECIPE_FINDER_ACCURACY 0.000000000001
//Declare constants for recipe finder.

int PhCalculator(
        char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        long num_type_1_database, long num_type_2_database);

int GraphGenerator(
        char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        long num_type_1_database, long num_type_2_database);

int RecipeFinder(
        char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        long num_type_1_database, long num_type_2_database);
//Declare the function prototypes

int main() {
    printf("INFO - Program started\n");

    long i, j, k, num_type_1_database, num_type_2_database;
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
                temp_char = (char)fgetc(type_1_database_file);
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
                temp_char = (char)fgetc(type_2_database_file);
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

    printf("INPUT - Select calculation type you want [pH-calculator : 0 | graph-generator : 1 | recipe-finder : 2] : ");
    scanf("%c", &temp_char);
    printf("\n");
    fflush(stdin);
    switch (temp_char) {
        case '0' :
            return PhCalculator(type_2_database, type_1_database, num_type_1_database, num_type_2_database);

        case '1' :
            return GraphGenerator(type_2_database, type_1_database, num_type_1_database,
                                  num_type_2_database);

        case '2' :
            return RecipeFinder(type_2_database, type_1_database, num_type_1_database, num_type_2_database);

        default :
            printf("ERROR - Invalid calculation type.\n");
            return -1;
    }   //Specify the function to execute, and call the function.
}

int PhCalculator(
        char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        long num_type_1_database, long num_type_2_database) {
    printf("INFO - PhCalculator function started.\n");

    long i, num_acid, num_base, num_type_1, index_type_1[MAX_SOLUTION_NUMBER], num_type_2, index_type_2[MAX_SOLUTION_NUMBER];
    double volume_all, volume, cen, concentration_acid[MAX_SOLUTION_NUMBER], concentration_base[MAX_SOLUTION_NUMBER], concentration_type_1[MAX_SOLUTION_NUMBER], mol_type_2[MAX_SOLUTION_NUMBER];
    char temp_input[MAX_NUMBER_LENGTH], name[LENGTH_OF_DB_DATA] = {0,};
    //Declare the variables needed to operate the function.

    printf("INPUT - Number of strong acid solutions : ");
    scanf("%s", temp_input);
    num_acid = strtol(temp_input, NULL, 10);
    printf("\n");
    fflush(stdin);
    if (0 > num_acid || num_acid > 255) {
        printf("ERROR - Invalid num_acid value.\n");
        printf("INFO - PhCalculator function ended.\n");
        return -1;
    }   //Get the number of strong acid solutions.
    printf("INPUT - Number of strong base solutions : ");
    scanf("%s", temp_input);
    num_base = strtol(temp_input, NULL, 10);
    printf("\n");
    fflush(stdin);
    if (0 > num_base || num_base > 255) {
        printf("ERROR - Invalid num_base value.\n");
        printf("INFO - PhCalculator function ended.\n");
        return -1;
    }   //Get the number of strong base solutions.
    printf("INPUT - Number of the Type 1 solutions : ");
    scanf("%s", temp_input);
    num_type_1 = strtol(temp_input, NULL, 10);
    printf("\n");
    fflush(stdin);
    if (0 > num_type_1 || num_type_1 > 255) {
        printf("ERROR - Invalid num_type_1 value.\n");
        printf("INFO - PhCalculator function ended.\n");
        return -1;
    }   //Get the number of Type 1 solutions.
    printf("INPUT - Number of the Type 2 powders : ");
    scanf("%s", temp_input);
    num_type_2 = strtol(temp_input, NULL, 10);
    printf("\n");
    fflush(stdin);
    if (0 > num_type_2 || num_type_2 > 255) {
        printf("ERROR - Invalid num_type_2 value.\n");
        printf("INFO - PhCalculator function ended.\n");
        return -1;
    }   //Get the number of Type 2 solutions.

    printf("\n\n");

    printf("INPUT - volume of pure water (L) : ");
    scanf("%s", temp_input);
    volume_all = strtod(temp_input, NULL);
    printf("\n");
    fflush(stdin);
    //Get volume of pure water.

    printf("\n\n");

    for (i = 0; i < num_acid; i++) {
        printf("INPUT - volume of strong acid solution (L) : ");
        scanf("%s", temp_input);
        volume = strtod(temp_input, NULL);
        volume_all = volume_all + volume;
        printf("\n");
        fflush(stdin);
        printf("INPUT - Concentration of the solution (mol/L) : ");
        scanf("%s", temp_input);
        cen = strtod(temp_input, NULL);
        concentration_acid[i] = cen * volume;
        printf("\n\n");
        fflush(stdin);
    }   //Get data of strong acid solutions.
    for (i = 0; i < num_base; i++) {
        printf("INPUT - volume of strong base solution (L) : ");
        scanf("%s", temp_input);
        volume = strtod(temp_input, NULL);
        volume_all = volume_all + volume;
        printf("\n");
        fflush(stdin);
        printf("INPUT - Concentration of the solution (mol/L) : ");
        scanf("%s", temp_input);
        cen = strtod(temp_input, NULL);
        concentration_base[i] = cen * volume;
        printf("\n\n");
        fflush(stdin);
    }   //Get data of strong base solutions.
    for (i = 0; i < num_type_1; i++) {
        printf("INPUT - Name of Type 1 solution : ");
        scanf("%s", name);
        index_type_1[i] = SpecifySolute(type_1_database, num_type_1_database, name);
        printf("\n");
        fflush(stdin);
        if (index_type_1[i] == -1) {
            printf("INFO - PhCalculator function ended.\n");
            return -1;
        }
        printf("INPUT - volume of the solution (L) : ");
        scanf("%s", temp_input);
        volume = strtod(temp_input, NULL);
        volume_all = volume_all + volume;
        printf("\n");
        fflush(stdin);
        printf("INPUT - Concentration of the solution (mol/L) : ");
        scanf("%s", temp_input);
        cen = strtod(temp_input, NULL);
        concentration_type_1[i] = volume * cen;
        printf("\n\n");
        fflush(stdin);
    }   //Get data of the Type 1 solutions.
    for (i = 0; i < num_type_2; i++) {
        printf("INPUT - Name of Type 2 powder : ");
        scanf("%s", name);
        index_type_2[i] = SpecifySolute(type_2_database, num_type_2_database, name);
        printf("\n");
        fflush(stdin);
        if (index_type_2[i] == -1) {
            printf("INFO - PhCalculator function ended.\n");
            return -1;
        }
        printf("INPUT - Mol of the powder (mol) : ");
        scanf("%s", temp_input);
        mol_type_2[i] = strtod(temp_input, NULL);
        printf("\n\n");
        fflush(stdin);
    }   //Get data of the Type 2 solutions.

    printf("\n");

    printf("%.3lf\n\n",
           PH(type_2_database, type_1_database, num_acid, num_base, num_type_1, num_type_2, concentration_acid,
                       concentration_base, concentration_type_1, index_type_1, mol_type_2, index_type_2, volume_all, PH_CALCULATOR_STARTPOINT,
                       PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION, 1));
    //Calculate and print out the pH value.

    printf("INFO - PhCalculator function ended.\n");
    return 0;
}

int GraphGenerator(
        char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        long num_type_1_database, long num_type_2_database) {
    printf("INFO - GraphGenerator function started.\n");

    long i, num_acid, num_base, num_type_1, index_type_1[MAX_SOLUTION_NUMBER], num_type_2, index_type_2[MAX_SOLUTION_NUMBER], time_titrant;
    double volume_all, volume, cen, concentration_acid[MAX_SOLUTION_NUMBER], concentration_base[MAX_SOLUTION_NUMBER], concentration_type_1[MAX_SOLUTION_NUMBER], mol_type_2[MAX_SOLUTION_NUMBER], volume_titrant, concentration_titrant;
    char result[6], temp_input[MAX_NUMBER_LENGTH], name[LENGTH_OF_DB_DATA] = {0,};
    //Declare the variables needed to operate the function.

    printf("INPUT - Number of strong acid titrands : ");
    scanf("%s", temp_input);
    num_acid = strtol(temp_input, NULL, 10);
    printf("\n");
    fflush(stdin);
    if (0 > num_acid || num_acid > 255) {
        printf("ERROR - Invalid num_acid value.\n");
        printf("INFO - GraphGenerator function ended.\n");
        return -1;
    }   //Get the number of strong acid titrands.
    printf("INPUT - Number of strong base titrands : ");
    scanf("%s", temp_input);
    num_base = strtol(temp_input, NULL, 10);
    printf("\n");
    fflush(stdin);
    if (0 > num_base || num_base > 255) {
        printf("ERROR - Invalid num_base value.\n");
        printf("INFO - GraphGenerator function ended.\n");
        return -1;
    }   //Get the number of strong base titrands.
    printf("INPUT - Number of the Type 1 titrands : ");
    scanf("%s", temp_input);
    num_type_1 = strtol(temp_input, NULL, 10);
    printf("\n");
    fflush(stdin);
    if (0 > num_type_1 || num_type_1 > 255) {
        printf("ERROR - Invalid Type 1 value.\n");
        printf("INFO - GraphGenerator function ended.\n");
        return -1;
    }   //Get the number of num_type_1 solutions.
    printf("INPUT - Number of the Type 2 powders : ");
    scanf("%s", temp_input);
    num_type_2 = strtol(temp_input, NULL, 10);
    printf("\n");
    fflush(stdin);
    if (0 > num_type_2 || num_type_2 > 255) {
        printf("ERROR - Invalid num_type_2 value.\n");
        printf("INFO - GraphGenerator function ended.\n");
        return -1;
    }   //Get the number of Type 2 solutions.

    printf("\n\n");

    printf("INPUT - volume of pure water (L) : ");
    scanf("%s", temp_input);
    volume_all = strtod(temp_input, NULL);
    printf("\n");
    fflush(stdin);
    //Get volume of pure water.

    printf("\n\n");

    for (i = 0; i < num_acid; i++) {
        printf("INPUT - volume of strong acid titrand (L) : ");
        scanf("%s", temp_input);
        volume = strtod(temp_input, NULL);
        volume_all = volume_all + volume;
        printf("\n");
        fflush(stdin);
        printf("INPUT - Concentration of the titrand (mol/L) : ");
        scanf("%s", temp_input);
        cen = strtod(temp_input, NULL);
        concentration_acid[i] = cen * volume;
        printf("\n\n");
        fflush(stdin);
    }   //Get data of strong acid titrands.
    for (i = 0; i < num_base; i++) {
        printf("INPUT - volume of strong base titrand (L) : ");
        scanf("%s", temp_input);
        volume = strtod(temp_input, NULL);
        volume_all = volume_all + volume;
        printf("\n");
        fflush(stdin);
        printf("INPUT - Concentration of the titrand (mol/L) : ");
        scanf("%s", temp_input);
        cen = strtod(temp_input, NULL);
        concentration_base[i] = cen * volume;
        printf("\n\n");
        fflush(stdin);
    }   //Get data of strong base titrands.
    for (i = 0; i < num_type_1; i++) {
        printf("INPUT - Name of Type 1 titrand : ");
        scanf("%s", name);
        index_type_1[i] = SpecifySolute(type_1_database, num_type_1_database, name);
        printf("\n");
        fflush(stdin);
        if (index_type_1[i] == -1) {
            printf("INFO - GraphGenerator function ended.\n");
            return -1;
        }
        printf("INPUT - volume of the titrand (L) : ");
        scanf("%s", temp_input);
        volume = strtod(temp_input, NULL);
        volume_all = volume_all + volume;
        printf("\n");
        fflush(stdin);
        printf("INPUT - Concentration of the titrand (mol/L) : ");
        scanf("%s", temp_input);
        cen = strtod(temp_input, NULL);
        concentration_type_1[i] = volume * cen;
        printf("\n\n");
        fflush(stdin);
    }   //Get data of the Type 1 solutions.
    for (i = 0; i < num_type_2; i++) {
        printf("INPUT - Name of Type 2 powder : ");
        scanf("%s", name);
        index_type_2[i] = SpecifySolute(type_2_database, num_type_2_database, name);
        printf("\n");
        fflush(stdin);
        if (index_type_2[i] == -1) {
            printf("INFO - GraphGenerator function ended.\n");
            return -1;
        }
        printf("INPUT - Mol of the powder (mol) : ");
        scanf("%s", temp_input);
        mol_type_2[i] = strtod(temp_input, NULL);
        printf("\n\n");
        fflush(stdin);
    }   //Get data of the Type 2 solutions.

    printf("\n");

    printf("INPUT - Type of titrant [strong acid : 0 | strong base : 1 | Type 1 : 2 | Type 2 : 3] : ");
    scanf("%s", temp_input);
    printf("\n");
    fflush(stdin);
    //Get type of titrant.

    FILE *resultGraphFile;
    resultGraphFile = fopen("result.pcd", "w");

    if (resultGraphFile == NULL) {
        printf("ERROR - Failed to open result database.\n");
        printf("INFO - GraphGenerator function ended.\n");
        return -1;
    }

    i = 0;
    switch (temp_input[0]) {
        case '0' :
            num_acid++;

            printf("INPUT - Number of times to add titrant (times) : ");
            scanf("%s", temp_input);
            time_titrant = strtol(temp_input, NULL, 10);
            printf("\n");
            fflush(stdin);
            printf("INPUT - volume of the titrant per time (L) : ");
            scanf("%s", temp_input);
            volume_titrant = strtod(temp_input, NULL);
            printf("\n");
            fflush(stdin);
            printf("INPUT - Concentration of the titrant (mol/L) : ");
            scanf("%s", temp_input);
            concentration_titrant = strtod(temp_input, NULL);
            printf("\n");
            fflush(stdin);
            //Get data of strong acid titrant.

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

        case '1' :
            num_base++;

            printf("INPUT - Number of times to add titrant (times) : ");
            scanf("%s", temp_input);
            time_titrant = strtol(temp_input, NULL, 10);
            printf("\n");
            fflush(stdin);
            printf("INPUT - volume of the titrant per time (L) : ");
            scanf("%s", temp_input);
            volume_titrant = strtod(temp_input, NULL);
            printf("\n");
            fflush(stdin);
            printf("INPUT - Concentration of the titrant (mol/L) : ");
            scanf("%s", temp_input);
            concentration_titrant = strtod(temp_input, NULL);
            printf("\n");
            fflush(stdin);
            //Get data of strong base titrands.

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

        case '2' :
            num_type_1++;

            printf("INPUT - Name of the Type 1 titrant : ");
            scanf("%s", name);
            index_type_1[num_type_1 - 1] = SpecifySolute(type_1_database, num_type_1_database, name);
            printf("\n");
            fflush(stdin);
            if (index_type_1[num_type_1 - 1] == -1) {
                printf("INFO - GraphGenerator function ended.\n");
                return -1;
            }
            printf("INPUT - Number of times to add Type 1 titrant (times) : ");
            scanf("%s", temp_input);
            time_titrant = strtol(temp_input, NULL, 10);
            printf("\n");
            fflush(stdin);
            printf("INPUT - volume of the Type 1 titrant per time (L) : ");
            scanf("%s", temp_input);
            volume_titrant = strtod(temp_input, NULL);
            printf("\n");
            fflush(stdin);
            printf("INPUT - Concentration of the Type 1 titrant (mol/L) : ");
            scanf("%s", temp_input);
            concentration_titrant = strtod(temp_input, NULL);
            printf("\n");
            fflush(stdin);
            //Get data of the Type 1 titrant.

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

        case '3' :
            num_type_2++;

            printf("INPUT - Name of the Type 2 titrant : ");
            scanf("%s", name);
            index_type_2[num_type_2 - 1] = SpecifySolute(type_2_database,
                                                               num_type_2_database, name);
            printf("\n");
            fflush(stdin);
            if (index_type_2[num_type_2 - 1] == -1) {
                printf("INFO - GraphGenerator function ended.\n");
                return -1;
            }
            printf("INPUT - Number of times to add Type 2 titrant (times) : ");
            scanf("%s", temp_input);
            time_titrant = strtol(temp_input, NULL, 10);
            printf("\n");
            fflush(stdin);
            printf("INPUT - Mol of the Type 2 titrant per time (mol) : ");
            scanf("%s", temp_input);
            concentration_titrant = strtod(temp_input, NULL);
            printf("\n");
            fflush(stdin);
            //Get data of the Type 2 titrant.

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

int RecipeFinder(
        char type_2_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        char type_1_database[NUMBER_OF_DB_SOLUTE][NUMBER_OF_DB_DATA][LENGTH_OF_DB_DATA],
        long num_type_1_database, long num_type_2_database) {
    printf("INFO - RecipeFinder function started.\n");

    long i, num_acid, num_base, num_type_1, index_type_1[MAX_SOLUTION_NUMBER], num_type_2, index_type_2[MAX_SOLUTION_NUMBER];
    double volume_all, volume, cen, concentration_acid[MAX_SOLUTION_NUMBER], concentration_base[MAX_SOLUTION_NUMBER], concentration_type_1[MAX_SOLUTION_NUMBER], mol_type_2[MAX_SOLUTION_NUMBER], concentration_titrant, titrand_ph, titrant_ph, target_ph, accuracy, error, answer, lowest_error, suitable_answer, suitable_volume, volume_titrant;
    char temp_input[MAX_NUMBER_LENGTH], name[LENGTH_OF_DB_DATA] = {0,};
    //Declare the variables needed to operate the function.

    printf("INPUT - Number of strong acid titrands : ");
    scanf("%s", temp_input);
    num_acid = strtol(temp_input, NULL, 10);
    printf("\n");
    fflush(stdin);
    if (0 > num_acid || num_acid > 255) {
        printf("ERROR - Invalid num_acid value.\n");
        printf("INFO - RecipeFinder function ended.\n");
        return -1;
    }   //Get the number of strong acid titrands.
    printf("INPUT - Number of strong base titrands : ");
    scanf("%s", temp_input);
    num_base = strtol(temp_input, NULL, 10);
    printf("\n");
    fflush(stdin);
    if (0 > num_base || num_base > 255) {
        printf("ERROR - Invalid num_base value.\n");
        printf("INFO - RecipeFinder function ended.\n");
        return -1;
    }   //Get the number of strong base titrands.
    printf("INPUT - Number of the Type 1 titrands : ");
    scanf("%s", temp_input);
    num_type_1 = strtol(temp_input, NULL, 10);
    printf("\n");
    fflush(stdin);
    if (0 > num_type_1 || num_type_1 > 255) {
        printf("ERROR - Invalid num_type_1 value.\n");
        printf("INFO - GraphGenerator function ended.\n");
        return -1;
    }   //Get the number of num_type_1 solutions.
    printf("INPUT - Number of the Type 2 powders : ");
    scanf("%s", temp_input);
    num_type_2 = strtol(temp_input, NULL, 10);
    printf("\n");
    fflush(stdin);
    if (0 > num_type_2 || num_type_2 > 255) {
        printf("ERROR - Invalid num_type_2 value.\n");
        printf("INFO - GraphGenerator function ended.\n");
        return -1;
    }   //Get the number of Type 2 solutions.

    printf("\n\n");

    printf("INPUT - volume of pure water (L) : ");
    scanf("%s", temp_input);
    volume_all = strtod(temp_input, NULL);
    printf("\n");
    //Get volume of pure water.

    printf("\n\n");

    for (i = 0; i < num_acid; i++) {
        printf("INPUT - volume of strong acid titrand (L) : ");
        scanf("%s", temp_input);
        volume = strtod(temp_input, NULL);
        volume_all = volume_all + volume;
        printf("\n");
        fflush(stdin);
        printf("INPUT - Concentration of the titrand (mol/L) : ");
        scanf("%s", temp_input);
        cen = strtod(temp_input, NULL);
        concentration_acid[i] = cen * volume;
        printf("\n\n");
        fflush(stdin);
    }   //Get data of strong acid titrands.
    for (i = 0; i < num_base; i++) {
        printf("INPUT - volume of strong base titrand (L) : ");
        scanf("%s", temp_input);
        volume = strtod(temp_input, NULL);
        volume_all = volume_all + volume;
        printf("\n");
        fflush(stdin);
        printf("INPUT - Concentration of the titrand (mol/L) : ");
        scanf("%s", temp_input);
        cen = strtod(temp_input, NULL);
        concentration_base[i] = cen * volume;
        printf("\n\n");
        fflush(stdin);
    }   //Get data of strong base titrands.
    for (i = 0; i < num_type_1; i++) {
        printf("INPUT - Name of Type 1 titrand : ");
        scanf("%s", name);
        index_type_1[i] = SpecifySolute(type_1_database, num_type_1_database, name);
        printf("\n");
        fflush(stdin);
        if (index_type_1[i] == -1) {
            printf("INFO - GraphGenerator function ended.\n");
            return -1;
        }
        printf("INPUT - volume of the titrand (L) : ");
        scanf("%s", temp_input);
        volume = strtod(temp_input, NULL);
        volume_all = volume_all + volume;
        printf("\n");
        fflush(stdin);
        printf("INPUT - Concentration of the titrand (mol/L) : ");
        scanf("%s", temp_input);
        cen = strtod(temp_input, NULL);
        concentration_type_1[i] = volume * cen;
        printf("\n\n");
        fflush(stdin);
    }   //Get data of the Type 1 solutions.
    for (i = 0; i < num_type_2; i++) {
        printf("INPUT - Name of Type 2 powder : ");
        scanf("%s", name);
        index_type_2[i] = SpecifySolute(type_2_database, num_type_2_database, name);
        printf("\n");
        fflush(stdin);
        if (index_type_2[i] == -1) {
            printf("INFO - GraphGenerator function ended.\n");
            return -1;
        }
        printf("INPUT - Mol of the powder (mol) : ");
        scanf("%s", temp_input);
        mol_type_2[i] = strtod(temp_input, NULL);
        printf("\n\n");
        fflush(stdin);
    }   //Get data of the Type 2 solutions.

    printf("\n");

    printf("INPUT - Target pH : ");
    scanf("%s", temp_input);
    target_ph = strtod(temp_input, NULL);
    printf("\n");
    //Get target pH value.

    printf("\n\n");

    titrand_ph = PH(type_2_database, type_1_database, num_acid, num_base, num_type_1, num_type_2,
                             concentration_acid, concentration_base, concentration_type_1, index_type_1, mol_type_2, index_type_2, volume_all,
                             PH_CALCULATOR_STARTPOINT, PH_CALCULATOR_ENDPOINT, PH_CALCULATOR_RESOLUTION,
                             PH_CALCULATOR_INITIALINTERVAL);

    printf("INPUT - Type of titrant [strong acid : 0 | strong base : 1 | Type 1 : 2 | Type 2 : 3] : ");
    scanf("%s", temp_input);
    printf("\n");
    fflush(stdin);
    //Get type of the titrant.

    i = 0;
    suitable_volume = 0;
    suitable_answer = 0;
    switch (temp_input[0]) {
        case '0' :
            num_acid++;
            printf("INPUT - Concentration of the titrant (mol/L) : ");
            scanf("%s", temp_input);
            concentration_titrant = strtod(temp_input, NULL);
            printf("\n");
            //Get data of strong acid titrant.

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

        case '1' :
            num_base++;
            printf("INPUT - Concentration of the titrant (mol/L) : ");
            scanf("%s", temp_input);
            concentration_titrant = strtod(temp_input, NULL);
            printf("\n");
            //Get data of strong base titrant.

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

        case '2' :
            num_type_1++;
            printf("INPUT - Name of the Type 1 titrant : ");
            scanf("%s", name);
            index_type_1[num_type_1 - 1] = SpecifySolute(type_1_database, num_type_1_database, name);
            printf("\n");
            if (index_type_1[i] == -1) {
                printf("INFO - RecipeFinder function ended.\n");
                return -1;
            }
            printf("INPUT - Concentration of the Type 1 titrant (mol/L) : ");
            scanf("%s", temp_input);
            concentration_titrant = strtod(temp_input, NULL);
            printf("\n");
            //Get data of the Type 1 titrant.

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

        case '3' :
            num_type_2++;
            printf("INPUT - Name of the Type 2 titrant : ");
            scanf("%s", name);
            index_type_2[num_type_2 - 1] = SpecifySolute(type_2_database,
                                                               num_type_2_database, name);
            printf("\n");
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
    return 0;
    //Return 0 after operation.
}


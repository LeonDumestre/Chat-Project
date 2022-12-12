
/*
 * SPDX-FileCopyrightText: 2022 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/* 
 * L'objectif principal de ce code est d'effectuer des tests unitaires et
 *  fonctionnels sur les différents composants créés dans ce projet.
 */

#include "tests.h"

void test_create_json()
{
    // Test de validation de JSON
    printf("Tests de création de JSON\n");

    char* result1 = writeJSON("message", "bonjour", true);
    printf("%s\n", result1);
    free(result1);

    char* result2 = writeJSON("calcule", "+ 2 3", true);
    printf("%s\n", result2);
    free(result2);

    char* result3 = writeJSON("calcule", "100", false);
    printf("%s\n", result3);
    free(result3);
}

void test_validation_json()
{
    // Test de validation de JSON
    printf("Tests de validation de JSON\n");

    char *json1 = "{\"code\":\"balises\",\"valeurs\":[ 2,\"#nature\",\"#arbre\"]}";
    char *json2 = "{\"code\":message,\"valeurs\":[\"bonjour\"]}";
    char *json3 = "\"code\":\"message\",\"valeurs\":[\"bonjour\"]}";
    char *json4 = "{\"code\":\"calcule\",\"valeurs\":[\"+\",2,54]}";
    
    int result = validation_JSON(json1);
    printf("result = %d\n", result == 1);
    result = validation_JSON(json2);
    printf("result = %d\n", result == 0);
    result = validation_JSON(json3);
    printf("result = %d\n", result == 0);
    result = validation_JSON(json4);
    printf("result = %d\n", result == 1);
}

void test_create_validation_json()
{
    char* str_res1 = writeJSON("message", "bonjour", true);
    printf("%s\n", str_res1);
    int int_res1 = validation_JSON(str_res1);
    printf("result = %d\n", int_res1 == 1);
    free(str_res1);

    char* str_res2 = writeJSON("calcule", "+ 34 2", true);
    printf("%s\n", str_res2);
    int int_res2 = validation_JSON(str_res2);
    printf("result = %d\n", int_res2 == 1);
    free(str_res2);
}

int main()
{
    printf("TESTS UNITAIRES :\n");
    test_create_json();
    printf("\n");
    test_validation_json();

    printf("\nTESTS FONCTIONNELS :\n");
    test_create_validation_json();
    return 0;
}
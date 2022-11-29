
/*
 * SPDX-FileCopyrightText: 2022 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/* 
 * Code pour traiter différents types d'opérations mathématiques
 */

#include "operations.h"
#include <math.h>

char* calcule(char data[])
{
  char op;
  float N1, N2;
  float res = 0.0;
  int conv = sscanf(data, "%c,%f,%f", &op, &N1, &N2);

  char* strRes = malloc(sizeof(char) * 1024);

  if (conv == 3)
  {
    switch (op)
    {
      case '+':
        res = N1 + N2;
        break;
      case '-':
        res = N1 - N2;
        break;
      case '*':
        res = N1 * N2;
        break;
      case '/':
        if (N2 != 0) res = N1 / N2;
        break;
      case '%':
        res = (int)N1 % (int)N2;
        break;
      case '&':
        res = (int)N1 & (int)N2;
        break;
      case '|':
        res = (int)N1 | (int)N2;
        break;
    }

    if (op == '~')
    {
      int res1 = ~(int)N1;
      int res2 = ~(int)N2;
    
      char* strRes1 = malloc(sizeof(char) * 1024);
      char* strRes2 = malloc(sizeof(char) * 1024);
      sprintf(strRes1, "%d", res1);
      sprintf(strRes2, "%d", res2);

      strcpy(strRes, strRes1);
      strcat(strRes, ",");
      strcat(strRes, strRes2);

      free(strRes1);
      free(strRes2);
    }
    else {
      sprintf(strRes, "%f", res);
    }
  }
  else {
    char* op = malloc(sizeof(char) * 1024);

    strcpy(op, data);

    // Récupération  de l'opérateur
    op = strtok(op, ",");

    // Supression de l'opérateur dans data
    memmove(data, data+4, strlen(data));

    // Récupération des nombres
    char* token = strtok(data, ",");
    int i = 0;
    float numbers[100];
    while (token != NULL)
    {
      numbers[i] = atof(token); 
      token = strtok(NULL, ",");
      i++;
    }

    if(strcmp(op, "avg") == 0){
      //Calcul de la moyenne
      float sum = 0.0;
      for (int j = 0; j < i; j++)
      {
        sum += numbers[j];
      }
      res = sum / i;
    }
    else if(strcmp(op, "min") == 0){
      //Calcul du minimum
      res = numbers[0];
      for (int j = 1; j < i; j++)
      {
        if (numbers[j] < res) res = numbers[j];
      }
    }
    else if(strcmp(op, "max") == 0){
      //Calcul du maximum
      res = numbers[0];
      for (int j = 1; j < i; j++)
      {
        if (numbers[j] > res) res = numbers[j];
      }
    }
    else if(strcmp(op, "gap") == 0){
      //Calcul de l'écart type
      float sum = 0.0;
      for (int j = 0; j < i; j++)
      {
        sum += numbers[j];
      }
      float avg = sum / i;
      sum = 0.0;
      for (int j = 0; j < i; j++)
      {
        sum += pow(numbers[j] - avg, 2);
      }
      res = sqrt(sum / i);
    }
    sprintf(strRes, "%f", res);
    free(op);
  }
  return strRes;
}
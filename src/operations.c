
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

char* calcule(char data[])
{
  char op;
  float N1, N2;
  sscanf(data, "%c,%f,%f", &op, &N1, &N2);

  float res = 0.0;
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

  char* strRes = malloc(sizeof(char) * 1024);

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

  return strRes;
}
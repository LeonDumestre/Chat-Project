
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
  int N1, N2;
  sscanf(data, "%c,%d,%d", &op, &N1, &N2);

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
  }

  char* strRes = malloc(sizeof(char) * 200);
  sprintf(strRes, "%f", res);
  return strRes;
}
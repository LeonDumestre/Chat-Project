/*
 * SPDX-FileCopyrightText: 2022 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include "json.h"

char* writeJSON(char message_type[], char message[])
{
  char* json = malloc(sizeof(char) * 2048);

  strcpy(json, "{\"code\":\"");
  strcat(json, message_type);
  strcat(json, "\",\"valeurs\":[");

  if (strcmp(message_type, "message") == 0)
  {
    strcat(json, "\"");
    strcat(json, message);
    strcat(json, "\"");
  }

  else if (strcmp(message_type, "calcule") == 0)
  {
    char op;
    int N1, N2;
    int conv = sscanf(message, "%c %d %d", &op, &N1, &N2);

    char* tmp = malloc(sizeof(char)*200);
    if (conv == 3 && (op == '+' || op == '-'))
    {
      strcat(json, "\"");
      char* pOp = &op;
      strcat(json, pOp);
      strcat(json, "\",");

      sprintf(tmp, "%d", N1);
      strcat(json, tmp);
      strcat(json, ",");

      sprintf(tmp, "%d", N2);
      strcat(json, tmp);
    }
    else
    {
      float f;
      conv = sscanf(message, "%f", &f);
      if (conv == 1)
      {
        sprintf(tmp, "%f", f);
        strcat(json, tmp);
      }
    }
    free(tmp);
  }

  else if (strcmp(message_type, "couleurs") == 0)
  {
    strcat(json, "\"");
    strcat(json, message);
    strcat(json, "\"");
  }

  else if (strcmp(message_type, "balises") == 0)
  {
    strcat(json, "\"");
    strcat(json, message);
    strcat(json, "\"");
  }
  
  strcat(json, "]}");
  return json;
}

void getCode()
{
  // char* str = json + 4;
  // printf("Test: %s", str);
  printf("test");
}

char* getCalculeOperator(char json[])
{

}

int* getCalculeMessage(char json[])
{

}
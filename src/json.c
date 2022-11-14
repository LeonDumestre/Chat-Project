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

  if (strcmp(message_type, "message") == 0 || strcmp(message_type, "nom") == 0)
  {
    if (message[0] != '"') strcat(json, "\"");
    strcat(json, message);
    if (message[(int)strlen(message)-1] != '"') strcat(json, "\"");
  }

  else if (strcmp(message_type, "calcule") == 0)
  {
    char op;
    float F1, F2;
    int conv = sscanf(message, "%c %f %f", &op, &F1, &F2);

    char* tmp = malloc(sizeof(char)*200);
    //Message du client ou Réponse du serveur si ~
    if (conv == 3)
    {
      strcat(json, "\"");
      json[strlen(json)] = op;
      json[strlen(json)] = '\0';
      strcat(json, "\",");

      sprintf(tmp, "%f", F1);
      strcat(json, tmp);
      strcat(json, ",");

      sprintf(tmp, "%f", F2);
      strcat(json, tmp);
    }
    //Réponse du serveur
    else
    {
      strcat(json, message);
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

char* getCode(char json[])
{
  char str[2048];
  strcpy(str, json);

  char* code = malloc(sizeof(char) * 10);
  int ind = 0, indCode = 0, isCode = 0;

  while (ind < (int)strlen(str))
  {
    if (isCode)
    {
      if (str[ind] == '"') {
        break;
      }

      code[indCode] = str[ind];
      indCode++;
    }

    else if (str[ind] == ':' && ind+1 < (int)strlen(str) && str[ind+1] == '\"')
    {
      ind++;
      isCode = 1;
    }
    ind++;
  }
  return code;
}

char* getValeurs(char json[])
{
  char str[2048];
  strcpy(str, json);

  char* valeurs = malloc(sizeof(char) * 10);
  int ind = 0, indValeurs = 0, isValeurs = 0;

  while (ind < (int)strlen(str))
  {
    if (isValeurs)
    {
      if (str[ind] == ']') break;
      if (str[ind] == '"') 
      {
        ind++;
        continue;
      }
      valeurs[indValeurs] = str[ind];
      indValeurs++;
    }

    else if (str[ind] == '[')
    {
      isValeurs = 1;
    }
    ind++;
  }
  return valeurs;
}
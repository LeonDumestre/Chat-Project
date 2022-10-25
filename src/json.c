/*
 * SPDX-FileCopyrightText: 2022 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

void writeJSON(char message_type[], char message[]) {
  char json[2048] = "{\"code\":\"";

  strcat(json, message_type);
  strcat(json, "\",\"valeurs\":[\"");

  if (strcmp(message_type, "message"))
  {
    strcat(json, message);
  }
  else if (strcmp(message_type, "calcule"))
  {
    char op;
    int N1, N2;
    int conv = sscanf(message, "%c %d %d", &op, &N1, &N2);
    if (op == '+' || op == '-')
    {
      char* tmp = malloc(sizeof(char)*200);

      strcat(json, "\"");
      strcat(json, op);
      strcat(json, ",");

      sprintf(tmp, "%d", N1);
      strcat(json, tmp);
      strcat(json, ",");

      sprintf(tmp, "%d", N2);
      strcat(json, tmp);

      free(tmp);
    }
  }
  else if (strcmp(message_type, "couleurs"))
  {
    strcat(json, message);
  }
  else if (strcmp(message_type, "balises"))
  {
    strcat(json, message);
  }
  
  strcat(json, "\"]}");

  printf("Json: %s\n", json);
  //return json;
}
#include "json.h"

/*
 * Fonction convertissant les données brut en JSON
 * Prend en argument message_type (message, calcule, couleurs, balises), message (donnée brut) et le booléen sendByClient
 * Return un pointeur vers le JSON transformé ou NULL si la donnée reçu n'est pas valide
 */
char* writeJSON(char message_type[], char message[], bool sendByClient)
{
  // Return NULL si le message est vide
  if (strlen(message_type) == 0 || strlen(message) == 0) return NULL;

  // Création du JSON et ajout du code
  char* json = malloc(sizeof(char) * 2048);
  strcpy(json, "{\"code\":\"");
  strcat(json, message_type);
  strcat(json, "\",\"valeurs\":[");

  // Si le message_type est message ou nom
  if (strcmp(message_type, "message") == 0 || strcmp(message_type, "nom") == 0)
  {
    // Copie simplement le message entre guillemets
    if (message[0] != '"') strcat(json, "\"");
    strcat(json, message);
    if (message[(int)strlen(message)-1] != '"') strcat(json, "\"");
  }

  // Sinon si le message_type est calcule
  else if (strcmp(message_type, "calcule") == 0)
  {
    // Message du client
    if (sendByClient)
    {
      char* op = malloc(sizeof(char) * 2);
      float F1, F2;
      int conv = sscanf(message, "%s %f %f", op, &F1, &F2);

      // Vérifie si message contient 1 opérateur valide et 2 nombres (float)
      if (conv == 3 && (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0 || strcmp(op, "&") == 0 || strcmp(op, "|") == 0 || strcmp(op, "~") == 0))
      {
        char* tmp = malloc(sizeof(char)*200);
        // Copie l'opérateur entre guillemets
        strcat(json, "\"");
        strcat(json, op);
        json[strlen(json)] = '\0';
        strcat(json, "\",");

        // Copie les 2 nombres
        sprintf(tmp, "%f", F1);
        strcat(json, tmp);
        strcat(json, ",");

        sprintf(tmp, "%f", F2);
        strcat(json, tmp);
        free(tmp);
      }
      else if (strcmp(op, "avg") == 0 || strcmp(op, "min") == 0 || strcmp(op, "max") == 0 || strcmp(op, "gap") == 0)
      {
        // Copie l'opérateur entre guillemets
        strcat(json, "\"");
        strcat(json, op);
        json[strlen(json)] = '\0';
        strcat(json, "\",");

        //Copie des nombres
        char* tmp = malloc(sizeof(char)*200);
        char* token = strtok(message, " ");
        token = strtok(NULL, " ");
        while (token != NULL)
        {
          sprintf(tmp, "%f", atof(token));
          strcat(json, tmp);
          strcat(json, ",");
          token = strtok(NULL, " ");
        }
        free(tmp);
      }
      // Si message ne contient pas 1 opérateur valide et 2 nombres (float)
      else {
        free(json);
        return NULL;
      }
      free(op);
    }
    // Réponse du serveur
    else {
      strcat(json, message);
    }
  }

  // Sinon si le message_type est couleurs ou balises
  else if (strcmp(message_type, "couleurs") == 0 || strcmp(message_type, "balises") == 0)
  {
    // Message du client
    if (sendByClient)
    {
      // Récupère le nombre d'items données au début de message et le supprime de message
      int nb;
      int conv = sscanf(message, "%d", &nb);
      int nbDigit = getNbDigit(nb) + 1;
      memmove(message, message + nbDigit, strlen(message));

      // Copie le nombre si valide
      if (conv == 1)
      {
        char* tmp = malloc(sizeof(char)*200);
        sprintf(tmp, "%d", nb);
        strcat(json, tmp);
        free(tmp);
        strcat(json, ",");

        // Boucle en foncttion du nombre d'items
        for (int i = 0; i < nb; i++)
        {
          char* item = malloc(sizeof(char) * 1024);
          // Boucle chaque caractère de message jusqu'à la fin
          for (int l = 0; l < (int)strlen(message); l++)
          {
            // Sort de la boucle si l'item est terminé (car ,)
            if (message[l] == ',') break;
            // Copie le caractère de message dans l'item
            item[l] = message[l];
          }
          // Si l'item ne commence pas par # et ne fait pas 6 caractères (7 car virgule) si c'est un couleur
          if (item[0] != '#' || (strcmp(message_type, "couleurs") == 0 && (int)strlen(item) != 7))
          {
            free(json);
            return NULL;
          }

          // Copie l'item dans le JSON et le supprime de message
          memmove(message, message + (int)strlen(item) + 1, strlen(message));
          strcat(json, "\"");
          strcat(json, item);
          strcat(json, "\"");
          if (i < nb-1) strcat(json, ",");
          free(item);
        }
      }
      // Si le nombre n'est pas valide
      else
      {
        free(json);
        return NULL;
      }
    }
    // Réponse du serveur
    else
    {
      strcat(json, "\"enregistré\"");
    }
  }
  
  strcat(json, "]}");
  return json;
}

/*
 * Fonction récupérant le code du json
 * Prend en argument le json
 * Return un pointeur vers le code récupéré
 */
char* getCode(char json[])
{
  char str[2048];
  strcpy(str, json);

  char* code = malloc(sizeof(char) * 10);
  int ind = 0, indCode = 0, isCode = 0;

  // Parcours la copie du json
  while (ind < (int)strlen(str))
  {
    // Si " est présent alors le code est terminé
    if (isCode)
    {
      if (str[ind] == '"') break;
      code[indCode] = str[ind];
      indCode++;
    }

    // Si :" est présent alors la suite est un code
    else if (str[ind] == ':' && ind+1 < (int)strlen(str) && str[ind+1] == '\"')
    {
      ind++;
      isCode = 1;
    }
    ind++;
  }
  return code;
}

/*
 * Fonction récupérant les valeurs du json
 * Prend en argument le json
 * Return un pointeur vers les valeurs récupérées
 */
char* getValeurs(char json[])
{
  char str[1024];
  strcpy(str, json);

  char* valeurs = malloc(sizeof(char) * 1024);
  int ind = 0, indValeurs = 0, isValeurs = 0;

  // Parcours la copie du json
  while (ind < (int)strlen(str))
  {
    if (isValeurs)
    {
      // Si ]" est présent alors les valeurs sont terminées
      if (str[ind] == ']') break;
      if (str[ind] == '"')
      {
        ind++;
        valeurs[indValeurs + 1] = '\0';
        continue;
      }
      valeurs[indValeurs] = str[ind];
      indValeurs++;
    }
    // Si [ est présent alors la suite contient les valeurs
    else if (str[ind] == '[')
    {
      isValeurs = 1;
    }
    ind++;
  }

  return valeurs;
}

/*
 * Fonction renvoyant le nombre de chiffres dans un nombre
 */
int getNbDigit(int number) {
  int count = 0, nb = number;
  while (nb != 0)
  {
    nb /= 10;
    count++;
  }
  return count;
}
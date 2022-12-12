/*
 * SPDX-FileCopyrightText: 2021 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/* 
 * Code du côté client. Le but principal de ce code est de communiquer avec le serveur, 
 * d'envoyer et de recevoir des messages. Ces messages peuvent être du simple texte ou des chiffres.
 */

#include "client.h"

/*
 * Fonction d'analyse d'une image.
 * Arguments: le chemin d'accès à l'image, la variable à remplir, le nombre de couleurs à analyser.
 */
void analyse(char *pathname, char *data, int nb_couleurs)
{
  // compte de couleurs
  couleur_compteur *cc = analyse_bmp_image(pathname);

  int count;
  char nb_coleurs_str[10];
  sprintf(nb_coleurs_str, "%d,", nb_couleurs);

  strcpy(data, "/c ");
  char temp_string[10] = "";
  strcpy(temp_string, nb_coleurs_str);
  if (cc->size < nb_couleurs)
  {
    sprintf(temp_string, "%d,", cc->size);
  }
  strcat(data, temp_string);

  for (count = 1; count < nb_couleurs + 1 && cc->size - count > 0; count++)
  {
    if (cc->compte_bit == BITS32)
    {
      sprintf(temp_string, "#%02x%02x%02x,", cc->cc.cc24[cc->size - count].c.rouge, cc->cc.cc32[cc->size - count].c.vert, cc->cc.cc32[cc->size - count].c.bleu);
    }
    if (cc->compte_bit == BITS24)
    {
      sprintf(temp_string, "#%02x%02x%02x,", cc->cc.cc32[cc->size - count].c.rouge, cc->cc.cc32[cc->size - count].c.vert, cc->cc.cc32[cc->size - count].c.bleu);
    }
    strcat(data, temp_string);
  }
}

/*
 * Fonction de conversion au format JSON
 * Argument: les données à convertir.
 */
char* convertToJson(char message[])
{
  // Si il n'y a pas de commande /, c'est un message par défaut
  char message_type[20] = "message";

  // Si la commande commence pas /
  if (message[0] == '/' && message[2] == ' ')
  {
    // Vérifie la lettre après le / et copie le type dans message_type
    char letter = message[1];
    memmove(message, message + 3, strlen(message));
    switch (letter)
    {
      case 'm':;
        break;
      case 'n':;
        strcpy(message_type, "calcule");
        break;
      case 'c':
        strcpy(message_type, "couleurs");
        break;
      case 'b':
        strcpy(message_type, "balises");
        break;
      default:
        return NULL;
        break;
    }
  }
  // Récupère le message transformé en JSON (NULL si erreur)
  char* json = writeJSON(message_type, message, true);
  return json;
}

/*
 * Fonction d'envoie et de réception du nom du client.
 * Arguments: l'identifiant de la socket.
 */
int envoie_nom_client(int socketfd)
{
  // Réception du nom du client
  char nom[25];
  gethostname(nom, 25);
  char* data = writeJSON("nom", nom, true);

  // Envoie du nom du client
  int write_status = write(socketfd, data, strlen(data));
  if (write_status < 0)
  {
    perror("erreur ecriture");
    exit(EXIT_FAILURE);
  }
  free(data);
  
  // Résultat de l'envoie
  char res[1024];

  // Réception du résultat
  int read_status = read(socketfd, res, sizeof(res));
  if (read_status < 0)
  {
    perror("erreur lecture");
    return -1;
  }

  printf("Nom recu: %s\n", res);
  return 0;
}

/*
 * Fonction d'envoi et de réception des couleurs d'une image
 * Il faut un argument : l'identifiant de la socket
 */
int envoie_couleurs(int socketfd, char *pathname, int nb_couleurs)
{
  // Analyse de l'image
  char data[1024];
  analyse(pathname, data, nb_couleurs);

  // Envoie des couleurs
  char* json = convertToJson(data);
  int write_status = write(socketfd, json, strlen(json));
  free(json);

  if (write_status < 0)
  {
    perror("erreur ecriture");
    exit(EXIT_FAILURE);
  }

  // Réinitialisation de la variable data
  memset(data, 0, sizeof(data));

  // Résultat de l'envoie
  int read_status = read(socketfd, data, sizeof(data));
  if (read_status < 0)
  {
    perror("erreur lecture");
    return -1;
  }

  printf("Message recu: %s\n", data);

  // Appel de la fonction pour envoyer un message
  envoie_recois_message(socketfd);

  return 0;
}

/*
 * Fonction d'envoi et de réception de messages
 * Argument : l'identifiant de la socket
 */
int envoie_recois_message(int socketfd)
{
  char data[2048];
  // la réinitialisation de l'ensemble des données
  memset(data, 0, sizeof(data));

  // Demandez à l'utilisateur d'entrer un message
  char message[1024];
  printf("Votre message (max 1000 caracteres): ");
  fgets(message, sizeof(message), stdin);

  if (message[(int)strlen(message)-1] == '\n') message[(int)strlen(message)-1] = '\0';
  // Récupère le message transformé en JSON (NULL si erreur)
  char* json = convertToJson(message);

  if (json != NULL)
  {
    // Envoie du message
    int write_status = write(socketfd, json, strlen(json));
    free(json);
    if (write_status < 0)
    {
      perror("erreur ecriture");
      exit(EXIT_FAILURE);
    }

    // Réinitialisation de la variable data
    memset(data, 0, sizeof(data));

    // Résultat de l'envoie
    int read_status = read(socketfd, data, sizeof(data));
    if (read_status < 0)
    {
      perror("erreur lecture");
      return -1;
    }

    printf("Message recu: %s\n", data);
  }
  else {
    printf("[ERREUR] La commande n'est pas valide\n");
  }

  // Appel de la fonction pour envoyer un nouveau message
  envoie_recois_message(socketfd);

  return 0;
}

int main(int argc, char **argv)
{
  int socketfd;
  struct sockaddr_in server_addr;

  /*
   * Creation d'une socket
   */
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // détails du serveur (adresse et port)
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // demande de connection au serveur
  int connect_status = connect(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (connect_status < 0)
  {
    perror("connection serveur");
    exit(EXIT_FAILURE);
  }
  if (argc != 2)
  {
    // envoyer et recevoir un nom
    envoie_nom_client(socketfd);
    // envoyer et recevoir un message
    envoie_recois_message(socketfd);
  }
  else
  {
    // envoyer et recevoir un nom
    envoie_nom_client(socketfd);

    // Demandez à l'utilisateur d'entrer un message
    char message[1024];
    int nb_colors = 10;

    printf("Veuillez indiquer le nombre de couleurs à analyser (<30): ");
    fgets(message, sizeof(message), stdin);

    if (message[(int)strlen(message)-1] == '\n') message[(int)strlen(message)-1] = '\0';

    if (message != NULL)
    {
      if (atoi(message) < 30 && atoi(message) > 0)
      {
        nb_colors = atoi(message);
      }
    }

    // envoyer et recevoir les couleurs prédominantes
    // d'une image au format BMP (argv[1])
    envoie_couleurs(socketfd, argv[1], nb_colors);
  }

  close(socketfd);
}

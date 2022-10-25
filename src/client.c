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

void analyse(char *pathname, char *data)
{
  // compte de couleurs
  couleur_compteur *cc = analyse_bmp_image(pathname);

  int count;
  strcpy(data, "couleurs: ");
  char temp_string[10] = "10,";
  if (cc->size < 10)
  {
    sprintf(temp_string, "%d,", cc->size);
  }
  strcat(data, temp_string);

  // choisir 10 couleurs
  for (count = 1; count < 11 && cc->size - count > 0; count++)
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
  // enlever le dernier virgule
  data[strlen(data) - 1] = '\0';
}

char* convertToJson(char message[])
{
  char message_type[20] = "message";

  if (message[0] == '/' && message[2] == ' ')
  {
    char letter = message[1];
    memmove(message, message + 3, strlen(message));
    switch (letter)
    {
      case 'n':;
        char op;
        int N1, N2;
        int conv = sscanf(message, "%c %d %d", &op, &N1, &N2);

        if (conv == 3 && (op == '+' || op == '-'))
        {
          strcpy(message_type, "calcule");
        }
        break;
      case 'c':
        strcpy(message_type, "couleurs");
        break;

      case 'b':
        strcpy(message_type, "balises");
        break;
    }
  }
  char* json = writeJSON(message_type, message);
  return json;
}

int envoie_nom_client(int socketfd)
{
  char data[25];
  // la réinitialisation de l'ensemble des données
  memset(data, 0, sizeof(data));

  // Demandez à l'utilisateur d'entrer un message
  char nom[25];
  gethostname(nom, 25);
  strcpy(data, "nom: ");
  strcat(data, nom);

  int write_status = write(socketfd, data, strlen(data));
  if (write_status < 0)
  {
    perror("erreur ecriture");
    exit(EXIT_FAILURE);
  }

  // la réinitialisation de l'ensemble des données
  memset(data, 0, sizeof(data));

  // lire les données de la socket
  int read_status = read(socketfd, data, sizeof(data));
  if (read_status < 0)
  {
    perror("erreur lecture");
    return -1;
  }

  printf("Nom recu: %s\n", data);

  return 0;
}

int envoie_couleurs(int socketfd, char *pathname)
{
  char data[1024];
  memset(data, 0, sizeof(data));
  analyse(pathname, data);

  int write_status = write(socketfd, data, strlen(data));
  if (write_status < 0)
  {
    perror("erreur ecriture");
    exit(EXIT_FAILURE);
  }
  return 0;
}

/*
 * Fonction d'envoi et de réception de messages
 * Il faut un argument : l'identifiant de la socket
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

  char* json = convertToJson(message);

  int write_status = write(socketfd, json, strlen(json));
  free(json);
  if (write_status < 0)
  {
    perror("erreur ecriture");
    exit(EXIT_FAILURE);
  }

  // la réinitialisation de l'ensemble des données
  memset(data, 0, sizeof(data));

  // lire les données de la socket
  int read_status = read(socketfd, data, sizeof(data));
  if (read_status < 0)
  {
    perror("erreur lecture");
    return -1;
  }

  printf("Message recu: %s\n", data);

  envoie_recois_message(socketfd);

  return 0;
}

int main(int argc, char **argv)
{
  int socketfd;
  struct sockaddr_in server_addr;

  // if (argc < 2)
  // {
  //   printf("usage: ./client chemin_bmp_image\n");
  //   return (EXIT_FAILURE);
  // }

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
    // envoyer et recevoir les couleurs prédominantes
    // d'une image au format BMP (argv[1])
    envoie_couleurs(socketfd, argv[1]);
  }

  close(socketfd);
}

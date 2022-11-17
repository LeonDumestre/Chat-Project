/*
 * SPDX-FileCopyrightText: 2021 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/*
 * Le code côté serveur. L'objectif principal est de recevoir des messages des clients,
 * de traiter ces messages et de répondre aux clients.
 */

#include "serveur.h"

void plot(char *data, int nb_couleurs)
{
  // Extraire le compteur et les couleurs RGB
  FILE *p = popen("gnuplot -persist", "w");
  int count = 0;
  char *saveptr = NULL;
  char *str = data;
  fprintf(p, "set xrange [-15:15]\n");
  fprintf(p, "set yrange [-15:15]\n");
  fprintf(p, "set style fill transparent solid 0.9 noborder\n");
  fprintf(p, "set title 'Top %d colors'\n", nb_couleurs);
  fprintf(p, "plot '-' with circles lc rgbcolor variable\n");
  while (1)
  {
    char *token = strtok_r(str, ",", &saveptr);
    if (token == NULL)
    {
      break;
    }
    str = NULL;
    fprintf(p, "0 0 %d %d %d 0x%s\n", nb_couleurs, (count - 1) * (360 / nb_couleurs), count * (360 / nb_couleurs), token + 1);
    count++;
  }
  fprintf(p, "e\n");
  pclose(p);
}

/*
 * Fonction d'enregistrement de données dans un fichier.
 * Arguments: les données à enregistrer, le chemin d'accès du fichier.
 */
void enregistre_data(char *data, char *pathname)
{
  // Remplissage du fichier avec les données
  FILE *f = fopen(pathname, "a");
  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }
  strcat(data, "\n");
  fprintf(f, "%s", data);
  fclose(f);
}

/*
 * Fonction de renvoi de données au client.
 * Arguments: l'identifiant du client, les données à envoyer.
 */
int renvoie_message(int client_socket_fd, char *data)
{
  // Envoi des données au client
  int data_size = write(client_socket_fd, (void *)data, strlen(data));

  if (data_size < 0)
  {
    perror("erreur ecriture");
    return (EXIT_FAILURE);
  }
  return (EXIT_SUCCESS);
}

/*
 * Fonction de réception et d'envoi de données au client.
 * Arguments: l'identifiant du client.
 */
int recois_envoie_message(int client_socket_fd)
{
  char data[2048];

  // Réinitialisation de la variable data
  memset(data, 0, sizeof(data));

  // Réception des données du client
  int data_size = read(client_socket_fd, (void *)data, sizeof(data));

  if (data_size < 0)
  {
    perror("erreur lecture");
    return (EXIT_FAILURE);
  }

  printf("Message recu: %s\n", data);

  // Récuération du flag
  char* message_type = getCode(data);

  // Traitement des données reçues en fonction du flag
  if (strcmp(message_type, "nom") == 0)
  {
    renvoie_message(client_socket_fd, data);
  }

  else if (strcmp(message_type, "message") == 0)
  {
    char* valeurs = getValeurs(data); // Récupération des valeurs
    char* final = writeJSON(message_type, valeurs, false); // Création du JSON

    renvoie_message(client_socket_fd, final);

    free(valeurs);
    free(final);
  }

  else if (strcmp(message_type, "calcule") == 0)
  {
    char* valeurs = getValeurs(data); // Récupération des valeurs
    char* res = calcule(valeurs); // Calcul du résultat
    char* final = writeJSON(message_type, res, false); // Création du JSON

    renvoie_message(client_socket_fd, final);

    free(valeurs);
    free(res);
    free(final);
  }

  else if (strcmp(message_type, "couleurs") == 0)
  {
    int nb_couleurs;
    char* valeurs = getValeurs(data); // Récupération des valeurs
    sscanf(valeurs, "%d", &nb_couleurs); // Récupération du nombre de couleurs
    int nbDigit = getNbDigit(nb_couleurs) + 1; // Récupération du nombre de chiffres du nombre de couleurs

    memmove(valeurs, valeurs + nbDigit, strlen(valeurs)); // Suppression du nombre de couleurs dans les valeurs

    enregistre_data(valeurs, "couleurs.txt");
    plot(valeurs, nb_couleurs);    
    renvoie_message(client_socket_fd, data);

    free(valeurs);
  }

  else if (strcmp(message_type, "balises") == 0)
  {
    int nb_balises;
    char* valeurs = getValeurs(data); // Récupération des valeurs
    sscanf(valeurs, "%d", &nb_balises); // Récupération du nombre de balises
    int nbDigit = getNbDigit(nb_balises) + 1; // Récupération du nombre de chiffres du nombre de balises

    memmove(valeurs, valeurs + nbDigit, strlen(valeurs)); // Suppression du nombre de balises dans les valeurs

    enregistre_data(valeurs, "balises.txt");
    renvoie_message(client_socket_fd, data);

    free(valeurs);
  }

  free(message_type);
  return (EXIT_SUCCESS);
}

int main()
{
  int socketfd;
  int bind_status;

  struct sockaddr_in server_addr;

  /*
   * Creation d'une socket
   */
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0)
  {
    perror("Unable to open a socket");
    return -1;
  }

  int option = 1;
  setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  // détails du serveur (adresse et port)
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Relier l'adresse à la socket
  bind_status = bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (bind_status < 0)
  {
    perror("bind");
    return (EXIT_FAILURE);
  }

  // Écouter les messages envoyés par le client
  listen(socketfd, 10);

  struct sockaddr_in client_addr;

  unsigned int client_addr_len = sizeof(client_addr);

  // nouvelle connection de client
  int client_socket_fd = accept(socketfd, (struct sockaddr *)&client_addr, &client_addr_len);
  if (client_socket_fd < 0)
  {
    perror("accept");
    return (EXIT_FAILURE);
  }

  while (1)
  {
    // Lire et répondre au client
    recois_envoie_message(client_socket_fd);
  }

  return 0;
}
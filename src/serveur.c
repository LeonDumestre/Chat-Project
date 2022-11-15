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

void plot(char *data)
{
  // Extraire le compteur et les couleurs RGB
  FILE *p = popen("gnuplot -persist", "w");
  int count = 0;
  char *saveptr = NULL;
  char *str = data;
  fprintf(p, "set xrange [-15:15]\n");
  fprintf(p, "set yrange [-15:15]\n");
  fprintf(p, "set style fill transparent solid 0.9 noborder\n");
  fprintf(p, "set title 'Top 10 colors'\n");
  fprintf(p, "plot '-' with circles lc rgbcolor variable\n");
  while (1)
  {
    char *token = strtok_r(str, ",", &saveptr);
    if (token == NULL)
    {
      break;
    }
    str = NULL;
    if (count != 1)
    {
      // Le numéro 36, parceque 360° (cercle) / 10 couleurs = 36
      fprintf(p, "0 0 10 %d %d 0x%s\n", (count - 1) * 36, count * 36, token + 1);
    }
    count++;
  }
  fprintf(p, "e\n");
  pclose(p);
}

void enregistre_data(char *data, char *pathname)
{
  FILE *f = fopen(pathname, "w");
  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }
  strcat(data, "\n");
  fprintf(f, "%s", data);
  fclose(f);
}

/* renvoyer un message (*data) au client (client_socket_fd) */
int renvoie_message(int client_socket_fd, char *data)
{
  int data_size = write(client_socket_fd, (void *)data, strlen(data));

  if (data_size < 0)
  {
    perror("erreur ecriture");
    return (EXIT_FAILURE);
  }
  return (EXIT_SUCCESS);
}

/* accepter la nouvelle connection d'un client et lire les données
 * envoyées par le client. En suite, le serveur envoie un message
 * en retour
 */
int recois_envoie_message(int client_socket_fd)
{
  char data[2048];

  // la réinitialisation de l'ensemble des données
  memset(data, 0, sizeof(data));

  // lecture de données envoyées par un client
  int data_size = read(client_socket_fd, (void *)data, sizeof(data));

  if (data_size < 0)
  {
    perror("erreur lecture");
    return (EXIT_FAILURE);
  }

  printf("Message recu: %s\n", data);
  char* message_type = getCode(data);

  if (strcmp(message_type, "nom") == 0)
  {
    renvoie_message(client_socket_fd, data);
  }

  else if (strcmp(message_type, "message") == 0)
  {
    char* valeurs = getValeurs(data);
    char* final = writeJSON(message_type, valeurs, false);
    renvoie_message(client_socket_fd, final);
    free(valeurs);
    free(final);
  }

  else if (strcmp(message_type, "calcule") == 0)
  {
    char* valeurs = getValeurs(data);
    char* res = calcule(valeurs);
    char* final = writeJSON(message_type, res, false);
    renvoie_message(client_socket_fd, final);
    free(valeurs);
    free(res);
    free(final);
  }

  else if (strcmp(message_type, "couleurs") == 0)
  {
    char* valeurs = getValeurs(data);
    enregistre_data(valeurs, "couleurs.txt");
    plot(valeurs); 
    free(valeurs);
    renvoie_message(client_socket_fd, data);
  }

  else if (strcmp(message_type, "balises") == 0)
  {
    char* valeurs = getValeurs(data);
    enregistre_data(valeurs, "balises.txt");
    free(valeurs);
    renvoie_message(client_socket_fd, data);
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
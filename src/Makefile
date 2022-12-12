CC ?= gcc

# Modifiez les deux lignes suivantes et ajoutez de nouveaux fichiers selon vos besoins
COBJS ?= client.o bmp.o couleur.o json.o operations.o validation.o
SOBJS ?= serveur.o json.o operations.o validation.o
TOBJS ?= tests.o json.o couleur.o operations.o validation.o

# Avertissements
CFLAGS ?= -Wall -Wextra

# Déboggage
DEBUG ?= -ggdb3

# Thread
THREAD ?= -pthread

# Math.h
MATH ?= -lm

.SUFFIXES: .c .o

SERVER = serveur
CLIENT = client
TESTS = tests

all: $(SERVER) $(CLIENT) $(TESTS)

# Création d'un exécutable pour le côté serveur
$(SERVER): $(SOBJS)
	$(CC) $(DEBUG) $(CFLAGS) $(THREAD) -o $(SERVER) $(SOBJS) $(MATH)

# Création d'un exécutable pour le côté client
$(CLIENT): $(COBJS)
	$(CC) $(DEBUG) $(CFLAGS) -o $(CLIENT) $(COBJS) $(MATH)

# Création d'un exécutable pour les tests
$(TESTS): $(TOBJS)
	$(CC) $(DEBUG) $(CFLAGS) -o $(TESTS) $(TOBJS) $(MATH)

# Compilation du code sans création d'un exécutable
.c.o: 
	$(CC) $(DEBUG) $(CFLAGS) $(THREAD) -c $*.c

#
# Makefile - Project de Programmation Avancee IMA3 : Correcteur Orthographique

# compilateur
CC = clang

# "drapeau" en paramètres
# ajouter "-O0" si besoin d'utiliser valgrind
# -ggdb3 pour les information de debugging
CFLAGS = -ggdb3 -O0 -Wall -Werror

# Nom de l'executable
EXE = correcteur

# Liste des headers
HDRS = dictionnaire.h

# Liste des sources
SRCS = correcteur.c dictionnaire.c

# Generation automatique des .o
OBJS = $(SRCS:.c=.o)

# Commande de compilation
$(EXE): $(OBJS) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# dependances
$(OBJS): $(HDRS) Makefile

# nettoyage
clean:
	rm -f core $(EXE) *.o

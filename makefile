CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -Llib -lknn -lmf -lgraph -lm

# Executables
EXEC = menu server client

# Cible principale
all: build_libs $(EXEC)

# === Compilation des exécutables ===

menu: lib/libknn.a lib/libmf.a lib/libgraph.a src/main_menu.c src/data_loader.c src/eval_metrics.c src/utils.c
	$(CC) $(CFLAGS) -o $@ $(filter %.c,$^) $(LDFLAGS)

server: lib/libknn.a lib/libmf.a lib/libgraph.a src/main_server.c src/data_loader.c src/utils.c src/eval_metrics.c
	$(CC) $(CFLAGS) -o server $^ $(LDFLAGS) -lpthread


client: src/main_client.c
	$(CC) $(CFLAGS) -o $@ $^

# === Bibliothèques statiques ===

build_libs: lib/libknn.a lib/libmf.a lib/libgraph.a

lib/libknn.a: build/knn.o
	ar rcs $@ $^

lib/libmf.a: build/mf.o
	ar rcs $@ $^

lib/libgraph.a: build/graph.o
	ar rcs $@ $^

# === Fichiers objets ===

build/knn.o: src/knn.c
	mkdir -p build lib
	$(CC) -c $< -Iinclude -o $@

build/mf.o: src/mf.c
	$(CC) -c $< -Iinclude -o $@

build/graph.o: src/graph.c
	$(CC) -c $< -Iinclude -o $@

# === Nettoyage ===

clean:
	rm -f $(EXEC) build/*.o lib/*.a

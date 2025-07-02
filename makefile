# CC = gcc
# CFLAGS = -Wall

# all: client server

# client: src/main_client.c
# 	$(CC) $(CFLAGS) -o client src/main_client.c

# server: src/main_server.c
# 	$(CC) $(CFLAGS) -o server src/main_server.c

# clean:
# 	rm -f client server

# data_test: src/test_data.c src/data_loader.c
# 	gcc -o data_test src/test_data.c src/data_loader.c -Iinclude
# gen_ratings: src/gen_ratings.c
# 	gcc -o gen_ratings src/gen_ratings.c
# knn_test: src/test_knn.c src/knn.c src/data_loader.c
# 	gcc -o knn_test src/test_knn.c src/knn.c src/data_loader.c -Iinclude -lm
# mf_test: src/test_mf.c src/mf.c src/data_loader.c
# 	gcc -o mf_test src/test_mf.c src/mf.c src/data_loader.c -Iinclude -lm


# client: src/main_client.c
# 	gcc -o client src/main_client.c

# server: src/main_server.c src/data_loader.c src/knn.c src/mf.c src/graph.c
# 	gcc -o server src/main_server.c src/data_loader.c src/knn.c src/mf.c src/graph.c -Iinclude -lm

# graph_test: src/test_graph.c src/graph.c src/data_loader.c
# 	gcc -o graph_test src/test_graph.c src/graph.c src/data_loader.c -Iinclude -lm
# eval_test: src/test_eval.c src/eval_metrics.c
# 	gcc -o eval_test src/test_eval.c src/eval_metrics.c -Iinclude -lm
# eval_topn_test: src/test_eval_topn.c src/eval_metrics.c
# 	gcc -o eval_topn_test src/test_eval_topn.c src/eval_metrics.c -Iinclude -lm
# menu: src/main_menu.c src/data_loader.c src/knn.c src/mf.c src/graph.c src/utils.c src/eval_metrics.c
# 	gcc -o menu src/main_menu.c src/data_loader.c src/knn.c src/mf.c src/graph.c src/utils.c src/eval_metrics.c -Iinclude -lm
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
	$(CC) $(CFLAGS) -o $@ $(filter %.c,$^) $(LDFLAGS)

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

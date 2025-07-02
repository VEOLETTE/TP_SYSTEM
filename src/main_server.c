#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>

#include "data.h"
#include "knn.h"
#include "mf.h"
#include "graph.h"
#include "utils.h"
#include "data_loader.h"

#define PORT 8080
#define BUFFER_SIZE 2048
#define MAX_CLIENTS 100

Rating ratings[100000];
int rating_count;

void* handle_client(void* client_sock_ptr) {
    int client_sock = *(int*)client_sock_ptr;
    free(client_sock_ptr);

    char buffer[BUFFER_SIZE] = {0};
    read(client_sock, buffer, BUFFER_SIZE);
    printf("Requête reçue : %s\n", buffer);

    // Extraire les données de la requête
    int user_id, nb;
    char algo[10];
    sscanf(buffer, "user_id=%d;algo=%[^;];nb=%d;", &user_id, algo, &nb);

    char result[BUFFER_SIZE] = "";

    if (strcmp(algo, "KNN") == 0) {
        freopen("tmp_knn.txt", "w", stdout);
        recommend_items(ratings, rating_count, user_id, 5, nb);
        fflush(stdout);
        freopen("/dev/tty", "w", stdout);
        FILE* f = fopen("tmp_knn.txt", "r");
        fread(result, 1, BUFFER_SIZE, f);
        fclose(f);
    }
    else if (strcmp(algo, "MF") == 0) {
        train_mf(ratings, rating_count, 20, 0.01, 0.1);
        freopen("tmp_mf.txt", "w", stdout);
        recommend_mf(ratings, rating_count, user_id, nb);
        fflush(stdout);
        freopen("/dev/tty", "w", stdout);
        FILE* f = fopen("tmp_mf.txt", "r");
        fread(result, 1, BUFFER_SIZE, f);
        fclose(f);
    }
    else if (strcmp(algo, "GRAPH") == 0) {
        build_graph(ratings, rating_count, 1000, 1000);
        freopen("tmp_graph.txt", "w", stdout);
        pagerank(user_id, 1000, nb);
        fflush(stdout);
        freopen("/dev/tty", "w", stdout);
        FILE* f = fopen("tmp_graph.txt", "r");
        fread(result, 1, BUFFER_SIZE, f);
        fclose(f);
    } else {
        snprintf(result, BUFFER_SIZE, "Algorithme non reconnu : %s", algo);
    }

    send(client_sock, result, strlen(result), 0);
    close(client_sock);
    pthread_exit(NULL);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    rating_count = load_ratings("data/ratings.txt", ratings, 100000);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, MAX_CLIENTS);

    printf("Serveur prêt sur le port %d\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        int* client_sock_ptr = malloc(sizeof(int));
        *client_sock_ptr = new_socket;

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_sock_ptr);
        pthread_detach(tid); // Ne pas bloquer
    }

    close(server_fd);
    return 0;
}

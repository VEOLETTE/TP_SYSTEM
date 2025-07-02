#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#include "data.h"
#include "knn.h"
#include "mf.h"
#include "graph.h"
#include "data_loader.h"

#define PORT 8080
#define BUFFER_SIZE 2048

Rating ratings[100000];
int rating_count;

void parse_request(char* request, int* user_id, char* algo, int* nb) {
    sscanf(request, "user_id=%d;algo=%[^;];nb=%d;", user_id, algo, nb);
}

void handle_request(int client_sock, char* request) {
    int user_id, nb;
    char algo[10];
    parse_request(request, &user_id, algo, &nb);

    char result[BUFFER_SIZE] = "";
    if (strcmp(algo, "KNN") == 0) {
        // rediriger stdout vers buffer
        freopen("tmp_knn.txt", "w", stdout);
        recommend_items(ratings, rating_count, user_id, 5, nb);
        fflush(stdout);
        freopen("/dev/tty", "w", stdout); // remettre stdout normal

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
        build_graph(ratings, rating_count, MAX_USERS, MAX_ITEMS);
        freopen("tmp_graph.txt", "w", stdout);
        pagerank(user_id, MAX_ITEMS, nb);
        fflush(stdout);
        freopen("/dev/tty", "w", stdout);

        FILE* f = fopen("tmp_graph.txt", "r");
        fread(result, 1, BUFFER_SIZE, f);
        fclose(f);
    }

    else {
        snprintf(result, BUFFER_SIZE, "Algorithme non reconnu : %s", algo);
    }

    send(client_sock, result, strlen(result), 0);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Charger les données
    rating_count = load_ratings("data/ratings.txt", ratings, 100000);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Serveur en écoute...\n");

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        read(new_socket, buffer, BUFFER_SIZE);
        printf("Requête reçue : %s\n", buffer);
        handle_request(new_socket, buffer);
        close(new_socket);
    }

    close(server_fd);
    return 0;
}

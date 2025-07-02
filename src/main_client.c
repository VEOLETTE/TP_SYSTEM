#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 2048

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    char algo[10];
    int user_id, nb;

    printf("Entrez l'ID utilisateur : ");
    scanf("%d", &user_id);
    printf("Choisissez l’algorithme (KNN ou MF ou GRAPH) : ");
    scanf("%s", algo);
    printf("Nombre de recommandations : ");
    scanf("%d", &nb);

    char request[BUFFER_SIZE];
    snprintf(request, BUFFER_SIZE, "user_id=%d;algo=%s;nb=%d;", user_id, algo, nb);

    send(sock, request, strlen(request), 0);
    read(sock, buffer, BUFFER_SIZE);
    printf("Réponse du serveur :\n%s\n", buffer);

    close(sock);
    return 0;
}

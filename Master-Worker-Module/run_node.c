#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

void run_as_leader() {
    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(1); }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); close(sockfd); exit(1);
    }

    if (listen(sockfd, 5) < 0) {
        perror("listen"); close(sockfd); exit(1);
    }

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (newsockfd < 0) { perror("accept"); continue; }

        char buffer[256];
        while (1) {
            ssize_t bytes_received = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0) break;
            buffer[bytes_received] = '\0';
            printf("%s", buffer);
        }

        close(newsockfd);
    }

    close(sockfd);
}

void run_as_worker(const char *leader_ip, const char *duco_user) {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(1); }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(leader_ip);
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect"); close(sockfd); exit(1);
    }

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "python3 miner.py -u %s", duco_user);

    FILE *fp = popen(cmd, "r");
    if (!fp) { perror("popen"); close(sockfd); exit(1); }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        if (send(sockfd, line, len, 0) < 0) break;
    }

    pclose(fp);
    close(sockfd);
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--leader") == 0) {
        run_as_leader();
    }
    else if (argc > 2 && strcmp(argv[1], "--worker") == 0) {
        run_as_worker(argv[2], argv[3]);
    }
    else {
        fprintf(stderr, "Usage: %s --leader | --worker <leader_ip> <duco_username>\n", argv[0]);
        exit(1);
    }
    return 0;
}

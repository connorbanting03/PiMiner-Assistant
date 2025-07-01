#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int run_node(int argc, char *argv[]){
    if (argc > 1 && strcmp(argv[1], "--leader") == 0){
        run_as_leader();
    }
    else if (argc > 2 && strcmp(argv[1], "--worker") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: %s --worker <leader_ip>\n", argv[0]);
            exit(1);
        }
        run_as_worker(argv[2]);
    } else {
        fprintf(stderr, "Usage: %s --leader | --worker <leader_ip>\n", argv[0]);
        exit(1);
    }

    return 0;
}

void run_as_leader() {
    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int port = 8080;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 5) < 0) {
        perror("listen");
        close(sockfd);
        exit(1);
    }

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (newsockfd < 0) {
            perror("accept");
            continue;
        }
        // Setup polling to recieve message string
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            perror("recv");
            close(newsockfd);
            continue;
        }
        prinft("Received %zd bytes\n", bytes_received);
        buffer[bytes_received] = '\0'; // Null-terminate the received string
        printf("Received message: %s\n", buffer);

        printf("Client connected\n");
        close(newsockfd);
    }

    close(sockfd);
}

void run_as_worker(int leader_ip){

    //Need to connect to leader 
    int sockfd;
    struct sockaddr_in server_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr(leader_ip);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        // Send a message to client

        char *message = "Hello from worker";
        while(1)
        {
            if (send(sockfd, message, strlen(message), 0) < 0) 
            {
            perror("send");
            }
            sleep(5);
        }
    
        close(sockfd);
        exit(1);
    }

    //send message
    

}
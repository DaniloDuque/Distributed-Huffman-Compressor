#include "util.h"

int handleSend() {
    
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct hostent * servidor = gethostbyname(SERVER_PUBLIC_IP);
    if (servidor == NULL) {
        fprintf(stderr, "Error: No se pudo resolver el hostname\n");
        return 1;
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    memcpy(&server_addr.sin_addr.s_addr, servidor->h_addr_list[0], 
            servidor->h_length);

    if (connect(server_socket, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1) {
        perror("Error al conectar al servidor");
        close(server_socket);
        return 1;
    }

    char *mensaje = "Hello!";

    if (send(server_socket, mensaje, strlen(mensaje), 0) == -1) {
        perror("Error al enviar el mensaje");
        close(server_socket);
        return 1;
    }

    printf("Mensaje enviado: %s\n", mensaje);

    close(server_socket);
    return 0;
}


int main(){
    handleSend();
    return  0;
}
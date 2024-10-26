#include "util.h"

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    FILE *fp;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Error al crear socket del cliente/trabajador");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "0.tcp.ngrok.io", &server_addr.sin_addr); 

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error al conectar al servidor central");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Conectado al servidor central\n");

    fp = fopen(FILE_PATH, "rb");
    if (fp != NULL) {
        while (1) {
            size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, fp);
            if (bytes_read == 0) break;
            send(sock, buffer, bytes_read, 0);
        }
        fclose(fp);
        printf("Archivo enviado al servidor central\n");
    }

    fp = fopen("received_file_from_server.txt", "wb");
    if (fp != NULL) {
        while (1) {
            int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) break;
            fwrite(buffer, 1, bytes_received, fp);
        }
        fclose(fp);
        printf("Archivo recibido del servidor central\n");
    }

    close(sock);
    return 0;
}
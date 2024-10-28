#include "util.h"
#include "FileChunker/FileChunker.h"
#include "RouteFinder.h"
#include "FileCompressor/FileCompressor.h"

int dto[MAX_SIZE*2] = {};
client_info info[MAX_CLIENT];
ll freqTable[MAX_SIZE];
mutex keys[MAX_SIZE];
mutex matex = PTHREAD_MUTEX_INITIALIZER;

int main() {

    int server_socket, client_socket, n, cont = 0;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    printf("Insert the number of client servers: ");
    scanf("%d", &n);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error while binding");
        exit(1);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Error while listening");
        exit(1);
    }

    printf("Central server listening port: %d...\n", PORT);
    while (cont<n) {
        info[cont].socket = accept(server_socket, (struct sockaddr*)&info[cont].client_addr, &info[cont].client_len);
        if (info[cont].socket == -1) {
            perror("Error in accept");
            continue;
        }
        info[cont].index = cont;
        printf("Client connected from %s:%d!\n", 
        inet_ntoa(info[cont].client_addr.sin_addr), 
        ntohs(info[cont].client_addr.sin_port));
        cont++;
    }

    if(splitFile(createSplitDTO(FILE_PATH,info,n))==false){
        perror("Error al dividir el archivo desde Main");
        return 1;
    }
    route ** routes = routeFinder(freqTable);
    for(int i = 0, j = 0; i < MAX_SIZE*2; i+=2, ++j){
        if(routes[j]!=NULL){
            dto[i] = routes[j]->len;
            dto[i+1] = routes[j]->msk;
        }
        else{
            dto[i]=0;
            dto[i+1]=0;
        }
    }
    if(compressFile(n,info) == false){
        perror("Error al crear los hilos desde main para comprimir");
        return 1;
    }
    
    //  New area

    FILE * fileC = fopen(PATH_FOR_COMPRESS,"wb");
    uchar byte=0;
    int bit=7;
    char buffer[BUFFER_SIZE];
    char cbuffer[BUFFER_SIZE];
    for(int i = 0; i < n; i++){
        char ruta[29];
        if(snprintf(ruta,sizeof(ruta),"%s%d",SAVED_FILE_ROUTE,i) >= sizeof(ruta)){
            printf("Error al calcular la ruta de %d\n",i);
            return 1;
        }
        FILE * file = fopen(ruta,"rb");
        
        size_t bytes_read;
    
        fseek(file, 0, SEEK_END);
        ll file_size = ftell(file) - 2;
        fseek(file, 0, SEEK_SET);        
        
        ll pos;
        while (file_size > 0) {
            pos=0;
            size_t bytes_to_read = (file_size < BUFFER_SIZE) ? file_size : BUFFER_SIZE;
            bytes_read = fread(buffer, 1, bytes_to_read, file);
            for(ll i2=0; i2<bytes_read; i2++){
                for(int bt=7; bt>=0; bt--){
                    byte|=((buffer[i2]&(1<<bt))<<bit);
                    bit--;
                    if(bit==-1) cbuffer[pos++]=byte, byte=0, bit=7;
                }
            }
            fwrite(cbuffer, 1, pos, fileC);
            file_size -= bytes_read;
        }

        fseek(file,-2,SEEK_END);
        uchar ceros, cantCeros;
        fread(&ceros,sizeof(uchar),1,file); 
        fread(&cantCeros,sizeof(uchar),1,file);

        for(int j = 7; cantCeros; j--, cantCeros--){
            byte |= (1 << bit) & (ceros&(1<<j));
            bit--;
            if(bit == -1){
                fwrite(&byte,1,1,fileC);
                byte=0;
                bit = 7;
            }
        }

        fclose(file);

        if(remove(ruta) != 0){
            fprintf(stderr,"Error al borrar el archivo %d\n",i);
        }
    }
    buffer[0]=byte;
    buffer[1]=(bit+1)%8;
    fwrite(buffer, 1, 2, fileC);
    
    fclose(fileC);
    printf("Se comprimio el archivo con exito\n");
    close(server_socket);
    return 0;
}
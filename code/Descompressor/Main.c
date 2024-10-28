#include "util.h"

int main(int argc, char const *argv[]) {
    FILE* huffman_table = fopen(DESCOMPRESS_TABLE, "rb");
    if (huffman_table == NULL) {
        perror("Error opening the huffman table file");
        return 1;
    }

    node* root = createNode('\0');

    for (int i = 0; i < 256; i++) {
        fscanf(huffman_table, "%d %d\n", &lenTable[i], &pathTable[i]);
        if (lenTable[i]) {
            int reversed_path = pathTable[i];
            printf("Inserting symbol %c with length %d and path %d (reversed: %d)\n", i, lenTable[i], pathTable[i], reversed_path);
            insert(0, lenTable[i], reversed_path, i, root);
        }
    }
    displayTree(root, "", 0);
    fclose(huffman_table);

    // Apertura del archivo comprimido
    FILE* fileRead = fopen(COMPRESSED_FILE, "rb");
    if (fileRead == NULL) {
        perror("Error opening the compressed file");
        return 1;
    }

    // Apertura del archivo para descompresión
    FILE* fileWrite = fopen(PATH_FOR_DESCOMPRESS, "wb");
    if (fileWrite == NULL) {
        perror("Error opening the decompressed file");
        fclose(fileRead);
        return 1;
    }

    // Lectura del tamaño del archivo comprimido
    fseek(fileRead, 0, SEEK_END);
    ll file_size = ftell(fileRead) - 2;
    fseek(fileRead, 0, SEEK_SET);

    ll pos;
    node* path = root;
    uchar buffer[BUFFER_SIZE];
    uchar wbuffer[BUFFER_SIZE];

    // Verifica que el root tenga al menos un hijo antes de iniciar
    if (root == NULL || (root->left == NULL && root->right == NULL)) {
        perror("Invalid Huffman tree");
        fclose(fileRead);
        fclose(fileWrite);
        return 1;
    }

    while (file_size > 0) {
        pos = 0;
        size_t bytes_to_read = (file_size < BUFFER_SIZE) ? file_size : BUFFER_SIZE;
        size_t bytes_read = fread(buffer, 1, bytes_to_read, fileRead);

        for (ll i = 0; i < bytes_read; i++) {
            uchar byteRead = buffer[i];
            for (int j = 7; j >= 0; j--) {
                printf("Processing bit...\n");

                // Verifica que path no sea NULL antes de acceder
                if (path == NULL) {
                    perror("Path node is null");
                    fclose(fileRead);
                    fclose(fileWrite);
                    return 1;
                }

                // Recorrido del árbol según el bit leído
                if (TEST(byteRead, j)) {
                    if (path->right == NULL) {
                        perror("Right child is null during traversal");
                        fclose(fileRead);
                        fclose(fileWrite);
                        return 1;
                    }
                    path = path->right;
                } else {
                    if (path->left == NULL) {
                        perror("Left child is null during traversal");
                        fclose(fileRead);
                        fclose(fileWrite);
                        return 1;
                    }
                    path = path->left;
                }

                printf("Symbol: %c\n", path->sym);

                // Si se llega a una hoja, se escribe el símbolo
                if (path->left == NULL && path->right == NULL) {
                    wbuffer[pos] = path->sym;
                    path = root; // Reinicia el recorrido desde la raíz
                    pos++;
                }
            }
        }

        fwrite(wbuffer, 1, pos, fileWrite);
        file_size -= bytes_read;
    }

    // Manejo del padding
    uchar padding = 0, totalPadding = 0;
    int hasPadding = 0;
    fseek(fileRead, -2, SEEK_END);
    fread(&padding, sizeof(uchar), 1, fileRead); 
    fread(&totalPadding, sizeof(uchar), 1, fileRead);
    fclose(fileRead);

    if (totalPadding) hasPadding = 1;
    if (!hasPadding && root != path) {
        perror("Inconsistent file");
        fclose(fileWrite);
        return 1;
    }

    // Manejo de bits de padding
    for (int j = 7; j >= 0 && totalPadding; j--, totalPadding--) {
        if (path == NULL) {
            perror("Path node is null in padding");
            fclose(fileWrite);
            return 1;
        }

        if (TEST(padding, j)) {
            if (path->right == NULL) {
                perror("Right child is null during padding");
                fclose(fileWrite);
                return 1;
            }
            path = path->right;
        } else {
            if (path->left == NULL) {
                perror("Left child is null during padding");
                fclose(fileWrite);
                return 1;
            }
            path = path->left;
        }
    }

    // Si se llega a una hoja, se escribe el último símbolo del padding
    if (path->left == NULL && path->right == NULL) {
        padding = path->sym;
        fwrite(&padding, 1, 1, fileWrite);
        printf("Padding symbol: %u\n", padding);
    } else {
        perror("Inconsistent padding");
        fclose(fileWrite);
        return 1;
    }

    fclose(fileWrite);
    printf("File decompressed successfully\n");
    return 0;
}
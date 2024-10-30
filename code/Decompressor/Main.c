#include "util.h"

int main(int argc, char const *argv[]) {
    FILE* huffman_table = fopen(DESCOMPRESS_TABLE, "rb");
    if (huffman_table == NULL) {
        perror("Error opening the Huffman table file");
        return 1;
    }

    node* root = createNode('\0');
    ll greatest=0;
    for (int i = 0; i < 256; i++) {
        fscanf(huffman_table, "%d %d\n", &lenTable[i], &pathTable[i]);
        if (lenTable[i]) {
            if(pathTable[i] > greatest) greatest = pathTable[i];
            insert(0, lenTable[i], pathTable[i], i, root);
        }
    }
    //displayTree(root, "", 0);
    fclose(huffman_table);
    printf("Largest path %lld\n", greatest);

    FILE* fileRead = fopen(COMPRESSED_FILE, "rb");
    if (fileRead == NULL) {
        perror("Error opening the compressed file");
        return 1;
    }

    FILE* fileWrite = fopen(PATH_FOR_DESCOMPRESS, "wb");
    if (fileWrite == NULL) {
        perror("Error opening the decompressed file");
        fclose(fileRead);
        return 1;
    }

    fseek(fileRead, 0, SEEK_END);
    ll file_size = ftell(fileRead) - 2;
    fseek(fileRead, 0, SEEK_SET);

    ll pos;
    node* path = root;
    uchar buffer[BUFFER_SIZE];
    uchar wbuffer[BUFFER_SIZE];
    if (file_size < 0) {
        perror("Error in the size of the file to decompress");
        fclose(fileRead);
        fclose(fileWrite);
        return 1;
    }

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

                if (path == NULL) {
                    perror("Path node is null");
                    fclose(fileRead);
                    fclose(fileWrite);
                    return 1;
                }

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

                if (path->left == NULL && path->right == NULL) {
                    wbuffer[pos] = path->sym;
                    path = root; 
                    pos++;
                }
            }
        }

        fwrite(wbuffer, 1, pos, fileWrite);
        file_size -= bytes_read;
    }

    uchar padding = 0, totalPadding = 0;
    fseek(fileRead, -2, SEEK_END);
    fread(&padding, sizeof(uchar), 1, fileRead); 
    fread(&totalPadding, sizeof(uchar), 1, fileRead);
    fclose(fileRead);

    pos = 0;
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
        if (path->left == NULL && path->right == NULL) {
            wbuffer[pos] = path->sym;
            pos++;
            path = root;
        }
    }
    if (path != root) {
        perror("Error inconsistent file");
        fclose(fileWrite);
        return 1;
    }
    if (pos) fwrite(wbuffer, 1, pos, fileWrite);

    fclose(fileWrite);
    printf("File decompressed successfully\n");
    return 0;
}

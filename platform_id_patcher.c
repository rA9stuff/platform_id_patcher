#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void patch_macho_signature(const char *filename) {
    FILE *file = fopen(filename, "r+b");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    unsigned char *buffer = malloc(fileSize);
    if (!buffer) {
        perror("Memory allocation error");
        fclose(file);
        return;
    }

    size_t bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead != fileSize) {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return;
    }

    // Signature to search for: 320000002000000001000000 (hex)
    unsigned char signature[]        = {0x32, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
    unsigned char patchedSignature[] = {0x32, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};

    unsigned char *found = memmem(buffer, fileSize, signature, sizeof(signature));
    if (!found) {
        printf("Signature not found\n");
        free(buffer);
        fclose(file);
        return;
    }

    long offset = found - buffer;
    printf("Signature found at offset: %ld\n", offset);

    memcpy(found, patchedSignature, sizeof(patchedSignature));
    printf("Patched the signature\n");

    fseek(file, 0, SEEK_SET);
    fwrite(buffer, 1, fileSize, file);

    free(buffer);
    fclose(file);
    printf("Successfully patched the Mach-O file\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path_to_macho_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    patch_macho_signature(argv[1]);
    return EXIT_SUCCESS;
}

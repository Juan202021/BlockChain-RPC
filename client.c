#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include "blockchain.h"
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <stdbool.h>
#include <string.h>

char *sha256_hex(const unsigned char *data, size_t len) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    if (!SHA256(data, len, hash)) return NULL;

    // 2 chars por byte + 1 para '\0'
    char *hex = malloc(SHA256_DIGEST_LENGTH * 2 + 1);
    if (!hex) return NULL;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        sprintf(hex + i*2, "%02x", hash[i]);
    hex[SHA256_DIGEST_LENGTH*2] = '\0';
    return hex;
}

unsigned long iterador(long cant_ceros, char *texto_original, unsigned long inicio, unsigned long final) {
    if (!texto_original) {
        return -1; 
    }

    char buffer[500];
    
    for (unsigned long i = inicio; i <= final; i++) {
        // Crear string: texto_original + número
        snprintf(buffer, sizeof(buffer), "%s%ld", texto_original, i);
        
        char *hash = sha256_hex((unsigned char*)buffer, strlen(buffer));
        if (!hash) continue;
        
        // Verificar si el hash tiene la cantidad requerida de ceros al final
        bool found = true;
        long hash_len = strlen(hash);
        
        // Verificar que hay suficientes caracteres
        if (hash_len < cant_ceros) {
            free(hash);
            continue;
        }
        
        // Verificar los últimos 'cant_ceros' caracteres
        for (long j = 0; j < cant_ceros; j++) {
            if (hash[hash_len - 1 - j] != '0') {
                found = false;
                break;
            }
        }
        
        if (found) {
            printf("Hash encontrado: %s\n", hash);
            free(hash);
            return i; // Número encontrado
        }
        
        free(hash);
    }
    return -1; // No se encontró
}

int main(int argc, char **argv) {
    CLIENT *cl;
    tipo_salida *outp;
    resultado_hash resultado;

    if (argc != 2) {
        fprintf(stderr, "Error: uso incorrecto.\n");
        fprintf(stderr, "Uso: %s <host>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    cl = clnt_create(argv[1], BLOCKCHAIN_PROGRAM, BLOCKCHAIN_VERSION, "tcp");
    if (cl == NULL) {
        fprintf(stderr, "Error al crear cliente: %s\n", clnt_spcreateerror(argv[1]));
        exit(EXIT_FAILURE);
    }

    if ((outp = blockchainprocedure_1(NULL, cl)) == NULL) {
        fprintf(stderr, "Error en llamada RPC: %s\n", clnt_sperror(cl, argv[1]));
        exit(EXIT_FAILURE);
    } 

    if (strcmp(outp->texto, "-1") == 0) {
        printf("Se alcanzó el número máximo de workers permitidos por el servidor.\n");
        clnt_destroy(cl);
        return 0;
    }

    printf("Texto: '%s'\n", outp->texto);

    unsigned long relleno = iterador(outp->cant_ceros, outp->texto, outp->inicio, outp->final);

    if (relleno != -1) {
        printf("\nSe encontró un número en el rango %ld-%ld que genera un hash con %ld ceros al final: %ld\n", 
               outp->inicio, outp->final,outp->cant_ceros, relleno);
        resultado.encontrado = true;
        resultado.relleno = relleno;
    } else {
        printf("\nNo se encontró ningún número en el rango %ld-%ld que genere un hash con %ld ceros al final.\n", 
               outp->inicio, outp->final, outp->cant_ceros);
        resultado.encontrado = false;     
    }
    void *resp = enviar_resultado_1(&resultado, cl);

    clnt_destroy(cl);
    return 0;
}
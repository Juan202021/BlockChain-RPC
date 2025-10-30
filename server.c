#include "rpc/rpc.h"
#include "blockchain.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <math.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#define FILE_NAME "texto.txt"

// Variables globales para configuración del servidor
static long cant_ceros = 4;
static long cant_workers = 5;
static unsigned long cant_clientes_actuales = 0;
static unsigned long cant_por_worker = 0;
static unsigned long valor_actual = 0;
static int relleno_encontrado = 0;

char *read_text_file() {
    if (FILE_NAME == NULL) return NULL;
    FILE *f = fopen(FILE_NAME, "rb");
    if (!f) return NULL;
    if (fseek(f, 0, SEEK_END) != 0) { 
        fclose(f); return NULL; 
    }
    long sz = ftell(f);
    if (sz < 0) { 
        fclose(f); return NULL; 
    }
    rewind(f);
    char *buf = (char *)malloc((size_t)sz + 1);
    if (!buf) { 
        fclose(f); return NULL; 
    }
    size_t read = fread(buf, 1, (size_t)sz, f);
    buf[read] = '\0';
    fclose(f);
    return buf;
}

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

tipo_salida * blockchainprocedure_1_svc(void *argp, struct svc_req *rqstp) {
    static tipo_salida salida;
    static int initialized = 0;

    if (cant_clientes_actuales >= cant_workers) {
        salida.texto = "-1";
        return &salida; // Máximo de clientes alcanzado
    } else if (relleno_encontrado) {
        salida.texto = "-2";
        return &salida; // Relleno ya encontrado
    }

    // Inicializar una vez
    if (!initialized) {
        cant_por_worker = pow(10, cant_ceros) / cant_workers;
        initialized = 1;
    }

    cant_clientes_actuales++;
    printf("\nWorkers activos: %lu\n", cant_clientes_actuales);

    /* leer el archivo y copiar a buffer estático */
    char *texto = read_text_file();
    static char texto_buf[256];
    if (texto == NULL) {
        strncpy(texto_buf, "Error al leer el archivo.", sizeof(texto_buf)-1);
        texto_buf[sizeof(texto_buf)-1] = '\0';
    } else {
        strncpy(texto_buf, texto, sizeof(texto_buf)-1);
        texto_buf[sizeof(texto_buf)-1] = '\0';
    }

    salida.texto = texto_buf;
    salida.cant_ceros = cant_ceros;
    salida.inicio = valor_actual;
    salida.final = valor_actual + cant_por_worker - 1;
    valor_actual += cant_por_worker;

    if (texto) free(texto);

    return &salida;
}

void * enviar_resultado_1_svc(resultado_hash *argp, struct svc_req *rqstp) {
    static int r;
    char buffer[300];

    /* leer el archivo y copiar a buffer estático */
    char *texto = read_text_file();
    static char texto_buf[256];
    if (texto == NULL) {
        strncpy(texto_buf, "Error al leer el archivo.", sizeof(texto_buf)-1);
        texto_buf[sizeof(texto_buf)-1] = '\0';
    } else {
        strncpy(texto_buf, texto, sizeof(texto_buf)-1);
        texto_buf[sizeof(texto_buf)-1] = '\0';
        free(texto); 
    }
    
    printf("\nCliente reportó resultado:\n");
    printf("  Relleno Encontrado: %s\n", argp->encontrado ? "Sí" : "No");
    if (argp->encontrado){
        relleno_encontrado = 1; // Marca que ya se encontró el relleno
        printf("  Relleno: %lu\n", argp->relleno);
        
        snprintf(buffer, sizeof(buffer), "%s%lu", texto_buf, argp->relleno);
        char *hash_resultado = sha256_hex((unsigned char*)buffer, strlen(buffer));
        if (hash_resultado) {
            printf("  Hash: %s\n", hash_resultado);
            free(hash_resultado);  // Liberar memoria del hash
        } else {
            printf("  Hash: Error al calcular\n");
        }
    }

    cant_clientes_actuales--;
    printf("\nWorkers activos: %lu\n", cant_clientes_actuales);
    
    return (void *)&r;
}
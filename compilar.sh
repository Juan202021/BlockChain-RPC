#!/bin/bash

# Cambiar al directorio del proyecto
cd "/home/gon/JAristizabal/U/TrabajosU/Sistemas distribuidos/c/BlockChain-RPC"

# Generar los archivos RPC
echo "Generando archivos RPC..."
rpcgen blockchain.x

# Verificar que se generaron los archivos necesarios
if [ ! -f blockchain_clnt.c ]; then
    echo "Error: blockchain_clnt.c no fue generado"
    exit 1
fi

if [ ! -f blockchain_svc.c ]; then
    echo "Error: blockchain_svc.c no fue generado"
    exit 1
fi

# Compilar el servidor
echo "Compilando servidor..."
gcc -I. -I/usr/include/tirpc -o server server.c blockchain_svc.c blockchain_xdr.c -ltirpc -lm -lssl -lcrypto

if [ $? -ne 0 ]; then
    echo "Error al compilar el servidor"
    exit 1
fi

# Compilar el cliente
echo "Compilando cliente..."
gcc -I. -I/usr/include/tirpc -o client client.c blockchain_clnt.c blockchain_xdr.c -ltirpc -lssl -lcrypto

if [ $? -ne 0 ]; then
    echo "Error al compilar el cliente"
    exit 1
fi

echo "Compilación exitosa!"
echo "Ejecutables generados:"
echo "  - server"
echo "  - client"
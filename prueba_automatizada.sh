#!/bin/bash

# Script para probar las conexiones de los workers

NUM_WORKERS=${1:-5}
SERVER_HOST="localhost"

echo -e "\nPrueba con $NUM_WORKERS workers conectándose al servidor en $SERVER_HOST"

# Limpiar logs anteriores
rm -f worker_*.log

# Lanzar workers
pids=()
for i in $(seq 1 $NUM_WORKERS); do
    (
        ./client $SERVER_HOST
    ) > "worker_${i}.log" 2>&1 &
    pids+=($!)
done

# Esperar a todos
for pid in "${pids[@]}"; do
    wait $pid
done

echo -e "\nRESULTADOS"

# Analizar resultados
encontraron=$(grep -h "Se encontró un número" worker_*.log | wc -l)
no_encontraron=$(grep -h "No se encontró ningún número" worker_*.log | wc -l)
max_workers=$(grep -h "máximo de workers" worker_*.log | wc -l)

echo "Workers que encontraron hash: $encontraron"
echo "Workers sin resultado: $no_encontraron"
echo "Workers rechazados (máximo alcanzado): $max_workers"
echo ""

echo "Hashes encontrados:"
grep -h "Hash encontrado:" worker_*.log | nl

echo -e "\nLos logs individuales se encuentran en: worker_#.log"
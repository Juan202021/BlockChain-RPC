# BlockChain-RPC
Este es un programa que emplea el protocolo RPC para simular lo que sería una BlockChain. Este proyecto es con fines académicos y educativos.

## Requisitos previos

Para ejecutar este programa necesitas tener instalado:
- Compilador GCC
- Librería RPC (tirpc)
- OpenSSL y sus librerías de desarrollo
- make (opcional)

En sistemas basados en Debian/Ubuntu, puedes instalar las dependencias con:
```bash
sudo apt-get install gcc libtirpc-dev libssl-dev make
```

## Compilación

Para compilar el programa, ejecuta el script de compilación:

```
./compilar.sh
```

Este script generará dos ejecutables:
- `server`: El servidor RPC que gestiona la blockchain
- `client`: El cliente que se conecta al servidor para realizar operaciones

## Uso

1. Primero, inicia el servidor en una terminal:
```
./server
```

2. En otra terminal, ejecuta el cliente especificando la dirección IP del servidor:
```
./client localhost
```
Si el servidor está en otra máquina, reemplaza `localhost` con la IP correspondiente.

### Funcionamiento

- El servidor acepta hasta 5 workers (clientes) simultáneos.
- El programa busca un número (relleno) que, al concatenarlo con el texto del archivo `texto.txt`, genera un hash SHA-256 que termina en una cantidad específica de ceros.
- El servidor divide el trabajo entre los clientes conectados.
- Cada cliente procesa un rango específico de números y reporta si encuentra una solución.

### Configuración

El servidor tiene las siguientes configuraciones predeterminadas:
- Cantidad de ceros requeridos al final del hash: 4
- Número máximo de workers: 5

### Estructura de archivos
- `blockchain.x`: Definición de la interfaz RPC
- `server.c`: Implementación del servidor
- `client.c`: Implementación del cliente
- `texto.txt`: Archivo con el texto base para generar los hashes
- `compilar.sh`: Script para compilar el proyecto


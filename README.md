# BlockChain-RPC
Este es un programa que emplea el protocolo RPC para simular lo que sería una BlockChain. Este proyecto es con fines académicos y educativos.

Elaborado por: J. Aristizabal - 160004903

## Requisitos previos

Para ejecutar este programa necesitas tener instalado:
- Compilador GCC
- Librería RPC (tirpc)
- OpenSSL y sus librerías de desarrollo
- make (opcional)
- rpcgen (generador de código RPC)

En sistemas basados en Debian/Ubuntu, puedes instalar las dependencias con:
```
sudo apt-get install gcc libtirpc-dev libssl-dev make rpcbind
```

Nota: El paquete `rpcbind` incluye la herramienta `rpcgen` necesaria para generar los archivos RPC.

## Compilación

Nota: Inicialmente ya se encuentran compilados los archivos necesarios para usarlo, si desea probarlo se puede saltar directamente al apartado de `uso`.

Para compilar el programa, ejecuta el script de compilación:

```
./compilar.sh
```

Este script generará dos ejecutables:
- `server`: El servidor RPC que gestiona la blockchain
- `client`: El cliente que se conecta al servidor para realizar operaciones

Además de los archivos generados automáticamente por rpcgen a partir de `blockchain.x` son:
- `blockchain.h`: Archivo de cabecera con definiciones compartidas
- `blockchain_xdr.c`: Funciones XDR para serialización de datos
- `blockchain_clnt.c`: Código del lado del cliente RPC
- `blockchain_svc.c`: Código del lado del servidor RPC

## Uso

Para la ejecución de cada uno de los siguientes ejecutables, debe de otorgarles permiso de ejecución.

1. Primero, inicia el servidor en una terminal:
```
./server
```

2. En otra terminal, ejecuta el cliente especificando la dirección IP del servidor:
```
./client localhost
```
Si el servidor está en otra máquina, reemplaza `localhost` con la IP correspondiente.

3. Una alternativa para probar simultaneamente la conexión de varios clientes (workers) al servidor, es usar el script `prueba_automatizada.sh` así:
```
./prueba_automatizada.sh <cantidad_clientes>
```

Nota: `<cantidad_clientes>` es un parametro opcional que indica la cantidad de clientes con la cual desea realizar la prueba. Por defecto la realiza con 5 clientes.


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
- `prueba_automatizada.sh`: Script para realizar prueba de conexiones simultáneas entre varios clientes (workers) y el servidor


/*Archivo de definicion de RPC - Remote Procedure Call*/

struct tipo_salida {
    long cant_ceros;
    string texto<256>;
    unsigned long inicio;
    unsigned long final;
};

struct resultado_hash {
    unsigned long relleno;
    int encontrado;   /* 1 si encontró, 0 si no */
};

program BLOCKCHAIN_PROGRAM {
    version BLOCKCHAIN_VERSION {
        tipo_salida BLOCKCHAINPROCEDURE()=1;
        void ENVIAR_RESULTADO(resultado_hash)=2; 
    }=1;
}=0x32130000;

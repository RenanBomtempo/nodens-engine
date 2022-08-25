/*  ========== Constants.h =========
    Encarregado de guardar todas as constantes
    importantes utilizadas, tanto as computacionais
    quanto as matematicas
*/

// Definir somente uma vez este arquivo header
#ifndef CONSTANTS_H
#define CONSTANTS_H

    // Constantes matematicas -----------------------------------------
    //#define PI   3.14159265358979323846264338327950288419716939937510

    // Constantes fisicas ---------------------------------------------
    #define R    287 // J / (kg  K)
    #define GAMMA   1.4  // razao de calores especificos
    #define PRESSAO_LIVRE    101325.0 // N / m^2
    #define TEMPERATURA_LIVRE   288.16 // K
    #define TEMPERATURA_PAREDE   288.16 // K
    #define VEL_SOM   340.28 // m / s
    #define MI_ZERO   1.7894e-5   // kg / (m s)
    #define PRANDTL   0.71 // a-dimensional
    #define LHORI   0.00001 // m
    #define DELTA_T   3.0e-11 // s

    //Constantes computacionais ---------------------------------------
    // Tamanho geometrico da tela
    #define LARGURA_TELA 800
    #define ALTURA_TELA 600

    // Tamanho geometrico da malha
    #define LARGURA_MALHA 200
    #define ALTURA_MALHA 200

#endif

/*  ========== Constants.h =========
    Encarregado de guardar todas as constantes
    importantes utilizadas, tanto as computacionais
    quanto as matematicas
*/

// Definir somente uma vez este arquivo header
#ifndef CONSTANTS_H
#define CONSTANTS_H

    // Macros uteis
    static double argAoQuadrado_constants_h;
    #define AO_QUADRADO(a)   ((argAoQuadrado_constants_h=(a)) == 0.0 ? 0.0 : (argAoQuadrado_constants_h * argAoQuadrado_constants_h) )

    // Constantes matematicas -----------------------------------------
    //#define PI      3.14159265358979323846264338327950288419716939937510
    #define COS30           0.866025403  // rad
    #define SEN30           0.5          // rad

    // Constantes fisicas ---------------------------------------------
    #define R                       287.0 //          J / (kg  K)
    #define GAMMA                   1.4 //          razao de calores especificos
    #define PRESSAO_LIVRE           101325.0 //     N / m^2
    #define TEMPERATURA_LIVRE       288.16 //       K
    #define TEMPERATURA_PAREDE      288.16 //       K
    #define VEL_SOM                 340.28 //       m / s
    #define MI_ZERO                 1.7894e-5 //    kg / (m s)
    #define PRANDTL                 0.71 //         a-dimensional
    #define LHORI                   0.00001 //      m
    #define DELTA_T                 3.0e-11 //      s
    #define VEL_PAREDE              0.0 //          m / s 
    #define VEL_LIVRE               0.5 * VEL_SOM  //         m / s

    //Constantes computacionais ---------------------------------------
    // Identificadores de variaveis fisicas
    #define ID_VAR_RHO          1
    #define ID_VAR_U            2
    #define ID_VAR_V            3
    #define ID_VAR_Et           4
    #define ID_VAR_T            5
    #define ID_VAR_P            6
    #define ID_VAR_MI           7
    #define ID_VAR_K            8
    #define ID_VAR_PLINHA       9
    // Identificadores de fronteira
    #define NAO_FRONTEIRA                                   1
    #define FRONTEIRA_NORTE                                 2  
    #define FRONTEIRA_SUL                                   3
    #define FRONTEIRA_LESTE                                 4
    #define FRONTEIRA_OESTE                                 5
    #define CONDICAO_FRONTEIRA_PRESCRITA                    6
    #define CONDICAO_FRONTEIRA_EXTRAPOLADA                  7
    #define CONDICAO_FRONTEIRA_ENTRADA_MASSA                8
    #define CONDICAO_FRONTEIRA_SAIDA_MASSA                  9
    // Cotas
    #define COTA_PRESSAO                        0.05
    #define COTA_VELOCIDADES                    0.05
    #define COTA_TEMPERATURA                    0.05
    #define COTA_GRADCONJUGADO                  1e-15
    #define LIMITE_ITERACOES_GRADBICONJUGADO    10000
    #define RELAXAMENTO_P                       0.00001
    #define RELAXAMENTO_U                       0.01
    #define RELAXAMENTO_V                       0.01
    #define RELAXAMENTO_T                       0.01
    #define MAXITER_A                           4
    #define MAXITER_B                           4
    #define MAXITER_C                           20

    // Tamanho geometrico da tela
    #define LARGURA_TELA                 800
    #define ALTURA_TELA                  600

    // Visualizacao
    #define PROFUNDIDADE_Z_PADRAO               10
    #define INCREMENTO_ZOOM                     10000.0
    #define MAXITER_GRAFICO_RESIDUOS            100
    #define MAIOR_RESIDUO_INICIAL               0.0

#endif

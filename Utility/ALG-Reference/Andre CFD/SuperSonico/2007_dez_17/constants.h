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
    // #define PI      3.14159265358979323846264338327950288419716939937510
    #define COS30                   0.866025403     // rad
    #define SEN30                   0.5             // rad

    // Constantes fisicas ---------------------------------------------
    #define R                       287.0           // J / (kg  K)
    #define GAMMA                   1.4             // razao de calores especificos
    #define PRESSAO_LIVRE           101325.0        // N / m^2
    #define TEMPERATURA_LIVRE       288.16          // K
    #define TEMPERATURA_PAREDE      288.16          // K
    #define REYNOLDS_SUPERSONICO    900.0           // adimensional
    #define VEL_SOM                 340.28          // m / s
    #define MI_ZERO                 1.7894e-5       // kg / (m s)
    #define PRANDTL                 0.71            // adimensional
    #define LHORI                   0.00001         // m
    #define DELTA_T                 3.0e-11         // s
    #define VEL_PAREDE              0.0             // m / s
    #define VEL_LIVRE               (4.0 * VEL_SOM) // m / s - Mach 4
    #define C_P                     ( GAMMA * (R / (GAMMA-1.0)) )

    //Constantes computacionais ---------------------------------------
    // Identificadores de variaveis fisicas
    #define ID_VAR_RHO              1
    #define ID_VAR_U                2
    #define ID_VAR_V                3
    #define ID_VAR_T                4
    #define ID_VAR_P                5
    #define ID_VAR_MI               6
    #define ID_VAR_K                7
    #define ID_VAR_PLINHA           8

    // Identificadores de variaveis auxiliares
    #define ID_VAR_TERMO_FONTE_U        9
    #define ID_VAR_TERMO_FONTE_V        10
    #define ID_VAR_A_P_X                11
    #define ID_VAR_A_P_Y                12
    #define ID_VAR_SOMATORIO_U_VIZINHOS 13
    #define ID_VAR_SOMATORIO_V_VIZINHOS 14
    #define ID_VAR_MASSA_ANTERIOR       15
    #define ID_VAR_U_ITERANTERIOR       16
    #define ID_VAR_V_ITERANTERIOR       17

    // Identificadores de fronteira
    #define NAO_FRONTEIRA                       1
    #define FRONTEIRA_NORTE                     2
    #define FRONTEIRA_SUL                       3
    #define FRONTEIRA_LESTE                     4
    #define FRONTEIRA_OESTE                     5
    #define CONDICAO_FRONTEIRA_PRESCRITA        6
    #define CONDICAO_FRONTEIRA_EXTRAPOLADA      7
    #define CONDICAO_FRONTEIRA_ENTRADA_MASSA    8
    #define CONDICAO_FRONTEIRA_SAIDA_MASSA      9
    #define ALEM_FRONTEIRA                      10

    // Identificadores de graficos
    #define ID_GRAFICO_DENS         1
    #define ID_GRAFICO_VELO         2
    #define ID_GRAFICO_TEMP         3
    #define ID_GRAFICO_PRES         4
    #define ID_GRAFICO_RESI         5
    #define ID_GRAFICO_GRIDPOS      6
    #define ID_GRAFICO_MACH         7

    // Cotas
    #define ITERACOES_FINAL                     10000 // iteracoes do tempo
    #define COTA_PRESSAO                        0.3
    #define COTA_VELOCIDADE_U                   0.6
    #define COTA_VELOCIDADE_V                   0.9
    #define COTA_TEMPERATURA                    0.3
    #define COTA_GRADCONJUGADO                  5e-4
    #define COTA_GRADCONJUGADO_TEMPERATURA      2e-8
    #define COTA_GRADCONJUGADO_PRESSAO          1e-8
    #define LIMITE_ITERACOES_GRADBICONJUGADO    8000
    #define RELAXAMENTO_P                       0.6
    #define RELAXAMENTO_U                       0.2
    #define RELAXAMENTO_V                       0.000001
    #define RELAXAMENTO_T                       1.0
    #define MAXITER_A                           30
    #define MAXITER_B                           700
    #define MAXITER_C                           20
    #define COTA_RESIDUO_PARA_DIMINUIR_DELTAT   4e-4
    #define COTA_RESIDUO_PARA_AUMENTAR_DELTAT   4e-9
    #define REFINAMENTO_MAX                     7
    #define REFINAMENTO_MIN                     4
    #define COTA_REFINAMENTO                    0.01
    #define COTA_DESREFINAMENTO                 0.0
    // Obs: COTA_REFINAMENTO > COTA_DESREFINAMENTO

    // Tamanho geometrico da tela
    #define LARGURA_TELA            800
    #define ALTURA_TELA             600

    // Visualizacao
    #define PROFUNDIDADE_Z_PADRAO   10
    #define INCREMENTO_ZOOM         10000.0

    // Graficos
    #define MAXITER_GRAFICO_RESIDUOS            100
    #define MAIOR_RESIDUO_INICIAL               0.0
    #define MENOR_RESIDUO_INICIAL               1e-6
    #define LIMITE_INFERIOR_RESIDUO             1e-13
    #define LEGENDA_LIM_SUP_DENSIDADE           2.2
    #define LEGENDA_LIM_INF_DENSIDADE           0.9
    #define LEGENDA_LIM_SUP_TEMPERATURA         480.0
    #define LEGENDA_LIM_INF_TEMPERATURA         250.0
    #define LEGENDA_LIM_SUP_PRESSAO             2.0 * PRESSAO_LIVRE
    #define LEGENDA_LIM_INF_PRESSAO             0.55 * PRESSAO_LIVRE
    #define LEGENDA_LIM_SUP_MACH                4.1
    #define LEGENDA_LIM_INF_MACH                0.0

    // Cores dos graficos
    #define COR_LIM_SUP                         (1.0, 0.0, 0.75)    // rosa
    #define COR_LIM_INF                         (0.75, 0.0, 1.0)    // preto
    #define COR_VETORES                         (0.0, 0.0, 0.3)     // azul escuro

    // Sistema operacional usado para compilar:
    //#define USANDO_LINUX                        "true"
    #define USANDO_WINDOWS                      "true"
    // Obs.: somente um dos acima pode estar definido.
#endif

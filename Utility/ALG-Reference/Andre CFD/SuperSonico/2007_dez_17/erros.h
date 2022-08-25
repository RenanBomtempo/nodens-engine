//erros.h
// Contem definicoes de codigos para representar erros

//Criado por Andre STALTZ dia 31/03/07
  
#ifndef ERROS_H
#define ERROS_H

    // Erros genericos ==================================================================
    #define SUCESSO                     0
    #define ERRO_FLUXO_EXECUCAO         1  // erro de programacao, pois o fluxo
                                           // de execucao nao deveria atingi'-lo
    #define INSUCESSO                   2

    // Erros de malha ===================================================================
    #define ERRO_MALHA_VAZIA            3
    #define ERRO_CELULA_INEXISTENTE     4

    // Erros de celula e resolucao do problema ==========================================
    #define ERRO_ID_VAR_INEXISTENTE     5
    #define ERRO_ELEMENT_NULO           6

    // Macros para impressoes de erro no terminal =======================================
    #ifndef IOSTREAM__INCLUIDO
    #define IOSTREAM__INCLUIDO
        #include <iostream>
            using std::cout;
            using std::endl;
            using std::ios;
            using std::cin;
    #endif // IOSTREAM_INCLUIDO

    #define IMPRIMIR_ERRO_FLUXO_EXECUCAO()  ({cout << "Erro: fluxo de execucao atingiu um local inesperado." << endl; pausar(1);})

    #define IMPRIMIR_ERRO_MALHA_VAZIA()  ({cout << "Erro: a malha esta' vazia, ou seja, nao possui celulas." << endl; pausar(1);})

    #define IMPRIMIR_ERRO_FRONTEIRA_DE_CELULA_INEXISTENTE() ({cout << "Erro: uma condicao de fronteira foi solicitada para uma celula inexistente!" << endl; pausar(1);})

    #define IMPRIMIR_ERRO_FRONTEIRA_DIRECAO_INEXISTENTE() ({cout << "Erro: uma condicao de fronteira foi solicitada para uma direcao inexistente!" << endl; pausar(1);})

    #define IMPRIMIR_ERRO_CELULA_INEXISTENTE() ({cout << "Erro: uma celula foi solicitada mas ela nao existe (ou aponta para o vazio)!" << endl; pausar(1);})

    #define IMPRIMIR_ERRO_ID_VARIAVEL_INEXISTENTE() ({cout << "Erro: foi solicitado uma id de variavel inexistente!" << endl; pausar(1);})

    #define IMPRIMIR_ERRO_FACE_INVALIDA_DERIVADA() ({cout << "Erro: foi solicitado calculo de derivada em uma face invalida!" << endl; pausar(1);})

    #define IMPRIMIR_ERRO_DERIVADA_FORA() ({cout << "Erro: foi solicitado o calculo de uma derivada fora do dominio, ou seja, em algum vol. ficticio!" << endl; pausar(1);})

    #define IMPRIMIR_ERRO_VARDEPENDENTE_INEXISTENTE() ({cout << "Erro: foi solicitado o calculo da derivada de uma variavel dependente nao implementada!" << endl; pausar(1);})

    #define IMPRIMIR_ERRO_VARINDEPENDENTE_INEXISTENTE() ({cout << "Erro: foi solicitado o calculo de uma derivada em relacao a uma variavel independente nao implementada!" << endl; pausar(1);})

    #define IMPRIMIR_ERRO_VAR_NAO_IMPLEMENTADA() ({cout << "Erro: foi solicitada uma variavel ainda nao implementada" << endl; pausar(1);})

    #define IMPRIMIR_ERRO_FACE_INVALIDA() ({cout << "Erro: foi solicitado calculo em uma face invalida!" << endl; pausar(1);})

    #define IMPRIMIR_ERRO_DIRECAO_INVALIDA() ({cout << "Erro: foi solicitada uma direcao invalida!" << endl; pausar(1);})

    #define IMPRIMIR_ERRO_ELEMENT_NULO() ({cout << "Erro: firstElement e' nulo, nao ha' como acessar seus campos!" << endl; pausar(1);})

    #define IMPRIMIR_ID_CICLO_INEXISTENTE() ({cout << "Erro: foi dado um identificador de ciclo (do algoritmo SIMPLEC) inexistente!" << endl; pausar(1);})

#endif // ERROS_H

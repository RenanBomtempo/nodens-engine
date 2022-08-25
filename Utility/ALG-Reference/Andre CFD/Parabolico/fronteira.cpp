
/*  ============ fronteira.cpp ===========
    Contem informacoes sobre as condicoes
    de fronteira do problema

Criado por Andre STALTZ dia 31/03/07
*/

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include "constants.h"
#include "erros.h"

/*______________________________________________________________________________
                            
                          valorCondicaoFronteira()
                            
    Retorna o valor de uma certa variavel (indicada pelo parametro) em uma das 
fronteiras do dominio (tambem especificada pelo parametro).
    
    Parametros:
         celula: a celula atual que possui fronteira
         id_variavel: um identificador para saber qual variavel esta sendo
                      solicitada;
         direcao: indica qual a fronteira (norte, sul, leste, oeste);

Criado por Andre STALTZ dia 31/03/07
Ampliado para u, v e p por Fernando Grossi dia 24/04/07
______________________________________________________________________________*/

double valorCondicaoFronteira( Cell *celula, short id_variavel, char direcao )
{   
    // Verificacao de erro de celula inexistente
    if( celula == NULL )
    {
        cout << "Na funcao condicaoFronteira():\n";
        IMPRIMIR_ERRO_FRONTEIRA_DE_CELULA_INEXISTENTE();
        return 0.0;    
    }

    // Verificacao de erro de direcao inva'lida
    if( direcao != 'n'
    &&  direcao != 's'
    &&  direcao != 'e'
    &&  direcao != 'w' )
    {
        cout << "Na funcao condicaoFronteira():\n";
        IMPRIMIR_ERRO_FRONTEIRA_DIRECAO_INEXISTENTE();
        return 0.0;
    }

    // Verificar qual variavel foi solicitada na fronteira
    switch( id_variavel )
    {
        case ID_VAR_RHO: // ===================== RHO =========================
            switch( direcao )
            {
                case 'n': // Rho no norte -------------------------------------

                    return ( celula->rhoAtual + celula->north->rhoAtual ) / 2.0;

                break;

                case 's': // Rho no sul ---------------------------------------

                    return ( celula->rhoAtual + celula->south->rhoAtual ) / 2.0;

                break;

                case 'e': // Rho no leste -------------------------------------

                    return PRESSAO_LIVRE / (R * TEMPERATURA_LIVRE); // Rho livre

                break; 

                case 'w': // Rho no oeste -------------------------------------

                    return PRESSAO_LIVRE / (R * TEMPERATURA_LIVRE); // Rho livre

                break;
            }
        break;

       /* case ID_VAR_T: // =================== Temperatura =====================
            switch( direcao )
            {
                case 'n': // Temperatura no norte -----------------------------
                    
                    return TEMPERATURA_PAREDE; // Temperatura da placa plana
                
                break;

                case 's': // Temperatura no sul -------------------------------

                    return TEMPERATURA_PAREDE; // Temperatura da placa plana

                break;

                case 'e': // Temperatura no leste -------------------------------
                    
                    return TEMPERATURA_LIVRE; // Temperatura livre
                
                break;

                case 'w': // Temperatura no leste -------------------------------
                    
                    return TEMPERATURA_LIVRE; // Temperatura livre
                
                break;
            }
        break;/**/

        case ID_VAR_U: // =================== Velocidade U =====================
            switch( direcao )
            {
                case 'n': // Velocidade U no norte -----------------------------
                    
                    return 0.0; // Velocidade da placa superior
                
                break;

                case 's': // Velocidade U no sul -------------------------------

                    return 0.0; // Velocidade da placa inferior

                break;

                case 'e': // Velocidade U no leste -------------------------------
                    
                    return celula->uAtual;

                break;

                case 'w': // Velocidade U no oeste -------------------------------

                    return VEL_LIVRE; // Velocidade livre

                break;
            }
        break;

        case ID_VAR_V: // =================== Velocidade V =====================
            switch( direcao )
            {
                case 'n': // Velocidade V no norte -----------------------------
                    
                    return 0.0; // Velocidade nula
                
                break;

                case 's': // Velocidade V no sul -------------------------------

                    return 0.0; // Velocidade nula

                break;

                case 'e': // Velocidade V no leste -------------------------------
                   
                    return celula->vAtual;

                break;

                case 'w': // Velocidade V no oeste -------------------------------
                    
                    return 0.0;

                break;
            }
        break;

        case ID_VAR_P: // =================== Pressao =====================
            switch( direcao )
            {
                case 'n': // Pressao no norte -------------------------------

                    return (celula->pAtual + celula->north->pAtual)/2.0;

                break;

                case 's': // Pressao no sul -------------------------------

                    return (celula->pAtual + celula->south->pAtual)/2.0;

                break;

                case 'e': // Pressao no leste -------------------------------

                    return 0.0;

                break;

                case 'w': // Pressao no oeste -------------------------------

                    return PRESSAO_LIVRE;

                break;
            }
        break;/**/

        case ID_VAR_MI: // =============== Viscosidade dinamica mi ================
            switch( direcao )
            {
                case 'n': // mi no norte -----------------------------

                    return ( celula->miAtual + celula->north->miAtual ) / 2.0;

                break;

                case 's': // mi no sul -------------------------------

                    return ( celula->miAtual + celula->south->miAtual ) / 2.0;

                break;

                case 'e': // mi no leste -------------------------------

                    return MI_ZERO; // Livre

                break;

                case 'w': // mi no leste -------------------------------

                    return MI_ZERO; // Livre

                break;
            }
        break;

        case ID_VAR_Et: // =============== Energia total Et ================
            switch( direcao )
            {
                case 'n': // Et no norte -----------------------------

                    // ALTERADO EM 21/06/07 !!!!!!!! ########################   
                    return PRESSAO_LIVRE / (R * TEMPERATURA_PAREDE) *
                           (
                               R * TEMPERATURA_PAREDE / (GAMMA - 1.0)
                             + AO_QUADRADO(0.0) / 2.0
                           );       // Prescrito

                break;

                case 's': // Et no sul -------------------------------

                    // ALTERADO EM 21/06/07 !!!!!!!! ########################   
                    return PRESSAO_LIVRE / (R * TEMPERATURA_PAREDE) *
                           (
                               R * TEMPERATURA_PAREDE / (GAMMA - 1.0)
                             + AO_QUADRADO(0.0) / 2.0
                           );       // Prescrito

                break;

                case 'e': // Et no leste -------------------------------

                    return celula->EtAtual;

                break;

                case 'w': // Et no oeste -------------------------------

                    return PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE) *
                           (
                               R * TEMPERATURA_LIVRE / (GAMMA - 1.0)
                             + AO_QUADRADO(VEL_LIVRE) / 2.0
                           );       // Livre
                break;
            }
        break;


        default:
            // Outros casos
            cout << "Na funcao condicaoFronteira():\n";
            IMPRIMIR_ERRO_FRONTEIRA_ID_VARIAVEL_INEXISTENTE();
            cout << "Do erro acima, id_variavel = " << id_variavel << endl;
            return 0.0;
        break;
    }

    cout << "Na funcao condicaoFronteira():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return 0.0;
}

/*______________________________________________________________________________
                            
                          tipoFronteira()
                            
    Retorna o identificador que indica qual o tipo da fronteira de uma certa
variavel em cada celula do dominio.
    
    Parametros:
         celula: a celula atual que possui fronteira
         id_variavel: um identificador para saber qual variavel esta sendo
                      solicitada;

Criado por Andre STALTZ dia 11/06/07
______________________________________________________________________________*/
short tipoCondicaoFronteira( Cell *celula, short id_variavel )
{
    // Verificacao de erro de celula inexistente
    if( celula == NULL )
    {
        cout << "Na funcao tipoCondicaoFronteira():\n";
        IMPRIMIR_ERRO_FRONTEIRA_DE_CELULA_INEXISTENTE();
        return 0;    
    }

    // Retorna para celulas internas ao dominio
    if( celula->tipoFronteira == NAO_FRONTEIRA )
        return NAO_FRONTEIRA;

    // Verificar qual variavel foi solicitada seu tipo de condicao na fronteira
    switch( id_variavel )
    {
        case ID_VAR_RHO: // ===================== RHO =========================

            if( celula->tipoFronteira == FRONTEIRA_NORTE )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_SUL )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_LESTE )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_OESTE )
                return CONDICAO_FRONTEIRA_PRESCRITA;

        break;

        case ID_VAR_T: // =================== Temperatura =====================

            if( celula->tipoFronteira == FRONTEIRA_NORTE )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_SUL )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_LESTE )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_OESTE )
                return CONDICAO_FRONTEIRA_PRESCRITA;

        break;

        case ID_VAR_U: // =================== Velocidade U =====================

            if( celula->tipoFronteira == FRONTEIRA_NORTE )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_SUL )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_LESTE )
                return CONDICAO_FRONTEIRA_SAIDA_MASSA;

            else if( celula->tipoFronteira == FRONTEIRA_OESTE )
                return CONDICAO_FRONTEIRA_ENTRADA_MASSA;

        break;

        case ID_VAR_V: // =================== Velocidade V =====================

            if( celula->tipoFronteira == FRONTEIRA_NORTE )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_SUL )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_LESTE )
                return CONDICAO_FRONTEIRA_SAIDA_MASSA;

            else if( celula->tipoFronteira == FRONTEIRA_OESTE )
                return CONDICAO_FRONTEIRA_ENTRADA_MASSA;

        break;

        case ID_VAR_P: // =================== Pressao =====================

            if( celula->tipoFronteira == FRONTEIRA_NORTE )
                return CONDICAO_FRONTEIRA_EXTRAPOLADA;

            else if( celula->tipoFronteira == FRONTEIRA_SUL )
                return CONDICAO_FRONTEIRA_EXTRAPOLADA;

            else if( celula->tipoFronteira == FRONTEIRA_LESTE )
                return CONDICAO_FRONTEIRA_SAIDA_MASSA; // ou prescrita?!

            else if( celula->tipoFronteira == FRONTEIRA_OESTE )
                return CONDICAO_FRONTEIRA_ENTRADA_MASSA;

        break;

        case ID_VAR_MI: // =============== Viscosidade dinamica mi ================

            if( celula->tipoFronteira == FRONTEIRA_NORTE )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_SUL )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_LESTE )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_OESTE )
                return CONDICAO_FRONTEIRA_PRESCRITA;

        break;

        case ID_VAR_Et: // =============== Energia total Et ================

            if( celula->tipoFronteira == FRONTEIRA_NORTE )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_SUL )
                return CONDICAO_FRONTEIRA_PRESCRITA;

            else if( celula->tipoFronteira == FRONTEIRA_LESTE )
                return CONDICAO_FRONTEIRA_SAIDA_MASSA;

            else if( celula->tipoFronteira == FRONTEIRA_OESTE )
                return CONDICAO_FRONTEIRA_ENTRADA_MASSA;

        break;


        default:
            // Outros casos
            cout << "Na funcao tipoCondicaoFronteira():\n";
            IMPRIMIR_ERRO_FRONTEIRA_ID_VARIAVEL_INEXISTENTE();
            cout << "Do erro acima, id_variavel = " << id_variavel << endl;
            return 0;
        break;
    }

    cout << "Na funcao tipoCondicaoFronteira():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return 0;

}/**/

/*______________________________________________________________________________
                            
                          definirCelulasFronteira()
                            
    Marca todas as celulas que s�o celulas de fronteira (volumes ficticios), e 
suas respectivas direcoes de fronteira.
    
    Parametros:
         malha: o dominio discretizado contendo todas celulas.

Criado por Andre STALTZ dia 11/06/07
______________________________________________________________________________*/
void definirCelulasFronteira( Grid *malha )
{
    Cell *celula;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Percorre celulas da borda direita ###########################
        for( celula = malha->firstGridCell;
             celula->south->type != 'w';
             celula = celula->south)
        {
            celula->volumeFicticio = true;

            // Define celulas com fronteira no leste
            if( celula->north->type == 'b'
            &&  celula->south->type == 'b' )
            {
                celula->tipoFronteira = FRONTEIRA_LESTE;
            }
            // Define celulas do canto nordeste
            else if( celula->north->type == 'w' 
            && celula->south->type == 'b' )
            {
                celula->active = false;
            }
        }

        // Percorre celulas da borda inferior ##########################
        for( ;
             celula->west->type != 'w';
             celula = celula->west)
        {
            celula->volumeFicticio = true;

            // Define celulas com fronteira no sul
            if( celula->east->type == 'b'
            &&  celula->west->type == 'b' )
            {
                celula->tipoFronteira = FRONTEIRA_SUL;
            }
            // Define celulas do canto sudeste
            else if( celula->east->type == 'w' 
            && celula->west->type == 'b' )
            {
                celula->active = false;
            }
        }

        // Percorre celulas da borda esquerda ##########################
        for( ;
             celula->north->type != 'w';
             celula = celula->north)
        {
            celula->volumeFicticio = true;

            // Define celulas com fronteira no oeste
            if( celula->north->type == 'b'
            &&  celula->south->type == 'b' )
            {
                celula->tipoFronteira = FRONTEIRA_OESTE;
            }
            // Define celulas do canto sudoeste
            else if( celula->north->type == 'b' 
            && celula->south->type == 'w' )
            {
                celula->active = false;
            }
        }

        // Percorre celulas da borda superior ##########################
        for( ;
             celula->east->type != 'w';
             celula = celula->east)
        {
            celula->volumeFicticio = true;

            // Define celulas com fronteira no norte
            if( celula->east->type == 'b'
            &&  celula->west->type == 'b' )
            {
                celula->tipoFronteira = FRONTEIRA_NORTE;
            }
            // Define celulas do canto noroeste
            else if( celula->east->type == 'b' 
            && celula->west->type == 'w' )
            {
                celula->active = false;
            }
        }
    }
    else
    {
        cout << "Na funcao definirCelulasFronteira():\n" << endl;
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return;
    }
}

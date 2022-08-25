/* =======================================================================
 * UFMG / ICEx
 *                    PEG Dinamica dos Fluidos Computacional
 *              Titulo: Metodo SIMPLEC para o escoamento compressivel
 *                      supersonico.
 *
 * ======================================================================= */
// main.cpp

// Bibliotecas basicas -----------------------

// glut para Windows ou Linux
#include <GL/glut.h>
#include <GL/gl.h>

#include <stdarg.h>
#include <iostream>
#include <stdio.h>

    using std::cout;
    using std::endl;
    using std::ios;
    using std::cin;

#include <cmath>

// Arquivos do meu projeto -------------------
#include "Grid.h"
#include "functions.h"
#include "constants.h"

// Ativa quando o programa sera' compilado para Linux:
#ifdef USANDO_LINUX
       #include <pthread.h> // Multiplas linhas de execucao
       #include <sys/time.h> // para funcao de pausar execucao

       // Variaveis globais de configuracao em tempo real
       pthread_t resolvedor;
#endif

double escalaZoomVisualizacao = 1.35 / LHORI;
double translacaoHorizontalVisualizacao = 0,
       translacaoVerticalVisualizacao = 0;

double residuosP[MAXITER_GRAFICO_RESIDUOS];
long iteracoesTempo = 0;
double maiorResiduo = 0.0;
long iterFinal = 100000;

int visualizarQualGrafico = ID_GRAFICO_RESI;
bool visualizarCarnaval = false,
     visualizarCelulasMalha = false,
     visualizarLegenda = true,
     visualizarPeano = false,
     visualizarResiduo = true,
     visualizarEnfeites = false,
     visualizarNumeroCelula = false,
     pausaSimulacao = true,
     fecharPrograma = false;

// Menus e submenus acessiveis pelo mouse
static int menuRaiz,
           subMenuVisualizarVariaveis,
           subMenuVisualizarAux,
           opcaoEscolhidaMenu;

// Malha principal do programa
Grid *malhaESSPP;

/* ----------------------------------------
     Alterar opcoes do menu
------------------------------------------- */
void aplicarOpcoesMenu(){
    switch (opcaoEscolhidaMenu){

        // Pausar a simulacao
        case 7:
            malhaESSPP->print( iteracoesTempo );
            opcaoEscolhidaMenu = -1;
        break;

        // Pausar a simulacao
        case 8:
            pausaSimulacao = !pausaSimulacao;
            opcaoEscolhidaMenu = -1;
        break;

        // Iniciar a simulacao
        case 9:
            condicaoInicial(malhaESSPP);
            pausaSimulacao = false;
            opcaoEscolhidaMenu = -1;
        break;

        // Escolher grafico
        case 11: // Densidade
            visualizarQualGrafico = ID_GRAFICO_DENS;
            visualizarResiduo = false;
            opcaoEscolhidaMenu = -1;
        break;

        case 12: // Temperatura
            visualizarQualGrafico = ID_GRAFICO_TEMP;
            visualizarResiduo = false;
            opcaoEscolhidaMenu = -1;
        break;

        case 13: // Velocidade
            visualizarQualGrafico = ID_GRAFICO_VELO;
            visualizarResiduo = false;
            opcaoEscolhidaMenu = -1;
        break;

        case 14: // Pressao
            visualizarQualGrafico = ID_GRAFICO_PRES;
            visualizarResiduo = false;
            opcaoEscolhidaMenu = -1;
        break;

        // Desenhar malha
        case 21:
            visualizarResiduo = false;
            visualizarCelulasMalha = (!visualizarCelulasMalha);
            opcaoEscolhidaMenu = -1;
        break;

        // Desenhar curva de Hilbert
        case 22:
            visualizarPeano = (!visualizarPeano);
            opcaoEscolhidaMenu = -1;
        break;

        // Ativar/desativar carnaval
        case 23:
            visualizarCarnaval = (!visualizarCarnaval);
            opcaoEscolhidaMenu = -1;
        break;

        // Desenhar legenda dos graficos coloridos
        case 24:
            visualizarLegenda = (!visualizarLegenda);
            opcaoEscolhidaMenu = -1;
        break;

        // Desenhar residuo
        case 25:
            visualizarResiduo = true;
            visualizarCelulasMalha = visualizarCarnaval 
                                   = visualizarEnfeites 
                                   = visualizarPeano
                                   = visualizarNumeroCelula
                                   = false;
            visualizarQualGrafico = ID_GRAFICO_RESI;
            opcaoEscolhidaMenu = -1;
        break;

        // Desenhar numeros das celulas
        case 26:
            visualizarNumeroCelula = !visualizarNumeroCelula;
            visualizarResiduo = false;
            visualizarCelulasMalha = true;

            if(visualizarQualGrafico != ID_GRAFICO_GRIDPOS)
                visualizarQualGrafico = ID_GRAFICO_GRIDPOS;
            else
                visualizarQualGrafico = 0;

            opcaoEscolhidaMenu = -1;
        break;

        // Desenhar enfeites
        case 27:
            visualizarEnfeites = !visualizarEnfeites;
            opcaoEscolhidaMenu = -1;
        break;

        // Apagar tudo
        case 28:
            visualizarCelulasMalha = false;
            visualizarCarnaval = false;
            visualizarLegenda = false;
            visualizarPeano = false;
            visualizarResiduo = false;
            visualizarNumeroCelula = false;
            visualizarEnfeites = false;
            visualizarQualGrafico = 0;
            opcaoEscolhidaMenu = -1;
        break;

        default:
        break;
    }
}

void sair()
{
    fecharPrograma = true;
    
    // Ativa quando programa sera' compilado para Linux:
    #ifdef USANDO_LINUX
           pthread_join( resolvedor, NULL );
    #endif
    exit(0);
}

/* --------------------------------------------------
      Interpreta algumas teclas do teclado
----------------------------------------------------- */
void Teclado(unsigned char tecla, int x, int y)
{
    switch (tecla)
    {
        // Ajustando o zoom + pelo teclado
        case '=':
        case '+':
            escalaZoomVisualizacao += INCREMENTO_ZOOM;
        break;
        // Ajustando o zoom - pelo teclado
        case '-':
        case '_':
            if (escalaZoomVisualizacao > INCREMENTO_ZOOM)
                escalaZoomVisualizacao -= INCREMENTO_ZOOM;
        break;


        case 'a':
        case 'A':
            iterFinal -= 10000;
        break;
        
        case 's':
        case 'S':
            iterFinal += 10000;
        break;

        // Imprimir resultados num txt
        case 'i':
        case 'I':
            malhaESSPP->print( iteracoesTempo );
        break;

        // Pausar/Iniciar simulacao
        case 'p':
        case 'P':
            pausaSimulacao = (!pausaSimulacao);
        break;

        // ESC sai do programa
        case 27:
            sair();

        default:
        break;
    }
}

/*-----------------------------------------------------------------------------*
* Funcao chamada para gerenciar eventos com teclas especiais do teclado, tais *
* como UP E DOWN.                                                             *
*----------------------------------------------------------------------------*/
void teclasEspeciais( int tecla, int x, int y )
{
    if( tecla == GLUT_KEY_UP )        // Translacao para cima.
        translacaoVerticalVisualizacao -= LHORI/20.0;

    if( tecla == GLUT_KEY_DOWN )      // Translacao para baixo.
        translacaoVerticalVisualizacao += LHORI/20.0;

    if( tecla == GLUT_KEY_RIGHT )     // Translacao para direita.
        translacaoHorizontalVisualizacao -= LHORI/20.0;

    if( tecla == GLUT_KEY_LEFT )      // Translacao para esquerda.
        translacaoHorizontalVisualizacao += LHORI/20.0;
}

// Verifica qual opcao do menu foi recebida
void menu(int value){
    if(value == 0){
        sair();
    }
    else{
        opcaoEscolhidaMenu = value;
    }
}

/* ------------------------------------------------
     Menu acessavel pelo mouse
--------------------------------------------------- */

void createMenu(void)
{
    // Cria um submenu
    subMenuVisualizarVariaveis = glutCreateMenu(menu);

    // Adiciona opcoes
    glutAddMenuEntry("Densidade", 11);
    glutAddMenuEntry("Temperatura", 12);
    glutAddMenuEntry("Velocidade", 13);
    glutAddMenuEntry("Pressao", 14);

    // Cria um submenu
    subMenuVisualizarAux = glutCreateMenu(menu);

    // Adiciona opcoes
    glutAddMenuEntry("Malha",21);
    glutAddMenuEntry("Curva de Hilbert",22);
    glutAddMenuEntry("Resultado numerico \"Carnaval\"",23);
    glutAddMenuEntry("Legenda", 24);
    glutAddMenuEntry("Grafico de residuos", 25);
    glutAddMenuEntry("Numero das celulas", 26);
    glutAddMenuEntry("Enfeites", 27);
    glutAddMenuEntry("- Apagar todos -", 28);

    // Menu principal
    menuRaiz = glutCreateMenu(menu);

    glutAddSubMenu("Visualizar variaveis", subMenuVisualizarVariaveis);
    glutAddSubMenu("Visualizar auxiliares", subMenuVisualizarAux);

    glutAddMenuEntry("Iniciar", 9);
    glutAddMenuEntry("Pausar", 8);
    glutAddMenuEntry("Imprimir resultado txt", 7);
    glutAddMenuEntry("Sair", 0);

    // O menu responde ao botao direito do mouse
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/*
-----------------------------------------------------------------------
   MEU DISPLAY: onde ser? pintado e executado funcoes principais
-----------------------------------------------------------------------
*/
void *resolveProblema(void *)
{
    while(!fecharPrograma)
    {
        /* Procede os calculos */
        if( !pausaSimulacao ) // Aqui entra as funcoes para resolver os Navier-Stokes
        { 
            // Resolve por SIMPLE
            solveSuperSonicSIMPLEC( malhaESSPP );

            calculaResiduos( malhaESSPP, iteracoesTempo );

            // O tempo e' incrementado e tudo se repete
            malhaESSPP->tempoAtual += malhaESSPP->deltaT;
            atualizaParametrosDoTempoAnterior( malhaESSPP );

            // Pausar a cada deltaT
            //pausaSimulacao = true; 
            //cout << "maiorResiduo = " << malhaESSPP->maiorResiduo << "\n";

            iteracoesTempo++;
            cout << "iterFinal = " << iterFinal << endl;
        }

        if( iteracoesTempo == iterFinal-1 )
        {
            malhaESSPP->print( ++iteracoesTempo );
            pausaSimulacao = true;
        }
    }
}

void myDisplay()
{
    // Prepara OpenGL para lidar com os modelos 3D
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gluLookAt(0,0,0,0,0,-100,0,1,0);

    // Ajusta o zoom
    glScalef(escalaZoomVisualizacao,escalaZoomVisualizacao,1.0);

    // Faz translacao da malha
    glTranslatef(translacaoHorizontalVisualizacao,
                 translacaoVerticalVisualizacao,
                 0);

    // Chama as opcoes de menu
    if( opcaoEscolhidaMenu != -1 )    
        aplicarOpcoesMenu();

    // Ativa quando o programa sera' compilado para Windows:
    #ifdef USANDO_WINDOWS
        /* Procede os calculos */
        if( !pausaSimulacao ) // Aqui entra as funcoes para resolver os Navier-Stokes
        { 
            // Resolve por SIMPLE
            solveSuperSonicSIMPLEC( malhaESSPP );

            calculaResiduos( malhaESSPP, iteracoesTempo );

            // O tempo e' incrementado e tudo se repete
            malhaESSPP->tempoAtual += malhaESSPP->deltaT;
            atualizaParametrosDoTempoAnterior( malhaESSPP );

            // Pausar a cada deltaT
            //pausaSimulacao = true; 
            //cout << "maiorResiduo = " << malhaESSPP->maiorResiduo << "\n";

            iteracoesTempo++;
            cout << "iterFinal = " << iterFinal << endl;
        }

        if( iteracoesTempo == iterFinal-1 )
        {
            malhaESSPP->print( ++iteracoesTempo );
            pausaSimulacao = true;
        }
    #endif

    // Desenha o grafico de residuos
    if( visualizarResiduo )
        desenharResiduo( malhaESSPP, iteracoesTempo );

    // Desenha Interface
    if( pausaSimulacao )
        desenharPausa( malhaESSPP, escalaZoomVisualizacao,
                       translacaoHorizontalVisualizacao,
                       translacaoVerticalVisualizacao );

    desenharTempo( iteracoesTempo, escalaZoomVisualizacao,
                   translacaoHorizontalVisualizacao, 
                   translacaoVerticalVisualizacao );
    /**/

    if( visualizarLegenda )
      desenharLegenda( malhaESSPP, escalaZoomVisualizacao, 
                       translacaoHorizontalVisualizacao, 
                       translacaoVerticalVisualizacao, visualizarQualGrafico );
    /**/

    // Plota a curva de Peano no plano XY 
    if( visualizarPeano )
      malhaESSPP->drawPeanoCurve(1);

    // Desenha os contornos de cada celula da malha
    if( visualizarCelulasMalha )
        desenharMalha( malhaESSPP );

    // Desenha o resultado numerico em cada celula
    if( visualizarCarnaval && visualizarQualGrafico != ID_GRAFICO_GRIDPOS ) 
        desenharValores( malhaESSPP, visualizarQualGrafico );

    if( visualizarNumeroCelula )
        desenharValores( malhaESSPP, ID_GRAFICO_GRIDPOS);

    // Plota as funcoes
    switch( visualizarQualGrafico )
    {
        case ID_GRAFICO_DENS:
            desenharDensidade( malhaESSPP );
        break;

        case ID_GRAFICO_TEMP:
            desenharTemperatura( malhaESSPP );
        break;

        case ID_GRAFICO_VELO:
            desenharVelocidade( malhaESSPP, 1.5*VEL_LIVRE );
        break;

        case ID_GRAFICO_PRES:
            desenharPressao( malhaESSPP );
        break;

        default: break;
    }

    // Desenha enfeites
    if( visualizarEnfeites ) 
        desenharPlacasPlanas( malhaESSPP );

    // Manda a saida de graficos para a tela
    glutSwapBuffers();
}

/*
========================================================================
      PARTE PRINCIPAL DO PROGRAMA
========================================================================
*/
int main(int argc, char** argv)
{  
    int refinamentoInicial, maxRef, minRef, erroPotencia, deltaTPotencia;
    malhaESSPP = new Grid;

    // Titulo na tela
    cout << "=======================================\n";
    cout << "ESCOAMENTO SUPERSONICO \n\n";
    cout << "-------- data: 21/08/07\n";
    cout << " PEG FLUIDOS \n";
    cout << "=======================================\n\n"; 

    // Entrada de variaveis
    cout << "Entre com o valor do refinamento inicial da malha:\n";
    scanf("%i",&refinamentoInicial);

    //cout << "\nEntre com o valor do refinamento maximo permitido:\n";
    //scanf("%i",&maxRef);

    //cout << "\nEntre com o valor do refinamento minimo permitido ao desrefinar:\n";
    //scanf("%i",&minRef);

    cout << "\nEntre com o incremento de tempo:\n";
    cout << "10^-";
    scanf("%i",&deltaTPotencia);
    malhaESSPP->deltaT = pow( 10.0,-((double)(deltaTPotencia)) );
    
    cout << "\nCarregando...\n";

    // Iniciar o GLUT (interface do OpenGL)
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(LARGURA_TELA,ALTURA_TELA);
    glutCreateWindow("Escoamento supersonico por SIMPLEC");
    //glutFullScreen();
   
    // Cor de fundo
    glClearColor(1.0, 1.0, 1.0, 1.0); // Branco
 
    // Ajustes iniciais do OpenGL
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0,1.0,
               -(1.0*ALTURA_TELA)/(1.0*LARGURA_TELA),(1.0*ALTURA_TELA)/(1.0*LARGURA_TELA),
                   -PROFUNDIDADE_Z_PADRAO,PROFUNDIDADE_Z_PADRAO);
    gluLookAt(0,0,0,0,0,-10,0,1,0);                 

    // Inicia grade
    malhaESSPP->initializeGrid( false );
    refineGrid( malhaESSPP, refinamentoInicial, 0.0 );
    malhaESSPP->refinamentoMaximo = refinamentoInicial;//maxRef;
    malhaESSPP->refinamentoMinimo = refinamentoInicial;//minRef;
    condicaoInicial(malhaESSPP);

    // Entrada de interface
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(Teclado);
    glutSpecialFunc(teclasEspeciais);
    createMenu();
    
    // Ativa quando programa sera' compilado para Linux:
    #ifdef USANDO_LINUX
        pthread_create( &resolvedor, NULL, resolveProblema, NULL );
    #endif
    
    // Loop principal do programa:
    glutIdleFunc(myDisplay);
    glutMainLoop();
	
    return(0); 
}

// Pausa a execucao por 'duracao' segundos
void pausar(int duracao)
{
    // Ativa quando programa sera' compilado para Linux:
    #ifdef USANDO_LINUX
        // Erro de parametro invalido
        if(duracao <= 0)
            return;

        timeval relogio;
        gettimeofday(&relogio, NULL);
        int inicio = relogio.tv_sec;
        int final = inicio;
    
        while(final - inicio < duracao)
        {
            gettimeofday(&relogio, NULL);
            final = relogio.tv_sec;
        }
    #endif   
}

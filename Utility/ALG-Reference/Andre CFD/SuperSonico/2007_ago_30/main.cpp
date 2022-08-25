/* =======================================================================
 * UFMG / ICEx
 *                    PEG Dinamica dos Fluidos Computacional
 *              Titulo: Metodo SIMPLEC para o escoamento compressivel
 *                      supersonico.
 *
 * Ultima atualizacao: 21/08/07
 * ======================================================================= */
// main.cpp

/* ###################################################
TAREFAS:

- Alterar interpolaVariavelNaFace para incluir celulas ficticias. ok

- Perguntar para o Rodney o tratamento de Omega quando AsScalarAs == 0, no GBCE.

##################################################### */


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

// Variaveis globais de configuracao em tempo real
double escalaZoomVisualizacao = 1.35/LHORI;
double translacaoHorizontalVisualizacao = 0,
       translacaoVerticalVisualizacao = 0;

double residuosP[MAXITER_GRAFICO_RESIDUOS];
int iteracoesTempo = 0;
double maiorResiduo = 0.0;

int visualizarQualGrafico = 1;
bool visualizarCarnaval = false,
     visualizarCelulasMalha = true,
     visualizarLegenda = true,
     visualizarPeano = false,
     visualizarResiduo = false,
     visualizarEnfeites = true,
     pausaSimulacao = true;

// Menus e submenus acessaveis pelo mouse
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
            visualizarQualGrafico = 1;
            visualizarResiduo = false;
            opcaoEscolhidaMenu = -1;
        break;

        case 12: // Temperatura
            visualizarQualGrafico = 2;
            visualizarResiduo = false;
            opcaoEscolhidaMenu = -1;
        break;

        case 13: // Velocidade
            visualizarQualGrafico = 3;
            visualizarResiduo = false;
            opcaoEscolhidaMenu = -1;
        break;

        case 14: // Pressao
            visualizarQualGrafico = 4;
            visualizarResiduo = false;
            opcaoEscolhidaMenu = -1;
        break;

        // Desenhar malha
        case 21:
            visualizarCelulasMalha = (!visualizarCelulasMalha);
            opcaoEscolhidaMenu = -1;
        break;

        // Desenhar malha
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
                                   = false;
            visualizarQualGrafico = 5;
        break;

        // Desenhar enfeites
        case 26:
            visualizarEnfeites = !visualizarEnfeites;
            opcaoEscolhidaMenu = -1;
        break;

        // Apagar tudo
        case 27:
            visualizarCelulasMalha = false;
            visualizarCarnaval = false;
            visualizarLegenda = false;
            visualizarPeano = false;
            visualizarResiduo = false;
            visualizarEnfeites = false;
            visualizarQualGrafico = 0;
            opcaoEscolhidaMenu = -1;
        break;

        default:
        break;
    }
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

        // Pausar/Iniciar simulacao
        case 'p':
        case 'P':
            pausaSimulacao = (!pausaSimulacao);
        break;

        // ESC sai do programa
        case 27:
            exit(0);

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
        exit(0);
    }
    else{
        opcaoEscolhidaMenu = value;
    }
}

/* ------------------------------------------------
     Menu acessavel pelo mouse
--------------------------------------------------- */

void createMenu(void){

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
    glutAddMenuEntry("Enfeites", 26);
    glutAddMenuEntry("- Apagar todos -", 27);

    // Menu principal
    menuRaiz = glutCreateMenu(menu);

    glutAddSubMenu("Visualizar variaveis", subMenuVisualizarVariaveis);
    glutAddSubMenu("Visualizar auxiliares", subMenuVisualizarAux);

    glutAddMenuEntry("Iniciar", 9);
    glutAddMenuEntry("Pausar", 8);
    glutAddMenuEntry("Sair", 0);

    // O menu responde ao botao direito do mouse
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/*
-----------------------------------------------------------------------
   MEU DISPLAY: onde ser? pintado e executado funcoes principais
-----------------------------------------------------------------------
*/
void myDisplay(void)
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

    /* Procede os calculos */
    if( !pausaSimulacao ) // Aqui entra as funcoes para resolver os Navier-Stokes
    { 
        // Resolve por SIMPLE
        solveSuperSonicSIMPLEC( malhaESSPP );

        calculaResiduos( malhaESSPP, iteracoesTempo );
    }

    // Desenha o grafico de residuos
    if( visualizarResiduo )
        desenharResiduo( malhaESSPP, iteracoesTempo );

    if( !pausaSimulacao ){
        // O tempo e' incrementado e tudo se repete
        malhaESSPP->tempoAtual += malhaESSPP->deltaT;
        atualizaParametrosDoTempoAnterior( malhaESSPP );

        // Pausar a cada deltaT
        //pausaSimulacao = true; 
        //cout << "maiorResiduo = " << malhaESSPP->maiorResiduo << "\n";

        iteracoesTempo++;
    }

    // Desenha Interface
    if( pausaSimulacao )
        desenharPausa( malhaESSPP, escalaZoomVisualizacao, 
                       translacaoHorizontalVisualizacao, 
                       translacaoVerticalVisualizacao );

    /*desenharTempo( malhaESSPP, escalaZoomVisualizacao, 
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
    if( visualizarCarnaval ) 
        desenharValores( malhaESSPP, visualizarQualGrafico );

    // Plota as funcoes     
    switch( visualizarQualGrafico )
    {
        case 1: 
            desenharDensidade( malhaESSPP ); 
        break;

        case 2:
            desenharTemperatura( malhaESSPP );
        break;

        case 3:
            desenharVelocidade( malhaESSPP, 1.5*VEL_LIVRE );
        break;

        case 4:
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
    glClearColor(1.0,1.0,1.0,1.0); // Branco
 
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
    
    // My Display
    glutIdleFunc(myDisplay);

    glutMainLoop();
	
    return(0); 
}

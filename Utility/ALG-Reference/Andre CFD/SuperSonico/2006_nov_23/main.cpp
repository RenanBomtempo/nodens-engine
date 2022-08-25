/* ==========================================
 * UFMG / ICEx
 * PAD Dinamica dos Fluidos Computacional
 * Grupo ALG
 * Titulo: Escoamento supersonico na placa plana
 * Ultima atualizacao: 29/11/03 08:26
 * ========================================== */
// main.cpp

//TAREFA: considerar multiplos vizinhos de uma face

//TAREFA: refinar e criterios de refinamento

// Bibliotecas basicas

  // Para linux
//#include <glut.h>         // glut.h includes gl.h and glu.h


// glut para Windows
#include <gl/glut.h>
#include <gl/gl.h>

#include <stdarg.h>
#include <iostream>
#include <stdio.h>

    using std::cout;
    using std::endl;
    using std::ios;
    using std::cin;

#include <cmath>

// Arquivos do meu projeto 
#include "Grid.h"                             
#include "functions.h"
#include "constants.h"

// Variaveis globais de configuracao em tempo real
float escala = 4;
bool peano = false,  
     pausa = true;

float dx = 0,
      dy = 0;

int QualGrafico = 1;
bool carnaval = false;

// Menus e submenus acessaveis pelo mouse
static int menuprincipal, 
           submenu1,
           submenu2, 
           opcao,
           win;

// Malha principal do programa
Grid *grade;

/* ----------------------------------------
     Alterar opcoes do menu
------------------------------------------- */
void opcoes(void){
    switch (opcao){

        // Pausar a simulacao
        case (8):
            pausa = !pausa;
            opcao = -1;
        break;

        // Iniciar a simulacao
        case (9):
            initialCondition(grade);
            pausa = false;
            opcao = -1;
        break;

        // Escolher grafico
        case (11): // Densidade
            QualGrafico = 1;
            opcao = -1;
        break;

        case (12): // Temperatura
            QualGrafico = 2;
            opcao = -1;
        break;
    
        case (13): // Velocidade
            QualGrafico = 3;
            opcao = -1;
        break;
    
        case (14): // Pressao
            QualGrafico = 4;
            opcao = -1;
        break;

        // Ativar/desativar carnaval
        case (15): 
            carnaval = (!carnaval);
            opcao = -1;
        break;
     
        default:
        break;
    }
}    

void Teclado(unsigned char tecla, int x, int y)
{
switch (tecla)
  {
    // Ajustando o zoom + pelo teclado
    case '=':
    case '+':
      escala += 0.2;
    break;
    // Ajustando o zoom - pelo teclado
    case '-':
    case '_':
      if (escala > 0.2)
         escala -= 0.2;
    break;

    // Visualizar a curva de Peano
    case 'p':
    case 'P':
      peano = !peano;
    break;

    // Pausar/Iniciar simulacao
    case 'Z':
    case 'z':
      pausa = (!pausa);
    break;

    // ESC sai do programa
    case 27:
    exit(0);

    default:
    break;
  }
}

/*-----------------------------------------------------------------------------*
* Função chamada para gerenciar eventos com teclas especiais do teclado, tais *
* como UP E DOWN.                                                             *
*----------------------------------------------------------------------------*/
void teclasEspeciais( int tecla, int x, int y )
{
    if( tecla == GLUT_KEY_UP )        // Translação para cima.
        dy -= 2.0;              
    
    if( tecla == GLUT_KEY_DOWN )      // Translação para baixo.
        dy += 2.0;
    
    if( tecla == GLUT_KEY_RIGHT )     // Translação para direita.
        dx -= 2.0;
    
    if( tecla == GLUT_KEY_LEFT )      // Translação para esquerda. 
        dx += 2.0;
}

// Verifica qual opcao do menu foi recebida
void menu(int value){
    if(value == 0){
        exit(0);
    }
    else{
        opcao = value;
    }
}


/* -----------------------------
     Menu acessavel pelo mouse
-------------------------------- */

void createMenu(void){

  // Cria um submenu
  submenu1 = glutCreateMenu(menu);

  // Adiciona opcoes
  glutAddMenuEntry("Densidade", 11);
  glutAddMenuEntry("Temperatura", 12);
  glutAddMenuEntry("Velocidade", 13);
  glutAddMenuEntry("Pressao", 14);
  glutAddMenuEntry("Resultado numérico \"Carnaval\"",15);

  // Cria um submenu
  submenu2 = glutCreateMenu(menu);

  // Adiciona opcoes
  glutAddMenuEntry("Opcao 2.1", 21);
  glutAddMenuEntry("Opcao 2.2", 22);

  // Menu principal
  menuprincipal = glutCreateMenu(menu);

  glutAddSubMenu("Graficos", submenu1);
  glutAddSubMenu("Segundo", submenu2);

  glutAddMenuEntry("Iniciar", 9);
  glutAddMenuEntry("Pausar", 8);
  glutAddMenuEntry("Sair", 0);

  // O menu responde ao botao direito do mouse
  glutAttachMenu(GLUT_RIGHT_BUTTON);

}

/* 
-------------------------------------------------------------
MEU DISPLAY: onde será pintado e executado funcoes principais
-------------------------------------------------------------
*/
void myDisplay(void)
{
    // Prepara OpenGL para lidar com os modelos 3D
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gluLookAt(0,0,0,0,0,-100,0,1,0);
        
    // Ajusta o zoom
    glScalef(escala,escala,1.0);

    // Faz translação da malha
    glTranslatef(dx,dy,0);
    
    // Chama as opcoes de menu    
    opcoes();

    /* Procede os calculos */
    if (!pausa) // Aqui entra as funcoes para resolver o Navier-Stokes
     { solveSuperSonic( grade ); }

    if (pausa)
        desenharPausa(grade, escala );

    /* Plota a curva de Peano no plano XY */
    if (peano)
      grade->drawPeanoCurve(6);
    
    /* Desenha enfeites */
    desenharPlacaPlana(grade);
    desenharTempo(grade, escala);

    /* Plota as funcoes */    
    switch(QualGrafico)
    {
        case 4:             
            if(carnaval) desenharValores(grade,4);

            desenharMalha(grade);
            desenharPressao(grade);
        break;

        case 3:
            if(carnaval) desenharValores(grade,3); 

            desenhaCampoVetorial( grade );   
        break;

        case 2:
            if(carnaval) desenharValores(grade,2);

            desenharMalha(grade);
            desenharTemperatura(grade);
        break;
        
        default:
        case 1:
            if(carnaval) desenharValores(grade,1);
    
            desenharMalha(grade);
            desenharDensidade(grade);
        break;
    }

    /* Manda a saida de graficos para a tela */
    glutSwapBuffers();
} 


/*
========================================================================
      PARTE PRINCIPAL DO PROGRAMA
========================================================================
*/
int main(int argc, char** argv)
{  
   int refinamentoInicial, erroPotencia, deltaTPotencia;
   grade = new Grid;

   // Titulo na tela
   cout << "=======================================\n";
   cout << "ESCOAMENTO SUPERSONICO NUMA PLACA PLANA\n\n";
   cout << "-------- data: 23/11/06\n";
   cout << " PAD FLUIDOS - ALG\n";
   cout << "=======================================\n\n"; 

   // Entrada de variaveis
   cout << "Entre com o valor do refinamento inicial:\n";
   scanf("%i",&refinamentoInicial);

   cout << "\nEntre com o valor do erro maximo nas iteracoes de picard:\n";
   cout << "10^-";
   scanf("%i",&erroPotencia);
   grade->erroPicard = pow( 10.0,-((double)(erroPotencia)) );

   cout << "\nEntre com o incremento de tempo:\n";
   cout << "10^-";
   scanf("%i",&deltaTPotencia);
   grade->deltaT = pow( 10.0,-((double)(deltaTPotencia)) );

   cout << "\nCarregando...\n";

   // Iniciar o GLUT (interface do OpenGL)
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(LARGURA_TELA,ALTURA_TELA);
   glutCreateWindow("Escoamento supersonico");

   // Cor de fundo
   glClearColor(1.0,1.0,1.0,1.0); // Branco

   // Ajustes iniciais do OpenGL
   glEnable(GL_DEPTH_TEST);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-LARGURA_TELA/1,LARGURA_TELA/1,
           -ALTURA_TELA/1,ALTURA_TELA/1,
           -200,200);
   gluLookAt(0,0,0,0,0,-10,0,1,0);                 

   // Inicia grade
   grade->initializeGrid( false );
   refineGrid(grade,refinamentoInicial,0.0,false);
   initialCondition(grade);

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

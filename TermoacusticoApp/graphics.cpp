/* **************************************** */
/* Programa: graphics.cpp                          
   Autora: Denise Burgarelli Duczmal
   Versão C++/OpenGL: Denise Burgarelli Duczmal e Rodney Josué Biezuner
   Data: Abril de 2004
______________________________________________________________________________*/



#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <gl/glu.h>
#include <math.h>
#include <stdlib.h>
#include "funcoes.h"


/* ************************************************************************** */
/*                     Funções gráficas gerais                                */
/******************************************************************************/

//_____________________________________________________________________________/
//  Desenha uma reta dados os pontos inicial e final em coordenadas inteiras.  /
//_____________________________________________________________________________/
void drawLine ( int x0, int y0, int x1, int y1 )
{
    glBegin(GL_LINES);
        glVertex2i(600*x0,600*y0);
        glVertex2i(600*x1,600*y1);
    glEnd();
}

//_____________________________________________________________________________/
//  Desenha uma reta dados os pontos inicial e final em coordenadas double.    /
//_____________________________________________________________________________/
void drawLine ( double x0, double y0, double x1, double y1 )
{
    glBegin(GL_LINES);
        glVertex2d(600*x0,600*y0);
        glVertex2d(600*x1,600*y1);
    glEnd();
}

//_____________________________________________________________________________/
//  Desenha um retângulo dados os vértices superior esquerdo e inferior direito
//  em coordenadas int.                                                     /
//_____________________________________________________________________________/
void drawRectangle ( int x0, int y0, int x1, int y1 )
{
    glRecti(x0, y0, x1, y1);
}

//_____________________________________________________________________________/
//  Desenha um retângulo dados os vértices superior esquerdo e inferior direito
//  em coordenadas double.                                                     /
//_____________________________________________________________________________/
void drawRectangle ( double x0, double y0, double x1, double y1 )
{
    glRectd(x0, y0, x1, y1);
}

//_____________________________________________________________________________/
//  Define o array tableOfColors que é uma tabela das cores no sistema RGB
//  a serem usadas pelo programa inteiro.                                      /
//_____________________________________________________________________________/
float tableOfColors[107][3]; //tabela das 108 cores usadas

void defineTableOfColors()
{   
    /***
    0 - preto
    1 - vermelho
    2 - amarelo
    3 - verde
    4 - azul claro
    5 - azul
    6 - violeta
    7 - branco
    ***/
    
    //Preto
    tableOfColors[0][0] = 0.0;
    tableOfColors[0][1] = 0.0;
    tableOfColors[0][2] = 0.0;
    //Vermelho
    tableOfColors[1][0] = 1.0;
    tableOfColors[1][1] = 0.0;
    tableOfColors[1][2] = 0.0;
    //Amarelo
    tableOfColors[2][0] = 1.0;
    tableOfColors[2][1] = 1.0;
    tableOfColors[2][2] = 0.0;
    //Verde
    tableOfColors[3][0] = 0.0;
    tableOfColors[3][1] = 1.0;
    tableOfColors[3][2] = 0.0;
    //Azul Claro
    tableOfColors[4][0] = 0.0;
    tableOfColors[4][1] = 1.0;
    tableOfColors[4][2] = 1.0;
    //Azul
    tableOfColors[5][0] = 0.0;
    tableOfColors[5][1] = 0.0;
    tableOfColors[5][2] = 1.0;
    //Violeta
    tableOfColors[6][0] = 1.0;
    tableOfColors[6][1] = 0.0;
    tableOfColors[6][2] = 1.0;
    //Branco
    tableOfColors[7][0] = 1.0;
    tableOfColors[7][1] = 1.0;
    tableOfColors[7][2] = 1.0;
    
    //Outras cores tendendo a cinza???????????????????????????????????????
    for( int m = 0; m < 100; m++ )
    {
      if ( m < 50 )
      {
            tableOfColors[m+8][0] = 0.01*m;
            tableOfColors[m+8][1] = 0.01*m;
            tableOfColors[m+8][2] = 0.02*m;
      }
      else 
      {
            tableOfColors[m+8][0] = 0.01*m;
            tableOfColors[m+8][1] = 0.01*m;
            tableOfColors[m+8][2] = 1.0;
      }
    }
}   

//_____________________________________________________________________________/
//              Define a cor que será usada para desenhar.                     /
//_____________________________________________________________________________/    
void setColor (int color)
{
    glColor3f(tableOfColors[color][0], tableOfColors[color][1], tableOfColors[color][2]);
}

//_____________________________________________________________________________/
//     Desenha um disco colorido com centro e cor especificados.               /
//_____________________________________________________________________________/    
void drawColorDisk ( double x, double y, int color )
{
    glColor3f(tableOfColors[color][0], tableOfColors[color][1], tableOfColors[color][2]);
    glPointSize(4.0);
    glBegin(GL_POINTS);
        glVertex2d(600*x,600*y);
    glEnd();
    //glPointSize(0.0);
}

//_____________________________________________________________________________/
//    Funções para apagar a tela durante refinamento e desrefinamento.         /
//_____________________________________________________________________________/
void clear()
{
   glClear(GL_COLOR_BUFFER_BIT); 
}

void clear_graf()
{
   glClear(GL_COLOR_BUFFER_BIT); //Não sabemos se esta função intencionava apagar apenas uma área
}

void flush()
{
    glFlush();    
}




/* ************************************************************************** */
/*          Funcoes gráficas especiais usadas pela malha adaptativa           */
/******************************************************************************/

//_____________________________________________________________________________/
/* Desenha a configuração
                            _____
                            _____
                            _____
                    
   com a cor especificada. O ponto de cooordenadas especificadas situa-se 
   aproximadamente no meio da configuração.
*/
//_____________________________________________________________________________/
void  drawE ( double x,  double y,  int color)
{
    int i,j;
    
    i = (int)x;
    j = (int)y; 
    
    setColor(color);
    
    drawLine(i-1,j-1,i+1,j-1);
    drawLine(i-1,j,i+1,j);
    drawLine(i-1,j+1,i+1,j+1);
    
    setColor(3); //cor verde
       
}
       
//_____________________________________________________________________________/
/* Desenha a configuração
                            _____
                            
                            _____
                    
   com a cor preta e a configuração (em forma de cruz, sem espaços)
                              |
                            -----
                              |  
   com a cor especificada. O ponto cujas cooordenadas são passadas à função
   situa-se aproximadamente no meio destas configurações.
*/
//_____________________________________________________________________________/
void drawEPlus ( double x,  double y,  int color)
{
    int i,j;
    
    i=(int)x;
    j=(int)y; 
    
    setColor(0); //cor preta
    
    drawLine(i-1,j-1,i+1,j-1);
    drawLine(i-1,j+1,i+1,j+1); 

    setColor(color);
       
    drawLine(i-1,j,i+1,j);
    drawLine(i,j-1,i,j+1);
   
    setColor(3); //cor verde
}
   
       
//_____________________________________________________________________________/
/* Desenha a configuração
                            _____
                            _____
                            _____
                    
   com a cor preta e a configuração (em forma de X, sem espaços)
                            \   /
                             \/
                            -----
                             /\ 
                            /  \
   com a cor especificada. O ponto cujas cooordenadas são passadas à função
   situa-se aproximadamente no meio destas configurações.
*/
//_____________________________________________________________________________/
/*
void drawEX ( double x,  double y,  int color)
{
    int i,j;
    
    i = (int)x;
    j = (int)y; 
    
    setColor(0); //cor preta
    
    
    drawLine(i-1,j-1,i+1,j-1);
    drawLine(i-1,j,i+1,j);
    drawLine(i-1,j+1,i+1,j+1);

    setColor(color);
    
    drawLine(i-1,j-1,i+1,j+1);
    drawLine(i-1,j+1,i+1,j-1);
    
    setColor(3); //cor verde
   
}
*/   
     
/******************************************************************************/
/*          Fim das funcoes graficas usadas pela malha adaptativa             */
/******************************************************************************/



/*********************** FIM DO PROGRAMA graphics.cpp**************************/

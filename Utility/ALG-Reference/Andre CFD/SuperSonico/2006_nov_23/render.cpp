/* -------------------------------------------------------------------
 *   render.cpp: funcoes para desenhar os graficos finais na tela
 *
 * ------------------------------------------------------------------- */

#include "functions.h"

  // Para linux
//#include <glut.h>         // glut.h includes gl.h and glu.h


// glut para Windows
#include <gl/glut.h>
#include <gl/gl.h>

#include <cmath>
#include <iostream>

#include "constants.h"

/* ----------------------------------------------
    Desenha a malha com suas celulas
------------------------------------------------ */
void desenharMalha(Grid *PgridDada)
{
   Cell *gridCell;

    // Percorre todas as celulas
    for(gridCell = PgridDada->firstGridCell; 
        gridCell != 0; 
        gridCell = gridCell->next)
    {  
      if( gridCell->active )
      {
        // ---------------- Desenha malha ---------------------
        // Desenha bordas das celulas no dominio
    
        glColor3f(0.0f,0.0f,0.0f);  // Preto
        drawLine((gridCell->centerX-gridCell->halfFaceLength)/(PgridDada->sideLength),
                 (gridCell->centerY+gridCell->halfFaceLength)/(PgridDada->sideLength), 
                 0,
                    (gridCell->centerX+gridCell->halfFaceLength)/(PgridDada->sideLength),
                    (gridCell->centerY+gridCell->halfFaceLength)/(PgridDada->sideLength),
                    0);

        drawLine((gridCell->centerX+gridCell->halfFaceLength)/(PgridDada->sideLength),
                 (gridCell->centerY+gridCell->halfFaceLength)/(PgridDada->sideLength), 
                 0,
                    (gridCell->centerX+gridCell->halfFaceLength)/(PgridDada->sideLength),
                    (gridCell->centerY-gridCell->halfFaceLength)/(PgridDada->sideLength),
                    0);

        drawLine((gridCell->centerX+gridCell->halfFaceLength)/(PgridDada->sideLength),
                 (gridCell->centerY-gridCell->halfFaceLength)/(PgridDada->sideLength), 
                 0,
                    (gridCell->centerX-gridCell->halfFaceLength)/(PgridDada->sideLength),
                    (gridCell->centerY-gridCell->halfFaceLength)/(PgridDada->sideLength),
                    0);

        drawLine((gridCell->centerX-gridCell->halfFaceLength)/(PgridDada->sideLength),
                 (gridCell->centerY-gridCell->halfFaceLength)/(PgridDada->sideLength), 
                 0,
                    (gridCell->centerX-gridCell->halfFaceLength)/(PgridDada->sideLength),
                    (gridCell->centerY+gridCell->halfFaceLength)/(PgridDada->sideLength),
                    0);
      } 
   }
}

/* -----------------------------------------------
    Desenha a placa plana na borda sul (enfeite)
-------------------------------------------------- */
void desenharPlacaPlana(Grid *grid)
{    
    // Desenhar placa plana na forma de poligono
    glBegin(GL_POLYGON);
        glColor3f(0.5f,0.5f,0.5f);       
        glVertex3d(-(LARGURA_MALHA/2.0),
                   -(ALTURA_MALHA/2),
                   0);
        glColor3f(0.4f,0.4f,0.4f);
        glVertex3d(LARGURA_MALHA*(0.15)-(LARGURA_MALHA/2.0),
                   -1.1*(ALTURA_MALHA/2),
                   0);
        glColor3f(0.55f,0.55f,0.55f);
        glVertex3d((LARGURA_MALHA/2),
                   -1.1*(ALTURA_MALHA/2),
                   0);
        glColor3f(0.38f,0.38f,0.38f);
        glVertex3d((LARGURA_MALHA/2),
                   -(ALTURA_MALHA/2),
                   0);
    glEnd();
}                           

/* -----------------------------------------------
    Desenha valores da densidade em cada celula
-------------------------------------------------- */
void desenharDensidade(Grid *grid)
{
    Cell *gridCell;

    // Percorre todas as celulas
    for(gridCell = grid->firstGridCell; 
        gridCell != 0; 
        gridCell = gridCell->next)
    {
      if( gridCell->active )
      {
        // Cor de acordo com o valor da densidade
        if( gridCell->rhoAtual >= 0  // Positivo
        && fabs(gridCell->rhoAtual) < (2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)) )
        {
           glColor3f(
              0.0f,
                 0.008f +
                 0.88*(gridCell->rhoAtual)
                 /(2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)),
                    (gridCell->rhoAtual)*(gridCell->rhoAtual)
                    /(2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)));
        }

        if( gridCell->rhoAtual < 0  // Negativo
        && fabs(gridCell->rhoAtual) < (2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)) )
        {
            glColor3f(
               0.0f,
                  (gridCell->rhoAtual*gridCell->rhoAtual)
                  /(2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)),
                     0.1f);
        }

        // Estourado
        if( fabs(gridCell->rhoAtual) > (2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)) )
            glColor3f(1.0f,0.0f,0.0f);

        // Desenha os quadrados
        glBegin(GL_QUADS);       
            glVertex3d(LARGURA_MALHA*
                    (gridCell->centerX-gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(LARGURA_MALHA/2.0),

                    ALTURA_MALHA*
                    (gridCell->centerY+gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(ALTURA_MALHA/2),

                    0);
        
            glVertex3d(LARGURA_MALHA*
                    (gridCell->centerX+gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(LARGURA_MALHA/2.0),

                    ALTURA_MALHA*
                    (gridCell->centerY+gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(ALTURA_MALHA/2),

                    0);

            glVertex3d(LARGURA_MALHA*
                    (gridCell->centerX+gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(LARGURA_MALHA/2.0),

                    ALTURA_MALHA*
                    (gridCell->centerY-gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(ALTURA_MALHA/2),

                    0);

            glVertex3d(LARGURA_MALHA*
                    (gridCell->centerX-gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(LARGURA_MALHA/2.0),

                    ALTURA_MALHA*
                    (gridCell->centerY-gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(ALTURA_MALHA/2),

                    0);
        glEnd();

      }    
    }
}

/* -----------------------------------------------
    Desenha valores da temperatura em cada celula
-------------------------------------------------- */
void desenharTemperatura(Grid *grid)
{
    Cell *gridCell;

    // Percorre todas as celulas
    for(gridCell = grid->firstGridCell; 
        gridCell != 0; 
        gridCell = gridCell->next)
    {
      if( gridCell->active )
      {
        // Cor de acordo com o valor da temperatura
        if( gridCell->TAtual > 273.0   // Acima de 0 celsius
        &&  fabs(gridCell->TAtual - 273.0) < (70.0*(TEMPERATURA_LIVRE-273.0)) )
        {
           glColor3f(              
              (gridCell->TAtual - 273.0)*(gridCell->TAtual - 273.0)/(70.0*(TEMPERATURA_LIVRE-273.0)),
                 1.3*(gridCell->TAtual - 273.0)/(70.0*(TEMPERATURA_LIVRE-273.0)),
                    0.0f);
        }

        if( gridCell->TAtual < 273.0   // Abaixo de 0 celsius
        &&  fabs(gridCell->TAtual - 273.0) < (70.0*(TEMPERATURA_LIVRE-273.0)) )
        {
           glColor3f(              
              0.0f,
                 1.8*fabs(gridCell->TAtual - 273.0)/(70.0*(TEMPERATURA_LIVRE-273.0)),
                    fabs(gridCell->TAtual - 273.0)*fabs(gridCell->TAtual - 273.0)/(70.0*(TEMPERATURA_LIVRE-273.0)));
        }

        // Estourado
        if( fabs(gridCell->TAtual - 273.0) > (70.0*(TEMPERATURA_LIVRE-273.0)) )
            glColor3f(0.0f,1.0f,0.0f);

        // Desenha os quadrados
        glBegin(GL_QUADS);       
            glVertex3d(LARGURA_MALHA*
                    (gridCell->centerX-gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(LARGURA_MALHA/2.0),

                    ALTURA_MALHA*
                    (gridCell->centerY+gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(ALTURA_MALHA/2),

                    0);
        
            glVertex3d(LARGURA_MALHA*
                    (gridCell->centerX+gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(LARGURA_MALHA/2.0),

                    ALTURA_MALHA*
                    (gridCell->centerY+gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(ALTURA_MALHA/2),

                    0);

            glVertex3d(LARGURA_MALHA*
                    (gridCell->centerX+gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(LARGURA_MALHA/2.0),

                    ALTURA_MALHA*
                    (gridCell->centerY-gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(ALTURA_MALHA/2),

                    0);

            glVertex3d(LARGURA_MALHA*
                    (gridCell->centerX-gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(LARGURA_MALHA/2.0),

                    ALTURA_MALHA*
                    (gridCell->centerY-gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(ALTURA_MALHA/2),

                    0);
        glEnd();
      }    
    }
}

/* -----------------------------------------------
    Desenha valores da pressao em cada celula
-------------------------------------------------- */
void desenharPressao(Grid *grid)
{
    Cell *gridCell;

    // Percorre todas as celulas
    for(gridCell = grid->firstGridCell; 
        gridCell != 0; 
        gridCell = gridCell->next)
    {
      if( gridCell->active )
      {
        // Cor de acordo com o valor da densidade
        if( fabs(gridCell->pAtual) < (5.0*PRESSAO_LIVRE) )
        {
           glColor3f(              
              0.0f,
                 (gridCell->pAtual)/(5.0*PRESSAO_LIVRE),
                    0.1*(gridCell->pAtual)/(5.0*PRESSAO_LIVRE));
        }

        // Estourado
        if( fabs(gridCell->pAtual) > (5.0*PRESSAO_LIVRE) )
            glColor3f(0.7f,0.7f,0.7f);

        // Desenha os quadrados
        glBegin(GL_QUADS);       
            glVertex3d(LARGURA_MALHA*
                    (gridCell->centerX-gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(LARGURA_MALHA/2.0),

                    ALTURA_MALHA*
                    (gridCell->centerY+gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(ALTURA_MALHA/2),

                    0);
        
            glVertex3d(LARGURA_MALHA*
                    (gridCell->centerX+gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(LARGURA_MALHA/2.0),

                    ALTURA_MALHA*
                    (gridCell->centerY+gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(ALTURA_MALHA/2),

                    0);

            glVertex3d(LARGURA_MALHA*
                    (gridCell->centerX+gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(LARGURA_MALHA/2.0),

                    ALTURA_MALHA*
                    (gridCell->centerY-gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(ALTURA_MALHA/2),

                    0);

            glVertex3d(LARGURA_MALHA*
                    (gridCell->centerX-gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(LARGURA_MALHA/2.0),

                    ALTURA_MALHA*
                    (gridCell->centerY-gridCell->halfFaceLength)
                        /
                    (grid->sideLength)
                    -(ALTURA_MALHA/2),

                    0);
        glEnd();
      }   
    }
}
  

/* -----------------------------------------------
    Desenha valores de uma variavel em cada celula
-------------------------------------------------- */
void desenharValores(Grid *grid, int variavel)
{
    Cell *gridCell;

    char str[5];

    glColor3f(0.6f,0.6f,0.6f);
    // Percorre todas as celulas
    for(gridCell = grid->firstGridCell; 
        gridCell != 0; 
        gridCell = gridCell->next)
    {
      if( gridCell->active )
      {
        // Preencher string com valores
        switch(variavel){
            default:
            case 1:  //imprimir densidade
                sprintf(str, "%1.3f", gridCell->rhoAtual );    
            break;

            case 2:  //imprimir temperatura
                sprintf(str, "%1.0f", gridCell->TAtual );    
            break;

            case 3:  //imprimir velocidade
                sprintf(str, "%1.0f", (gridCell->uAtual+gridCell->vAtual) );    
            break;

            case 4:  //imprimir pressao
                sprintf(str, "%1.0f", (gridCell->pAtual) );    
            break;
        }

	    drawString(str,GLUT_BITMAP_HELVETICA_10,
            LARGURA_MALHA*(gridCell->centerX- (gridCell->halfFaceLength/1.6))/(grid->sideLength)-(LARGURA_MALHA/2.0),
            ALTURA_MALHA*gridCell->centerY/(grid->sideLength)-(ALTURA_MALHA/2.0),
            0);
      }
    }
}

/* -----------------------------------------------
    Desenha o valor do tempo atual
-------------------------------------------------- */
void desenharTempo(Grid *grid, double zoom)
{
    char string_tempo[9];
    char rotuloT[7] = "Tempo:";
    char rotuloS[2] = "s";

    // Preencher string com valores
    sprintf(string_tempo, "%1.9f", grid->tempoAtual);  //imprimir tempo

    // Cor preta para as fontes
    glColor3f(0.0f,0.0f,0.0f);
    
      // Desenhar "Tempo:"
    drawString(rotuloT,GLUT_BITMAP_HELVETICA_18,
            -(LARGURA_TELA/1.1)/zoom,
            +(ALTURA_TELA/1.2)/zoom,
            0);
      // Desenhar o valor do tempo atual
    drawString(string_tempo,GLUT_BITMAP_HELVETICA_18,
            -(LARGURA_TELA/1.4)/zoom,
            +(ALTURA_TELA/1.2)/zoom,
            0);
      // Desenhar "s"
    drawString(rotuloS,GLUT_BITMAP_HELVETICA_18,
            -(LARGURA_TELA/2.41)/zoom,
            +(ALTURA_TELA/1.2)/zoom,
            0);
}

/* -----------------------------------------------
    Desenha um aviso de pausa
-------------------------------------------------- */
void desenharPausa(Grid *grid, double zoom)
{
    char rotulo[13] = "- Em pausa -";

    // Cor preta para as fontes
    glColor3f(0.0f,0.0f,0.0f);
    
      // Desenhar "Tempo:"
    drawString(rotulo,GLUT_BITMAP_HELVETICA_18,
            -(LARGURA_TELA/6.0)/zoom,
            +(ALTURA_TELA/1.1)/zoom,
            0.0);

}

/*--------------------------------------------------------------------------
                     Desenha o vetor velocidade de uma célula                   
-----------------------------------------------------------------------------*/
void desenhaVetor( double fatorPosicao, double fatorTamanho, 
                   double u, double v, double x, double y )
{
     double xA = x;
     double yA = y;

     double xB = x + (u/(4.0*(VEL_SOM)))*fatorTamanho; 
     double yB = y + (v/(2.5*(VEL_SOM)))*fatorTamanho;

     // O vetor é desenhado em vermelho
     glColor3f( 0.5f, 0.2f, 0.0f );

     // Corpo do vetor                    
     glBegin( GL_LINES );
            glVertex3d( LARGURA_MALHA*xA/fatorPosicao - (LARGURA_MALHA/2.0),
                        ALTURA_MALHA*yA/fatorPosicao - (ALTURA_MALHA/2.0), 
                        0 );                          
            glVertex3d( LARGURA_MALHA*xB/fatorPosicao - (LARGURA_MALHA/2.0),
                        ALTURA_MALHA*yB/fatorPosicao - (ALTURA_MALHA/2.0), 
                        0 );
     glEnd();

     // Ponta do vetor
     glPointSize(3);
     glBegin( GL_POINTS );
            glVertex3d( LARGURA_MALHA*xB/fatorPosicao - (LARGURA_MALHA/2.0),
                        ALTURA_MALHA*yB/fatorPosicao - (ALTURA_MALHA/2.0), 
                        0 );
     glEnd();     
}


/*------------------------------------------------------------------------------
                    Desenha o campo vetorial de velocidades                     
------------------------------------------------------------------------------*/
void desenhaCampoVetorial( Grid *grid )
{
     Cell *gridCell = grid->firstGridCell;
     while( gridCell != 0 )
     {
       if( gridCell->active )
       { 
         desenhaVetor( grid->sideLength, 1.5*(gridCell->halfFaceLength), 
                       gridCell->uAtual, gridCell->vAtual, 
                       gridCell->centerX, gridCell->centerY );
       
       } 
         gridCell = gridCell->next;      
     }
}


/* -------------------------------------------------------------------
 *   render.cpp: funcoes para desenhar os graficos finais na tela
 *
 * ------------------------------------------------------------------- */

#include "functions.h"

// glut para Windows e Linux
#include <GL/glut.h>
#include <GL/gl.h>

#include <cmath>
#include <iostream>
    using std::cout;

#include "constants.h"
#include "erros.h"

/* ----------------------------------------------
    Desenha a malha com suas celulas
------------------------------------------------ */
void desenharMalha( Grid *malha )
{
   Cell *celula;

    // Percorre todas as celulas
    for(celula = malha->firstGridCell; 
        celula != 0; 
        celula = celula->next)
    {  
        // Somente para celulas ativas
        if( celula->active )
        {
            // ---------------- Desenha malha ---------------------
            // Desenha bordas das celulas no dominio

            glColor3f(0.0f,0.0f,0.0f);  // Preto
            drawLine(-malha->sideLength/2+(celula->centerX - celula->halfFaceLength),
                    -malha->sideLength/2+(celula->centerY + celula->halfFaceLength), 
                    0,
                        -malha->sideLength/2+(celula->centerX + celula->halfFaceLength),
                        -malha->sideLength/2+(celula->centerY + celula->halfFaceLength),
                        0);
    
            drawLine(-malha->sideLength/2+(celula->centerX + celula->halfFaceLength),
                    -malha->sideLength/2+(celula->centerY + celula->halfFaceLength), 
                    0,
                        -malha->sideLength/2+(celula->centerX + celula->halfFaceLength),
                        -malha->sideLength/2+(celula->centerY - celula->halfFaceLength),
                        0);
    
            drawLine(-malha->sideLength/2+(celula->centerX + celula->halfFaceLength),
                    -malha->sideLength/2+(celula->centerY - celula->halfFaceLength), 
                    0,
                        -malha->sideLength/2+(celula->centerX - celula->halfFaceLength),
                        -malha->sideLength/2+(celula->centerY -celula->halfFaceLength),
                        0);
    
            drawLine(-malha->sideLength/2+(celula->centerX - celula->halfFaceLength),
                    -malha->sideLength/2+(celula->centerY - celula->halfFaceLength), 
                    0,
                        -malha->sideLength/2+(celula->centerX - celula->halfFaceLength),
                        -malha->sideLength/2+(celula->centerY + celula->halfFaceLength),
                        0);
    
            // Marcar X quando e' volume ficticio
            if(celula->volumeFicticio == true){
                drawLine(-malha->sideLength/2+(celula->centerX - celula->halfFaceLength),
                        -malha->sideLength/2+(celula->centerY + celula->halfFaceLength), 
                        0,
                            -malha->sideLength/2+(celula->centerX + celula->halfFaceLength),
                            -malha->sideLength/2+(celula->centerY - celula->halfFaceLength),
                            0);
    
                drawLine(-malha->sideLength/2+(celula->centerX - celula->halfFaceLength),
                        -malha->sideLength/2+(celula->centerY - celula->halfFaceLength), 
                        0,
                            -malha->sideLength/2+(celula->centerX + celula->halfFaceLength),
                            -malha->sideLength/2+(celula->centerY + celula->halfFaceLength),
                            0);            
            } 
        }
    }
}                          

/* -----------------------------------------------
    Desenha valores da densidade em cada celula
-------------------------------------------------- */
void desenharDensidade( Grid *malha )
{
    Cell *celula;

    // Percorre todas as celulas
    for(celula = malha->firstGridCell; 
        celula != 0; 
        celula = celula->next)
    {
        if(celula->volumeFicticio == false && celula->active )
        {
            // Cor de acordo com o valor da densidade
            if( celula->rhoAtual >= 0  // Positivo
                && fabs(celula->rhoAtual) 
                    < (2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)) )
            {
            glColor3f(
                0.0f ,
                    0.008f +
                    0.88*(celula->rhoAtual)
                    /(2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)) ,
                        (celula->rhoAtual)*(celula->rhoAtual)
                        /(2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)));
            }
            // Estourado
            else 
                glColor3f(1.0f,0.0f,1.0f);
            
            // Desenha os quadrados
            glBegin(GL_QUADS);       
                glVertex3d(
                        (celula->centerX-celula->halfFaceLength)        
                        -(malha->sideLength/2.0),
                            
                        (celula->centerY+celula->halfFaceLength)
                        -(malha->sideLength/2.0),
                                
                        0);
            
                glVertex3d(
                        (celula->centerX+celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        (celula->centerY+celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        0);
    
                glVertex3d(
                        (celula->centerX+celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        (celula->centerY-celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        0);
    
                glVertex3d(
                        (celula->centerX-celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        (celula->centerY-celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        0);
            glEnd();
        }    
    }
}

/* -----------------------------------------------
    Desenha valores da temperatura em cada celula
-------------------------------------------------- */
void desenharTemperatura( Grid *malha )
{
    Cell *celula;

    // Percorre todas as celulas
    for(celula = malha->firstGridCell; 
        celula != 0; 
        celula = celula->next)
    {
        if(celula->volumeFicticio == false && celula->active)
        {
            // Cor de acordo com o valor da densidade
            if( celula->TAtual >= 273.0   // Acima de 0 celsius
                &&  fabs(celula->TAtual - 273.0) 
                    < (70.0*(TEMPERATURA_LIVRE-273.0)) )
            {
            glColor3f(              
                4.0*(celula->TAtual - 273.0)/(70.0*(TEMPERATURA_LIVRE-273.0)),
                    0.004*(celula->TAtual - 273.0)*(celula->TAtual - 273.0)
                    /(70.0*(TEMPERATURA_LIVRE-273.0))
                    + 800.0*(TEMPERATURA_LIVRE-273.0)/((celula->TAtual - 90.0)
                    *(celula->TAtual - 90.0)),
                        800.0*(TEMPERATURA_LIVRE-273.0)/((celula->TAtual - 90.0)
                        *(celula->TAtual - 90.0)) );
            }
            else if( celula->TAtual < 273.0   // Abaixo de 0 celsius
                &&  fabs(celula->TAtual - 273.0) 
                    < (70.0*(TEMPERATURA_LIVRE-273.0)) )
            {
            glColor3f(              
                0.0f,
                    800.0*(TEMPERATURA_LIVRE-273.0)
                    /(fabs(celula->TAtual + 456.0)
                    *fabs(celula->TAtual + 456.0)),
                        fabs(celula->TAtual - 273.0)
                        /(10.0*(TEMPERATURA_LIVRE-273.0))
                        + 800.0*(TEMPERATURA_LIVRE-273.0)
                        /(fabs(celula->TAtual + 456.0)
                        *fabs(celula->TAtual + 456.0)) );
            }
            // Estourado
            else
                glColor3f(1.0f,0.0f,1.0f);
    
            // Desenha os quadrados
            glBegin(GL_QUADS);       
                glVertex3d(
                        (celula->centerX-celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        (celula->centerY+celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        0);
            
                glVertex3d(
                        (celula->centerX+celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        (celula->centerY+celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        0);
    
                glVertex3d(
                        (celula->centerX+celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        (celula->centerY-celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        0);
    
                glVertex3d(
                        (celula->centerX-celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        (celula->centerY-celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        0);
            glEnd();    
        }
    }
}

/*------------------------------------------------------------------------------
                    Desenha o campo vetorial de velocidades
------------------------------------------------------------------------------*/
void desenharVelocidade( Grid *malha, double valorMax = 0.0 )
{
    double tamanhoHorizontal, tamanhoVertical;
    double xA, yA,
           xB, yB,
           xC, yC,
           xD, yD;

    Cell *celula;

    // O tamanho maximo precisa ser positivo
    if( valorMax < 0.0 )
    {
        valorMax = fabs(valorMax);
    }

    // Encontrar o maior valor u ou v entre todas as celulas
    for(celula = malha->firstGridCell;
        celula != NULL;
        celula = celula->next)
    {
        if(celula->volumeFicticio == false && celula->active)
        {
            // Maior u
            if( fabs(celula->uAtual) > fabs(valorMax) ) 
                valorMax = fabs(celula->uAtual);
    
            // Maior v
            if( fabs(celula->vAtual) > fabs(valorMax) )
                valorMax = fabs(celula->vAtual);
        }
    }

    // Desenha um vetor em cada celula
    for(celula = malha->firstGridCell;
        celula != NULL;
        celula = celula->next)
    {
        if(celula->volumeFicticio == false && celula->active)
        {
            tamanhoHorizontal = ( celula->uAtual / ( (valorMax != 0.0)? valorMax : 1.0 ) )
                            * 1.5 * (celula->halfFaceLength);
            tamanhoVertical = ( celula->vAtual / ( (valorMax != 0.0)? valorMax : 1.0 ) )
                            * 1.5 * (celula->halfFaceLength);
    
            // Inicio do segmento do vetor
            xA = celula->centerX - 0.5*tamanhoHorizontal;
            yA = celula->centerY - 0.5*tamanhoVertical;
    
            // Final do segmento do vetor
            xB = celula->centerX + 0.5*tamanhoHorizontal; 
            yB = celula->centerY + 0.5*tamanhoVertical;
    
            // O triangulo na ponta do vetor tem vertices B, C, D
            xC = xB + (-0.25*tamanhoHorizontal)*COS30 
                        + (-0.25*tamanhoVertical)*SEN30;
            yC = yB - (-0.25*tamanhoHorizontal)*SEN30 
                        + (-0.25*tamanhoVertical)*COS30;
    
            xD = xB + (-0.25*tamanhoHorizontal)*COS30 
                        - (-0.25*tamanhoVertical)*SEN30;
            yD = yB + (-0.25*tamanhoHorizontal)*SEN30 
                        + (-0.25*tamanhoVertical)*COS30;
    
            // O vetor e' desenhado em azul escuro
            glColor3f( 0.0f, 0.08f, 0.35f );
    
            // Corpo do vetor                    
            glBegin( GL_LINES );
                    glVertex3d( xA - (malha->sideLength/2.0),
                                yA - (malha->sideLength/2.0), 
                                0 );                          
                glVertex3d( xB - (malha->sideLength/2.0),
                            yB - (malha->sideLength/2.0), 
                            0 );
            glEnd();
    
            // desenhando a ponta do vetor.
            glBegin( GL_TRIANGLES );   
                glVertex3d( xB - (malha->sideLength/2.0),
                            yB - (malha->sideLength/2.0),
                            0.0 );
                glVertex3d( xC - (malha->sideLength/2.0),
                            yC - (malha->sideLength/2.0),
                            0.0 );
                glVertex3d( xD - (malha->sideLength/2.0),
                            yD - (malha->sideLength/2.0),
                            0.0 );
            glEnd();
        }
    }
}

/* -----------------------------------------------
    Desenha valores da pressao em cada celula
-------------------------------------------------- */
void desenharPressao( Grid *malha )
{
    Cell *celula;

    // Percorre todas as celulas
    for(celula = malha->firstGridCell; 
        celula != 0; 
        celula = celula->next)
    {
        if(celula->volumeFicticio == false && celula->active)
        {
            // Cor de acordo com o valor da densidade
            if( fabs( (celula->pAtual)*(celula->pAtual) ) 
                < (8.0*PRESSAO_LIVRE*PRESSAO_LIVRE)
            && celula->pAtual > 0 )
            {
            glColor3f(              
                0.0f,
                    (celula->pAtual)*(celula->pAtual)
                    /(8.0*PRESSAO_LIVRE*PRESSAO_LIVRE)
                    + (PRESSAO_LIVRE*PRESSAO_LIVRE)
                    /(((celula->pAtual)+150000)*((celula->pAtual)+150000)),
                        (PRESSAO_LIVRE*PRESSAO_LIVRE)
                        /(((celula->pAtual)+150000)*((celula->pAtual)+150000)) );
            }
            // Estourado
            else
                glColor3f(1.0f,0.0f,1.0f);
    
            // Desenha os quadrados
            glBegin(GL_QUADS);       
                glVertex3d(
                        (celula->centerX-celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        (celula->centerY+celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        0);
            
                glVertex3d(
                        (celula->centerX+celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        (celula->centerY+celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        0);
    
                glVertex3d(
                        (celula->centerX+celula->halfFaceLength)
                        -(malha->sideLength/2.0),
                        
                        (celula->centerY-celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        0);
    
                glVertex3d(
                        (celula->centerX-celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        (celula->centerY-celula->halfFaceLength)
                        -(malha->sideLength/2.0),
    
                        0);
            glEnd();    
        }
    }
}


/* -----------------------------------------------
    Desenha valores de uma variavel em cada celula
-------------------------------------------------- */
void desenharValores( Grid *malha , int QualGrafico )
{
    Cell *malhaCell;
    char valor[5];

    glColor3f(0.6f,0.6f,0.6f);
    // Percorre todas as celulas
    for(malhaCell = malha->firstGridCell; 
        malhaCell != 0; 
        malhaCell = malhaCell->next)
    {
        if(malhaCell->volumeFicticio == false && malhaCell->active)
        {
            // Preencher string com valores
            switch(QualGrafico)
            {
                case 1:  //imprimir densidade
                    sprintf(valor, "%1.2f", (malhaCell->rhoAtual) );    
                break;

                case 2:  //imprimir temperatura
                    sprintf(valor, "%1.0f", malhaCell->TAtual );    
                break;

                case 3:  //imprimir velocidade
                    sprintf(valor, "%1.1f", pow(    AO_QUADRADO(malhaCell->uAtual) 
                                                + AO_QUADRADO(malhaCell->vAtual), 0.5) );    
                break;          

                case 4:  //imprimir pressao
                    sprintf(valor, "%1.0f", (malhaCell->pAtual) );
                break;

                default:
                break;
            }

            drawString(valor,GLUT_BITMAP_HELVETICA_10,
                ( malhaCell->centerX - ((malhaCell->halfFaceLength)/1.6) ) - ( malha->sideLength/2.0 ),
                malhaCell->centerY - ( malha->sideLength/2.0 ),
                0);
        }
    }
}

/* -----------------------------------------------
    Desenha o valor do tempo atual
-------------------------------------------------- */
void desenharTempo( Grid *malha, double zoom, 
                    double deslocamentoX, double deslocamentoY )
{
    char string_tempo[13];

    // Preencher string com valor do tempo atual
    sprintf( string_tempo, "%1.11f", malha->tempoAtual );    
                       
    // Cor preta para as fontes
    glColor3f(0.0f,0.0f,0.0f);
    
    // Desenhar "Tempo:"
    drawString("Tempo:",GLUT_BITMAP_HELVETICA_18,
            - (0.970/zoom) - deslocamentoX,            
            + (0.695/zoom) - deslocamentoY,             
            0);
    // Desenhar o valor do tempo atual
    drawString(string_tempo,GLUT_BITMAP_HELVETICA_18,
            - (0.780/zoom) - deslocamentoX,
            + (0.695/zoom) - deslocamentoY,
            0);
      // Desenhar "s"
    drawString("s",GLUT_BITMAP_HELVETICA_18,
            - (0.455/zoom) - deslocamentoX,
            + (0.695/zoom) - deslocamentoY,
            0);

    glColor3f(0.7f,0.7f,0.7f);
    // Desenhar fundo (retangulo cinza) para a string
    glBegin(GL_QUADS);       
            glVertex3d(- (0.99/zoom) - deslocamentoX,
                            + (0.74/zoom) - deslocamentoY,
                                0.0);
        
            glVertex3d(- (0.42/zoom) - deslocamentoX,
                            + (0.74/zoom) - deslocamentoY,
                                0.0);

            glVertex3d(- (0.42/zoom) - deslocamentoX,
                            + (0.68/zoom) - deslocamentoY,
                                 0.0);

            glVertex3d(- (0.99/zoom) - deslocamentoX,
                            + (0.68/zoom) - deslocamentoY,
                                 0.0);
    glEnd(); 

}/**/

/* -----------------------------------------------
    Desenha um aviso de pausa
-------------------------------------------------- */
void desenharPausa( Grid *malha, double zoom , 
                    double deslocamentoX, double deslocamentoY )
{
    // Cor preta para as fontes
    glColor3f(0.0f,0.0f,0.0f);
    
      // Desenhar "- Em pausa -"
    drawString("- Em pausa -",GLUT_BITMAP_HELVETICA_18,
            -(0.16/zoom) - deslocamentoX,
            +(0.69/zoom) - deslocamentoY,
            0.0);

    glColor3f(0.7f,0.7f,0.7f);
    // Desenhar fundo (retangulo cinza) para a string
    glBegin(GL_QUADS);       
            glVertex3d(-(0.18/zoom) - deslocamentoX,
                           +(0.74/zoom) - deslocamentoY,
                                 0.0);      
        
            glVertex3d(+(0.13/zoom) - deslocamentoX,
                           +(0.74/zoom) - deslocamentoY,
                                 0.0);

            glVertex3d(+(0.13/zoom) - deslocamentoX,
                           +(0.67/zoom) - deslocamentoY,
                                 0.0);

            glVertex3d(-(0.18/zoom) - deslocamentoX,
                           +(0.67/zoom) - deslocamentoY,
                                 0.0);
    glEnd();

}


/*--------------------------------------------------------------------------
                         Desenha a legenda
-----------------------------------------------------------------------------*/
void desenharLegenda( Grid *malha, double zoom , 
                    double deslocamentoX, double deslocamentoY,
                    int QualGrafico )
{
    char Texto[20];
    int i = 2;

    // Desenha titulo exceto para velocidade
    if( QualGrafico != 3 )
    {
        // Cor preta para as fontes
        glColor3f(0.0f, 0.0f, 0.0f);
    
        drawString( "Legenda:", GLUT_BITMAP_HELVETICA_18,
                    (0.746/zoom) - deslocamentoX,
                    (0.692/zoom) - deslocamentoY,
                     0.0);
    }
    
    //Mostra a legenda de acordo com o grafico atual
    switch( QualGrafico )
    {
      case 1: // ----------------- DENSIDADE ---------------------------
        glColor3f(0.0f, 0.0f, 0.0f);
        // Desenhar titulo da legenda          
        drawString( "Densidade (kg / m^3)",
                    GLUT_BITMAP_HELVETICA_12,
                    (0.69/zoom) - deslocamentoX,
                    (0.65/zoom) - deslocamentoY,
                     0.0 );

        // Criar o contorno da barra
        drawLine( (0.87/zoom) - deslocamentoX,
                  (0.60/zoom) - deslocamentoY,
                  0.0,
                     (0.92/zoom) - deslocamentoX,
                     (0.60/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.92/zoom) - deslocamentoX,
                  (0.60/zoom) - deslocamentoY,
                  0.0,
                     (0.92/zoom) - deslocamentoX,
                     (0.20/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.92/zoom) - deslocamentoX,
                  (0.20/zoom) - deslocamentoY,
                  0.0,
                     (0.87/zoom) - deslocamentoX,
                     (0.20/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.87/zoom) - deslocamentoX,
                  (0.20/zoom) - deslocamentoY,
                  0.0,
                     (0.87/zoom) - deslocamentoX,
                     (0.60/zoom) - deslocamentoY,
                     0.0 );       

        // Criar uma barra gradiente da densidade
        for( i = 0; i <= 8; i++ )
        {
            if( i > 0 )
            {
            glBegin(GL_QUADS);
              glColor3f(
                0.0f ,
                    0.008f +
                    0.88*( ((double)i)*(1.22516/4.0) )
                    /(2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)) ,
                         ( ((double)i)*(1.22516/4.0) )
                        *( ((double)i)*(1.22516/4.0) )
                        /(2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)));            
                                     
                glVertex3d( (0.87/zoom) - deslocamentoX,
                            ( 0.20 + (i*0.05) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (0.92/zoom)  - deslocamentoX,
                            ( 0.20 + (i*0.05) )/zoom - deslocamentoY,
                            0.0 );

                glColor3f(
                    0.0f ,
                        0.008f +
                        0.88*( ((double)(i-1))*(1.22516/4.0) )
                        /(2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)) ,
                             ( ((double)(i-1))*(1.22516/4.0) )
                            *( ((double)(i-1))*(1.22516/4.0) )
                            /(2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)));

                glVertex3d( (0.92/zoom) - deslocamentoX,
                            ( 0.20 + ((i-1)*0.05) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (0.87/zoom) - deslocamentoX,
                            ( 0.20 + ((i-1)*0.05) )/zoom - deslocamentoY,
                            0.0 );
            glEnd();
            }
            // Rotulo dos valores
            if( i%2 == 0 )
            {
                glColor3f(0.0f,0.0f,0.0f);
                // Risco indicador
                drawLine( (0.85/zoom) - deslocamentoX,
                          ( 0.20 + (i*0.05) )/zoom - deslocamentoY, 
                          0.0,
                             (0.88/zoom) - deslocamentoX,
                             ( 0.20 + (i*0.05) )/zoom - deslocamentoY, 
                             0.0 );
            
                // Valor em forma de string
                double valorRho = ((double)i)*(1.22518/4.0);
                sprintf( Texto, "%1.3f", valorRho );

                drawString( Texto, GLUT_BITMAP_HELVETICA_12,
                            (0.765/zoom) - deslocamentoX,
                            ( 0.19 + (i*0.05) )/zoom - deslocamentoY, 
                            0.0 );    
            }
        }

        // Desenha borda para legenda "evalorMaximostourado"
        glColor3f(0.0f,0.0f,0.0f);
        drawLine( (0.87/zoom) - deslocamentoX,
                  (0.15/zoom) - deslocamentoY,
                  0.0,
                     (0.92/zoom) - deslocamentoX,
                     (0.15/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.92/zoom) - deslocamentoX,
                  (0.15/zoom) - deslocamentoY,
                  0.0,
                     (0.92/zoom) - deslocamentoX,
                     (0.10/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.92/zoom) - deslocamentoX,
                  (0.10/zoom) - deslocamentoY,
                  0.0,
                     (0.87/zoom) - deslocamentoX,
                     (0.10/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.87/zoom) - deslocamentoX,
                  (0.10/zoom) - deslocamentoY,
                  0.0,
                     (0.87/zoom) - deslocamentoX,
                     (0.15/zoom) - deslocamentoY,
                     0.0 );

        // Desenha quadrado colorido de "estourado"
        glColor3f(1.0f,0.0f,1.0f);
        glBegin(GL_QUADS);
            glVertex3d( (0.87/zoom) - deslocamentoX,
                        (0.15/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.92/zoom)  - deslocamentoX,
                        (0.15/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.92/zoom) - deslocamentoX,
                        (0.10/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.87/zoom) - deslocamentoX,
                        (0.10/zoom) - deslocamentoY,
                         0.0 );
        glEnd();

        // Desenha string de "estourado"
        glColor3f(0.0f,0.0f,0.0f);
        drawString( "Estourado",
                    GLUT_BITMAP_HELVETICA_12,
                    (0.722/zoom) - deslocamentoX,
                    (0.118/zoom) - deslocamentoY,
                     0.0 );
      break;

      case 2: // ---------------------- TEMPERATURA -----------------------
        glColor3f(0.0f, 0.0f, 0.0f);
        // Desenhar titulo da legenda          
        drawString( "Temperatura (K)",
                    GLUT_BITMAP_HELVETICA_12,
                    (0.74/zoom) - deslocamentoX,
                    (0.65/zoom) - deslocamentoY,
                     0.0 );

        // Criar o contorno da barra
        drawLine( (0.87/zoom) - deslocamentoX,
                  (0.60/zoom) - deslocamentoY,
                  0.0,
                     (0.92/zoom) - deslocamentoX,
                     (0.60/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.92/zoom) - deslocamentoX,
                  (0.60/zoom) - deslocamentoY,
                  0.0,
                     (0.92/zoom) - deslocamentoX,
                     (0.20/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.92/zoom) - deslocamentoX,
                  (0.20/zoom) - deslocamentoY,
                  0.0,
                     (0.87/zoom) - deslocamentoX,
                     (0.20/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.87/zoom) - deslocamentoX,
                  (0.20/zoom) - deslocamentoY,
                  0.0,
                     (0.87/zoom) - deslocamentoX,
                     (0.60/zoom) - deslocamentoY,
                     0.0 );       

        // Criar uma barra gradiente da temperatura
        for( i = 0; i <= 10; i++ )
        {
            if( i > 0      // PARTE FRIA
            &&  i < 3 )
            {                         
            glBegin(GL_QUADS);
                glColor3f(              
                    0.0f,
                        800.0*(TEMPERATURA_LIVRE-273.0)
                        /(fabs(i*(477.0/8.0) - 302.25)
                        *fabs(i*(477.0/8.0) - 302.25)),
                            fabs(i*(477.0/8.0) - 119.25)
                            /(10.0*(TEMPERATURA_LIVRE-273.0))
                            + 800.0*(TEMPERATURA_LIVRE-273.0)
                            /(fabs(i*(477.0/8.0) - 302.25)
                            *fabs(i*(477.0/8.0) - 302.25)) );                    
                                     
                glVertex3d( (0.87/zoom) - deslocamentoX,
                            ( 0.20 + (i*0.04) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (0.92/zoom)  - deslocamentoX,
                            ( 0.20 + (i*0.04) )/zoom - deslocamentoY,
                            0.0 );

                glColor3f(              
                    0.0f,
                        800.0*(TEMPERATURA_LIVRE-273.0)
                        /(fabs((i-1)*(477.0/8.0) - 302.25)
                        *fabs((i-1)*(477.0/8.0) - 302.25)),
                            fabs((i-1)*(477.0/8.0) - 119.25)
                            /(10.0*(TEMPERATURA_LIVRE-273.0))
                            + 800.0*(TEMPERATURA_LIVRE-273.0)
                            /(fabs((i-1)*(477.0/8.0) - 302.25)
                            *fabs((i-1)*(477.0/8.0) - 302.25)) );

                glVertex3d( (0.92/zoom) - deslocamentoX,
                            ( 0.20 + ((i-1)*0.04) )/zoom - deslocamentoY,
                            0.0 );
                                                  
                glVertex3d( (0.87/zoom) - deslocamentoX,
                            ( 0.20 + ((i-1)*0.04) )/zoom - deslocamentoY,
                            0.0 );
            glEnd();
            }
            if( i >= 3 )       // PARTE QUENTE
            {                         
            glBegin(GL_QUADS);
                glColor3f(              
                    4.0*(i*(477.0/8.0) - 119.25)
                    /(70.0*(TEMPERATURA_LIVRE-273.0)),
                        0.004*(i*(477.0/8.0) - 119.25)*(i*(477.0/8.0) - 119.25)
                        /(70.0*(TEMPERATURA_LIVRE-273.0))
                        + 800.0*(TEMPERATURA_LIVRE-273.0)/((i*(477.0/8.0) + 63.25)
                        *(i*(477.0/8.0) + 63.25)),
                            800.0*(TEMPERATURA_LIVRE-273.0)/((i*(477.0/8.0) + 63.25)
                            *(i*(477.0/8.0) + 63.25)) );                    
                                     
                glVertex3d( (0.87/zoom) - deslocamentoX,
                            ( 0.20 + (i*0.04) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (0.92/zoom)  - deslocamentoX,
                            ( 0.20 + (i*0.04) )/zoom - deslocamentoY,
                            0.0 );

                glColor3f(              
                    4.0*((i-1)*(477.0/8.0) - 119.25)
                    /(70.0*(TEMPERATURA_LIVRE-273.0)),
                        0.004*((i-1)*(477.0/8.0) - 119.25)*((i-1)*(477.0/8.0) - 119.25)
                        /(70.0*(TEMPERATURA_LIVRE-273.0))
                        + 800.0*(TEMPERATURA_LIVRE-273.0)/(((i-1)*(477.0/8.0) + 63.25)
                        *((i-1)*(477.0/8.0) + 63.25)),
                            800.0*(TEMPERATURA_LIVRE-273.0)/((i*(477.0/8.0) + 63.25)
                            *((i-1)*(477.0/8.0) + 63.25)) );

                glVertex3d( (0.92/zoom) - deslocamentoX,
                            ( 0.20 + ((i-1)*0.04) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (0.87/zoom) - deslocamentoX,
                            ( 0.20 + ((i-1)*0.04) )/zoom - deslocamentoY,
                            0.0 );
            glEnd();
            }
            // Rotulo dos valores
            if( i%2 == 0 )
            {
                glColor3f(0.0f,0.0f,0.0f);
                // Risco indicador
                drawLine( (0.85/zoom) - deslocamentoX,
                          ( 0.20 + (i*0.04) )/zoom - deslocamentoY, 
                          0.0,
                             (0.88/zoom) - deslocamentoX,
                             ( 0.20 + (i*0.04) )/zoom - deslocamentoY, 
                             0.0 );
            
                // Valor em forma de string
                double valorT = ((double)i)*(477.0/8.0) + 153.75;
                sprintf( Texto, "%1.1f", valorT );

                drawString( Texto, GLUT_BITMAP_HELVETICA_12,
                            (0.765/zoom) - deslocamentoX,
                            ( 0.19 + (i*0.04) )/zoom - deslocamentoY, 
                            0.0 );    
            }
        }

        // Desenha borda para legenda "estourado"
        glColor3f(0.0f,0.0f,0.0f);
        drawLine( (0.87/zoom) - deslocamentoX,
                  (0.15/zoom) - deslocamentoY,
                  0.0,
                     (0.92/zoom) - deslocamentoX,
                     (0.15/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.92/zoom) - deslocamentoX,
                  (0.15/zoom) - deslocamentoY,
                  0.0,
                     (0.92/zoom) - deslocamentoX,
                     (0.10/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.92/zoom) - deslocamentoX,
                  (0.10/zoom) - deslocamentoY,
                  0.0,
                     (0.87/zoom) - deslocamentoX,
                     (0.10/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.87/zoom) - deslocamentoX,
                  (0.10/zoom) - deslocamentoY,
                  0.0,
                     (0.87/zoom) - deslocamentoX,
                     (0.15/zoom) - deslocamentoY,
                     0.0 );

        // Desenha quadrado colorido de "estourado"
        glColor3f(1.0f,0.0f,1.0f);
        glBegin(GL_QUADS);
            glVertex3d( (0.87/zoom) - deslocamentoX,
                        (0.15/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.92/zoom)  - deslocamentoX,
                        (0.15/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.92/zoom) - deslocamentoX,
                        (0.10/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.87/zoom) - deslocamentoX,
                        (0.10/zoom) - deslocamentoY,
                         0.0 );
        glEnd();

        // Desenha string de "estourado"
        glColor3f(0.0f,0.0f,0.0f);
        drawString( "Estourado",
                    GLUT_BITMAP_HELVETICA_12,
                    (0.722/zoom) - deslocamentoX,
                    (0.118/zoom) - deslocamentoY,
                     0.0 );
      break;

      case 4:  // ------------------------- PRESSAO -------------------------
        glColor3f(0.0f, 0.0f, 0.0f);
        // Desenhar titulo da legenda          
        drawString( "Pressao (Pa)",
                    GLUT_BITMAP_HELVETICA_12,
                    (0.75/zoom) - deslocamentoX,
                    (0.65/zoom) - deslocamentoY,
                     0.0 );

        // Criar o contorno da barra
        drawLine( (0.87/zoom) - deslocamentoX,
                  (0.60/zoom) - deslocamentoY,
                  0.0,
                     (0.92/zoom) - deslocamentoX,
                     (0.60/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.92/zoom) - deslocamentoX,
                  (0.60/zoom) - deslocamentoY,
                  0.0,
                     (0.92/zoom) - deslocamentoX,
                     (0.20/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.92/zoom) - deslocamentoX,
                  (0.20/zoom) - deslocamentoY,
                  0.0,
                     (0.87/zoom) - deslocamentoX,
                     (0.20/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.87/zoom) - deslocamentoX,
                  (0.20/zoom) - deslocamentoY,
                  0.0,
                     (0.87/zoom) - deslocamentoX,
                     (0.60/zoom) - deslocamentoY,
                     0.0 );       

        // Criar uma barra gradiente da pressao
        for( i = 0; i <= 12; i++ )
        {
            if( i > 0 )
            {
            glBegin(GL_QUADS);
                glColor3f(              
                    0.0f,
                        (i)*(PRESSAO_LIVRE/4.0)*(i)*(PRESSAO_LIVRE/4.0)
                        /(8.0*PRESSAO_LIVRE*PRESSAO_LIVRE)
                        + (PRESSAO_LIVRE*PRESSAO_LIVRE)
                        /(((i)*(PRESSAO_LIVRE/4.0)+150000)
                         *((i)*(PRESSAO_LIVRE/4.0)+150000)),
                            (PRESSAO_LIVRE*PRESSAO_LIVRE)
                            /(((i)*(PRESSAO_LIVRE/4.0)+150000)
                             *((i)*(PRESSAO_LIVRE/4.0)+150000)) );            
                                     
                glVertex3d( (0.87/zoom) - deslocamentoX,
                            ( 0.20 + (i/30.0) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (0.92/zoom)  - deslocamentoX,
                            ( 0.20 + (i/30.0) )/zoom - deslocamentoY,
                            0.0 );

                glColor3f(              
                    0.0f,
                        (i-1)*(PRESSAO_LIVRE/4.0)*(i-1)*(PRESSAO_LIVRE/4.0)
                        /(8.0*PRESSAO_LIVRE*PRESSAO_LIVRE)
                        + (PRESSAO_LIVRE*PRESSAO_LIVRE)
                        /(((i-1)*(PRESSAO_LIVRE/4.0)+150000)
                         *((i-1)*(PRESSAO_LIVRE/4.0)+150000)),
                            (PRESSAO_LIVRE*PRESSAO_LIVRE)
                            /(((i-1)*(PRESSAO_LIVRE/4.0)+150000)
                             *((i-1)*(PRESSAO_LIVRE/4.0)+150000)) );

                glVertex3d( (0.92/zoom) - deslocamentoX,
                            ( 0.20 + ((i-1)/30.0) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (0.87/zoom) - deslocamentoX,
                            ( 0.20 + ((i-1)/30.0) )/zoom - deslocamentoY,
                            0.0 );
            glEnd();
            }
            // Rotulo dos valores
            if( i == 0 
            ||  i == 4
            ||  i == 8 
            ||  i == 12 )
            {
                glColor3f(0.0f,0.0f,0.0f);
                // Risco indicador
                drawLine( (0.85/zoom) - deslocamentoX,
                          ( 0.20 + (i/30.0) )/zoom - deslocamentoY, 
                          0.0,
                             (0.88/zoom) - deslocamentoX,
                             ( 0.20 + (i/30.0) )/zoom - deslocamentoY, 
                             0.0 );
            
                // Valor em forma de string
                double valorP = ((double)i)*(PRESSAO_LIVRE/4.0);
                sprintf( Texto, "%1.0f", valorP );

                if( valorP != 0 )
                    drawString( Texto, GLUT_BITMAP_HELVETICA_12,
                            (0.735/zoom) - deslocamentoX,
                            ( 0.19 + (i/30.0) )/zoom - deslocamentoY, 
                            0.0 );
                else
                    drawString( Texto, GLUT_BITMAP_HELVETICA_12,
                            (0.81/zoom) - deslocamentoX,
                            ( 0.19 + (i/30.0) )/zoom - deslocamentoY, 
                            0.0 );     
            }
        }

        // Desenha borda para legenda "estourado"
        glColor3f(0.0f,0.0f,0.0f);
        drawLine( (0.87/zoom) - deslocamentoX,
                  (0.15/zoom) - deslocamentoY,
                  0.0,
                     (0.92/zoom) - deslocamentoX,
                     (0.15/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.92/zoom) - deslocamentoX,
                  (0.15/zoom) - deslocamentoY,
                  0.0,
                     (0.92/zoom) - deslocamentoX,
                     (0.10/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.92/zoom) - deslocamentoX,
                  (0.10/zoom) - deslocamentoY,
                  0.0,
                     (0.87/zoom) - deslocamentoX,
                     (0.10/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (0.87/zoom) - deslocamentoX,
                  (0.10/zoom) - deslocamentoY,
                  0.0,
                     (0.87/zoom) - deslocamentoX,
                     (0.15/zoom) - deslocamentoY,
                     0.0 );

        // Desenha quadrado colorido de "estourado"
        glColor3f(1.0f,0.0f,1.0f);
        glBegin(GL_QUADS);
            glVertex3d( (0.87/zoom) - deslocamentoX,
                        (0.15/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.92/zoom)  - deslocamentoX,
                        (0.15/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.92/zoom) - deslocamentoX,
                        (0.10/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.87/zoom) - deslocamentoX,
                        (0.10/zoom) - deslocamentoY,
                         0.0 );
        glEnd();

        // Desenha string de "estourado"
        glColor3f(0.0f,0.0f,0.0f);
        drawString( "Estourado",
                    GLUT_BITMAP_HELVETICA_12,
                    (0.722/zoom) - deslocamentoX,
                    (0.118/zoom) - deslocamentoY,
                     0.0 );
      break;

      default:
      break;
    }
    
    //Desenha o retangulo ao fundo, exceto para a velocidade
    if( QualGrafico != 3 )
    {
        glColor3f(0.7f, 0.7f, 0.7f);

        glBegin(GL_QUADS);
            glVertex3d( (0.685/zoom) - deslocamentoX,
                        (0.74/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.99/zoom)  - deslocamentoX,
                        (0.74/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.99/zoom) - deslocamentoX,
                        (0.07/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.685/zoom) - deslocamentoX,
                        (0.07/zoom) - deslocamentoY,
                         0.0 );
        glEnd();
    }

}/**/

/* -----------------------------------------------
    Desenha a placa plana na borda sul (enfeite)
-------------------------------------------------- */
void desenharPlacasPlanas( Grid *malha )
{
    // Desenhar placas planas na forma de poligono
    glBegin(GL_POLYGON);
        glColor3f(0.5f,0.5f,0.5f);       
        glVertex3d(-malha->sideLength/2.0 + malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   -malha->sideLength/2.0 + malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   0);

        glColor3f(0.48f,0.48f,0.48f);
        glVertex3d(-(malha->sideLength/2.0) + malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   -1.1*(malha->sideLength/2) + malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   0);

        glColor3f(0.55f,0.55f,0.55f);
        glVertex3d((malha->sideLength/2) - malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   -1.1*(malha->sideLength/2) + malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   0);

        glColor3f(0.38f,0.38f,0.38f);
        glVertex3d((malha->sideLength/2) - malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   -(malha->sideLength/2) + malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   0);
    glEnd();

    /*glBegin(GL_POLYGON);
        glColor3f(0.5f,0.5f,0.5f);       
        glVertex3d(-malha->sideLength/2.0 + malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   malha->sideLength/2.0 - malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   0);

        glColor3f(0.48f,0.48f,0.48f);
        glVertex3d(-(malha->sideLength/2.0) + malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   1.1*(malha->sideLength/2) - malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   0);

        glColor3f(0.55f,0.55f,0.55f);
        glVertex3d((malha->sideLength/2) - malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   1.1*(malha->sideLength/2) - malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   0);

        glColor3f(0.38f,0.38f,0.38f);
        glVertex3d((malha->sideLength/2) - malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   (malha->sideLength/2) - malha->sideLength/AO_QUADRADO((double)malha->refinamentoMaximo),
                   0);
    glEnd();*/
}


/* ----------------------------------------------
    Desenha o grafico de residuos
------------------------------------------------ */
void desenharResiduo( Grid *malha, int iteracaoAtual )
{
    char valor[5];

    // Desenha eixo x
    glColor3f(0.0f,0.0f,0.0f);
    drawLine(-malha->sideLength / 2.0, -malha->sideLength / 2.0, 0,
              malha->sideLength / 2.0, -malha->sideLength / 2.0, 0); // linha
    // Ponta
    drawTriangle(malha->sideLength / 2.0,-malha->sideLength / 2.0 + malha->sideLength / 60.0, 0,
                 malha->sideLength / 2.0,-malha->sideLength / 2.0 - malha->sideLength / 60.0, 0,
                 malha->sideLength / 2.0 + malha->sideLength / 30.0, -malha->sideLength / 2.0, 0 ); 
    // Rotulo
    drawString("iteracoes", GLUT_BITMAP_HELVETICA_12,
             malha->sideLength / 2.0 + malha->sideLength / 20.0, 
            -malha->sideLength / 2.0 - malha->sideLength / 30.0, 
             0);
    // Marcador
    sprintf(valor, "%d", iteracaoAtual );
    drawString(valor, GLUT_BITMAP_HELVETICA_12,
               malha->sideLength / 2.0 - malha->sideLength / 30.0, 
               -malha->sideLength / 2.0 - malha->sideLength / 40.0, 
               0);
    // Marcador
    sprintf(valor, "%d", iteracaoAtual - MAXITER_GRAFICO_RESIDUOS );
    drawString(valor, GLUT_BITMAP_HELVETICA_12,
               -malha->sideLength / 2.0 - malha->sideLength / 20.0, 
               -malha->sideLength / 2.0 - malha->sideLength / 40.0, 
               0);

    // Desenha eixo y
    drawLine(-malha->sideLength / 2.0, -malha->sideLength / 2.0, 0,
             -malha->sideLength / 2.0, +malha->sideLength / 2.0, 0); // linha
    // Ponta
    drawTriangle(-malha->sideLength / 2.0,malha->sideLength / 2.0 + malha->sideLength / 30.0, 0,
                 -malha->sideLength / 2.0 - malha->sideLength / 60.0, malha->sideLength / 2.0, 0,
                 -malha->sideLength / 2.0 + malha->sideLength / 60.0, malha->sideLength / 2.0, 0 );
    // Rotulo
    drawString("residuo (x 10^9)", GLUT_BITMAP_HELVETICA_12,
               -malha->sideLength / 2.0 + malha->sideLength / 40.0, malha->sideLength / 2.0, 0);
    // Marcador
    sprintf(valor, "%1.1f", malha->maiorResiduo * 1000000000 );
    drawString(valor, GLUT_BITMAP_HELVETICA_12,
               -malha->sideLength / 2.0 - malha->sideLength / 20.0, 
               malha->sideLength / 2.0 - malha->sideLength / 40.0, 
               0);
    // Marcador
    sprintf(valor, "%1.1f", malha->maiorResiduo * 500000000 );
    drawString(valor, GLUT_BITMAP_HELVETICA_12,
               -malha->sideLength / 2.0 - malha->sideLength / 20.0, 0, 0);

    // Plota os valores dos residuos para cada iteracao 
    int i, j;
    for(i = 1; i < MAXITER_GRAFICO_RESIDUOS; i++)
    {
        // i: indice no eixo x (de 1 a MAXITER_GRAFICO_RESIDUOS)
        // j: indice no vetor
        j = (i + iteracaoAtual) % MAXITER_GRAFICO_RESIDUOS;

        // Pressao
        glColor3f(0.0f,0.6f,0.0f); // verde
        drawLine( -malha->sideLength / 2.0 + ( ((double)(i-1)) / MAXITER_GRAFICO_RESIDUOS) 
                                                * malha->sideLength,
                  -malha->sideLength / 2.0 + (malha->residuosP[j] / malha->maiorResiduo) 
                                                * malha->sideLength, 
                  0,
                  -malha->sideLength / 2.0 + ( ((double)i) / MAXITER_GRAFICO_RESIDUOS) 
                                                       * malha->sideLength,
                  -malha->sideLength / 2.0 + (malha->residuosP[(j+1)%MAXITER_GRAFICO_RESIDUOS] 
                                                / malha->maiorResiduo) * malha->sideLength, 
                  0 );

        // Temperatura
        glColor3f(1.0f,0.3f,0.0f); // vermelho
        drawLine( -malha->sideLength / 2.0 + ( ((double)(i-1)) / MAXITER_GRAFICO_RESIDUOS) 
                                                    * malha->sideLength,
                  -malha->sideLength / 2.0 + (malha->residuosT[j] / malha->maiorResiduo) 
                                                    * malha->sideLength, 
                  0,
                  -malha->sideLength / 2.0 + ( ((double)i) / MAXITER_GRAFICO_RESIDUOS) 
                                                    * malha->sideLength,
                  -malha->sideLength / 2.0 + (malha->residuosT[(j+1)%MAXITER_GRAFICO_RESIDUOS] 
                                    / malha->maiorResiduo) * malha->sideLength, 
                  0 );

        // Velocidade
        glColor3f(0.0f,0.0f,0.3f); // velocidade
        drawLine( -malha->sideLength / 2.0 + ( ((double)(i-1)) / MAXITER_GRAFICO_RESIDUOS) 
                                                * malha->sideLength,
                  -malha->sideLength / 2.0 + (malha->residuosVel[j] / malha->maiorResiduo) 
                                                    * malha->sideLength, 
                  0,
                  -malha->sideLength / 2.0 + ( ((double)i) / MAXITER_GRAFICO_RESIDUOS) 
                                                    * malha->sideLength,
                  -malha->sideLength / 2.0 + (malha->residuosVel[(j+1)%MAXITER_GRAFICO_RESIDUOS] 
                                    / malha->maiorResiduo) * malha->sideLength, 
                  0 );
    }
}

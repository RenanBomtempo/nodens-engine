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
            if( celula->rhoAtual >= LEGENDA_LIM_INF_DENSIDADE 
            &&  celula->rhoAtual <  LEGENDA_LIM_SUP_DENSIDADE )
            {
                glColor3f(
                    0.0f ,
                        0.008f + 
                        0.88*(celula->rhoAtual)
                             / 
                        LEGENDA_LIM_SUP_DENSIDADE ,
                            (celula->rhoAtual)*(celula->rhoAtual)
                                    / 
                            LEGENDA_LIM_SUP_DENSIDADE );
            }
            // Estourado abaixo
            else if(celula->rhoAtual < LEGENDA_LIM_INF_DENSIDADE)
            {
                glColor3f COR_LIM_INF_DENSIDADE;
            }
            // Estourado acima
            else if(celula->rhoAtual > LEGENDA_LIM_SUP_DENSIDADE )
            {
                glColor3f COR_LIM_SUP_DENSIDADE;
            }
            
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
            // Estourado abaixo
            else if(celula->TAtual < LEGENDA_LIM_INF_TEMPERATURA)
            {
                glColor3f COR_LIM_INF_TEMPERATURA;
            }
            // Estourado acima
            else if(celula->TAtual > LEGENDA_LIM_SUP_TEMPERATURA )
            {
                glColor3f COR_LIM_SUP_TEMPERATURA;
            }
    
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
            // Estourado abaixo
            else if(celula->pAtual < LEGENDA_LIM_INF_PRESSAO)
            {
                glColor3f COR_LIM_INF_PRESSAO;
            }
            // Estourado acima
            else if(celula->pAtual > LEGENDA_LIM_SUP_PRESSAO )
            {
                glColor3f COR_LIM_SUP_PRESSAO;
            }
    
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

    const float XESQBARRA = 0.87;
    const float XDIRBARRA = 0.92;
    const float YSUPBARRA = 0.56;
    const float YINFBARRA = 0.18;

    const float ALTURAESTOURADO = 0.042;

    const int PASSOSDENS = 8;
    const int PASSOSTEMP = 10;
    const int PASSOSPRES = 12;
    const float DELTAYGRAD_DENS = (YSUPBARRA - YINFBARRA) 
                                    / 
                                 ( (float) PASSOSDENS );
    const float DELTAYGRAD_TEMP = (YSUPBARRA - YINFBARRA) 
                                    / 
                                 ( (float) PASSOSTEMP );
    const float DELTAYGRAD_PRES = (YSUPBARRA - YINFBARRA) 
                                    / 
                                 ( (float) PASSOSPRES );

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
    
    // Desenha elementos comuns, exceto para a velocidade
    if( QualGrafico != 3 && QualGrafico != 5 ) // 3 = velocidade
    {
        // Criar o contorno da barra
        drawLine( (XESQBARRA/zoom) - deslocamentoX,
                  (YSUPBARRA/zoom) - deslocamentoY,
                  0.0,
                     (XDIRBARRA/zoom) - deslocamentoX,
                     (YSUPBARRA/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (XDIRBARRA/zoom) - deslocamentoX,
                  (YSUPBARRA/zoom) - deslocamentoY,
                  0.0,
                     (XDIRBARRA/zoom) - deslocamentoX,
                     (YINFBARRA/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (XDIRBARRA/zoom) - deslocamentoX,
                  (YINFBARRA/zoom) - deslocamentoY,
                  0.0,
                     (XESQBARRA/zoom) - deslocamentoX,
                     (YINFBARRA/zoom) - deslocamentoY,
                     0.0 );
        drawLine( (XESQBARRA/zoom) - deslocamentoX,
                  (YINFBARRA/zoom) - deslocamentoY,
                  0.0,
                     (XESQBARRA/zoom) - deslocamentoX,
                     (YSUPBARRA/zoom) - deslocamentoY,
                     0.0 );
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

        // Criar uma barra gradiente da densidade
        for( i = 0; i <= PASSOSDENS; i++ )
        {
            if( i > 0 )
            {
            glBegin(GL_QUADS);
                glColor3f(
                    0.0f ,
                        0.008f +
                        0.88*( ((double)i)*(1.22516/4.0) )
                            / 
                        LEGENDA_LIM_SUP_DENSIDADE ,
                            ( ((double)i)*(1.22516/4.0) )
                            *( ((double)i)*(1.22516/4.0) )
                                /
                            LEGENDA_LIM_SUP_DENSIDADE );            
                                     
                glVertex3d( (XESQBARRA/zoom) - deslocamentoX,
                            ( YINFBARRA + (i*DELTAYGRAD_DENS) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (XDIRBARRA/zoom)  - deslocamentoX,
                            ( YINFBARRA + (i*DELTAYGRAD_DENS) )/zoom - deslocamentoY,
                            0.0 );

                glColor3f(
                    0.0f ,
                        0.008f +
                        0.88*( ((double)(i-1))*(1.22516/4.0) )
                            /
                        LEGENDA_LIM_SUP_DENSIDADE ,
                             ( ((double)(i-1))*(1.22516/4.0) )
                            *( ((double)(i-1))*(1.22516/4.0) )
                                /
                            LEGENDA_LIM_SUP_DENSIDADE );

                glVertex3d( (XDIRBARRA/zoom) - deslocamentoX,
                            ( YINFBARRA + ((i-1)*DELTAYGRAD_DENS) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (XESQBARRA/zoom) - deslocamentoX,
                            ( YINFBARRA + ((i-1)*DELTAYGRAD_DENS) )/zoom - deslocamentoY,
                            0.0 );
            glEnd();
            }
            // Rotulo dos valores
            if( i%2 == 0 )
            {
                glColor3f(0.0f,0.0f,0.0f);
                // Risco indicador
                drawLine( ((XESQBARRA-0.02)/zoom) - deslocamentoX,
                          ( YINFBARRA + (i*DELTAYGRAD_DENS) )/zoom - deslocamentoY, 
                          0.0,
                             ((XESQBARRA+0.01)/zoom) - deslocamentoX,
                             ( YINFBARRA + (i*DELTAYGRAD_DENS) )/zoom - deslocamentoY, 
                             0.0 );
            
                // Valor em forma de string
                double valorRho = ((double)i)*(1.22518/4.0);
                sprintf( Texto, "%1.3f", valorRho );

                drawString( Texto, GLUT_BITMAP_HELVETICA_12,
                            ((XESQBARRA-0.105)/zoom) - deslocamentoX,
                            ( (YINFBARRA-0.01) + (i*DELTAYGRAD_DENS) )/zoom - deslocamentoY, 
                            0.0 );    
            }
        }

      break;

      case 2: // ---------------------- TEMPERATURA -----------------------
        glColor3f(0.0f, 0.0f, 0.0f);
        // Desenhar titulo da legenda          
        drawString( "Temperatura (K)",
                    GLUT_BITMAP_HELVETICA_12,
                    (0.74/zoom) - deslocamentoX,
                    (0.65/zoom) - deslocamentoY,
                     0.0 );

        // Criar uma barra gradiente da temperatura
        for( i = 0; i <= PASSOSTEMP; i++ )
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
                                     
                glVertex3d( (XESQBARRA/zoom) - deslocamentoX,
                            ( YINFBARRA + (i*DELTAYGRAD_TEMP) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (XDIRBARRA/zoom)  - deslocamentoX,
                            ( YINFBARRA + (i*DELTAYGRAD_TEMP) )/zoom - deslocamentoY,
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

                glVertex3d( (XDIRBARRA/zoom) - deslocamentoX,
                            ( YINFBARRA + ((i-1)*DELTAYGRAD_TEMP) )/zoom - deslocamentoY,
                            0.0 );
                                                  
                glVertex3d( (XESQBARRA/zoom) - deslocamentoX,
                            ( YINFBARRA + ((i-1)*DELTAYGRAD_TEMP) )/zoom - deslocamentoY,
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
                                     
                glVertex3d( (XESQBARRA/zoom) - deslocamentoX,
                            ( YINFBARRA + (i*DELTAYGRAD_TEMP) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (XDIRBARRA/zoom)  - deslocamentoX,
                            ( YINFBARRA + (i*DELTAYGRAD_TEMP) )/zoom - deslocamentoY,
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

                glVertex3d( (XDIRBARRA/zoom) - deslocamentoX,
                            ( YINFBARRA + ((i-1)*DELTAYGRAD_TEMP) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (XESQBARRA/zoom) - deslocamentoX,
                            ( YINFBARRA + ((i-1)*DELTAYGRAD_TEMP) )/zoom - deslocamentoY,
                            0.0 );
            glEnd();
            }
            // Rotulo dos valores
            if( i%2 == 0 )
            {
                glColor3f(0.0f,0.0f,0.0f);
                // Risco indicador
                drawLine( ((XESQBARRA-0.02)/zoom) - deslocamentoX,
                          ( YINFBARRA + (i*DELTAYGRAD_TEMP) )/zoom - deslocamentoY, 
                          0.0,
                             ((XESQBARRA+0.01)/zoom) - deslocamentoX,
                             ( YINFBARRA + (i*DELTAYGRAD_TEMP) )/zoom - deslocamentoY, 
                             0.0 );
            
                // Valor em forma de string
                double valorT = ((double)i)*(477.0/8.0) + LEGENDA_LIM_INF_TEMPERATURA;
                sprintf( Texto, "%1.1f", valorT );

                drawString( Texto, GLUT_BITMAP_HELVETICA_12,
                            ((XESQBARRA-0.105)/zoom) - deslocamentoX,
                            ( (YINFBARRA-0.01) + (i*DELTAYGRAD_TEMP) )/zoom - deslocamentoY, 
                            0.0 );    
            }
        }
      break;

      case 4:  // ------------------------- PRESSAO -------------------------
        glColor3f(0.0f, 0.0f, 0.0f);
        // Desenhar titulo da legenda          
        drawString( "Pressao (Pa)",
                    GLUT_BITMAP_HELVETICA_12,
                    (0.75/zoom) - deslocamentoX,
                    (0.65/zoom) - deslocamentoY,
                     0.0 );
  
        // Criar uma barra gradiente da pressao
        for( i = 0; i <= PASSOSPRES; i++ )
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
                                     
                glVertex3d( (XESQBARRA/zoom) - deslocamentoX,
                            ( YINFBARRA + (i*DELTAYGRAD_PRES) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (XDIRBARRA/zoom)  - deslocamentoX,
                            ( YINFBARRA + (i*DELTAYGRAD_PRES) )/zoom - deslocamentoY,
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

                glVertex3d( (XDIRBARRA/zoom) - deslocamentoX,
                            ( YINFBARRA + ((i-1)*DELTAYGRAD_PRES) )/zoom - deslocamentoY,
                            0.0 );

                glVertex3d( (XESQBARRA/zoom) - deslocamentoX,
                            ( YINFBARRA + ((i-1)*DELTAYGRAD_PRES) )/zoom - deslocamentoY,
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
                drawLine( ((XESQBARRA-0.02)/zoom) - deslocamentoX,
                          ( YINFBARRA + (i*DELTAYGRAD_PRES) )/zoom - deslocamentoY, 
                          0.0,
                             ((XESQBARRA+0.01)/zoom) - deslocamentoX,
                             ( YINFBARRA + (i*DELTAYGRAD_PRES) )/zoom - deslocamentoY, 
                             0.0 );
            
                // Valor em forma de string
                double valorP = ((double)i)*(PRESSAO_LIVRE/4.0);
                sprintf( Texto, "%1.0f", valorP );

                if( valorP != 0 )
                    drawString( Texto, GLUT_BITMAP_HELVETICA_12,
                            ((XESQBARRA-0.129)/zoom) - deslocamentoX,
                            ( (YINFBARRA-0.01) + (i*DELTAYGRAD_PRES) )/zoom - deslocamentoY, 
                            0.0 );
                else
                    drawString( Texto, GLUT_BITMAP_HELVETICA_12,
                            ((XESQBARRA-0.06)/zoom) - deslocamentoX,
                            ( (YINFBARRA-0.01) + (i*DELTAYGRAD_PRES) )/zoom - deslocamentoY, 
                            0.0 );     
            }
        }
      break;

      case 5:  // ------------------------- Residuos -------------------------
          // Linhas grossas para o grafico
          glLineWidth(2.5);          
          
          //desenha Pressão
          glColor3f(0.0, 0.85, 0.0); // verde
          drawLine( (0.72/zoom) - deslocamentoX, (0.65/zoom) - deslocamentoY, 0.0,
                    (0.76/zoom) - deslocamentoX, (0.65/zoom) - deslocamentoY, 0.0);
          glColor3f(0.0, 0.0, 0.0); // preto
          drawString( "Pressao",
                      GLUT_BITMAP_HELVETICA_12,
                      (0.77/zoom) - deslocamentoX,
                      (0.64/zoom) - deslocamentoY,
                       0.0 );
                     
          //desenha Energia
          glColor3f(1.0f,0.1f,0.0f); // vermelho
          drawLine( (0.72/zoom) - deslocamentoX, (0.61/zoom) - deslocamentoY, 0.0,
                    (0.76/zoom) - deslocamentoX, (0.61/zoom) - deslocamentoY, 0.0);
          glColor3f(0.0, 0.0, 0.0); // preto
          drawString( "Energia",
                      GLUT_BITMAP_HELVETICA_12,
                      (0.77/zoom) - deslocamentoX,
                      (0.60/zoom) - deslocamentoY,
                       0.0 );
          
          //desenha Velocidade u
          glColor3f(0.0f,0.0f,1.0f); // azul puro
          drawLine( (0.72/zoom) - deslocamentoX, (0.57/zoom) - deslocamentoY, 0.0,
                    (0.76/zoom) - deslocamentoX, (0.57/zoom) - deslocamentoY, 0.0);
          glColor3f(0.0, 0.0, 0.0); // preto
          drawString( "Velocidade u",
                      GLUT_BITMAP_HELVETICA_12,
                      (0.77/zoom) - deslocamentoX,
                      (0.56/zoom) - deslocamentoY,
                       0.0 );
                       
          //desenha Velocidade v
          glColor3f(0.0, 0.5, 0.5); // azul esverdeado
          drawLine( (0.72/zoom) - deslocamentoX, (0.53/zoom) - deslocamentoY, 0.0,
                    (0.76/zoom) - deslocamentoX, (0.53/zoom) - deslocamentoY, 0.0);
          glColor3f(0.0, 0.0, 0.0); // preto
          drawString( "Velocidade v",
                      GLUT_BITMAP_HELVETICA_12,
                      (0.77/zoom) - deslocamentoX,
                      (0.52/zoom) - deslocamentoY,
                       0.0 );   
                       
          //desenha Massa
          glColor3f(0.7f,0.7f,0.0f); // amarelo escuro
          drawLine( (0.72/zoom) - deslocamentoX, (0.49/zoom) - deslocamentoY, 0.0,
                    (0.76/zoom) - deslocamentoX, (0.49/zoom) - deslocamentoY, 0.0);
          glColor3f(0.0, 0.0, 0.0); // preto
          drawString( "Massa",
                      GLUT_BITMAP_HELVETICA_12,
                      (0.77/zoom) - deslocamentoX,
                      (0.48/zoom) - deslocamentoY,
                       0.0 );

          // Desfaz linhas grossas
          glLineWidth(1.0);

         //desenha o retangulo branco da legenda do grafico de resíduos
        glColor3f(1.0f, 1.0f, 1.0f); //cor branca

        glBegin(GL_QUADS);
            glVertex3d( (0.715/zoom) - deslocamentoX,
                        (0.67/zoom) - deslocamentoY,
                         0.0);

            glVertex3d( (0.765/zoom) - deslocamentoX,
                        (0.67/zoom) - deslocamentoY,
                         0.0);

            glVertex3d( (0.765/zoom) - deslocamentoX,
                        (0.47/zoom) - deslocamentoY,
                         0.0);

            glVertex3d( (0.715/zoom) - deslocamentoX,
                        (0.47/zoom) - deslocamentoY,
                         0.0);
        glEnd();

      break;

      default:
      break;
    }
    
    // Desenha indicador de valores limites da legenda
    if( QualGrafico != 5 && QualGrafico != 3 ) // 5 = residuos, 3 = velocidade
    {
        // Desenha borda para legenda de valores estourados
        glColor3f(0.0f,0.0f,0.0f);
        // Quadrado de cima
        drawLine( (XESQBARRA/zoom) - deslocamentoX,
                    ((YSUPBARRA+ALTURAESTOURADO)/zoom) - deslocamentoY,
                    0.0,
                        (XDIRBARRA/zoom) - deslocamentoX,
                        ((YSUPBARRA+ALTURAESTOURADO)/zoom) - deslocamentoY,
                        0.0 );
        drawLine( (XDIRBARRA/zoom) - deslocamentoX,
                    ((YSUPBARRA+ALTURAESTOURADO)/zoom) - deslocamentoY,
                    0.0,
                        (XDIRBARRA/zoom) - deslocamentoX,
                        ((YSUPBARRA+ ALTURAESTOURADO/8.0)/zoom) - deslocamentoY,
                        0.0 );
        drawLine( (XDIRBARRA/zoom) - deslocamentoX,
                    ((YSUPBARRA+ ALTURAESTOURADO/8.0)/zoom) - deslocamentoY,
                    0.0,
                        (XESQBARRA/zoom) - deslocamentoX,
                        ((YSUPBARRA+ ALTURAESTOURADO/8.0)/zoom) - deslocamentoY,
                        0.0 );
        drawLine( (XESQBARRA/zoom) - deslocamentoX,
                    ((YSUPBARRA+ ALTURAESTOURADO/8.0)/zoom) - deslocamentoY,
                    0.0,
                        (XESQBARRA/zoom) - deslocamentoX,
                        ((YSUPBARRA+ALTURAESTOURADO)/zoom) - deslocamentoY,
                        0.0 );
    
        // Quadrado de baixo
        drawLine( (XESQBARRA/zoom) - deslocamentoX,
                    ((YINFBARRA-ALTURAESTOURADO)/zoom) - deslocamentoY,
                    0.0,
                        (XDIRBARRA/zoom) - deslocamentoX,
                        ((YINFBARRA-ALTURAESTOURADO)/zoom) - deslocamentoY,
                        0.0 );
        drawLine( (XDIRBARRA/zoom) - deslocamentoX,
                    ((YINFBARRA-ALTURAESTOURADO)/zoom) - deslocamentoY,
                    0.0,
                        (XDIRBARRA/zoom) - deslocamentoX,
                        ((YINFBARRA- ALTURAESTOURADO/8.0)/zoom) - deslocamentoY,
                        0.0 );
        drawLine( (XDIRBARRA/zoom) - deslocamentoX,
                    ((YINFBARRA- ALTURAESTOURADO/8.0)/zoom) - deslocamentoY,
                    0.0,
                        (XESQBARRA/zoom) - deslocamentoX,
                        ((YINFBARRA- ALTURAESTOURADO/8.0)/zoom) - deslocamentoY,
                        0.0 );
        drawLine( (XESQBARRA/zoom) - deslocamentoX,
                    ((YINFBARRA- ALTURAESTOURADO/8.0)/zoom) - deslocamentoY,
                    0.0,
                        (XESQBARRA/zoom) - deslocamentoX,
                        ((YINFBARRA-ALTURAESTOURADO)/zoom) - deslocamentoY,
                        0.0 );
    
        // Desenha quadrados coloridos de "estourado acima"
        // Quadrado de cima
        if(QualGrafico == 1)
            glColor3f COR_LIM_SUP_DENSIDADE;
        else if(QualGrafico == 2)
            glColor3f COR_LIM_SUP_TEMPERATURA;
        else if(QualGrafico == 4)
            glColor3f COR_LIM_SUP_PRESSAO;
        glBegin(GL_QUADS);
            glVertex3d( (XESQBARRA/zoom) - deslocamentoX,
                        ((YSUPBARRA+ALTURAESTOURADO)/zoom) - deslocamentoY,
                            0.0 );
    
            glVertex3d( (XDIRBARRA/zoom)  - deslocamentoX,
                        ((YSUPBARRA+ALTURAESTOURADO)/zoom) - deslocamentoY,
                            0.0 );
    
            glVertex3d( (XDIRBARRA/zoom) - deslocamentoX,
                        ((YSUPBARRA+ ALTURAESTOURADO/8.0)/zoom) - deslocamentoY,
                            0.0 );
    
            glVertex3d( (XESQBARRA/zoom) - deslocamentoX,
                        ((YSUPBARRA+ ALTURAESTOURADO/8.0)/zoom) - deslocamentoY,
                            0.0 );
        glEnd();
    
        // Quadrado de baixo
        if(QualGrafico == 1)
            glColor3f COR_LIM_INF_DENSIDADE;
        else if(QualGrafico == 2)
            glColor3f COR_LIM_INF_TEMPERATURA;
        else if(QualGrafico == 4)
            glColor3f COR_LIM_INF_PRESSAO;
        glBegin(GL_QUADS);
            glVertex3d( (XESQBARRA/zoom) - deslocamentoX,
                        ((YINFBARRA-ALTURAESTOURADO)/zoom) - deslocamentoY,
                            0.0 );
    
            glVertex3d( (XDIRBARRA/zoom)  - deslocamentoX,
                        ((YINFBARRA-ALTURAESTOURADO)/zoom) - deslocamentoY,
                            0.0 );
    
            glVertex3d( (XDIRBARRA/zoom) - deslocamentoX,
                        ((YINFBARRA- ALTURAESTOURADO/8.0)/zoom) - deslocamentoY,
                            0.0 );
    
            glVertex3d( (XESQBARRA/zoom) - deslocamentoX,
                        ((YINFBARRA- ALTURAESTOURADO/8.0)/zoom) - deslocamentoY,
                            0.0 );
        glEnd();
    }

    // Desenha fundo cinza, que e' um elemento comum, exceto para a velocidade
    if( QualGrafico != 3 ) // 3 = velocidade
    {
        // Fundo cinza
        glColor3f(0.7f, 0.7f, 0.7f);

        glBegin(GL_QUADS);
            glVertex3d( (0.685/zoom) - deslocamentoX,
                        (0.74/zoom) - deslocamentoY,
                         0.0 );

            glVertex3d( (0.99/zoom)  - deslocamentoX,
                        (0.74/zoom) - deslocamentoY,
                         0.0 );

            // Coordenada y da borda inferior para todas legendas exceto o do residuos
            if (QualGrafico != 5) // 5 = residuos
            {
               glVertex3d( (0.99/zoom) - deslocamentoX,
                           (0.12/zoom) - deslocamentoY,
                            0.0 );

               glVertex3d( (0.685/zoom) - deslocamentoX,
                           (0.12/zoom) - deslocamentoY,
                            0.0 );
            }
            // Coordenada y da borda inferior para legenda do residuos
            else
            {
                glVertex3d( (0.99/zoom) - deslocamentoX,
                            (0.45/zoom) - deslocamentoY,
                             0.0 );
                             
                glVertex3d( (0.685/zoom) - deslocamentoX,
                            (0.45/zoom) - deslocamentoY,
                             0.0 );
            }
            
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

    const double L = malha->sideLength;

    // Linhas grossas para o grafico
    glLineWidth(2.5);

    // Plota os valores dos residuos para cada iteracao
    int i, j;
    for(i = 1; i < MAXITER_GRAFICO_RESIDUOS; i++)
    {
        // i: indice no eixo x (de 1 a MAXITER_GRAFICO_RESIDUOS)
        // j: indice no vetor
        j = (i + iteracaoAtual) % MAXITER_GRAFICO_RESIDUOS;

        // Pressao
        if( malha->residuosP[j] > LIMITE_INFERIOR_RESIDUO
        &&  malha->residuosP[(j+1)%MAXITER_GRAFICO_RESIDUOS] > LIMITE_INFERIOR_RESIDUO)
        {
            glColor3f(0.0, 0.85, 0.0); // verde
            drawLine( -L / 2.0 + ( ((double)(i-1)) / MAXITER_GRAFICO_RESIDUOS) * L,
                    +L / 2.0 + ( (log10(malha->residuosP[j])) 
                                                / fabs(log10(malha->menorResiduo)) ) * L,
                    0,
                    -L / 2.0 + ( ((double)i) / MAXITER_GRAFICO_RESIDUOS) * L,
                    +L / 2.0 + (log10(malha->residuosP[(j+1)%MAXITER_GRAFICO_RESIDUOS])
                                                / fabs(log10(malha->menorResiduo)) ) * L,
                    0 );
        }

        // Energia
        if( malha->residuosE[j] > LIMITE_INFERIOR_RESIDUO
        &&  malha->residuosE[(j+1)%MAXITER_GRAFICO_RESIDUOS] > LIMITE_INFERIOR_RESIDUO)
        {
            glColor3f(1.0f,0.1f,0.0f); // vermelho
            drawLine( -L / 2.0 + ( ((double)(i-1)) / MAXITER_GRAFICO_RESIDUOS) * L,
                    +L / 2.0 + ( log10(malha->residuosE[j]) / fabs(log10(malha->menorResiduo)) ) * L,
                    0,
                    -L / 2.0 + ( ((double)i) / MAXITER_GRAFICO_RESIDUOS) * L,
                    +L / 2.0 + ( log10(malha->residuosE[(j+1)%MAXITER_GRAFICO_RESIDUOS])
                                        / fabs(log10(malha->menorResiduo)) ) * L,
                    0 );
        }

        // Velocidade u
        if( malha->residuosU[j] > LIMITE_INFERIOR_RESIDUO
        &&  malha->residuosU[(j+1)%MAXITER_GRAFICO_RESIDUOS] > LIMITE_INFERIOR_RESIDUO)
        {
            glColor3f(0.0f,0.0f,1.0f); // azul puro
            drawLine( -L / 2.0 + ( ((double)(i-1)) / MAXITER_GRAFICO_RESIDUOS) * L,
                    +L / 2.0 + ( log10(malha->residuosU[j]) / fabs(log10(malha->menorResiduo)) ) * L,
                    0,
                    -L / 2.0 + ( ((double)i) / MAXITER_GRAFICO_RESIDUOS) * L,
                    +L / 2.0 + ( log10(malha->residuosU[(j+1)%MAXITER_GRAFICO_RESIDUOS])
                                        / fabs(log10(malha->menorResiduo)) ) * L,
                    0 );
        }

        // Velocidade v
        if( malha->residuosV[j] > LIMITE_INFERIOR_RESIDUO
        &&  malha->residuosV[(j+1)%MAXITER_GRAFICO_RESIDUOS] > LIMITE_INFERIOR_RESIDUO)
        {
            glColor3f(0.0, 0.5, 0.5); // azul esverdeado
            drawLine( -L / 2.0 + ( ((double)(i-1)) / MAXITER_GRAFICO_RESIDUOS) * L,
                    +L / 2.0 + ( log10(malha->residuosV[j]) / fabs(log10(malha->menorResiduo)) ) * L,
                    0,
                    -L / 2.0 + ( ((double)i) / MAXITER_GRAFICO_RESIDUOS) * L,
                    +L / 2.0 + ( log10(malha->residuosV[(j+1)%MAXITER_GRAFICO_RESIDUOS])
                                        / fabs(log10(malha->menorResiduo)) ) * L,
                    0 );
        }

        // Massa
        if( malha->residuosM[j] > LIMITE_INFERIOR_RESIDUO
        &&  malha->residuosM[(j+1)%MAXITER_GRAFICO_RESIDUOS] > LIMITE_INFERIOR_RESIDUO)
        {
            glColor3f(0.7f,0.7f,0.0f); // amarelo escuro
            drawLine( -L / 2.0 + ( ((double)(i-1)) / MAXITER_GRAFICO_RESIDUOS) * L,
                    +L / 2.0 + ( log10(malha->residuosM[j]) / fabs(log10(malha->menorResiduo)) ) * L,
                    0,
                    -L / 2.0 + ( ((double)i) / MAXITER_GRAFICO_RESIDUOS) * L,
                    +L / 2.0 + ( log10(malha->residuosM[(j+1)%MAXITER_GRAFICO_RESIDUOS])
                                        / fabs(log10(malha->menorResiduo)) ) * L,
                    0 );
        }
    }

    // Desenha eixo x ==========================================================================
    glColor3f(0.0f,0.0f,0.0f);
    glLineWidth(2.0);
    drawLine(-L / 2.0, -L / 2.0, 0,
              L / 2.0, -L / 2.0, 0); // linha
    // Ponta
    drawTriangle(L / 2.0,-L / 2.0 + L / 60.0, 0,
                 L / 2.0,-L / 2.0 - L / 60.0, 0,
                 L / 2.0 + L / 30.0, -L / 2.0, 0 ); 
    // Rotulo
    drawString("iteracoes", GLUT_BITMAP_HELVETICA_12,
             L / 2.0 + L / 20.0, 
            -L / 2.0 - L / 30.0, 
             0);
    // Marcador
    sprintf(valor, "%d", iteracaoAtual );
    drawString(valor, GLUT_BITMAP_HELVETICA_12,
                L * 0.48, 
               -L * 0.535, 
               0);

    glLineWidth(1.0);
    drawLine(L / 2.0 - L / 30.0, -L * 0.5,0,
             L / 2.0 - L / 30.0, -L * 0.52,0);

    // Marcador
    sprintf(valor, "%d", iteracaoAtual - MAXITER_GRAFICO_RESIDUOS );

    if(iteracaoAtual - MAXITER_GRAFICO_RESIDUOS > 0)
    {
          drawString(valor, GLUT_BITMAP_HELVETICA_12,
                     -L * 0.48,
                     -L * 0.535,
                     0);

          drawLine(-L * 0.49, -L * 0.5,0,
                   -L * 0.49, -L * 0.52,0);
    }

    // Desenha eixo y ============================================================================
    glLineWidth(2.0);
    drawLine(-L / 2.0, -L / 2.0, 0,
             -L / 2.0, +L / 2.0, 0); // linha
    // Ponta
    drawTriangle(-L / 2.0,L / 2.0 + L / 30.0, 0,
                 -L / 2.0 - L / 60.0, L / 2.0, 0,
                 -L / 2.0 + L / 60.0, L / 2.0, 0 );
    // Rotulo
    drawString("Residuo", GLUT_BITMAP_HELVETICA_12,
               -L / 2.0 + L / 40.0, L / 2.0, 0);
    // Marcador
    glLineWidth(1.0);
    int k;
    const int MARCADORES = 5;
    for(k = 0; k <= MARCADORES; k++){
        sprintf(valor, "1e-%d", (int) (
                                        ( ((double)k) 
                                          * fabs(log10(malha->menorResiduo))
                                        )
                                        / ((double)MARCADORES)
                                      ) );

        glColor3f(0.0,0.0,0.0);
        drawString(valor, GLUT_BITMAP_HELVETICA_12,
                -L * 0.588, 
                +L * 0.48 - 0.98*(L / ((double)MARCADORES) ) * ((double)k), 
                0);
        drawLine(-L * 0.52, +L * 0.488 - 0.98*(L / ((double)MARCADORES) ) * ((double)k), 0,
                 -L * 0.5, +L * 0.488 - 0.98*(L / ((double)MARCADORES) ) * ((double)k), 0 ); // risco

        glColor3f(0.9,0.9,0.9);
        drawLine(-L * 0.5, +L * 0.488 - 0.98*(L / ((double)MARCADORES) ) * ((double)k), 0,
                 +L * 0.5, +L * 0.488 - 0.98*(L / ((double)MARCADORES) ) * ((double)k), 0 ); // linha
    }
}

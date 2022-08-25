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

#include "constants.h"

/* ----------------------------------------------
    Desenha a malha com suas celulas
------------------------------------------------ */
void desenharMalha( Grid *grid )
{
   Cell *gridCell;

    // Percorre todas as celulas
    for(gridCell = grid->firstGridCell; 
        gridCell != 0; 
        gridCell = gridCell->next)
    {  
        // ---------------- Desenha malha ---------------------
        // Desenha bordas das celulas no dominio
    
        glColor3f(0.0f,0.0f,0.0f);  // Preto
        drawLine(-LHORI/2+(gridCell->centerX-gridCell->halfFaceLength),
                 -LHORI/2+(gridCell->centerY+gridCell->halfFaceLength), 
                 0,
                    -LHORI/2+(gridCell->centerX+gridCell->halfFaceLength),
                    -LHORI/2+(gridCell->centerY+gridCell->halfFaceLength),
                    0);

        drawLine(-LHORI/2+(gridCell->centerX+gridCell->halfFaceLength),
                 -LHORI/2+(gridCell->centerY+gridCell->halfFaceLength), 
                 0,
                    -LHORI/2+(gridCell->centerX+gridCell->halfFaceLength),
                    -LHORI/2+(gridCell->centerY-gridCell->halfFaceLength),
                    0);

        drawLine(-LHORI/2+(gridCell->centerX+gridCell->halfFaceLength),
                 -LHORI/2+(gridCell->centerY-gridCell->halfFaceLength), 
                 0,
                    -LHORI/2+(gridCell->centerX-gridCell->halfFaceLength),
                    -LHORI/2+(gridCell->centerY-gridCell->halfFaceLength),
                    0);

        drawLine(-LHORI/2+(gridCell->centerX-gridCell->halfFaceLength),
                 -LHORI/2+(gridCell->centerY-gridCell->halfFaceLength), 
                 0,
                    -LHORI/2+(gridCell->centerX-gridCell->halfFaceLength),
                    -LHORI/2+(gridCell->centerY+gridCell->halfFaceLength),
                    0); 
   }
}                          

/* -----------------------------------------------
    Desenha valores da densidade em cada celula
-------------------------------------------------- */
void desenharDensidade( Grid *grid )
{
    Cell *gridCell;

    // Percorre todas as celulas
    for(gridCell = grid->firstGridCell; 
        gridCell != 0; 
        gridCell = gridCell->next)
    {
        // Cor de acordo com o valor da densidade
        if( gridCell->rhoAtual >= 0  // Positivo
            && fabs(gridCell->rhoAtual) 
                 < (2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)) )
        {
           glColor3f(
              0.0f ,
                 0.008f +
                 0.88*(gridCell->rhoAtual)
                 /(2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)) ,
                    (gridCell->rhoAtual)*(gridCell->rhoAtual)
                    /(2.2*PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE)));
        }
        // Estourado
        else 
            glColor3f(1.0f,0.0f,1.0f);
         
        // Desenha os quadrados
        glBegin(GL_QUADS);       
            glVertex3d(
                    (gridCell->centerX-gridCell->halfFaceLength)        
                    -(LHORI/2.0),
                        
                    (gridCell->centerY+gridCell->halfFaceLength)
                    -(LHORI/2.0),
                            
                    0);
        
            glVertex3d(
                    (gridCell->centerX+gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    (gridCell->centerY+gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    0);

            glVertex3d(
                    (gridCell->centerX+gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    (gridCell->centerY-gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    0);

            glVertex3d(
                    (gridCell->centerX-gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    (gridCell->centerY-gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    0);
        glEnd();    
    }
}

/* -----------------------------------------------
    Desenha valores da temperatura em cada celula
-------------------------------------------------- */
void desenharTemperatura( Grid *grid )
{
    Cell *gridCell;

    // Percorre todas as celulas
    for(gridCell = grid->firstGridCell; 
        gridCell != 0; 
        gridCell = gridCell->next)
    {
        // Cor de acordo com o valor da densidade
        if( gridCell->TAtual >= 273.0   // Acima de 0 celsius
            &&  fabs(gridCell->TAtual - 273.0) 
                  < (70.0*(TEMPERATURA_LIVRE-273.0)) )
        {
           glColor3f(              
              4.0*(gridCell->TAtual - 273.0)/(70.0*(TEMPERATURA_LIVRE-273.0)),
                 0.004*(gridCell->TAtual - 273.0)*(gridCell->TAtual - 273.0)
                 /(70.0*(TEMPERATURA_LIVRE-273.0))
                 + 800.0*(TEMPERATURA_LIVRE-273.0)/((gridCell->TAtual - 90.0)
                 *(gridCell->TAtual - 90.0)),
                    800.0*(TEMPERATURA_LIVRE-273.0)/((gridCell->TAtual - 90.0)
                    *(gridCell->TAtual - 90.0)) );
        }
        else if( gridCell->TAtual < 273.0   // Abaixo de 0 celsius
            &&  fabs(gridCell->TAtual - 273.0) 
                  < (70.0*(TEMPERATURA_LIVRE-273.0)) )
        {
           glColor3f(              
              0.0f,
                 800.0*(TEMPERATURA_LIVRE-273.0)
                 /(fabs(gridCell->TAtual + 456.0)
                 *fabs(gridCell->TAtual + 456.0)),
                    fabs(gridCell->TAtual - 273.0)
                    /(10.0*(TEMPERATURA_LIVRE-273.0))
                    + 800.0*(TEMPERATURA_LIVRE-273.0)
                    /(fabs(gridCell->TAtual + 456.0)
                    *fabs(gridCell->TAtual + 456.0)) );
        }
        // Estourado
        else
            glColor3f(1.0f,0.0f,1.0f);

        // Desenha os quadrados
        glBegin(GL_QUADS);       
            glVertex3d(
                    (gridCell->centerX-gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    (gridCell->centerY+gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    0);
        
            glVertex3d(
                    (gridCell->centerX+gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    (gridCell->centerY+gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    0);

            glVertex3d(
                    (gridCell->centerX+gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    (gridCell->centerY-gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    0);

            glVertex3d(
                    (gridCell->centerX-gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    (gridCell->centerY-gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    0);
        glEnd();    
    }
}

/*------------------------------------------------------------------------------
                    Desenha o campo vetorial de velocidades                     
------------------------------------------------------------------------------*/
void desenharVelocidade( Grid *malha, double valorMaximo = 0.0 )
{
    double tamanhoHorizontal, tamanhoVertical;
    double xA, yA,
           xB, yB,
           xC, yC,
           xD, yD;
	
	Cell *celula;

	// Encontrar o maior valor u ou v entre todas as celulas
	for(celula = malha->firstGridCell;
        celula != NULL;
        celula = celula->next)
    {
		// Maior u
		if( fabs(celula->uAtual) > valorMaximo ) 
			valorMaximo = fabs(celula->uAtual);

		// Maior v
		if( fabs(celula->vAtual) > valorMaximo )
			valorMaximo = fabs(celula->vAtual);
	}

	// Desenha um vetor em cada celula
	for(celula = malha->firstGridCell;
        celula != NULL;
        celula = celula->next)
    {
        tamanhoHorizontal = ( celula->uAtual / ( (valorMaximo != 0.0)? valorMaximo : 1.0 ) )
                            *1.5*(celula->halfFaceLength);
        tamanhoVertical = ( celula->vAtual / ( (valorMaximo != 0.0)? valorMaximo : 1.0 ) )
                            *1.5*(celula->halfFaceLength);

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

        // O vetor é desenhado em azul
        glColor3f( 0.0f, 0.08f, 0.35f );

        // Corpo do vetor                    
        glBegin( GL_LINES );
                glVertex3d( xA - (LHORI/2.0),
                            yA - (LHORI/2.0), 
                            0 );                          
               glVertex3d( xB - (LHORI/2.0),
                           yB - (LHORI/2.0), 
                           0 );
        glEnd();

        // desenhando a ponta do vetor.
        glBegin( GL_TRIANGLES );   
             glVertex3d( xB - (LHORI/2.0),
                         yB - (LHORI/2.0),
                         0.0 );
            glVertex3d( xC - (LHORI/2.0),
                        yC - (LHORI/2.0),
                        0.0 );
            glVertex3d( xD - (LHORI/2.0),
                        yD - (LHORI/2.0),
                        0.0 );
        glEnd();
    }
}

/* -----------------------------------------------
    Desenha valores da pressao em cada celula
-------------------------------------------------- */
void desenharPressao( Grid *grid )
{
    Cell *gridCell;

    // Percorre todas as celulas
    for(gridCell = grid->firstGridCell; 
        gridCell != 0; 
        gridCell = gridCell->next)
    {
        // Cor de acordo com o valor da densidade
        if( fabs( (gridCell->pAtual)*(gridCell->pAtual) ) 
              < (8.0*PRESSAO_LIVRE*PRESSAO_LIVRE)
        && gridCell->pAtual > 0 )
        {
           glColor3f(              
              0.0f,
                 (gridCell->pAtual)*(gridCell->pAtual)
                 /(8.0*PRESSAO_LIVRE*PRESSAO_LIVRE)
                 + (PRESSAO_LIVRE*PRESSAO_LIVRE)
                 /(((gridCell->pAtual)+150000)*((gridCell->pAtual)+150000)),
                    (PRESSAO_LIVRE*PRESSAO_LIVRE)
                    /(((gridCell->pAtual)+150000)*((gridCell->pAtual)+150000)) );
        }
        // Estourado
        else
            glColor3f(1.0f,0.0f,1.0f);

        // Desenha os quadrados
        glBegin(GL_QUADS);       
            glVertex3d(
                    (gridCell->centerX-gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    (gridCell->centerY+gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    0);
        
            glVertex3d(
                    (gridCell->centerX+gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    (gridCell->centerY+gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    0);

            glVertex3d(
                    (gridCell->centerX+gridCell->halfFaceLength)
                    -(LHORI/2.0),
                    
                    (gridCell->centerY-gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    0);

            glVertex3d(
                    (gridCell->centerX-gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    (gridCell->centerY-gridCell->halfFaceLength)
                    -(LHORI/2.0),

                    0);
        glEnd();    
    }
}
  

/* -----------------------------------------------
    Desenha valores de uma variavel em cada celula
-------------------------------------------------- */
void desenharValores( Grid *grid , int QualGrafico )
{
    Cell *gridCell;
    char valor[5];

    glColor3f(0.6f,0.6f,0.6f);
    // Percorre todas as celulas
    for(gridCell = grid->firstGridCell; 
        gridCell != 0; 
        gridCell = gridCell->next)
    {
        // Preencher string com valores
        switch(QualGrafico){
            case 1:  //imprimir densidade
                sprintf(valor, "%1.2f", (gridCell->rhoAtual) );    
            break;

            case 2:  //imprimir temperatura
                sprintf(valor, "%1.0f", gridCell->TAtual );    
            break;

            case 3:  //imprimir velocidade
                sprintf(valor, "%1.1f", (gridCell->uAtual+gridCell->vAtual) );    
            break;          
        
            case 4:  //imprimir pressao               
                sprintf(valor, "%1.0f", (gridCell->pAtual) );    
            break;
		
			default:
			break;
        }

	    drawString(valor,GLUT_BITMAP_HELVETICA_10,
            ( gridCell->centerX - ((gridCell->halfFaceLength)/1.6) ) - ( LHORI/2.0 ),
            gridCell->centerY - ( LHORI/2.0 ),
            0);
    }
}

/* -----------------------------------------------
    Desenha o valor do tempo atual
-------------------------------------------------- */
void desenharTempo( Grid *grid, double zoom, 
                    double deslocamentoX, double deslocamentoY )
{
    char string_tempo[13];

    // Preencher string com valor do tempo atual
    sprintf( string_tempo, "%1.11f", grid->tempoAtual );    
                       
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

}

/* -----------------------------------------------
    Desenha um aviso de pausa
-------------------------------------------------- */
void desenharPausa( Grid *grid, double zoom , 
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
void desenharLegenda( Grid *grid, double zoom , 
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

}

/* -----------------------------------------------
    Desenha a placa plana na borda sul (enfeite)
-------------------------------------------------- */
void desenharPlacaPlana( Grid *grid )
{    
    // Desenhar placa plana na forma de poligono
    glBegin(GL_POLYGON);
        glColor3f(0.5f,0.5f,0.5f);       
        glVertex3d(-LHORI/2.0,
                   -LHORI/2.0,
                   0);

        glColor3f(0.48f,0.48f,0.48f);
        glVertex3d(LHORI*(0.15)-(LHORI/2.0),
                   -1.1*(LHORI/2),
                   0);

        glColor3f(0.55f,0.55f,0.55f);
        glVertex3d((LHORI/2),
                   -1.1*(LHORI/2),
                   0);

        glColor3f(0.38f,0.38f,0.38f);
        glVertex3d((LHORI/2),
                   -(LHORI/2),
                   0);
    glEnd();
} 

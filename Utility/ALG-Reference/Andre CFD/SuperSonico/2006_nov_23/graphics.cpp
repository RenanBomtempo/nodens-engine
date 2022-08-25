//graphics.cpp

  // Para linux
//#include <glut.h>         // glut.h includes gl.h and glu.h


// glut para Windows
#include <gl/glut.h>
#include <gl/gl.h>

#include "constants.h"

//Desenha uma reta dados os pontos inicial e final em coordenadas double.
void drawLine ( double x0, double y0, double z0, double x1, double y1,  double z1)
{
    glBegin(GL_LINES);       
        glVertex3d(LARGURA_MALHA*x0 - (LARGURA_MALHA/2),
                   ALTURA_MALHA*y0 - (ALTURA_MALHA/2),
                   20*z0);
        glVertex3d(LARGURA_MALHA*x1 - (LARGURA_MALHA/2),
                   ALTURA_MALHA*y1 - (ALTURA_MALHA/2),
                   20*z1); 
    glEnd();
}

//Colore um retângulo dados os vértices da diagonal / (e não de \).
void paintRectangle ( double x0, double y0, double x1, double y1 )
{
    glBegin(GL_POLYGON);
        glVertex2d (400*x0, 400*y0);
        glVertex2d (400*x1, 400*y0);
        glVertex2d (400*x1, 400*y1);
        glVertex2d (400*x0, 400*y1);
    glEnd();
}

//Colore uma célula dados as coordenadas do seu centro e metade do seu comprimento.
void paintCell( double centerX, double centerY, double halfCellSide )
{
    double x0, y0, x1, y1;
    x0 = centerX - halfCellSide;
    y0 = centerY - halfCellSide;
    x1 = centerX + halfCellSide;
    y1 = centerY + halfCellSide;
    paintRectangle( x0, y0, x1, y1 );
}

//Define o array tableOfColors que é uma tabela das cores principais no sistema 
//RGB a serem usadas pela função setColor.
static float tableOfColors[8][3] = { 
                              {0.0, 0.0, 0.0},  // 0 = preto
                              {1.0, 0.0, 0.0},  // 1 = vermelho
                              {1.0, 1.0, 0.0},  // 2 = amarelo
                              {0.0, 1.0, 0.0},  // 3 = verde
                              {0.0, 1.0, 1.0},  // 4 = azul claro
                              {0.0, 0.0, 1.0},  // 5 = azul
                              {1.0, 0.0, 1.0},  // 6 = violeta
                              {1.0, 1.0, 1.0}}; // 7 = branco

//Define a cor que será usada para desenhar.
void setColor (int color)
{
    glColor3f(tableOfColors[color][0], 
              tableOfColors[color][1], 
              tableOfColors[color][2]);

}

// Pinta uma string na tela, com uma dada fonte e posicao
void drawString(const char* string, void* fonte, double Px, double Py, double Pz) 
{
    // Posiciona
    glRasterPos3f(Px,Py,Pz);
    
    // Imprime cada caractere da string
    char* aux = (char*) string;
    while( *aux != '\0')
        glutBitmapCharacter(fonte, *aux++);

}

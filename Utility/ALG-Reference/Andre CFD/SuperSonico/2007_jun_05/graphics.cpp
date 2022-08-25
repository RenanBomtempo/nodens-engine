//graphics.cpp

// glut para Windows e Linux
#include <GL/glut.h>
#include <GL/gl.h>

#include "constants.h"

//Desenha uma reta dados os pontos inicial e final em coordenadas double.
void drawLine ( double x0, double y0, double z0, double x1, double y1,  double z1)
{
    glBegin(GL_LINES);       
        glVertex3d(x0,y0,z0);
        glVertex3d(x1,y1,z1); 
    glEnd();
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

/*
        drawTriangle
    Desenha um triangulo na tela com os vertices cujas coordenadas sao recebidas por parametro.
*/
void drawTriangle( double x0, double y0, double z0,
                   double x1, double y1, double z1,
                   double x2, double y2, double z2 )
{
   glBegin(GL_POLYGON);
      glVertex3d(x0, y0, z0);
      glVertex3d(x1, y1, z1);
      glVertex3d(x2, y2, z2);
   glEnd();
}

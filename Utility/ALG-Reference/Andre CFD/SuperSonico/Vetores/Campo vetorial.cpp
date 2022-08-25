#include <gl/glut.h>
#include <gl/gl.h>
#include <math.h>

GLfloat escala = 1;      // Escala dos eixos coordenados.
GLint zoom_In, zoom_Out; // Variáveis que indicam um zoom positivo ou negativo, respectivamente.
GLfloat dx = 0;          // Variação da translação no eixo Y.
GLfloat dy = 0;          // Variação da translação no eixo X.
GLfloat dz = 0;          // Variação da translação no eixo Z.
GLfloat angx = 0;        // Ângulo de rotação em torno do eixo X.
GLfloat angy = 0;        // Ângulo de rotação em torno do eixo Y.
GLint eixos = 0;         // Variável que ativa ou não a exibição dos eixos coordenados.
GLint opcao = 0;         // Opção de objeto a ser desenhado.


// Função chamada para escrever textos
void escreveTexto( double x, double y, char *string )
{
  	glPushMatrix();
        // Posição do espaço onde o texto será escrito.
		glRasterPos2f( x, y );
        // Exibe caracter a caracter.
        while(*string)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*string++);
	glPopMatrix();
}

// Função chamada para fazer um zoom na figura.
void zoom( void )
{
     if ( (zoom_In == 1) || (zoom_Out == 1) )
        glScalef( escala, escala, 0 );
}

// Eixos coordenados.
void Eixos( void )
{
     glLineWidth(2);
     glBegin( GL_LINES );   
          glColor3f(1,1,1);
          glVertex2f(0,0);
          glVertex2f(11,0);
          glVertex2f(0,0);  
          glVertex2f(0,11);
          glVertex2f(0,0);
          glVertex2f(0,0);
     glEnd();
     
     glColor3f(0,1,0);
     escreveTexto(12,0,"x");
     escreveTexto(0,12,"y");
}

//______________________________________________________________________________

// função que desenha um vetor dado suas coordenadas  iniciais  e  o  incremento
// nessas variaveis.
void desenhaVetor( float x, float y, float u, float v )
{
     float tamanho = 0.1, /* tamanho do triângulo. (Obs.:  Nâo  ponha  valores 
                              maiores ou muito próximo de 1. */
                              
           // coordenadas do meio do vetor.
           x_medio = x,
           y_medio = y,
           
           // coordenadas do início do vetor.                 
           xA = x_medio - u / 2.0,
           yA = y_medio - v / 2.0,
           
           // coordenadas do final do vetor.
           xC = x_medio + u / 2.0,
           yC = y_medio + v / 2.0,
           
           // coordenada do ponto médio da base do triângulo.
           xB = (1 - tamanho) * (xC - xA) + xA,
           yB = (1 - tamanho) * (yC - yA) + yA,           
           
           // comprimento do vetor e outras variáveis auxiliares.
           modAC = sqrt( u*u + v*v ),
           r = (0.95*(tamanho) / 2.0 ) * modAC,
           a = (yB - yA) / (xB - xA),
           c = -1.0 / a,
           d = (xB / a) + yB,
           
           k1 = 1 + c*c,
           k2 = 2.0*c*d - 2.0*xB - 2.0*yB*c,
           k3 = xB*xB + d*d - 2.0*yB*d + yB*yB - r*r,
           delta = k2*k2 - 4.0*k1*k3,
           
           // coordenadas de um dos vértices do triângulo.
           xD = (-k2 + sqrt( delta )) / (2*k1),
           yD = c*xD + d,
           
           // coordenadas de outro vértice do triângulo.
           xE = (-k2 - sqrt( delta )) / (2*k1),   
           yE = c*xE + d;
           
      // configurando a cor para vermelho e a largura da linha para 1.
      glColor3f( 1, 0, 0 );
      glLineWidth(1);
           
      // desenhando o corpo do vetor.
      glBegin( GL_LINES );
          glVertex3d( xA, yA, 0 );
          glVertex3d( xC, yC, 0 );
      glEnd();
           
      // desenhando a ponta do vetor.
      glBegin( GL_TRIANGLES );
          glVertex3d( xD, yD, 0 );
          glVertex3d( xE, yE, 0 );
          glVertex3d( xC, yC, 0 );
      glEnd();
}

// função que desenha um campo vetorial
void desenhaCampo()
{
     zoom();
     //desenhaVetor( -1, 1, -3, -5 );
     
     for( int i = -5; i <= 5; i ++ )
     {
          for( int j = -5; j <= 5; j ++)
          {
               // campo 1.
               //if( i != 0 && j != 0 )
               desenhaVetor( i, j, 0.5*(-j), 0.0*i );
               
               // campo 2.
               //desenhaVetor( i , j, j / (sqrt(i*i + j*j)), i / ((sqrt(i*i + j*j)))) ;
               
               // campo 3
               //desenhaVetor( i, j, 0.5*j, 2.1*sin(i) );
               
               // campo 4
               //desenhaVetor( i, j, 0.5*j, 0.5*i );
               
               //campo 5
               //desenhaVetor( i, j, 1.3*sin(i), 1.3*sin(j) );
          }
     }
     
} 

//______________________________________________________________________________

// Função chamada para gerenciar eventos do teclado.
void teclado( unsigned char tecla, int x, int y )
{
    switch ( tecla )
    {
            case '+': 
            case '=': { zoom_In = 1; escala *= 1.02; break; }
            
            case '_':
            case '-': { zoom_Out = 1; escala /= 1.02; break; } 
    }
    glutPostRedisplay();
}

/* Função  chamada para gerenciar eventos 
  com teclas especiais do teclado, tais como UP, DOWN, LEFT e RIGHT. */
void teclasEspeciais( int tecla, int x, int y )
{
    if(tecla == GLUT_KEY_UP)        // Translação para cima.
        dy += 0.7;              

    if(tecla == GLUT_KEY_DOWN)      // Translação para baixo.
        dy -= 0.7;

    if(tecla == GLUT_KEY_RIGHT)     // Translação para direita.
        dx += 0.7;

    if(tecla == GLUT_KEY_LEFT)      // Translação para esquerda. 
        dx -= 0.7;

    glutPostRedisplay();
}

//_____________________________________________________________________________________________________________________________________________

// Função chamada para iniciar parâmetros usados pela OpenGL. 
void inicializa ( void )
{   
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);
}

void desenha( void )
{
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(-20, 20, -20, 20, -20, 20);    
     glClear(GL_COLOR_BUFFER_BIT );
     glTranslatef(dx,dy,0);
     desenhaCampo();

     if ( eixos == 0) 
        Eixos();

    glutSwapBuffers();
}
//_______________________________________________________________________________________________________________________     


int main(void)
{
     glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
     glutInitWindowSize(900,900);
     glutInitWindowPosition(10,10);
     glutCreateWindow("Maycon da Costa");
     glutDisplayFunc(desenha);
     glutIdleFunc(desenha);
     glutKeyboardFunc(teclado);
     glutSpecialFunc(teclasEspeciais);
     inicializa();    
     glutMainLoop();
}

/*
           /*
           glPointSize(4);
           glColor3f( 0, 0, 1);
           glBegin( GL_POINTS );
                glVertex2f( xA, yA );
                glVertex2f( xC, yC );
                glVertex2f( xB, yB );
                glVertex2f( xD, yD );
                glVertex2f( xE, yE );
           glEnd();
           

           glColor3f( 1, 1, 0 );
           escreveTexto(xA + 0.5, yA , "A");
           escreveTexto(xC + 0.5, yC ,"C");
           escreveTexto(xB + 0.5, yB ,"B");
           escreveTexto(xD + 0.5, yD ,"D");
           escreveTexto(xE + 0.5, yE ,"E");
*/
 

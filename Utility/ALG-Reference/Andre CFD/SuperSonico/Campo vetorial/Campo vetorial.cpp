#include <GL/glut.h>
#include <math.h>

GLfloat escala = 1;      // Escala dos eixos coordenados.
GLint zoom_In, zoom_Out; // Variáveis que indicam um zoom positivo ou negativo, respectivamente.
GLfloat dx = 0;          // Variação da translação no eixo Y.
GLfloat dy = 0;          // Variação da translação no eixo X.
GLint eixos = 0;         // Variável que ativa ou não a exibição dos eixos coordenados.
GLint opcao = 0;         // Opção do campo a ser desenhado.
GLfloat ang_z = 0;       // Ângulo de rotação em torno do eixo z.
GLint rot = 0;           // Indica se o estado da rotação é ativo ou inativo.

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
     glLineWidth(1);
     glColor3f(0,0,0);
     glBegin( GL_LINES );   
          // eixo x.
          glVertex2f(0,0);
          glVertex2f(11,0);

          // eixo y.
          glVertex2f(0,0);  
          glVertex2f(0,11);
     glEnd();
     
     glBegin( GL_TRIANGLES );
          // ponta do eixo x.
          glVertex2f(11,0);
          glVertex2f(10.6,0.15);
          glVertex2f(10.6,-0.15);

          // ponta do eixo y.
          glVertex2f(0,11);          
          glVertex2f(0.15,10.6);
          glVertex2f(-0.15,10.6);
     glEnd();
     
     glColor3f(0,0,1);
     escreveTexto(11.5,0,"x");
     escreveTexto(0,11.5,"y");
}

//______________________________________________________________________________

// função que desenha um vetor dado suas coordenadas  de seu centro e os valores
// dos deslocamentos nas direções x e y, respectivamente.
void desenhaVetor( float x, float y, float u, float v )
{
     float tamanho = 0.15, /* tamanho do triângulo. (Obs.:  Nâo  ponha  valores 
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
           
           // coordenas de dois vértices do triangulo.
           xD, yD,
           xE, yE,           
           
           // comprimento do vetor e outras variáveis auxiliares.
           modAC = sqrt( u*u + v*v ),
           r = (0.95*(tamanho) / 2.0 ) * modAC,
           a = (yB - yA) / (xB - xA);
     
     if (a != 0)
     {
           float c = -1.0 / a,
                 d = (xB / a) + yB,
                  
                 k1 = 1 + c*c,
                 k2 = 2.0*c*d - 2.0*xB - 2.0*yB*c,
                 k3 = xB*xB + d*d - 2.0*yB*d + yB*yB - r*r,
                 delta = k2*k2 - 4.0*k1*k3;
                   
                 // coordenadas de um dos vértices do triângulo.
                 xD = (-k2 + sqrt( delta )) / (2*k1);
                 yD = c*xD + d;
                   
                 // coordenadas de outro vértice do triângulo.
                 xE = (-k2 - sqrt( delta )) / (2*k1);   
                 yE = c*xE + d;
     }
     else
     { 
         xD = xB;
         yD = yB + r;
         xE = xB;
         yE = yB - r;
     }
           
      // configurando a cor para vermelho e a largura da linha para 1.3 .
      glColor3f(1, 0, 0);
      glLineWidth(1.3);
           
      // desenhando o corpo do vetor.
      glBegin( GL_LINES );
          glVertex2f( xA, yA );
          glVertex2f( xC, yC );
      glEnd();
           
      // desenhando a ponta do vetor.
      glBegin( GL_TRIANGLES );
          glVertex2f( xD, yD );
          glVertex2f( xE, yE );
          glVertex2f( xC, yC );
      glEnd();
}

// função que desenha um campo vetorial
void desenhaCampo( int opcao )
{
     zoom();
     switch( opcao )
     {
          case 1: 
          {
               for( int i = -10; i <= 10; i ++ )
                   for( int j = -10; j <= 10; j ++)
                        desenhaVetor( i, j, 0.5*(-j), 0.5*i );
               break;
          }
          
          case 2: 
          {
               for( int i = -10; i <= 10; i ++ )
                   for( int j = -10; j <= 10; j ++)
                        desenhaVetor( i , j, j / (sqrt(i*i + j*j)), i / ((sqrt(i*i + j*j)))) ;
               break;
          }
          
          case 3: 
          {
               for( int i = -10; i <= 10; i ++ )
                   for( int j = -10; j <= 10; j ++)
                        desenhaVetor( i, j, 0.5*j, 0.5*i );
               break;
          }
          
          case 4: 
          {
               for( int i = -10; i <= 10; i ++ )
                   for( int j = -10; j <= 10; j ++)
                        desenhaVetor( i, j, 1.3*sin(i), 1.3*sin(j) );
               break;
          }
          
          default: break;
     }        
} 

// função que relaciona o campo a ser desenhado com as opções do menu.
void campo( int op )
{
     switch ( op )
     {
            case 1: { opcao = 1; break; }
            case 2: { opcao = 2; break; }
            case 3: { opcao = 3; break; }
            case 4: { opcao = 4; break; }
            default : break;
     }
}

// função que relaciona ferramentas do programa com o menu.
void ferramentas( int op )
{
     switch ( op )
     {
         case 11: { eixos = 1; break; }
         case 21: { eixos = 0; break; }
         case 31: { rot = 1; break; }
         case 41: { rot = 0; break; }
         default : break;
     }
}     

// função que cria menu
void criaMenu()
{
     int menu, submenu;
          
     submenu = glutCreateMenu( ferramentas );
     glutAddMenuEntry( "Exibir eixos", 11 );
     glutAddMenuEntry( "Remover eixos", 21 );
     glutAddMenuEntry( "Ativar rotação", 31 );
     glutAddMenuEntry( "Desativar rotação", 41 );
                    
     menu = glutCreateMenu( campo );
     glutAddMenuEntry( "Campo 1", 1 );
     glutAddMenuEntry( "Campo 2", 2 );
     glutAddMenuEntry( "Campo 3", 3 );
     glutAddMenuEntry( "Campo 4", 4 );
     glutAddSubMenu( "Ferramentas", submenu );
     
     glutAttachMenu( GLUT_RIGHT_BUTTON );
} 

// Função chamada para gerenciar eventos do mouse.
void mouse( int botao, int estado, int x, int y )
{
    switch( botao )
    {
              case GLUT_RIGHT_BUTTON:
                 {
                      if (estado == GLUT_DOWN)
                          criaMenu();
                 } 
              
              default:
              break;
    }       
    glutPostRedisplay();
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glShadeModel(GL_SMOOTH);
}

void desenha( void )
{
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(-15, 15, -15, 15, -15, 15);    
     glClear(GL_COLOR_BUFFER_BIT );
     glTranslatef(dx,dy,0);
     glRotatef( ang_z, 0, 0, 1);
     
     desenhaCampo( opcao );
   
     if( rot == 1 )
          ang_z += 0.5;
          
     if( eixos == 1 )
          Eixos();  

     glutSwapBuffers();
}
//_______________________________________________________________________________________________________________________     


int main(void)
{
     glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
     glutInitWindowSize( 900, 900 );
     glutInitWindowPosition( 10, 10 );
     glutCreateWindow( "Maycon da Costa" );
     glutDisplayFunc( desenha );
     glutIdleFunc( desenha );
     glutMouseFunc( mouse );
     glutKeyboardFunc( teclado );
     glutSpecialFunc( teclasEspeciais );
     inicializa();    
     glutMainLoop();
}

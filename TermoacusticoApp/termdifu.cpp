/*******************************************************/
/* PROGRAMA: termdifu.cpp                                */
/* AUTORA  : DENISE BURGARELLI DUCZMAL                 */
/* DATA    : 29/06/96                                  */
/*******************************************************/

#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "funcoes.h"
#include "quad.h" 
#include <fstream>
    using std::ofstream;

#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;
    
#define null 0
/* ******** variaveis externas ***************** */

extern int flagglobal;
extern int flagpeano;
extern quad gne0;
FILE *fopen();

   
extern int npcont; /* numero de quad. */

/****** variaveis globais neste programa ******** */  
     
double deltat_term; /* valor que vem do programa term.c    */ 
int flag_term;      /* tipo da incognita que vem do term.c */



/* ******************************************************* */
/* FUNCAO : Inicializa o vetor cx para o metodo grad.conj. */
/* ******************************************************* */
void term_init_gradconj()
{
 quad q1;
 double noise;

 noise = ( ( (float)(rand()) ) / ( (float)(RAND_MAX ) ) ) *0.0001;
 q1=gne0;
 do{
   if(q1->ativo == 1){
      if(flag_term == 0){
         /* velocidade na direcao x */

         /* if((q1->velxatual<0.0001)&&(q1->velxatual>(-0.0001)) ){ 
              q1->cx =0.00001 ; 
          } else{ */

             q1->cx = (q1-> velxatual) ;

          /*}*/
       }
       else if(flag_term == 1 ){
               /* velocidade na direcao y */
               /*q1->cx = 100.0; */ 
              /* if((q1->velyatual<0.0001)&&(q1->velyatual>-0.0001)){
                   q1->cx = 0.00001;
                }
               else{ */
                   q1->cx=(q1->velyatual);
              /* }*/
        }
        else if(flag_term ==2 ){
               /* temperatura */
               /*  q1->cx =200.0 ; */  
              q1->cx = (q1->temperatual);
        }
   }
   q1= q1->next;
 }while(q1!=null);

} 

/***********************************************************/
/* FUNCAO:                                                 */
/* Coloca coeficiente da matriz principal do sistema Ax=b  */
/* numa posicao da lista encadeada associada ao quadr. q1. */
/* Esta lista contem todos os elementos nao-nulos da linha */
/* da matriz correspondente ao quadradinho q1.             */
/* Se a aresta for fronteira, devemos atualizar o vetor b. */
/***********************************************************/
void term_coefquad(quad q, quad q1, quad qua, char dir)
{
  double fr,fr0,val,meiolado;
  int fri,i,col,col1,aux,nivel;
  mlin atual,novo; 

  if(qua->nivel > q->nivel)
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->c;
    }
  else
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->b;
    }
  q=qua;

  /* **************************************************** */
  if((q->tipo=='a') || (q->ativo == 0)){
   

    /* Aresta fronteira: adicione ao vetor b. */
    fri=1;
    i=q1->nivel; /* ATENCAO */
    while(i>0){
      fri*=2;
      i--;
    }
  /*calculo de meiolado do quad. q1 */
   aux=2;
   nivel=q1->nivel;
   for(i=0;i<nivel;i++){
      aux=2*aux;
   } 
  meiolado=1.0/(float)aux;

   /* Use as funcoes do valor de contorno. */
   /* Adiciona ao vetor cb o valor devido a fronteira */ 
   if(dir=='b'){
     if (flag_term==0){
         if(q1->placa==1){
             fr= q1->velxbaixo / meiolado;   /* ATENCAO */ 
             q1->cb+= (deltat_term*fr);
      q1->elem->valor=(q1->elem->valor)+(deltat_term/meiolado);
          }  
     }
     else if (flag_term == 1){
              fr= q1->velybaixo  / meiolado;  
              q1->cb+=(deltat_term*fr); 
      q1->elem->valor=(q1->elem->valor)+(deltat_term/meiolado);
     }
          else if (flag_term == 2){
               if(q1->placa==1){
                   fr= q1->temperbaixo  / meiolado;
                   q1->cb+=(deltat_term*fr); 
      q1->elem->valor=(q1->elem->valor)+(deltat_term/meiolado);
               }
           }
   }
    else if(dir=='c') {
     if (flag_term == 1){
         fr= q1->velycima  / meiolado; 
         q1->cb+=(deltat_term*fr); 
         q1->elem->valor=(q1->elem->valor)+(deltat_term/meiolado);
     }
    }  
    else if(dir=='d') {
     if (flag_term==0){
         fr= q1->velxdireita / meiolado; 
         q1->cb+=(deltat_term*fr);   
         q1->elem->valor=(q1->elem->valor)+(deltat_term/meiolado);
     }
     else if (flag_term == 1){
              fr= q1->velydireita  / meiolado;
              q1->cb+=(deltat_term*fr);  
         q1->elem->valor=(q1->elem->valor)+(deltat_term/meiolado);
     }
    }
    else if(dir=='e') {
     if (flag_term==0){
        fr= q1->velxesquerda  /meiolado ;
        q1->cb+=(deltat_term*fr); 
         q1->elem->valor=(q1->elem->valor)+(deltat_term/meiolado);
       
     }
     else if (flag_term == 1){
             fr= q1->velyesquerda / meiolado;
             q1->cb+=(deltat_term*fr); 
         q1->elem->valor=(q1->elem->valor)+(deltat_term/meiolado);
     }
    }
}/*final do se 'a'ou inativo */
  /* ************************************************************ */
  else  if((qua->tipo=='q')&& (qua->ativo==1) ){

    /* Aresta interna */  
    if((q1->nivel > q->nivel)||
       ((q1->nivel == q->nivel) &&
        ((q1->coord[0] > q->coord[0])||
        ((q1->coord[0] == q->coord[0])&&(q1->coord[1] > q->coord[1]))))
      )
       /* ordem do dicionario */
    {
      atual=q1->elem;
      /* Procura se existe elemento com coluna q->np na lista */
      do{  
        atual=atual->next;
      }while((atual != null) && (atual->coluna != col));
      /*if( (atual == null) ){*/   
      /* Ainda nao foi visitado; marque abaixo e acima da diagonal. */
        fri=1;
        /* i=(q->nivel) - (q1->nivel); */  /* Muita  ATENCAO */
        i=(q1->nivel) - (q->nivel);  /* Muita ATENCAO */
        while(i>0){
          fri*=2;
          i--;
        }
        fr=(1.0)/(float)fri;
        fri=1;
        i=q1->nivel; /* ATENCAO */
        while(i>0){
          fri*=2;
          i--;
        }
        fr/=(float)fri;

        
  /* O procedimento abaixo vai fazer a marcacao simultanea */
  /* do relacionamento de q1 com q e de q com q1.          */ 

  /* Marque a diagonal de q1. e a diagonal de q.*/
  if(dir=='b'){
    q1->elem->valor=(q1->elem->valor)+((deltat_term)/(q1->meiolado+q1->meiolado));
    q->elem->valor =(q->elem->valor) +((deltat_term)/(q1->meiolado+q->meiolado));
  }
  else if(dir=='c'){
    q1->elem->valor=(q1->elem->valor)+((deltat_term)/(q1->meiolado+q1->meiolado));
    q->elem->valor =(q->elem->valor) +((deltat_term)/(q1->meiolado+q->meiolado));
  }
  else if(dir=='d'){
   q1->elem->valor=(q1->elem->valor)+((deltat_term)/(q1->meiolado + q1->meiolado));
   q->elem->valor =(q->elem->valor) +((deltat_term)/(q1->meiolado + q->meiolado));
  }
  else if(dir=='e'){
  q1->elem->valor=(q1->elem->valor)+((deltat_term)/(q1->meiolado + q1->meiolado));
  q->elem->valor =(q->elem->valor) +((deltat_term)/(q1->meiolado + q1->meiolado));
  }
       
 
   atual=q1->elem;
   col=q->np;
   /* Procura se existe elemento com coluna col na lista */
   do{  
      novo=atual;
      atual=atual->next;
   }while((atual != null) && (atual->coluna != col));
   if(atual != null){  
      /* Caso ja exista, adicione val ao valor existente */
      if ( dir=='b'){
           atual->valor+= -(deltat_term) / (q1->meiolado+q->meiolado);
      }
      else if(dir=='c') {
           atual->valor+=(-(deltat_term)) / (q1->meiolado+q->meiolado);
      }
      if ( dir=='d'){
           atual->valor+= -(deltat_term) / (q1->meiolado+q->meiolado);
      }
      else if(dir=='e') {
           atual->valor+=(-(deltat_term)) / (q1->meiolado+q->meiolado);
      }
   }
   else{ /* Senao, crie novo elemento com valor val */ 
       novo->next=(mlin)malloc(sizeof(pmlin));
       atual=novo->next;
       atual->coluna=col;
       if (dir=='b'){
            atual->valor=(-(deltat_term)) / (q1->meiolado + q->meiolado );
       }
       else if (dir=='c'){
            atual->valor=(-(deltat_term)) / (q1->meiolado + q->meiolado );
       }
       else if (dir=='d'){
            atual->valor=(-(deltat_term)) / (q1->meiolado + q->meiolado );
       }
       else if (dir=='e'){
            atual->valor=(-(deltat_term)) / (q1->meiolado + q->meiolado );
       }
       atual->next=null;
       atual->q=q;
   }

        atual=q->elem;
        col=q1->np;
        /* Procura se existe elemento com coluna col na lista */
        do{  
          novo=atual;
          atual=atual->next;
        }while((atual != null) && (atual->coluna != col));
        if(atual != null){  
          /* Caso ja exista, adicione val ao valor existente */
          if (dir=='b'){
          atual->valor+= (-(deltat_term)) / ( q1->meiolado+q->meiolado);
          }
          else if (dir=='c') {
            atual->valor+=(-(deltat_term)) / ( q1->meiolado + q->meiolado);
          }
          else if (dir=='d'){
          atual->valor+= (-(deltat_term)) / ( q1->meiolado + q->meiolado);
          }
          else if (dir=='e') {
            atual->valor+=(-(deltat_term)) / ( q1->meiolado + q->meiolado);
          }

        }
        else{ /* Senao, crie novo elemento com valor val */ 
          novo->next=(mlin)malloc(sizeof(pmlin));
          atual=novo->next;
          atual->coluna=col;
          if (dir=='b') {
            atual->valor=(-(deltat_term)) / ( q1->meiolado+q->meiolado);
          }
          else if (dir=='c'){
            atual->valor=(-(deltat_term)) / ( q1->meiolado+q->meiolado);
          }
          else if (dir=='d') {
            atual->valor=(-(deltat_term)) / ( q1->meiolado+q->meiolado);
          }
          else if (dir=='e'){
            atual->valor=(-(deltat_term)) / ( q1->meiolado+q->meiolado);
          }
          atual->next=null;
          atual->q=q1;
        }
    }
  }
}

/* ******************************************************* */
/* FUNCAO: Atualiza os valor do vetor b a cada passo de    */
/*         tempo                                           */
/* ******************************************************* */
 void term_atualiza_cb()
{
    quad q1;

    q1=gne0;
    do{
      if(q1->ativo == 1){
         q1->cb=(q1->ladodireito); /* olhar o sinal */
      }
      q1=q1->next;
    }while(q1!=null);
}


/* ******************************************************* */
/* FUNCAO:Inicial. lista de colunas nao nulas em cada quad */
/* ******************************************************* */
void term_init_col()
{  
quad q1;
q1=gne0;
npcont=0;
do{
   q1->np=npcont;
   npcont++;
   if(q1->ativo ==1){
     q1->cb=(q1->ladodireito); /* olhar o sinal */
     q1->elem=(mlin)malloc(sizeof(pmlin));
     q1->elem->coluna=q1->np;
     q1->elem->valor= q1->fator; /* ( valor do elemento da diagonal? 1.0*/
     q1->elem->next=null;
     q1->elem->q=q1; /* diagonal */
   }    
   q1=q1->next;
}while(q1!=null);

}

/* ****************************************************** */
/* FUNCAO: Coloca os coeficientes na matriz virtual       */
/* ****************************************************** */

void term_monta_matriz()
{
 quad q1,qua,q;
 char dir;

/*  q1=gne0;
  do{
     if(q1->ativo == 1){
        q=q1; qua=q; qua=qua->b; dir='b';
        term_coefquad(q,q1,qua,dir);
        q=q1; qua=q; qua=qua->c; dir='c'; 
        term_coefquad(q,q1,qua,dir);
        q=q1; qua=q; qua=qua->d; dir='d';
        term_coefquad(q,q1,qua,dir);
        q=q1; qua=q; qua=qua->e; dir='e';
        term_coefquad(q,q1,qua,dir);
     }
     q1=q1->next;
  } while(q1!=null);

*/
 /*********************TESTANDO ****************************/
 /* Aqui a matriz considera a priori o deltax=deltay        */
 
  termmat_flag(flag_term);
  q1=gne0;
   do{
     if(q1->ativo == 1){
        q=q1; qua=q; qua=qua->b; dir='b';
        term_coefquad_matriz(q,q1,qua,dir,deltat_term);
        q=q1; qua=q; qua=qua->c; dir='c'; 
        term_coefquad_matriz(q,q1,qua,dir,deltat_term);
        q=q1; qua=q; qua=qua->d; dir='d';
        term_coefquad_matriz(q,q1,qua,dir,deltat_term);
        q=q1; qua=q; qua=qua->e; dir='e';
        term_coefquad_matriz(q,q1,qua,dir,deltat_term);
     }
     q1=q1->next;
  } while(q1!=null);

}

/* ******************************************************* */
/* FUNCAO: Chama as funcoes necessarias para a resolucao   */
/*        da equacao de difusao                            */
/* *****************************************************   */
void term_difusao( int valor, double dt) 
{
  quad q1;
  flag_term=valor;
  deltat_term = dt;
  term_init_gradconj();
  /*term_atualiza_cb();*/
  term_init_col();        /*Inicial. lista de colunas nao nulas*/
  term_monta_matriz();    /*  matriz virtual A de Ax-b         */
  gradconj();              /* metodo gradiente conjugado       */

  q1=gne0;
  do{
    if(q1->ativo == 1){
       if(flag_term == 0){ q1->velxatual=(q1->cx); }
          else if(flag_term == 1){ q1->velyatual=(q1->cx);}
            else if(flag_term == 2){ q1->temperatual=(q1->cx); }
    }
    q1=q1->next;
  } while(q1!=null);

}

/***********************************************************/
/* ****************************************************** */
/* ******************************************************* */
/* FUNCAO: Escreve a matriz A num arquivo de saida,smatriz */
/* ******************************************************* */
void term_write_matrix()
{    
 quad q1;
 mlin atual;
   
   ofstream outputFile( "outputMatrix.txt", ios::app ); //trunc
    
 q1=gne0;
 do{
    if(q1->ativo ==1){
       atual=q1->elem;
       outputFile << "Row " << q1->np + 1 << ": ";
       do{ 
           outputFile << atual->valor << " (" << q1->np + 1
                << "," << atual->coluna + 1 << ") ";
          atual=atual->next;
       }while(atual != null);
            outputFile << endl;
   }
   q1=q1->next;
 }while(q1!=null);
 
    outputFile << "________________________________________________________________________\n";
 
}


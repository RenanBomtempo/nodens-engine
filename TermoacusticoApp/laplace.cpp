/*******************************************************/
/* PROGRAMA: laplace.cpp                                 */
/* AUTORA  : DENISE BURGARELLI DUCZMAL                 */
/* DATA    : 29/06/96                                  */
/*******************************************************/

#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "funcoes.h"
#include "quad.h"
#include "PerfTimer.h"

#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;
    
#include <fstream>
    using std::ofstream;

#define null 0
/* ******** variaveis externas ***************** */
extern int flagglobal;
extern int flagpeano;
extern double cotalaplace;
extern double cotadeslap;
extern quad gne0;

extern int npcont; /* numero de quad. */
int aindaref;

double fx0(double x)
{
     double y;
     y=0.0;
     //return 10.0;
     //return x - y;
     return (x*x-y*y);
     //return (x*x*x*y-y*y*y*x);
}
double fx1(double x)
{
     double y;
     y=1.0;
     //return 10.0;
     //return x - y;
     return (x*x-y*y);
     //return (x*x*x*y-y*y*y*x);
}
double f0y(double y)
{
     double x;
     x=0.0;
     //return 10.0;
     //return x - y;
     return (x*x-y*y);
     //return (x*x*x*y-y*y*y*x);
}
double f1y(double y)
{
     double x;
     x=1.0;
     //return 10.0;
     //return x - y;
     return (x*x-y*y);
     //return (x*x*x*y-y*y*y*x);
}
double fxy(double x, double y)
{
     //return 10.0;
     //return x - y;
     return (x*x-y*y);
     //return (x*x*x*y-y*y*y*x);
}
   
double fronteira( double x, double y)
{
     
     //return 10.0;
     //return x - y;
     return (x*x-y*y);
     //return (x*x*x*y-y*y*y*x);
}

/********************************************************* */
/*Funcao diferx :Calc. a diferenca do valor obtido q1->cx*/
/*de cada quad. q1 com os vizinhos e coloca o resultado em */
/* q1->fb, q1->fc,q1->fd,q1->fe (fluxos)                   */
/* ******************************************************* */
void diferx_laplace(quad q, quad q1, quad qua, char dir) 
{
  double fr,fr0,meiolado;
  int fri,i,nivel,aux;

  if(qua->nivel > q->nivel)
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->c;
    }
  else
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->b;
    }
  q=qua;

  /*calculo de meiolado */
    aux=2;
    nivel=q1->nivel;
    for(i=0;i<nivel;i++){
       aux=2*aux;
    }
   meiolado=1.0/(float)aux;

  if((q->tipo=='a') || (q->ativo==0) ){
    if(dir=='b')      {
    fr=(((q1->cx)-fronteira(q1->coord[0],q1->coord[1]-meiolado)))*2.0;
     q1->difer+=fabs(fr);
    /* q1->fb+=fr; */
    /* Atencao : teste */
     fr=fabs(fr);
     if (fr>q1->fb) {q1->fb=fr;}
    }
    else if(dir=='c') {
    fr=(((q1->cx)-fronteira(q1->coord[0],q1->coord[1]+meiolado)))*2.0;
     q1->difer+=fabs(fr);
     /*q1->fc+=fr; */
     /* Atencao : teste */
     fr=fabs(fr);
     if(fr>q1->fc){q1->fc=fr;}
    
    }
    else if(dir=='d') {
    fr=(((q1->cx)-fronteira(q1->coord[0]+meiolado,q1->coord[1])))*2.0;
     q1->difer+=fabs(fr);
     /* q1->fd+=fr; */
     /* Atencao : teste */
     fr=fabs(fr);
     if(fr>q1->fd) {q1->fd=fr;}
    }
    else if(dir=='e') {
    fr=(((q1->cx)-fronteira(q1->coord[0]-meiolado,q1->coord[1])))*2.0;
     q1->difer+=fabs(fr);
     /* q1->fe+=fr; */
     /* Atencao : teste */
     fr=fabs(fr);
     if(fr>q1->fe) {q1->fe=fr;}
    }
  }

  else  if((qua->tipo=='q')&&(qua->ativo==1)){
    if((q1->nivel > q->nivel)||
       ((q1->nivel == q->nivel) &&
       ((q1->coord[0] > q->coord[0])||
       ((q1->coord[0] == q->coord[0])&&(q1->coord[1] > q->coord[1]))))
      )
       /* ordem do dicionario */
    {
      fri=1;
      i=(q1->nivel) - (q->nivel);
      while(i>0){
        fri*=2;
        i--;
      }
      fr=((q1->cx)-(q->cx))/(float)fri;
      q1->difer+=fabs(fr);
      q->difer+= fabs(fr);
      if(dir=='b'){
            /*  q1->fb+=fr;  */
            /*  q->fc -=fr;  */
            /* Atencao: teste */
            fr=fabs(fr);
            if(fr>(q1->fb)){ q1->fb=fr;}
            if(fr>(q->fc)) { q->fc=fr;}
      }
      else if(dir=='c'){
             /* q1->fc+=fr; */
             /* q->fb -=fr; */
             /* Atencao: teste */
             fr=fabs(fr);
             if(fr>(q1->fc)){ q1->fc=fr;}
             if(fr>(q->fb)) { q->fb=fr;}
      
      }
      else if(dir=='d'){
             /* q1->fd+=fr; */
             /* q->fe -=fr; */
             /* Atencao: teste */
             fr=fabs(fr);
             if(fr>(q1->fd)){ q1->fd=fr;}
             if(fr>(q->fe)) { q->fe=fr;}
      }
      else if(dir=='e'){
             /* q1->fe+=fr; */
             /* q->fd -=fr; */
             /* Atencao: teste */
             fr=fabs(fr);
             if(fr>(q1->fe)){ q1->fe=fr;}
             if(fr>(q->fd)) { q->fd=fr;}
      }

    }
  }
}

/***********************************************************/
/* Coloca coeficiente da matriz principal do sistema Ax=b  */
/* numa posicao da lista encadeada associada ao quadr. q1. */
/* Esta lista contem todos os elementos nao-nulos da linha */
/* da matriz correspondente ao quadradinho q1.             */
/* Se a aresta for fronteira, devemos atualizar o vetor b. */
/***********************************************************/
void coefquad_laplace(quad q, quad q1, quad qua, char dir)
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


  if((q->tipo=='a')||(q->ativo==0)){
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

    /* Use as funcoes fx0,fx1,f0y,f1y do valor de contorno. */ 
    if(dir=='b'){
      fr=(fronteira(q1->coord[0],q1->coord[1]-meiolado))*2.0/(float)fri;
      /* fr=(fx0(q1->coord[0]))*2.0/(float)fri;*/
      q1->cb+=fr;
    }
    else if(dir=='c') {
      fr=(fronteira(q1->coord[0],q1->coord[1]+meiolado))*2.0/(float)fri;
      /* fr=(fx1(q1->coord[0]))*2.0/(float)fri; */
      q1->cb+=fr;
    }
    else if(dir=='d') {
      fr=(fronteira(q1->coord[0]+meiolado,q1->coord[1]))*2.0/(float)fri;
      /*  fr=(f1y(q1->coord[1]))*2.0/(float)fri;*/
      q1->cb+=fr;
    }
    else if(dir=='e') {
      fr=(fronteira(q1->coord[0]-meiolado,q1->coord[1]))*2.0/(float)fri;
      /* fr=(f0y(q1->coord[1]))*2.0/(float)fri; */
      q1->cb+=fr;
    }
    /* Adiciona ao vetor b o valor devido `a fronteira. */
    q1->elem->valor += 2.0/(float)fri;
    /* Marque a diagonal de q1.                                 */
    /* Adiciona o valor -fr na coluna q1->np da lista q1->elem. */
    /* Este e' o elemento diagonal.                             */
  }
  else  if((qua->tipo=='q')&&(qua->ativo==1) ){
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
         i=(q->nivel) - (q1->nivel);   /* Muita  ATENCAO */
        /*i=(q1->nivel) - (q->nivel);*/  /* Muita ATENCAO */
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

        q1->elem->valor += fr;
        /* Marque a diagonal de q1. */
        q->elem->valor  += fr;
        /* Marque a diagonal de q.  */

        

        atual=q1->elem;
        col=q->np;
        /* Procura se existe elemento com coluna col na lista */
        do{  
          novo=atual;
          atual=atual->next;
        }while((atual != null) && (atual->coluna != col));
        if(atual != null){  
          /* Caso ja exista, adicione val ao valor existente */
          atual->valor-=fr;
        }
        else{ /* Senao, crie novo elemento com valor val */ 
          novo->next=(mlin)malloc(sizeof(pmlin));
          atual=novo->next;
          atual->coluna=col;
          atual->valor=-fr;
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
          atual->valor-=fr;
        }
        else{ /* Senao, crie novo elemento com valor val */ 
          novo->next=(mlin)malloc(sizeof(pmlin));
          atual=novo->next;
          atual->coluna=col;
          atual->valor=-fr;
          atual->next=null;
          atual->q=q1;
        }


        /* Marque abaixo e acima da diagonal. */
        /* Marque que ja foi visitado. */
   /* } */
      /* Adiciona o valor fr na coluna q->np da lista q1->elem   */
      /* Adiciona o valor fr na coluna q1->np da lista q->elem   */
      /* Se o elemento ja existir, adicione o valor `a posicao   */
      /* ja existente na lista. Senao, crie novo item na lista.  */ 

      /* Adiciona o valor -fr na coluna q1->np da lista q1->elem */
      /* Adiciona o valor -fr na coluna q->np da lista q->elem   */
      /* Estes sao os elementos diagonais.                       */
    }
  }
}


/* ******************************************************* */
/*FUNCAO: Calcula  difquadx para cada quadradinho          */
/* ******************************************************* */
 void predifquadx_laplace()
{  
 quad q1,q,qua;
 char dir;

 q1=gne0;  /* Percorre a curva de Peano */
 do{ 
  q1->difer=0.0;
  q1->fb=0.0;    q1->fc=0.0;    q1->fd=0.0;     q1->fe=0.0;
  q1=q1->next;
}while(q1!=null);

 q1=gne0;
do{
  if(q1->ativo==1){
   q=q1; qua=q; qua=qua->b; dir='b'; difquadx_laplace(q,q1,qua,dir);
   q=q1; qua=q; qua=qua->c; dir='c'; difquadx_laplace(q,q1,qua,dir);
   q=q1; qua=q; qua=qua->d; dir='d'; difquadx_laplace(q,q1,qua,dir);
   q=q1; qua=q; qua=qua->e; dir='e'; difquadx_laplace(q,q1,qua,dir);
  }
  
   q1=q1->next;
 }while(q1!=null);
     
}
/* ******************************************************* */
/********************************************************* */
/*Funcao difquadx :Calc. a diferenca do valor obtido q1->cx*/
/*de cada quad. q1 com os vizinhos e coloca o resultado em */
/* q1->fb, q1->fc,q1->fd,q1->fe (gradiente)                */
/* ******************************************************* */
void difquadx_laplace( quad q, quad q1, quad qua, char dir) 
{
  double fr,fr0,meiolado,meioladoq,menormeiolado,deltadistancia;
  int fri,i,nivel,aux;

  if(qua->nivel > q->nivel)
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->c;
    }
  else
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->b;
    }
  q=qua;

  /*calculo de meiolado */
    aux=2;
    nivel=q1->nivel;
    for(i=0;i<nivel;i++){
       aux=2*aux;
    }
   meiolado=1.0/(float)aux;

  if((q->tipo=='a') || (q->ativo==0) ){
    if(dir=='b')      {
    fr=(((q1->cx)-fronteira(q1->coord[0],q1->coord[1]-meiolado)))/meiolado;
     fr=fabs(fr);
     if(q1->fb<fr) q1->fb=fr;
    }
    else if(dir=='c') {
    fr=(((q1->cx)-fronteira(q1->coord[0],q1->coord[1]+meiolado)))/meiolado;
     fr=fabs(fr);
     if(q1->fc<fr) q1->fc=fr;
     
    }
    else if(dir=='d') {
    fr=(((q1->cx)-fronteira(q1->coord[0]+meiolado,q1->coord[1])))/meiolado;
    fr=fabs(fr);
    if(q1->fd<fr) q1->fd=fr;
     
    }
    else if(dir=='e') {
    fr=(((q1->cx)-fronteira(q1->coord[0]-meiolado,q1->coord[1])))/meiolado;
    fr=fabs(fr);
    if(q1->fe<fr) q1->fe=fr;

    }
  }

  else  if((qua->tipo=='q')&&(qua->ativo==1)){

      /* Calculo de meiolado de q  */
       aux=2;
       nivel=q->nivel;
       for(i=0;i<nivel;i++){
           aux=2*aux;
       }
       meioladoq=1.0/(float)aux;
       menormeiolado=meiolado;
       if (meioladoq<menormeiolado) menormeiolado=meioladoq;
       deltadistancia=2*menormeiolado;
       

       fr=((q1->cx)-(q->cx))/deltadistancia;
       fr=fabs(fr);

     if(dir=='b')      {
      if(q1->fb<fr) q1->fb=fr;
      if(q->fb<fr)   q->fb=fr;
     }
    else if(dir=='c') {
      if(q1->fc<fr) q1->fc=fr;
      if(q->fc<fr)   q->fc=fr;
     
    }
    else if(dir=='d') {
    if(q1->fd<fr) q1->fd=fr;
    if(q->fd<fr)   q->fd=fr;
     
    }
    else if(dir=='e') {
    if(q1->fe<fr) q1->fe=fr;
    if(q->fe<fr)   q->fe=fr;

    }
    }
}


/* ******************************************************* */
/* ******************************************************* */
/* FUNCAO:preenche fa,fb,fc,fd chamando a funcao difquad   */
/* ******************************************************* */
 void prediferx_laplace()
{
quad q1,q,qua;
char dir;
 
  q1=gne0;
  do{
     q1->difer=0; q1->fb=0; q1->fc=0; q1->fd=0; q1->fe=0;
     q1=q1->next;
  }while(q1!=null);
  
  q1=gne0;
  do{
     if(q1->ativo==1){
      q=q1; qua=q; qua=qua->b; dir='b'; diferx_laplace(q,q1,qua,dir);
      q=q1; qua=q; qua=qua->c; dir='c'; diferx_laplace(q,q1,qua,dir);
      q=q1; qua=q; qua=qua->d; dir='d'; diferx_laplace(q,q1,qua,dir);
      q=q1; qua=q; qua=qua->e; dir='e'; diferx_laplace(q,q1,qua,dir);
    } 
     q1=q1->next;
   }while(q1!=null);
   
}


/* ******************************************************* */
/*FUNCAO:Inicial. o vetor x por interp. linear da fronteira*/
/* ******************************************************* */
void init_gradconj()
{
  quad q1;
     
  q1=gne0;  /* Percorre a curva de Peano */

  do{
     if(q1->ativo==1){
       q1->cx =10.0;
       /*( ( q1->coord[1]*fx1(q1->coord[0]) +
               (1.0-q1->coord[1])*fx0(q1->coord[0]) +
               q1->coord[0]*f1y(q1->coord[1]) +
               (1.0-q1->coord[0])*f0y(q1->coord[1]) )/2.0); 
       
      /* q1->cx=fronteira(q1->coord[0],q1->coord[1]);*/
      }
     q1=q1->next;
  }while(q1!=null);
 
}

/* ******************************************************* */
/* Metodo do gradiente conjugado                           */
/* ******************************************************* */
void gradconj()
{ 
  int nit,maxnit,iteracao;
  quad q1,q,qua;
  char dir;    
  mlin atual,novo;
  double gama,lambda,omega,omega1;
  double norma,maxnorma,rms,erro,maxerro,tin;
  
  /* **************************************************** */
  /*Calculo inicial do h e do g, g inicial= b-A*xinicial  */
  /* **************************************************** */
  /* Calcula o vetor z=A*x e calcula  h=b-z   */
  tin=0.0;
  q1=gne0;  /* Percorre a curva de Peano */
  do{
   if(q1->ativo==1){
    q1->cz=0.0; /* Zera np-esima coordenada do vetor z */
    atual=q1->elem;
    do{ /*Produto interno da np-esima linha de A pelo vetorx*/
       q1->cz += (atual->valor) * (atual->q->cx);
       atual=atual->next;
    }while(atual != null);
    q1->ch = q1->cb - q1->cz;
    q1->cg = q1->ch;
    if(fabs(q1->ch)>tin)tin=fabs(q1->ch);
   }
   q1=q1->next;
  }while(q1!=null);
  
    
  /* ***************************************************** */
  /* Calculo do omega inicial                              */
  /* ***************************************************** */
  q1=gne0;  /* Percorre a curva de Peano */
  omega1=0.0; /* Calcula omega1=g*g  */
  do {
     if(q1->ativo==1){
     omega1 += q1->cg * q1->cg;
     }
     q1=q1->next;
  }while(q1!=null);
  omega=omega1;
  /* *************************************************** */
  maxnorma=1e-9; /*Precisao atingida pelas iteracoes norma do
                    maximo */
  maxnit=npcont; /* Numero maximo de iteracoes */
  nit=0; /* Numero de iteracoes */
 /* ***************************************************** */
 /* Inicio das iteracoes do metodo do gradiente conjugado */
 /* ***************************************************** */
 if(tin>=maxnorma) do{
   /* Calcula o vetor z=A*h  */
   q1=gne0;  /* Percorre a curva de Peano */

   do{
     if(q1->ativo==1){
       q1->cz=0.0; /* Zera np-esima coordenada do vetor z */
       atual=q1->elem;
       do{ /*Produto interno da np-esima linha de A por vetorb*/
          q1->cz += (atual->valor) * (atual->q->ch);
          atual=atual->next;
       }while(atual != null);
     }
     q1=q1->next;
   }while(q1!=null);
    
   /* ************************** */
   /* Calcula lambda=omega/(h*z) */
   /* ************************** */
   lambda=0.0;
   q1=gne0;  /* Percorre a curva de Peano */
   do{ 
     if(q1->ativo==1){
        lambda += q1->ch * q1->cz;
     }
     q1=q1->next;
   }while(q1!=null);
   lambda=omega/lambda;
   /* ********************************* */
   /* Calcula vetor g1=g-lambda*z       */
   /* ********************************* */
   omega1=0.0;
   q1=gne0;  /* Percorre a curva de Peano */
   do{
      if(q1->ativo==1){
         q1->cg1 = q1->cg - lambda * q1->cz;
         omega1 += q1->cg1 * q1->cg1;
      }
      q1=q1->next;
   }while(q1!=null);
   gama=omega1/omega;
   /* *************************************** */
   /* Calcula vetor h1 = g1 + gama * h        */
   /* *************************************** */
   norma=0.0;
   rms=0.0;
   maxerro=0.0;
   q1=gne0;  /* Percorre a curva de Peano */
   do{
     if(q1->ativo==1){
       q1->ch1 = q1->cg1 + gama * q1->ch;
       q1->cx += lambda * q1->ch; /*Calcula o vetor solucao x*/
       if(fabs(q1->ch - q1->ch1) > norma)
         norma = fabs(q1->ch - q1->ch1);
       q1->cg=q1->cg1; /* Atualiza g */
       q1->ch=q1->ch1; /* Atualiza h */
       erro= fabs(q1->cx - q1->f);
       rms+=erro*erro;
       if(erro > maxerro)
        maxerro = erro;
     }
     q1=q1->next;
   }while(q1!=null);
   omega=omega1; /* Atualiza omega */
   nit++;
 }while((norma>=maxnorma) && (nit<maxnit) );  
 /* *******************************************  */
 /* Termino das iteracoes do gradiente conjugado */
 /* *******************************************  */
 /* ***************************************  */
 /* Faz free em todos os membros de  ->elem  */
 /* ***************************************  */
 
 q1=gne0; 
 do{
    if(q1->ativo==1){  // ATENCAO PRECISA?
      atual=q1->elem;
      while(atual != null){ // Varre a lista de ->elem  
        novo=atual->next;
        free(atual);
        atual=novo;
      }
    }
    q1=q1->next;
 }while(q1!=null);
 
}

/* ******************************************************* */
/* FUNCAO:Inicial. lista de colunas nao nulas em cada quad */
/* ******************************************************* */
void init_col_laplace()
{  
    quad q1;

    q1=gne0;
    npcont=0;
    do{
      q1->np=npcont;
      npcont++;
      q1->cb=0.0;
      q1->elem=(mlin)malloc(sizeof(pmlin));
      q1->elem->coluna=q1->np;
      q1->elem->valor=0.0;
      q1->elem->next=null;
      q1->elem->q=q1; /* diagonal */
      q1=q1->next;
    }while(q1!=null);


}


/* ****************************************************** */
/* FUNCAO: Coloca os coeficientes na matriz virtual       */
/* ****************************************************** */

void monta_matriz_laplace()
{
 quad q1,qua,q;
 char dir;

 q1=gne0;
 do{
 q=q1; qua=q; qua=qua->b; dir='b';
 coefquad_laplace(q,q1,qua,dir);
 q=q1; qua=q; qua=qua->c; dir='c'; 
 coefquad_laplace(q,q1,qua,dir);
 q=q1; qua=q; qua=qua->d; dir='d';
 coefquad_laplace(q,q1,qua,dir);
 q=q1; qua=q; qua=qua->e; dir='e';
 coefquad_laplace(q,q1,qua,dir);
 q1=q1->next;
}while(q1!=null);

}

/* ******************************************************* */
/* FUNCAO: Monta matriz dos coef. e coloca as entradas     */
/*        desta  matriz em um arquivo                      */
/* ******************************************************* */
int resolve_laplace()
{
int iteracao;
quad q1;

double refinementTime;

/*Inicializa o vetor x por interp.lin. da fronteira*/
init_gradconj(); 
iteracao=0;

/* Comeca o grande loop de refinamento;                 */
/* em cada etapa faremos um metodo de grad.conj.        */
do{

  init_col_laplace(); /*Inicial. lista de colunas nao nulas*/
  monta_matriz_laplace();  /*  matriz virtual A de Ax-b    */
  
    ofstream outputFile2( "outputMatrix.txt", ios::trunc );
    
    outputFile2 << "Matrix:\n\n";
    q1=gne0;
    mlin atual;   
    int i = 1;
    do{
        outputFile2 << "Row " << i << ": ";
        atual=q1->elem;
        do{
              outputFile2 << atual->valor << "(" << atual->coluna << ")"
                 << " ";
              atual=atual->next;
        }while(atual != null);
        outputFile2 << "\n";
        i++;    
        q1=q1->next;
    }while(q1!=null);
    outputFile2 << "\nEnd of Matrix.\n\n";
    outputFile2 << 
    "________________________________________________________________________\n\n";
  
    
  gradconj();      /* metodo gradiente conjugado   */
  aindaref=0;    /*flag indica se precisou de mais refinam */
  Dx_ref_laplace();       /* chama a funcao de refinamento */
  //D_desref_laplace(); /* chama a funcao ref. e desref. */

  npcont=0;
  q1=gne0;  /* Percorre a curva de Peano */
  do{
      q1=q1->next;
      npcont++;
  }while(q1!=null);
  iteracao++;
  }while(aindaref); /* Fim do grande loop;                   */
                  /* repete enquanto for feito refinamento */
 
 /*
 ofstream outputFile( "output.txt", ios::trunc );
    
    //int print()
    q1=gne0;
    do{
        outputFile << "Cell " << q1->np << "\n\n";        
        outputFile << "Valor Calculado = " << q1->cx << "\n";
        double exactValue = fxy( q1->coord[0], q1->coord[1] );
        outputFile << "Valor Exato = " << exactValue << "\n";
        double relativeError = fabs( fabs(exactValue) - fabs(q1->cx) );
        outputFile << "Erro Relativo = " << relativeError << "%\n";
        //outputFile << "Total Flux = " << q1->difer << "\n";
        outputFile << 
        "________________________________________________________________________\n";
        q1=q1->next;
    }while(q1!=null);
    outputFile << 
        "________________________________________________________________________\n"; 
    outputFile << 
        "________________________________________________________________________\n";
        
        */
    cout << npcont;
    
    return 0;
 
}   

/* ******************************************************* */
/* FUNCAO: Chama a funcao que refina a malha de acordo     */
/*  com a cota de refinamento                              */
/***********************************************************/
void den_play_laplace()
{
   if (flagpeano == 1) 
      linpeano(7); // Desenha a curva de Peano em cima da anterior, i.e., apaga
              
   resolve_laplace();
   
   if (flagpeano == 1) 
      linpeano(1); // Desenha a nova curva de Peano

 }  
 
/* ******************************************************* */
/* FUNCAO: Criterio para decidir o desrefinamento          */
/* ******************************************************* */
void condes_laplace(quad qref)
{ 
  int aux1,aux2,aux3,aux4;
  double dif1,dif2,dif3,dif4;
  quad  q1,q2,q3,q4;
 
  if(flagpeano==1){
     linpeano(7);
  } 

  /*  Percorrendo a curva de peano,*/
  /*  decidir se estao no mesmo cacho  */

  aux1= (qref->ncon)/10;
  aux2= (qref->next->ncon)/10;
  aux3= (qref->next->next->ncon)/10;
  aux4= (qref->next->next->next->ncon)/10;


   q1=qref;
   q2=q1->next;
   q3=q2->next;
   q4=q3->next; 
 

   /* fluxos */
   dif1=q1->difer;
   dif2=q2->difer;
   dif3=q3->difer;
   dif4=q4->difer; 
  
   
/* Decide se o cacho quer ser desrefinado */
if ((aux1==aux2) && (aux1==aux3) && (aux1==aux4)){
    if((dif1<cotadeslap) && (dif2<cotadeslap) &&
      (dif3<cotadeslap) && (dif4<cotadeslap)) 
   { 
     desrefina(q1);
     aindaref=1; 
   }
}         

}  /* fim da funcao condes  */

/* ******************************************************  */
/* FUNCAO: Decide se refina a malha                        */
/*         pecorrendo toda a curva de peano                */
/* ******************************************************* */
double Dx_ref_laplace ()
{
 quad q1,qua,q2;
 int frefina,flag_prox;
 double diferenca;
 double maior;
 double time = 0.0;


     //Mede tempo gasto no refinamento
   CPerfTimer PerftempRef;
   PerftempRef.Start();
   
   prediferx_laplace();   /* fluxos    */
   
   q1=gne0;
   frefina=0;
            
   
   
     do{   
         
       flag_prox=0;
       if (q1->ativo==1){
          
         /* criterio dos fluxos */
         diferenca=(q1->difer);
         
         
             
         if( (q1->nivel < 6 ) &&  (diferenca >= cotalaplace)){  
           flag_prox=1;
           qua=q1;
           q2=q1->next;  
           
            
            refina(qua);
            
           
            
           aindaref=1;
           qua=q1;
           qua->b->proc=qua->proc;
           qua->b->e->proc=qua->proc;
           qua->e->proc=qua->proc;
           //drawE(qua->coord[0],qua->coord[1],0); 
           //drawE(qua->coord[0],qua->coord[1],qua->proc); 
           qua=qua->e;
           //drawE(qua->coord[0],qua->coord[1],qua->proc); 
           qua=qua->b;
           //drawE(qua->coord[0],qua->coord[1],qua->proc); 
           qua=qua->d;
           //drawE(qua->coord[0],qua->coord[1],qua->proc);  
           qua=qua->c;
           frefina=1;
           q1=q2;
         }
      }
      if(flag_prox==0){q1=q1->next;} /* por causa de q1=q2*/
    }while(q1!=null); 
    
    
    PerftempRef.Stop();
    time = PerftempRef.Elapsedms();
           
    
    return time;


}

/* ******************************************************* */
/* FUNCAO : Decide se desrefina a malha                    */
/*          pecorrendo toda a curva de peano               */
/* ******************************************************* */
double D_desref_laplace()
{
quad qauxa;
quad qteste;
int  nivel;
double time;

if(flagpeano==1) linpeano(7); 
aindaref = 0;

CPerfTimer tempDesRef;
tempDesRef.Start();
    
prediferx_laplace();

    
qauxa=gne0;
do{
   nivel=qauxa->nivel;
   qteste = qauxa->next;
   if(qauxa->nivel>2) 
   {
    if (qauxa->next!=null)
    {
     if ( qauxa->next->next!=null)
     {
      if (qauxa->next->next->next!=null)
      {
       if((nivel==qauxa->next->nivel)&&
          (nivel==qauxa->next->next->nivel)&&
          (nivel==qauxa->next->next->next->nivel))
          {
            if(qauxa->next->next->next->next!=null)
                 qteste=qauxa->next->next->next->next;
                 
  
              /* fluxos */
            condes_laplace(qauxa);
          }
     }
    }
   }
  }
  qauxa = qteste;
 
} while(qauxa!=null);

tempDesRef.Stop();

time = tempDesRef.Elapsedms();

return time;

}
    
/* ******************************************************* */
/* FIM DO PROGRAMA : LAPLACE.C
/* ****************************************************** */


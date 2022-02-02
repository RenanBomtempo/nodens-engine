 
/*******************************************************/
/* PROGRAMA: onda.cpp                                    */
/* AUTORA  : DENISE BURGARELLI DUCZMAL                 */
/* DATA    : 29/06/97                                  */
/* cotaref ideal : 40.0 na tela                        */
/* cotadesref    : 30.0 na tela                        */
/*******************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <gl/glu.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "funcoes.h"
#include "quad.h" 
#include "PerfTimer.h"
#include <iostream>


#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;
    
#include <fstream>
    using std::ofstream;


#define null 0
/* ************** variaveis externas ***************** */
extern int flagglobal;
extern int flagonda_inicio;
extern int flagpeano;
extern double cotaref;     
extern double cotades;
extern quad gne0;

int npcont; /* numero de quadradinhos */          

/*********** variaveis globais neste programa ******** */

int fdesrefina=0;
double vel=2.0;
double deltat= 0.007;
double tfinal=0.2;//0.5;//1.0
double xpe,ype; /* coordenadas do pe da caracteristica */
quad qcam;
double refinementTime = 0.0, 
       derefinementTime = 0.0,
       computationalTime = 0.0,
       characteristicsTime = 0.0;
       
double waveExactSolution( double, double, double );
       
double waveExactSolution( double x, double y, double t )
{
    double exactValue = 0.0;
    double f,g;
    
    f = 1-16*( (x- t*vel - 0.25 )* (x - t*vel - 0.25) 
             + (y- t*vel - 0.25 )* (y - t*vel - 0.25) );
             
    /*
    g = 1-16*( (x+t*vel - 0.75 )* (x+t*vel - 0.75) 
             + (y+t*vel - 0.75 )* (y+t*vel - 0.75) );
    
    if( f > 0 && g > 0 )
    {
        exactValue = f + g;
        return exactValue;
    }
    else if( f > 0 )
    {
        exactValue = f;
        return exactValue;
    }
    else if( g > 0 )
    {
        exactValue = g;
        return exactValue;
    } 
    */
    if( f > 0 )
    {
        exactValue = f;
        return exactValue;
    }
    return exactValue;
}

/* *****************************************************/
/* Calcula a diferenca do valor q1->usoma   de cada    */
/* quad. q1 com os vizinhos e coloca o resultado       */
/* em q1->fb, q1->fc,q1->fd, q1->fe (gradiente)        */
/* *****************************************************/
void difquad_onda(quad q, quad q1, quad qua, char dir)
{
  double fr,frp,frn;
  int fri,i,nivel,aux;
  double meiolado, meioladoq, deltadistancia;
  double menormeiolado;

 
  if(qua->nivel > q->nivel)
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->c;
    }
  else
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->b;
    }
  q=qua;

  aux=2;
  nivel=q1->nivel;
  for(i=0;i<nivel;i++){
      aux=2*aux;
  }
  meiolado=1.0/(float)aux;

  if(q->tipo=='a'){
    deltadistancia = meiolado;
    fr=fabs( ((q1->usoma) - 0.0)/deltadistancia ); 
    q1->difer+=fabs(fr);
     

    if(dir=='b')      {
       if( fr>(q1->fb)){ q1->fb=fr;}  /* Era so esta linha antes :q1->fb=fr;*/
       if(frp>(q1->fbp)){q1->fbp=frp;} /* Mudou agora */
       if(frn>(q1->fbn)){q1->fbn=frn;} /* Mudou agora */
    }
    else if(dir=='c') {
       if(fr>(q1->fc)){q1->fc=fr;}  /* Era so esta linha antes q1->fc=fr;*/
       if(frp>(q1->fcp)){q1->fcp=frp;} /* Mudou agora */
       if(frn>(q1->fcn)){q1->fcn=frn;} /* Mudou agora */

    }
    else if(dir=='d') {
     if(fr>(q1->fd)){q1->fd=fr;}   /* Era so esa linha antes : q1->fd=fr;*/
     if(frp>(q1->fdp)){q1->fdp=frp;} /* Mudou agora */
     if(frn>(q1->fdn)){q1->fdn=frn;} /* Mudou agora */
    }
    else if(dir=='e') {
       if(fr>(q1->fe)){q1->fe=fr;}  /* Era so esta linha antes:q1->fe=fr; */
       if(frp>(q1->fep)){q1->fep=frp;} /* Mudou agora */
       if(frn>(q1->fen)){q1->fen=frn;} /* Mudou agora */
    }
  }
  else  if(q->tipo=='q'){
      menormeiolado=meiolado; 
      aux=2;
      nivel=q->nivel;
      for(i=0;i<nivel;i++){
           aux=2*aux;
           }
      meioladoq=1.0/(float)aux;

      if(meioladoq<menormeiolado) menormeiolado=meioladoq;
      deltadistancia=2*menormeiolado;
 
     fr=fabs( ((q1->usoma)-(q->usoma))/ (deltadistancia) );
     q1->difer+=fabs(fr);
     q->difer+=fabs(fr);
     
      if(dir=='b'){
        if(fr>(q1->fb)) {q1->fb=fr;}
        if(fr>(q->fc )) {q->fc=fr; } 
      }
      else if(dir=='c'){
        if(fr>(q1->fc)) {q1->fc=fr;}
        if(fr>(q->fb))  {q->fb=fr;} 
      }
      else if(dir=='d'){
        if(fr>(q1->fd)) {q1->fd=fr;}
        if(fr>(q->fe))  {q->fe=fr;} 
        
      }
      else if(dir=='e'){
        if(fr>(q1->fe)) {q1->fe=fr;}
        if(fr>(q->fd))  {q->fd=fr;} 
        
      }


  }
}

/* *****************************************************/
/* Calcula a diferenca do valor q1->usoma   de cada    */
/* quad. q1 com os vizinhos e coloca o resultado       */
/* em q1->difer  (fluxo)                               */
/* *****************************************************/
void difer_onda(quad q, quad q1, quad qua, char dir)
{
  double fr,frp,frn;
  int fri,i,nivel,aux;
  double meiolado, meiolado1, deltadistancia;
  double menormeiolado;

 
  if(qua->nivel > q->nivel)
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->c;
    }
  else
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->b;
    }
  q=qua;

  if(q->tipo=='a'){
      fr=((q1->usoma)-0.0) *2.0;
      q1->difer+=fabs(fr);
  }
          
  else  if(q->tipo=='q')
        if((q1->nivel > q->nivel)||
          ((q1->nivel == q->nivel) &&
          ((q1->coord[0] > q->coord[0])||
          ((q1->coord[0] == q->coord[0])&&
           (q1->coord[1] > q->coord[1]))))  )
    {
       fri=1;
       i=(q1->nivel)-(q->nivel);
       while(i>0){
          fri*=2;
          i--;
       }
       fr=((q1->usoma)-(q->usoma))/(float)fri;
       q1->difer+=fabs(fr);
       q->difer+=fabs(fr);

   }
}

/*******************************************************/
void predifer_onda()
{
  quad q1,q,qua;
  char dir;

  q1=gne0;
  do{
      q1->difer=0.0;
      q1=q1->next;
   } while(q1!=null);

 q1=gne0;  
do{ 
 q=q1; qua=q;qua=qua->b; dir='b'; difer_onda(q,q1,qua,dir);
 q=q1; qua=q;qua=qua->c; dir='c'; difer_onda(q,q1,qua,dir);
 q=q1; qua=q;qua=qua->d; dir='d'; difer_onda(q,q1,qua,dir);
 q=q1; qua=q;qua=qua->e; dir='e'; difer_onda(q,q1,qua,dir);
 q1=q1->next;
}while(q1!=null);

}

/* ******************************************************/
/* ******************************************************/
void difquad_pe(quad q, quad q1, quad qua, char dir)
{
  double fr,frp,frn;
  int fri,i,nivel;
  double meiolado, meiolado1,menormeiolado;
  double deltadistancia;
  int aux;

  if(qua->nivel > q->nivel)
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->c;
    }
  else
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->b;
    }
  q=qua;


  if(q->tipo=='a'){

    aux=2;
    nivel=q1->nivel;
    for(i=0;i<nivel;i++){
        aux=2*aux;
    }
    meiolado=1.0/(float)aux;
    deltadistancia = meiolado; /* meiolado */
    frp= (( (q1->uantp) - 0.0)/deltadistancia );
    frn= (( (q1->uantn) - 0.0)/deltadistancia );
 

    if(dir=='b')      {
      q1->fbp+=frp;
      q1->fbn+=frn;
    }
    else if(dir=='c') {
      q1->fcp+=frp;
      q1->fcp+=frn;
    }
    else if(dir=='d') {
      q1->fdp+=frp;
      q1->fdn+=frn;
    }
    else if(dir=='e') {
      q1->fep+=frp;
      q1->fen+=frn;
    }
  }

  else  if(q->tipo=='q'){
    if((q1->nivel > q->nivel)||
      ((q1->nivel == q->nivel) &&
      ((q1->coord[0] > q->coord[0])||
      ((q1->coord[0] == q->coord[0])&&
      (q1->coord[1] > q->coord[1]))))  )
       /* ordem do dicionario */
    {

      aux=2;
      nivel=q->nivel;
      for(i=0;i<nivel;i++){
           aux=2*aux;
      }
      meiolado=1.0/(float)aux;
      menormeiolado=meiolado;

      aux=2;
      nivel=q1->nivel;
      for(i=0;i<nivel;i++){
          aux=2*aux;
      }
      meiolado1=1.0/(float)aux;

      if(meiolado1<menormeiolado) menormeiolado=meiolado1;
      deltadistancia=2*menormeiolado;

/*    fri=1;
      i=(q1->nivel)-(q->nivel);
      while(i>0){
      fri*=2;
      i--;
      }  
      frp=  ((q1->uantp)-(q->uantp))/ (float)fri;
      frn=  ((q1->uantn)-(q->uantn))/ (float)fri; 
*/ 
     frp= ( ((q1->uantp)-(q->uantp))/ (deltadistancia) );
     frn= ( ((q1->uantn)-(q->uantn))/ (deltadistancia) ); 

      if(dir=='b'){
        q1->fbp+=frp;
        q->fcp-=frp;
        q1->fbn+=frn;
        q->fcn-=frn;
      }
      else if(dir=='c'){
        q1->fcp+=frp;
        q->fbp -=frp;
        q1->fcn+=frn;
        q->fbn -=frn;
      }
      else if(dir=='d'){
        q1->fdp+=frp;
        q->fep -=frp;
        q1->fdn+=frn;
        q->fen -=frn;
      }
      else if(dir=='e'){
        q1->fep+=frp;
        q->fdp -=frp;
        q1->fen+=frn;
        q->fdn -=frn;
      }

    }
 }
}
/* *************************************************** */
/* FUNCAO:preenche fape,fbpe,fcpe,fdpe                 */
/* chamando  difquad_pe (gradiente )                   */
/* *************************************************** */
 void predifquad_pe()
{
quad q1,q2,q,qua;
char dir;


q1=gne0;  /* Percorre a curva de Peano */
do{ 
 q1->fbp=0.0;  q1->fcp=0.0;   q1->fdp=0.0;  q1->fep=0.0;
 q1->fbn=0.0;  q1->fcn=0.0;   q1->fdn=0.0;  q1->fen=0.0;
 q1=q1->next;
}while(q1!=null);


q1=gne0;  /* Percorre a curva de Peano */
do{ 
 q=q1; qua=q;qua=qua->b; dir='b'; difquad_pe(q,q1,qua,dir);
 q=q1; qua=q;qua=qua->c; dir='c'; difquad_pe(q,q1,qua,dir);
 q=q1; qua=q;qua=qua->d; dir='d'; difquad_pe(q,q1,qua,dir);
 q=q1; qua=q;qua=qua->e; dir='e'; difquad_pe(q,q1,qua,dir);
 q1=q1->next;
}while(q1!=null);
     
}

/* *************************************************** */
/* FUNCAO:preenche fa,fb,fc,fd chamando  difquad       */
/* *************************************************** */
 void predifquad_onda()
{
quad q1,q,qua;
char dir;


q1=gne0;  /* Percorre a curva de Peano */
do{ 
 q1->fb=0.0;    q1->fc=0.0;    q1->fd=0.0;   q1->fe=0.0;
 q1->difer=0.0;
 q1=q1->next;
}while(q1!=null);

q1=gne0;  /* Percorre a curva de Peano */
do{ 
q=q1; qua=q; qua=qua->b; dir='b'; difquad_onda(q,q1,qua,dir);
q=q1; qua=q; qua=qua->c; dir='c'; difquad_onda(q,q1,qua,dir);
q=q1; qua=q; qua=qua->d; dir='d'; difquad_onda(q,q1,qua,dir);
q=q1; qua=q; qua=qua->e; dir='e'; difquad_onda(q,q1,qua,dir);
 q1=q1->next;
}while(q1!=null);
     
}
/* *************************************************** */
/* FUNCAO: Calcula o valor atual de uatualp ou uatualn */
/*         fazendo uma interpolacao com o valor do     */
/*         quad. onde esta o pe da caracteristica      */
/*         e usando os gradientes fbpe,fcpe,fdpe,fepe  */
/*         dependendo do quadrante onde se encontra o  */
/*         pe da caracteristica                        */
/* *************************************************** */
/*  void calcula_valor_atual(q1,qaux,tipo,meiolado)
quad q1;
quad qaux;
char tipo;
double meiolado;
{
     q1->uatualp=(qaux->uatualp)+
                 ( (ad0/meiolado)*qaux->

  
*/



       
/* *************************************************** */
/* FUNCAO: Monta as condicoes iniciais do problema     */
/* *************************************************** */
void onda_cond_inicial()
{
  double raio_pulso_inicial=0.125;
  double r_circulo;     
  quad q1;
  npcont=0;
    
  q1=gne0;
  do{
       q1->uatualp=1* ( 1-16*( 
       (q1->coord[0]-0.25)*(q1->coord[0]-0.25)+
       (q1->coord[1]-0.25)*(q1->coord[1]-0.25)) );
       if ( q1->uatualp < 0.0 ){ q1->uatualp = 0.0; }
       q1->uantp=q1->uatualp;

       q1->uatualn=1 * ( 1-16*(
       (q1->coord[0]-0.75)*(q1->coord[0]-0.75)+
       (q1->coord[1]-0.75)*(q1->coord[1]-0.75)) );
       if ( q1->uatualn < 0.0 ){ q1->uatualn = 0.0; } 
       q1->uantn=q1->uatualn;

       q1->usoma= (q1->uatualp) + (q1->uatualn) ;
 
       npcont++;
       q1=q1->next;
  }while(q1!=null);

}
/***************************************************** */
/* FUNCAO: Atualiza os dados iniciais no refinamento   */
/*         inicial                                     */
/* *************************************************** */
void atualiza_inicio_onda( quad qajuda)
 {
  double raio_pulso_inicial=0.125;
  double r_circulo;     
  quad q1;
    
   /* qajuda->uantp=qajuda->uatualp; */ /* Atencao teste */
   qajuda->uatualp=1 * (1-16*( 
   (qajuda->coord[0]-0.25)*(qajuda->coord[0]-0.25)+
   (qajuda->coord[1]-0.25)*(qajuda->coord[1]-0.25)));
   if ( qajuda->uatualp < 0.0 ){ qajuda->uatualp = 0.0;}
   qajuda->uantp=qajuda->uatualp; 
 
   /* qajuda->uantn=qajuda->uatualn; */ 
   qajuda->uatualn=1 * (1-16*(
   (qajuda->coord[0]-0.75)*(qajuda->coord[0]-0.75)+
   (qajuda->coord[1]-0.75)*(qajuda->coord[1]-0.75)));
   if ( qajuda->uatualn < 0.0 ){ qajuda->uatualn = 0.0;}
   qajuda->uantn=qajuda->uatualn; 
    
   qajuda->usoma= (qajuda->uatualp) + (qajuda->uatualn);
 
 }
/* *************************************************** */
/* FUNCAO: Procura o pe da caracteristica caminhando   */
/*         por quadradinhos e arvores                  */
/* *************************************************** */
void caminha_arvore(quad qaux, quad q1, quad qua, char tipo)
{
  if(qua->nivel > qaux->nivel)
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->c;
    }
  else
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->b;
    }
  qcam=qua;



   if(qcam->tipo=='a'){  /* modificado em 04ago97 */
    if (tipo=='p') {
        q1->uatualp=0.0;
    }
    else if (tipo=='n'){      
        q1->uatualn=0.0; 
    } 
  }


}
/* **************************************************  */
/* *************************************************** */
/* FUNCAO: Metodo das caracteristicas modificado       */
/*         Calcula as coordenadas do pe da             */
/*         caracteristica                              */
/* *************************************************** */
 void pe_carac(quad q1, char tipo)
 
 {
 /* tipo p : onda caminhando com velocidade positiva */
 /* tipo n : onda caminhando com velocidade negativa */
 /* xpe,ype: coordenadas do pe da caracteristica     */
   
 if(tipo == 'p'){ 
    xpe=q1->coord[0]-vel*deltat; 
    ype=q1->coord[1]-vel*deltat;
 }

 else if(tipo == 'n'){ 
    xpe=q1->coord[0]+vel*deltat; 
    ype=q1->coord[1]+vel*deltat;

 }

 /* (sugestao usar um vel1 e um vel2 ) */
       
 }

/* *************************************************** */
/* **************************************************  */
/* FUNCAO: Pecorre a malha procurando o quadradinho    */
/*         mais  proximo do pe da caracteristica       */
/* *************************************************** */
 void  calcula_por_carac()
 {
 quad q1,qaux,qua,q2;
 double d0,d1,ad0,ad1,meiolado,lado;
 double auxatual,auxanterior;
 int i,aux,j,nivel,cont_aux; 
 char tipo; /* identifica se a equac. da onda tem      */
            /*  veloci.positiva ou negativa            */

 q1=gne0;
 do
 { 
   tipo='p';
   for(j=0;j<2;j++)
   {
      pe_carac(q1,tipo);
       
      if((xpe<=0.0) || (ype<=0.0) ||
         (xpe>=1.0) || (ype>=1.0))
      {
           if (tipo=='p'){
               q1->uatualp=0.0;
           } 
           else if (tipo=='n'){
               q1->uatualn=0.0;
           }
      }
      else
      {
     
        qaux=q1;
         
        d0=(xpe - qaux->coord[0]); ad0=fabs(d0);
        d1=(ype - qaux->coord[1]); ad1=fabs(d1);

        aux=2;  
        nivel=qaux->nivel;
        for(i=0;i<nivel;i++) { aux=2*aux; }
        meiolado=1.0/(float)aux; 
          
        if((ad0>meiolado)||(ad1>meiolado)){
         do{
          q2=qaux; 
          if(ad0>=ad1)
          {
              if( (qaux->coord[0]) > xpe)
              { 
                 qaux=q2->e; 
                 if(qaux->tipo=='a'){
                    qaux=q2;
                    qua=q2->e;
                    caminha_arvore(qaux,q1,qua,tipo);
                    qaux=qcam;
                 }
              } 
              else if ((qaux->coord[0]) <= xpe) 
              {
                  qaux=q2->d;
                  if(qaux->tipo=='a'){
                     qaux=q2;
                     qua=q2->d;
                     caminha_arvore(qaux,q1,qua,tipo);
                     qaux=qcam;
                  }
             
              }
          }  /* fim do if (ad0 >=ad1)  */
          else if (ad1 >ad0)
          {
              if( (qaux->coord[1]) > ype)
              {
                   qaux=q2->b;
                   if(qaux->tipo=='a')
                   { 
                      qaux=q2; 
                      qua=q2->b; 
                      caminha_arvore(qaux,q1,qua,tipo);
                      qaux=qcam;
                   } 

              }
              else if ( (qaux->coord[1]) <= ype) {
                   qaux=q2->c;
                   if(qaux->tipo=='a'){ 
                      qaux=q2;
                      qua=q2->c; 
                      caminha_arvore(qaux,q1,qua,tipo);
                      qaux=qcam;
                   }
              }  
          }
        
          if(qaux->tipo=='q'){
             d0=(xpe - qaux->coord[0]); ad0=fabs(d0);
             d1=(ype - qaux->coord[1]); ad1=fabs(d1);
             aux=2;  
             nivel=qaux->nivel;
             for(i=0;i<nivel;i++) { aux=2*aux; }
             meiolado=1.0/(float)aux; 
          } 
          else if ( qaux->tipo=='a'){
             ad0=meiolado/2.0;
             ad1=meiolado/2.0;   /* TESTE */
          }
        
         } while((ad0 >meiolado) ||  (ad1>meiolado));

 /* }  */ /* antigo fim do if(ado >meiolado ou ad1 >meiolado ) */
 
         if(qaux->tipo=='q'){
            /* lado=2.0*meiolado; */

            if(tipo=='p'){
               q1->uatualp = qaux->uantp;   /*Isto era antes*/       
              /* calcula_valor_atual(q1,qaux); */
              /* if (d0<=0) {
                    if (d1<=0){
                       q1->uatualp= (qaux->uantp) +
                       ( (ad0/(lado) )*(qaux->fep) )+
                       ( (ad1/(lado) )*(qaux->fbp) );
                    }
                    else if( (d1>0) ){
                      q1->uatualp= (qaux->uantp) +
                      ( (ad0/(lado) )*(qaux->fep) )+
                      ( (ad1/(lado) )*(qaux->fcp) );
                    }

                }
            else if (d0>0){
                 if (d1>=0) {
                      q1->uatualp= (qaux->uantp) +
                      ( (ad0/(lado) )*(qaux->fdp) )+
                      ( (ad1/(lado) )*(qaux->fcp) );
                  }
          
                 else if(d1<0) {
                      q1->uatualp= qaux->uantp +
                      ( (ad0/(lado) )*(qaux->fdp) )+
                      ( (ad1/(lado) )*(qaux->fbp) );
                  }  
             } */    /* fim  */  

         }  
         else if(tipo=='n'){
              q1->uatualn = qaux->uantn;  /*Isto antes */ 
              /* calcula_valor_atual(q1,qaux); */
            
          /*  if(d0<=0) {
                if(d1<=0){
                   q1->uatualn= qaux->uantn +
                   ( (ad0/(lado) )*(qaux->fen) )+
                   ( (ad1/(lado) )*(qaux->fbn) );
                }
               else if(d1>0){
                   q1->uatualn= qaux->uantn +
                   ( (ad0/(lado) )*(qaux->fen) )+
                   ( (ad1/(lado) )*(qaux->fcn) );
                }
             }
            else if(d0>0) {
                  if(d1>=0){
                   q1->uatualn= qaux->uantn +
                   ( (ad0/(lado) )*(qaux->fdn) )+
                   ( (ad1/(lado) )*(qaux->fcn) );
                  }
                  else if(d1<0) {
                  q1->uatualn= qaux->uantn +
                   ( (ad0/lado)*(qaux->fdn) )+
                   ( (ad1/lado)*(qaux->fbn) );
                  }
            } */     /* fim */    
             
          } /* fim do tipo n */  
      }   /* fim do if (qaux->tipo->q) */
     }  /* coloquei o fim do if( (ad0>meiolado) || (ad1>meiolado) ) aqui */
    else 
      {   
            if (tipo == 'p') { 
            q1->uatualp=q1->uantp;
            }
      
            if (tipo == 'n') { 
            q1->uatualn=q1->uantn;
            }
       }  
                 
   } /* fim do else xpe>0.0 .... */
   
   tipo='n';   
   } /* for */     
q1->usoma=(q1->uatualp)+(q1->uatualn); 
q1=q1->next;
}while(q1!=null);   

}
/* *************************************************** */
/* Funcao que guarda o valor da onda                   */
/* *************************************************** */
void D_guarda_anterior()
{
      quad quaux;
      
      quaux=gne0;
      do{
          quaux->uantp=quaux->uatualp;
          quaux->uantn=quaux->uatualn;
          quaux= quaux->next;
       } while(quaux!=null);
}


/* *************************************************** */
/* FUNCAO: Resolve a equacao da onda                   */
/* *************************************************** */
void resolve_onda()
{
  double t;
  
  double refinementTime = 0.0,
         derefinementTime = 0.0,
         computationTime = 0.0,
         totalRefinementTime = 0.0,
         totalDerefinementTime = 0.0,
         totalComputationTime = 0.0;
    
    ofstream outputFile( "output.txt", ios::app ); //trunc
    outputFile << "\nEquação da Onda: u_t + c_1 u_x + c_2 u_y = 0" << "\n\n";
    outputFile << "\nSimulação 1 " << "\n\n";
    outputFile << "Condição inicial: u(x,y,0) = f(x,y) + g(x,y). " << endl;
    outputFile << "Tempo final: 5 s" << endl;
    outputFile << "Incremento de tempo: 0.05 s" << endl;
    outputFile << "Nível de refinamento máximo: 5" << endl;
    outputFile << "Cota de refinamento: 0.1" << endl;
    outputFile << "Cota de desrefinamento: 0.06" << endl;
    outputFile << "Tamanho mínimo da célula: 0.03125" << endl;
    outputFile << "________________________________________________________________________\n"  << endl; 


  flagonda_inicio=0;
  onda_cond_inicial();
  totalRefinementTime += D_ref_onda_inicial();
  curvas_de_nivel();
  
 
  D_guarda_anterior(); 
  flagonda_inicio = 1;
  
  totalRefinementTime += D_ref_onda();
  precalcviz();
  //D_desref_onda();
  
  
    /*
    int numberOfCellsGreaterThan100 = 0,
        numberOfCellsGreaterThan10 = 0,
        numberOfCellsGreaterThan5 = 0,
        numberOfCellsGreaterThan2 = 0;
    double exactValue, relativeError;
    */
  
  for( t = deltat; t < tfinal; t += deltat )   /* double t = 0.0*/ /* MODIFICAÇÔES 10/03/2005 */
  {   
      computationTime = 0.0;
      refinementTime = 0.0; 
      derefinementTime = 0.0;
      /*
      flush();
      clear();
      
      numberOfCellsGreaterThan100 = 0,
      numberOfCellsGreaterThan10 = 0,
      numberOfCellsGreaterThan5 = 0,
      numberOfCellsGreaterThan2 = 0;
      */
      
       
      
      CPerfTimer tempNum;
      tempNum.Start();
      
      calcula_por_carac();
      
      tempNum.Stop();     // Finaliza a medição do tempo
      computationTime = tempNum.Elapsedms();
      
      
          //ofstream outputFile( "output.txt", ios::app ); //trunc
          
          outputFile << "\n\nTIME: " << t << "\n\n";
               
          
          quad q1 = gne0;
          
          D_guarda_anterior(); 
      
          refinementTime = D_ref_onda();
          precalcviz();
          derefinementTime = D_desref_onda();
           
          curvas_de_nivel();
          D_guarda_anterior();
          /*
          do{
        
          double exactValue = waveExactSolution( q1->coord[0], q1->coord[1], t );
          double difference = q1->uatualp - exactValue;
          relativeError
          = 100 * fabs( ( fabs(difference) ) / exactValue );    
          if( relativeError > 2.0 )
          {
              ++numberOfCellsGreaterThan2;
              //drawColorDisk( q1->coord[0], q1->coord[1], 3 );
          }    
          if( relativeError > 5.0 )
          {
            ++numberOfCellsGreaterThan5;
            //drawColorDisk( q1->coord[0], q1->coord[1], 5 );
          }   
          if( relativeError > 10.0 )
          {
            ++numberOfCellsGreaterThan10;
            //drawColorDisk( q1->coord[0], q1->coord[1], 1 );
          }
            
          if( relativeError >= 100.0 )
          {
            ++numberOfCellsGreaterThan100;
            //drawColorDisk( q1->coord[0], q1->coord[1], 0 );
          }
          
          {
        
            outputFile << "Cell " << q1->np << "\n\n";
            outputFile << "Valor Calculado = " << q1->cx << "\n";
            outputFile << "Valor Exato = " << exactValue << "\n";
            outputFile << "Erro Relativo = " << relativeError << "%\n";
            outputFile << 
            "________________________________________________________________________\n" 
            << endl;
            
            
        q1=q1->next;
        }while(q1!=null);
        
    outputFile << "Número de células = " << npcont << "\n\n";
    
    outputFile << "Number of cells with error greater than 100% = " 
                << numberOfCellsGreaterThan100 << " - Percentage = "
                << (double)numberOfCellsGreaterThan100/npcont * 100 << "%" << endl;
    outputFile << "Number of cells with error greater than 10% = " 
                << numberOfCellsGreaterThan10 << " - Percentage = "
                << (double)numberOfCellsGreaterThan10/npcont * 100 << "%" << endl;
    outputFile << "Number of cells with error greater than 5% = " 
                << numberOfCellsGreaterThan5 << " - Percentage = "
                << (double)numberOfCellsGreaterThan5/npcont * 100 << "%" << endl;
    outputFile << "Number of cells with error greater than 2% = " 
                << numberOfCellsGreaterThan2 << " - Percentage = "
                << (double)numberOfCellsGreaterThan2/npcont * 100 << "%" << endl;
        */
    
    totalRefinementTime += refinementTime;
    totalDerefinementTime += derefinementTime;
    totalComputationTime += computationTime;
    
    outputFile << "Tempo de refinamento (s): " << refinementTime/1000 << std::endl;
    outputFile << "Tempo de desrefinamento (s): " << derefinementTime/1000 << std::endl;
    outputFile << "Tempo de computação (s): " << computationTime/1000 << std::endl;
    outputFile << 
            "________________________________________________________________________\n";
       
  }
  outputFile << "________________________________________________________________________\n" << endl;       
    outputFile << "Tempo de refinamento total (s): " << totalRefinementTime/1000 << endl;
    outputFile << "Tempo de desrefinamento total (s): " << totalDerefinementTime/1000 << endl;
    outputFile << "Tempo de refinamento e desrefinamento total (s): " << (totalRefinementTime + totalDerefinementTime)/1000
    << endl;
    outputFile << "Tempo de computação total (s): " << totalComputationTime/1000 << endl;
  
 
    
    
}   

/* *************************************************** */
/* FUNCAO: Chama a funcao que resolve a equacao da     */
/*         onda                                        */
/* *************************************************** */
void den_play_wave()
{
   
   if (flagpeano == 1) linpeano(7); //Apaga a curva de Peano anterior
   resolve_onda();
   if (flagpeano==1) linpeano(1);

}  


/* *************************************************** */
/* FUNCAO: Criterio para decidir o desrefinamento      */
/* *************************************************** */
void condes_onda(quad qref)
{ 
  int aux1,aux2,aux3,aux4;
  double maior1,maior2,maior3,maior4,maior;
  quad  q1,q2,q3,q4;
 
  if(flagpeano==1){
     linpeano(0);
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
  
   maior1=fabs(q1->fb);
   if((fabs(q1->fc))>maior1) maior1=q1->fc;
   if((fabs(q1->fd))>maior1) maior1=q1->fd;
   if((fabs(q1->fe))>maior1) maior1=q1->fe;
 
   maior2=fabs(q2->fb);
   if((fabs(q2->fc))>maior2) maior2=q2->fc;
   if((fabs(q2->fd))>maior2) maior2=q2->fd;
   if((fabs(q2->fe))>maior2) maior2=q2->fe;

   maior3=fabs(q3->fb);
   if((fabs(q3->fc))>maior3) maior3=q3->fc;
   if((fabs(q3->fd))>maior3) maior3=q3->fd;
   if((fabs(q3->fe))>maior3) maior3=q3->fe;
 
   maior4=fabs(q4->fb);
   if((fabs(q4->fc))>maior4) maior4=q4->fc;
   if((fabs(q4->fd))>maior4) maior4=q4->fd;
   if((fabs(q4->fe))>maior4) maior4=q4->fe;

    maior=maior1;
    if(maior2>maior) maior=maior2;
    if(maior3>maior) maior=maior3;
    if(maior4>maior) maior=maior4; 
    
   
         
      
   /* maior=q1->difer;
    if(q2->difer>maior) maior=q2->difer;
    if(q3->difer>maior) maior=q3->difer;
    if(q4->difer>maior) maior=q4->difer; */ 
   /* Decidi se o cacho quer ser desrefinado  */
    if ((aux1==aux2) && (aux1==aux3) && (aux1==aux4)){
       if(maior<=cotades)
       {    
         desrefina(q1);
         fdesrefina=1;
       }
   }  

}  /* fim da funcao condes  */

/* ********************************************** */
/* FUNCAO:  Refinamento inicial                   */
/*          Decide se refina a malha              */
/*          pecorrendo toda a curva de peano      */
/* ********************************************** */
double D_ref_onda_inicial()
{
 quad q1,qua;
 int frefina,flagnext;
 double maior_gradiente, time;
 
 if(flagpeano==1) linpeano(7); 
 
 
 //Medidor de tempo de refinamento
 CPerfTimer PerftempRef;
 PerftempRef.Start();
 
 predifquad_onda();
 
 do{       
       
   /*predifer_onda(); */ 
   q1=gne0;
   frefina=0;
   do{
      flagnext=0;
      maior_gradiente = fabs(q1->fb);
      if((fabs(q1->fc))>(maior_gradiente))
         maior_gradiente=fabs(q1->fc);
      if((fabs(q1->fd))>(maior_gradiente))
         maior_gradiente=fabs(q1->fd);
      if((fabs(q1->fe))>(maior_gradiente))
         maior_gradiente=fabs(q1->fe);

        if((q1->nivel <7)&&(maior_gradiente >= cotaref)){
       qua=q1;
       q1=qua->next;
       refina(qua);
       
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
       flagnext=1;
      }
      if (flagnext==0) {q1=q1->next; }  
   }while(q1!=null); 

  }while(frefina); 
  
  PerftempRef.Stop();
  time = PerftempRef.Elapsedms();

  if(flagpeano==1) linpeano(1);
  
  return time;

}

/* ************************************************** */
/* FUNCAO: Decide se refina a malha                   */
/*         pecorrendo toda a curva de peano           */
/* ************************************************** */
double D_ref_onda()
{
 quad q1,qua;
 int frefina,flagnext;
 double maior_gradiente, time;

 if(flagpeano==1) linpeano(7); 
 //Medidor de tempo de refinamento
 CPerfTimer PerftempRef;
 PerftempRef.Start();
      
   predifquad_onda();    
  do{   /* do para o refinamento varias vezes,  ate ser satisfeito */
            /* o criterio da cota     */  
   /*predifer_onda(); */ 
   q1=gne0;
   frefina=0;
   do{
      flagnext=0;
      maior_gradiente = fabs(q1->fb);
      if((fabs(q1->fc))>(maior_gradiente))
         maior_gradiente=fabs(q1->fc);
      if((fabs(q1->fd))>(maior_gradiente))
         maior_gradiente=fabs(q1->fd);
      if((fabs(q1->fe))>(maior_gradiente))
         maior_gradiente=fabs(q1->fe);

        if((q1->nivel <  7)&&(maior_gradiente >= cotaref)){  
       /* if((q1->nivel <7)&&(q1->difer >= cotaref)){ */ 
       qua=q1;
       q1=qua->next;
       refina(qua);
       /* qua=q1; */
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
       flagnext=1;
      }
      if (flagnext==0) {q1=q1->next; }  
   }while(q1!=null); 

   }while(frefina);  
  PerftempRef.Stop();
  time = PerftempRef.Elapsedms();

  if(flagpeano==1) linpeano(1); 
 
  return time;

}
/* ************************************************** */
/* FUNCAO : Decide se desrefina a malha               */
/*          pecorrendo toda a curva de peano          */
/* ************************************************** */
double D_desref_onda()
{
quad qauxa,qajuda;
int  nivel;
quad qteste;
double time;
   
if(flagpeano==1) linpeano(7); 

    //Medidor de tempo de desrefinamento
    CPerfTimer PerftempDesRef;
    PerftempDesRef.Start();
 
    fdesrefina=0;  
    predifer_onda();   
    qauxa=gne0;
    do
    {
      
      nivel = qauxa->nivel;
      
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
                 if ( qauxa->next->next->next->next != null )
                 qteste = qauxa->next->next->next->next; 
                 condes_onda(qauxa);
              }
            }
           }
         }
      } 
      
      
      qauxa = qteste;
      
      
      
    } while(qauxa!=null);  
  PerftempDesRef.Stop();
  time = PerftempDesRef.Elapsedms();
      
      

  if (flagpeano == 1) linpeano(1);
  
  return time;

}


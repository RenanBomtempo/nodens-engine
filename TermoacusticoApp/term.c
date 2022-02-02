
/*******************************************************/
/* PROGRAMA: term.c                                    */
/* AUTORA  : DENISE BURGARELLI DUCZMAL                 */
/* DATA    : 29/06/97                                  */
/*******************************************************/

#include <X11/X.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "funcoes.h"
#include "quad.h" 

#define null 0
/******************************************************* */
/* ************** variaveis externas ******************* */
/* ***************************************************** */
extern int flagglobal;
extern int flagdominio;
extern int flagonda_inicio;
/*extern int flagterm_inicio;*/  
extern int flagpeano;
extern double cotaref_term;     
extern double cotades_term;
extern quad gne0;            /* primeiro quad. da lista */
extern quad pq[];
extern int nquad[];
extern int npcont;          /* numero de quadradinhos   */   

/* ********************************************************/
/* Variaveis globais no programa term                     */
/**********************************************************/

/* double  deltatempo=0.00001;      (0.00001)/6.0 seg     */
/* double  tempofinal=10.0 * deltatempo;  0.0050, 72.0    */          
/* 4unid. de tempo para cada celula de nivel 9. (0.05cm)  */

double deltatempo,deltatempomax;
double tempofinal;
double tempoatual;
double tempo_fluxo;
int cor[8];
int flag_converge;             /*flag indica se convergiu ou nao          */
int fdesrefina_term;           /* indica se o desrefinamento se repete    */
int fdesrefina_proximo=0;      /* flag que indica se o proximo desref     */
int flag_placa=0;              /* indica se a fronteira e' a placa        */
int flag_write=0;              /* escreve nos arquivos svelx e stemper    */
int flagterm_inicio=0;         /* flag indica o inicio do programa term   */ 
int flag_fluxo=0;              /* flag que indica se deve ou nao zerar    */
                               /* os vetores da media do                  */
                               /* fluxo de energia                        */
int cont_fluxo=0;
int flag_incognita;            /* se 0 : velocidade direcao x             */
                               /* se 1 : velocidade direcao y             */
                               /* se 2 : temperatura                      */
int flag_desenho=0;            /* se 0: desenha variacoes de temper e vel */
                               /* se 1: desenha o vetores do fluxo de eneg*/
double  xiplaca,xfplaca;       /* dimensoes da placa                      */ 
                               /* Helium                                  */
double  viscmedia  = 1.876e-4; /* (1.876e-4)(gr/(cm.seg)) viscosidade dinamica*/
double  tempermedia= 255.0   ; /* 255(Kelvin)        temperatura media       */
double  condutmedia= 3.27e2;   /*(3.27e-4)(cal/(cm seg K) condut. termica    */
double  densmedia  = 1.9e-3  ; /* 1.9e-3(grama/(cm^3) ) densidade            */
double  auxpress =1.0;
double  Rgas       = 1.98  ; /*2.06e7*//*(1.986)((cal/mol K))/g)Const.univ.dosgasespormassa(1.10)*/
double  pressmedia ;       /*= 1e7;*/ /* (grama/(cm s2)) 1e7 Rgas*dens*temper */
double  frequencia = 1562.5   ; /* (Hz )           frequencia   3125           */
double  calorv     = 0.74; /* 1.34e6 */  /* (0.74)(cal/(gr.K))  calor esp por mass v */
double  calorp     = 1.24; /* 2.23e6 */ /* (1.24)(cal/(gr.K))   calor esp por mass p */
double  xescterm,yescterm;     /* mudanca de escala                       */
double  eps=0.0001;            /* 0.0001  criterio de convergencia da etapa k a k+1*/
FILE    *fopen(); 
/* *sterm, *sfront, *scondt,*svelx,*stemper,*aux;*/

/* ************************************************************ */
/* distancia em x e y : cm                                      */
/* dimensoes do tubo  : horizontal: 40cm e vertical:0.3125      */
/* velocidade         : cm/seg        */
/* pressao            :               */
/* temperatur         : kelvin        */
/* omega              : 2pifrequencia */
/* frequencia         : 500 Hz        */
/* calorv             : cal/(grama*K) */
/* densidade          : grama/(cm^3)  */
/* condut. termica    : cal/(cm seg K)*/
/* viscosidade        : gr/(cm*seg)   */
/* velocidade do som  : 94000 cm/seg 30000cm/seg   */ 
/* Numero de Mach     : 0.01                         */
/* velx0=Mach*velsom  :  940cm/seg  */
/* calor esp. pressao : cal/(grama*K)   */
/* calor esp, volume  : cal/(grama*K)   */ 
/****************************************************************** */
/* *************************************************** */
/* Funcao escala x e y                                 */
/* *************************************************** */
void term_escala(x,y)
double x;
double y;
{
  xescterm = 8.0 * (x); /* nas coordenadas fisicas (reais)         */
  yescterm = 8.0 * (y); /* nas coordenadas fisicas (reais)         */
                        /* dimensao objeto: x=1.0 por y=0.0625     */
                        /* dimensao fisica: x=8.0 por y=0.5        */
                        /* com malha uniforme nivel 6: 4 X 64 quad */
}

/* ***************************************************  */
  void term_lin_placa()
  {
   setcolor(0);
   lin(0.0,0.0,1.0,0.0); 
   xiplaca = 0.25; 
   xfplaca = 0.75;
   setcolor(3);
   lin(xiplaca,0.0,xfplaca,0.0);
}
  void term_lin_placa_cima()
  {
   setcolor(0);
   lin(0.0,0.0,1.0,0.0); 
   xiplaca = 0.25; 
   xfplaca = 0.75;
   setcolor(3);
   lin(xiplaca,0.262,xfplaca,0.262);
}
/* *************************************************** */
/* *************************************************** */
void fronteira_baixo(qaux)
quad qaux;
{

double x,y;

qaux->frontb=1;
x=qaux->coord[0];
y=qaux->coord[1];
if(flag_placa == 0){
  term_lin_placa();
flag_placa=1;}
  xiplaca=0.25;
  xfplaca=0.75; 

/*term_escala(x,y);
x=xescterm; */
/* ****************************** */
if ((0.0  <= x) && (x < xiplaca)) {          
    qaux->velxfrontbaixo= qaux->velxatual;
    qaux->velyfrontbaixo= 0.00;  /* 0.0 */
    qaux->densfrontbaixo=qaux->densatual;  
    qaux->temperfrontbaixo=qaux->temperatual;  
    qaux->pressfrontbaixo= qaux->pressatual;
    qaux->placa=0;
 }
   else if (( xiplaca <= x)&&(x <= xfplaca)) {     
      qaux->velxfrontbaixo = 0.0;  
      qaux->velyfrontbaixo= 0.0;
      qaux->densfrontbaixo= densmedia;       /*g/cm3*/     /*densmedia;*/
      qaux->temperfrontbaixo=tempermedia-
       0.1*(qaux->coord[0]-(xiplaca+xfplaca)/2.0)*tempermedia; /* 0.4 */
      qaux->pressfrontbaixo= pressmedia;
      qaux->placa=1;
   }
   else if ( (xfplaca < x) && (x <= 1.0)){        
     qaux->velxfrontbaixo =qaux->velxatual;  
     qaux->velyfrontbaixo= 0.00;  
     qaux->densfrontbaixo=qaux->densatual;  
     qaux->temperfrontbaixo=qaux->temperatual; 
     qaux->pressfrontbaixo= qaux->pressatual;
     qaux->placa=0;
   }
   
/* ***************************** */

}

/******************************************************* */
void fronteira_cima(qaux)
quad qaux;
{
 qaux->frontc=1;
 qaux->velxfrontcima = qaux->velxatual;  
 qaux->velyfrontcima = 0.0;
 qaux->densfrontcima = qaux->densatual;
 qaux->temperfrontcima=qaux->temperatual;  
 qaux->pressfrontcima =qaux->pressatual;
}
/******************************************************* */
void fronteira_direita(qaux)
quad qaux;
{
double omega,velx0,t0;
    omega=2.0*(3.141592654)*frequencia;
    velx0 =1000.0 ;                         
    t0=(8.0)/100000.0;
 qaux->frontd=1;
 /*qaux->velxfrontdireita   = velx0 * (sin(omega*(tempoatual+t0)));*/ 
 /*qaux->velxfrontdireita   = velx0 * 2.0*(sin(omega*(tempoatual+t0)));*/  
 qaux->velxfrontdireita   =   
   1.0*velx0 * (sin(omega*(tempoatual+t0)));  
 qaux->velyfrontdireita   = 0.0;
 qaux->densfrontdireita   = qaux->densatual;
 qaux->temperfrontdireita = qaux->temperatual;  
 qaux->pressfrontdireita  = qaux->pressatual;
    
}
/******************************************************* */
void fronteira_esquerda(qaux)
quad qaux;
{
  double velx0;
  double aux;
  double ang,pi;
  double omega,t0;
 
 
    omega=2.0*(3.141592654)*frequencia;
    velx0 =1000.0 ;                         
    /*aux= velx0 * (sin(omega*tempoatual));*/  
    t0=(8.0)/100000.0;
    aux= 
    0.1*velx0 * (sin(omega*tempoatual));  
    qaux->fronte=1;
    qaux->velxfrontesquerda   = aux;  
    qaux->velyfrontesquerda   = 0.0;
      /* O pistao como um reservatorio termico */
    /*
    qaux->densfrontesquerda   = densmedia;     
    qaux->temperfrontesquerda = tempermedia;    
    qaux->pressfrontesquerda  = pressmedia;  
    */
      /* O pistao como um isolante termico */
   qaux->densfrontesquerda   =  qaux->densatual;    
   qaux->temperfrontesquerda =  qaux->temperatual;   
   /* qaux->pressfrontesquerda  = pressmedia;*/  
    qaux->pressfrontesquerda=  qaux->pressatual;
  /*qaux->pressfrontesquerda =pressmedia*(1.0+0.03*sin(omega*tempoatual));*/  


      
}
/****************************************************** */

/*******************************************************/
/*******************************************************/
/* FUNCAO :                                            */
/* Calcula a diferenca do valor q1->usoma   de cada    */
/* quad. q1 com os vizinhos e coloca o resultado       */
/* em q1->fb, q1->fc,q1->fd, q1->fe (gradiente)        */
/* *****************************************************/

void term_difquad(q,q1,qua,dir)
quad q;
quad q1;
quad qua;
char dir;
{
  double fr,frp,frn;
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


  if((q->tipo=='a') || (q->ativo ==0) ){
    if(dir=='b')      {
       fr= fabs(q1->temperatual- q1->temperbaixo);
       fr=fr/q1->deltabaixo;
       if( fr>(q1->fb)){ q1->fb=fr;} 
       q1->difer+=fabs(fr);

    }
    else if(dir=='c') {
       fr= fabs(q1->temperatual- q1->tempercima);
       fr=fr/q1->deltacima;
      if(fr>(q1->fc)){q1->fc=fr;}  
       q1->difer+=fabs(fr);

    }
    else if(dir=='d') {
    fr= fabs(q1->temperatual- q1->temperdireita);
     fr=fr/q1->deltadireita;
     if(fr>(q1->fd)){q1->fd=fr;}
     q1->difer+=fabs(fr);   
    }
    else if(dir=='e') {
    fr= fabs(q1->temperatual- q1->temperesquerda);
    fr=fr/q1->deltaesquerda;
    if(fr>(q1->fe)){q1->fe=fr;}
    q1->difer+=fabs(fr);
     
    }
  }
  else  if((q->tipo=='q') && (q->ativo == 1)){

      fr=fabs( (q1->temperatual)-(q->temperatual) );
      q1->difer+=fabs(fr);
      q->difer+=fabs(fr);
     
      if(dir=='b'){
        fr=fr/q1->deltabaixo;
        if(fr>(q1->fb)) {q1->fb=fr;}
        if(fr>(q->fc )) {q->fc=fr; } 
        q1->difer+=fabs(fr);
        q->difer+=fabs(fr);
      }
      else if(dir=='c'){
        fr=fr/q1->deltacima;
        if(fr>(q1->fc)) {q1->fc=fr;}
        if(fr>(q->fb))  {q->fb=fr;} 
        q1->difer+=fabs(fr);
        q->difer+=fabs(fr);
        
      }
      else if(dir=='d'){
        fr=fr/q1->deltadireita;
        if(fr>(q1->fd)) {q1->fd=fr;}
        if(fr>(q->fe))  {q->fe=fr;} 
        q1->difer+=fabs(fr);
        q->difer+=fabs(fr);
      }
      else if(dir=='e'){
        fr=fr/q1->deltaesquerda;
        if(fr>(q1->fe)) {q1->fe=fr;}
        if(fr>(q->fd))  {q->fd=fr;} 
        q1->difer+=fabs(fr);
        q->difer+=fabs(fr);
      }


  }
}

/*******************************************************/
/*******************************************************/

/*******************************************************/
/* *****************************************************/
/* Calcula a diferenca do valor q1->usoma   de cada    */
/* quad. q1 com os vizinhos e coloca o resultado       */
/* em q1->difer  (fluxo)                               */
/* *****************************************************/
/* Esta funcao nao esta adaptatda a este programa */
void term_difer(q,q1,qua,dir)
quad q;
quad q1;
quad qua;
char dir;
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
      fr=((q1->temperatual)-0.0) *2.0; /* NAO */
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
       fr=((q1->temperatual)-(q->temperatual))/(float)fri;
       q1->difer+=fabs(fr);
       q->difer+=fabs(fr);

   }
}
/*******************************************************/
/*******************************************************/
 void term_predifer()
{
  quad q1,q,qua;
  char dir;

  q1=gne0;
  do{
      if(q1->ativo==1){
         q1->difer=0.0;
      }
      q1=q1->next;
   } while(q1!=null);

 q1=gne0;  
do{ 
 if(q1->ativo ==1){
 q=q1; qua=q;qua=qua->b; dir='b'; term_difer(q,q1,qua,dir);
 q=q1; qua=q;qua=qua->c; dir='c'; term_difer(q,q1,qua,dir);
 q=q1; qua=q;qua=qua->d; dir='d'; term_difer(q,q1,qua,dir);
 q=q1; qua=q;qua=qua->e; dir='e'; term_difer(q,q1,qua,dir);
 }
 q1=q1->next;
}while(q1!=null);

} 

/* **************************************************** */
/* *************************************************** */
/* FUNCAO:preenche fa,fb,fc,fd chamando  difquad       */
/* *************************************************** */
 void term_predifquad()
{
quad q1,q,qua;
char dir;


q1=gne0;  /* Percorre a curva de Peano */
do{
 if(q1->ativo ==1 ){ 
   q1->fb=0.0;    q1->fc=0.0;    q1->fd=0.0;   q1->fe=0.0;
   q1->difer=0.0;
 }
 q1=q1->next;
}while(q1!=null);

q1=gne0;  /* Percorre a curva de Peano */
do{
  if(q1->ativo ==1){ 
    q=q1; qua=q; qua=qua->b; dir='b'; term_difquad(q,q1,qua,dir);
    q=q1; qua=q; qua=qua->c; dir='c'; term_difquad(q,q1,qua,dir);
    q=q1; qua=q; qua=qua->d; dir='d'; term_difquad(q,q1,qua,dir);
    q=q1; qua=q; qua=qua->e; dir='e'; term_difquad(q,q1,qua,dir);
 }
 q1=q1->next;
}while(q1!=null);
     
}
/* *************************************************** */
/* *****************************************************/
/* FUNCAO : term_vizquad()                             */
/* Calcula o valor q1->valordireita  q1->valoresquerda */
/*                  q1->valorbaixo   q1->valorcima     */
/* de cada                                             */
/* quad. q1 com os vizinhos                            */
/* *****************************************************/

void  term_vizquad(q,q1,qua,dir)
quad q;
quad q1;
quad qua;
char dir;
{
  double fr;
  int    fri,i,nivel,aux;
  double razao;
  double deltadistancia,distancia;
  quad   qajuda;

  /* fprintf(stdout,"\n ANTES  q1->np  %d qua->nivel %d  ",q1->np,qua->nivel);
   fprintf(stdout,"\n q1->nivel %d qua->tipo %d\n",q1->nivel,qua->tipo);*/

   if(qua->nivel > q->nivel)
        while((qua->tipo=='a')&&(qua->b!=null)){
       qua=qua->c;
     }
    else
      while((qua->tipo=='a')&&(qua->b!=null)){
            qua=qua->b;
       }
   
  q=qua;

 /* fprintf(stdout,"\n DEPOIS  q1->np    %d qua->nivel %d  \n",q1->np,qua->nivel);
  fprintf(stdout,"q1->nivel %d qua->tipo %d\n",q1->nivel,qua->tipo);*/

 if((q->tipo=='a') || (q->ativo == 0)){
    if(dir=='b')      {
     /* fprintf(stdout,"\nFRONT B\n");*/
      q1->deltabaixo = q1->meiolado;
      /*qajuda =q1;*/
      fronteira_baixo(q1);
      /*q1=qajuda;*/
      
      q1->velxbaixo =   q1->velxfrontbaixo;
      q1->velybaixo =   q1->velyfrontbaixo;
      q1->densbaixo =   q1->densfrontbaixo;
      q1->temperbaixo=  q1->temperfrontbaixo;
      q1->pressbaixo =  q1->pressfrontbaixo;
    }
    else if(dir=='c') {
    /*  fprintf(stdout,"\nFRONT C\n");*/
       q1->deltacima = q1->meiolado;
       /*qajuda =q1;*/
       fronteira_cima(q1);
       /*q1 =qajuda;*/
 
       q1->velxcima   =  q1->velxfrontcima;
       q1->velycima   =  q1->velyfrontcima;
       q1->denscima   =  q1->densfrontcima;
       q1->tempercima =  q1->temperfrontcima;
       q1->presscima  =  q1->pressfrontcima;
    }
    else if(dir=='d') {
    /*  fprintf(stdout,"\nFRONT D\n");*/
       q1->deltadireita = q1->meiolado;
       /*qajuda =q1;*/
       fronteira_direita(q1);
       /*q1=qajuda;*/

       q1->velxdireita   =  q1->velxfrontdireita;
       q1->velydireita   =  q1->velyfrontdireita;
       q1->densdireita   =  q1->densfrontdireita;
       q1->temperdireita =  q1->temperfrontdireita;
       q1->pressdireita  =  q1->pressfrontdireita;
    }
    else if(dir=='e') {
     /* fprintf(stdout,"\nFRONT E\n");*/
       q1->deltaesquerda = q1->meiolado;
       /*qajuda =q1;*/
       fronteira_esquerda(q1);
       /*q1=qajuda;*/

       q1->velxesquerda   = q1->velxfrontesquerda;
       q1->velyesquerda   = q1->velyfrontesquerda;
       q1->densesquerda   = q1->densfrontesquerda;
       q1->temperesquerda = q1->temperfrontesquerda;
       q1->pressesquerda  = q1->pressfrontesquerda;
    }
  }
 else  if((q->tipo=='q') && (q->ativo ==1)){
   if( (q1->nivel >q->nivel) ||
      ((q1->nivel ==q->nivel) &&
      ((q1->coord[0] >q->coord[0]) ||
      ((q1->coord[0] == q->coord[0]) &&(q1->coord[1] >q->coord[1])))))
   {

    fri=1;
    i=(q1->nivel)-(q->nivel);
    while(i>0){
       fri*=2;
       i--;
    }
   
    razao=1.0/(float)fri;
    deltadistancia= q1->meiolado +q->meiolado;        
    if(dir=='b'){
       distancia=q->deltacima;
      if(deltadistancia >(distancia) ){
         q->deltacima = deltadistancia;
      }
    
      q1->deltabaixo    =  2*(q1->meiolado); 
      q1->velybaixo    += (q->velyatual)   ;
      q1->velxbaixo    += (q->velxatual)   ;       
      q1->pressbaixo   += (q->pressatual)  ;
      q1->densbaixo    += (q->densatual)   ;
      q1->temperbaixo  += (q->temperatual) ;

      q->velycima    +=  (q1->velyatual)  * razao ;
      q->velxcima    +=  (q1->velxatual)  * razao ;       
      q->presscima   +=  (q1->pressatual) * razao ;
      q->denscima    +=  (q1->densatual)  * razao ;
      q->tempercima  +=  (q1->temperatual)* razao ;


      }
      else if(dir=='c'){
        distancia=q->deltabaixo;
        if(deltadistancia >(distancia)){
           q->deltabaixo = deltadistancia;
       }
       q1->deltacima    = 2*(q1->meiolado);
       q1->velycima    +=  (q->velyatual)   ;
       q1->velxcima    +=  (q->velxatual)   ;       
       q1->presscima   +=  (q->pressatual)  ;
       q1->denscima    +=  (q->densatual)   ;
       q1->tempercima  +=  (q->temperatual) ;

       q->velybaixo    += (q1->velyatual)  * razao;
       q->velxbaixo    += (q1->velxatual)  * razao;       
       q->pressbaixo   += (q1->pressatual) * razao;
       q->densbaixo    += (q1->densatual)  * razao;
       q->temperbaixo  += (q1->temperatual)* razao;
             
        
      }
      else if(dir=='d'){
        distancia=q->deltaesquerda;
       if(deltadistancia >(distancia)){
         q->deltaesquerda = deltadistancia;
       }
       q1->deltaesquerda  = 2*(q1->meiolado);
       q1->velxdireita   += q->velxatual   ;
       q1->velydireita   += q->velyatual   ;
       q1->pressdireita  += q->pressatual  ;
       q1->densdireita   += q->densatual   ;
       q1->temperdireita += q->temperatual ;
     
       q->velxesquerda    += q1->velxatual * razao  ;
       q->velyesquerda    += q1->velyatual * razao  ;
       q->pressesquerda   += q1->pressatual* razao  ;
       q->densesquerda    += q1->densatual * razao  ;
       q->temperesquerda  += q1->temperatual * razao;
      }
      else if(dir=='e'){
         distancia=(q->deltadireita);
       if(deltadistancia >(distancia)){
         q->deltadireita = deltadistancia;
       }

       q1->deltaesquerda   = 2*q1->meiolado;
       q1->velxesquerda   += q->velxatual  ;
       q1->velyesquerda   += q->velyatual  ;
       q1->pressesquerda  += q->pressatual ;
       q1->densesquerda   += q->densatual  ;
       q1->temperesquerda += q->temperatual;

       q->velxdireita    += q1->velxatual  * razao ;
       q->velydireita    += q1->velyatual  * razao ;
       q->pressdireita   += q1->pressatual * razao ;
       q->densdireita    += q1->densatual  * razao ;
       q->temperdireita  += q1->temperatual* razao ;

      }

   }
  }
}

/* *************************************************** */
/* *************************************************** */
/* FUNCAO:preenche fa,fb,fc,fd chamando  difquad       */
/* *************************************************** */
 void term_previzquad()
{
quad q1,q,qua;
char dir;
int aux,i,nivel;
double meiolado,ajuda;


q1=gne0;  /* Percorre a curva de Peano */
do{
  if(q1->ativo ==1){ 
   q1->velxcima      =0.0;  q1->velxbaixo =0.0;  q1->velxdireita =0.0;
   q1->velxesquerda  =0.0;
   q1->velycima      =0.0;  q1->velybaixo =0.0;  q1->velydireita =0.0;   
   q1->velyesquerda  =0.0;
   q1->denscima      =0.0; q1->densbaixo  =0.0;   q1->densdireita=0.0; 
   q1->densesquerda  =0.0;
   q1->presscima     =0.0; q1->pressbaixo =0.0;  q1->pressdireita=0.0; 
   q1->pressesquerda =0.0;
   q1->tempercima    =0.0; q1->temperbaixo=0.0; q1->temperdireita=0.0;
   q1->temperesquerda=0.0;
   q1->frontd=0;
   q1->fronte=0;
   q1->frontb=0;
   q1->frontc=0;


   aux=2;
   nivel=q1->nivel;
   for(i=0;i<nivel;i++){
       aux=2*aux;
   }
   meiolado=8/((float)aux); 
   q1->meiolado      = meiolado;
   q1->deltadireita  = meiolado;
   q1->deltaesquerda = meiolado;
   q1->deltacima     = meiolado;
   q1->deltabaixo    = meiolado;
 
 }
 /*fprintf(stdout,"\n q1->d->np %d \n",q1->d->np);*/
 q1=q1->next;
}while(q1!=null);

q1=gne0; 
do{ 
   if(q1->ativo ==1){
    /*fprintf(stdout,"\nDIR B\n");*/
    q=q1; qua=q; qua=qua->b; dir='b'; term_vizquad(q,q1,qua,dir);
    /*fprintf(stdout,"\nDIR C\n");*/
    q=q1; qua=q; qua=qua->c; dir='c'; term_vizquad(q,q1,qua,dir);
    /*fprintf(stdout,"\nDIR D\n");*/
    q=q1; qua=q; qua=qua->d; dir='d'; 
    term_vizquad(q,q1,qua,dir);
    /*fprintf(stdout,"\nDIR E\n");*/
    q=q1; qua=q; qua=qua->e; dir='e'; term_vizquad(q,q1,qua,dir);
   }
   q1=q1->next;
}while(q1!=null);
 
  /*fprintf(stdout,"\nPASSEI POR TERM_VIZQUAD\n");*/

/* calculo das derivadas laterais */
  /*q1=gne0;
  do{
    if(q1->ativo ==1){
    q1->dvelxbaixo  = (q1->velxatual   - q1->velxbaixo  )/q1->deltabaixo;
    q1->dvelybaixo  = (q1->velyatual   - q1->velybaixo  )/q1->deltabaixo;
    q1->dtemperbaixo= (q1->temperatual - q1->temperbaixo)/q1->deltabaixo;
    q1->ddensbaixo  = (q1->densatual   - q1->densbaixo  )/q1->deltabaixo;
    q1->dpressbaixo = (q1->pressatual  - q1->pressbaixo )/q1->deltabaixo;

    q1->dvelxcima  =  (q1->velxcima   - q1->velxatual  )/q1->deltacima;
    q1->dvelycima  =  (q1->velycima   - q1->velyatual  )/q1->deltacima;
    q1->dtempercima=  (q1->tempercima - q1->temperatual)/q1->deltacima;
    q1->ddenscima  =  (q1->denscima   - q1->densatual  )/q1->deltacima;
    q1->dpresscima =  (q1->presscima  - q1->pressatual )/q1->deltacima;

    q1->dvelxdireita  =(q1->velxdireita  -q1->velxatual   )/q1->deltadireita;
    q1->dvelydireita  =(q1->velydireita  -q1->velyatual   )/q1->deltadireita;
    q1->dtemperdireita=(q1->temperdireita-q1->temperatual )/q1->deltadireita;
    q1->ddensdireita  =(q1->densdireita  -q1->densatual   )/q1->deltadireita;
    q1->dpressdireita =(q1->pressdireita -q1->pressatual  )/q1->deltadireita;

  q1->dvelxesquerda  =(q1->velxatual  -q1->velxesquerda  )/q1->deltaesquerda;
  q1->dvelyesquerda  =(q1->velyatual  -q1->velyesquerda  )/q1->deltaesquerda;
  q1->dtemperesquerda=(q1->temperatual-q1->temperesquerda)/q1->deltaesquerda;
  q1->ddensesquerda  =(q1->densatual  - q1->densesquerda )/q1->deltaesquerda;
  q1->dpressesquerda =(q1->pressatual - q1->pressesquerda)/q1->deltaesquerda;

   }
   q1=q1->next;
  }while(q1!=null);  */


}
/* *************************************************** */
/* **************************************************  */
/* *************************************************** */

/* *************************************************** */
/* FUNCAO: Monta as condicoes iniciais do problema     */
/* *************************************************** */
void term_cond_inicial()
{
quad q1;
int numero;
double fatx=0;
double faty=0;
/*long RAND_MAX;*/


/*RAND_MAX = 1024 *1024 *1024 * 2 -1;*/

q1=gne0;
numero=0;
do{   
    q1->np = numero;
    if(q1->ativo == 1){
       /* condicao inicial da densidade*/
       q1->densatual =  densmedia+(((float)rand())/(float)(RAND_MAX+1.0) )*faty;    
       q1->densant   =  densmedia+(((float)rand())/(float)(RAND_MAX+1.0) )*faty;    
       
       /* condicao inicial da temperatura */
       q1->temperatual = tempermedia+(((float)rand())/(float)(RAND_MAX+1.0))*faty;    
       q1->temperant   = tempermedia+(((float)rand())/(float)(RAND_MAX+1.0))*faty;    

       /* condicao inicial da velocidade */
       
       q1->velxatual = ( ((float)rand()) /(float)(RAND_MAX+1.0) )*fatx;    
       q1->velxant   = (((float)rand()) /(float)(RAND_MAX+1.0))*fatx; 
       q1->velyatual = ( ((float)rand()) /(float)(RAND_MAX+1.0) )*faty;    
       q1->velyant   = (((float)rand()) /(float)(RAND_MAX+1.0))*faty;  


       /* condicao inicial da pressao */
       q1->pressatual = pressmedia+(((float)rand())/(float)(RAND_MAX+1.0))*faty;    
       q1->pressant   = q1->pressatual+(((float)rand())/(float)(RAND_MAX+1.0))*faty;    

       /* condicao inicial da energia interna */
       q1->energatual = 0.0+(((float)rand())/(float)(RAND_MAX+1.0))*faty;    
       q1->energant   = 0.0+(((float)rand())/(float)(RAND_MAX+1.0))*faty;    

       /* condicao inicial da viscosidade */
       q1->viscatual = viscmedia+(((float)rand())/(float)(RAND_MAX+1.0))*faty;    
       q1->viscant   = viscmedia+(((float)rand())/(float)(RAND_MAX+1.0))*faty;    
        
       /* condicao inicial da condutividade termica */
       q1->condutatual = condutmedia+(((float)rand())/(float)(RAND_MAX+1.0))*faty;    
       q1->condutant   = condutmedia+(((float)rand())/(float)(RAND_MAX+1.0))*faty;    

       /* condutmedia = 1.4;  condutividade termica media (mW/cmK)  */
    }
    numero=numero +1;
    q1=q1->next;
  }while(q1!=null);
      
 }
/* ************************************************** */
/* ************************************************** */
/* Funcao que resolve a equacao da pressao            */
/* ************************************************* */
 void term_pressao()
 {
  quad q1;
  q1=gne0;
  do{
    if(q1->ativo ==1){
       q1->pressatual= auxpress*Rgas*(q1->densatual)*(q1->temperatual);
       if(flag_write==1){
        /*if(q1->np==33){fprintf(svelx,"\npress %.9g",q1->pressatual);}*/
      }
    }
   q1=q1->next;
  }while(q1!=null);



  /* Para evitar que haja instabilidade com a fronteira */
  /* O valor que esta a direita do quad. que faz divisa com a fronteira */
  /* esquerda, recebe o valor da media, isto e,                          */
   q1=gne0;
   do{
    if(q1->ativo==1){
     if(q1->fronte==1){
       if(q1->e->tipo=='a'){
          q1->d->pressatual = (q1->pressatual)*0.0 + (q1->d->pressatual)*1.0; 
       }
     }
    }
    q1=q1->next;
   }while(q1!=null);
 }
/* ************************************************** */
/* Funcao que resolve a equacao da viscosidade        */
/* ************************************************** */
 void term_visc()
 {
  quad q1;
  double aux;
  q1=gne0;
  do{
    if(q1->ativo == 1){ 
       aux= exp( 0.7 * log(fabs( (q1->temperatual)/tempermedia)));
       q1->viscatual= viscmedia * aux;
       if(flag_write==1){
       /*  if(q1->np==33) fprintf(svelx,"\nvisc=%.9g",q1->viscatual); */
       }
    }
    q1=q1->next;
  }while(q1!=null);
}
/* ********************************************************* */
/* Funcao que resolve a equacao da condutividade termica     */
/* ********************************************************* */
 void term_condut()
 {
  quad q1;
  double aux;
  q1=gne0;
  do{
     if(q1->ativo ==1){
        aux = exp( (0.7) * log(fabs ((q1->temperatual)/tempermedia)));
        q1->condutatual= condutmedia * aux;
     }
     q1=q1->next;
  }while(q1!=null);

 }
/* ************************************************************ */
/* Funcao que resolve a equacao do momentum                     */
/* ************************************************************ */
 void term_momentum()
 {
 quad q1;
 double dux2,dvyx2,dvy2,duxy2;
 double aux1,aux2,aux3,aux4;
 double dpxd,dpxe,dpx,dpyc,dpyb,dpy; 
 double dist, daux1,daux2;
 double pressfacedireita,pressfaceesquerda;
 double pressfacecima,pressfacebaixo;
 double velxfacedireita,velxfaceesquerda;
 double velyfacecima,velyfacebaixo;

 /* Equacao do momentum na direcao x */
 term_previzquad();

 q1=gne0;
 do{
    if(q1->ativo ==1){

     if(q1->frontd==0){
      velxfacedireita  = (q1->velxdireita +q1->velxatual)/2.0;
      pressfacedireita = (q1->pressdireita +  q1->pressatual)/2.0; 
     }
     else {
      velxfacedireita  = q1->velxdireita ;
      pressfacedireita = q1->pressdireita ; 
     }
     if(q1->fronte==0){
      velxfaceesquerda = (q1->velxesquerda+q1->velxatual)/2.0;
      pressfaceesquerda=(q1->pressesquerda + q1->pressatual)/2.0; 
     }
     else {
      velxfaceesquerda = q1->velxesquerda;
      pressfaceesquerda= q1->pressesquerda ; 
     }

    /* Calculo da derivada 2 da veloc. horiz. com relacao a x */
    /*dux2=(velxfacedireita-q1->velxatual)/q1->meiolado -
          (q1->velxatual-velxfaceesquerda)/q1->meiolado;*/ 
    
    dux2= 2 *((velxfacedireita-q1->velxatual) -
               (q1->velxatual-velxfaceesquerda)); /* *4 */ 

   


     /* Derivada com relacao a x e depois a y  TEMPORARIAMENTE=0 */
     dvyx2= 0.0; 
     aux1= (dux2 +dvyx2)/3.0;

     /* Calculo da derivada pressao */
      dpx=(pressfacedireita-pressfaceesquerda)*(2*q1->meiolado);
      
     aux2 = dpx/(q1->viscatual);
     q1->ladodireito = deltatempo * (aux1 -aux2);
     q1->fator =  (q1->densatual/ q1->viscatual) *(2*q1->meiolado) 
                   *(2*q1->meiolado); /* sinal */
    }
    q1= q1->next;
 }while(q1!=null);

 /* **************************************************** */
 /* Resolucao pelo metodo das caracteristicas modificado */
 /* u_t +u u_x +v u_y =  (u_n+1 -u~_n)/deltat            */
 /* **************************************************** */ 
 /*  flag_incognita = 0; */
   
 term_conveccao(0,deltatempo); 
 q1=gne0;
 do{
  if(q1->ativo ==1 ){
     q1->ladodireito= q1->ladodireito +(q1->fator * q1->velxpe );
   if(flag_write==1){
    if(q1->np==33){
   /*fprintf(svelx,"\n np=%d fator=%.9g ladodireito=%.9g\n",q1->np, q1->fator,q1->ladodireito); */
    }
     }
  }
  q1=q1->next;
 }while(q1!=null);
         
 /* ************************************************* */ 
 /* Construcao da matriz associada ao laplaciano e    */
 /* resolucao desta pelo gradiente conjugado          */
 /* laplac u - fator( u_n+1) = ladodireito            */
 /* ************************************************* */
  flag_incognita=0; 
  /*fprintf(stdout,"DIFUSAOOOOO");*/
  term_difusao(0,deltatempo);     
  /*fprintf(stdout,"fim velx\n");*/

  /* Para evitar que haja instabilidade com a fronteira */
  /* O valor que esta a direita do quad. que faz divisa com a fronteira */
  /* esquerda, recebe o valor da media, isto e,                          */
   q1=gne0;
   do{
    if(q1->ativo==1){
     if(q1->fronte==1){
       if(q1->e->tipo=='a'){
          q1->d->velxatual = (q1->velxatual)*0.0 + (q1->d->velxatual)*1.0; 

       }
     }
    }
    q1=q1->next;
   }while(q1!=null);

 /* ************************************************ */
 /* equacao do momentum na direcao y                 */
 /* ************************************************ */
  term_previzquad();
  q1=gne0;
  do{
    if(q1->ativo ==1){

     if(q1->frontc==0){     
       velyfacecima  = (q1->velycima +q1->velyatual)/2.0;
       pressfacecima =(q1->presscima  + q1->pressatual)/2.0;
     }
     else{
       velyfacecima  = q1->velycima ;
       pressfacecima =q1->presscima ;

     }
     if(q1->frontb==0){ 
       velyfacebaixo = (q1->velybaixo+q1->velyatual)/2.0;
       pressfacebaixo=(q1->pressbaixo + q1->pressatual)/2.0;
     }
     else{ 
       velyfacebaixo = q1->velybaixo;
       pressfacebaixo=q1->pressbaixo ;
     }

     /*Calculo da derivada seg. da veloc. vert. com relacao a y*/
     /*dvy2=(velyfacecima-q1->velyatual)/q1->meiolado -
          (q1->velyatual-velyfacebaixo)/q1->meiolado;*/  

     dvy2=( (velyfacecima-q1->velyatual) -
          (q1->velyatual-velyfacebaixo) )* (2);  /* * (2);*/  



      /* derivada com relacao a x e depois a y TEMPORARIAMENTE=0 */
      duxy2=0.0 ; 
      aux1=(1.0/3.0) * (dvy2 +duxy2);

     /* calculo de pressao acima  */
     dpy=(pressfacecima-pressfacebaixo)*(2*q1->meiolado);

      aux2 = dpy/(q1->viscatual);
      q1->ladodireito = deltatempo * (aux1 -aux2);
      q1->fator=(q1->densatual /q1->viscatual) *(2*q1->meiolado)
                 *(2*q1->meiolado);/*sinal */
      

     }
     q1=q1->next;
  }while(q1!=null);
  /* **************************************************** */
  /* Resolucao pelo metodo das caracteristicas modificado */
  /* v_t +u v_x +v v_y =  (v_n+1 -v~_n)/deltat            */
  /* **************************************************** */
  /* flag_incognita=1 */
  term_conveccao(1,deltatempo);
  q1=gne0;
  do{
     if(q1->ativo ==1 ){
       q1->ladodireito= q1->ladodireito +(q1->fator *q1->velype );
     
    }  
     q1=q1->next;
  }while(q1!=null);
         
  /* ************************************************* */ 
  /* Construcao da matriz associada ao laplaciano e    */
  /* resolucao desta pelo gradiente conjugado          */
  /* (deltatempo)laplac v - fator( v_n+1) = ladodireito*/
  /* ************************************************* */
  /* flag_incognita=1; */ 
  /*term_ativo();*/
  term_difusao(1,deltatempo);     

  /*fprintf(stdout,"fim vely\n");*/

 }
/* ***************************************************** */
/* ***************************************************** */
/*  Funcao que calcula o valor de phi                    */ 
/* ***************************************************** */
void term_phi()
{
quad q1;
double dux,duy, dvx,dvy;
double duxd,duxe,duyc,duyb, dvxd,dvxe,dvyc,dvyb;
double velxfacedireita,velxfaceesquerda,velxfacecima,velxfacebaixo;
double velyfacedireita,velyfaceesquerda,velyfacecima,velyfacebaixo;

    
term_previzquad();
q1=gne0;
    
do{
   if(q1->ativo ==1 ){
    
  if(q1->frontd==0){
   velxfacedireita  = (q1->velxdireita  +q1->velxatual)/2.0; 
   velyfacedireita  = (q1->velydireita  +q1->velyatual)/2.0; 
  }
  else{
   velxfacedireita  = q1->velxdireita ; 
   velyfacedireita  = q1->velydireita ; 
  }

  if(q1->fronte==0){
   velxfaceesquerda = (q1->velxesquerda +q1->velxatual)/2.0;
   velyfaceesquerda = (q1->velyesquerda +q1->velyatual)/2.0;
   }
   else{
   velxfaceesquerda = q1->velxesquerda ;
   velyfaceesquerda = q1->velyesquerda ;
   }

  if(q1->frontb==0){
   velxfacebaixo    = (q1->velxbaixo    +q1->velxatual)/2.0;
   velyfacebaixo    = (q1->velybaixo    +q1->velyatual)/2.0;
   }
  else{
   velxfacebaixo    = q1->velxbaixo   ;
   velyfacebaixo    = q1->velybaixo   ;
   }

  if(q1->frontc==0){
   velxfacecima   = (q1->velxcima     +q1->velxatual)/2.0;
   velyfacecima   = (q1->velycima     +q1->velyatual)/2.0;
   }
   else{
   velxfacecima   = q1->velxcima   ;
   velyfacecima   = q1->velycima   ;
   }

   dux= (velxfacedireita - velxfaceesquerda)/(2*q1->meiolado); 
   duy= (velxfacecima    - velxfacebaixo)/(2*q1->meiolado);
   dvx= (velyfacedireita - velyfaceesquerda)/(2*q1->meiolado);
   dvy= (velyfacecima    - velyfacebaixo )/(2*q1->meiolado);

     q1->phi=( (dux*dux) + 
               (dvy*dvy) + 
               (0.5 * (duy+dvx)*(duy+dvx)) -
               ( (dux+dvy)*(dux+dvy)/3.0 ) ) ;

   }
   q1=q1->next;
  }while(q1!=null);

}
/* ************************************************* */
/*  Funcao que resolve a equacao da temperatura      */
/* ************************************************* */
void term_temperatura()
{
quad q1;
double dux,dvy;
double divergvel; /* divergente da velocidade */
double aux1,aux2,aux3,aux4;
double temperpe; /* temperatura no pe da caracteristica */
double duxd,duxe,dvyc,dvyb;
double velxfacedireita  ; 
double velxfaceesquerda ;
double velyfacecima     ;
double velyfacebaixo    ;
double integdivergvel   ;
      
    
term_previzquad();
q1=gne0;
do{
 if(q1->ativo ==1){
   if(q1->frontd==0){
     velxfacedireita  =(q1->velxdireita + q1->velxatual)/2.0;
   }
   else{
     velxfacedireita  =q1->velxdireita;
   } 
   if(q1->fronte==0){
     velxfaceesquerda =(q1->velxesquerda+ q1->velxatual)/2.0;
   }
   else{
     velxfaceesquerda =q1->velxesquerda;
   }
   if(q1->frontc==0){
   velyfacecima     =(q1->velycima    + q1->velyatual)/2.0;
   }
   else{
   velyfacecima     =q1->velycima;
   
   }
   if(q1->frontb==0){
   velyfacebaixo    =(q1->velybaixo   + q1->velyatual)/2.0;
    }
    else{
   velyfacebaixo    =q1->velybaixo;

     }
   integdivergvel=(velxfacedireita-velxfaceesquerda+
                  velyfacecima-velyfacebaixo)*(2*q1->meiolado);
       

      /*dux = (q1->dvelxdireita + q1->dvelxesquerda)/2.0  ;
      dvy   = (q1->dvelycima    + q1->dvelybaixo   )/2.0 ;
      divergvel = dux+dvy;
      aux1  = ((q1->pressatual)*divergvel*/

      aux1 = ( (q1->pressatual)* integdivergvel );
      aux2 = 2*(q1->viscatual)*(q1->phi)*(2*q1->meiolado)*(2*q1->meiolado);



      aux1 = aux1/(q1->condutatual);
      aux2 = aux2/(q1->condutatual);
      aux3 = ((q1->densatual)*calorv)/(q1->condutatual); 
      q1->fator = aux3*(2*q1->meiolado)*(2*q1->meiolado); /*sinal */
      q1->ladodireito = (aux2 - aux1)*deltatempo;
     
/*      fprintf(aux,  " \n q1->np = %d  ",q1->np);
      fprintf(aux,  " fronte= %d", q1->fronte);
      fprintf(aux,  " frontd=%d", q1->frontd);
      fprintf(aux,  " velxdireita = %.9g  ",q1->velxdireita);
      fprintf(aux,  " velxatual = %.9g  ",q1->velxatual);
      fprintf(aux,  " velxesquerda = %.9g  ",q1->velxesquerda);
      fprintf(aux,  " velycima = %.9g",q1->velycima);
      fprintf(aux,  " velybaixo = %.9g",q1->velybaixo);
      fprintf(aux,  " \n integdivergvel = %.9g  ",integdivergvel);
      fprintf(aux,  " phi = %.9g  ", q1->phi);
      fprintf(aux,  " pressatual = %.9g  ",q1->pressatual);
      fprintf(aux,  " \n viscatual = %.9g  ",q1->viscatual);
      fprintf(aux,  " condut = %.9g  ", q1->condutatual);
      fprintf(aux,  " aux1 = %.9g  ",aux1);
      fprintf(aux,  " aux2 = %.9g  ",aux2);
      fprintf(aux,  " aux3 = %.9g  " ,aux3);
      fprintf(aux,  " \n fator = %.9g  ",q1->fator);
      fprintf(aux,  " ladodireito = %.9g  ",q1->ladodireito);
      fprintf(aux,  "\n *********************************************"); */
     
     }
     q1=q1->next;
   }while(q1!=null); 
  /* **************************************************** */
  /* Resolucao pelo metodo das caracteristicas modificado */
  /* T_t +u T_x +v T_y =  (T_n+1 -T~_n)/deltat            */
  /* **************************************************** */
  /*  flag_incognita = 2 */
  term_conveccao(2,deltatempo);
  q1=gne0;
  do{
   if(q1->ativo ==1){
     q1->ladodireito= q1->ladodireito+(q1->fator)*(q1->temperpe);
   /*  fprintf(aux,"\n ladodireito= %.9g  ",q1->ladodireito); */
   }
   q1=q1->next;
  }while(q1!=null);
         
   /* ************************************************* */ 
   /* Construcao da matriz associada ao laplaciano e    */
   /* resolucao desta pelo gradiente conjugado          */
   /* (deltatempo)laplac T - fator( T_n+1) = ladodireito*/
   /* ************************************************* */
   /* flag_incognita=2; */
   /*term_ativo();  */       
   term_difusao(2,deltatempo );     

}

/* ************************************************* */
/* ************************************************ */
/* Funcao que resolve a equacao da energia interna  */
/* ************************************************ */
void  term_energia()
{
 quad q1;
   
 q1=gne0;
 do {
    if(q1->ativo ==1){
       q1->energatual= calorv *( (q1->temperatual) - (tempermedia) );
    }
    q1=q1->next;
 }while(q1!=null);

 }
/* *************************************************** */
/* Funcao que resolve a equacao da continuidade        */
/* A incognita e' a densidade                          */
/* *************************************************** */
void term_continuidade()
{
 quad q1;
 double dxdens,dydens,dxvelx,dyvely;
 double div_dens_vel;
 double densfacedireita,densfaceesquerda,velxfacedireita,
        velxfaceesquerda,
        densfacecima,densfacebaixo,velyfacecima,velyfacebaixo;

 term_previzquad();
 q1=gne0;
 do{
    if(q1->ativo ==1 ){ 
       if(q1->frontd==0){
       densfacedireita =((q1->densdireita) + (q1->densatual))/(2.0);
       velxfacedireita =((q1->velxdireita)  +(q1->velxatual))/(2.0);
       }
       else{
       densfacedireita =q1->densdireita;
       velxfacedireita =q1->velxdireita;
       }
      if(q1->fronte==0){
       densfaceesquerda=((q1->densesquerda)+ (q1->densatual))/(2.0);
       velxfaceesquerda=((q1->velxesquerda) +(q1->velxatual))/(2.0);
      }
      else{
       densfaceesquerda=q1->densesquerda;
       velxfaceesquerda=q1->velxesquerda;
      }
      if(q1->frontc==0){
       densfacecima =((q1->denscima)  +(q1->densatual   ))/(2.0);
       velyfacecima =((q1->velycima)  +(q1->velyatual))/(2.0);
      }
      else{
       densfacecima =q1->denscima;
       velyfacecima =q1->velycima;

      }     
      if(q1->frontb==0){ 
       densfacebaixo=((q1->densbaixo) +(q1->densatual   ))/(2.0);
       velyfacebaixo=((q1->velybaixo) +(q1->velyatual))/(2.0);
      }
      else{
       densfacebaixo=q1->densbaixo;
       velyfacebaixo=q1->velybaixo;
       }


        dxdens=(densfacedireita  - densfaceesquerda)/(2*q1->meiolado);
        dydens=(densfacecima     - densfacebaixo)/(2*q1->meiolado);
        dxvelx=(velxfacedireita  - velxfaceesquerda)/(2*q1->meiolado);
        dyvely=(velyfacecima     - velyfacebaixo)/(2*q1->meiolado);

       div_dens_vel= (q1->velxatual)* (dxdens) +
                     (q1->densatual)* (dxvelx) +
                     (q1->densatual)* (dyvely) +
                     (q1->velyatual)* (dydens); 

       q1->densatual = (- ((div_dens_vel) * deltatempo))
                       + q1->densant;

       if(flag_write==1){
          if(q1->np==33){ 
          /*fprintf(svelx,"\ndens=%.9g",q1->densatual);*/
          }
       }
    } 
    q1=q1->next;
 }while(q1!=null);
   
}

/* *************************************************** */
/* Funcao que guarda o valor do tempo anterior         */
/* *************************************************** */
 void term_guarda_anterior()
 {
  quad quaux;
   
  quaux=gne0;
  do{
     if(quaux->ativo ==1){
       quaux->densant=quaux->densatual;
       quaux->pressant=quaux->pressatual;
       quaux->temperant= quaux->temperatual;
       quaux->velxant=quaux->velxatual;
       quaux->velyant=quaux->velyatual;
       quaux->viscant= quaux->viscatual;
       quaux->condutant=quaux->condutatual;
       quaux->energant=quaux->energatual;
      }
    quaux= quaux->next;
  } while(quaux!=null);
 }
/* ********************************************* */
/* Funcao  atualiza o valor na etapa k           */
/* ********************************************* */
 void term_atualiza_k()
 {
  quad q1;
 
  flag_converge=0; 
  q1=gne0;
  do{
    if(q1->ativo ==1){
     (q1->densk)  = (q1->densatual); 
     (q1->pressk) = (q1->pressatual); 
     (q1->visck)  = (q1->viscatual);  
     (q1->condutk)= (q1->condutatual); 
     (q1->velxk)  = (q1->velxatual); 
     (q1->velyk)  = (q1->velyatual); 
     (q1->temperk)= (q1->temperatual); 
     (q1->energk) = (q1->energatual);  
    } 
    q1=q1->next;
  }while(q1!=null);

 }

/* ********************************************* */
/* Funcao  atualiza o valor na etapa k+1         */
/* ********************************************* */
 void term_atualiza_k1()
 {
  quad q1;
 
  flag_converge=0; 
  q1=gne0;
  do{
    if(q1->ativo ==1){
     (q1->densk1)  = (q1->densatual);
     (q1->pressk1) = (q1->pressatual); 
     (q1->visck1)  = (q1->viscatual);  
     (q1->condutk1)= (q1->condutatual); 
     (q1->velxk1)  = (q1->velxatual); 
     (q1->velyk1)  = (q1->velyatual); 
     (q1->temperk1)= (q1->temperatual); 
     (q1->energk1) = (q1->energatual);  
    }
    q1=q1->next;
  }while(q1!=null);

 }

/* **************************************************   */
/* ***************************************************  */
/* Funcao verifica se a solucao convergiu               */
/* em um determinado tempo                              */
/* erro relativo :  2*|a-b| < eps * |a+b|               */
/* ***************************************************  */
 void term_converge_solucao()
 {
  quad q1;
  double aux1dens, aux2dens, aux1press, aux2press, aux1visc,
         aux2visc,aux1velx, aux2velx,aux1vely,
         aux2vely,aux1temper,aux2temper,
         aux1condut, aux2condut, aux1energ, aux2energ;
  double eps2;

  eps2 = eps* (0.5); 
  flag_converge=1; 
  q1=gne0;
  do{
     if(q1->ativo ==1){
        aux1dens = fabs(  q1->densk - q1->densk1 );
        aux2dens = fabs(  q1->densk + q1->densk1  );

        aux1press = fabs(  q1->pressk- q1->pressk1 );
        aux2press = fabs( q1->pressk + q1->pressk1 );

        aux1visc = fabs( (q1->visck- q1->visck1) );
        aux2visc = fabs( q1->visck + q1->visck1 );

        aux1condut = fabs(  q1->condutk - q1->condutk1 );
        aux2condut = fabs( q1->condutk + q1->condutk1 );

        aux1velx = fabs(  q1->velxk- q1->velxk1 );
        aux2velx = fabs( (fabs(q1->velxk) + fabs(q1->velxk1)) ) ;

        aux1vely = fabs( (q1->velyk- q1->velyk1) );
        aux2vely = fabs(( fabs(q1->velyk) + fabs(q1->velyk1)) );
 
        aux1temper = fabs(  q1->temperk- q1->temperk1 );
        aux2temper = fabs( q1->temperk + q1->temperk1 );

        aux1energ = fabs( (q1->energk- q1->energk1) );
        aux2energ = fabs( q1->energk + q1->energk1 );
     
         
         aux2dens  =  eps2 * aux2dens  ;
         aux2press =  eps2 * aux2press ;
         aux2visc  =  eps2 * aux2visc  ;
         aux2condut=  eps2 * aux2condut;
         aux2velx  =  eps2 * aux2velx  ;
         aux2vely  =  eps2 * aux2vely  ;
         aux2temper=  eps2 * aux2temper;
         aux2energ =  eps2 * aux2energ ; 
       

        if (  ( ( aux1dens   >  aux2dens )  && (aux1dens >eps) )
         ||   ( ( aux1press  >  aux2press)  && (aux1press>eps) )
         ||   ( ( aux1visc   >  aux2visc )  && (aux1visc >eps) )
         ||   ( ( aux1condut >  aux2condut) && (aux1condut>eps))
         ||   ( ( aux1velx   >  aux2velx)   && (aux1velx>eps)  )
         ||   ( ( aux1vely   >  aux2vely )  && (aux1vely>eps)  )
         ||   ( ( aux1temper >  aux2temper) && (aux1temper>eps))
         ||   ( ( aux1energ  >  aux2energ)  && (aux1energ>eps) )
          )
        {
            /*fprintf(stdout,"\nNAO CONVERGE\n");*/
           /* fprintf(stdout,"aux1dens= %.9g  ",aux1dens);
            fprintf(stdout,"aux2dens=   %.9g\n",aux2dens);
            fprintf(stdout,"aux1press=  %.9g  ",aux1press);
            fprintf(stdout,"aux2press=  %.9g\n",aux2press);
            fprintf(stdout,"aux1visc=   %.9g  ",aux1visc);
            fprintf(stdout,"aux2visc=   %.9g\n",aux2visc);
            fprintf(stdout,"aux1condut= %.9g  ",aux1condut);
            fprintf(stdout,"aux2condut= %.9g\n",aux2condut);
            fprintf(stdout,"aux1velx=   %.9g  ",aux1velx);
            fprintf(stdout,"aux2velx=   %.9g\n",aux2velx);
            fprintf(stdout,"aux1vely=   %.9g  ",aux1vely);
            fprintf(stdout,"aux2vely=   %.9g\n",aux2vely);
            fprintf(stdout,"aux1temper= %.9g  ",aux1temper);
            fprintf(stdout,"aux2temper= %.9g\n",aux2temper);
            fprintf(stdout,"aux1energ=  %.9g  ",aux1energ);
            fprintf(stdout,"aux2energ=  %.9g\n",aux2energ);*/
         
            flag_converge=0;
         }

        }
        q1=q1->next;
   }while((q1!=null) && (flag_converge==1));

 }

/* **************************************************************** */
/* FUNCAO : Coloca todos os quadr ativos para o gradiente conjugado */
/****************************************************************** */
/*void term_ativo()
 {
   quad qaux;
   qaux=gne0;
   do {
      qaux->ativo=1;
      qaux=qaux->next;
    }while(qaux!=null);
  }*/
/* **************************************************** */
/* **************************************************** */
void term_write_conta()
 {
   quad qaux;
   int contador,ndesativo;

   qaux=gne0;
   contador=0;
   ndesativo=0;
   do {
      contador=contador+1;
      if(qaux->ativo == 0){
         ndesativo=ndesativo+1;
      }
      qaux=qaux->next;
    }while(qaux!=null);
   /*fprintf(stdout,"\nNUMERO DE QUAD. = %d", contador);
   fprintf(stdout,"\nNUMERO DE QUAD. NAO ATIVOS = %d",ndesativo );*/
  }
/* *************************************************** */
/* FUNCAO: Calcula  a densidade do fluxo de energia    */
/* *************************************************** */
void term_fluxo_energia()
{
 quad q1;
 double Tdireita,Tesquerda,Tcima, Tbaixo;
 double dTx;   /* derivada da temperatura com relacao a x  */
 double dTy;   /* derivada da temperatura com relacao a y  */
 double densw; /* densidade*W  = densi *calorp * (T-Tm)    */
 double velx2; /* velocidade horizontal ao quadrado / 2    */
 q1=gne0;
 term_previzquad();
 do{
  if(q1->ativo == 1){
   cont_fluxo+=1; 
   Tdireita = (q1->temperdireita  + q1->temperatual)/2.0; 
   Tesquerda= (q1->temperesquerda + q1->temperatual)/2.0;
   dTx=(Tdireita-Tesquerda)/(2*q1->meiolado); 
   Tcima =(q1->tempercima  + q1->temperatual)/2.0;
   Tbaixo=(q1->temperbaixo + q1->temperatual)/2.0;
   dTy= (Tcima-Tbaixo)/(2*q1->meiolado);
    
   densw= q1->densatual*(calorp)*(q1->temperatual-tempermedia);
   velx2 =(q1->velxatual *q1->velxatual)/2.0;
 
   q1->hx = q1->densatual*(q1->velxatual) * velx2+
            densw*q1->velxatual-
            q1->condutatual * (dTx);

   q1->hy = q1->densatual*(q1->velyatual) * velx2+
            densw*q1->velyatual-
            q1->condutatual * (dTy);

  }
  q1=q1->next;
}while(q1!=null);

}

/* ******************************************************** */
/* FUNCAO: Calcula a media da densidade de fluxo de energia */
/* ******************************************************** */
void term_media_fluxo_energia()
{
  quad q1;

  term_fluxo_energia();
  q1=gne0;
  do {
     if(q1->ativo == 1){
       q1->hxmedia = q1->hxmedia + (q1->hx);
       q1->hymedia = q1->hymedia + (q1->hy);
     }
     q1=q1->next;
  }while(q1!=null);

   
}
/* ************************************************************* */
/* FUNCAO : Desenha o campo de vetores do fluxo de energia       */
/* ************************************************************* */
 void term_desenha_fluxo()
 {
   quad q1;
   double aux;
   double pi=3.141592654;
   double fator,hx,hy,hxn,hyn,x1,y1;
   double omega;
   int i;
   int my=484; /* 384 */
   /*FILE *deseflux;*/

   /*deseflux=fopen("deseflux","w");*/
    omega=2.0 * (3.141592654)*frequencia;

   q1=gne0;
   aux = omega/ (2*pi);
   do{
     if(q1->ativo ==1 ){
       q1->hxmedia = aux *(q1->hxmedia)/cont_fluxo;
       q1->hymedia = aux *(q1->hymedia)/cont_fluxo;
     }
     q1=q1->next;
    }while(q1!=null);

   q1=gne0;
   fator = q1->hxmedia;
   do{
     if(q1->ativo ==1){ 
       if( q1->hxmedia >fator){fator=q1->hxmedia;}
       if (q1->hymedia >fator){fator=q1->hymedia;}
     }
     q1=q1->next;
   }while(q1!=null);
   /*fprintf(stdout,"fator=%.15g\n",fator);*/
   xiplaca = 0.25; 
   xfplaca = 0.75;
   setcolor(3);
   lin(xiplaca,0.5,xfplaca,0.5);
   q1=gne0;
   do {
      if(q1->ativo == 1){
         x1=q1->coord[0];
         y1=q1->coord[1]*2.0; /* escala aumenta 2 vezes na vertical */

         /*********************************/
         hx=(q1->hxmedia)/fator;
         hy=(q1->hymedia)/fator;
         /*********************************/
         /*********************************
         Campo vetorial da energia: 
         vetor=(hxmedia,hymedia)
         *********************************/

         /**********************************
         ATENCAO: campo vetorial ortogonal !
         Traca as isolinhas de energia:
         vetor=(hymedia,-hxmedia)
         hx=(q1->hymedia)/fator;
         hy=-(q1->hxmedia)/fator;
         **********************************/

         hxn=0.01*hx/sqrt(hx*hx+hy*hy);
         hyn=0.01*hy/sqrt(hx*hx+hy*hy);
         setcolor(4);
         lin(x1,y1+0.5,x1+0.02*hx,y1+0.02*hy+0.5);
         setcolor(5);
         lin(x1,y1+0.8,x1+hxn,y1+hyn+0.8);
         setcolor(3);
         lin(x1,y1+0.5,x1,y1+0.5);
         /*fprintf(deseflux,"%.9g %.9g %.9g %.9g\n",x1,y1,hx,hy);*/
      }
      q1=q1->next;
   }while(q1!=null);
   /*fclose(deseflux);*/
} 
/* ************************************************************** */
/* FUNCAO : Chama a funcao que calcula a media do fluxo de energia */
/* ************************************************************** */  
void term_pre_fluxo_energia()
{
  quad q1;
  double aux;
  double pi=3.141592654;
  double auxtempo;
  double omega;
  FILE *sfluxo;
  q1=gne0;

  omega=2.0*pi*frequencia;
  auxtempo = (2*pi)/omega;
 
  if (flag_fluxo ==0){
     do{
        if(q1->ativo==1){
          q1->hxmedia = 0.0;
          q1->hymedia = 0.0;
        }
        q1=q1->next;
     }while(q1!=null);
     tempo_fluxo=0.0;
     flag_fluxo =1;
     cont_fluxo=0;
  }
  tempo_fluxo+=deltatempo;
  /*term_fluxo_energia();ATENCAO!!!*/
  term_media_fluxo_energia();
  sfluxo=fopen("sfluxo","w");
  /*fprintf(stdout,"\nTEMPO_FLUXO= %.9g\n",tempo_fluxo); */
  if((tempo_fluxo >= auxtempo) && (flag_fluxo == 1))
     {
      term_desenha_fluxo();
      /*fprintf(stdout,"\nDESENHO\n");*/
      flag_fluxo =0;
      q1=gne0;
      do{
       if(q1->ativo==1){
        /*fprintf(sfluxo,"/n q1->hxmedia %.9g q1->hymedia", q1->hxmedia,q1->hymedia); */
       }
      q1=q1->next;
      }while(q1!=null);
    }
    fclose(sfluxo);                                                  
}

/* *************************************************** */  
/* FUNCAO: Desativa e ativa os quadradinhos            */
/* *************************************************** */
void term_ativa()
{
  quad q1;
  int contador; 
  double ymax;

  /*q1=gne0;
  do{
     if((q1->nivel==0)||(q1->nivel==1)){
        q1->ativo=0;
      }
     else{ q1->ativo=1;}
  }while(q1!=null);*/


   q1=gne0;
   ymax=-1e30;
   do{
      if(q1->ativo==1){
         if(q1->coord[1]>ymax) {ymax =q1->coord[1];}
      }
      q1=q1->next;
  }while(q1!=null);
 
  if(ymax>(1.0/16.0)){  /* 1.0/64.0 */ 
     q1=gne0;
     do{
        if(q1->ativo == 1){
          if(q1->coord[1] <ymax) {q1->ativo =1; }
          else {q1->ativo=0;} 
        }
        q1=q1->next;
     }while(q1!=null);
  }
  else {
       q1=gne0;
       do{
          if(q1->coord[1]<=ymax) {q1->ativo=1;}
          q1=q1->next;
       }while(q1!=null);
   } 
       
}
/* *************************************************** */
/* FUNCAO: Resolve o sistema termoacustico             */
/* *************************************************** */
void term_resolve()
{
 double t;
 int etapa;
 quad q1;
 int graf;
 int write=0;
 int contador_tempo=0;
 
 deltatempo=1e-6;/*1.5e-7*/              
 deltatempomax=1e-6; /*1.5e-7*/
 tempofinal= 740.0 * deltatempomax;    
 pressmedia =auxpress*Rgas*densmedia*tempermedia;


 flagterm_inicio=0;
 term_cond_inicial();
 /*fprintf(stdout,"\nENTREI TERM_PREV\n");*/
 tempoatual=0.0;
 term_previzquad();
 /*fprintf(stdout,"\nSAI TERM_PREV\n");*/
 /*term_write_inicial();*/
 /* disproc(); */
 flag_converge=0;
 flagterm_inicio=1;
 term_predifquad();
 term_ref();
 term_cond_inicial();
 term_curvas_de_nivel(); 
 graf=0;
 tempoatual =deltatempo; 
 for(t=deltatempo;t<=tempofinal;t+=deltatempo){
    /*fprintf(stdout,"****************************"); 
    fprintf(stdout,"\nTEMPO = %.9g",tempoatual); */
    term_guarda_anterior();
    /* Enquanto a solucao no tempo (n+1) nao convergir faca */
    etapa=0;
  do{ 
      term_atualiza_k(); 
      term_continuidade();
      term_pressao();
      term_visc();
      term_condut();
      term_momentum();
      term_phi(); 
      /*if(contador_tempo>=50){*/
      term_temperatura(); 
      /*}*/
      term_energia();
      term_atualiza_k1();
      term_converge_solucao();
      etapa= etapa + 1;
      /*term_saida_3d();*/ /* provisorio */
      /*fprintf(stdout,"\n iteracao Picard = %d",etapa);*/
   }while((flag_converge==0) &&  ( etapa < 200) ) ;
   /*fprintf(stdout,"\nETAPA = %d  ", etapa); 
   fprintf(stdout,"CONVERGIU");  */
   /*term_refdes(); */
   /*term_write_conta();*/ 
   /* Desenha as variacoes de temperatura e velocidade */
       if(flag_desenho==0){
       term_curvas_de_nivel(); /* desenha temperatura */
       /*term_desenha_pressao();*/ 
       /*term_campo_vel();*/
       term_pre_fluxo_energia();
       }
       graf=0;
       /*flag_write=1;
       fprintf(svelx,"\n**************************");*/    
        /*escrever nos arquivos svelx e stemper*/
    /* } */
     
    graf=graf+1;
    /*if(graf==2){flag_write=0;}*/ 
    if(deltatempo<deltatempomax)deltatempo*=2.0;
    tempoatual=tempoatual+deltatempo;
    contador_tempo+=1;
  }


  /* saida em arquivos */  
  tempoatual=tempoatual-deltatempo;
  /*term_saida_3d();*/
  /*term_write_front();  */  
  /*fclose(sterm);  */    /*teste */
  /* fclose(sfront);*/
}   

/***************************************************** */
/***************************************************** */
 
/* *************************************************** */
/* FUNCAO: Escreve os dados em um arquivo para         */
/*          MATHEMATICA                                */
/* *************************************************** */
  void term_saida_3d()
  {
    quad q1;
    int  contador;
    q1=gne0;  
    contador=0;
   
    do{
       if(q1->ativo ==1){
       /*fprintf(sterm,"velx=%.9g  ",q1->velxatual);
       fprintf(sterm,"vely=%.9g  ",q1->velyatual);
       fprintf(sterm,"temp=%.9g  ",q1->temperatual);
       fprintf(sterm,"press=%.9g\n",q1->pressatual);*/

       /*if((q1->b->tipo =='a')||(q1->c->tipo=='a')||
          (q1->d->tipo =='a')||(q1->e->tipo=='a')){
           fprintf(sterm," FRONTEIRA");
       } */
       /* fprintf(sterm," q1->np= %d",q1->np);*/
       if(q1->placa==1){
        /* fprintf(sterm,"PLACA\n"); */
        }

      }
       contador= contador+1;
       q1=q1->next;
   }while(q1!=null);
   /*fprintf(sterm,"numero TOTAL = %d \n ",contador);*/
   /*fclose(sterm); */  /* ATENCAO */

}

/* ************************************************* */
int term_write_front()
{
quad q1;
q1=gne0;  

term_previzquad();
do{
   if(q1->ativo ==1){
    /* if(q1->e->tipo=='a'){*/
      /* fprintf(sfront,"\n np              %d",q1->np);
       fprintf(sfront,"\n velxatual      %.9g",q1->velxatual);
       fprintf(sfront,"\n velxbaixo      %.9g",q1->velxbaixo);
       fprintf(sfront,"\n velxcima       %.9g",q1->velxcima);
       fprintf(sfront,"\n velxdireita    %.9g",q1->velxdireita);
       fprintf(sfront,"\n velxesquerda   %.9g",q1->velxesquerda);
       fprintf(sfront,"\n velyatual      %.9g",q1->velyatual);
       fprintf(sfront,"\n velybaixo      %.9g",q1->velybaixo);
       fprintf(sfront,"\n velycima       %.9g",q1->velycima);
       fprintf(sfront,"\n velydireita    %.9g",q1->velydireita);
       fprintf(sfront,"\n velyesquerda   %.9g",q1->velyesquerda);
       fprintf(sfront,"\n temperatual    %.9g",q1->temperatual);
       fprintf(sfront,"\n temperbaixo    %.9g",q1->temperbaixo);
       fprintf(sfront,"\n tempercima     %.9g",q1->tempercima);
       fprintf(sfront,"\n temperdireita  %.9g",q1->temperdireita);
       fprintf(sfront,"\n temperesquerda %.9g",q1->temperesquerda);
       fprintf(sfront,"\n densatual      %.9g",q1->densatual);
       if(q1->placa==1)fprintf(sfront,"    PLACA");
       fprintf(sfront,"\n *************************"); */
   
   /*  }*/
  }
  q1=q1->next;
}while(q1!=null);


}

/* ************************************************* */
/* ************************************************* */
int term_write_inicial()
{
quad q1;
int nqdireita;
q1=gne0;
      
do{
   if(q1->ativo ==1){

       /*fprintf(scondt,"nivel= %d   ",q1->nivel);
       fprintf(scondt,"numero= %d  ",q1->np);
       fprintf(scondt,"meiolado=%.9g  ",q1->meiolado);
       fprintf(scondt,"x=%.9g   ",q1->coord[0]);
       fprintf(scondt,"y=%.9g\n",q1->coord[1]);
       fprintf(scondt,"********************************\n"); */
      }
      
  q1=q1->next;
}while(q1!=null);

 /*  nqdireita=0;
   q1=gne0;
   do{
   if(q1->ativo==1){
   if(q1->np==12){
      q1=gne0;
      do{
          if(q1->ativo==1){
             if(q1->e->e!=null){
                if(q1->e->e->np==12)
                   nqdireita=nqdireita+1;
             }
           }
        
       q1=q1->next;
       }while(q1!=null);
    }
    }
    q1=q1->next;
    }while(q1->np<=12);
   fprintf(scondt,"\n nqdireita %d ",nqdireita);
*/

/*fclose(scondt);*/

}

/* ************************************************* */
/* FUNCAO: Chama a funcao que resolve o sistema      */
/*         termoacustico                             */
/* ************************************************* */
void den_play_therm(valor)
int valor;
{

   if (flagpeano == 1) linpeano(0);
   /* Abre arquivos de saida de dados */
    /*sterm=fopen("sterm","w");  
    sfront=fopen("sfront","w");     
    scondt=fopen("scondt","w");
    svelx=fopen("svelx","w");
    aux=fopen("aux","w");
    stemper=fopen("stemper","w"); */
    term_write_conta();     

   /* scond=fopen("scond","w");       */
   clear_graf();
   term_resolve();  /* ATENCAO PROVISORIO */
   /*fclose(svelx);
   fclose(stemper);
   fclose(aux);*/
   fprintf(stdout,"\nFIM\n ");  
   if (flagpeano==1) linpeano(1);

 }  

/* *************************************************** */
/* *************************************************** */

/* *************************************************** */
/* FUNCAO: Criterio para decidir o desrefinamento      */
/* *************************************************** */
void term_condes(qref)
quad qref;
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
    /* fprintf(stdout,"\n MAIOR = %.9g \n",maior);         */
    /* fprintf(stdout,"\n COTA = %.9g \n ", cotades_term); */
     
   /* Decidi se o cacho quer ser desrefinado */
    if ((aux1==aux2) && (aux1==aux3) && (aux1==aux4)){
       if(maior<=cotades_term)
       {
        fprintf(stdout,"\nPASSEI DESREFINO\n" ); 
         term_desrefina(q1); /* term_desrefina */
         fdesrefina_term=1;  
         fdesrefina_proximo =1;
       }
   }         

}  /* fim da funcao condes  */
/* ********************************************** */
/* ********************************************** */
/* FUNCAO:  Refinamento inicial                   */
/*          Decide se refina a malha              */
/*          pecorrendo toda a curva de peano      */
/* ********************************************** */
void term_ref_inicial()
{
 quad q1,qua;
 int frefina,flagnext;
 double maior_gradiente;

 if(flagpeano==1) linpeano(0); 
 term_predifquad();
 do{       
   q1=gne0;
   frefina=0;
   do{
      flagnext=0;
      if (q1->ativo ==1){
      maior_gradiente = fabs(q1->fb);
      if((fabs(q1->fc))>(maior_gradiente))
         maior_gradiente=fabs(q1->fc);
      if((fabs(q1->fd))>(maior_gradiente))
         maior_gradiente=fabs(q1->fd);
      if((fabs(q1->fe))>(maior_gradiente))
         maior_gradiente=fabs(q1->fe);

        if((q1->nivel <6)&&(maior_gradiente >= cotaref_term)){  
       /* if((q1->nivel <7)&&(q1->difer >= cotaref_term)){ */ 
       qua=q1;
       q1=qua->next;
       term_refina(qua);
       /* qua=q1; */
       qua->b->proc=qua->proc;
       qua->b->e->proc=qua->proc;
       qua->e->proc=qua->proc;
       pixelb(qua->coord[0],qua->coord[1]); 
       pixelw(qua->coord[0],qua->coord[1],cor[qua->proc]); 
       qua=qua->e;
       pixelw(qua->coord[0],qua->coord[1],cor[qua->proc]); 
       qua=qua->b;
       pixelw(qua->coord[0],qua->coord[1],cor[qua->proc]); 
       qua=qua->d;
       pixelw(qua->coord[0],qua->coord[1],cor[qua->proc]);  
       qua=qua->c;
       frefina=1;
       flagnext=1;
      }
      }
      if (flagnext==0) {q1=q1->next; }  
   }while(q1!=null); 

  }while(frefina); 

 if(flagpeano==1) linpeano(1); 
 /*fprintf(stdout,"\n refina OK "); */

}
/* ************************************************** */

/* ************************************************** */
/* FUNCAO: Decide se refina a malha                   */
/*         pecorrendo toda a curva de peano           */
/* ************************************************** */
void term_ref()
{
quad q1,qua;
int frefina,flagnext;
double maior_gradiente;

if(flagpeano==1) linpeano(0); 
do{   /* do para o refinamento varias vezes,  ate ser satisfeito */
   term_predifquad();      
   q1=gne0;
   frefina=0;
   do{
     flagnext=0;
     if(q1->ativo==1){
       maior_gradiente = fabs(q1->fb);
       if((fabs(q1->fc))>(maior_gradiente))
          maior_gradiente=fabs(q1->fc);
       if((fabs(q1->fd))>(maior_gradiente))
          maior_gradiente=fabs(q1->fd);
       if((fabs(q1->fe))>(maior_gradiente))
           maior_gradiente=fabs(q1->fe);

       if((q1->nivel <6)&&(maior_gradiente >= cotaref_term)){  
        /* if((q1->nivel <7)&&(q1->difer >= cotaref_term)){ */ 
          qua=q1;
          q1=qua->next;
          term_refina(qua);
          /* qua=q1; */
          qua->b->proc=qua->proc;
          qua->b->e->proc=qua->proc;
          qua->e->proc=qua->proc;
          pixelb(qua->coord[0],qua->coord[1]); 
          pixelw(qua->coord[0],qua->coord[1],cor[qua->proc]); 
          qua=qua->e;
          pixelw(qua->coord[0],qua->coord[1],cor[qua->proc]); 
          qua=qua->b;
          pixelw(qua->coord[0],qua->coord[1],cor[qua->proc]); 
          qua=qua->d;
          pixelw(qua->coord[0],qua->coord[1],cor[qua->proc]);  
          qua=qua->c;
          frefina=1;
          flagnext=1;
        }
     }
    if (flagnext==0) {q1=q1->next; }
    }while(q1!=null); 
  }while(frefina);  

 if(flagpeano==1) linpeano(1); 
 /*fprintf(stdout,"\n refina OK  "); */

}
/* ************************************************** */
/* FUNCAO : Decide se desrefina a malha               */
/*          pecorrendo toda a curva de peano          */
/* ************************************************** */
void term_desref()
{
quad qauxa,qajuda;
int  nivel;

if(flagpeano==1) linpeano(0); 
/* do{ */    /* do do fdesrefina */
    fdesrefina_term=0; 
     term_predifquad();  /* teste 07/03/98 */    
    /*term_predifer(); */ /* teste 20/02/98 */   
    qauxa=gne0;
    do{
      fdesrefina_proximo= 0; 
      if(qauxa->ativo == 1){      
      nivel=qauxa->nivel;
      if(qauxa->nivel>4) {
         if (qauxa->next!=null){
           if ( qauxa->next->next!=null){
            if (qauxa->next->next->next!=null){
                 qajuda = qauxa ->next->next->next->next; 
              if((nivel==qauxa->next->nivel)&&
              (nivel==qauxa->next->next->nivel)&&
              (nivel==qauxa->next->next->next->nivel)){
                   term_condes(qauxa);
                   
       }
     }
    }
   }
  }
    /*qauxa=qauxa->next;*/
   if (fdesrefina_proximo ==0) qauxa=qauxa->next;
   } 
   else if (fdesrefina_proximo == 1) qauxa= qajuda; 
  } while(qauxa!=null);
/* }while(fdesrefina_term); */   /* while do fdesrefina */ 

if (flagpeano == 1) linpeano(1);
/*fprintf(stdout," Desrefina OK \n "); */

}

/* ******************************************************* */
/* FUNCAO : REFINA E DESREFINA                             */
/* ******************************************************* */
void term_refdes()
{
   term_ref();
   precalcviz();
   term_desref();        

}
    
/* ******************************************************* */

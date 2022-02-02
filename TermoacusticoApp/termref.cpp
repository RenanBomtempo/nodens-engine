
/*******************************************************/
/* PROGRAMA: termref.c                                  */
/* AUTORA  : DENISE BURGARELLI DUCZMAL                 */
/* DATA    : 97/02/06                                  */
/*******************************************************/

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "funcoes.h"
#include "quad.h" 

#define null 0

/******** variaveis globais para este programa ***** */




extern int nq;
extern quad pq[16];  /*primeiro quad. da curva peano para cada proce. */
extern int nquad[16];/*guarda o numero de quad. em cada processador   */


/* ************Variaveis externas******************* */
extern int flagglobal;
extern int flagproblem;
extern int  flagpeano;
extern int flagonda_inicio;




/************************************************************* */
/* Refina cada quadr.e  cuida da distribuicao otima dos        */
/*  processadores                                              */
/* Para o refinamento sao feitos os seguintes passos  :        */
/* 1: Criacao das arvores                                      */
/* 2: ne, o quadradinho a ser refinado passa a ser o quad.     */
/*     na posicao                                              */
/*     nordeste e criamos outros quadradinhos deste cacho      */
/* 3: decidi qual a nova configuracao da curva de peano        */
/* *********************************************************** */

void term_refina(quad ne)
{
 quad q0,q1,q2,qaux,qajuda;
 double a,a2,cx,cy,po2,cxatual,cxanterior,usoma_de_ne;
 double uanteriorp,uatualizadop;
 double uanteriorn,uatualizadon;
 extern double lado;
 int i,j,n,n0,n1,ni,n2,po,popo,k,q1p,n3,n4,cont;
 int ativo_herdado;

 double    densatual     ;
 double    densant       ;
 double    viscatual     ;
 double    viscant       ;
 double    pressatual    ;
 double    pressant      ;
 double    temperatual   ;
 double    temperant     ;
 double    condutatual   ;
 double    condutant     ;
 double    velxatual     ;
 double    velxant       ;
 double    velyatual     ; 
 double    velyant       ;

 /* Os valores abaixo  devem ser                    */
 /* distribuidos para os novos quadradinhos         */
 /* de acordo com o tipo de equacao a ser resolvida */
 /* cxatual: valor a ser distribuido para os novos  */
 /*         quadradinhos, este valor e a solucao    */
 /*         da matriz Ax=b por Laplace ou calor     */
 if ((flagproblem == 0) || (flagproblem ==1 ) ){
 cxatual=ne->cx; 
 }
 /* cxanterior: solucao no tempo anterior  para a   */
 /*             equacao do calor                    */
 cxanterior=ne->cxant;
 /* valor calculado para as duas eq. da onda        */
 if (flagproblem == 2){
 usoma_de_ne  = ne->usoma;
 uanteriorp   = ne->uantp;
 uatualizadop = ne->uatualp;
 uanteriorn   = ne->uantn;
 uatualizadon = ne->uatualn;
}
 if (flagproblem ==3 ){
    /* termoacustico */
    densatual   = ne->densatual;
    densant     = ne->densant;
    viscatual   = ne->viscatual;
    viscant     = ne->viscant;
    pressatual  = ne->pressatual;
    pressant    = ne->pressant;
    temperatual = ne->temperatual;
    temperant   = ne->temperant;
    condutatual = ne->condutatual;
    condutant   = ne->condutant;
    velxatual   = ne->velxatual;
    velxant     = ne->velxant;
    velyatual   = ne->velyatual;
    velyant     = ne->velyant;
 } 
/* ********************************* */
 ativo_herdado= ne->ativo;        

 j=1;
 for(i=0;i<ne->nivel;i++){
     j*=2;
 }
 po=j;
 po2=po*2.0;
 popo=po*po;
 (ne->nivel)=(ne->nivel+1);
  (ne->ativo)= ativo_herdado;  /* Ja foi feito */
 /*  ncon : identifica os quad. com relacao a serem   */
 /*         filhos do mesmo pai */
 (ne->ncon)= ((ne->ncon)*10)+1;
 /* npeapai: guarda o numero de peano do quad. pai    */
 /*           que esta sendo refinado                 */
  ne->npeapai=ne->pea;

/* Calcula o centro (cx,cy) do quadrado a ser refinado. */
 a=1.0/po2;
 a2=a/2.0;
 cx=(ne->coord[0]);
 cy=(ne->coord[1]);
 setColor(5);
 drawLine(cx-a,cy,cx+a,cy); 
 drawLine(cx,cy-a,cx,cy+a); 

/* Aqui comeca a criacao dos nodos arvores   */
 /* Cria e liga o nodo arvore `a esquerda. */
 qaux = (quad)malloc(sizeof(pquad));
 (qaux->tipo)= 'a';
 (qaux->nivel)=(ne->nivel);
 (qaux->ativo)=(ne->ativo); 
 (qaux->b) =( ne->e);
 if((ne->e->b)==ne)(ne->e->b)=qaux;
 else if((ne->e->c)==ne)(ne->e->c)=qaux;
 else if((ne->e->d)==ne)(ne->e->d)=qaux;
 else if((ne->e->e==ne))(ne->e->e)=qaux;
 (ne->e )= qaux;
 /* Cria e liga o nodo arvore `a esquerda. */

 /* Cria e liga o nodo arvore acima. */
 qaux = (quad)malloc(sizeof(pquad));
 (qaux->tipo)= 'a';
 (qaux->nivel)=(ne->nivel);
 (qaux->ativo)=(ne->ativo);
 (qaux->b) = (ne->c);
 if((ne->c->b)==ne)(ne->c->b)=qaux;
 else if((ne->c->c)==ne)(ne->c->c)=qaux;
 else if((ne->c->d)==ne)(ne->c->d)=qaux;
 else if((ne->c->e)==ne)(ne->c->e)=qaux;
 (ne->c) = qaux;
 /* Cria e liga o nodo arvore acima. */

 /* Cria e liga o nodo arvore `a direita. */
 qaux = (quad)malloc(sizeof(pquad));
 (qaux->tipo)= 'a';
 (qaux->nivel)=(ne->nivel);
 (qaux->ativo)=(ne->ativo);
 (qaux->b) = (ne->d);
 if((ne->d->b)==ne)(ne->d->b)=qaux;
 else if((ne->d->c)==ne)(ne->d->c)=qaux;
 else if((ne->d->d)==ne)(ne->d->d)=qaux;
 else if((ne->d->e)==ne)(ne->d->e)=qaux;
 (ne->d) = qaux;

 /* Cria e liga o nodo arvore abaixo. */
 qaux = (quad)malloc(sizeof(pquad));
 (qaux->tipo)= 'a';
 (qaux->nivel)=(ne->nivel);
 (qaux->ativo)=(ne->ativo);
 (qaux->b) = (ne->b);
 if((ne->b->b)==ne)(ne->b->b)=qaux;
 else if((ne->b->c)==ne)(ne->b->c)=qaux;
 else if((ne->b->d)==ne)(ne->b->d)=qaux;
 else if((ne->b->e)==ne)(ne->b->e)=qaux;
 (ne->b) = qaux;

 /* Criacao dos quadradinhos  */
 /* Calcula o centro do quadradinho refinado nordeste. */
 (ne->coord[0])=cx+a2;
 (ne->coord[1])=cy+a2;
 (ne->f)=fxy(ne->coord[0],ne->coord[1]);
 (ne->cx)=cxatual;
 (ne->cxant)=cxanterior;
 (ne->difer)=0;  /* Eu acho que isto nao deve ser colocado */

 if (flagproblem == 2 ){
 if(flagonda_inicio==1){
 /* Herda o valores calculados antes de refinar o quadr. */
    ne->usoma=usoma_de_ne;
    ne->uantp= uanteriorp;
    ne->uatualp=uatualizadop;
    ne->uantn=uanteriorn;
    ne->uatualn=uatualizadon;

 }
 else {
          
       qajuda=ne; 
       atualiza_inicio_onda(qajuda); 
       ne=qajuda; 
     

  }
}
 if (flagproblem == 3 ){
     
    ne->densatual  = densatual;
    ne->densant    = densant;
    ne->viscatual  = viscatual;
    ne->viscant    =viscant;
    ne->pressatual = pressatual;
    ne->pressant   = pressant;
    ne->temperatual= temperatual;
    ne->temperant  = temperant;
    ne->condutatual= condutatual;
    ne->condutant  = condutant;
    ne->velxatual  = velxatual;
    ne->velxant    = velxant;
    ne->velyatual  = velyatual;
    ne->velyant    =velyant;


  }

/* Criacao do nodo noroeste                   */
/* Cria o novo nodo, liga, e calcula o centro */
/* do quadradinho refinado noroeste.          */
 qaux = (quad)malloc(sizeof(pquad));
 (qaux->tipo)='q';
 (qaux->nivel)=(ne->nivel);
 (qaux->ativo)=(ne->ativo);
 (qaux->ncon)=(ne->ncon)+1;
 qaux->e = ne->e;
 ne->e = qaux;
/*Herda cx calculado antes de refinar o quadr. */
 ne->e->cx = cxatual;
 ne->e->cxant=cxanterior;
 ne->e->coord[0]=cx-a2;
 ne->e->coord[1]=cy+a2;
 ne->e->f=fxy(ne->e->coord[0],ne->e->coord[1]);
 ne->e->difer=0;

 if (flagproblem == 2){
 if(flagonda_inicio==1){
/*Herda usoma calculado antes de refinar o quad.*/
    ne->e->usoma=usoma_de_ne;
    ne->e->uantp= uanteriorp;
    ne->e->uatualp=uatualizadop;
    ne->e->uantn=uanteriorn;
    ne->e->uatualn=uatualizadon;
    
 }
 else {
          
     qajuda=ne->e;
     atualiza_inicio_onda(qajuda);
     ne->e=qajuda;
 }
}
if (flagproblem == 3 ){

    ne->e->densatual  = densatual;
    ne->e->densant    = densant;
    ne->e->viscatual  = viscatual;
    ne->e->viscant    = viscant;
    ne->e->pressatual = pressatual;
    ne->e->pressant   = pressant;
    ne->e->temperatual= temperatual;
    ne->e->temperant  = temperant;
    ne->e->condutatual= condutatual;
    ne->e->condutant  = condutant;
    ne->e->velxatual  = velxatual;
    ne->e->velxant    = velxant;
    ne->e->velyatual  = velyatual;
    ne->e->velyant    = velyant;
}
/* Criacao do nodo sudoeste                   */
/* Cria o novo nodo, liga, e calcula o centro */
/* do quadradinho refinado sudoeste.          */
 qaux = (quad)malloc(sizeof(pquad));
 qaux->tipo='q';
 qaux->nivel=ne->nivel;
 qaux->ativo=ne->ativo;
 qaux->ncon=(ne->ncon)+2;
 qaux->b = ne->b;
 ne->e->b = qaux;
/* Herda  cx calculado antes de refinar o quadr. */  
 ne->e->b->cx = cxatual;
 ne->e->b->cxant = cxanterior;
 ne->e->b->coord[0]=cx-a2;
 ne->e->b->coord[1]=cy-a2;
 ne->e->b->f=fxy(ne->e->b->coord[0],ne->e->b->coord[1]);
 ne->e->b->difer=0;

if ( flagproblem == 2 ){
if(flagonda_inicio==1){
/* Herda  usoma calculado antes de refinar o quadr. */  
   ne->e->b->usoma = usoma_de_ne;
   ne->e->b->uantp= uanteriorp;
   ne->e->b->uatualp=uatualizadop;
   ne->e->b->uantn=uanteriorn;
   ne->e->b->uatualn=uatualizadon;
}
else { 
      qajuda=ne->e->b;
      atualiza_inicio_onda(qajuda);
      ne->e->b=qajuda;
} 

}

if (flagproblem == 3 ){

    ne->e->b->densatual  = densatual;
    ne->e->b->densant    = densant;
    ne->e->b->viscatual  = viscatual;
    ne->e->b->viscant    = viscant;
    ne->e->b->pressatual = pressatual;
    ne->e->b->pressant   = pressant;
    ne->e->b->temperatual= temperatual;
    ne->e->b->temperant  = temperant;
    ne->e->b->condutatual= condutatual;
    ne->e->b->condutant  = condutant;
    ne->e->b->velxatual  = velxatual;
    ne->e->b->velxant    = velxant;
    ne->e->b->velyatual  = velyatual;
    ne->e->b->velyant    = velyant;
}
/* criacao do nodo sudeste                    */
/* Cria o novo nodo, liga, e calcula o centro */
/* do quadradinho refinado sudeste.           */
 qaux = (quad)malloc(sizeof(pquad));
 qaux->tipo='q';
 qaux->nivel=ne->nivel;
 qaux->ativo=ne->ativo;
 qaux->ncon=(ne->ncon)+3;
 qaux->d = ne->d;
 ne->e->b->d = qaux;
/* Herda  cx calculado antes de refinar o quadr. */  
 ne->e->b->d->cx = cxatual;
 ne->e->b->d->cxant = cxanterior;
 qaux->c = ne;
 ne->e->b->d->coord[0]=cx+a2;
 ne->e->b->d->coord[1]=cy-a2;
 ne->e->b->d->f=
 fxy(ne->e->b->d->coord[0],ne->e->b->d->coord[1]);
 ne->e->b->d->difer=0;

 if ( flagproblem == 2){
 if(flagonda_inicio==1){ 
/* Herda  usoma calculado antes de refinar o quadr. */  
 ne->e->b->d->usoma = usoma_de_ne;
 ne->e->b->d->uantp= uanteriorp;
 ne->e->b->d->uatualp=uatualizadop;
 ne->e->b->d->uantn=uanteriorn;
 ne->e->b->d->uatualn=uatualizadon;
 
 } 
 else{
         qajuda=ne->e->b->d;
         atualiza_inicio_onda(qajuda);    
         ne->e->b->d=qajuda;
 }

}
if (flagproblem == 3) {

    ne->e->b->d->densatual  = densatual;
    ne->e->b->d->densant    = densant;
    ne->e->b->d->viscatual  = viscatual;
    ne->e->b->d->viscant    =viscant;
    ne->e->b->d->pressatual = pressatual;
    ne->e->b->d->pressant   = pressant;
    ne->e->b->d->temperatual= temperatual;
    ne->e->b->d->temperant  = temperant;
    ne->e->b->d->condutatual= condutatual;
    ne->e->b->d->condutant  = condutant;
    ne->e->b->d->velxatual  = velxatual;
    ne->e->b->d->velxant    = velxant;
    ne->e->b->d->velyatual  = velyatual;
    ne->e->b->d->velyant    =velyant;

}
/* **************************************************** */
 /* Completa a ligacao dos nodos quadradinhos entre si. */
 ne->b = ne->e->b->d;
 ne->e->d = ne;
 ne->e->b->c = ne->e;
 ne->e->b->d->e = ne->e->b;
 ne->e->c = ne->c;
 ne->e->b->e = ne->e->e;
 ne->e->b->d->b = ne->e->b->b;
 /* *************************************** */

/*Liga o nodo arvore cima aos quad. noroeste e nordeste*/
 ne->c->c = ne->e;
 ne->c->d = ne;
/* **************************************** */

/*Liga o nodo arvore esquerda aos quad. sudoeste e noroeste*/
 ne->e->e->c = ne->e->b;
 ne->e->e->d = ne->e;

/* **************************************** */
/*Liga o nodo arvore baixo aos quad. sudoeste e sudeste*/
 ne->e->b->b->c = ne->e->b;
 ne->e->b->b->d = ne->e->b->d;

/* **************************************** */
/*Liga o nodo arvore direita aos quad. sudeste e nordeste*/
 ne->d->c = ne->b;
 ne->d->d = ne;

/* determina qual o formato da curva de peano*/
/*  que liga estes quad.                     */
 n0=ne->pea;
 n2=n0;
 ni=ne->nivel;
 n1=peano(n2,ni);

 if(n1==0){
   q1=ne;
   ne->pea=n0;
   ne->e->pea=n0+popo;
   ne->e->b->pea=n0+2*popo;
   ne->b->pea=n0+3*popo;

   ne->e->next=ne->e->b;
   ne->e->b->next=ne->b;
   ne->b->next=ne->next;
   ne->next=ne->e;

   ne->e->prev=ne;
   ne->e->b->prev=ne->e;
   ne->b->prev=ne->b->e;
   if(ne->b->next!=null) {
   ne->b->next->prev=ne->b;
   }
 }
 else
 if(n1==1){

   q1=ne;
   ne->pea=n0;
   ne->b->pea=n0+popo;
   ne->b->e->pea=n0+2*popo;
   ne->e->pea=n0+3*popo;

   ne->b->next=ne->b->e;
   ne->b->e->next=ne->e;
   ne->e->next=ne->next;
   ne->next=ne->b;

   ne->b->prev=ne;
   ne->b->e->prev=ne->b;
   ne->e->prev=ne->b->e;
   if(ne->e->next!=null){
   ne->e->next->prev=ne->e;
   }
 }

 else
 if(n1==2){
   q1=ne->e->b;
   if(ne==pq[ne->proc])pq[ne->proc]=q1;
   ne->e->b->pea=n0;
   ne->b->pea=n0+popo;
   ne->pea=n0+2*popo;
   ne->e->pea=n0+3*popo;

   if(ne->prev!=null) 
      ne->prev->next=ne->e->b;
   ne->e->b->next=ne->b;
   ne->b->next=ne;
   ne->e->next=ne->next;
   ne->next=ne->e;

   ne->e->prev=ne;
   ne->b->prev=ne->b->e;
   ne->b->e->prev=ne->prev;
   ne->prev=ne->b;
   if(ne->e->next!=null){
   ne->e->next->prev=ne->e;
   }
 }

 else
 if(n1==3){
   q1=ne->e->b;
   if(ne==pq[ne->proc])pq[ne->proc]=q1;
   ne->e->b->pea=n0;
   ne->e->pea=n0+popo;
   ne->pea=n0+2*popo;
   ne->b->pea=n0+3*popo;

   ne->b->next=ne->next;
   ne->e->next=ne;
   ne->e->b->next=ne->e;
   ne->e->b->prev=ne->prev;
   ne->e->b->prev->next=ne->e->b;

   ne->b->prev=ne;
   ne->e->prev=ne->e->b;
   if (ne->next!=null){
   ne->next->prev=ne->b;
   }   
   ne->next=ne->b; 
   ne->prev=ne->e;

 }

/* Colore o centro do quadradinho de acordo com a cor */
/* do seu processador , observe que q1 e o primeiro   */
/* quadradinho do pedaco da curva de peano , apos     */
/*  este refinamento  . O quad. q1 foi obtido na      */
/* etapa acima                                        */
 nq+=3;
 q1p=ne->proc;  
 drawE(q1->coord[0],q1->coord[1],q1p); 
 q1=q1->next;
 q1->proc=q1p;
 drawE(q1->coord[0],q1->coord[1],q1->proc); 
 q1=q1->next;
 q1->proc=q1p;
 drawE(q1->coord[0],q1->coord[1],q1->proc); 
 q1=q1->next;
 q1->proc=q1p;
 drawE(q1->coord[0],q1->coord[1],q1->proc); 
 nquad[q1p]+=3;   




/* Caso seja necessario, faz simplificacao. */
 q0=ne->d;
 simplif(q0);

 q0=ne->c;
 simplif(q0);

 q0=ne->e->e;
 simplif(q0);

 q0=ne->b->b;
 simplif(q0);
/* Chamada  que distribui os quad. para os processadores */

} /* fim da  funcao refina */
/* **************************************************** */
/* **************************************************** */

/* *******************************************************/
/* Funcao de desrefinamento do quadradinho de acordo com */
/* um criterio de desrefinamento                         */
/* ***************************************************** */

void term_desrefina(quad qref)
{
quad qa0,qa1,qa2,qa3;        /* auxiliares                 */
quad cbaux,ccaux,cdaux,ceaux; /* arvores em volta do quad. */
quad ned;    /*quad. do nordeste do cacho a ser desrefinado*/
quad qnext,qprev;/* quad. posterior e anterior a ned       */
quad qajuda;
int  cnivel;     /* nivel das arvores em volta do quad.    */
int flagcon;     /* saber se os viz. sao arvores e/ou quad */
double sa0,sa1,sa2,sa3;
double maior;
int menor;
double pos,xaux,yaux;
double cxatual,cxanterior,usomaant,uanteriorp,uanteriorn,
       uatualizadop,uatualizadon;
int processador; /* numero do proc. do quad. apos desref. */
double    densatual     ;
double    densant       ;
double    viscatual     ;
double    viscant       ;
double    pressatual    ;
double    pressant      ;
double    temperatual   ;
double    temperant     ;
double    condutatual   ;
double    condutant     ;
double    velxatual     ;
double    velxant       ;
double    velyatual     ;
double    velyant       ;



            
nq-=3;
/* ******************************************************* */
/* Primeira etapa:nodo nordeste do cacho a ser desrefinado */
/* ******************************************************* */

 qnext=qref->next->next->next->next;
 qprev=qref->prev;
 cxatual=qref->cx;
 cxanterior=qref->cxant;

 processador = qref->proc;
 if (flagproblem == 2){
 usomaant=qref->usoma;
 uatualizadop=qref->uatualp;
 uatualizadon=qref->uatualn;
 uanteriorp=qref->uantp;
 uanteriorn=qref->uantn;
 /* processador=qref->proc; */
 }
if (flagproblem == 3 ) {

    densatual     = qref->densatual;
    densant       = qref->densant;
    viscatual     = qref->viscatual;
    viscant       = qref->viscant;
    pressatual    = qref->pressatual;
    pressant      = qref->pressant;
    temperatual   = qref->temperatual;
    temperant     = qref->temperant;
    condutatual   = qref->condutatual;
    condutant     = qref->condutant;
    velxatual     = qref->velxatual;
    velxant       = qref->velxant;
    velyatual     = qref->velyatual;
    velyant       = qref->velyant;

}
 qa0=qref;
 qa1=qa0->next;
 qa2=qa1->next;
 qa3=qa2->next;
 
/* diminui dos processadores um quadradinho */
if (nquad[qa0->proc]>1)  {
    nquad[qa0->proc]--;
}
if (nquad[qa1->proc]>1)  {
    nquad[qa1->proc]--;
}
if (nquad[qa2->proc]>1)  {
    nquad[qa2->proc]--;
}
if (nquad[qa3->proc]>1)  {
    nquad[qa3->proc]--;
}

 /* Apagar graficamente os quadradinhos do cacho */
 drawE(qa0->coord[0],qa0->coord[1],7); 
 drawE(qa1->coord[0],qa1->coord[1],7); 
 drawE(qa2->coord[0],qa2->coord[1],7); 
 drawE(qa3->coord[0],qa3->coord[1],7); 


/* Calculo do novo numero de peano */
menor=qa0->pea;
if(qa1->pea<menor) menor=qa1->pea;
if(qa2->pea<menor) menor=qa2->pea;
if(qa3->pea<menor) menor=qa3->pea;


/*Coloca em ned o quad. da posicao nordeste do cacho desref*/
 sa0=(qa0->coord[0]) + (qa0->coord[1]);
 sa1=(qa1->coord[0]) + (qa1->coord[1]);    
 sa2=(qa2->coord[0]) + (qa2->coord[1]);
 sa3=(qa3->coord[0]) + (qa3->coord[1]);

 maior= sa0;
 ned = qa0;
 if(sa1>maior)  { maior=sa1; ned=qa1; }
 if(sa2>maior)  { maior=sa2; ned=qa2; }
 if(sa3>maior)  { maior=sa3; ned=qa3; }

 /******************************************************/
 /* Segunda etapa: preechimento dos ponteiros de ned   */
 /* ****************************************************/
 /* coordenadas de ned  */
 xaux=ned->coord[0];
 yaux=ned->e->coord[0];
 pos=xaux- yaux;
 ned->coord[0] =((ned->coord[0]) + (ned->e->coord[0])) / 2.0;
 ned->coord[1] =((ned->coord[1]) + (ned->b->coord[1])) / 2.0;

 /* quad. anterior e posterior ao quad. ned */
 ned->prev=qprev;
 ned->next=qnext;
 if (ned->prev!=null) ned->prev->next=ned;
 if (ned->next!=null) ned->next->prev=ned;

/*coloca um quad. no numero de quad. do proc. de ned */
 ned->proc=processador;
 nquad[ned->proc]+=1;

/*Atualiza o vetor pq que guarda o 1_o quad. de cada quad. */
 if (ned->prev==null) pq[ned->proc]=ned;
 if (ned->prev!=null){
    if(ned->proc!=ned->prev->proc)  pq[ned->proc]=ned;
 }

 /* Apaga as linhas do quadradinho a ser desrefinado */
 xaux=ned->coord[0];
 yaux=ned->coord[1];
 setColor(7);
 drawLine(xaux,yaux+pos,xaux,yaux-pos);
 setColor(7); 
 drawLine(xaux+pos,yaux,xaux-pos,yaux); 

 /* nivel de ned */
 ned->nivel=(ned->nivel)-1;

 /* Identificador de cacho  */   
 ned->ncon= ((ned->ncon)/10);

/* Atualiza o numero de peano do quad. ned */
 ned->pea= menor; 
 
/*Atualiza ned->f, ned->cx,ned->usoma */
 ned->f= fxy(ned->coord[0],ned->coord[1]);
 ned->cx=cxatual;
 ned->cxant=cxanterior;

/* Atualiza usoma */
if (flagproblem == 2 ){
if(flagonda_inicio==1){
   ned->usoma = usomaant;
 ned->uatualp=uatualizadop;
 ned->uatualn=uatualizadon;
 ned->uantp=uanteriorp;
 ned->uantn=uanteriorn;
 processador=qref->proc;

}
else { 
      qajuda=ned;
      atualiza_inicio_onda(qajuda);
      ned=qajuda;
} 
}

if (flagproblem == 3){
    ned->densatual= densatual;
    ned->densant= densant;
    ned->viscatual= viscatual;
    ned->viscant=viscant;
    ned->pressatual= pressatual;
    ned->pressant= pressant;
    ned->temperatual= temperatual;
    ned->temperant= temperant;
    ned->condutatual= condutatual;
    ned->condutant= condutant;
    ned->velxatual= velxatual;
    ned->velxant= velxant;
    ned->velyatual= velyatual;
    ned->velyant=velyant;


}
 /* Herda os valores abaixo do quad. do nordeste do     */
 /* cacho a ser desrefinado                             */
 /* ned->difer=0.0;
 ned->fb=0.0;
 ned->fc=0.0;
 ned->fd=0.0;
 ned->fe=0.0;
 ned->cb=0.0;
 ned->cg=0.0;
 ned->cg1=0.0;
 ned->ch=0.0;
 ned->ch1=0.0;
 ned->cz=0.0;  */

 /* ***************************************************** */
 /*Terceira etapa: Reorganizacao do novo nodo             */
 /* ***************************************************** */

 /*Criacao de 4 nodos arvores para reorganizar o novo nodo*/
 cbaux=(quad)malloc(sizeof(pquad));
 ccaux=(quad)malloc(sizeof(pquad));
 cdaux=(quad)malloc(sizeof(pquad));
 ceaux=(quad)malloc(sizeof(pquad));

 cbaux->b=ned;
 ccaux->b=ned;
 cdaux->b=ned;
 ceaux->b=ned;
  
 cbaux->tipo ='a';
 ccaux->tipo ='a';
 cdaux->tipo ='a';
 ceaux->tipo ='a';
   

 cbaux->nivel=(ned->nivel)+1;
 ccaux->nivel=(ned->nivel)+1;
 cdaux->nivel=(ned->nivel)+1;
 ceaux->nivel=(ned->nivel)+1;

 cnivel=cbaux->nivel;
  
 cbaux->c=ned->b->e->b;
 cbaux->d=ned->b->b;
 ccaux->c=ned->e->c;
 ccaux->d=ned->c;
 cdaux->c=ned->d;
 cdaux->d=ned->b->d;
 ceaux->c=ned->e->e;
 ceaux->d=ned->e->b->e;

 /*eliminacao dos nodos do cacho que esta sendo desref.  */
 free(ned->b->e);
 free(ned->b);
 free(ned->e);

 /* ne aponta para os nodos arvores criados */
 ned->b=cbaux;
 ned->c=ccaux;
 ned->d=cdaux;
 ned->e=ceaux;

/* ******************* conector cbaux ******************* */
flagcon=0;
/*  Se os  viz. forem do tipo arvore do mesmo nivel    */
if ( (cbaux->c->tipo == 'a') && (cbaux->d->tipo == 'a') )
 {
 if ((cbaux->c->nivel==cnivel) && (cbaux->d->nivel==cnivel))
    {
      ned->b=cbaux->c->b;
      if(cbaux->c->b->tipo == 'q')
         cbaux->c->b->c=ned; 
      if(cbaux->c->b->tipo == 'a'){
         if(cbaux->c->b->d==cbaux->c)
            cbaux->c->b->d=ned;
         if(cbaux->c->b->c==cbaux->c)
            cbaux->c->b->c=ned;
      }

      free(cbaux->c);
      free(cbaux);
      flagcon=1;
    }
 }

/* **************** */
if (flagcon==0){    
/* Se os viz. forem do tipo arvore de nivel dif. de cbaux */
 if (((cbaux->c->nivel)>cnivel) && (cbaux->c->tipo=='a')) {
      cbaux->c->b=cbaux;}
 if (((cbaux->d->nivel)>cnivel) && (cbaux->d->tipo=='a'))
    { cbaux->d->b=cbaux;}

/* Se os nodos vizinhos forem do tipo quadradinho */
 if (cbaux->c->tipo == 'q') cbaux->c->c=cbaux;
 if (cbaux->d->tipo == 'q') cbaux->d->c=cbaux;
}

/* ******* conector ccaux ******************************* */
 flagcon=0;
/* Se os nodos viz. forem do tipo arvore do mesmo nivel */
if ((ccaux->c->tipo == 'a') && (ccaux->d->tipo == 'a'))
 {
 if ((ccaux->c->nivel==cnivel) && (ccaux->d->nivel==cnivel))
    {
       ned->c=ccaux->c->b;
       if(ccaux->c->b->tipo=='q')
          ccaux->c->b->b=ned;  
       if(ccaux->c->b->tipo == 'a'){
          if(ccaux->c->b->d==ccaux->c)
             ccaux->c->b->d=ned;
          if(ccaux->c->b->c==ccaux->c)
             ccaux->c->b->c=ned;
       }
       free(ccaux->c); 
       free(ccaux);
       flagcon=1; 
   }
  }
/* *********** */

 if  (flagcon==0){
/* Se os vizinhos forem do tipo arvore de nivel diferente */
   if ((ccaux->c->tipo=='a') && ( ccaux->c->nivel>cnivel)) {
        ccaux->c->b=ccaux;
    }
   if ((ccaux->d->tipo=='a') && ( ccaux->d->nivel>cnivel)) {
      ccaux->d->b=ccaux;
    }

  /* Se os vizinhos forem do tipo quadradinho */
    if (ccaux->c->tipo == 'q') ccaux->c->b=ccaux;
    if (ccaux->d->tipo == 'q') ccaux->d->b=ccaux;
 } 

/* ************** conector cdaux ************************ */
 flagcon=0;
if ( (cdaux->c->tipo == 'a') && (cdaux->d->tipo == 'a') )
 {
 if (( cdaux->c->nivel==cnivel) && (cdaux->d->nivel==cnivel))
  {
       ned->d=cdaux->c->b;
       if(cdaux->c->b->tipo=='q')
          cdaux->c->b->e=ned;
       if(cdaux->c->b->tipo == 'a'){
          if(cdaux->c->b->d==cdaux->c)
             cdaux->c->b->d=ned;
          if(cdaux->c->b->c==cdaux->c)
             cdaux->c->b->c=ned;
        }
        free(cdaux->c); 
        free(cdaux);
        flagcon=1;
  }
}

/* *************** */
 if (flagcon==0){
/* Se os vizinhos forem do tipo arvore de nivel diferente */
   if ((cdaux->c->tipo=='a')&& ( cdaux->c->nivel>cnivel)) {
       cdaux->c->b=cdaux;
    }

    if ((cdaux->d->tipo=='a') && (cdaux->d->nivel>cnivel)) {
        cdaux->d->b=cdaux;
    }

   /* Se os vizinhos forem do tipo quadradinho */
    if (cdaux->c->tipo == 'q') cdaux->c->e=cdaux;
    if (cdaux->d->tipo == 'q') cdaux->d->e=cdaux;
 }

/* **************conector ceaux ********************** */   
 flagcon=0;
if ( (ceaux->c->tipo == 'a') && (ceaux->d->tipo == 'a') )
 {
 if ((ceaux->c->nivel==cnivel) && (ceaux->d->nivel==cnivel))
  {
      ned->e=ceaux->c->b;
      if(ceaux->c->b->tipo=='q')
         ceaux->c->b->d=ned;  
      if(ceaux->c->b->tipo == 'a'){
         if(ceaux->c->b->d==ceaux->c)
            ceaux->c->b->d=ned;
         if(ceaux->c->b->c==ceaux->c)
            ceaux->c->b->c=ned;
      }
     free(ceaux->c);
     free(ceaux);
     flagcon=1;
  }
}

if (flagcon==0){ 
/* Se os vizinhos forem do tipo arvore de nivel diferente */
if ((ceaux->c->tipo=='a') && ((ceaux->c->nivel) > cnivel)) {
       ceaux->c->b=ceaux;
    }
if ((ceaux->d->tipo=='a') && ((ceaux->d->nivel) > cnivel)) {
        ceaux->d->b=ceaux;
    }

/* Se os nodos vizinhos forem do tipo quadradinho */
  if (ceaux->c->tipo == 'q') ceaux->c->d=ceaux;
  if (ceaux->d->tipo == 'q') ceaux->d->d=ceaux;
} 

/****************************************** */
/* Quarta etapa: Reenumera a curva de peano */
/* **************************************** */
   numpea();

/* **************************************** */
/* Quinta etapa: desenha a novo quadradinho */
/* **************************************** */

   drawE(ned->coord[0],ned->coord[1],ned->proc); 

}  /* Fim da funcao desrefina */


/* ************** FIM DO PROGRAMA TERMREF.C ************ */

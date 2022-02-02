
/*******************************************************/
/* PROGRAMA: refino.cpp                                  */
/* AUTORA  : DENISE BURGARELLI DUCZMAL                 */
/* DATA    : 97/02/06                                  */
/*******************************************************/

#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "funcoes.h"
#include "quad.h" 

#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;
         

#define null 0
/******** variaveis globais para este programa ***** */
double lado;
int grafo[4][4];
int peanot[4][4];
int sie,seqsie;
int nq,np,oi;

int minfront,calfront;
int viz[16];
quad gq,gq1;
quad pq[16];  /*primeiro quad. da curva peano para cada proce. */
int nquad[16];/*guarda o numero de quad. em cada processador   */
quad gne0;

/* ************Variaveis externas******************* */
extern int flagglobal;
extern int flagproblem;
extern int  flagpeano;
extern int flagonda_inicio;
extern double xiplaca;
extern double xfplaca;

/*******************************************************/
void simplif ( quad q0d)
{
 quad q1d;
 /* q1d=null; */
 q1d = (q0d->b);
 if((q0d->tipo=='a')&&(q1d->tipo=='a')){
   if(q0d->nivel==q1d->nivel){
          if(q0d==q0d->d->b)q0d->d->b=q1d->d;
     else if(q0d==q0d->d->c)q0d->d->c=q1d->d;
     else if(q0d==q0d->d->d)q0d->d->d=q1d->d;
     else if(q0d==q0d->d->e)q0d->d->e=q1d->d;

          if(q0d==q0d->c->b)q0d->c->b=q1d->c;
     else if(q0d==q0d->c->c)q0d->c->c=q1d->c;
     else if(q0d==q0d->c->d)q0d->c->d=q1d->c;
     else if(q0d==q0d->c->e)q0d->c->e=q1d->c;

          if(q1d==q1d->d->b)q1d->d->b=q0d->d;
     else if(q1d==q1d->d->c)q1d->d->c=q0d->d;
     else if(q1d==q1d->d->d)q1d->d->d=q0d->d;
     else if(q1d==q1d->d->e)q1d->d->e=q0d->d;

          if(q1d==q1d->c->b)q1d->c->b=q0d->c;
     else if(q1d==q1d->c->c)q1d->c->c=q0d->c;
     else if(q1d==q1d->c->d)q1d->c->d=q0d->c;
     else if(q1d==q1d->c->e)q1d->c->e=q0d->c;

     free(q0d);
     free(q1d);

   }
 }
}

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

void refina(quad ne)
{
 quad q0,q1,q2,qaux,qajuda;
 double a,a2,cx,cy,po2,cxatual,cxanterior,usoma_de_ne,
        uanteriorp,uatualizadop,
        uanteriorn,uatualizadon;
 extern double lado;
 int i,j,n,n0,n1,ni,n2,po,popo,k,q1p,n3,n4,cont;
 int ativo_herdado;

 /* Os valores abaixo  devem ser                    */
 /* distribuidos para os novos quadradinhos         */
 /* de acordo com o tipo de equacao a ser resolvida */
 /* cxatual: valor a ser distribuido para os novos  */
 /*         quadradinhos, este valor e a solucao    */
 /*         da matriz Ax=b por Laplace ou calor     */
 cxatual=ne->cx; 
 /* cxanterior: solucao no tempo anteiror  para a   */
 /*             equacao do calor                    */
 cxanterior=ne->cxant;
 /* valor calculado para as duas eq. da onda        */
 usoma_de_ne=ne->usoma;
 uanteriorp=ne->uantp;
 uatualizadop=ne->uatualp;
 uanteriorn=ne->uantn;
 uatualizadon=ne->uatualn;
/* ********************************* */
/*  ativo_herdado= ne->ativo;        */

 j=1;
 for(i=0;i<ne->nivel;i++){
     j*=2;
 }
 po=j;
 po2=po*2.0;
 popo=po*po;
 (ne->nivel)=(ne->nivel+1);
 /* (ne->ativo)= ativo_herdado; */ /* Ja foi feito */
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
 //setColor(5);
 //drawLine(cx-a,cy,cx+a,cy); 
 //drawLine(cx,cy-a,cx,cy+a);

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
 //drawE(q1->coord[0],q1->coord[1],q1p);
 q1=q1->next;
 q1->proc=q1p;
 //drawE(q1->coord[0],q1->coord[1],q1->proc);
 q1=q1->next;
 q1->proc=q1p;
 //drawE(q1->coord[0],q1->coord[1],q1->proc);
 q1=q1->next;
 q1->proc=q1p;
 //drawE(q1->coord[0],q1->coord[1],q1->proc);
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

   //if( ne->nivel == 9 ) cout << "Atingiu o nível!";

} /* fim da  funcao refina */

/* *******************************************************/
/* Funcao de desrefinamento do quadradinho de acordo com */
/* um criterio de desrefinamento                         */
/* ***************************************************** */

void desrefina(quad qref)
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
            
nq-=3; /* diminui 3 quadradinhos da malha */
/* ******************************************************* */
/* Primeira etapa:nodo nordeste do cacho a ser desrefinado */
/* ******************************************************* */

 qnext=qref->next->next->next->next;
 qprev=qref->prev;
 cxatual=(qref->cx + qref->next->cx + qref->next->next->cx + qref->next->next->next->cx)/4;
 cxanterior=(qref->cxant + qref->next->cxant + qref->next->next->cxant + qref->next->next->next->cxant)/4;

 usomaant=qref->usoma;
 uatualizadop=qref->uatualp;
 uatualizadon=qref->uatualn;
 uanteriorp=qref->uantp;
 uanteriorn=qref->uantn;
 processador=qref->proc;

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
 //drawE(qa0->coord[0],qa0->coord[1],7);
 //drawE(qa1->coord[0],qa1->coord[1],7);
 //drawE(qa2->coord[0],qa2->coord[1],7); 
 //drawE(qa3->coord[0],qa3->coord[1],7); 


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
 //drawLine(xaux,yaux+pos,xaux,yaux-pos);
 setColor(7); 
 //drawLine(xaux+pos,yaux,xaux-pos,yaux); 

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

/* ************** */
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

   //drawE(ned->coord[0],ned->coord[1],ned->proc); 

}  /* Fim da funcao desrefina */

/******************************************************/
/* Calcula o numero de vizinhos de cada quadradinho   */
/* *************************************************** */
void calcviz(quad q, quad q1, quad qua)
{
  if(qua->nivel > q->nivel)
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->c;
    }
  else
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->b;
    }
  if(qua->tipo=='q'){
    q=qua;
/*drawLine(q->coord[0],q->coord[1],q1->coord[0],q1->coord[1]);*/
    if((q1->nivel>q->nivel)&&(q1->proc!=q->proc)){
      viz[q->proc]++;
      viz[q1->proc]++;
    }
    if((q->nivel==q1->nivel)&&(q1->proc>q->proc)){
      viz[q->proc]++;
      viz[q1->proc]++;
    }
  }
}
/* ***************************************************** */
/* Funcao que antecede a funcao calcviz                  */
/* ***************************************************** */
void precalcviz()
{
 quad q1,qua,q;
 int promax,i,promin,vizmax;

 q1=gne0;
 do{
     q=q1;
     qua=q;
     qua=qua->b;
     calcviz(q,q1,qua);
    
      q=q1;
      qua=q;
      qua=qua->c;
      calcviz(q,q1,qua);
    
      q=q1;
      qua=q;
      qua=qua->d;
      calcviz(q,q1,qua);
   
      q=q1;
      qua=q;
      qua=qua->e;
      calcviz(q,q1,qua);
 
    
      q1=q1->next;
    
   }while(q1!=null);
    
   vizmax=0;
   for(i=0;i<np;i++){
   if(vizmax<viz[i])vizmax=viz[i];
      viz[i]=0;
   }
   
   promax=0;promin=32000;
   for(i=0;i<np;i++){
     if(promax<nquad[i])promax=nquad[i];
     if(promin>nquad[i])promin=nquad[i];
    }
   
}

/*****************************************************/
/* Gera as coordenadas de peano                      */
/*****************************************************/
int peano(int n0, int niv)
{
  int n1,i,j,k;
  n1=n0;
  seqsie=n0;
  i=0;
  for(k=1;k<niv;k++){
    j=n1%4;
    i=peanot[i][j];
    n1/=4;
  }
  sie=i;
  return(i);
}

/* ******************************************************** */
/* FUNCAO: Esta funcao enumera a curva de peano             */
/* ******************************************************** */
   void numpea()
   {
       quad qnu;
       int i;
        
       qnu=gne0;
       i=0;
       do{   qnu->np=i;
             i=i+1;
             qnu=qnu->next;
        } while(qnu!=null);
      }

/* *********************************************************** */
void linpeano(int valor)
 {
 quad q1;
 setColor(valor);
 q1=gne0;
 do{
  
  if (q1->next!=null){
   //drawLine(q1->coord[0],q1->coord[1],q1->next->coord[0],q1->next->coord[1]);
 }
 q1=q1->next;
 } while (q1!=null);
}

/************************************************************* */
/* FUNCAO : Esta funcao desenha as curvas de nivel da onda     */
/* *********************************************************** */
void curvas_de_nivel()
 {
 quad q1;
 int aux,i,nivel,xiei,yiei,xsdi,ysdi,cy,cor,cx;
 double meiolado,xie,yie,xsd,ysd;
 

 q1=gne0;
 do{
   aux=2;
   nivel=q1->nivel;
   for(i=0;i<nivel;i++) aux=2*aux;
   meiolado=1.0/(float)aux;
   xie=q1->coord[0]-meiolado; yie=q1->coord[1]-meiolado;
   xsd=q1->coord[0]+meiolado; ysd=q1->coord[1]+meiolado;
   xiei=(int)(xie+.5); yiei=(int)(-yie+.5); 
   xsdi=(int)(xsd+.5); ysdi=(int)(-ysd+.5); 
   /*RODNEY COMMENT Antigamente era assim:
   xiei=(int)(xie*cy+.5); yiei=(int)(cy-yie*cy+.5); 
   xsdi=(int)(xsd*cy+.5); ysdi=(int)(cy-ysd*cy+.5); 
   */
   cor=(int)(75.0*(q1->usoma+0.1));
   if(cor<0)
      cor=0;
   else if(cor>100)
      cor=100; 
   setColor(cor+8);  
   for(i=ysdi;i<=yiei;i++){
     //drawLine(xiei,i,xsdi,i);  
   }
   q1=q1->next;
 } while (q1!=null);
}


/*************************************************************      */
/* FUNCAO : Esta funcao desenha as curvas de nivel da temperatura   */
/* ***********************************************************      */
void term_curvas_de_nivel()

{
 quad q1;
 int aux,i,nivel,xiei,yiei,xsdi,ysdi,cy,cor,cx,iii;
 double meiolado,xie,yie,xsd,ysd,tmin,tmax,delta,riii;

 q1=gne0;
 tmin= 1e30;
 tmax=-1e30;
 term_previzquad();
 do{
   if(q1->ativo == 1){
     if(tmin > q1->temperatual) tmin=q1->temperatual;
     if(tmax < q1->temperatual) tmax=q1->temperatual;
   }
   q1=q1->next;
 } while (q1!=null);
 
 delta=tmax-tmin;
 if(delta<0.00001){delta=1.0;} /* atencao */
 q1=gne0;
 do{
  if(q1->ativo == 1){
   riii=2;
   /*for(iii=0;iii<30000;iii++)riii*=1.001;*/
   aux=2;
   nivel=q1->nivel;
   for(i=0;i<nivel;i++) aux=2*aux;
   meiolado=1.0/(float)aux;
   xie=q1->coord[0]-meiolado; yie=q1->coord[1]-meiolado; yie*=1.0; xie*=1.0;
   xsd=q1->coord[0]+meiolado; ysd=q1->coord[1]+meiolado; ysd*=1.0; xsd*=1.0;
   xiei=(int)(384*xie+.5); yiei=(int)(384*yie-.5); 
   xsdi=(int)(384*xsd+.5); ysdi=(int)(384*ysd-.5);
   cor=(int)(128.0*((5.0*q1->temperatual + 0.0*q1->temperdireita+ 0.0*q1->tempercima+0.0*q1->temperbaixo+0.0*q1->temperesquerda)/5.0-tmin)/(delta)+.5);
   if(cor<1)
      cor=1;
   else if(cor>99)
      cor=99; 
   setColor(cor+8);  
   if(delta==1.0)
      setColor(5);
   drawRectangle(xiei+10,120,xsdi+10,160); 
   /*  
   for(i=yiei;i<ysdi;i++){ //desenha temperatura em cima sem velocidade
   drawLine((double)xiei/400,(double)100/400,(double)(xsdi-1)/400,(double)100/400);   //i-7-100 
   }*/
  }
  q1=q1->next;
 }while (q1!=null);
 term_lin_placa_cima();
 
}

/****************************************************************** */
/* FUNCAO : Esta funcao desenha as curvas de nivel da pressao       */
/* **************************************************************** */
void term_desenha_pressao()
{
 quad q1;
 int aux,i,nivel,xiei,yiei,xsdi,ysdi,cy,cor,cx,iii;
 double meiolado,xie,yie,xsd,ysd,tmin,tmax,delta,riii;
 
 q1=gne0;
 tmin= 1e30;
 tmax=-1e30;
 do{
   if(q1->ativo == 1){
     if(tmin > q1->pressatual) tmin=q1->pressatual;
     if(tmax < q1->pressatual) tmax=q1->pressatual;
   }
   q1=q1->next;
 } while (q1!=null);
 delta=tmax-tmin;
 if(delta<0.00001){delta=1.0;} /* atencao */
 q1=gne0;
 do{
  if(q1->ativo == 1){
   riii=2;
   aux=2;
   nivel=q1->nivel;
   for(i=0;i<nivel;i++) aux=2*aux;
   meiolado=1.0/(float)aux;
   xie=q1->coord[0]-meiolado; yie=q1->coord[1]-meiolado; yie*=16.0; xie*=5.0;
   xsd=q1->coord[0]+meiolado; ysd=q1->coord[1]+meiolado; ysd*=16.0; xsd*=5.0;
   xiei=(int)(xie+.5); yiei=(int)(-yie-.5); 
   xsdi=(int)(xsd+.5); ysdi=(int)(-ysd-.5);
   
   cor=(int)(128.0*(q1->pressatual-tmin)/(delta)+.5);
   if(cor<0)cor=0;
   else if(cor>127)cor=127; 
   setColor(cor+8);  
   if(delta==1.0){setColor(5);}
   for(i=ysdi;i<yiei;i++){
     drawLine(xiei+1,i-9-100,xsdi,i-9-100); /*i-10*/ 
   }
  }
  q1=q1->next;
 }while (q1!=null);
 /*term_lin_placa();*/
}



/*************************************************************  */
/* FUNCAO : Esta funcao desenha o campo vetorial de velocidades */
/* ***********************************************************  */
void term_campo_vel()
 {
 quad q1;
 double xi,yi,xf,yf;
 double fator,fator1,fator2,vxmax,vymax,vxmin,vymin;

 setColor(1);

 vxmax=-1e30;
 vymax=-1e30;
 vxmin=1e30;
 vymin=1e30;
 q1=gne0;
 do{
   if(q1->ativo == 1){ 
     if(fabs(q1->velxatual)>vxmax) vxmax=fabs(q1->velxatual);
     if(fabs(q1->velyatual)>vymax) vymax=fabs(q1->velyatual);
     if(fabs(q1->velxatual)<vxmin) vxmin=fabs(q1->velxatual);
     if(fabs(q1->velyatual)<vymin) vymin=fabs(q1->velyatual);
   }
   q1=q1->next;
 } while (q1!=null);

 fator1= vxmax;
 if(vymax>fator1) fator1= vymax;
 fator2 = vxmin;
 if(vymin<fator2) fator2= vymin;
 fator=fator1;
 if((vxmax>1e-9)||(vymax>1e-9)){
   q1=gne0;
   do{
     if(q1->ativo == 1){
       xi=q1->coord[0]; 
       yi=q1->coord[1];
       xf=xi+(q1->velxatual)/(fator*45);
       yf=yi+(q1->velyatual)/(fator*45);
       setColor(1);
       drawLine(xi,yi,xf,yf);  
       setColor(3);
       drawLine(xi,yi,xi,yi);  
     }
     q1=q1->next;
   } while (q1!=null);
 }
}



/* ************************************************** */
/* FUNCAO : Decide se desenha ou nao a curva de Peano */
/* ************************************************** */
void Dchange_peano(int valor)
{

      
quad qauxa;
if(valor==0) {
   flagpeano=0;
}
if (valor==1){
   /* esta funcao fui usada aqui pra teste*/
   flagpeano=1; 
 }
}


/* ************************************************** */
/* **************************************************** */
/* FUNCAO: Chama a funcao den_play, se o flag permitir  */
/* **************************************************** */
void play(int flagplay)
{ 
    if (flagplay == 0) den_play_laplace();
    if (flagplay == 1) den_play_wave();  
    if (flagplay == 2) den_play_heat();
    if (flagplay == 3) den_play_therm();
}
/* ********************************************* */
/* FUNCAO: Inicio do programa de refinamento     */
/* ********************************************** */

/* *********************************************** */
/* FUNCAO: Inicio do programa de refinamento       */
/*         Coloca inicialmente 4 quadradinhos de   */
/*         nivel 1 em todo o dominio               */
/*         0<=x<=1 e 0<=y<=1                       */
/* **********************************************  */

void init ()
{
int i,j,k,n,n0,n1,n2,n3,vizmax,vizmin,promax,promin,
     viztot,cc,frefina;
char dir;
double  centro,a,lado,ff; 
quad ne0,nw0,sw0,se0,conb,conc,cond,cone,qua,qua1,q,q1,qq;
double xi,xf;
int naux;
quad q2;
double yaux,xiplaca0,xfplaca0;

 np=4;
 minfront=1;
 calfront=1; 
 cc=0;
 //clear_graf();


 
 setColor(5);
 int cor[8];
 cor[0]=1;
 cor[1]=3;
 cor[2]=5; 
 cor[3]=6;
 cor[4]=7;
 cor[5]=2;	
 cor[6]=1;
 cor[7]=0;
   
   
 /* cc=0; */
 oi=0;
/* numero de quadradinhos na malha */
 nq=4;
 lado=1.0;
 centro=lado/2.0;
 a=lado/4.0;
 /* entrar com os dados do arquivo entrada */

/**************************************************** */
   
 grafo[0][0]=0;
 grafo[0][1]=grafo[1][0]=0;
 grafo[0][2]=grafo[2][0]=0;
 grafo[0][3]=grafo[3][0]=0;
 grafo[1][1]=0;
 grafo[1][2]=grafo[2][1]=0;
 grafo[1][3]=grafo[3][1]=0;
 grafo[2][2]=0;
 grafo[2][3]=grafo[3][2]=0;
 grafo[3][3]=0;
   
/*****************************************************/
     
 peanot[0][0]=1;
 peanot[0][1]=0;
 peanot[0][2]=0;
 peanot[0][3]=3;
     
 peanot[1][0]=0;
 peanot[1][1]=1;
 peanot[1][2]=1;
 peanot[1][3]=2;
   
 peanot[2][0]=3;
 peanot[2][1]=2;
 peanot[2][2]=2;
 peanot[2][3]=1;
   
 peanot[3][0]=2;
 peanot[3][1]=3;
 peanot[3][2]=3;
 peanot[3][3]=0;
   
 /******************************************************/
      
     
 /* Cria nodos quadradinho iniciais. */
 ne0= (quad)malloc(sizeof(pquad));
 nw0= (quad)malloc(sizeof(pquad));
 sw0= (quad)malloc(sizeof(pquad));
 se0= (quad)malloc(sizeof(pquad));
    
 /* No caso de ser nodo arvore,                            */
 /* Conector1(...->c) sempre menor que conector2(...->d) . */
 /* ...->b aponta sempre para o nodo arvore pai.           */
 /* ...->c  e  ...->d apontam sempre para os nodos filhos. */
  
 /* Cria nodos arvore iniciais. */
 conb= (quad)malloc(sizeof(pquad));
 conc= (quad)malloc(sizeof(pquad));
 cond= (quad)malloc(sizeof(pquad));
 cone= (quad)malloc(sizeof(pquad));
  
 conb->tipo='a';
 conc->tipo='a';
 cond->tipo='a';
 cone->tipo='a';
  
 conb->nivel=1;
 conc->nivel=1;
 cond->nivel=1;
 cone->nivel=1;
  
 conb->ativo=1;
 conc->ativo=1;
 cond->ativo=1;
 cone->ativo=1;

 conb->b=null;
 conb->c=sw0;
 conb->d=se0;
  
 conc->b=null;
 conc->c=nw0;
 conc->d=ne0;
  
 cond->b=null;
 cond->c=se0;
 cond->d=ne0;



 cone->b=null;
 cone->c=sw0;
 cone->d=nw0;
 /* Liga nodos arvore aos quadradinhos. */
  
 ne0->tipo= 'q';
 ne0->ncon=1;
 ne0->nivel= 1;
 ne0->ativo=1;
 ne0->coord[0]= centro+a;
 ne0->coord[1]= centro+a;
 ne0->f=fxy(ne0->coord[0],ne0->coord[1]);
 ne0->cx=fxy(ne0->coord[0],ne0->coord[1]);
 ne0->b=se0;
 ne0->c=conc;
 ne0->d=cond;
 ne0->e=nw0;
 ne0->proc=0;/*0*/
 ne0->next=nw0;
 ne0->prev=null;
 ne0->pea=0;
 ne0->difer=0;
 ne0->fb=0;
 ne0->fc=0;
 ne0->fd=0;
 ne0->fe=0;
 
  
 nw0->tipo= 'q';
 nw0->nivel= 1;
 nw0->ativo=1;
 nw0->ncon=2;
 nw0->coord[0]= centro-a;
 nw0->coord[1]= centro+a;
 nw0->f=fxy(nw0->coord[0],nw0->coord[1]);
 nw0->cx=fxy(nw0->coord[0],nw0->coord[1]);
 nw0->b=sw0;
 nw0->c=conc;
 nw0->d=ne0;
 nw0->e=cone;
 nw0->proc=0; /*0*/
 nw0->next=sw0;
 nw0->prev=ne0;
 nw0->pea=1;
 nw0->difer=0;
 nw0->fb=0;
 nw0->fc=0;
 nw0->fd=0;
 nw0->fe=0;
  
  
 sw0->tipo= 'q';
 sw0->nivel= 1;
 sw0->ativo=1;
 sw0->ncon=3;
 sw0->coord[0]= centro-a;
 sw0->coord[1]= centro-a;
 sw0->f=fxy(sw0->coord[0],sw0->coord[1]);
 sw0->f=fxy(sw0->coord[0],sw0->coord[1]);
 sw0->b=conb;
 sw0->c=nw0;
 sw0->d=se0;
 sw0->e=cone;
 sw0->proc=0; /*0*/
 sw0->next=se0;
 sw0->prev=nw0;
 sw0->pea=2;
 sw0->difer=0;
 sw0->fb=0;
 sw0->fc=0;
 sw0->fd=0;
 sw0->fe=0;
  
   
 se0->tipo= 'q';
 se0->nivel= 1;
 se0->ativo=1;
 se0->ncon=4;
 se0->coord[0]= centro+a;
 se0->coord[1]= centro-a;
 se0->f=fxy(se0->coord[0],se0->coord[1]); 
 se0->cx=fxy(se0->coord[0],se0->coord[1]); 
 se0->b=conb;
 se0->c=ne0;
 se0->d=cond;
 se0->e=sw0;
 se0->proc=0; /*0*/
 se0->next=null;
 se0->prev=sw0;
 se0->pea=3;
 se0->difer=0;
 se0->fb=0;
 se0->fc=0;
 se0->fd=0;
 se0->fe=0;
  
   
/* Calcula as coordenadas dos centros dos quadr.e liga */
/* os quadradinhos entre si e com os nodos arvore. */
      
     /* Fim da inicializacao. */
 if(np==4){
    ne0->proc=0;pq[0]=ne0;nquad[0]=1;
    nw0->proc=1;pq[1]=nw0;nquad[1]=1;
    sw0->proc=2;pq[2]=sw0;nquad[2]=1;
    se0->proc=3;pq[3]=se0;nquad[3]=1;
 }
 if(np==3){
    ne0->proc=0;pq[0]=ne0;nquad[0]=1;
    nw0->proc=1;
    sw0->proc=1;pq[1]=sw0;nquad[1]=2;
    se0->proc=2;pq[2]=se0;nquad[2]=1;
 }
 if(np==2){
    ne0->proc=0;
    nw0->proc=0;pq[0]=nw0;nquad[0]=2;
    sw0->proc=1;
    se0->proc=1;pq[0]=se0;nquad[1]=2;
 }
    
    
    
/* Aqui comeca a ferramenta de navegacao no grafo   */
/* para escolhermos os nodos que serao explodidos   */
/* (quadradinhos que serao refinados). */ 
   setColor(5);
   //drawLine(0.0,0.0,1.0,0.0);
   //drawLine(1.0,0.0,1.0,1.0);
   //drawLine(1.0,1.0,0.0,1.0);
   //drawLine(0.0,1.0,0.0,0.0);
   //drawLine(0.0,0.5,1.0,0.5);
   //drawLine(0.5,0.0,0.5,1.0);   /*  ATENCAO */
   q=ne0;

   //drawEPlus(q->coord[0],q->coord[1],cor[q->proc]);  
   q=q->e;
   //drawE(q->coord[0],q->coord[1],cor[q->proc]); 
   q=q->b;
   //drawE(q->coord[0],q->coord[1],cor[q->proc]);
   q=q->d;
   //drawE(q->coord[0],q->coord[1],cor[q->proc]); 
   q=q->c;
   
   setColor(1);
   q1=ne0;
   if(flagpeano == 1){
   do{
   //drawLine(q1->coord[0],q1->coord[1],q1->next->coord[0],q1->next->coord[1]);
   q1=q1->next;
     }while(q1->next!=null);
}

/* *********************************************** */
gne0=ne0;
gq=q;
gq1=q1; 
flagglobal=1;
/* **** inicializacao da funcao da onda  ****** */
if (flagproblem==1){
   if(flagpeano==1) linpeano(7);
       
 for(i=0;i<3;i++){
   q=gne0;
   do{
      q1=q;
    if(q1->ativo==1){
    if(q1->nivel<4) {
      /*provisorio pois e de laplace */ /*predifquad(); */
      qua=q1;
      refina(qua);
      qua=q1;
      qua->b->proc=qua->proc;
      qua->b->e->proc=qua->proc;
      qua->e->proc=qua->proc;
      q=qua;
      //drawEPlus(q->coord[0],q->coord[1],cor[q->proc]);  
      q=q->e;
      //drawE(q->coord[0],q->coord[1],cor[q->proc]); 
      q=q->b;
      //drawE(q->coord[0],q->coord[1],cor[q->proc]);
      q=q->d;
      //drawE(q->coord[0],q->coord[1],cor[q->proc]); 
      q=q->c;
      q=qua;
      precalcviz();
     }
     }
     q=q->next;
   }while (q!=null);
  }
   /* inicializa usoma */
   /* q1=gne0;
   do{
      q1->usoma=0.0;
      q1=q1->next;
    }while(q1!=null); */ /* nao precisa disto */

   if (flagpeano==1) linpeano(1); 
}

 if (flagproblem==3){
 term_cond_inicial();
 if(flagpeano==1) linpeano(7);
 xi=0.0; /* 0.015 */
 xf=1.0; /* 0.285 */     
 for(i=0;i<5;i++){  /*i<8 */
  term_ativa(); 
   q1=gne0;
   do{
      
    q2=q1->next;
    if(q1->ativo==1){
    if(q1->nivel<7) {  /* q1->nivel<9 */ 
     if(i>50){  /* i>5 i bloqueado*/
       if((q1->coord[0]>=xi)&&(q1->coord[0]<=xf)){
         qua=q1;
         term_refina(qua);
         /*qua=q1;*/
         qua->b->proc=qua->proc;
         qua->b->e->proc=qua->proc;
         qua->e->proc=qua->proc;
         q=qua;
         //drawEPlus(q->coord[0],q->coord[1],1);  
         q=q->e;
         //drawE(q->coord[0],q->coord[1],1); 
         q=q->b;
         //drawE(q->coord[0],q->coord[1],1);
         q=q->d;
         //drawE(q->coord[0],q->coord[1],1); 
         q=q->c;
         q=qua;
         precalcviz();
        }
       }
        else {
         qua=q1;
         term_refina(qua);
         /*qua=q1;*/
         qua->b->proc=qua->proc;
         qua->b->e->proc=qua->proc;
         qua->e->proc=qua->proc;
         q=qua;
         //drawEPlus(q->coord[0],q->coord[1],cor[q->proc]);  
         q=q->e;
         //drawE(q->coord[0],q->coord[1],cor[q->proc]); 
         q=q->b;
         //drawE(q->coord[0],q->coord[1],cor[q->proc]);
         q=q->d;
         //drawE(q->coord[0],q->coord[1],cor[q->proc]); 
         q=q->c;
         q=qua;
         precalcviz(); }
     }
     }
     q1=q2;
   }while (q1!=null);
  }
 
  term_ativa();

/* ************************** */
/* refina mais perto da placa */
q1=gne0;
yaux =  (0.0625)/2.0;
term_lin_placa();
xiplaca0=0.25 -(4/64.0);  
xfplaca0=0.75 + (4/64.0); 
do{
    if(q1->ativo ==1){ 
       if( (q1->coord[0]<=xfplaca0) && (q1->coord[0]>=xiplaca0) && 
           (q1->coord[1]<yaux)&&(q1->nivel<7)){
          qua=q1;
          term_refina(qua);
          qua->b->proc=qua->proc;
          qua->b->e->proc=qua->proc;
          qua->e->proc=qua->proc;
          q=qua;
          //drawEPlus(q->coord[0],q->coord[1],1);  
          q=q->e;
          //drawE(q->coord[0],q->coord[1],1); 
          q=q->b;
          //drawE(q->coord[0],q->coord[1],1);
          q=q->d;
          //drawE(q->coord[0],q->coord[1],1); 
          q=q->c;
          q=qua;
          precalcviz();
       }
  } 
 q1=q1->next;
}while(q1!=null);
term_ativa(); 
/* ************************* */


   if (flagpeano==1) linpeano(1); 
}  /* fim do term */ 

}  /* Fim da funcao init */
/* ********************************************** */
/*************** FIM DO PROGRAMA REFINO  ******************** */

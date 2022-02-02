/*******************************************************/
/* PROGRAMA: termmat.cpp                                 */
/* AUTORA  : DENISE BURGARELLI DUCZMAL                 */
/* DATA    : 29/06/96                                  */
/*******************************************************/

#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "funcoes.h"
#include "quad.h" 

#define null 0
/***************** variaveis externas ***************** */

extern int flagglobal;
extern int flagpeano; 
extern quad gne0;

   
extern int npcont; /* numero de quad. */

/************ variaveis globais neste programa ******** */  
double dt;       /* valor que vem do programa term.c    */ 
int flag_t;

/******************************************************* */
void termmat_flag( int ft)
{
   flag_t=ft;
}


/***********************************************************/
/* FUNCAO:                                                 */
/* Coloca coeficiente da matriz principal do sistema Ax=b  */
/* numa posicao da lista encadeada associada ao quadr. q1. */
/* Esta lista contem todos os elementos nao-nulos da linha */
/* da matriz correspondente ao quadradinho q1.             */
/* Se a aresta for fronteira, devemos atualizar o vetor b. */
/***********************************************************/
void term_coefquad_matriz(quad q, quad q1, quad qua, char dir, double t)
{
  double fr,fr0,val,meiolado;
  int fri,i,col,col1,aux,nivel;
  mlin atual,novo;
  double deltad;
  double coef;

   dt=t; /* atualizao do delta tempo */ 

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
    i=q1->nivel;
    while(i>0){
      fri*=2;
      i--;
    }
  
    coef=(2.0/(float)fri)*(dt);
    /*coef=(dt/q1->meiolado); */
   
   /* *********************************************** */
   /* Use as funcoes do valor de contorno.            */
   /* Adiciona ao vetor cb o valor devido a fronteira */ 
   /* *********************************************** */
   if(dir=='b'){
       if (flag_t==0){
           if(q1->placa==1){
             fr= q1->velxfrontbaixo ;    
             q1->cb+= fr*coef; 
             q1->elem->valor=(q1->elem->valor)+coef;/*diagonal*/
           }
       }
       else if (flag_t == 1){
             fr= q1->velyfrontbaixo ;  
             q1->cb+=fr*coef; 
             q1->elem->valor=(q1->elem->valor)+coef;/*diagonal*/
       }
       else if (flag_t == 2){
            if(q1->placa==1){
             fr= q1->temperfrontbaixo ;
             q1->cb+=fr*coef; 
             q1->elem->valor=(q1->elem->valor)+coef; /*diagonal*/
           }
       }
   }
   else if(dir=='c') {
     if (flag_t == 1){
         fr= q1->velyfrontcima; 
         q1->cb+=fr*coef; 
         q1->elem->valor=(q1->elem->valor)+coef; /*diagonal*/
     }
   }
   else if(dir=='d') {
      if (flag_t==0){
         fr= q1->velxfrontdireita; 
         q1->cb+=fr*coef;  
         q1->elem->valor=(q1->elem->valor)+coef; /*diagonal*/
      }
      else if (flag_t == 1){
          fr= q1->velyfrontdireita;
          q1->cb+=fr*coef;
          q1->elem->valor=(q1->elem->valor)+coef; /*diagonal*/
      }
   }
   
   else if(dir=='e') {
      if (flag_t==0){
        fr= q1->velxfrontesquerda; 
        q1->cb+=fr*coef;
        q1->elem->valor =(q1->elem->valor)+ coef; /*diagonal*/
      }
      else if (flag_t == 1){
        fr= q1->velyfrontesquerda ;
        q1->cb+=fr*coef; 
        q1->elem->valor =(q1->elem->valor)+ coef;/*diagonal*/
      }

   }

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

      /* Atencao: como q tem nivel de profundidade menor, significa
                  que ele tem maior meiolado do que q1, logo o
                  deltax ou deltay deve ser olhado em relacao a q*/

      /*if(dir=='b') {deltad = q->deltacima;}
      if(dir=='c')   {deltad = q->deltabaixo;}
      if(dir=='d')   {deltad = q->deltaesquerda;}
      if(dir=='e')   {deltad = q->deltadireita;} */
      
      /*deltad=2.0*q->meiolado;*/
      deltad=1;

      atual=q1->elem;
      /* Procura se existe elemento com coluna q->np na lista */
      do{  
        atual=atual->next;
      }while((atual != null) && (atual->coluna != col));
      /*if( (atual == null) ){*/   
      /* Ainda nao foi visitado; marque abaixo e acima da diagonal. */
        fri=1;
        /*i=(q->nivel) - (q1->nivel);*/   /* Muita  ATENCAO */
        i=(q1->nivel) - (q->nivel);  /* Muita ATENCAO */
        while(i>0){
          fri*=2;
          i--;
        }
        fr=(1.0)/(float)fri;
        
        
        /* O procedimento abaixo vai fazer a marcacao simultanea */
        /* do relacionamento de q1 com q e de q com q1.          */ 

        /* Marque a diagonal de q1. */
        q1->elem->valor = (q1->elem->valor)+( (dt/deltad) );
        /* Marque a diagonal de q.  */
        q->elem->valor  = (q->elem->valor)+(fr * (dt/deltad) );

        atual=q1->elem;
        col=q->np;
        /* Procura se existe elemento com coluna col na lista */
        do{  
          novo=atual;
          atual=atual->next;
        }while((atual != null) && (atual->coluna != col));
        if(atual != null){  
          /* Caso ja exista, adicione val ao valor existente */
          atual->valor=(atual->valor)-((dt/deltad));
        }
        else{ /* Senao, crie novo elemento com valor val */ 
          novo->next=(mlin)malloc(sizeof(pmlin));
          atual=novo->next;
          atual->coluna=col;
          atual->valor= (-(dt/deltad));
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
          atual->valor=(atual->valor)-(dt/deltad)*fr;
        }
        else{ /* Senao, crie novo elemento com valor val */ 
          novo->next=(mlin)malloc(sizeof(pmlin));
          atual=novo->next;
          atual->coluna=col;
          atual->valor=(-(dt/deltad)*(fr));
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


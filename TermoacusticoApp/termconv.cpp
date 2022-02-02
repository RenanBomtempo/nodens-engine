 
/*******************************************************/
/* PROGRAMA: termconv.cpp                                */
/* AUTORA  : DENISE BURGARELLI DUCZMAL                 */
/* DATA    : 29/06/97                                  */
/*******************************************************/

#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "funcoes.h"
#include "quad.h" 

#define null 0

/* ************** variaveis externas ***************** */
extern int flagglobal;
extern int flagonda_inicio;
extern int flagpeano;
extern quad gne0;


extern int npcont; /* numero de quadradinhos */          

/*********** variaveis globais neste programa *************** */

double deltatempo_term; /* este valor vem do programa term.c   */
double cxpe,cype;       /* coordenadas do pe da caracteristica */
quad qcaminha; 
int flag_incognita_term;


/* *************************************************** */
/* FUNCAO: Procura o pe da caracteristica caminhando   */
/*         por quadradinhos e arvores                  */
/* *************************************************** */
void term_caminha_arvore( quad qaux, quad q1, quad qua, char direcao)
{
  
  
  if(qua->nivel > qaux->nivel)
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->c;
    }
  else
    while((qua->tipo=='a')&&(qua->b!=null)){
      qua=qua->b;
    }
  qcaminha=qua;
/*

 if((qcaminha->tipo=='a' || qcaminha->ativo == 0)){ 
   // Preciso saber o valor na fronteira 
   if (flag_incognita_term == 0) {
       if(direcao == 'b'){  
        q1->velxpe=qaux->velxbaixo;
       }
       else if(direcao == 'c'){  
        q1->velxpe=qaux->velxcima;
       }
       else if(direcao == 'd'){  
        q1->velxpe=qaux->velxdireita;
       }
       else if(direcao == 'e'){  
        q1->velxpe=qaux->velxesquerda;
       }
   }
   if (flag_incognita_term == 1) {
       if(direcao == 'b'){  
        q1->velype=qaux->velybaixo;
       }
       else if(direcao == 'c'){  
        q1->velype=qaux->velycima;
       }
       else if(direcao == 'd'){ 
        q1->velype=qaux->velydireita;
       }
       else if(direcao == 'e'){
        q1->velype=qaux->velyesquerda;
       }
   }
   if (flag_incognita_term == 2) {
       if(direcao == 'b'){  
        q1->temperpe=qaux->temperbaixo;
       }
       else if(direcao == 'c'){  
        q1->temperpe=qaux->tempercima;
       }
       else if(direcao == 'd'){  
        q1->temperpe=qaux->temperdireita;
       }
       else if(direcao == 'e'){  
        q1->temperpe=qaux->temperesquerda;
       }
   }
   
  }*/


}
/* *************************************************** */
/* FUNCAO: Metodo das caracteristicas modificado       */
/*         Calcula as coordenadas do pe da             */
/*         caracteristica                              */
/* *************************************************** */
 void term_pe_carac( quad q1 )
 {
  double coordx_fisica;
  double coordy_fisica;

    /* cxpe,cype: coordenadas do pe da caracteristica     */
    coordx_fisica= 8.0*(q1->coord[0]); 
    coordy_fisica= 8.0*(q1->coord[1]);
    
    cxpe=(coordx_fisica) - (q1->velxatual * deltatempo_term); 
    cype=(coordy_fisica) - (q1->velyatual * deltatempo_term);
 }


/* **************************************************  */
/* FUNCAO: Pecorre a malha procurando o quadradinho    */
/*         mais  proximo do pe da caracteristica       */
/* *************************************************** */
 void  term_calcula_por_carac()
 {
 quad q1,qaux,qua,q2;
 double d0,d1,ad0,ad1,meiolado,lado;
 double auxatual,auxanterior;
 int i,aux,j,nivel,cont_aux; 
 double coordx_fisica, coordy_fisica;

 q1=gne0;
 do{
    if(q1->ativo ==1){
      term_pe_carac(q1);
      /******************************************* */ 
      /* Pe da caracteristica fora do dominio      */
      /* ***************************************** */
      if((cxpe<0.0) || (cype<0.0) ||
         (cxpe>8.0) || (cype>0.5)){ 
           /* tamanho vertical do tubo  0.5 */
            if(flag_incognita_term == 0){
               q1->velxpe=q1->velxant;
             
            }
            else if (flag_incognita_term == 1) {
                 q1->velype = q1->velyant;
            }
                 else if (flag_incognita_term == 2){
                          q1->temperpe = q1->temperant;
                 }
      }
      /********************************************* */
      /* Pe da caracteristica dentro do dominio      */
      /********************************************* */
      else
      {
        qaux=q1;
        coordx_fisica=8.0*(qaux->coord[0]);  
        coordy_fisica=8.0*(qaux->coord[1]);

        d0=(cxpe - coordx_fisica); ad0=fabs(d0);
        d1=(cype - coordy_fisica); ad1=fabs(d1);


        meiolado=qaux->meiolado; 
          
        if((ad0>meiolado)||(ad1>meiolado)){
         
         do{
          q2=qaux; 
          coordx_fisica=8.0*qaux->coord[0]; 
          coordy_fisica=8.0*qaux->coord[1];

          if(ad0>=ad1)
          {
              if( coordx_fisica > cxpe)
              { 
                 qaux=q2->e; 
                 if(qaux->tipo=='a'){
                    qaux=q2;
                    qua=q2->e;
                    term_caminha_arvore(qaux,q1,qua,'e'); //e adicionado
                    qaux=qcaminha;
                 }
              } 
              else if (coordx_fisica <= cxpe) 
              {
                  qaux=q2->d;
                  if(qaux->tipo=='a'){
                     qaux=q2;
                     qua=q2->d;
                     term_caminha_arvore(qaux,q1,qua,'d'); //d adicionado
                     qaux=qcaminha;
                  }
             
              }
          }  /* fim do if (ad0 >=ad1)  */
          else if (ad1 >ad0)
          {
              if(coordy_fisica > cype)
              {
                   qaux=q2->b;
                   if(qaux->tipo=='a')
                   { 
                      qaux=q2; 
                      qua=q2->b; 
                      term_caminha_arvore(qaux,q1,qua,'b'); // b adicionado
                      qaux=qcaminha;
                   } 

              }
              else if ( coordy_fisica <= cype) {
                   qaux=q2->c;
                   if(qaux->tipo=='a'){ 
                      qaux=q2;
                      qua=q2->c; 
                      term_caminha_arvore(qaux,q1,qua,'c'); //c adicionado
                      qaux=qcaminha;
                   }
              }  
          }
        
          if((qaux->tipo=='q') && (qaux->ativo==1)){
             coordx_fisica=8.0*qaux->coord[0]; 
             coordy_fisica=8.0*qaux->coord[1];
             d0=(cxpe - coordx_fisica); ad0=fabs(d0);
             d1=(cype - coordy_fisica); ad1=fabs(d1);
             meiolado=qaux->meiolado;
          } 
          else if((qaux->tipo=='a') || (qaux->ativo==0)){
             ad0=meiolado/2.0;
             ad1=meiolado/2.0;   /* TESTE */
          }
        
         } while((ad0 >meiolado) ||  (ad1>meiolado));

 
         if((qaux->tipo=='q') && (qaux->tipo == 1)){

            if(flag_incognita_term== 0){
               q1->velxpe = qaux->velxant;          
            }
            else if(flag_incognita_term== 1){
               q1->velype = qaux->velyant;          
            }
            else if(flag_incognita_term== 2){
               q1->temperpe = qaux->temperant;          
            }
              
             
      }   /* fim do if (qaux->tipo->q) */
    } /*coloquei o fim do if((ad0>meiolado)||(ad1>meiolado)) aqui */
    else 
      {   
           /* Neste caso, o pe da caracteristica do quad. q1 esta  */
           /*  no proprio quad. q1                                 */
            if(flag_incognita_term== 0){
               q1->velxpe = q1->velxant;          
            }
            else if(flag_incognita_term== 1){
               q1->velype = q1->velyant;          
            }
            else if(flag_incognita_term== 2){
               q1->temperpe = q1->temperant;          
            }
       }  
                 
   } /* fim do else cxpe>0.0 .... */
 }/* fim do ativo */
 q1=q1->next;
}while(q1!=null);   

}


/* *************************************************** */
void term_conveccao( int valor, double dt)
{
    
    flag_incognita_term=valor;
    deltatempo_term=dt;
    term_calcula_por_carac();
}
/* ************************************************** */


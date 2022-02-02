/*******************************************************/
/* PROGRAMA: calor.cpp                                   */
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
extern quad gne0;

     
extern int npcont; /* numero de quad. */


/****** variaveis globais neste programa ******** */  

double deltat_calor=0.1; /* 0.05*/


/***********************************************************/
/* Coloca coeficiente da matriz principal do sistema Ax=b  */
/* numa posicao da lista encadeada associada ao quadr. q1. */
/* Esta lista contem todos os elementos nao-nulos da linha */
/* da matriz correspondente ao quadradinho q1.             */
/* Se a aresta for fronteira, devemos atualizar o vetor b. */
/***********************************************************/
void coefquad_calor(quad q, quad q1, quad qua, char dir)
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
      //fr=(fronteira(q1->coord[0],q1->coord[1]-meiolado))*2.0/(float)fri;
      fr=(fx0(q1->coord[0]))*2.0/(float)fri;
      q1->cb+=(fr);    //q1->cb+=(deltat_calor*fr); modificado em 29/03/2005
    }
    else if(dir=='c') {
      //fr=(fronteira(q1->coord[0],q1->coord[1]+meiolado))*2.0/(float)fri;
      fr=(fx1(q1->coord[0]))*2.0/(float)fri;
      q1->cb+=(fr); //q1->cb+=(deltat_calor*fr); modificado em 29/03/2005
    }
    else if(dir=='d') {
      //fr=(fronteira(q1->coord[0]+meiolado,q1->coord[1]))*2.0/(float)fri;
      fr=(f1y(q1->coord[1]))*2.0/(float)fri;
      q1->cb+=(fr); //q1->cb+=(deltat_calor*fr); modificado em 29/03/2005
    }
    else if(dir=='e') {
      //fr=(fronteira(q1->coord[0]-meiolado,q1->coord[1]))*2.0/(float)fri;
      fr=(f0y(q1->coord[1]))*2.0/(float)fri;
      q1->cb+=(fr); //q1->cb+=(deltat_calor*fr); modificado em 29/03/2005
    }
    /* Adiciona ao vetor b o valor devido `a fronteira. */
    q1->elem->valor =( q1->elem->valor)+ 2.0/(float)fri;
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
        /* Observe que o procedimento acima e' diferente */
        /* de apenas fazer i=q->nivel;                   */
        
        /* O procedimento abaixo vai fazer a marcacao simultanea */
        /* do relacionamento de q1 com q e de q com q1.          */ 

        /* Marque a diagonal de q1. */
        q1->elem->valor = (q1->elem->valor)+(deltat_calor)*fr;
        /* Marque a diagonal de q.  */
        q->elem->valor  = (q->elem->valor)+(deltat_calor)*fr;

        atual=q1->elem;
        col=q->np;
        /* Procura se existe elemento com coluna col na lista */
        do{  
          novo=atual;
          atual=atual->next;
        }while((atual != null) && (atual->coluna != col));
        if(atual != null){  
          /* Caso ja exista, adicione val ao valor existente */
          atual->valor=(atual->valor)-((deltat_calor)*fr);
        }
        else{ /* Senao, crie novo elemento com valor val */ 
          novo->next=(mlin)malloc(sizeof(pmlin));
          atual=novo->next;
          atual->coluna=col;
          atual->valor=(deltat_calor)*(-fr);
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
          atual->valor=(atual->valor)-(deltat_calor)*fr;
         
        }
        else{ /* Senao, crie novo elemento com valor val */ 
          novo->next=(mlin)malloc(sizeof(pmlin));
          atual=novo->next;
          atual->coluna=col;
          atual->valor=(deltat_calor)*(-fr);
          atual->next=null;
          atual->q=q1;
          
        }

    }
  }
}


/* ******************************************************* */
/* FUNCAO:Inicial. lista de colunas nao nulas em cada quad */
/* ******************************************************* */
void init_col_calor()
{  
    quad q1;

    q1=gne0;
    npcont=0;
    do{
      if (q1->ativo==1){
      q1->np=npcont;
      npcont++;
      q1->cb=(q1->cxant)*(2*q1->meiolado)*(2*q1->meiolado); /* olhar o sinal */
      q1->elem=(mlin)malloc(sizeof(pmlin));
      q1->elem->coluna=q1->np;
      q1->elem->valor=1.0 * (2*q1->meiolado)*(2*q1->meiolado); /* valor do elemento da diagonal? 1.0*/
      q1->elem->next=null;
      q1->elem->q=q1; /* diagonal */
      }
      q1=q1->next;
    }while(q1!=null);


}

/* ****************************************************** */
/* FUNCAO: Coloca os coeficientes na matriz virtual       */
/* ****************************************************** */

void monta_matriz_calor()
{
 quad q1,qua,q;
 char dir;

 q1=gne0;
 do{
 if(q1->ativo==1){
 q=q1; qua=q; qua=qua->b; dir='b';
 coefquad_calor(q,q1,qua,dir);
 q=q1; qua=q; qua=qua->c; dir='c'; 
 coefquad_calor(q,q1,qua,dir);
 q=q1; qua=q; qua=qua->d; dir='d';
 coefquad_calor(q,q1,qua,dir);
 q=q1; qua=q; qua=qua->e; dir='e';
 coefquad_calor(q,q1,qua,dir);
 }
 q1=q1->next;
}while(q1!=null);

}

/* ******************************************************* */
/* FUNCAO: Calcula meiolado                                */
/* ******************************************************* */
  void calcula_meiolado()
  {
   quad q1;
   int aux,i,nivel;
   
   q1=gne0;
   
   do{
      if(q1->ativo==1){
        aux=2;
        nivel = q1->nivel;
        for(i=0; i<=nivel; i++){
           aux= 2*aux;
        }
       q1->meiolado = 1.0/(float)aux;
      }
     q1=q1->next;
    }while(q1!=null);
  }
  
/* ******************************************************* */
/* FUNCAO : Condicao inicial                               */
/* ******************************************************* */
  void cond_ini_calor()
  {
   quad q1;
   
   q1=gne0;
   do{
      if( q1->ativo==1){
          q1->cx= 1.0; 
          q1->cxant=q1->cx;
      }
    q1=q1->next;      
   } while(q1!=null);
  } 
  
  
/* ******************************************************* */
/* FUNCAO : atualiza o valor anterior                      */
/* ******************************************************* */
  void atualiza_cxant()
  {
   quad q1;
   
   q1=gne0;
   do{
      if( q1->ativo==1){
          q1->cxant=q1->cx;
      }
    q1=q1->next;      
   } while(q1!=null);
  }
  
  
/******************************************************** */
/*********************************************************/
void calcula_calor()
{
    calcula_meiolado();    
    init_col_calor();      /*Inicial. lista de colunas nao nulas*/
    init_gradconj();  
    monta_matriz_calor();  /*  matriz virtual A de Ax-b         */
    gradconj();            /* metodo gradiente conjugado        */
} 

/* ******************************************************* */
/* FUNCAO: Chama as funcoes necessarias para a resolucao   */
/*        da equacao do calor                              */
/* *****************************************************   */
void resolve_calor()
{
    double t,tfinal,taux;
    int iteracao;
    quad q1;
    
    double refinementTime = 0.0,
           derefinementTime = 0.0,
           computationTime = 0.0,
           totalRefinementTime = 0.0,
           totalDerefinementTime = 0.0,
           totalComputationTime = 0.0,
           maximumNumberOfCells = 0.0;
    
    ofstream outputFile( "output.txt", ios::app ); //trunc
    outputFile << "\nEquação do Calor " << "\n\n";
    outputFile << "\nSimulação 1A " << "\n\n";
    outputFile << "Condição de fronteira: f(x,y) = 10." << endl;
    //outputFile << "Condição inicial: u(x,0) = 0. " << endl;
    //outputFile << "Tempo final: 1 s" << endl;
    //outputFile << "Incremento de tempo: 0.1 s" << endl;
    outputFile << "Nível de refinamento máximo: 5" << endl;
    //outputFile << "Cota de refinamento: 0.1" << endl;
    //outputFile << "Cota de desrefinamento: 0.06" << endl;
    //outputFile << "Tamanho mínimo da célula: 0.03125" << endl;

    
    cond_ini_calor();
    
    totalRefinementTime += Dx_ref_laplace();
    
    tfinal=10*deltat_calor;
    taux=deltat_calor;
    
    int numberOfCellsGreaterThan100 = 0,
        numberOfCellsGreaterThan10 = 0,
        numberOfCellsGreaterThan5 = 0,
        numberOfCellsGreaterThan2 = 0;
    double exactValue, relativeError;


    
    for (t=0.0;t<=tfinal;t+=taux)
    {
        
        computationTime = 0.0;
        refinementTime = 0.0;
        derefinementTime = 0.0;   
        
        //flush();
        //clear();
        
        /*
        numberOfCellsGreaterThan100 = 0,
        numberOfCellsGreaterThan10 = 0,
        numberOfCellsGreaterThan5 = 0,
        numberOfCellsGreaterThan2 = 0;
        */
        
        
        
        
        
        
        iteracao=0;
        atualiza_cxant();
        
        //Mede o tempo gasto nas contas numéricas
        CPerfTimer tempNum;
        tempNum.Start();
        
        calcula_calor();
        
        tempNum.Stop();     // Finaliza a medição do tempo
        computationTime = tempNum.Elapsedms();
       
        quad q1=gne0; 
        
        refinementTime = Dx_ref_laplace();
        derefinementTime = D_desref_laplace();
        
        npcont=0;
        do{
           q1=q1->next;
           npcont++;
        }while(q1!=null);
        
        if ( npcont > maximumNumberOfCells )
             maximumNumberOfCells = npcont;
    
        iteracao++;
        precalcviz();
        
    
        
        numpea();
        
        q1=gne0;
        
        /*
        ofstream outputFile( "output.txt", ios::app ); //trunc
        outputFile << "Instante de tempo: " << t << endl;
        
        
        do{
        
        exactValue = fxy( q1->coord[0], q1->coord[1] );
        relativeError =  fabs(fabs(exactValue) - fabs(q1->cx));    
        if( relativeError > 0.001 )
        {
              ++numberOfCellsGreaterThan2;
              drawColorDisk( q1->coord[0], q1->coord[1], 3 );//verde
        }    
        if( relativeError > 0.01 )
        {
            ++numberOfCellsGreaterThan5;
            drawColorDisk( q1->coord[0], q1->coord[1], 5 );//azul
        }   
        if( relativeError > 0.1 )
        {
            ++numberOfCellsGreaterThan10;
            drawColorDisk( q1->coord[0], q1->coord[1], 1 );//vermelho
        }
            
        if( relativeError >= 1.0 )
        {
            ++numberOfCellsGreaterThan100;
            drawColorDisk( q1->coord[0], q1->coord[1], 0 );//preto
        }
        /*
        if ( t > 99*deltat_calor )
        {
            outputFile << "Cell " << q1->np << "\n\n";
            outputFile << "Valor Calculado = " << q1->cx << "\n";
            outputFile << "Valor Exato = " << exactValue << "\n";
            outputFile << "Erro Relativo = " << relativeError << "%\n";
            //outputFile << "Total Flux = " << q1->difer << "\n";
            outputFile << 
            "________________________________________________________________________\n" 
            << endl;
        }    
        
        
            
        q1=q1->next;
        }while(q1!=null);
        */
        
        
    /*
    outputFile << "Número de células = " << npcont << "\n\n";
    outputFile << "Number of cells with error greater than 1 = " 
                << numberOfCellsGreaterThan100 << " - Percentage = "
                << (double)numberOfCellsGreaterThan100/npcont * 100 << "%" << endl;
    outputFile << "Number of cells with error greater than 0.1 = " 
                << numberOfCellsGreaterThan10 << " - Percentage = "
                << (double)numberOfCellsGreaterThan10/npcont * 100 << "%" << endl;
    outputFile << "Number of cells with error greater than 0.01 = " 
                << numberOfCellsGreaterThan5 << " - Percentage = "
                << (double)numberOfCellsGreaterThan5/npcont * 100 << "%" << endl;
    outputFile << "Number of cells with error greater than 0.001 = " 
                << numberOfCellsGreaterThan2 << " - Percentage = "
                << (double)numberOfCellsGreaterThan2/npcont * 100 << "%" << endl;
                */
     
    
    totalRefinementTime += refinementTime;
    totalDerefinementTime += derefinementTime;
    totalComputationTime += computationTime;
    /*
    outputFile << "Tempo de refinamento (s): " << refinementTime/1000 << std::endl;
    outputFile << "Tempo de desrefinamento (s): " << derefinementTime/1000 << std::endl;
    outputFile << "Tempo de computação (s): " << computationTime/1000 << std::endl;
    outputFile << 
            "________________________________________________________________________\n";
            */
    
         
    } 

    
    outputFile << "Número máximo de células = " << maximumNumberOfCells << endl; 
    outputFile << "________________________________________________________________________\n"  
    << endl;
    outputFile << "Tempo de refinamento total (s): " << totalRefinementTime/1000 << endl;
    outputFile << "Tempo de desrefinamento total (s): " << totalDerefinementTime/1000 << endl;
    //outputFile << "Tempo de refinamento e desrefinamento total (s): " 
    //<< (totalRefinementTime + totalDerefinementTime)/1000 << endl;
    outputFile << "Tempo de computação total (s): " << totalComputationTime/1000 << endl;
    outputFile << "Percent: " << ((totalRefinementTime + totalDerefinementTime)/totalComputationTime)*100 << endl;
        
}  

/* ******************************************************* */
/* FUNCAO: Chama a funcao que refina a malha de acordo     */
/*  com a cota de refinamento                              */
/***********************************************************/
void den_play_heat()
{
   if (flagpeano == 1) linpeano(7);
   resolve_calor();
   if (flagpeano==1) linpeano(1);

 }  

/* ******************************************************* */
/* FIM DO PROGRAMA : CALOR.C
/* ****************************************************** */

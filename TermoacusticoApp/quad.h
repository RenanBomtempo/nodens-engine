/* *************************************************** */
/* Program: quad.h                                     */
/* Name: Denise Burgarelli Duczmal                     */
/* *************************************************** */

#ifndef ESTRUTURA

typedef struct pquad *quad; 
/*Observe que declaramos antes para que mlin possa apontar para quad*/

typedef struct pmlin *mlin;
typedef struct pmlin
{
  int coluna;  /* coluna da linha associada ao quadradinho        */
  double valor;/* valor desse elemento da matriz                  */
  mlin next;   /* proximo elemento nao nulo da linha              */
  quad q;      /* aponta para o quadradinho correspondente        */
}pmlin;        /* lista dos elementos nao nulos na linha da matriz*/


typedef struct pquad
{
  char tipo;       /* quad ('q') ou nodo de arvore ('a')      */
  int ativo;        /* indica o quad. esta vivo(1) ou morto(0) */
  int nivel;       /* nivel de refinamento                 */
  int ncon;        /* identificador de cacho               */
  int npeapai;     /* numero de peano do quad. pai         */
  int proc;        /* proces. ao qual esta'alocado o quad. */
  quad next;       /* proximo da lista de quad.  feitos.   */ 
  quad prev;       /* anterior da lista de quad. feitos.   */  
  double coord[3]; /* coord.do ponto central do quad       */ 
  int pea;         /* coord. de Peano em grupos de 2 bits. */ 
  quad b;          /* pont. para baixo (q) ou conector0 (a)*/
  quad c;          /* pont. para cima (q) ou conector1 (a) */
  quad d;          /* pont. p/ direita (q) ou conector2 (a)*/
  quad e;          /* ponteiro para esquerda (q)           */
  double f;        /* valor  exato de f(x,y)               */
  double difer;    /* diferenca de f(x,y) os vizinhos      */
  double residuo;  /* usado pelo programa calor            */
  double fb,fc,fd,fe; /* fluxo das direcoes b,c,d,e        */
  int np;          /* numero associado a cada quadradinho  */
  double cb;       /* coordenada do vetor b em Ax=b        */
  double cg;       /* coordenada do vetor g do grad.conj.  */
  double cg1;      /* coordenada do vetor g1 do grad.conj. */
  double ch;       /* coordenada do vetor h do grad.conj.  */
  double ch1;      /* coordenada do vetor h1 do grad.conj. */
  double cx;       /* coordenada do vetor x solucao        */
  double cxant;    /* coordenada do vetor x solucao no     */
                   /* tempo anterior                       */
  double cz;       /* coordenada do vetor z                */
  mlin elem;       /* lista dos elementos nao-nulos da     */
                   /* linha associada na matriz principal  */
  /* ponteiros usados pelo programa onda   */
  double uantp;    /*valor de u(x,y,0) para veloc. positiva*/
  double uantn;    /*valor de u(x,y,0) para veloc. negativa*/
  double uatualp;  /*valor de u(x,y,t) para veloc. positiva*/
  double uatualn;  /*valor de u(x,y,t) para veloc. negativa*/
  double usoma;    /* soma de u(x,y,t) para vel.pos.e neg. */
  double fbp,fcp,fdp,fep; /* fluxo das direcoes b,c,d,e    */
                          /* de uantp  */
  double fbn,fcn,fdn,fen; /* fluxo das direcoes b,c,d,e    */
                          /* de uantn  */
 
  /* ************************************** */
  /* ponteiros usados pelo programa term.c  */
  /* ************************************** */
  int frontb; /* 1: indica que o quad. esta na fronteira baixo   */
  int frontc; /* 1: indica que o quad. esta na fronteira cima    */
  int frontd; /* 1: indica que o quad. esta na fronteira direita */
  int fronte; /* 1: indica que o quad. esta na fronteira esquerda*/
 
  double densatual;    /* densidade */
  double densant;
  double densk;
  double densk1;

  double temperatual;  /* temperatura */
  double temperant;
  double temperk;     
  double temperk1;

  double pressatual;   /* pressao */
  double pressant;
  double pressk; 
  double pressk1;

  double energatual;   /* energia */
  double energant;
  double energk;    
  double energk1;

  double viscatual;    /* viscosidade */
  double viscant;
  double visck;      
  double visck1;

  double condutatual;  /* condutividade termica */
  double condutant;
  double condutk;      
  double condutk1;

  double velxatual;    /* velocidade */
  double velxant;
  double velyatual;
  double velyant;
  double velxk;     
  double velxk1;
  double velyk;
  double velyk1;

  double phi;
  double ladodireito; /* lado direito da matriz */

  double fator;       /* fator que multiplica a incognita na  */
                      /* resolucao de                         */
                      /*laplacianoU + fatorUn1=ladodireito;   */

  double velxpe;      /* no metodo das caracteristicas         */
  double velype;      /* guarda os valores do pe da            */
  double temperpe;    /* da caracteristica                     */ 

  double deltaesquerda;  /* distancia  a direita e a esquerda  */
  double deltadireita;   /* distancia    abaixo e acima        */
  double deltacima;
  double deltabaixo;
  double meiolado;           /* meiolado */
  double velxdireita;    /* velocidade x a direita do quad.    */
  double velxesquerda;   /* velocidade x a esquerda do quad.   */
  double velxcima;       /* velocidade x acima do quad.        */
  double velxbaixo;
  double velycima;       /* velocidade y acima do quad.        */
  double velybaixo;      /* velocidade y abaixo do quad.       */
  double velydireita;    /* velocidade y  direita do quad.     */
  double velyesquerda;
  double denscima;       /* densidade  acima  do quad.         */
  double densbaixo;
  double densdireita;    /* densidade  a direita  do quad.     */
  double densesquerda;
  double pressdireita;   /* pressao a direita do quad.         */
  double pressesquerda;
  double presscima;      /* pressao acima do quad.             */
  double pressbaixo;
  double tempercima;
  double temperbaixo;
  double temperesquerda;
  double temperdireita;

  double velxfrontdireita;    /* velocidade x a direita do quad.    */
  double velxfrontesquerda;   /* velocidade x a esquerda do quad.   */
  double velxfrontcima;       /* velocidade x acima do quad.        */
  double velxfrontbaixo;
  double velyfrontcima;       /* velocidade y acima do quad.        */
  double velyfrontbaixo;      /* velocidade y abaixo do quad.       */
  double velyfrontdireita;    /* velocidade y  direita do quad.     */
  double velyfrontesquerda;
  double densfrontcima;       /* densidade  acima  do quad.         */
  double densfrontbaixo;
  double densfrontdireita;    /* densidade  a direita  do quad.     */
  double densfrontesquerda;
  double pressfrontdireita;   /* pressao a direita do quad.         */
  double pressfrontesquerda;
  double pressfrontcima;      /* pressao acima do quad.             */
  double pressfrontbaixo;
  double temperfrontcima;
  double temperfrontbaixo;
  double temperfrontesquerda;
  double temperfrontdireita;

  double dvelxdireita;    /* velocidade x a direita do quad.    */
  double dvelxesquerda;   /* velocidade x a esquerda do quad.   */
  double dvelxcima;       /* velocidade x acima do quad.        */
  double dvelxbaixo;
  double dvelycima;       /* velocidade y acima do quad.        */
  double dvelybaixo;      /* velocidade y abaixo do quad.       */
  double dvelydireita;    /* velocidade y  direita do quad.     */
  double dvelyesquerda;
  double ddenscima;       /* densidade  acima  do quad.         */
  double ddensbaixo;
  double ddensdireita;    /* densidade  a direita  do quad.     */
  double ddensesquerda;
  double dpressdireita;   /* pressao a direita do quad.         */
  double dpressesquerda;
  double dpresscima;      /* pressao acima do quad.             */
  double dpressbaixo;
  double dtempercima;
  double dtemperbaixo;
  double dtemperesquerda;
  double dtemperdireita;

 
  double hx;      /* densidade de fluxo de energia na direcao horizontal*/
  double hxmedia; /* densidade de fluxo de energia na direcao horizontal*/
  double hy;      /* densidade de fluxo de energia na direcao vertical */
  double hymedia; /* densidade de fluxo de energia na direcao vertical */
  
  int placa;
}pquad;            /* Cada um dos quadradinhos da malha    */
   



#define null 0
#define ESTRUTURA
#endif



/* **************************************************************** */


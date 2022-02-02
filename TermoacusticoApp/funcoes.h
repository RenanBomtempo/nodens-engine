/* ************************************************************ */
    /* Program : funcoes.h                                          */
    /* Name : Denise Burgarelli Duczmal                             */
    /* ************************************************************ */ 

#include "quad.h" 
/* __________________________________________________________________________ */
/* ______________________________ graphics.cpp ______________________________ */
    void clear();
    void clear_graf();
    void defineTableOfColors();
    void setColor(int);
    void drawColorDisk( double, double, int );
    void flush();
    /* Funções de desenho usadas pela malha adaptativa */
    void drawLine(int,int,int,int);
    void drawLine(double,double,double,double);
    void drawRectangle(int,int,int,int);
    void drawRectangle(double,double,double,double);
    void drawE(double,double,int);
    void drawEPlus(double,double,int);
    //void drawEX(double,double,int);
/* __________________________________________________________________________ */  
/* _______________________________ refino.cpp ________________________________*/
    void refina(quad);
    void simplif(quad);
    void precalcviz();
    void calcviz(quad,quad,quad);
    void desrefina(quad);
    void numpea();
    void linpeano(int);
    int peano(int,int);
    void init();
    void play(int);
    void Dchange_ref( double );
    void Dchange_des( double ); 
    void Dchange_peano( int );
    void curvas_de_nivel();
    void term_curvas_de_nivel();
/* __________________________________________________________________________ */
/* _________________________________laplace.cpp_______________________________*/
    void coefquad_laplace(quad,quad,quad,char);
    void den_play_laplace();
    int resolve_laplace();
    void init_col_laplace();
    void monta_matriz_laplace();
 /* As funcoes abaixo estao em laplace.c, mas sao */
 /* usadas por laplace.c e calor.c                */
 /* Estas funcoes sao responsaveis pela definicao */
 /* na fronteira,criterios de refinamento e       */
 /* desrefinamento, gradiente conjugado           */
    double fx0(double);
    double fx1(double);
    double f1y(double);
    double f0y(double);
    double fxy(double,double);
    double fronteira(double,double);
    void condes_laplace(quad);
    void prediferx_laplace();
    void difquadx_laplace(quad,quad,quad,char);
    void diferx_laplace(quad,quad,quad,char);
    double D_desref_laplace();
    double Dx_ref_laplace();
    void init_gradconj();
    void gradconj();
/* ************************************************************* */
/* *************************onda.cpp ***************************** */
    void calcula_por_carac();
    void caminha_arvore(quad,quad,quad,char);
    void pe_carac(quad,char);
    void den_play_wave();
    void onda_cond_inicial();
    void atualiza_inicio_onda(quad);
    void D_guarda_anterior();
    void resolve_onda();
    void condes_onda(quad);
    void predifquad_onda();
    void difquad_onda(quad,quad,quad,char);
    void difquad_pe(quad,quad,quad,char);
    void predifquad_pe();
    double D_desref_onda();
    double D_ref_onda();
    double D_ref_onda_inicial();
    void resolve_onda();
    void predifer_onda();
    void difer_onda(quad,quad,quad,char);
/* *********************************************************** */
/* ***********************calor.cpp **************************** */
    void coefquad_calor(quad,quad,quad,char);
    void den_play_heat();
    void resolve_calor();
    void init_col_calor();
    void atualiza_cb_calor();
    void monta_matriz_calor();
    void calcula_meiolado();
/* *********************************************************** */
/* *************************term.cpp *************************** */
    void term_lin_placa();
    void term_lin_placa_cima();
    void term_ativa();
    void den_play_therm();
    void term_cond_inicial();
    void term_cond_front();
    void term_atualiza_inicio();
    void term_guarda_anterior();
    void term_resolve();
    void print();
    void term_condes(quad);
    void term_predifquad();
    void term_difquad(quad,quad,quad,char);
    void term_desref();
    void term_ref();
    void term_ref_inicial();
    void term_refdes();
    void term_predifer();
    void term_difer(quad,quad,quad,char);
    void term_ativo();

    void term_vizquad(quad,quad,quad,char);
    void term_previzquad();
    void term_continuidade();
    void term_pressao();
    void term_visc();
    void term_condut();
    void term_momentum();
    void term_phi();
    void term_temperatura();
    void term_energia ();

    double fronteira_term();
/* ********************************************************** */
/* ****************** termconv.cpp **************************** */
    void term_caminha_arvore(quad,quad,quad,char);
    void term_pe_carac(quad);
    void term_calcula_por_carac();
    void term_conveccao(int,double);
/* ********************************************************** */
/* ****************** termdifu.cpp **************************** */
    void term_coefquad(quad,quad,quad,char);
    void term_difusao(int,double);
    void term_init_col();
    void term_atualiza_cb();
    void term_monta_matriz();
    void term_write_matrix();  
/* ********************************************************** */
/* ****************** termref.cpp ***************************** */
   void term_refina(quad);
   void term_desrefina(quad);
/* ********************************************************** */
/*********************termmat.cpp ***************************** */
   void term_coefquad_matriz(quad,quad,quad,char,double);
   void termmat_flag(int);
/* ****************** FIM FUNCOES.H ************************* */

/*********************************************************************
//  Trabalho 1 - Transformada discreta do Cosseno
//  Disciplina: Computacao Grafica
//  Professor: Cezar Pozzer
//  Aluno: Vinicius Mateus Dreifke
//  Matricula: 201413424
//  Data: 10/04/2017
//
//  Descricao:
//      Calculo da DCT e iDCT amostrado em sinais unidimensionais
//      entre 8 amostras de valores entre -100 e 100.
//      O programa permite alterar o valor do sinal original com o
//      mouse ou com funcoes predefinidas nos botoes (sen, cos, random),
//      resultando automaticamente nos sinais dct e idct.
//      Alem de alterar os valores, eh possivel ligar e desligar o uso
//      das funcoes ortogonais mostradas no lado direito da tela,
//      pressionando uma ou mais delas.
//      Tambem pode ser utilizada a opcao de quantizar (Fator de 2) o sinal,
//      alterando o valor da dct e idct.
//
//  Instruções:
//      - Utilize o mouse no retangulo "Original + iDCT" para alterar
//      os valores do vetor.
//      - O botao limpar reseta os sinais para o valor padrao, reniciando
//      tambem as funcoes ortogonais e a opcao de quantizacao.
//      - O botao seno, cosseno predefinem o sinal em uma funcao da opcao
//      selecionada.
//      - O botao random altera os valores aleatoriamente dos sinais.
//      - Cada funcao ortogonal pode ser clicada, ativando ou desativando
//      os calculos feitos com as respectivas funcoes.
//      - O checkbox da quantizacao ativa e desativa o calculo dos sinais.
//
//  Quesitos implementados:
//      1 - Sinal Original
//      2 - Sinal IDCT
//      3 - Sinal DCT
//      4 - 8 Funcoes ortogonais
//      5 - Delimitadores dos sinais
//      6 - Ativicao de uma ou mais das funcoes ortogonais sobre a DCT
//      7 - Botao para resetar a interface e os valores
//      8 - Botao para desenhar uma curva Seno
//      9 - Botao para desenhar uma curva Cosseno
//      10 - Botao para mudar os valores do vetores aleatoriamente
//      11 - Checkbox para aplicar a quantizacao sobre as funcoes
//      12 - Amostragem dos valores do sinal original
//      13 - Caixas para delimitar cada funcao
//      14 - Alteracao dos valores do sinal original com o Mouse em qualquer posicao do vetor
//      15 - Janela 1280x720
//      16 - Valores alterados automaticamente, sem utilizar um botao para atualizar
//      17 - Tentativa de implementar com a funcao bidimensinal (NAO TERMINADA)
//
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "gl_canvas2d.h"
#define LAG_TELA 1280 //Largura da janela
#define ALT_TELA 720 //Altura da janela
#define LAG_BOX 500 //Largura da caixa que eh feita a transformacao do cosseno
#define ALT_BOX 200 //Altura da caixa que eh feita a transformacao do cosseno
#define LAG_BOX2 240 //Largura da caixa que eh feita as funcoes ortogonais
#define ALT_BOX2 150 //Altura da caixa que eh feita as funcoes ortogonais
#define OFFSET 40 //Espacamento entre caixas, textos, linhas e outros
#define PI 3.14 //valor de PI
#define TAM 8 //Tamanho do vetor da DCT
#define DIM 1 //Dimensao do tamanho, pode ser aumentado proporcionalmente com o TAM

using namespace std;

//variaveis globais
int   opcao  = 49;
int acao; //Acao do mouse sobre as caixas de selecao
float global = 0;
double sinal_original[TAM]; //Sinal original, onde eh feita todas as funcoes sobre
double sinal_dct[TAM]; //Sinal aplicado a DCT
double sinal_idct[TAM]; //Sinal aplicado a iDCT
double amostra[TAM][TAM]; //Funcoes ortogonais
bool amostra_bool[TAM]; //Checagem para aplicar a funcao das respectivas funcoes ortogonais
double ortogonais[TAM][240]; //Funcoes ortogonais aplicadas
int quantiz[TAM] = {3, 5, 7, 9, 11, 13, 15, 17 }; //Matriz de quantizacao para fator 2
bool quant_bool = false; //Checkbox da quantizacao
int quantizada[TAM]; //Sinal aplicado com a quantizacao ao DCT e iDCT

//Funcao C da conta do DCT
double C(int w){
    if(w == 0)
        return 0.70710678118654752440084436210485; // 1/raiz(2)
    else
        return 1.0;
}

//Preenche os valores iniciais de cada vetor, utilizado tambem no botao Limpar
void PreencherValores(){
    quant_bool = false;
    for(int i=0;i<TAM;i++){
        sinal_original[i] = 0.0;
        sinal_dct[i] = 0.0;
        sinal_idct[i] = 0.0;
        for(int j=0;j<TAM;j++){
            amostra[i][j] = 0;
        }
        for(int j=0;j<TAM;j++){
            amostra_bool[i] = true;
        }
    }

}

//Preencher os valores das amostras com o calculo DCT
void PreencherAmostras(){
    for(int u=0;u<TAM;u++){
        for(int x=0;x<240;x++){
            ortogonais[u][x] = cos(((2.0*(x/30.0)+1.0)*u*PI)/(2.0*TAM)) * C(u);
        }
    }
}

//Checagem de cada box ativo ou inativo das funcoes ortogonais, parte da interface.
void On_Off(int i){
    if(amostra_bool[i] == true)
        color(0.95f,0.95f,0.95f);
    else
        color(0.6f,0.6f,0.6f);
}

//Funcao para orginizar a interface com textos, retangulos, caixas, linhas de delimitacoes,
//valores da regiao, descricao da area e sinalizacao.
void CriaInterface()
{
    //Retangulos da DCT e iDCT
    color(0.95f,0.95f,0.95f);
    rectFill( OFFSET, OFFSET, LAG_BOX + OFFSET, ALT_BOX + OFFSET);
    color(0.95f,0.95f,0.95f);
    rectFill( OFFSET, 2*OFFSET + ALT_BOX, OFFSET + LAG_BOX, 2*ALT_BOX + 2*OFFSET );

    //Retangulos das funcoes ortogonais
    //Primeira metade
    On_Off(0);
    rectFill( 2*OFFSET + LAG_BOX, OFFSET, 2*OFFSET+LAG_BOX + LAG_BOX2, OFFSET*0.2 + ALT_BOX2 );
    On_Off(1);
    rectFill( 2*OFFSET + LAG_BOX, OFFSET + ALT_BOX2, 2*OFFSET+LAG_BOX + LAG_BOX2, ALT_BOX2*2);
    On_Off(2);
    rectFill( 2*OFFSET + LAG_BOX, OFFSET + ALT_BOX2*2, 2*OFFSET+LAG_BOX + LAG_BOX2, ALT_BOX2*3);
    On_Off(3);
    rectFill( 2*OFFSET + LAG_BOX, OFFSET + ALT_BOX2*3, 2*OFFSET+LAG_BOX + LAG_BOX2, ALT_BOX2*4);
    //Segunda metade
    On_Off(4);
    rectFill( 4*OFFSET + LAG_BOX + LAG_BOX2, OFFSET, 4*OFFSET+LAG_BOX + LAG_BOX2*2, OFFSET*0.2 + ALT_BOX2 );
    On_Off(5);
    rectFill( 4*OFFSET + LAG_BOX + LAG_BOX2, OFFSET + ALT_BOX2, 4*OFFSET+LAG_BOX + LAG_BOX2*2, ALT_BOX2*2);
    On_Off(6);
    rectFill( 4*OFFSET + LAG_BOX + LAG_BOX2, OFFSET + ALT_BOX2*2, 4*OFFSET+LAG_BOX + LAG_BOX2*2, ALT_BOX2*3);
    On_Off(7);
    rectFill( 4*OFFSET + LAG_BOX + LAG_BOX2, OFFSET + ALT_BOX2*3, 4*OFFSET+LAG_BOX + LAG_BOX2*2, ALT_BOX2*4);
    //Mostrar a linha de divisao dos valores positivos e negativos
    color(0,0,0);
    line(2*OFFSET + LAG_BOX, OFFSET+60, 2*OFFSET+LAG_BOX + LAG_BOX2, OFFSET+60);
    line(2*OFFSET + LAG_BOX, OFFSET + ALT_BOX2+38, 2*OFFSET+LAG_BOX + LAG_BOX2, OFFSET + ALT_BOX2+38);
    line(2*OFFSET + LAG_BOX, OFFSET + ALT_BOX2*2+38, 2*OFFSET+LAG_BOX + LAG_BOX2, OFFSET + ALT_BOX2*2+38);
    line(2*OFFSET + LAG_BOX, OFFSET + ALT_BOX2*3+42, 2*OFFSET+LAG_BOX + LAG_BOX2, OFFSET + ALT_BOX2*3+42);

    line(4*OFFSET + LAG_BOX + LAG_BOX2, OFFSET+60, 4*OFFSET+LAG_BOX + LAG_BOX2*2, OFFSET+60);
    line(4*OFFSET + LAG_BOX + LAG_BOX2, OFFSET + ALT_BOX2+38, 4*OFFSET+LAG_BOX + LAG_BOX2*2, OFFSET + ALT_BOX2+38);
    line(4*OFFSET + LAG_BOX + LAG_BOX2, OFFSET + ALT_BOX2*2+38, 4*OFFSET+LAG_BOX + LAG_BOX2*2, OFFSET + ALT_BOX2*2+38);
    line(4*OFFSET + LAG_BOX + LAG_BOX2, OFFSET + ALT_BOX2*3+42, 4*OFFSET+LAG_BOX + LAG_BOX2*2, OFFSET + ALT_BOX2*3+42);

    //Botoes
    //Limpar, seno, cosseno, random e quantizacao
    //Retangulos
    color(0.9f,0.9f,0.9f);
    rectFill(OFFSET, OFFSET*3 + ALT_BOX*2, OFFSET*3, OFFSET*4 + ALT_BOX*2);
    rectFill(OFFSET*4, OFFSET*3 + ALT_BOX*2, OFFSET*5.5, OFFSET*4 + ALT_BOX*2);
    rectFill(OFFSET*6.5, OFFSET*3 + ALT_BOX*2, OFFSET*8.8, OFFSET*4 + ALT_BOX*2);
    rectFill(OFFSET*9.5, OFFSET*3 + ALT_BOX*2, OFFSET*11.5, OFFSET*4 + ALT_BOX*2);
    //Checkbox da quantozacao
    color(0,0,0);
    line(OFFSET + 10, OFFSET*4.5 + ALT_BOX*2, OFFSET*1.8, OFFSET*4.5 + ALT_BOX*2);
    line(OFFSET + 10, OFFSET*5 + ALT_BOX*2, OFFSET*1.8, OFFSET*5 + ALT_BOX*2);
    line(OFFSET + 10, OFFSET*4.5 + ALT_BOX*2, OFFSET + 10, OFFSET*5 + ALT_BOX*2);
    line(OFFSET*1.8, OFFSET*4.5 + ALT_BOX*2, OFFSET*1.8, OFFSET*5 + ALT_BOX*2);
    //Textos dos botoes
    text(OFFSET + 10, OFFSET*3 + ALT_BOX*2 + 5, "Limpar");
    text(OFFSET*4 +10, OFFSET*3 + ALT_BOX*2 + 5, "Seno");
    text(OFFSET*6.5 +10, OFFSET*3 + ALT_BOX*2 + 5, "Cosseno");
    text(OFFSET*9.5 +10, OFFSET*3 + ALT_BOX*2 + 5, "Random");
    text(OFFSET*2 , OFFSET*4.5 + ALT_BOX*2 + 5, "Quantizacao");
    //Divisao dos valores positivos e negativos da funcoes
    text(5, OFFSET, "-100");
    text(15, OFFSET*3 + 15, "0");
    text(5, OFFSET*6, "100");
    text(5, OFFSET*7, "-100");
    text(15, OFFSET*9 + 15, "0");
    text(5, OFFSET*12, "100");
    //Textos dos retangulos
    color(0, 0, 1);
    text(OFFSET + LAG_BOX*0.3, OFFSET*1.2 + ALT_BOX, "Original");
    color(1, 0, 0);
    text(OFFSET + LAG_BOX*0.5, OFFSET*1.2 + ALT_BOX, "+ iDCT");
    color(0, 1, 0);
    text(OFFSET + LAG_BOX*0.4, OFFSET*2.2 + ALT_BOX*2, "DCT");
    //Checkbox da quantizacao marcado ou nao
    if(quant_bool == true){
        color(0,0,0);
        line(OFFSET +10, OFFSET*4.5 + ALT_BOX*2, OFFSET * 1.8, OFFSET*5 + ALT_BOX*2);
        line(OFFSET +10, OFFSET*5 + ALT_BOX*2, OFFSET * 1.8, OFFSET*4.5 + ALT_BOX*2);
    }
}

//Calculo da DCT
void DCT(){
    double soma;
    color(1,0,1);
    for(int u=0;u<TAM;u++){
        soma = 0.0;
        for(int x=0;x<TAM;x++){
            //As funcoes ortogonais ja possuem o calculo feito
            //e sao apenas aplicados ao sinal original. Caso desativo,
            //nao sera aplicado a soma no sinal.
            soma += ortogonais[u][x*30] * sinal_original[x];
        }
        if(amostra_bool[u] == true)
            sinal_dct[u] = sqrt(2.0*0.125) * soma;
        else
            sinal_dct[u] = 0.0;
    }
}

//Calculo da iDCT
void iDCT(){
    double soma;
    for(int x=0;x<TAM;x++){
        soma = 0;
        for(int u=0;u<TAM;u++){
            if (quant_bool == true){
                //Se a quantizacao estiver ativa, ela sera aplicada aqui
                //sobre o vetor quantiz e a soma a funcao ortogonal
                double temp = quantizada[u] * quantiz[u];
                soma+= temp * ortogonais[u][x*30];
            }
            else
                //O calculo eh aplicado sobre o vetor da DCT.
                soma+= sinal_dct[u] * ortogonais[u][x*30];
        }
        sinal_idct[x] = sqrt(2*0.125) * soma;
    }
}

//Funcao predefinida do Seno, utilizada pelo botao
void FuncSeno(){
    float a = 2*PI/TAM;
    for(int i=0;i<TAM;i++){
        sinal_original[i] = sin(a*i)*TAM*10 + OFFSET + ALT_BOX*0.3;
    }
}

//Funcao predefinida do Cosseno, utilizada pelo botao
void FuncCos(){
    float a = 2*PI/TAM;
    for(int i=0;i<TAM;i++){
        sinal_original[i] = cos(a*i)*TAM*10 + OFFSET + ALT_BOX*0.3;
    }
}

//Funcao predefinida de gerar valores aleatorios, utilizada pelo botao
void FuncRandom(){
    int rand_num;
    for(int i=0;i<TAM;i++){
        //valor de -100 a 100
        rand_num = (rand() % 201 - 100) + 100;
        sinal_original[i] = rand_num;
    }
}

//Funcao para aplicar a quantizacao dos sinais, utilizada pelo checkbox
void Quantizacao(){
    for(int i=0;i<TAM;i++){
        quantizada[i] = (int)(sinal_dct[i] / quantiz[i]);
    }
}

//Checagem do botao de quantizacao
void Ativar_Quant(){
    if(quant_bool == true){
        quant_bool = false;
    }
    else{
        quant_bool = true;
    }
}

//Tela onde eh feito os calculos unidimensionais
//USADA COM A TECLA '1'
void uniDimensional(){
    clear(1,1,1);
    DCT();
    iDCT();
    if(quant_bool == true)
        Quantizacao();
}

//Local de renderizacao dos pontos e linhas de todos os vetores do programa
void renderPoints(){
    //Linhas de divisao
    color(0.5,0.5,0.5);
    line(OFFSET, OFFSET + ALT_BOX/2, OFFSET + LAG_BOX, OFFSET + ALT_BOX/2);
    line(OFFSET, OFFSET*2 + ALT_BOX/2 + ALT_BOX, OFFSET + LAG_BOX, OFFSET*2 + ALT_BOX/2 + ALT_BOX);
    //Render do sinal original e iDCT
    color(0, 0, 1);
    int cont = DIM;
    for(int i=0;i<TAM;i++){
        cont++;
        if(cont >= DIM){
            //Divisao para marcar os pontos de sinais
            circleFill( i*(60/DIM) + OFFSET*2, sinal_original[i] + OFFSET, 8 , 4);
            char a[10];
            sprintf( a, "%d", (int)(sinal_original[i]-100));
            text(i*(60/DIM) + OFFSET*2 + 5, sinal_original[i] + OFFSET + 5, a);
            cont = 0;
        }
        //Ligar com linhas os pontos das funcoes
        if(i<TAM-1){
            line(i*(60/DIM) + OFFSET*2, sinal_original[i] + OFFSET, (i+1)*(60/DIM) + OFFSET*2 , sinal_original[i+1] + OFFSET);
        }
    }
    //Os mesmos passos se repete para as proximas linhas de codigo
    color(1, 0, 0);
    cont = DIM;
    for(int i=0;i<TAM;i++){
        cont++;
        if(cont >= DIM){
            circleFill( i*(60/DIM) + OFFSET*2, sinal_idct[i] + OFFSET, 4 , 4);
            cont = 0;
        }
        if(i<TAM-1){
            line(i*(60/DIM) + OFFSET*2, sinal_idct[i] + OFFSET, (i+1)*(60/DIM) + OFFSET*2 , sinal_idct[i+1] + OFFSET);
        }
    }
    //Render do sinal DCT (parte de cima)
    color(0, 1, 0);
    for(int i=0;i<TAM;i++){
        if(quant_bool == true){
            circleFill( i*(60/DIM) + OFFSET*2, quantizada[i]/3 + 2*OFFSET + (ALT_BOX*0.5) + ALT_BOX, 5 , 4);
            if(i<TAM-1)
                line(i*(60/DIM) + OFFSET*2, quantizada[i]/3 + 2*OFFSET+(ALT_BOX*0.5) + ALT_BOX, (i+1)*(60/DIM) + OFFSET*2 , quantizada[i+1]/3 + 2*OFFSET+(ALT_BOX*0.5) + ALT_BOX);
        }
        else{
            circleFill( i*(60/DIM) + OFFSET*2, sinal_dct[i]/3 + 2*OFFSET + (ALT_BOX*0.5) + ALT_BOX, 5 , 4);
            if(i<TAM-1)
                line(i*(60/DIM) + OFFSET*2, sinal_dct[i]/3 + 2*OFFSET+(ALT_BOX*0.5) + ALT_BOX, (i+1)*(60/DIM) + OFFSET*2 , sinal_dct[i+1]/3 + 2*OFFSET+(ALT_BOX*0.5) + ALT_BOX);
        }
    }
    //Render das amostras ortogonais
    color(1, 0, 1);
    for(int i=0;i<4;i++){
        for(int j=0;j<240;j++){
            //O for vai ate 240 (tamanho x determinado da funcoes) e eh desenhado um ponto
            //a cada 30 pixeis para delimitar uma das amostras ortogonais
            if(j%30 == 0)
                circleFill( 10 + j + 2*OFFSET +LAG_BOX , ortogonais[i][j]*30 + 2*OFFSET + ALT_BOX2*i, 5 , 4);
            if(j<210)
                line(10 + j + 2*OFFSET +LAG_BOX, ortogonais[i][j]*30 + 2*OFFSET + ALT_BOX2*i, 10 + (j+1) + 2*OFFSET +LAG_BOX , ortogonais[i][j+1]*30 + 2*OFFSET + ALT_BOX2*i);
        }
    }
    //O mesmo se repete para as outras 4 funcoes ortogonais que ficam ao lado
    color(1, 0, 1);
    for(int i=0;i<4;i++){
        for(int j=0;j<240;j++){
            if(j%30 == 0)
                circleFill( 10 + j + 4*OFFSET + LAG_BOX*1.5 , ortogonais[i+4][j]*30 + 2*OFFSET + ALT_BOX2*i, 5 , 4);
            if(j<210)
                line(10 + j + 4*OFFSET + LAG_BOX*1.5, ortogonais[i+4][j]*30 + 2*OFFSET + ALT_BOX2*i, 10 + (j+1) + 4*OFFSET +LAG_BOX*1.5 , ortogonais[i+4][j+1]*30 + 2*OFFSET + ALT_BOX2*i);
        }
    }
}

//Tela onde eh aplicada as funcoes bidimensionais (IMPLEMENTADO APENAS EM UMA MATRIZ, SEM IMAGEM)
//USADA COM A TECLA '2'
void biDimensional(){
    clear(1,1,1);
    int u, v, x, y;
    float pix, cor;
    for(u=0; u<8; u++)
        for(v=0; v<8; v++)
            for(x=0; x<8; x++)
                for(y=0; y<8; y++){
                    pix = cos(((2*x+1)*PI*u)/16.0) * cos(((2*y+1)*PI*v)/16.0);
                    //normaliza entre 0 e 2
                    cor = pix+1;
                    //normaliza entre 0 e 255
                    cor = cor*127;
                    cor = cor/255;
                    color(cor,cor,cor);
                    point(((u*10 + x)+ 50), ((v*10+y))+75);
                }
}

//Renderizacao entre unidimensional, bidimensional e criacao da interface
void render(){
    CriaInterface();
    if( opcao == 49 ){
        uniDimensional();
        color(0,0,0);
        text(OFFSET, ALT_TELA - OFFSET-20, "DCT Unidimensional");
        text(LAG_TELA - LAG_BOX, ALT_TELA - OFFSET-20, "Funcoes Ortogonais");
    }
    if( opcao == 50 ){
        biDimensional();
        color(0,0,0);
        text(OFFSET, ALT_TELA - OFFSET-20, "DCT Bidimensional");
    }
    renderPoints();
}

//Funcao da chamada do teclado (PADRAO)
void keyboard(int key){
   if( key <= 50 && key >= 49 ){
      opcao = key;
   }
   switch(key){
      case 27:
	     exit(0);
	  break;
   }
}

//Funcao da chamada de liberacao de uma tecla
void keyboardUp(int key)
{

}

//Funcao de tratamento do mouse. Checagem do lugar do clique e aplicada as funcoes dos botoes.
void mouse(int button, int state, int x, int y){
    if( (button == 0) && (state == 0))
        acao = 1;
    if ((button == 0) && (state == 1))
        acao = 0;
    y = ALT_TELA - y;
    //Se o mouse estiver pressionado, ele podera ser arrastado e alterado os valores do local
    if( acao == 1){
        //Checagem se o mouse foi clicado no retangulo do sinal original
        if( (x>OFFSET) && (x<LAG_BOX + OFFSET) && (y>OFFSET) && (y<ALT_BOX + OFFSET)){
            for(int i=0;i<TAM;i++){
                if((x > OFFSET + i*(60/DIM)) && (x < OFFSET*3 + i*(60/DIM))){
                    //Altera do valor do sinal original conforme o local do clique
                    sinal_original[i*DIM] = y - OFFSET;
                }
            }
        }
    }
    //Se o botao do mouse for pressionado
    if(state == 0){
        //Checagem se algum botao foi clicado
        if( (y > OFFSET*3 + ALT_BOX*2) && (y < OFFSET*4 + ALT_BOX*2)){
            if( (x > OFFSET) && (x < OFFSET*3)){
                PreencherValores();
            }
            if( (x > OFFSET*4) && (x < OFFSET*5.5)){
                FuncSeno();
            }
            if( (x > OFFSET*6.5) && (x < OFFSET*8.5)){
                FuncCos();
            }
            if( (x > OFFSET*9.5) && (x < OFFSET*11.5)){
                FuncRandom();
            }
        }
        //Clicar nas funcoes ortogonais e altera os estados delas
        if( (x > 2*OFFSET + LAG_BOX) && ( x < 2*OFFSET+LAG_BOX + LAG_BOX2) ){
            if( (y > OFFSET) && (y < OFFSET*0.2 + ALT_BOX2)){
                if (amostra_bool[0] == true)
                    amostra_bool[0] = false;
                else
                    amostra_bool[0] = true;
            }
            if( (y > OFFSET + ALT_BOX2) && (y < ALT_BOX2*2)){
                if (amostra_bool[1] == true)
                    amostra_bool[1] = false;
                else
                    amostra_bool[1] = true;
            }
            if( (y > OFFSET + ALT_BOX2*2) && (y < ALT_BOX2*3)){
                if (amostra_bool[2] == true)
                    amostra_bool[2] = false;
                else
                    amostra_bool[2] = true;
            }
            if( (y > OFFSET + ALT_BOX2*3) && (y < ALT_BOX2*4)){
                if (amostra_bool[3] == true)
                    amostra_bool[3] = false;
                else
                    amostra_bool[3] = true;
            }
        }
        if( (x > 4*OFFSET + LAG_BOX + LAG_BOX2) && ( x < 4*OFFSET+LAG_BOX + LAG_BOX2*2) ){
            if( (y > OFFSET) && (y < OFFSET*0.2 + ALT_BOX2)){
                if (amostra_bool[4] == true)
                    amostra_bool[4] = false;
                else
                    amostra_bool[4] = true;
            }
            if( (y > OFFSET + ALT_BOX2) && (y < ALT_BOX2*2)){
                if (amostra_bool[5] == true)
                    amostra_bool[5] = false;
                else
                    amostra_bool[5] = true;
            }
            if( (y > OFFSET + ALT_BOX2*2) && (y < ALT_BOX2*3)){
                if (amostra_bool[6] == true)
                    amostra_bool[6] = false;
                else
                    amostra_bool[6] = true;
            }
            if( (y > OFFSET + ALT_BOX2*3) && (y < ALT_BOX2*4)){
                if (amostra_bool[7] == true)
                    amostra_bool[7] = false;
                else
                    amostra_bool[7] = true;
            }
        }
        //Checkbox de quantizacao
        if( ( x > OFFSET + 10 ) && ( x < OFFSET * 1.8 ) ){
            if( (y > OFFSET*4.5 + ALT_BOX*2) && ( y < OFFSET*5 + ALT_BOX*2) ){
                Ativar_Quant();
            }
        }
    }
}

int main(void){
    srand (time(NULL));
    PreencherValores();
    PreencherAmostras();
    initCanvas(LAG_TELA, ALT_TELA);
    runCanvas();
}

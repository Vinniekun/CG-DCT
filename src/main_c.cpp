/*********************************************************************
// Canvas para desenho - Versao C.
//  Autor: Cesar Tadeu Pozzer
//         10/2007
//
//  Pode ser utilizada para fazer desenhos ou animacoes, como jogos simples.
//  Tem tratamento de mosue
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Instruções:
//	  Para alterar a animacao, digite numeros entre 1 e 5
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gl_canvas2d.h"
#define LAG_TELA 1280
#define ALT_TELA 720
#define LAG_BOX 500
#define ALT_BOX 200
#define LAG_BOX2 300
#define ALT_BOX2 200
#define OFFSET 40
#define PI 3.14


//variaveis globais
int   opcao  = 49;
float global = 0;
int sinal_original[8] = {0,50,75,100,0,-50,-100,-70};
int sinal2_dct[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int sinal2_idct[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void CriaInterface()
{
    //Retangulos Grandes
    color(0.9f,0.9f,0.9f);
    rectFill( OFFSET, OFFSET, LAG_BOX + OFFSET, ALT_BOX + OFFSET);
    color(0.9f,0.9f,0.9f);
    rectFill( OFFSET, 2*OFFSET + ALT_BOX, OFFSET + LAG_BOX, 2*ALT_BOX + 2*OFFSET );

    //Retangulos pequenos, Amostras
    color(0.9f,0.9f,0.9f);
    rectFill( 2*OFFSET + LAG_BOX, OFFSET, 2*OFFSET+LAG_BOX + LAG_BOX2, ALT_BOX2 );

    color(0.9f,0.9f,0.9f);
    rectFill( 3*OFFSET+LAG_BOX + LAG_BOX2, OFFSET, 3*OFFSET + LAG_BOX + 2*LAG_BOX2, ALT_BOX2 );


}

double C(int w){
    if(w == 0)
        return 0.70710678118654752440084436210485; // 1/raiz(2)
    else
        return 1;
}

void DCT(){
    double soma;

    //Calculo DCT uni
    for(int u=0;u<8;u++){
        soma = 0;
        for(int x=0;x<8;x++){
            soma+= sinal_original[x] * cos(((2*x+1)*u*PI)/(2*8)) * C(u);
        }
        sinal_dct[x] = sqrt(2*0.125) * soma;
    }

}

void iDCT(){
    double soma;

    //Calculo DCT uni
    for(int x=0;x<8;x++){
        soma = 0;
        for(int u=0;u<8;u++){
            soma+= sinal_dct[u] * cos(((2*x+1)*u*PI)/(2*8)) * C(u);
        }
        sinal_idct[x] = sqrt(2*0.125) * soma;
    }



}

void uniDimensional(){
    clear(1,1,1);






}

void renderPoints(){
    color(0,0,0);
    //Primeira, de baixo
    for(int i=0;i<8;i++){
        circleFill( i*60 + OFFSET*2, sinal_original[i] + OFFSET+ALT_BOX*0.5, 5 , 10);
        line(i*60 + OFFSET*2, sinal_original[i] + OFFSET+ALT_BOX*0.5, (i+1)*60 + OFFSET*2 , sinal_original[i+1] + OFFSET+ALT_BOX*0.5);
    }
    //Segunda, de cima
    for(int i=0;i<8;i++){
        circleFill( i*60 + OFFSET*2, sinal_dct[i] + 2*OFFSET + (ALT_BOX*0.5) + ALT_BOX, 5 , 10);
        line(i*60 + OFFSET*2, sinal_dct[i] + 2*OFFSET+(ALT_BOX*0.5) + ALT_BOX, (i+1)*60 + OFFSET*2 , sinal_dct[i+1] + 2*OFFSET+(ALT_BOX*0.5) + ALT_BOX);
    }


}

void biDimensional(){
    clear(1,1,1);
    int u, v, x, y;
    float pix, cor;

    float pi = 3.14;
    for(u=0; u<8; u++)
        for(v=0; v<8; v++)
            for(x=0; x<8; x++)
                for(y=0; y<8; y++){
                    //pix vale entre -1 e 1.
                    pix = cos(((2*x+1)*pi*u)/16.0) * cos(((2*y+1)*pi*v)/16.0);
                    //normaliza entre 0 e 2
                    cor = pix+1;
                    //normaliza entre 0 e 255
                    cor = cor*127;
                    cor = cor/255;
                    color(cor,cor,cor);
                    point(((u*10 + x)+ 50), ((v*10+y))+75);
                }

}

void render()
{
    CriaInterface();

    if( opcao == 49 ) // = '8' MINHA FUNC
    {
        uniDimensional();
        color(0,0,0);
        text(20,20,"DCT Unidimensional.");
    }

    if( opcao == 50 ) // = '7' senoide
    {
        biDimensional();
        color(0,0,0);
        text(20,20,"DCT Bidimensional.");

    }
    renderPoints();

}


//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
   //printf("\nTecla: %d" , key);
   if( key <= 50 && key >= 49 )
   {
      opcao = key;
   }

   switch(key)
   {
      case 27:
	     exit(0);
	  break;
   }
}
//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   //printf("\nLiberou: %d" , key);
}


//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int x, int y)
{
   y = LAG_TELA - y;
   //printf("\nmouse %d %d %d %d", button, state, x, y);

   if( state == 0 ) //atirou
   {
      printf("\nDisparou");
   }

}

int main(void)
{
   initCanvas(LAG_TELA, ALT_TELA);
   runCanvas();
}

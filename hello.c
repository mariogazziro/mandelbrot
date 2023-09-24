#include "neslib.h"
//#link "chr_generic.s"
#define MAXITERATE 16 //numero maximo de iteracoes
#define NORM_BITS 13 //fator de normalizacao dos reais para inteiros
#define NORM ((long) 1 << NORM_BITS) //fator de 13 bits de normalizacao
int iterate(long real0, long imag0) {
   long realq, imagq, real, imag;
   int i;
   real = real0;
   imag = imag0;
   for (i = 0; i < MAXITERATE; i++) {
     realq = (real * real) >> NORM_BITS; //elevando ao quadrado...
     imagq = (imag * imag) >> NORM_BITS; //...o ultimo elemento.
     if ((realq + imagq) > 4 * NORM)
        break;   //(se o valor exceder o limiar, sair do funcao) 
     imag = ((real * imag) >> (NORM_BITS - 1)) + imag0; //adicionando...
     real = realq - imagq + real0; //...seu numero inicial
   }
   return i;
}
void mand_calc(long realmin, long imagmin, long realmax, long imagmax, long hres, long vres) 
{
  int deltareal, deltaimag, real0,  imag0, x, y, z;
  deltareal = (realmax - realmin) / hres; //define o passo em funcao...
  deltaimag = (imagmax - imagmin) / vres; //...da resolucao do fractal
  real0 = realmin;
  for (y = 1; y <= vres; y++) { //varredura na resolucao...
    imag0 = imagmax;
    for (x = 1; x <= hres; x++) { //...dos blocos, 32x30 ao todo
      z=iterate(real0, imag0);
      if (z>=MAXITERATE) {//se excedeu o limiar, insirir um bloco escuro
        ppu_off(); //desliga o controlador grafico antes de escrever
        vram_adr(NTADR_A(x,y)); //ajusta as coordenadas na tela 
        vram_put(1); //desenha o bloco escuro nas coordenadas
        ppu_on_bg(); //liga o controlador grafico novamente e exibe tela
      }
      imag0 = imag0 - deltaimag; //atualiza os indices complexos...
    }
    real0 = real0 + deltareal; // ...para a proxima iteracao
  }
}
void main(void) {
  long C[]={-2,-1,1,1}; //coordenadas da janela no plano complexo
  int width = 32, height = 30; //resolucao do fractal em pixels
  pal_col(0,0xA); //ajusta a tela de fundo para verde escuro    
  mand_calc(C[0]*NORM,C[1]*NORM,C[2]*NORM,C[3]*NORM,width,height);
  while (1); //a funcao acima multiplica os parametros reais pela norma
}

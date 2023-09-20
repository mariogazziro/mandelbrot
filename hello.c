#include "neslib.h"
//#link "chr_generic.s"
#define MAXITERATE 16
#define NORM_BITS 13
#define NORM_FACT ((long) 1 << NORM_BITS)

int iterate(long real0, long imag0)
{
   long realq, imagq, real, imag;
   int i;
   real = real0;
   imag = imag0;
   for (i = 0; i < MAXITERATE; i++) {
     realq = (real * real) >> NORM_BITS;
     imagq = (imag * imag) >> NORM_BITS;
     if ((realq + imagq) > 4 * NORM_FACT)
        break;
     imag = ((real * imag) >> (NORM_BITS - 1)) + imag0;
     real = realq - imagq + real0;
   }
   return i;
}

void mand_calc(long realmin, long imagmin, long realmax, long imagmax, long hres, long vres)
{
  int deltareal, deltaimag, real0,  imag0;
  int x, y, z;
  deltareal = (realmax - realmin) / hres;
  deltaimag = (imagmax - imagmin) / vres;
  real0 = realmin;
  for (y = 1; y <= vres; y++) {
    imag0 = imagmax;
    for (x = 1; x <= hres; x++) {
      z=iterate(real0, imag0);
      if (z>=MAXITERATE) {
        ppu_off();
        vram_adr(NTADR_A(x,y));   
        vram_put(1);
        ppu_on_bg();
      }
      imag0 = imag0 - deltaimag;
    }
    real0 = real0 + deltareal;
  }
}

void main(void) {
  long int bbox[]={-2,-1,1,1}; //realmin, imagmin, realmax, imagmax
  int width = 32, height = 30; // pixel resolution
  pal_col(0,0xA); // set screen to light green  
  mand_calc(bbox[0]*NORM_FACT, bbox[1]*NORM_FACT, bbox[2]*NORM_FACT, bbox[3]*NORM_FACT, width, height);
  while (1); // infinite loop
}

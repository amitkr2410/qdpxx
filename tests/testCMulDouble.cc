#include "qdp.h"
#include "testCMulDouble.h"
#include "unittest.h"
#include <cmath>

using namespace QDP;
using namespace std;
using namespace Assertions;




#include<xmmintrin.h>


typedef union {
  __m128d v;
  double  d[2];
} VD;

#if 0
#define CMUL(z,x,y)		\
  { \
    __m128d t1,t2,t3,t4; \
    t1 = _mm_mul_pd(x,y); \
    t2 = _mm_shuffle_pd(t1,t1,0x1); \
    t3 = _mm_shuffle_pd(y,y,0x1);\
    z = _mm_sub_pd(t1,t2); \
    t2 = _mm_mul_pd(x,t3); \
    t3 = _mm_shuffle_pd(t2,t2,0x1); \
    t3 = _mm_add_pd(t2,t3); \
    z= _mm_shuffle_pd(z,t3,0x2); \
  }

#define CMADD(z,x,y)				\
  { \
    __m128d t1,t2,t3,t4; \
    t1 = _mm_mul_pd(x,y); \
    t2 = _mm_shuffle_pd(t1,t1,0x1); \
    t3 = _mm_shuffle_pd(y,y,0x1);\
    t4 = _mm_sub_pd(t1,t2); \
    t2 = _mm_mul_pd(x,t3); \
    t3 = _mm_shuffle_pd(t2,t2,0x1); \
    t3 = _mm_add_pd(t2,t3); \
    t4= _mm_shuffle_pd(t4,t3,0x2); \
    z = _mm_add_pd(z,t4); \
  }
#endif
#include <pmmintrin.h>

/* SSE 3? */
#define CMUL(z,x,y)		\
  { \
    __m128d t1,t2; \
    t1 = _mm_mul_pd((x),(y)); \
    (z) = _mm_hsub_pd(t1,t1);			\
    t2 = _mm_shuffle_pd((y),(y),0x1);\
    t2 = _mm_mul_pd((x),t2); \
    t1 = _mm_hadd_pd(t2,t2); \
    (z)= _mm_shuffle_pd((z),t1,0x2);		\
  }

#define CMADD(z,x,y)				\
  { \
    __m128d t1,t2; \
    t1 = _mm_mul_pd((x),(y)); \
    t1 = _mm_hsub_pd(t1,t1); \
    t2 = _mm_shuffle_pd((y),(y),0x1);\
    t2 = _mm_mul_pd((x),t2); \
    t2 = _mm_hadd_pd(t2,t2); \
    t1= _mm_shuffle_pd(t1,t2,0x2);		\
    (z) = _mm_add_pd((z),t1);			\
  }


void
testCMul::run()
{
  DComplex x=cmplx(Double(1.5),Double(4.0));
  DComplex y=cmplx(Double(2.5),Double(3.0));
  DComplex z1 = x*y;

  VD x_v;
  VD y_v;

  x_v.d[0] = toDouble(real(x));
  x_v.d[1] = toDouble(imag(x));

  y_v.d[0] = toDouble(real(y));
  y_v.d[1] = toDouble(imag(y));

  VD z_v;

  CMUL(z_v.v, x_v.v, y_v.v);

  DComplex z2=cmplx(Real(z_v.d[0]), Real(z_v.d[1]));

  double realdiff = fabs(toDouble( real(z2-z1) ));
  QDPIO::cout << endl << "Real diff = " << realdiff << endl;

  double imagdiff = fabs(toDouble( imag(z2-z1) ));
  QDPIO::cout << "Imag diff = " << imagdiff << endl;

  assertion( realdiff < 1.0e-14) ;
  assertion( imagdiff < 1.0e-14) ;
}

void
testCMadd::run()
{
  DComplex x=cmplx(Double(1.5),Double(4.0));
  DComplex y=cmplx(Double(2.5),Double(3.0));
  DComplex z1 = cmplx(Double(4), Double(-0.5));

 
  VD z_v;
  VD x_v;
  VD y_v;

  z_v.d[0] = toDouble(real(z1));
  z_v.d[1] = toDouble(imag(z1));

  z1 += x*y;

  x_v.d[0] = toDouble(real(x));
  x_v.d[1] = toDouble(imag(x));

  y_v.d[0] = toDouble(real(y));
  y_v.d[1] = toDouble(imag(y));


  CMADD(z_v.v, x_v.v, y_v.v);

  DComplex z2=cmplx(Real(z_v.d[0]), Real(z_v.d[1]));

  double realdiff = fabs(toDouble( real(z2-z1) ));
  QDPIO::cout << endl << "Real diff = " << realdiff << endl;

  double imagdiff = fabs(toDouble( imag(z2-z1) ));
  QDPIO::cout << "Imag diff = " << imagdiff << endl;

  assertion( realdiff < 1.0e-14) ;
  assertion( imagdiff < 1.0e-14) ;
}

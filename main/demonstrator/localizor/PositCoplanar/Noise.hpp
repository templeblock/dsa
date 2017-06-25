/*
 * Noise.hpp
 *
 *  Created on: 16.02.2013
 *      Author: user
 */

#ifndef NOISE_HPP_
#define NOISE_HPP_

namespace PositCoplanar {

using namespace PositCoplanar;

#include <math.h>

#include <PositCoplanar/Error.hpp>
#include <PositCoplanar/SVD.hpp>

/*******************************************************************************************************/
/*Fonction tiree de l'ouvrage "Numerical Recipes" de W.H. Press, B.P. Flannery, S.A. Teukolsky, et*/
/*W.T. Vetterling, Cambridge University Press*/

#define M1 259200
#define IA1 7141
#define IC1 54773
#define RM1 (1.0/M1)
#define M2 134456
#define IA2 8121
#define IC2 28411
#define RM2 (1.0/M2)
#define M3 243000
#define IA3 4561
#define IC3 51349

double ran1(long int *idum)
{
        static long ix1,ix2,ix3;
        static double r[98];
        double temp;
        static long int iff=0;
        long int j;

        if (*idum < 0 || iff == 0) {
                iff=1;
                ix1=(IC1-(*idum)) % M1;
                ix1=(IA1*ix1+IC1) % M1;
                ix2=ix1 % M2;
                ix1=(IA1*ix1+IC1) % M1;
                ix3=ix1 % M3;
                for (j=1;j<=97;j++) {
                        ix1=(IA1*ix1+IC1) % M1;
                        ix2=(IA2*ix2+IC2) % M2;
                        r[j]=(ix1+ix2*RM2)*RM1;
                }
                *idum=1;
        }
        ix1=(IA1*ix1+IC1) % M1;
        ix2=(IA2*ix2+IC2) % M2;
        ix3=(IA3*ix3+IC3) % M3;
        j=1 + ((97*ix3)/M3);
        if (j > 97 || j < 1) nrerror("RAN1: This cannot happen.");
        temp=r[j];
        r[j]=(ix1+ix2*RM2)*RM1;
        return (temp);
}

#undef M1
#undef IA1
#undef IC1
#undef RM1
#undef M2
#undef IA2
#undef IC2
#undef RM2
#undef M3
#undef IA3
#undef IC3



}


#endif /* NOISE_HPP_ */

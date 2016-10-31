#ifndef RANDOM_H
#define	RANDOM_H

#include <cmath>      /* log, sqrt */

extern  void init_genrand(unsigned long s);
extern  void init_by_array(unsigned long init_key[], int key_length);
extern  double genrand_real3(); /* generates a random number on (0,1)-real-interval */

inline double uniform()
{
    return genrand_real3();
}
inline double uniform(double a, double b)
{
    return a + (b - a) * uniform();
}


/*
 * Polar form of Box-Muller(1958) transformation.
 * It is both faster and more robust numerically than standard version.
 * The polar form is faster because it does the equivalent of the sine and cosine
 * geometrically without a call to the trigonometric function library.
 */
inline double gaussian(double m, double s)
{
    double v1, v2, w, y;
    double x1;//, x2;

    do
    {
        v1 = 2.0 * uniform() - 1.0;
        v2 = 2.0 * uniform() - 1.0;
        w = (v1 * v1) + (v2 * v2);
    }
    while (w > 1.0);

    y = sqrt((-2.0 * log(w)) / w);
    x1 = v1 * y;
    //x2 = v2 * y;
    return (m + x1 * s);
}

inline double normal(void)
{
    return (gaussian(0.0, 1.0));
}

#endif	/* RANDOM_H */
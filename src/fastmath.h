#ifndef FAST_MATH_H
#define	FAST_MATH_H

#define SQRT_MAGIC_D 0x5fe6ec85e7de30da

namespace fastmath
{
    inline double fast_invsqrt(double x)
    {
        union // get bits for floating value
        {
            double x;
            long i;
        } u;

        const double xhalf = 0.5d * x;
        u.x = x;
        u.i = SQRT_MAGIC_D - (u.i >> 1);  // gives initial guess y0
//        u.x = u.x*(1.5d - xhalf*u.x*u.x);
        u.x = u.x * (1.5008908d - xhalf * u.x * u.x); // Correction following Hacker's delight, 17-4
        return u.x * (1.5d - xhalf * u.x * u.x); // Newton step, repeating increases accuracy
    }

    inline double fast_sqrt(double x)
    {
        return x * fast_invsqrt(x);
    }
}

#endif

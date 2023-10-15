/*
  ==============================================================================

    Compressor.cpp
    Created: 11 Oct 2023 10:29:30pm
    Author:  Samuel Vergara Alvarez

  ==============================================================================
*/

#include "Limiter.h"
#include <cmath>
Limiter::Limiter():
yn1(0),
alphaat(0),
alphare(0)
{}

float Limiter::Limiting(float xn)
{
    
    float xdB = 20*log10(abs(xn));
    float xg,yg,xl,yl = 0;
    
    
    
    if (xdB < -120)
    {
        xg = -120;
        
    }
    else
    {
        xg = xdB;
    }

    if (xg >= TH)
    {
        yg = TH;
    }
    else
    {
        yg = xg;
    }
    

    xl =  xg - yg ;

    if (xl > yn1)
    {
        yl = (alphaat * yn1) + (1 - alphaat)*xl;
    }
    else
    {
        yl = (alphare * yn1) + (1 -  alphare)*xl;
    }
    yn1 = yl;
    
    
    float c = pow(10,(-yl/20));

    return (xn * c);
}


void Limiter::setLimiter(float th,float atime,float rtime,float Fs)
{
    TH = th;
    
    alphaat = exp(-log(9)/(Fs * atime));
    alphare = exp(-log(9)/(Fs * rtime));
}

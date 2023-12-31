/*
  ==============================================================================

    Compressor.cpp
    Created: 11 Oct 2023 10:29:30pm
    Author:  Samuel Vergara Alvarez

  ==============================================================================
*/

#include "Compressor.h"
#include <cmath>
Compressor::Compressor():
yn1(0),
alphaat(0),
alphare(0),
makeup(0),
Fsamp(44.1e3)
{}

float Compressor::Compressing(float xn, double delta)
{
    
    Autoballistic(delta);
    float xdB = 20*log10(abs(xn+1e-8));
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
        yg = TH + (xg - TH)*(1/R);
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
    

    float Mu = pow(float (10),float (makeup)/20);
    c = pow(float (10),float (-yl)/20);
    
    float reducc = xn * c;
    
   

    return (por * Mu)*reducc + (1 - por)*reducc;
}

void Compressor::Autoballistic(double delta)
{   float absDelta = abs(delta);
    float maxDelta = 0.6;
    
    if (absDelta > maxDelta)
    {
        delta = maxDelta*absDelta/delta;
    }
    
    float attackTime = Tamax*(1-(2 * float (std::max(0.0, delta)))); //Tamax y Trmax LO MODIFICA EL USUARIO
    float releaseTime = Trmax*(1+(2 * float (std::min(0.0, delta))));

    alphaat = exp(-1/(1e-3 * Fsamp * attackTime));
    alphare = exp(-1/(1e-3 * Fsamp * releaseTime));
}

void Compressor::calculateMakeUp(float valuein)
{
    double Md = -TH*(1-(1/R));
    float g = Md * (1 - (valuein / meanTarget));
    
    if(abs(g) > Md)
    {
        makeup = Md * abs(g)/g;
    }
    else
    {
        makeup = g;
    }
    
}

void Compressor::setAutoM(float Auto)
{
    
    
    por = Auto;
}

void Compressor::setARtmax(float at, float rt)
{
    Tamax = at;
    Trmax = rt;
}

void Compressor::setTHandR(float th, float r,float Fs)
{
    TH = th;
    R = r;
    Fsamp = Fs;
}

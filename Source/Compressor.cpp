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
        yg = TH + (xg - TH)*(1 - (1/R));
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
    
    

//%              MAKE UP GAIN
//%             Md=-obj.TH*(1-(1/obj.R));
//%             Mu = 10^(Md/20);

    float Mu = pow(10,(makeup/20));
    float c = pow(10,(-yl/20));

    return (xn * c)*Mu;
}

void Compressor::Autoballistic(double delta)
{
    float attackTime = Tamax*(1-(2 * std::max(0.0, delta))); //Tamax y Trmax LO MODIFICA EL USUARIO
    float releaseTime = Trmax*(1+(2 * std::min(0.0, delta)));

    alphaat = exp(-1/(1e-3 * Fsamp * attackTime));
    alphare = exp(-1/(1e-3 * Fsamp * releaseTime));
}

void Compressor::calculateMakeUp(float valuein)
{
    double Md = 9; // SE MODIFICA EN LA INTERFAZ GRÁFICA
    makeup = Md * (1 - (valuein / meanTarget));
    
    // Limitar el valor de makeup
    if (makeup < -Md)
    {
        makeup = -Md;
    }
}

void Compressor::setTHandR(float th, float r,float Fs)
{
    TH = th;
    R = r;
    Fsamp = Fs;
}

/*
  ==============================================================================

    Compressor.h
    Created: 11 Oct 2023 10:29:30pm
    Author:  Samuel Vergara Alvarez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include <cmath>
#include "Limiter.h"

class Compressor
{
public:
    Compressor();
    
    float Compressing(float xn, double delta);
    void Autoballistic(double delta);
    void calculateMakeUp(float valuein);
    void setTHandR(float Thresh, float Ratio, float fs);
    void setARtmax(float at,float rt);
    void setAutoM(float Auto);
    
    float Tamax = 100e-3;
    float Trmax = 200e-3;
    float c;
    
private:
    float yn1;
    float alphaat, alphare;
    float meanTarget = pow(float (10),float (-14)/20);
    float makeup;
    float TH, R, Fsamp;
    float por=1;
    
};


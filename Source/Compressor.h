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

class Compressor
{
public:
    Compressor();
    
    float Compressing(float xn, double delta, float fs);
    void Autoballistic(double delta, float fs);
    void calculateMakeUp(float valuein);
    void setTHandR(float Thresh, float Ratio);
    
    float Tamax, Trmax;

    
private:
    float yn1;
    float alphaat, alphare;
    float meanTarget = pow(10,-14/20);
    float makeup;
    float TH, R;
    
};


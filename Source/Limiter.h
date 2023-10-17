/*
  ==============================================================================

    Limiter.h
    Created: 15 Oct 2023 4:15:44pm
    Author:  Samuel Vergara Alvarez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include <cmath>

class Limiter
{
public:
    Limiter();
    
    float Limiting(float xn);
    void setLimiter(float Thresh, float aTime, float rTime,float fs);
        
    
private:
    float yn1;
    float alphaat, alphare;
    float TH;
};

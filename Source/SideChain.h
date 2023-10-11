/*
  ==============================================================================

    SideChain.h
    Created: 28 Sep 2023 4:13:41pm
    Author:  Samuel Vergara Alvarez

  ==============================================================================
*/
#include "JuceHeader.h"
#include <cmath>
#pragma once
class Filter
{
public:
    Filter();
    void setFc(float nFc);
    void setSRandFT(float nSampleRate, int fT);
    float processSample(float xn);

    void calCoeffHPF();
    void calCoeffLPF();
    void calCoeffAPF();

    
private:
//    float k;
    
    float Fc;
    float sampleRate;
    int filterType;
    float a0, a1, a2;
    float b1, b2;

    float xn1, xn2;
    float yn1, yn2;
    
    const float pi = juce::MathConstants<double>::pi;
};


class NonL
{
public:
    NonL();
    float TANH(float xn);
    float SIGMOID(float xn);
    void setSat(float k1);
private:
    float k;
    
};

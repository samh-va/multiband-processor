#pragma once
#include "JuceHeader.h"
#include <cmath>

class Filter
{
public:
    Filter();
    void setFc(float nFc);
    void setSRandFT(float nSampleRate, int fType);
    float processSample(float xn);

    void calCoeffHPF();
    void calCoeffLPF();
    void calCoeffAPF();


private:

    float fc;
    float sampleRate;
    int filterType;
    float a0, a1, a2;
    float b1, b2;

    float xn1, xn2;
    float yn1, yn2;

    const float pi = juce::MathConstants<double>::pi;
};

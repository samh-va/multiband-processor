/*
  ==============================================================================

    SideChain.cpp
    Created: 28 Sep 2023 4:31:23pm
    Author:  Samuel Vergara Alvarez

  ==============================================================================
*/

#include "SideChain.h"
Filter::Filter():
Fc(1000),sampleRate(44.1e3),
a0(0),a1(0), a2(0),
b1(0),b2(0),
xn1(0), xn2(0),
yn1(0), yn2(0)
{}

void Filter::setSRandFT(float nSampleRate, int fT)
{
    sampleRate = nSampleRate;
    filterType = fT;
}

void Filter::calCoeffHPF()
{
    
    float th = pi*Fc/sampleRate;
    float om = pi*Fc;
    float k = om/tan(th);
    float dlt = k*k + om*om + 2*k*om;
    
        a0 = k * k/dlt;
        a1 = -2 * k * k/dlt;
        a2 = k * k/dlt;
        b1 = ((-2 * k * k) + (2 * om * om)) / dlt ;
        b2 = ((-2 * k * om)+(k * k)+(om * om)) / dlt;
}

void Filter::calCoeffLPF()
{
    
    float th = pi*Fc/sampleRate;
    float om = pi*Fc;
    float k = om/tan(th);
    float dlt = k*k + om*om + 2*k*om;
    
        a0 = om * om/dlt;
        a1 = 2 * om * om/dlt;
        a2 = om * om/dlt;
        b1 = ((-2 * k * k) + (2 * om * om)) / dlt ;
        b2 = ((-2 * k * om)+(k * k)+(om * om)) / dlt;
}

void Filter::calCoeffAPF()
{
    float Q = 1;
    float BW = Fc/Q;
    float gam = (tan(pi*BW/sampleRate)-1)/(tan(pi*BW/sampleRate)+1);
    
    
    a0 = gam;
    a1 = 1;
    a2 = 0;
    b1 = gam;
    b2 = 0;
}

void Filter::setFc(float nFc)
{
    Fc = nFc;
    
    if (filterType== 1)
    {
        calCoeffHPF();
    }
    else if (filterType == 2)
    {
        calCoeffLPF();
    }
    else if (filterType == 3)
    {
        calCoeffAPF();
    }
}

float Filter::processSample(float xn)
{
    float yn = a0*xn + a1*xn1 + a2*xn2 - b1*yn1 - b2*yn2;

    xn2 = xn1;
    xn1 = xn;

    yn2 = yn1;
    yn1 = yn;

    return yn;
}



//-------------------------------------------------------------------------------------------
NonL::NonL():
k(1)
{}

float NonL::TANH(float xn)
{
    
    float yn = tanh(k*xn)/tanh(k);

    return yn;
}

float NonL::SIGMOID(float xn)
{
    
    float yn = (2/(1+exp(-k*xn)))-1;

    return yn;
}

void NonL::setSat(float k1)
{
    k = k1;
}

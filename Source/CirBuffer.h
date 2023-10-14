/*
  ==============================================================================

    CirBuffer.h
    Created: 12 Oct 2023 8:08:21pm
    Author:  Samuel Vergara Alvarez

  ==============================================================================
*/
#pragma once
#include "JuceHeader.h"
#include <cmath>

class CirBuffer
{
    CirBuffer();
public:
    void setSample(double value);
    double calculateDelta();
    double getdeltaRMS();
    double getRMS();
    double getSample();
    bool getupdateRMS();
    void setBuffSize(int value);
    
    
private:
    double rms(int start, int end);
    
    
    std::vector<double> buffer;
    int index;
    double deltaRMS;
    double RMSactual;
    double RMSanterior;
    int counter;
    int delayindex;
    bool updateRMS;
    int bufferSize;
    
};

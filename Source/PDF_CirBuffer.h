/*
  ==============================================================================

    PDF_CirBuffer.h
    Created: 12 Oct 2023 9:27:03pm
    Author:  Samuel Vergara Alvarez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include <cmath>

class CirPDF
{
    CirPDF();
    
public:
    void setPDF(float value);
    float calculatePDF();
    float getMedia();
    bool getFlag();
    void setbuffSizePDF(int size);
    
    
    
    
private:
        std::vector<double> buffer;
        int index;
        bool takeMediaOut;
        float media;
        int bufSize;
};

/*
  ==============================================================================

    PDF_CirBuffer.cpp
    Created: 12 Oct 2023 9:27:03pm
    Author:  Samuel Vergara Alvarez

  ==============================================================================
*/

#include "PDF_CirBuffer.h"
#include <numeric>
#include <cmath>

CirPDF::CirPDF():
index(0),
takeMediaOut(false),
media(0)
{}

void CirPDF::setPDF(float value)
{
    buffer[index] = value;
    index++;

    if (index >= bufSize) {
        media = calculatePDF();
        takeMediaOut = true;
        index = 0;
    }
}

float CirPDF::calculatePDF() {
    float sum = std::accumulate(buffer.begin(), buffer.end(), 0.0);
    return (sum / float(bufSize));
}

float CirPDF::getMedia() {
        return media;
    }

    bool CirPDF::getFlag() {
        bool flag = takeMediaOut;
        takeMediaOut = false;
        return flag;
    }
void CirPDF::setbuffSizePDF(int size)
{
    buffer.resize(size,0.0);
    bufSize = size;
    
}

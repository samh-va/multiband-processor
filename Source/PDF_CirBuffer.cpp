/*
  ==============================================================================

    PDF_CirBuffer.cpp
    Created: 12 Oct 2023 9:27:03pm
    Author:  Samuel Vergara Alvarez

  ==============================================================================
*/

#include "PDF_CirBuffer.h"
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
    double sum = std::accumulate(buffer.begin(), buffer.end(), 0.0);
    return sum / bufSize;
}

float CirPDF::getMedia() {
        return media;
    }

    bool CirPDF::getFlag() {
        bool flag = takeMediaOut;
        takeMediaOut = false;
        return flag;
    }

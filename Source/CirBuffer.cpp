/*
  ==============================================================================

    CirBuffer.cpp
    Created: 12 Oct 2023 8:08:21pm
    Author:  Samuel Vergara Alvarez

  ==============================================================================
*/

#include "CirBuffer.h"
#include <cmath>

CirBuffer::CirBuffer():
index(0),
deltaRMS(0),
RMSactual(0),
RMSanterior(0),
counter(0),
updateRMS(false),
bufferSize(512)
{}
//En el preparetoPlay inicializo el tamaño del buffer. Hago una función que me setee el tamaño del buffer
// y además me inicialice el delayindex
void CirBuffer::setSample(double value) {
        buffer[index] = value;
        counter++;

        if (counter == bufferSize) {
            deltaRMS = calculateDelta();
            updateRMS = true;
            counter = 0;
        }

        index++;
        if (index >= bufferSize*2)
        {
            index -= bufferSize*2;
        }
    }

double CirBuffer::calculateDelta() {
    int ini = (index - counter + 1);
        RMSactual = rms(ini, index);
        double delta = RMSactual - RMSanterior;
        RMSanterior = RMSactual;
        return delta;
    }

double CirBuffer::getdeltaRMS() {
        return deltaRMS;
    }
double CirBuffer::getRMS() {
    return RMSactual;
}

double CirBuffer::getSample() {
    double samp = buffer[delayindex];
    delayindex++;
    if (delayindex >= bufferSize*2) {
        delayindex -= bufferSize*2;
    }
    return samp;
}

bool CirBuffer::getupdateRMS() {
    bool value = updateRMS;
    updateRMS = false;
    return value;
}

void CirBuffer::setBuffSize(int size)
{
    buffer.resize(2 * size, 0.0);
    bufferSize = size;
    delayindex = index + size;
}

double CirBuffer::rms(int start, int end) {
    double sum = 0.0;
    for (int i = start; i <= end; ++i) {
        sum += buffer[i] * buffer[i];
    }
    return sqrt(sum / double (bufferSize));
}

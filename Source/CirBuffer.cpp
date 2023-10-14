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
delayindex(0),
updateRMS(false),
bufferSize(512)
{}
//En el preparetoPlay inicializo el tamaño del buffer. Hago una función que me setee el tamaño del buffer
// y además me inicialice el delayindex
void CirBuffer::setSample(double value) {
        buffer[index] = value;
        counter++;

        if (counter == bufferSize / 2) {
            deltaRMS = calculateDelta();
            updateRMS = true;
            counter = 0;
        }

        index++;
        if (index >= bufferSize) {
            index -= bufferSize;
        }
    }

double CirBuffer::calculateDelta() {
        int ini = (index - counter + 1 + bufferSize) % bufferSize;
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
    if (delayindex >= bufferSize) {
        delayindex -= bufferSize;
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
    delayindex = index + size;
}

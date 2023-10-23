/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

//#include "PluginEditor.h"
#include <JuceHeader.h>
#include "Compressor.h"
#include "PDF_CirBuffer.h"
#include "CirBuffer.h"
#include "Limiter.h"
#include "Filter/Filter.h"
//==============================================================================
/**
*/
class MultibandCompressorAudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
	, public juce::AudioProcessorARAExtension
#endif
{
public:
	//==============================================================================
	MultibandCompressorAudioProcessor();
	~MultibandCompressorAudioProcessor() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;
    
    int CBvalueL = 1;
    int CBvalueM = 1;
    int CBvalueH = 1;
    
    float value_low=0;
    float value_mid=0;
    float value_high=0;
    
    
//    float grL;
//    float grM;
//    float grH;
//
//    float inL;
//    float inM;
//    float inH;
//
//    float outLow;
//    float outMid;
//    float outHigh;
    

    float TaMaxLow;
    float TrMaxLow;
    float AutoMGLow=1;
    
    float TaMaxMid;
    float TrMaxMid;
    float AutoMGMid=1;
    
    float TaMaxHigh;
    float TrMaxHigh;
    float AutoMGHigh=1;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultibandCompressorAudioProcessor)

// Limitadores  ----------------------------------------------------------------------
    
    Limiter LimitL;
    Limiter LimitR;
    
// Compresores  ----------------------------------------------------------------------
    
    Compressor CompL_L;
    Compressor CompHL_L;
    Compressor CompH_L;
    
    Compressor CompL_R;
    Compressor CompHL_R;
    Compressor CompH_R;
    
// Buffer Circulares  ----------------------------------------------------------------
   
    CirBuffer buffCirL_L;
    CirBuffer buffCirHL_L;
    CirBuffer buffCirH_L;
    
    CirBuffer buffCirL_R;
    CirBuffer buffCirHL_R;
    CirBuffer buffCirH_R;
    
    
    
    CirPDF buffpdfL_L;
    CirPDF buffpdfHL_L;
    CirPDF buffpdfH_L;
    
    CirPDF buffpdfL_R;
    CirPDF buffpdfHL_R;
    CirPDF buffpdfH_R;

// Filtros --------------------------------------------------------------------------------
    Filter LPFR;
    Filter LPFL;
    Filter HPFR;
    Filter HPFL;
    Filter HPFmL;
    Filter LPFmL;
    Filter HPFmR;
    Filter LPFmR;
    Filter APFR;
    Filter APFL;
    
//    float firstPR;
//    float secondPR;
//    float thirdPR;
//    float firstPL;
//    float secondPL;
//    float thirdPL;
    
// OTROS -----------------------------------------------------------------------------------
        float Fs;
        float buffSize;
    
    float Rhl = 1.21496449059066;
    float Thl = -27;

    float Rll = 1.79346916291038;
    float Tll = -14;

    float Rml = 23.4508335553063;
    float Tml = -17;
    
// --------------------------------
    float Rhr = 2.42445105959837;
    float Thr = -33;

    float Rlr = 19.8281739707482;
    float Tlr = -15;

    float Rmr = 164.248089526385;
    float Tmr = -16;
    
};

/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once


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

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultibandCompressorAudioProcessor)

// Limitadores  ----------------------------------------------------------------------
    
    Limiter LimitL;
        //Limiter LimitR;
    
// Compresores  ----------------------------------------------------------------------
    
    Compressor CompL_L;
    Compressor CompHL_L;
    Compressor CompH_L;
        //Compressor CompL_R, CompHL_R, CompH_R;
    
// Buffer Circulares  ----------------------------------------------------------------------
   
    CirBuffer buffCirL_L;
    CirBuffer buffCirHL_L;
    CirBuffer buffCirH_L ;
        //CirBuffer buffCirL_R, buffCirHL_R, buffCirH_R ;
    
    CirPDF buffpdfL_L;
    CirPDF buffpdfHL_L;
    CirPDF buffpdfH_L;
        //CirPDF buffpdfL_R,buffpdfHL_R,buffpdfH_R;

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
    Filter APFhL;
    
    float firstPR;
    float secondPR;
    float thirdPR;
    float firstPL;
    float secondPL;
    float thirdPL;
    
// OTROS -----------------------------------------------------------------------------------
        float Fs;
        float buffSize;
    
    float Thl = -12; //-23;
    float Rhl = 0.0519852981190975;
    float Tml = -8;
    float Rml = 0.517709503697767;
    float Tll = -12;
    float Rll = 0.0416596326145323;
};

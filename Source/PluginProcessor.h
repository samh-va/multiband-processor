/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "Filter/Filter.h"
#include <JuceHeader.h>
#include "Compressor.h"
#include "PDF_CirBuffer.h"
#include "CirBuffer.h"
#include "Limiter.h"
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
    Limiter LimitL;
    Compressor CompL;
    CirBuffer buffCirL;
    CirPDF buffPDF_L;



    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource transportSource;

    float Fs;
    float buffSize;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultibandCompressorAudioProcessor);
	float FS;
	Filter LPFR, LPFL, HPFR, HPFL, HPFmL, LPFmL, HPFmR, LPFmR, APFR, APFL;
	float firstPR, secondPR, thirdPR, firstPL, secondPL, thirdPL = 0;
};

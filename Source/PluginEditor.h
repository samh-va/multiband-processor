/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "StyleSheet.h"

//==============================================================================
/**
*/
class MultibandCompressorAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	MultibandCompressorAudioProcessorEditor(MultibandCompressorAudioProcessor&);
	~MultibandCompressorAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	MultibandCompressorAudioProcessor& audioProcessor;
	double progressBarUI;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultibandCompressorAudioProcessorEditor)
	juce::StyleSheet customLNF;
	//LowBand Compressor Panel
	juce::CustomProgressBar LowCProgressBar{ progressBarUI };
	juce::ComboBox LowCCombo;
	juce::CustomDial LowCAUMSlider;
	juce::CustomDial LowCAMSlider;
	juce::CustomDial LowCRMSlider;

	//HighBand Compressor Panel
	juce::CustomProgressBar HighCProgressBar{ progressBarUI };
	juce::ComboBox HighCCombo;
	juce::CustomDial HighCAUMSlider;
	juce::CustomDial HighCAMSlider;
	juce::CustomDial HighCRMSlider;

	//Band Compressor Panel
	juce::CustomProgressBar BandCProgressBar{ progressBarUI };
	juce::ComboBox BandCCombo;
	juce::CustomDial BandCAUMSlider;
	juce::CustomDial BandCAMSlider;
	juce::CustomDial BandCRMSlider;
};

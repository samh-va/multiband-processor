/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "StyleSheet.h"
#include "VerticalMeter.h"

//==============================================================================
/**
*/
class MultibandCompressorAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
	MultibandCompressorAudioProcessorEditor(MultibandCompressorAudioProcessor &);

	~MultibandCompressorAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics &) override;

	void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	MultibandCompressorAudioProcessor &audioProcessor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultibandCompressorAudioProcessorEditor)
	juce::StyleSheet customLNF;
	juce::Array<juce::String> dropDownOptions = {"In", "GN", "Out"};
	int comboWidth = 215;
	int comboHeight = 35;
	int dialWidth = 153;
	int dialHeight = 125;
    int meterWidth = 40;
    int meterHeight = 402;

	//LowBand Compressor Panel
	juce::Label LbPgBarLabel;
	juce::Label LbCTitle;
	juce::Label LbAUMSliderLabel;
	juce::Label LbAMSSliderLabel;
	juce::Label LbRMSSliderLabel;
	std::vector<juce::Label *> LCLabels = {&LbAUMSliderLabel, &LbAMSSliderLabel, &LbRMSSliderLabel, &LbCTitle};
	UI::VerticalGradientMeter LowCVerticalMeter;
	juce::CustomCombo LowCCombo;
	juce::CustomDial LowCAUMSlider;
	juce::CustomDial LowCAMSlider;
	juce::CustomDial LowCRMSlider;

	//HighBand Compressor Panel
	juce::Label HbCTitle;
	UI::VerticalGradientMeter HighCVerticalMeter;
	juce::CustomCombo HighCCombo;
	juce::CustomDial HighCAUMSlider;
	juce::CustomDial HighCAMSlider;
	juce::CustomDial HighCRMSlider;
	juce::Label HbPgBarLabel;
	juce::Label HbAUMSliderLabel;
	juce::Label HbAMSSliderLabel;
	juce::Label HbRMSSliderLabel;
	std::vector<juce::Label *> HCLabels = {&HbAUMSliderLabel, &HbAMSSliderLabel, &HbRMSSliderLabel, &HbCTitle};

	//Band Compressor Panel
	juce::Label BCTitle;
	juce::Label BCPgBarLabel;
	juce::Label BCAUMSliderLabel;
	juce::Label BCAMSSliderLabel;
	juce::Label BCRMSSliderLabel;
	std::vector<juce::Label *> BCLabels = {&BCAMSSliderLabel, &BCRMSSliderLabel, &BCAUMSliderLabel, &BCTitle};
	UI::VerticalGradientMeter BandCVerticalMeter;
	juce::CustomCombo BandCCombo;
	juce::CustomDial BandCAUMSlider;
	juce::CustomDial BandCAMSlider;
	juce::CustomDial BandCRMSlider;
    
    UI::VerticalGradientMeter BandCVerticalMeterR;
    UI::VerticalGradientMeter HighCVerticalMeterR;
    UI::VerticalGradientMeter LowCVerticalMeterR;
};

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultibandCompressorAudioProcessorEditor::MultibandCompressorAudioProcessorEditor(MultibandCompressorAudioProcessor &p)
		: AudioProcessorEditor(&p), audioProcessor(p) {
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(1080, 520);
	juce::LookAndFeel::setDefaultLookAndFeel(&customLNF);
	progressBarUI = 1.0;
	//LowBand Compressor Panel
	addAndMakeVisible(LowCProgressBar);
	addAndMakeVisible(LowCCombo);
	addAndMakeVisible(LowCAMSlider);
	addAndMakeVisible(LowCAUMSlider);
	addAndMakeVisible(LowCRMSlider);

	//HighBand Compressor Panel
	addAndMakeVisible(HighCProgressBar);
	addAndMakeVisible(HighCCombo);
	addAndMakeVisible(HighCAMSlider);
	addAndMakeVisible(HighCAUMSlider);
	addAndMakeVisible(HighCRMSlider);

	//Band Compressor Panel
	addAndMakeVisible(BandCProgressBar);
	addAndMakeVisible(BandCCombo);
	addAndMakeVisible(BandCAMSlider);
	addAndMakeVisible(BandCAUMSlider);
	addAndMakeVisible(BandCRMSlider);
}

MultibandCompressorAudioProcessorEditor::~MultibandCompressorAudioProcessorEditor() {
	juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

//==============================================================================
void MultibandCompressorAudioProcessorEditor::paint(juce::Graphics &g) {
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(juce::Colour::fromRGB(64, 61, 57));
	g.setColour(juce::Colour::fromRGB(37,36,34));
	g.drawRoundedRectangle(8, 14, 345, 494, 20, 1);
	g.drawRoundedRectangle(370, 13, 345, 494, 20, 1);
	g.drawRoundedRectangle(730, 13, 345, 494, 20, 1);
	g.setColour(juce::Colour::fromRGB(64,61,57));
	g.fillRect(64,161,268,327);

	g.setColour(juce::Colours::white);
	g.setFont(15.0f);
}

void MultibandCompressorAudioProcessorEditor::resized() {
	//LowBand Compressor Panel
	LowCProgressBar.setBounds(20, 66, 48, 402);
	LowCCombo.setBounds(64, 66, 222, 55);
	LowCAMSlider.setBounds(179, 310, 153, 125);
	LowCAUMSlider.setBounds(121, 148, 153, 125);
	LowCRMSlider.setBounds(59, 310, 153, 125);

	//HighBand Compressor Panel
	HighCProgressBar.setBounds(381, 66, 48, 402);
	HighCCombo.setBounds(438, 66, 222, 55);
	HighCAMSlider.setBounds(553, 310, 153, 125);
	HighCAUMSlider.setBounds(495, 148, 153, 125);
	HighCRMSlider.setBounds(433, 310, 153, 125);

	//Band Compressor Panel
	BandCProgressBar.setBounds(740, 66, 48, 402);
	BandCCombo.setBounds(797, 66, 222, 55);
	BandCAMSlider.setBounds(912, 310, 153, 125);
	BandCAUMSlider.setBounds(854, 148, 153, 125);
	BandCRMSlider.setBounds(792, 310, 153, 125);
}

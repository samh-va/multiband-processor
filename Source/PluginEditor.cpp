#include "PluginProcessor.h"
#include "PluginEditor.h"

auto levels = 0.2f;

//==============================================================================
MultibandCompressorAudioProcessorEditor::MultibandCompressorAudioProcessorEditor(MultibandCompressorAudioProcessor &p)
		: AudioProcessorEditor(&p), audioProcessor(p), LowCVerticalMeter([&]() { return levels; }),
		  HighCVerticalMeter([&]() { return levels; }), BandCVerticalMeter([&]() { return levels; }) {

	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(1080, 520);
	juce::LookAndFeel::setDefaultLookAndFeel(&customLNF);

	//LowBand Compressor Panel
	addAndMakeVisible(LbCTitle);

	addAndMakeVisible(HbCTitle);
	addAndMakeVisible(BCTitle);
	addAndMakeVisible(LbPgBarLabel);
	addAndMakeVisible(LbRMSSliderLabel);
	addAndMakeVisible(LbAMSSliderLabel);
	addAndMakeVisible(LbAUMSliderLabel);
	addAndMakeVisible(LowCVerticalMeter);
	addAndMakeVisible(LowCVerticalMeter);
	addAndMakeVisible(LowCCombo);
	addAndMakeVisible(LowCAMSlider);
	addAndMakeVisible(LowCAUMSlider);
	addAndMakeVisible(LowCRMSlider);

	//Dropdowns
	LowCCombo.addItemList(dropDownOptions, 1);
	HighCCombo.addItemList(dropDownOptions, 1);
	BandCCombo.addItemList(dropDownOptions, 1);
	LowCCombo.setTextWhenNothingSelected("SELECT A SOURCE");
	HighCCombo.setTextWhenNothingSelected("SELECT A SOURCE");
	BandCCombo.setTextWhenNothingSelected("SELECT A SOURCE");

	//Labels
	LbCTitle.setText("LowBand Compressor", juce::dontSendNotification);
	LbAUMSliderLabel.setText("Auto MakeUp", juce::dontSendNotification);
	LbRMSSliderLabel.setText("Release Max", juce::dontSendNotification);
	LbAMSSliderLabel.setText("Attack Max", juce::dontSendNotification);
	LbPgBarLabel.attachToComponent(&LowCVerticalMeter, false);
	LbAUMSliderLabel.attachToComponent(&LowCAUMSlider, false);
	LbRMSSliderLabel.attachToComponent(&LowCRMSlider, false);
	LbAMSSliderLabel.attachToComponent(&LowCAMSlider, false);
	for (int i = 0; i < LCLabels.size(); ++i) {
		LCLabels[i]->setJustificationType(juce::Justification::centred);
	}
	//----------------------------------------------------------------------------------------------------------------------//

	//HighBand Compressor Panel
	addAndMakeVisible(HighCVerticalMeter);
	addAndMakeVisible(HighCCombo);
	addAndMakeVisible(HighCAMSlider);
	addAndMakeVisible(HighCAUMSlider);
	addAndMakeVisible(HighCRMSlider);
	//Labels
	HbCTitle.setText("HighBand Compressor", juce::dontSendNotification);
	HbAUMSliderLabel.setText("Auto MakeUp", juce::dontSendNotification);
	HbRMSSliderLabel.setText("Release Max", juce::dontSendNotification);
	HbAMSSliderLabel.setText("Attack Max", juce::dontSendNotification);
	HbPgBarLabel.attachToComponent(&HighCVerticalMeter, false);
	HbAUMSliderLabel.attachToComponent(&HighCAUMSlider, false);
	HbRMSSliderLabel.attachToComponent(&HighCRMSlider, false);
	HbAMSSliderLabel.attachToComponent(&HighCAMSlider, false);
	for (int i = 0; i < LCLabels.size(); ++i) {
		HCLabels[i]->setJustificationType(juce::Justification::centred);
	}
//----------------------------------------------------------------------------------------------//

	//Band Compressor Panel
	addAndMakeVisible(BandCVerticalMeter);
	addAndMakeVisible(BandCCombo);
	addAndMakeVisible(BandCAMSlider);
	addAndMakeVisible(BandCAUMSlider);
	addAndMakeVisible(BandCRMSlider);

	//Labels
	BCTitle.setText("Band Compressor", juce::dontSendNotification);
	BCAUMSliderLabel.setText("Auto MakeUp", juce::dontSendNotification);
	BCRMSSliderLabel.setText("Release Max", juce::dontSendNotification);
	BCAMSSliderLabel.setText("Attack Max", juce::dontSendNotification);
	BCPgBarLabel.attachToComponent(&BandCVerticalMeter, false);
	BCAUMSliderLabel.attachToComponent(&BandCAUMSlider, false);
	BCRMSSliderLabel.attachToComponent(&BandCRMSlider, false);
	BCAMSSliderLabel.attachToComponent(&BandCAMSlider, false);
	for (int i = 0; i < LCLabels.size(); ++i) {
		BCLabels[i]->setJustificationType(juce::Justification::centred);
	}
}

MultibandCompressorAudioProcessorEditor::~MultibandCompressorAudioProcessorEditor() {
	juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

//==============================================================================
void MultibandCompressorAudioProcessorEditor::paint(juce::Graphics &g) {
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(juce::Colour::fromRGB(64, 61, 57));

	//LowBand Compressor Panel
	g.setColour(juce::Colour::fromRGB(37, 36, 34));
	g.drawRoundedRectangle(5, 14, 345, 494, 20, 1);
	g.drawRoundedRectangle(367, 13, 345, 494, 20, 1);
	g.drawRoundedRectangle(730, 13, 345, 494, 20, 1);
	g.setColour(juce::Colour::fromRGB(64, 61, 57));
	g.fillRoundedRectangle(64, 161, 268, 327, 20);

	g.setColour(juce::Colours::white);
	g.setFont(15.0f);
}

void MultibandCompressorAudioProcessorEditor::resized() {
	//LowBand Compressor Panel
	LbCTitle.setBounds(15, 14, 323, 58);
	LbPgBarLabel.setBounds(20, 407, 20, 20);
	LowCVerticalMeter.setBounds(20, 66, meterWidth,meterHeight);
	LowCCombo.setBounds(115, 66, comboWidth,comboHeight);
	LowCAUMSlider.setBounds(152, 160, dialWidth,dialHeight);
	LowCAMSlider.setBounds(210, 343, dialWidth,dialHeight);
	LowCRMSlider.setBounds(90, 343, dialWidth,dialHeight);

	//HighBand Compressor Panel
	HbCTitle.setBounds(374, 14, 323, 58);
	HighCVerticalMeter.setBounds(380, 66, meterWidth,meterHeight);
	HighCCombo.setBounds(475, 66, comboWidth,comboHeight);
	HighCAUMSlider.setBounds(512, 160, dialWidth,dialHeight);
	HighCAMSlider.setBounds(570, 343, dialWidth,dialHeight);
	HighCRMSlider.setBounds(450, 343, dialWidth,dialHeight);

	//Band Compressor Panel
	BCTitle.setBounds(733, 14, 323, 58);
	BandCVerticalMeter.setBounds(750, 66, meterWidth,meterHeight);
	BandCCombo.setBounds(845, 66, comboWidth,comboHeight);
	BandCAUMSlider.setBounds(882, 160, dialWidth,dialHeight);
	BandCAMSlider.setBounds(940, 343, dialWidth,dialHeight);
	BandCRMSlider.setBounds(820, 343, dialWidth,dialHeight);
}

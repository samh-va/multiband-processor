#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
MultibandCompressorAudioProcessorEditor::MultibandCompressorAudioProcessorEditor(MultibandCompressorAudioProcessor &p)
		: AudioProcessorEditor(&p), audioProcessor(p), LowCVerticalMeter([&]() { return (audioProcessor.value_low) ; }),
		  HighCVerticalMeter([&]() { return audioProcessor.value_high; }), BandCVerticalMeter([&]() { return audioProcessor.value_mid; }),
BandCVerticalMeterR([&]() { return audioProcessor.value_midR; }), HighCVerticalMeterR([&]() { return audioProcessor.value_highR; }),LowCVerticalMeterR([&]() { return audioProcessor.value_lowR ;}) {

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
    addAndMakeVisible(LowCVerticalMeterR);

              
	addAndMakeVisible(LowCCombo);
    
              
	addAndMakeVisible(LowCAMSlider);
              LowCAMSlider.setRange(50e-3, 200e-3);
              LowCAMSlider.setValue(100e-3);
              LowCAMSlider.onValueChange=[this]

    {
        audioProcessor.TaMaxLow=LowCAMSlider.getValue();
    };
              
	addAndMakeVisible(LowCAUMSlider);
              LowCAUMSlider.setRange(0, 1);
              LowCAUMSlider.setValue(1);
              LowCAUMSlider.onValueChange=[this]
    {
        audioProcessor.AutoMGLow=LowCAUMSlider.getValue();
    };
	addAndMakeVisible(LowCRMSlider);
              LowCRMSlider.setRange(100e-3, 300e-3);
              LowCRMSlider.setValue(200e-3);
              LowCRMSlider.onValueChange=[this]
    {
        audioProcessor.TrMaxLow=LowCRMSlider.getValue();
    };

	//Dropdowns
              LowCCombo.setTextWhenNothingSelected("SELECT A SOURCE");
              HighCCombo.setTextWhenNothingSelected("SELECT A SOURCE");
              BandCCombo.setTextWhenNothingSelected("SELECT A SOURCE");
              
              LowCCombo.addItem("IN", 1);
              LowCCombo.addItem("GR", 2);
              LowCCombo.addItem("OUT", 3);
              
              LowCCombo.onChange=[this]
              {
                  if (LowCCombo.getSelectedId() == 1)
                  {
                      LowCCombo.setSelectedId(1);
                      audioProcessor.CBvalueL = 1;
                  }
                  else if (LowCCombo.getSelectedId() == 2)
                  {
                      LowCCombo.setSelectedId(2);
                      audioProcessor.CBvalueL = 2;
                  }
                  else if(LowCCombo.getSelectedId() == 3)
                  {
                      LowCCombo.setSelectedId(3);
                      audioProcessor.CBvalueL = 3;
                  }
              };
              
              BandCCombo.addItem("IN", 1);
              BandCCombo.addItem("GR", 2);
              BandCCombo.addItem("OUT", 3);
              
              BandCCombo.onChange=[this]
              {
                  if (BandCCombo.getSelectedId() == 1)
                  {
                      BandCCombo.setSelectedId(1);
                      audioProcessor.CBvalueM = 1;
                  }
                  else if (BandCCombo.getSelectedId() == 2)
                  {
                      BandCCombo.setSelectedId(2);
                      audioProcessor.CBvalueM = 2;
                  }
                  else if(BandCCombo.getSelectedId() == 3)
                  {
                      BandCCombo.setSelectedId(3);
                      audioProcessor.CBvalueM = 3;
                  }
              };
              
              
              HighCCombo.addItem("IN", 1);
              HighCCombo.addItem("GR", 2);
              HighCCombo.addItem("OUT", 3);
              
              HighCCombo.onChange=[this]
              {
                  if (HighCCombo.getSelectedId() == 1)
                  {
                      HighCCombo.setSelectedId(1);
                      audioProcessor.CBvalueH = 1;
                  }
                  else if (HighCCombo.getSelectedId() == 2)
                  {
                      HighCCombo.setSelectedId(2);
                      audioProcessor.CBvalueH = 2;
                  }
                  else if(HighCCombo.getSelectedId() == 3)
                  {
                      HighCCombo.setSelectedId(3);
                      audioProcessor.CBvalueH = 3;
                  }
              };
              


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
    addAndMakeVisible(HighCVerticalMeterR);

	addAndMakeVisible(HighCCombo);
              
	addAndMakeVisible(HighCAMSlider);
              HighCAMSlider.setRange(50e-3, 200e-3);
              HighCAMSlider.setValue(100e-3);
              HighCAMSlider.onValueChange=[this]
    {
        audioProcessor.TaMaxHigh=HighCAMSlider.getValue();
    };
	addAndMakeVisible(HighCAUMSlider);
              HighCAUMSlider.setRange(0, 1);
              HighCAUMSlider.setValue(1);
              HighCAUMSlider.onValueChange=[this]
    {
        audioProcessor.AutoMGHigh=HighCAUMSlider.getValue();
    };
              
	addAndMakeVisible(HighCRMSlider);
              HighCRMSlider.setRange(100e-3, 300e-3);
              HighCRMSlider.setValue(200e-3);
              HighCRMSlider.onValueChange=[this]
    {
        audioProcessor.TrMaxHigh=HighCRMSlider.getValue();
    };
                        
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
    addAndMakeVisible(BandCVerticalMeterR);

	addAndMakeVisible(BandCCombo);
              addAndMakeVisible(BandCAMSlider);
                        BandCAMSlider.setRange(50e-3, 200e-3);
                        BandCAMSlider.setValue(100e-3);
                        BandCAMSlider.onValueChange=[this]
              {
                  audioProcessor.TaMaxMid=BandCAMSlider.getValue();
              };
              addAndMakeVisible(BandCAUMSlider);
                        BandCAUMSlider.setRange(0, 1);
                        BandCAUMSlider.setValue(1);
                        BandCAUMSlider.onValueChange=[this]
              {
                  audioProcessor.AutoMGMid=BandCAUMSlider.getValue();
              };
                        
              addAndMakeVisible(BandCRMSlider);
                        BandCRMSlider.setRange(100e-3, 300e-3);
                        BandCRMSlider.setValue(200e-3);
                        BandCRMSlider.onValueChange=[this]
              {
                  audioProcessor.TrMaxMid=BandCRMSlider.getValue();
              };

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
    LowCVerticalMeterR.setBounds(60, 66, meterWidth,meterHeight);
    LowCCombo.setBounds(115, 66, comboWidth,comboHeight);
	LowCAUMSlider.setBounds(152, 160, dialWidth,dialHeight);
	LowCAMSlider.setBounds(210, 343, dialWidth,dialHeight);
	LowCRMSlider.setBounds(90, 343, dialWidth,dialHeight);

	//HighBand Compressor Panel
	HbCTitle.setBounds(733, 14, 323, 58);
    HighCVerticalMeter.setBounds(750, 66, meterWidth,meterHeight);
    HighCVerticalMeterR.setBounds(790, 66, meterWidth,meterHeight);
	HighCCombo.setBounds(845, 66, comboWidth,comboHeight);
	HighCAUMSlider.setBounds(882, 160, dialWidth,dialHeight);
	HighCAMSlider.setBounds(940, 343, dialWidth,dialHeight);
	HighCRMSlider.setBounds(820, 343, dialWidth,dialHeight);

	//Band Compressor Panel
	BCTitle.setBounds(374, 14, 323, 58);
    BandCVerticalMeter.setBounds(380, 66, meterWidth,meterHeight);
    BandCVerticalMeterR.setBounds(420, 66, meterWidth,meterHeight);
	BandCCombo.setBounds(475, 66, comboWidth,comboHeight);
	BandCAUMSlider.setBounds(512, 160, dialWidth,dialHeight);
	BandCAMSlider.setBounds(570, 343, dialWidth,dialHeight);
	BandCRMSlider.setBounds(450, 343, dialWidth,dialHeight);
    
    
//    HbCTitle.setBounds(374, 14, 323, 58);
//    HighCVerticalMeter.setBounds(750, 66, meterWidth,meterHeight);
//    HighCVerticalMeterR.setBounds(790, 66, meterWidth,meterHeight);
//    HighCCombo.setBounds(475, 66, comboWidth,comboHeight);
//    HighCAUMSlider.setBounds(512, 160, dialWidth,dialHeight);
//    HighCAMSlider.setBounds(570, 343, dialWidth,dialHeight);
//    HighCRMSlider.setBounds(450, 343, dialWidth,dialHeight);
    
}

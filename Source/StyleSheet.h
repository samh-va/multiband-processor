#pragma once

#include <JuceHeader.h>

namespace juce {


	class StyleSheet : public juce::LookAndFeel_V4 {
	public:
		StyleSheet();

		void drawRotarySlider(juce::Graphics &, int x, int y, int width, int height, float sliderPosProportional,
							  float rotaryStartAngle, float rotaryEndAngle, juce::Slider &) override;
	private:
		void drawLinearProgressBar (Graphics& g, const ProgressBar& progressBar,
														 int width, int height, double progress,
														 const String& textToShow);
	};

	class CustomDial : public Slider {
	public:
		CustomDial();
	};

	class CustomCombo:public ComboBox{
	public:
		CustomCombo();
	};
}

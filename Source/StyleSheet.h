#pragma once

#include <JuceHeader.h>

namespace juce {

	class StyleSheet : public juce::LookAndFeel_V4 {
	public:


	};

	class CustomDial : public Slider {
	public:
		CustomDial() {
			setColour(juce::Slider::rotarySliderFillColourId, Colour::fromRGB(235, 94, 40));
			setColour(juce::Slider::rotarySliderOutlineColourId, Colour::fromRGB(255, 252, 242));
			setColour(juce::Slider::thumbColourId, Colour::fromRGB(37, 36, 34));
			setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
			setTextBoxIsEditable(false);
		}
	};

	class CustomProgressBar : public ProgressBar {
	public:
		CustomProgressBar(double &progress) : ProgressBar(progress) {
			setColour(juce::ProgressBar::backgroundColourId, Colour::fromRGB(255, 252, 242));
			setColour(juce::ProgressBar::foregroundColourId, Colour::fromRGB(235, 94, 40));
			setStyle(Style::linear);
			setSize(48,402);
		}

	};
}

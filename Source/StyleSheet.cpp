#include <JuceHeader.h>
#include "StyleSheet.h"

namespace juce {
	StyleSheet::StyleSheet() {}

	void StyleSheet::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
									  float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
									  juce::Slider &slider) {
		auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
		auto fill = slider.findColour(Slider::rotarySliderFillColourId);

		auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);

		auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
		auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		auto lineW = jmin(8.0f, radius * 0.5f);
		auto arcRadius = radius - lineW * 0.5f;

		Path backgroundArc;
		backgroundArc.addCentredArc(bounds.getCentreX(),
									bounds.getCentreY(),
									arcRadius,
									arcRadius,
									0.0f,
									rotaryStartAngle,
									rotaryEndAngle,
									true);

		g.setColour(outline);
		g.strokePath(backgroundArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));

		if (slider.isEnabled()) {
			Path valueArc;
			valueArc.addCentredArc(bounds.getCentreX(),
								   bounds.getCentreY(),
								   arcRadius,
								   arcRadius,
								   0.0f,
								   rotaryStartAngle,
								   toAngle,
								   true);

			g.setColour(fill);
			g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));
		}

		auto thumbWidth = lineW * 2.0f;
		Point<float> thumbPoint(bounds.getCentreX() + arcRadius * std::cos(toAngle - MathConstants<float>::halfPi),
								bounds.getCentreY() + arcRadius * std::sin(toAngle - MathConstants<float>::halfPi));

		g.setColour(slider.findColour(juce::Slider::thumbColourId));
		g.fillEllipse(Rectangle<float>(thumbWidth / 2, thumbWidth / 2).withCentre(thumbPoint));
		g.drawLine(backgroundArc.getBounds().getCentreX(), backgroundArc.getBounds().getCentreY(), thumbPoint.getX(),
				   thumbPoint.getY(), lineW);
	}


	CustomDial::CustomDial() {
		setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
		setColour(juce::Slider::rotarySliderFillColourId, Colour::fromRGB(235, 94, 40));
		setColour(juce::Slider::rotarySliderOutlineColourId, Colour::fromRGB(255, 252, 242));
		setColour(juce::Slider::thumbColourId, Colour::fromRGB(37, 36, 34));
		setNumDecimalPlacesToDisplay(1);
		setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
	}

	CustomCombo::CustomCombo() {
		setColour(juce::ComboBox::backgroundColourId,Colour::fromRGB(255, 252, 242));
		setColour(juce::ComboBox::buttonColourId,Colour::fromRGB(255, 252, 242));
		setColour(juce::ComboBox::textColourId,Colours::black);
		setColour(juce::ComboBox::arrowColourId,Colour::fromRGB(37, 36, 34));
	}
}
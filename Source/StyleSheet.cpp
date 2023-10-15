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

	void StyleSheet::drawLinearProgressBar(Graphics& g, const ProgressBar& progressBar, int width, int height,
										   double progress, const String& textToShow) {
		auto background = progressBar.findColour(ProgressBar::backgroundColourId);
		auto foreground = progressBar.findColour(ProgressBar::foregroundColourId);

		auto barBounds = progressBar.getLocalBounds().toFloat();

		g.setColour(background);
		g.fillRoundedRectangle(barBounds, 0);

		if (progress >= 0.0f && progress <= 1.0f) {
			Path p;
			p.addRoundedRectangle(barBounds, 0);
			g.reduceClipRegion(p);

			barBounds.setWidth(barBounds.getWidth() * (float) progress);
			g.setColour(foreground);
			g.fillRoundedRectangle(barBounds, 0);
		} else {
			// spinning bar..
			g.setColour(background);

			auto stripeWidth = height * 2;
			auto position = static_cast<int> (Time::getMillisecondCounter() / 15) % stripeWidth;

			Path p;

			for (auto x = static_cast<float> (-position);
				 x < (float) (width + stripeWidth); x += (float) stripeWidth)
				p.addQuadrilateral(x, 0.0f,
								   x + (float) stripeWidth * 0.5f, 0.0f,
								   x, static_cast<float> (height),
								   x - (float) stripeWidth * 0.5f, static_cast<float> (height));

			Image im(Image::ARGB, width, height, true);

			{
				Graphics g2(im);
				g2.setColour(foreground);
				g2.fillRoundedRectangle(barBounds, (float) progressBar.getHeight() * 0.5f);
			}

			g.setTiledImageFill(im, 0, 0, 0.85f);
			g.fillPath(p);
		}

		if (textToShow.isNotEmpty()) {
			g.setColour(Colour::contrasting(background, foreground));
			g.setFont((float) height * 0.6f);

			g.drawText(textToShow, 0, 0, width, height, Justification::centred, false);
		}
	}


	CustomDial::CustomDial() {
		setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
		setColour(juce::Slider::rotarySliderFillColourId, Colour::fromRGB(235, 94, 40));
		setColour(juce::Slider::rotarySliderOutlineColourId, Colour::fromRGB(255, 252, 242));
		setColour(juce::Slider::thumbColourId, Colour::fromRGB(37, 36, 34));
		setNumDecimalPlacesToDisplay(1);
		setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
	}
}
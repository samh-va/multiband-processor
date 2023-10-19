#pragma once

#include <JuceHeader.h>

namespace UI
{
	class VerticalGradientMeter : public juce::Component, public juce::Timer
	{
	public:
		explicit VerticalGradientMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction))
		{
			startTimerHz(24);
//			grill = juce::ImageCache::getFromMemory(BinaryData::MeterGrill_png, BinaryData::MeterGrill_pngSize);
		}

		void paint(juce::Graphics& g) override
		{
			auto bounds = getLocalBounds().toFloat().reduced(3.f);

			g.setColour(juce::Colours::black);
			g.fillRoundedRectangle(bounds,5);

			g.setGradientFill(gradient);
			const auto scaledY = juce::jmap(valueSupplier(), -120.f, 0.f, 0.f, static_cast<float>(getHeight()));
			g.fillRoundedRectangle(bounds.removeFromBottom(scaledY),5);
		}

		void resized() override
		{
			const auto bounds = getLocalBounds().toFloat();
			gradient = juce::ColourGradient{ juce::Colours::green, bounds.getBottomLeft(), juce::Colours::red, bounds.getTopLeft(), false };
			gradient.addColour(0.5, juce::Colours::yellow);
		}

		void paintOverChildren(::juce::Graphics& g) override
		{
//			g.drawImage(grill, getLocalBounds().toFloat());
		}

		void timerCallback() override
		{
			repaint();
		}
	private:
		std::function<float()> valueSupplier;
		juce::ColourGradient gradient{};
		juce::Image grill;
	};
}
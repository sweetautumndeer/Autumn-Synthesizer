/*
  ==============================================================================

    FilterComponent.cpp
    Created: 2 Mar 2022 2:15:43pm
    Author:  Autumn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterID)
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    cutoffAttachment = std::make_unique<SliderAttachment>(apvts, filterID + "CUTOFF", cutoffSlider);
    resonanceAttachment = std::make_unique<SliderAttachment>(apvts, filterID + "RESONANCE", resonanceSlider);

    setSliderParams(cutoffSlider, cutoffLabel, "Cutoff");
    setSliderParams(resonanceSlider, resonanceLabel, "Resonance");

    // Add Filter Selector
    juce::StringArray choices = { "Low Pass", "Band Pass", "High Pass" };
    filterType.addItemList(choices, 1);
    addAndMakeVisible(filterType);
    filterTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterID + "TYPE", filterType);

}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::setSliderParams(juce::Slider& slider, juce::Label& label, juce::String name) 
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 160, slider.getTextBoxHeight());
    slider.setTextValueSuffix(" sec");
    addAndMakeVisible(slider);

    label.setText(name, juce::dontSendNotification);
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);
}

void FilterComponent::paint (juce::Graphics& g) 
{
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void FilterComponent::resized()
{
    const auto bounds = getLocalBounds();
    const auto paddingX = bounds.getWidth() / 20;
    const auto paddingY = bounds.getHeight() / 20;
    const auto textPadding = bounds.getHeight() / 10;
    const auto comboboxWidth = bounds.getWidth() / 2 - paddingX * 2;
    const auto comboboxHeight = bounds.getHeight() / 3 - paddingY * 2;
    const auto sliderWidth = bounds.getWidth() - paddingX * 2;
    const auto sliderHeight = bounds.getHeight() / 3.5 - paddingY * 2;


    filterType.setBounds(paddingX, paddingY, comboboxWidth, comboboxHeight);
    cutoffSlider.setBounds(paddingX, filterType.getBottom() + paddingY + textPadding, sliderWidth, sliderHeight);
    resonanceSlider.setBounds(paddingX, cutoffSlider.getBottom() + paddingY + textPadding, sliderWidth, sliderHeight);
}

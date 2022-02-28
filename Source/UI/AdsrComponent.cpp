/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 27 Feb 2022 10:33:55pm
    Author:  Autumn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(juce::AudioProcessorValueTreeState& apvts) {
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    attackAttachment = std::make_unique<SliderAttachment>(apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, "RELEASE", releaseSlider);

    setSliderParams(attackSlider, attackLabel, "Attack");
    setSliderParams(decaySlider, decayLabel, "Decay");
    setSliderParams(sustainSlider, sustainLabel, "Sustain");
    setSliderParams(releaseSlider, releaseLabel, "Release");
}

AdsrComponent::~AdsrComponent() {
}

void AdsrComponent::setSliderParams(juce::Slider& slider, juce::Label& label, juce::String name) {
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 160, slider.getTextBoxHeight());
    slider.setTextValueSuffix(" sec");
    addAndMakeVisible(slider);

    label.setText(name, juce::dontSendNotification);
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);
}

void AdsrComponent::paint (juce::Graphics& g) {
    
}

void AdsrComponent::resized() {
    const auto bounds = getLocalBounds().reduced(10); // get plugin bounds with margin
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getWidth() / 4 - padding;
    const auto sliderStartX = padding;
    const auto sliderStartY = bounds.getHeight() / 2 - (sliderHeight);

    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
}

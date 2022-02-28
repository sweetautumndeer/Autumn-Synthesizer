/*
  ==============================================================================

    OscComponent.cpp
    Created: 28 Feb 2022 12:06:53am
    Author:  Autumn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorID)
{
    juce::StringArray choices = { "Sine", "Triangle", "Saw", "Square" };
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);

    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorID, oscWaveSelector);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g) {

}

void OscComponent::resized()
{
    oscWaveSelector.setBounds(0, 0, 90, 20);
}

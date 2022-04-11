/*
  ==============================================================================

    OscComponent.h
    Created: 28 Feb 2022 12:06:53am
    Author:  Autumn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
/*
*/
class OscComponent  : public juce::Component
{
public:
    OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorID);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox oscWaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;
    FilterComponent oscFilter;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"
#include "UI/OscComponent.h"
#include "UI/FilterComponent.h"

//==============================================================================
/**
*/
class Music167AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Music167AudioProcessorEditor (Music167AudioProcessor&);
    ~Music167AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    Music167AudioProcessor& audioProcessor;

    // GUI Elements
    AdsrComponent adsr;
    OscComponent osc1;
    OscComponent osc2;
    OscComponent osc3;
    FilterComponent filter;

    // GUI Dimensions
    int size[2] = { 800, 600 };
    int oscSize[2] = { 300, 200 };
    int adsrSize[2] = { 300, 200 };
    int filterSize[2] = { 300, 200 };
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Music167AudioProcessorEditor)
};

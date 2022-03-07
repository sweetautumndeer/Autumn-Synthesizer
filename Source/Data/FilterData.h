/*
  ==============================================================================

    FilterData.h
    Created: 2 Mar 2022 1:40:15pm
    Author:  Autumn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterData {
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(const int filterType, const float cutoffFreq, const float resonance); 
    void reset();

private:
    juce::dsp::StateVariableTPTFilter<float> filter;

    bool isPrepared{ false };
};

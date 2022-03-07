/*
  ==============================================================================

    FilterData.cpp
    Created: 2 Mar 2022 1:40:15pm
    Author:  Autumn

  ==============================================================================
*/

#include "FilterData.h"

void FilterData::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels) {
    filter.reset();

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;

    filter.prepare(spec);

    isPrepared = true;
}

void FilterData::process(juce::AudioBuffer<float>& buffer) {
    jassert(isPrepared);

    juce::dsp::AudioBlock<float> block { buffer };
    filter.process(juce::dsp::ProcessContextReplacing<float>(block));
}

void FilterData::updateParameters(const int filterType, const float cutoffFreq, const float resonance) {
    // Set Filter Type
    switch (filterType) {
    case 0:
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        break;
    case 1:
        filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        break;
    case 2:
        filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        break;
    default:
        jassertfalse;
        break;
    }
    filter.setCutoffFrequency(cutoffFreq);
    filter.setResonance(resonance);
}

void FilterData::reset() {
    filter.reset();
}
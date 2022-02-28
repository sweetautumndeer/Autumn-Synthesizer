/*
  ==============================================================================

    OscData.cpp
    Created: 27 Feb 2022 11:37:51pm
    Author:  Autumn

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec) {
    prepare(spec);
}

void OscData::setWaveType(const int choice) {
    switch (choice) {
    case 0:
        // sine
        initialise([](float x) { return std::sin(x); }, 200);
        break;
    case 1:
        // triangle
        initialise([](float x) { return (std::abs(x / juce::MathConstants<float>::pi) - 0.5f) * 2;  }, 200);
        break;
    case 2:
        // saw
        initialise([](float x) { return x / juce::MathConstants<float>::pi; }, 200);
        break;
    case 3:
        // square
        initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 200);
        break;
    default:
        jassertfalse; // error
        break;
    }
}

void OscData::setWaveFrequency(const int midiNoteNumber) {
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {
    process(juce::dsp::ProcessContextReplacing<float>(block));
}
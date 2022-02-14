/*
  ==============================================================================

    SynthSound.h
    Created: 13 Feb 2022 5:52:57pm
    Author:  Autumn Moulios

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};

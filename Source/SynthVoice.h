/*
  ==============================================================================

    SynthVoice.h
    Created: 13 Feb 2022 5:52:34pm
    Author:  Autumn Moulios

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice {
public:
    // Inherited via SynthesiserVoice
    bool canPlaySound(juce::SynthesiserSound*) override;
    virtual void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    virtual void stopNote(float velocity, bool allowTailOff) override;
    virtual void pitchWheelMoved(int newPitchWheelValue) override;
    virtual void controllerMoved(int controllerNumber, int newControllerValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void updateADSR(const float attack, const float decay, const float sustain, const float release);
    virtual void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

private:
    juce::dsp::Oscillator<float> sine{ [](float x) { return std::sin(x); }, 200 }; // Sine Wave Osc
    juce::dsp::Oscillator<float> saw{ [](float x) { return x / juce::MathConstants<float>::pi; }, 200 }; // Saw Wave Osc
    juce::dsp::Oscillator<float> square{ [](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 200 }; // Square Wave Osc

    juce::dsp::Oscillator<float> osc{ [](float x) { return x / juce::MathConstants<float>::pi; }, 200 };
    juce::dsp::Gain<float> gain; // Output Volume

    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;

    bool isPrepared{ false };
};
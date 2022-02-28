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
#include "Data/AdsrData.h"
#include "Data/OscData.h"

class SynthVoice : public juce::SynthesiserVoice {
public:
    // Inherited via SynthesiserVoice
    bool canPlaySound(juce::SynthesiserSound*) override;
    virtual void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    virtual void stopNote(float velocity, bool allowTailOff) override;
    virtual void pitchWheelMoved(int newPitchWheelValue) override;
    virtual void controllerMoved(int controllerNumber, int newControllerValue) override;
    void updateADSR(const float attack, const float decay, const float sustain, const float release);
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    virtual void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    OscData& getOscillator() { return osc; };

private:
    OscData osc;
    juce::dsp::Gain<float> gain; // Output Volume
    AdsrData adsr;

    juce::AudioBuffer<float> synthBuffer;
    bool isPrepared{ false };
};
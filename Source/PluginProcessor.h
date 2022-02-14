/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"

//==============================================================================
/**
*/
class Music167AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Music167AudioProcessor();
    ~Music167AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    juce::Synthesiser synth;

    juce::dsp::Oscillator<float> sine{ [](float x) { return std::sin(x); }, 200 }; // Sine Wave Osc
    juce::dsp::Oscillator<float> saw{ [](float x) { return x / juce::MathConstants<float>::pi; }, 200 }; // Saw Wave Osc
    juce::dsp::Oscillator<float> square{ [](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 200 }; // Square Wave Osc
    
    juce::dsp::Oscillator<float> osc{ [](float x) { return x / juce::MathConstants<float>::pi; }, 200 };
    juce::dsp::Gain<float> gain; // Output Volume

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Music167AudioProcessor)
};
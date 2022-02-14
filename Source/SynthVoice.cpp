/*
  ==============================================================================

    SynthVoice.cpp
    Created: 13 Feb 2022 5:52:34pm
    Author:  Autumn Moulios

  ==============================================================================
*/

#include "SynthVoice.h"
#include "SynthSound.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    adsr.noteOff();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    osc.prepare(spec);
    gain.prepare(spec);

    gain.setGainLinear(0.3f);

    isPrepared = true;
}

void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release) {
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    adsr.setParameters(adsrParams);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared);

    juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };
    osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
}

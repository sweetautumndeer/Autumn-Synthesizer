/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"
#include "SynthSound.h"

constexpr int NUMVOICES = 15; // max polyphony voices

//==============================================================================
Music167AudioProcessor::Music167AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound(new SynthSound());
    for (int i = 0; i < NUMVOICES; ++i)
        synth.addVoice(new SynthVoice());
}

Music167AudioProcessor::~Music167AudioProcessor()
{
}

//==============================================================================
const juce::String Music167AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Music167AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Music167AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Music167AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Music167AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Music167AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Music167AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Music167AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Music167AudioProcessor::getProgramName (int index)
{
    return {};
}

void Music167AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Music167AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    synth.setCurrentPlaybackSampleRate(sampleRate);

    // Prepare all voices
    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    }

    // Prepare filter
    filter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
}

void Music167AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Music167AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Music167AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Check that voices exist
    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            // Osc controls
            auto& oscWave0 = *apvts.getRawParameterValue("OSC0WAVETYPE");
            //auto& oscWave1 = *apvts.getRawParameterValue("OSC1WAVETYPE");
            //auto& oscWave2 = *apvts.getRawParameterValue("OSC2WAVETYPE");
            voice->getOscillator(0).setWaveType(oscWave0);
            //voice->getOscillator(1).setWaveType(oscWave1);
            //voice->getOscillator(2).setWaveType(oscWave2);

            // ADSR
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");
            voice->updateADSR(attack.load(), decay.load(), sustain.load(), release.load());
            
            // LFO
        }
    }

    // Render
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Filter
    auto& filterType = *apvts.getRawParameterValue("MASTERFILTERTYPE");
    auto& cutoffFreq = *apvts.getRawParameterValue("MASTERFILTERCUTOFF");
    auto& resonance = *apvts.getRawParameterValue("MASTERFILTERRESONANCE");
    filter.updateParameters(filterType, cutoffFreq, resonance);
    filter.process(buffer);
}

//==============================================================================
bool Music167AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Music167AudioProcessor::createEditor()
{
    return new Music167AudioProcessorEditor (*this);
}

//==============================================================================
void Music167AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void Music167AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Music167AudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout Music167AudioProcessor::createParams() 
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Osc 0 + Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC0WAVETYPE", "Osc 0 Wave Type", juce::StringArray{ "Sine", "Triangle", "Saw", "Square" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC0FILTERTYPE", "Osc 0 Filter Type", juce::StringArray{ "Low Pass", "Band Pass", "High Pass" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC0FILTERCUTOFF", "Osc 0 Filter Cutoff", juce::NormalisableRange<float>{ 20.0f, 20000.0f, 0.1f, }, 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC0FILTERRESONANCE", "Osc 0 Filter Resonance", juce::NormalisableRange<float>{ 1.0f, 10.0f, 0.1f }, 1.0f));
    // Osc 1 + Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", juce::StringArray{ "Sine", "Triangle", "Saw", "Square" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1FILTERTYPE", "Osc 1 Filter Type", juce::StringArray{ "Low Pass", "Band Pass", "High Pass" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1FILTERCUTOFF", "Osc 1 Filter Cutoff", juce::NormalisableRange<float>{ 20.0f, 20000.0f, 0.1f, }, 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1FILTERRESONANCE", "Osc 1 Filter Resonance", juce::NormalisableRange<float>{ 1.0f, 10.0f, 0.1f }, 1.0f));
    // Osc 2 + Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2WAVETYPE", "Osc 2 Wave Type", juce::StringArray{ "Sine", "Triangle", "Saw", "Square" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2FILTERTYPE", "Osc 2 Filter Type", juce::StringArray{ "Low Pass", "Band Pass", "High Pass" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2FILTERCUTOFF", "Osc 2 Filter Cutoff", juce::NormalisableRange<float>{ 20.0f, 20000.0f, 0.1f, }, 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2FILTERRESONANCE", "Osc 2 Filter Resonance", juce::NormalisableRange<float>{ 1.0f, 10.0f, 0.1f }, 1.0f));

    // ADSR parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>{ 0.01f, 1.0f, 0.01f }, 0.01f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>{ 0.1f, 1.0f, 0.01f }, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>{ 0.0f, 1.0f, 0.01f }, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>{ 0.0f, 3.0f, 0.01f }, 0.4f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MASTERGAIN", "Master Gain", juce::NormalisableRange<float>{ -80.0f, 20.0f, 0.01f }, 0.0f));

    // Master Filter parameters
    params.push_back(std::make_unique<juce::AudioParameterChoice>("MASTERFILTERTYPE", "Filter Type", juce::StringArray{ "Low Pass", "Band Pass", "High Pass" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MASTERFILTERCUTOFF", "Filter Cutoff", juce::NormalisableRange<float>{ 20.0f, 20000.0f, 0.1f, }, 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MASTERFILTERRESONANCE", "Filter Resonance", juce::NormalisableRange<float>{ 1.0f, 10.0f, 0.1f }, 1.0f));

    return{ params.begin(), params.end() };
}

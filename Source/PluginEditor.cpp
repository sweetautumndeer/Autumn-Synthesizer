/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Music167AudioProcessorEditor::Music167AudioProcessorEditor (Music167AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), adsr (audioProcessor.apvts), osc (audioProcessor.apvts, "OSC1WAVETYPE"), filter (audioProcessor.apvts, "FILTER1") {
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(adsr);
    addAndMakeVisible(osc);
    addAndMakeVisible(filter);
}

Music167AudioProcessorEditor::~Music167AudioProcessorEditor() {
}

//==============================================================================
void Music167AudioProcessorEditor::paint (juce::Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Autumn's Synthesizer", getLocalBounds(), juce::Justification::centredTop, 1);
}

void Music167AudioProcessorEditor::resized() {
    adsr.setBounds(getLocalBounds());
    osc.setBounds(10, 10, 100, 30);
    filter.setBounds(10, 125, 350, 100);
}



/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Music167AudioProcessorEditor::Music167AudioProcessorEditor (Music167AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), adsr (audioProcessor.apvts), osc0 (audioProcessor.apvts, "OSC0"), osc1 (audioProcessor.apvts, "OSC1"), 
    osc2 (audioProcessor.apvts, "OSC2"), filter (audioProcessor.apvts, "MASTERFILTER")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);

    addAndMakeVisible(adsr);
    addAndMakeVisible(osc0);
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);
    addAndMakeVisible(filter);
}

Music167AudioProcessorEditor::~Music167AudioProcessorEditor() 
{

}

//==============================================================================
void Music167AudioProcessorEditor::paint (juce::Graphics& g) 
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Autumn's Synthesizer", getLocalBounds(), juce::Justification::centredTop, 1);
}

void Music167AudioProcessorEditor::resized() 
{
    const auto bounds = getLocalBounds(); // get plugin bounds
    const auto padding = bounds.getWidth() / 80;
    const auto oscStartY = bounds.getHeight() / 24;
    const auto widthThirds = (bounds.getWidth() - padding * 4) / 3;
    const auto widthHalf = (bounds.getWidth() - padding * 3) / 2;
    const auto heightThirds = (bounds.getHeight() - padding * 4) / 3;

    osc0.setBounds(padding, oscStartY, widthThirds, heightThirds);
    osc1.setBounds(osc0.getRight() + padding, oscStartY, widthThirds, heightThirds);
    osc2.setBounds(osc1.getRight() + padding, oscStartY, widthThirds, heightThirds);
    adsr.setBounds(padding, osc0.getBottom() + padding, widthHalf, heightThirds);
    filter.setBounds(adsr.getRight() + padding, osc0.getBottom() + padding, widthHalf, heightThirds);
}



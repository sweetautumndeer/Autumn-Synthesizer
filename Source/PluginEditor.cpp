/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Music167AudioProcessorEditor::Music167AudioProcessorEditor (Music167AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    // Attach GUI elements to parameters
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    attackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);
    oscSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", oscSelector);

    setSliderParams(attackSlider, attackLabel, "Attack");
    setSliderParams(decaySlider, decayLabel, "Decay");
    setSliderParams(sustainSlider, sustainLabel, "Sustain");
    setSliderParams(releaseSlider, releaseLabel, "Release");
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

void Music167AudioProcessorEditor::resized() {
    const auto bounds = getLocalBounds().reduced(10); // get plugin bounds with margin
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getWidth() / 4 - padding;
    const auto sliderStartX = padding;
    const auto sliderStartY = bounds.getHeight() / 2 - (sliderHeight);

    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
}

void Music167AudioProcessorEditor::setSliderParams(juce::Slider& slider, juce::Label& label, juce::String name) {
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 160, slider.getTextBoxHeight());
    slider.setTextValueSuffix(" sec");
    addAndMakeVisible(slider);

    label.setText(name, juce::dontSendNotification);
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);
}

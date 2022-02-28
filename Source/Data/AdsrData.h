/*
  ==============================================================================

    AdsrData.h
    Created: 27 Feb 2022 10:34:26pm
    Author:  Autumn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AdsrData : public juce::ADSR {
public:
    void updateADSR(const float attack, const float decay, const float sustain, const float release);

private:
    juce::ADSR::Parameters adsrParams;

};
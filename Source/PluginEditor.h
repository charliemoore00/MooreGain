/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MooreGainAudioProcessorEditor  : public juce::AudioProcessorEditor//, private juce::Slider::Listener //inheritence for gain
{
public:
    MooreGainAudioProcessorEditor (MooreGainAudioProcessor&);
    ~MooreGainAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MooreGainAudioProcessor& audioProcessor;
    
    juce::Slider gain; //create gain slider

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MooreGainAudioProcessorEditor)
};

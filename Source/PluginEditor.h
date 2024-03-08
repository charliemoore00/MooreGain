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
class MooreGainAudioProcessorEditor  : public juce::AudioProcessorEditor, 
                                       private juce::Slider::Listener //for gain knob
{
public:
    MooreGainAudioProcessorEditor (MooreGainAudioProcessor&);
    ~MooreGainAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider* slider) override; //default callback function

private:
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MooreGainAudioProcessor& audioProcessor;
    
    juce::Slider gainSlider; //create gain slider

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MooreGainAudioProcessorEditor)
};

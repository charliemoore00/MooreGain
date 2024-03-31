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
    
    //==============================================================================
    //CHARLIE MOORE CODE
    
    void sliderValueChanged(juce::Slider* slider) override; //default callback function
    
    //slider attachment for parameter interaction with DAW
    juce::ScopedPointer <juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttach;
    //TODO: use std::unique_ptr because ScopedPointer is deprecated
    //std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttach;
    //sliderAttach = std::make_unique <juce::AudioProcessorValueTreeState:SliderAttachment> ("Gain");
    

private:
    
    //==============================================================================
    //CHARLIE MOORE CODE
    
    juce::ImageComponent mImageComponent;
    
    //quick way for the editor to access the processor object that created it.
    MooreGainAudioProcessor& audioProcessor;
    
    juce::Slider gainSlider; //create gain slider

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MooreGainAudioProcessorEditor)
};

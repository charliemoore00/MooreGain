/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#define INIT_GAIN -5.0

//==============================================================================
/**
*/
class MooreGainAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MooreGainAudioProcessor();
    ~MooreGainAudioProcessor() override;

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
    
    
    //==============================================================================
    //CHARLIE MOORE CODE
    
    //previousGain for gain smoothing in process block
    //float previousGain;
    
    //for plugin parameters for automation from DAW
    juce::AudioProcessorValueTreeState treeState;
    //for efficient parameter layout for APVTS constructor
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    

private:
    //==============================================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MooreGainAudioProcessor)
};

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MooreGainAudioProcessor::MooreGainAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        treeState(*this, nullptr, "PARAMETERS", {
                std::make_unique<juce::AudioParameterFloat>(GAIN_ID, GAIN_NAME, -60.0f, 0.0f, INIT_GAIN)
        })

#endif
{
    
    /* PARAM INTEGRATION */
    //juce::NormalisableRange<float> gainRange (-60.0, 0.0);
    
     
}

MooreGainAudioProcessor::~MooreGainAudioProcessor()
{
}

//==============================================================================
const juce::String MooreGainAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MooreGainAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MooreGainAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MooreGainAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MooreGainAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MooreGainAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MooreGainAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MooreGainAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MooreGainAudioProcessor::getProgramName (int index)
{
    return {};
}

void MooreGainAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MooreGainAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    /* FOR POTENTIAL GAIN SMOOTHING - DOES NOT WORK RN */
    //previousGain = rawVolume;
}

void MooreGainAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MooreGainAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MooreGainAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    
    /* POTENTIAL GAIN SMOOTHING - DOES NOT WORK RN */
    /*
    auto currentGain = rawVolume;
    
    if (juce::approximatelyEqual(static_cast<float>(previousGain), static_cast<float>(currentGain)))
    {
        buffer.applyGain(pow (10, currentGain / 20));
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
        previousGain = currentGain;
    }
    */
    
    /*
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
 
    //==========================================================================
    //CHARLIE MOORE CODE
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

   //iterate through each channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        //iterate through ALL the samples in the buffer
        for(int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            //actual gain processing
            channelData[sample] = buffer.getSample(channel, sample) * juce::Decibels::decibelsToGain(rawVolume);
                                                                            //make it logarithmic
        }
    }
     */
    
    /* CONDENSED VERSION USING applyGain: */
    
    auto sliderGainValue = treeState.getRawParameterValue(GAIN_ID);
    buffer.applyGain(juce::Decibels::decibelsToGain<float>(*sliderGainValue));
     
}

//==============================================================================
bool MooreGainAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MooreGainAudioProcessor::createEditor()
{
    return new MooreGainAudioProcessorEditor (*this);
}

//==============================================================================
void MooreGainAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    
    //create a copy of the AudioProcessorValueTreeState
    auto state = treeState.copyState();
    
    //create a dynamic pointer for the new xml for state information
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    
    if (xml != nullptr) {
        copyXmlToBinary(*xml, destData);
        std::cout << "copied xml to binary";
    }
    else
    {
        std::cout << "xml is null";
    }
    
     
    
}

void MooreGainAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // LOAD STATE FROM XML
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    //do some error checking for safety
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(treeState.state.getType()))
        {
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
    
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MooreGainAudioProcessor();
}

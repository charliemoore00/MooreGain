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
        rawVolume(INIT_GAIN),
        treeState(*this, nullptr)

#endif
{
    //create variable for the range of gain
    juce::NormalisableRange<float> gainRange (-60.0, 0.0);
    //create var for default value of gain
    float defaultValue = INIT_GAIN;
    
    //create and add parameter
    //TODO: switch code to JUCE official documentation - check forum
    treeState.createAndAddParameter(GAIN_ID, GAIN_NAME, GAIN_NAME, gainRange, defaultValue, nullptr, nullptr);
    treeState.state = juce::ValueTree("saved parameters");
    
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
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
            channelData[sample] = buffer.getSample(channel, sample) * (pow (10, rawVolume / 20));
                                                                        //make it logarithmic
        }
    }
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
    copyXmlToBinary(*xml, destData);
    
}

void MooreGainAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
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

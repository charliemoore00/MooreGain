/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MooreGainAudioProcessorEditor::MooreGainAudioProcessorEditor (MooreGainAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    //get our background image - returns type image
    auto backgroundImage = juce::ImageCache::getFromMemory(BinaryData::IMG_1325_jpeg, BinaryData::IMG_1325_jpegSize);
    
    if (! backgroundImage.isNull())
    {
        mImageComponent.setImage(backgroundImage, juce::RectanglePlacement::stretchToFit);
    }
    

    
    setSize (200, 300);
    
    //attaches treeState to gainSlider
    sliderAttach = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, GAIN_ID, gainSlider);
    
    
    //define gain knob
    gainSlider.setSliderStyle(juce::Slider::LinearVertical);
    gainSlider.setRange(0, 6, 0.01); //Simulating an Ableton mixing slider
                                    // after adjusting mathematically, should read -60 to 6
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 100, 25);
    gainSlider.setPopupDisplayEnabled(true, true, this);
    gainSlider.setTitle("Gain");
    gainSlider.setValue(1);
    
    //add a listener to the slider
    gainSlider.addListener(this);
    
    //add to editor
    addAndMakeVisible(&gainSlider);
    
}

MooreGainAudioProcessorEditor::~MooreGainAudioProcessorEditor()
{
}

//==============================================================================
void MooreGainAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawFittedText("Gain", getLocalBounds(), juce::Justification::topRight, 1);
}

void MooreGainAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor for when the window gets resized
    
    gainSlider.setBounds(getLocalBounds());
}

void MooreGainAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider)
    {
        //set rawVolume to what the slider sets it to - the processor will then set the gain appropriately
        audioProcessor.rawVolume = gainSlider.getValue();
    }
}

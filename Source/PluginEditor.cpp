/*
 * PluginEditor.cpp
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

DelayPluginAudioProcessorEditor::DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor& p)
    : AudioProcessorEditor (&p)
    , delayTimeAttachment (*p.params.delayTime, delayTime)
    , delayFeedbackAttachment (*p.params.delayFeedback, delayFeedback)
    , mixAttachment (*p.params.mix, mix)
    , audioProcessor (p)
{
    addAndMakeVisible (delayTime);
    addAndMakeVisible (delayFeedback);
    addAndMakeVisible (mix);
    
    setSize (400, 300);
}

void DelayPluginAudioProcessorEditor::paint (juce::Graphics& Graphics)
{
    const auto width = getWidth() - 10;
    const auto height = getHeight() - 10;
    
    Graphics.fillAll (juce::Colours::black);
    
    auto mainBounds = getLocalBounds().toFloat().withSizeKeepingCentre (width, height);
    
    
    Graphics.setColour (juce::Colours::blue);
    Graphics.fillRoundedRectangle (mainBounds, 10.f);
    
    Graphics.setColour (juce::Colours::black);
    Graphics.fillRoundedRectangle (mainBounds.removeFromTop (mainBounds.getHeight() * 0.5).reduced (5, 5), 10.f);
    
}

void DelayPluginAudioProcessorEditor::resized()
{
    auto bounds = getBounds().removeFromBottom (getHeight() * 0.5f);
    auto sliderWidth = getWidth() / 3;
    
    delayTime.setBounds (bounds.removeFromLeft (sliderWidth));
    delayFeedback.setBounds (bounds.removeFromLeft (sliderWidth));
    mix.setBounds (bounds);
}

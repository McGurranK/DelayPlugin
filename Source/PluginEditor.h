/*
 * PluginEditor.h
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class DelayPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor&);
    ~DelayPluginAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider delayTime;
    juce::Slider delayFeedback;
    juce::Slider mix;
    
    juce::SliderParameterAttachment delayTimeAttachment;
    juce::SliderParameterAttachment delayFeedbackAttachment;
    juce::SliderParameterAttachment mixAttachment;
    
    DelayPluginAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessorEditor)
};

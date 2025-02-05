/*
 * PluginEditor.h
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FifoGUIBuffer.h"
#include "ScrollingWaveform.h"


class DelayPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor&);
    ~DelayPluginAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CustomScrollingView graphView;
    
    juce::Slider delayTime { juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox };
    juce::Slider delayFeedback { juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox };
    juce::Slider mix { juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox };
    
    juce::SliderParameterAttachment delayTimeAttachment;
    juce::SliderParameterAttachment delayFeedbackAttachment;
    juce::SliderParameterAttachment mixAttachment;
    
    DelayPluginAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessorEditor)
};

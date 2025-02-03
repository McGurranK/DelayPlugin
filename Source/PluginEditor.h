/*
 * PluginEditor.h
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

// Class to draw the delay signal ontop of the dry signal over 4 seconds
// Todo: steal my Fifo code from Atesh
// Todo
class CustomScrollingView final : public juce::Component
{
public:
    CustomScrollingView (juce::RangedAudioParameter& MixControlParameter)
    : mixControlAttachment (MixControlParameter, [this](float MixParameterValue) { mixParameterValue = MixParameterValue; })
    , vblankAttachment (this, [this]{ updatePoints(); })
    
    {}
    
    float calculateXPosition()
    {
        return 0.f;
    }
    
    float calculateYPosition()
    {
        return 0.f;
    }
    
    void updatePoints()
    {
        repaint();
    }
    
    void paint (juce::Graphics& GraphicsRef)
    {
        GraphicsRef.setColour (juce::Colours::purple.withAlpha ( 1.f - mixParameterValue));
        GraphicsRef.fillPath (drySignalPath);
        
        GraphicsRef.setColour (juce::Colours::purple.withAlpha ( 1.f - mixParameterValue));
        GraphicsRef.fillPath (wetSignalPath);
    }
    
    float mixParameterValue { 1.f };
    
    juce::Path drySignalPath;
    juce::Path wetSignalPath;
    
    juce::ParameterAttachment mixControlAttachment;
    juce::VBlankAttachment vblankAttachment;
};

class DelayPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor&);
    ~DelayPluginAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider delayTime { juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox };
    juce::Slider delayFeedback { juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox };
    juce::Slider mix { juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox };
    
    juce::SliderParameterAttachment delayTimeAttachment;
    juce::SliderParameterAttachment delayFeedbackAttachment;
    juce::SliderParameterAttachment mixAttachment;
    
    DelayPluginAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessorEditor)
};

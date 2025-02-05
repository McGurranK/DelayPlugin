#pragma once

#include <JuceHeader.h>
#include "FifoGUIBuffer.h"

class CustomScrollingView final : public juce::Component
{
public:
    CustomScrollingView (juce::RangedAudioParameter& MixControlParameter, FifoBuffer& DryFifoRef, FifoBuffer& WetFifoRef);
    ~CustomScrollingView() override;
    
    float calculateYPosition (float magnitude);
    
private:
    void configure ();
    void updatePoints();
    void paint (juce::Graphics& GraphicsRef) override;
    
    float mixParameterValue { 1.f };
    
    juce::Array<float> dryMagnitudes;
    juce::Array<float> wetMagnitudes;
    
    juce::AudioBuffer<float> drySignalBuffer;
    juce::AudioBuffer<float> wetSignalBuffer;
    
    FifoBuffer& drySignal;
    FifoBuffer& wetSignal;
    
    juce::Path drySignalPath;
    juce::Path wetSignalPath;
    
    juce::ParameterAttachment mixControlAttachment;
    juce::VBlankAttachment vblankAttachment;
};

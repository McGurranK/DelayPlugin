/*
* ScopedMixer.h
*/

#pragma once

template <class SampleType>
class ScopedReplacingContextMixer final
{
public:
    explicit ScopedReplacingContextMixer (juce::AudioBuffer<SampleType>& OutputBufferRef, juce::dsp::DryWetMixer<SampleType>& MixerRef)
        : outputBuffer (OutputBufferRef)
        , mixerDSPRef (MixerRef)
    {
        juce::dsp::AudioBlock<SampleType> inputBlock { outputBuffer };
        mixerDSPRef.pushDrySamples (inputBlock);
    }
    
    ~ScopedReplacingContextMixer()
    {
        juce::dsp::AudioBlock<SampleType> outputBlock { outputBuffer };
        mixerDSPRef.mixWetSamples (outputBlock);
    }
    
private:
    juce::AudioBuffer<SampleType>& outputBuffer;
    juce::dsp::DryWetMixer<SampleType>& mixerDSPRef;
};

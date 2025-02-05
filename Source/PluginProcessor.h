
#pragma once

#include <JuceHeader.h>
#include "FifoGUIBuffer.h"

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


struct ParametersStucture
{
    ParametersStucture (juce::AudioProcessor& ProcesorRef)
    {
        delayTime = new juce::AudioParameterFloat("DT", "Delay Time", juce::NormalisableRange<float> (1.f, 48000.f), 2000.f);
        delayFeedback = new juce::AudioParameterFloat("FB", "Delay Feedback", juce::NormalisableRange<float> (0.f, 1.f), 0.5f);
        mix = new juce::AudioParameterFloat("mix", "Wet & Dry Mix", juce::NormalisableRange<float> (0.f, 1.f), 0.5f);
        
        ProcesorRef.addParameter (delayTime);
        ProcesorRef.addParameter (delayFeedback);
        ProcesorRef.addParameter (mix);
    }
    
    juce::RangedAudioParameter* delayTime;
    juce::RangedAudioParameter* delayFeedback;
    juce::RangedAudioParameter* mix;
};


class DelayPluginAudioProcessor : public juce::AudioProcessor
{
public:
	DelayPluginAudioProcessor();
	~DelayPluginAudioProcessor() override;

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;
    
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

    ParametersStucture params;
    
    FifoBuffer dryFifo;
    FifoBuffer wetFifo;
    
private:
    juce::dsp::DryWetMixer<float> mixer;
    juce::dsp::DelayLine<float> delayLine;
    
    juce::SmoothedValue<float> delayTimeSmoothing;
    juce::SmoothedValue<float> gainSmoothing;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessor)
};						  

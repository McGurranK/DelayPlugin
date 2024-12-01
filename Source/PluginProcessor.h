
#pragma once

#include <JuceHeader.h>


// Todo : Unit test
class DCBlocker final
{
public:
    DCBlocker () = default;
    
    void process (juce::AudioBuffer<float>& AudioBufferRef)
    {
        const auto numberOfChannels = AudioBufferRef.getNumChannels();
        const auto numberOfSamples = AudioBufferRef.getNumSamples();
        
        for (int channelIndex = 0; channelIndex < numberOfChannels; channelIndex++)
        {
            const auto channelPointer = AudioBufferRef.getWritePointer (channelIndex);
            
            for (int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
            {
                auto currentInput = channelPointer[sampleIndex];
                channelPointer[sampleIndex] = currentInput - previousInputSample + (r * previousOutputSample);
                previousInputSample = currentInput;
                previousInputSample = channelPointer[sampleIndex];
            }
        }
    }
    
    float previousInputSample { 0.f };
    float previousOutputSample { 0.f };
    float r { 0.955f };
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

private:
    
	std::vector<float>	mDelayBufferLeft { 1024, 0 };
	std::vector<float>	mDelayBufferRight { 1024, 0 };
	
	int mWritePosition{ 0 }, mReadPosition{ 0 }, mDelayBufferSize{ 0 }, mFeedbackIndex{ 0 };

	float mDelayTime{0.5};
	float outputLeft{ 0 };
	float outputRight{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessor)
};						  

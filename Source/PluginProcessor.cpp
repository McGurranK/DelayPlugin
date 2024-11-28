#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayPluginAudioProcessor::DelayPluginAudioProcessor()
     : AudioProcessor (BusesProperties().withInput  ("Input", juce::AudioChannelSet::stereo(), true).withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
}

DelayPluginAudioProcessor::~DelayPluginAudioProcessor()
{

	mDelayBufferLeft.clear();
	mDelayBufferRight.clear();
}

const juce::String DelayPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayPluginAudioProcessor::acceptsMidi() const
{
    return false;
}

bool DelayPluginAudioProcessor::producesMidi() const
{
    return false;
}

bool DelayPluginAudioProcessor::isMidiEffect() const
{
    return false;
}

double DelayPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayPluginAudioProcessor::getNumPrograms()
{
    return 1;
}

int DelayPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	mDelayBufferSize = (mDelayTime*sampleRate)*2;
	
	mDelayBufferLeft.resize(mDelayBufferSize);
	mDelayBufferRight.resize(mDelayBufferSize);
}

void DelayPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DelayPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    
    return true;
}

void DelayPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

		// Incoming samples

		auto *channelDataLeft = buffer.getWritePointer(0);
		auto *channelDataRight = buffer.getWritePointer(1);
		
		// interate through each sample
		for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
		{			
			// Feedback Parameter
			mFeedbackIndex = mWritePosition - (mDelayBufferSize / 2);
			if (mFeedbackIndex < 0) 
			{
				mFeedbackIndex = mDelayBufferSize + mFeedbackIndex;
			}
			
			if (mFeedbackIndex > mDelayBufferSize) 
			{
				mFeedbackIndex = 0;
			}

			channelDataLeft[sample] += (mDelayBufferLeft[mFeedbackIndex])*0.5;
			channelDataRight[sample] += (mDelayBufferRight[mFeedbackIndex])*0.5;

			// If the write position is less than the delay bufferSize
			// Set the data at this position in the incoming channel data
			if (mWritePosition < mDelayBufferSize)
			{

				mDelayBufferLeft[mWritePosition] = channelDataLeft[sample];
				mDelayBufferRight[mWritePosition] = channelDataRight[sample];


			}
			// Else if the write position is greater than or equal to the delay buffer size
			// Reset the write position to Zero and write the sample into this position
			else if (mWritePosition >= mDelayBufferSize)
			{
				mWritePosition = 0;
				mDelayBufferLeft[mWritePosition] = channelDataLeft[sample];
				mDelayBufferRight[mWritePosition] = channelDataRight[sample];
			}

			// Readposition is the the write position divided by two
			// Done this way to leave extra space
			mReadPosition = mWritePosition - (mDelayBufferSize / 2);


			// If the read position is less than zero it will be a minus value
			// So to wrap the index around add this to the buffer size
			if (mReadPosition < 0)
			{
				mReadPosition = mDelayBufferSize + mReadPosition;
			}
			
			//
			if (mReadPosition > mDelayBufferSize)
			{
				mReadPosition = 0;

			}				

			// Writing data to the left and write channels
			channelDataLeft[sample] += mDelayBufferLeft[mReadPosition];
			channelDataRight[sample] += mDelayBufferRight[mReadPosition];


		// Iterate write and read position
		mWritePosition++;
		mReadPosition++;
}
}

bool DelayPluginAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayPluginAudioProcessor::createEditor()
{
    return new DelayPluginAudioProcessorEditor (*this);
}


void DelayPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void DelayPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayPluginAudioProcessor();
}

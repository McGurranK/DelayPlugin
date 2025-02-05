#include "PluginProcessor.h"
#include "PluginEditor.h"

DelayPluginAudioProcessor::DelayPluginAudioProcessor()
     : AudioProcessor (BusesProperties().withInput  ("Input", juce::AudioChannelSet::stereo(), true).withOutput ("Output", juce::AudioChannelSet::stereo(), true))
     , params (*this)
     , dryFifo (4096, 2)
     , wetFifo (4096, 2)
{
}

DelayPluginAudioProcessor::~DelayPluginAudioProcessor()
{
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

void DelayPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumInputChannels();
    
    delayLine.prepare (spec);
    mixer.prepare (spec);
    
    mixer.setMixingRule(juce::dsp::DryWetMixingRule::balanced);
    mixer.setWetMixProportion(0.5f);
    
    delayLine.setMaximumDelayInSamples (sampleRate * 4);
    delayLine.setDelay (1000);
    
    delayTimeSmoothing.setCurrentAndTargetValue (0.f);
    delayTimeSmoothing.reset (sampleRate, 0.1);

    gainSmoothing.setCurrentAndTargetValue (0.4f);
    gainSmoothing.reset (sampleRate, 0.1);
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
    
    auto delayRange = params.delayTime->getNormalisableRange();
    
    delayTimeSmoothing.setTargetValue (params.delayTime->getValue());
    gainSmoothing.setTargetValue (params.delayFeedback->getValue());
    
    dryFifo.addToFifo (buffer);
    
    mixer.setWetMixProportion (params.mix->getValue());

    {
        ScopedReplacingContextMixer<float> scopedMixing (buffer, mixer);
        
        for (int channelIndex = 0; channelIndex < totalNumInputChannels; channelIndex++)
        {
            auto channelPointer = buffer.getWritePointer (channelIndex);
            
            for (int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
            {
                auto& samplePointer = channelPointer [sampleIndex];
                auto tempInput = samplePointer;
                
                auto delayTime = delayRange.convertFrom0to1 (delayTimeSmoothing.getNextValue());
                
                samplePointer = (delayLine.popSample (channelIndex, delayTime) * gainSmoothing.getNextValue());
                
                tempInput += samplePointer;
                
                delayLine.pushSample (channelIndex, tempInput);
            }
        }
        
        wetFifo.addToFifo (buffer);
    }
}

bool DelayPluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* DelayPluginAudioProcessor::createEditor()
{
    return new DelayPluginAudioProcessorEditor (*this);
}

void DelayPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::XmlElement stateXML ("Parameters");
    
    for (auto& parameter : { params.delayTime, params.delayFeedback, params.mix})
    {
        const auto parameterName = parameter->paramID;
        auto parameterValue = parameter->getValue();
        const auto range = parameter->getNormalisableRange();
        parameterValue = range.convertFrom0to1 (parameterValue);
        
        const auto parameterValueString = juce::String(parameterValue);
        
        auto state = stateXML.createNewChildElement(parameterName);
        state->setAttribute ("Value" , parameterValueString);
    }
    
    copyXmlToBinary (stateXML, destData);
}

void DelayPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    const auto xmlElement = getXmlFromBinary (data, sizeInBytes);
        
    if (xmlElement.get())
    {
        auto parameter = xmlElement->getFirstChildElement();
        auto str = "Value";
        
        while (parameter != nullptr)
        {
            const auto parameterName = parameter->getTagName();
            const auto parameterText = parameter->getStringAttribute (str);
            
            // Check name for parameters & set value
            
            parameter = parameter->getNextElement();
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayPluginAudioProcessor();
}

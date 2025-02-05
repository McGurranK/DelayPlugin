/*
* FifoBuffer.cpp
*/

#include "FifoGUIBuffer.h"

FifoBuffer::FifoBuffer (int BufferSize, int NumberOfChannels)
    : internalSamplesBuffer (NumberOfChannels, BufferSize)
{
    // Check you have set channelNumber of NumberOfSamples correctly
    jassert (BufferSize > NumberOfChannels);
    
    readAndWriteIndex.setTotalSize (BufferSize);
}
    
void FifoBuffer::setAtomicFlag (bool StartDrawing)
{
    if (StartDrawing)
    {
        internalSamplesBuffer.clear();
        readAndWriteIndex.reset();
    }

    readyToWrite.exchange (StartDrawing);
}
    
void FifoBuffer::addToFifo (juce::AudioBuffer<float>& AudioBuffer)
{
    if (! readyToWrite)
        return;
    
    auto scopedWriter = juce::AbstractFifo::ScopedWrite (readAndWriteIndex, AudioBuffer.getNumSamples());
    
    if (scopedWriter.blockSize1 != 0)
    {
        for (int currentChannel = 0; currentChannel < AudioBuffer.getNumChannels(); currentChannel++)
            internalSamplesBuffer.copyFrom (currentChannel, scopedWriter.startIndex1, AudioBuffer, currentChannel, 0, scopedWriter.blockSize1);
    }
    if (scopedWriter.blockSize2 != 0)
    {
        for (int currentChannel = 0; currentChannel < AudioBuffer.getNumChannels(); currentChannel++)
            internalSamplesBuffer.copyFrom (currentChannel, scopedWriter.startIndex2, AudioBuffer, currentChannel, scopedWriter.blockSize1, scopedWriter.blockSize2);
    }
}
    
int FifoBuffer::readFromFiFo (juce::AudioBuffer<float>& AudioBuffer)
{
    /* You haven't set the atomic flag from the GUI to tell the DSP to start writing samples*/
    jassert (readyToWrite);
    
    // Channel Count is not the same
    jassert (AudioBuffer.getNumChannels() == internalSamplesBuffer.getNumChannels());
    
    auto scopedReader = juce::AbstractFifo::ScopedRead (readAndWriteIndex, static_cast<int> (AudioBuffer.getNumSamples()));
    
    if (scopedReader.blockSize1 != 0)
    {
        for (int channel = 0; channel < AudioBuffer.getNumChannels(); channel++)
            AudioBuffer.copyFrom (channel, 0, internalSamplesBuffer, channel, scopedReader.startIndex1, scopedReader.blockSize1);
    }
    
    if (scopedReader.blockSize2 != 0)
    {
        for (int channel = 0; channel < AudioBuffer.getNumChannels(); channel++)
            AudioBuffer.copyFrom (channel, scopedReader.blockSize1, internalSamplesBuffer, channel, scopedReader.startIndex2, scopedReader.blockSize2);
      }
    
    return static_cast<int> (scopedReader.blockSize1 + scopedReader.blockSize2);
}

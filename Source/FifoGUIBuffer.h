/*
* AteshFiFo.h
*/

#pragma once

/*
* Fifo for drawing blocks of audio without it dropping out
*/

#include <JuceHeader.h>

#pragma once

class FifoBuffer final
{
public:
    FifoBuffer (int BufferSize, int NumberOfChannels);
    
    void setAtomicFlag (bool StartDrawing);
    
    void addToFifo (juce::AudioBuffer<float>& AudioBuffer);
    
    int readFromFiFo (juce::AudioBuffer<float>& AudioBuffer);
    
private:
    std::atomic_bool readyToWrite { false };
    
    juce::AbstractFifo readAndWriteIndex { 1024 };
    
    juce::AudioBuffer<float> internalSamplesBuffer;
};

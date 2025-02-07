/*
* Paramaters.h
*/

#pragma once

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
    
    juce::RangedAudioParameter* delayTime { nullptr };
    juce::RangedAudioParameter* delayFeedback { nullptr };
    juce::RangedAudioParameter* mix { nullptr };

private:
    
    JUCE_LEAK_DETECTOR (ParametersStucture)
};

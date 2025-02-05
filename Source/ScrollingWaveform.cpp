#include "ScrollingWaveform.h"

CustomScrollingView::CustomScrollingView (juce::RangedAudioParameter& MixControlParameter, FifoBuffer& DryFifoRef, FifoBuffer& WetFifoRef)
: drySignalBuffer (2, 2048)
, wetSignalBuffer (2, 2048)
, drySignal (DryFifoRef)
, wetSignal (WetFifoRef)
, mixControlAttachment (MixControlParameter, [this](float MixParameterValue) { mixParameterValue = MixParameterValue; })
, vblankAttachment (this, [this]{ updatePoints(); })
{
    drySignal.setAtomicFlag (true);
    wetSignal.setAtomicFlag (true);
}

CustomScrollingView::~CustomScrollingView()
{
    drySignal.setAtomicFlag (false);
    wetSignal.setAtomicFlag (false);
}

float CustomScrollingView::calculateYPosition (float magnitude)
{
    const auto height = static_cast<float> (getHeight());
    const auto db = juce::Decibels::gainToDecibels (magnitude);
    
    return std::clamp ((height - juce::jmap (db, -60.f, 0.f, 0.f, height)), 0.f, height);
}

void CustomScrollingView::configure ()
{
    dryMagnitudes.ensureStorageAllocated (getWidth() * 2.f);
    wetMagnitudes.ensureStorageAllocated (getWidth() * 2.f);
    drySignalPath.preallocateSpace (getWidth() * 4.f);
    wetSignalPath.preallocateSpace (getWidth() * 4.f);
}

void CustomScrollingView::updatePoints()
{
    // Preallocate Resources here
    configure();
    
    const auto drySamplesNumber = drySignal.readFromFiFo (drySignalBuffer);
    const auto wetSamplesNumber = wetSignal.readFromFiFo (wetSignalBuffer);
    
    const auto magnitudeDry = drySignalBuffer.getMagnitude (0, drySamplesNumber);
    const auto magnitudeWet = wetSignalBuffer.getMagnitude (0, wetSamplesNumber);
    
    // Lock internally shouldn't use
    if (dryMagnitudes.size() >= getWidth())
        dryMagnitudes.remove (0);
    if (wetMagnitudes.size() >= getWidth())
        wetMagnitudes.remove (0);
    
    dryMagnitudes.add (magnitudeDry);
    wetMagnitudes.add (magnitudeWet);
    
    repaint();
}

void CustomScrollingView::paint (juce::Graphics& GraphicsRef)
{
    jassert (dryMagnitudes.size() == wetMagnitudes.size());
    
    drySignalPath.clear();
    wetSignalPath.clear();
    
    for (int index = 0; index < dryMagnitudes.size(); index++)
    {
        const auto dryY = calculateYPosition (dryMagnitudes[index]);
        const auto wetY = calculateYPosition (wetMagnitudes[index]);
        
        if (index == 0)
        {
            drySignalPath.startNewSubPath (index, dryY);
            wetSignalPath.startNewSubPath (index, wetY);
        }
        else
        {
            drySignalPath.lineTo (index, dryY);
            wetSignalPath.lineTo (index, wetY);
        }
    }
    
    const auto closePath = [this] (juce::Path& PathRef)
    {
        const auto bounds = PathRef.getBounds();
        PathRef.lineTo(bounds.getBottomRight().withY (getHeight()));
        PathRef.lineTo(bounds.getBottomLeft().withY (getHeight()));
        PathRef.closeSubPath();
    };
    
    closePath (drySignalPath);
    closePath (wetSignalPath);
    
    GraphicsRef.setColour (juce::Colours::white.withAlpha ( 1.f - mixParameterValue));
    GraphicsRef.fillPath (drySignalPath);
    
    GraphicsRef.setColour (juce::Colours::purple.withAlpha (mixParameterValue));
    GraphicsRef.fillPath (wetSignalPath);
}

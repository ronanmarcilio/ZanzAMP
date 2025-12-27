#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "PluginProcessor.h"

class ZanzAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ZanzAudioProcessorEditor (ZanzAudioProcessor&);
    ~ZanzAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ZanzAudioProcessor& audioProcessor;

    juce::Slider preAmpSlider, buzzSlider, punchSlider, crunchSlider, driveSlider, lowSlider, highSlider, levelSlider;
    juce::ToggleButton bypassButton;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preAmpAttachment, buzzAttachment, punchAttachment, crunchAttachment, driveAttachment, lowAttachment, highAttachment, levelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;

    void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZanzAudioProcessorEditor)
};

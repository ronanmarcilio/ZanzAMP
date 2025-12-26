#include "PluginProcessor.h"
#include "PluginEditor.h"

ZanzAudioProcessorEditor::ZanzAudioProcessorEditor (ZanzAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    auto setupSlider = [this](juce::Slider& slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment, const juce::String& paramID) {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(slider);
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, paramID, slider);
    };

    setupSlider(preAmpSlider, preAmpAttachment, "PREAMP");
    setupSlider(buzzSlider, buzzAttachment, "BUZZ");
    setupSlider(punchSlider, punchAttachment, "PUNCH");
    setupSlider(crunchSlider, crunchAttachment, "CRUNCH");
    setupSlider(driveSlider, driveAttachment, "DRIVE");
    setupSlider(lowSlider, lowAttachment, "LOW");
    setupSlider(highSlider, highAttachment, "HIGH");
    setupSlider(levelSlider, levelAttachment, "LEVEL");

    bypassButton.setButtonText("BYPASS");
    addAndMakeVisible(bypassButton);
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "BYPASS", bypassButton);

    setSize (900, 350);
}

ZanzAudioProcessorEditor::~ZanzAudioProcessorEditor() {}

void ZanzAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background - Brushed Metal Look
    auto backgroundGradient = juce::ColourGradient(
        juce::Colour(0xffb0b0b0), 0, 0,
        juce::Colour(0xff808080), 0, getHeight(), false);
    g.setGradientFill(backgroundGradient);
    g.fillAll();
    
    // Subtle brushed metal texture lines
    g.setColour (juce::Colours::black.withAlpha(0.05f));
    for (int i = 0; i < getWidth(); i += 1)
        g.drawVerticalLine(i, 0, getHeight());

    // Top Bar (Dark area like in the image)
    g.setColour(juce::Colour(0xff333333));
    g.fillRect(0, 0, getWidth(), 50);

    g.setColour (juce::Colours::white);
    g.setFont (juce::Font("Arial", 24.0f, juce::Font::bold));
    g.drawText ("ZanzAMP", 20, 10, 200, 30, juce::Justification::left);
    
    g.setColour(juce::Colours::black);
    g.setFont (juce::Font("Arial", 14.0f, juce::Font::bold));
    
    auto drawLabel = [&](const juce::String& text, juce::Rectangle<int> area) {
        g.drawText(text, area.withY(area.getY() - 25).withHeight(20), juce::Justification::centred);
    };

    drawLabel("PRE-AMP", preAmpSlider.getBounds());
    drawLabel("BUZZ", buzzSlider.getBounds());
    drawLabel("PUNCH", punchSlider.getBounds());
    drawLabel("CRUNCH", crunchSlider.getBounds());
    drawLabel("DRIVE", driveSlider.getBounds());
    drawLabel("LOW", lowSlider.getBounds());
    drawLabel("HIGH", highSlider.getBounds());
    drawLabel("LEVEL", levelSlider.getBounds());

    // Bottom Branding
    g.setFont (juce::Font("Arial", 18.0f, juce::Font::bold));
    g.drawText("ZanzAMP", 20, getHeight() - 40, 150, 30, juce::Justification::left);
    g.setFont (10.0f);
    g.drawText("MODEL Z-1", getWidth() - 100, getHeight() - 30, 80, 20, juce::Justification::right);
}

void ZanzAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    
    // Top Bar area
    auto topBar = area.removeFromTop(50);
    bypassButton.setBounds(topBar.removeFromRight(100).reduced(5));

    area.removeFromTop(40); // Space for labels
    
    auto width = area.getWidth() / 8;
    preAmpSlider.setBounds(area.removeFromLeft(width));
    buzzSlider.setBounds(area.removeFromLeft(width));
    punchSlider.setBounds(area.removeFromLeft(width));
    crunchSlider.setBounds(area.removeFromLeft(width));
    driveSlider.setBounds(area.removeFromLeft(width));
    lowSlider.setBounds(area.removeFromLeft(width));
    highSlider.setBounds(area.removeFromLeft(width));
    levelSlider.setBounds(area.removeFromLeft(width));
}

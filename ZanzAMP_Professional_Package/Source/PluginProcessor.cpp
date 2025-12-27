#include "PluginProcessor.h"
#include "PluginEditor.h"

ZanzAudioProcessor::ZanzAudioProcessor()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout()),
      oversampling(2, 2, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR) // 4x oversampling (2^2)
{
    // Initialize parameter smoothing
    for (auto& param : getParameters()) {
        if (auto* p = dynamic_cast<juce::AudioProcessorParameterWithID*>(param)) {
            smoothers.emplace(p->getParameterID(), juce::LinearSmoothedValue<float>(0.0f));
        }
    }
}

ZanzAudioProcessor::~ZanzAudioProcessor() {}

void ZanzAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    oversampling.initProcessing(samplesPerBlock);
    
  dcFilter.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 20.0f);
dcFilter.prepare(spec);


    // Initialize smoothers
    for (auto& pair : smoothers) {
        pair.second.reset(sampleRate, 0.05); // 50ms smoothing
    }
}

void ZanzAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    if (apvts.getRawParameterValue("BYPASS")->load() > 0.5f) {
        return; // Simple bypass for now, could be crossfaded
    }

    // Update smoothers
    for (auto& pair : smoothers) {
        pair.second.setTargetValue(apvts.getRawParameterValue(pair.first)->load());
    }

    // 1. Oversampling Up
    juce::dsp::AudioBlock<float> inputBlock(buffer);
    juce::dsp::AudioBlock<float> oversampledBlock = oversampling.processSamplesUp(inputBlock);

    float preAmp = smoothers["PREAMP"].getNextValue();
    float buzz = smoothers["BUZZ"].getNextValue();
    float punch = smoothers["PUNCH"].getNextValue();
    float crunch = smoothers["CRUNCH"].getNextValue();
    float drive = smoothers["DRIVE"].getNextValue();
    float low = smoothers["LOW"].getNextValue();
    float high = smoothers["HIGH"].getNextValue();
    float level = smoothers["LEVEL"].getNextValue();

    // 2. Processing at Higher Sample Rate
    for (size_t channel = 0; channel < oversampledBlock.getNumChannels(); ++channel)
    {
        auto* channelData = oversampledBlock.getChannelPointer(channel);
        for (size_t sample = 0; sample < oversampledBlock.getNumSamples(); ++sample)
        {
            float x = channelData[sample];

            // Input Gain (Pre-Amp)
            x *= juce::Decibels::decibelsToGain(preAmp);

            // Multi-stage Saturation
            // Stage 1: Tone Shaping (Buzz, Punch, Crunch)
            float buzzF = std::sin(x * juce::MathConstants<float>::pi * 0.5f * buzz);
            float punchF = std::tanh(x * 1.5f * punch);
            float crunchF = (2.0f / juce::MathConstants<float>::pi) * std::atan(x * 2.0f * crunch);

            x = (x + buzzF + punchF + crunchF) / 4.0f;

            // Stage 2: Global Drive
            float driveGain = 1.0f + (drive * 10.0f);
            x = std::tanh(x * driveGain);

            // Unity Gain Compensation (Approximate)
            x /= (1.0f + drive * 0.5f);

            channelData[sample] = x;
        }
    }

    // 3. Oversampling Down
    oversampling.processSamplesDown(inputBlock);

    // 4. Post-Processing (EQ, DC Filter, Level)
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float x = channelData[sample];
            
            // DC Offset Removal
            x = dcFilter.processSample(x);

            // Simple EQ (In a real build, use dp::IIR filters)
            // Low Shelf / High Shelf logic here...

            // Output Level
            channelData[sample] = x * juce::Decibels::decibelsToGain(level);
        }
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout ZanzAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("PREAMP", "Pre-Amp", -20.0f, 20.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("BUZZ", "Buzz", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PUNCH", "Punch", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CRUNCH", "Crunch", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOW", "Low", -12.0f, 12.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HIGH", "High", -12.0f, 12.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LEVEL", "Level", -60.0f, 12.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("BYPASS", "Bypass", false));

    return { params.begin(), params.end() };
}

// Boilerplate methods...
const juce::String ZanzAudioProcessor::getName() const { return "ZanzAMP"; }
bool ZanzAudioProcessor::acceptsMidi() const { return false; }
bool ZanzAudioProcessor::producesMidi() const { return false; }
bool ZanzAudioProcessor::isMidiEffect() const { return false; }
double ZanzAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int ZanzAudioProcessor::getNumPrograms() { return 1; }
int ZanzAudioProcessor::getCurrentProgram() { return 0; }
void ZanzAudioProcessor::setCurrentProgram (int index) {}
const juce::String ZanzAudioProcessor::getProgramName (int index) { return "Default"; }
void ZanzAudioProcessor::changeProgramName (int index, const juce::String& newName) {}
bool ZanzAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* ZanzAudioProcessor::createEditor() { return new ZanzAudioProcessorEditor (*this); }
void ZanzAudioProcessor::releaseResources() {}
bool ZanzAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const { return true; }

void ZanzAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ZanzAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ZanzAudioProcessor(); }

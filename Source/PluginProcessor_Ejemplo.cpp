/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SideChain.h"
//==============================================================================
HerewegoagainAudioProcessor::HerewegoagainAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

HerewegoagainAudioProcessor::~HerewegoagainAudioProcessor()
{
}

//==============================================================================
const juce::String HerewegoagainAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HerewegoagainAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HerewegoagainAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HerewegoagainAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HerewegoagainAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HerewegoagainAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HerewegoagainAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HerewegoagainAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HerewegoagainAudioProcessor::getProgramName (int index)
{
    return {};
}

void HerewegoagainAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HerewegoagainAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    float Fs = AudioProcessor::getSampleRate();
    
    LPFR.setSRandFT(Fs,2);
    LPFR.setFc(300);
    LPFL.setSRandFT(Fs,2);
    LPFL.setFc(300);
    
    HPFR.setSRandFT(Fs,1);
    HPFR.setFc(4000);
    HPFL.setSRandFT(Fs,1);
    HPFL.setFc(4000);
    
    HPFmL.setSRandFT(Fs, 1);
    HPFmL.setFc(300);
    LPFmL.setSRandFT(Fs, 2);
    LPFmL.setFc(4000);
    
    HPFmR.setSRandFT(Fs, 1);
    HPFmR.setFc(300);
    LPFmR.setSRandFT(Fs, 2);
    LPFmR.setFc(4000);
    
//    APFmR.setSRandFT(Fs,3);
//    APFmR.setFc(1);
//    APFmL.setSRandFT(Fs,3);
//    APFmL.setFc(1);
    
    APFR.setSRandFT(Fs,3);
    APFR.setFc(1);
    APFL.setSRandFT(Fs,3);
    APFL.setFc(4e3);

    TanH.setSat(Sat1);
    SigMm.setSat(Sat2);
    SigMl.setSat(Sat2);
    
    

}

void HerewegoagainAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HerewegoagainAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HerewegoagainAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    float onePL = 0;
    float twoPL = 0;
    float threePL = 0;
    float onePR = 0;
    float twoPR = 0;
    float threePR = 0;
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            
            float samp = gIn*channelData[sample];
            if (channel == 0)
            {
                LPFL.setFc(Freq1);
                HPFL.setFc(Freq2);
                HPFmL.setFc(Freq1);
                LPFmL.setFc(Freq2);
                APFL.setFc(Freq2);
                
                onePL = LPFL.processSample(APFL.processSample(samp));
                twoPL = HPFmL.processSample(LPFmL.processSample(samp));
                threePL = HPFL.processSample(samp);
                
                
                if (Toggle)
                {
                    SigMl.setSat(Sat1);
                    firstpL = SigMl.SIGMOID(onePL);
                }
                else
                {
                    firstpL = onePL;
                }
                
                 if (Toggle2)
                {
                    SigMm.setSat(Sat2);
                    secondpL = SigMm.SIGMOID(twoPL);
                }
                 else
                 {
                     secondpL = twoPL;
                 }
                
                
                 if (Toggle3)
                {
                    TanH.setSat(Sat3);
                    thirdpL = TanH.TANH(threePL);
                }
                 else
                 {
                     thirdpL = threePL;
                 }
                

                
                float outsampL = firstpL - secondpL + thirdpL;
                channelData[sample] = gOut*(Blnd*outsampL+(1-Blnd)*samp);
                
                
            }
            else if (channel == 1)
            {
                LPFR.setFc(Freq1);
                HPFR.setFc(Freq2);
                HPFmR.setFc(Freq1);
                LPFmR.setFc(Freq2);
                APFR.setFc(Freq2);
                
                onePR = LPFR.processSample(APFR.processSample(samp));
                twoPR = HPFmR.processSample(LPFmR.processSample(samp));
                threePR = HPFR.processSample(samp);
                
                
                if (Toggle)
                {
                    SigMl.setSat(Sat1);
                    firstpR = SigMl.SIGMOID(onePR);
                }
                else
                {
                    firstpR = onePR;
                }
                
                 if (Toggle2)
                {
                    SigMm.setSat(Sat2);
                    secondpR = SigMm.SIGMOID(twoPR);
                }
                 else
                 {
                     secondpR = twoPR;
                 }
                
                
                 if (Toggle3)
                {
                    TanH.setSat(Sat3);
                    thirdpR = TanH.TANH(threePR);
                }
                 else
                 {
                     thirdpR = threePR;
                 }
                float outsampR = firstpR - secondpR + thirdpR;
                channelData[sample] = gOut*(Blnd*outsampR+(1-Blnd)*samp);
            }
                        
           

        }
    }
}

//==============================================================================
bool HerewegoagainAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HerewegoagainAudioProcessor::createEditor()
{
    return new HerewegoagainAudioProcessorEditor (*this);
}

//==============================================================================
void HerewegoagainAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HerewegoagainAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HerewegoagainAudioProcessor();
}

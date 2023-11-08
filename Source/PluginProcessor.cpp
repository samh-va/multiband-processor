/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
MultibandCompressorAudioProcessor::MultibandCompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
}

MultibandCompressorAudioProcessor::~MultibandCompressorAudioProcessor()
{
}

//==============================================================================
const juce::String MultibandCompressorAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool MultibandCompressorAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool MultibandCompressorAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool MultibandCompressorAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double MultibandCompressorAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int MultibandCompressorAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int MultibandCompressorAudioProcessor::getCurrentProgram()
{
	return 0;
}

void MultibandCompressorAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String MultibandCompressorAudioProcessor::getProgramName(int index)
{
	return {};
}

void MultibandCompressorAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void MultibandCompressorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    Fs = AudioProcessor::getSampleRate();
    buffSize = AudioProcessor::getBlockSize();
    setLatencySamples(buffSize);
 

    int buffPDFsize = 40;

    
// COMPRESORES ---------------------------------------------------------------------------------------------
    
    CompL_L.setTHandR(Tll, Rll,Fs);
    CompHL_L.setTHandR(Tml, Rml,Fs);
    CompH_L.setTHandR(Thl, Rhl,Fs);

    
    CompL_R.setTHandR(Tll, Rll,Fs); // POR AHORA SE DEJAN LOS THR Y RATIO DEL LADO L. La idea es cambiar de estados entre esos TH y RT
    CompHL_R.setTHandR(Tml, Rml,Fs);
    CompH_R.setTHandR(Thl, Rhl,Fs);
 
    
// LIMITADORES ---------------------------------------------------------------------------------------------
    
    LimitL.setLimiter(-0.1, 0.05e-4, 0.05e-4, Fs);
    LimitR.setLimiter(-0.1, 0.05e-4, 0.05e-4, Fs);

    

// BUFFER CIRCULARES --------------------------------------------------------------------------------------

    buffCirL_L.setBuffSize(buffSize);
    buffCirHL_L.setBuffSize(buffSize);
    buffCirH_L.setBuffSize(buffSize);
    
    buffCirL_R.setBuffSize(buffSize);
    buffCirHL_R.setBuffSize(buffSize);
    buffCirH_R.setBuffSize(buffSize);
    
    
    buffpdfL_L.setbuffSizePDF(buffPDFsize);
    buffpdfHL_L.setbuffSizePDF(buffPDFsize);
    buffpdfH_L.setbuffSizePDF(buffPDFsize);
    
    buffpdfL_R.setbuffSizePDF(buffPDFsize);
    buffpdfHL_R.setbuffSizePDF(buffPDFsize);
    buffpdfH_R.setbuffSizePDF(buffPDFsize);
    
    
//  FILTROS-------------------------------------------------------------------------------------------------
   
    LPFR.setSRandFT(Fs, 2);
    LPFR.setFc(300);
    
    LPFL.setSRandFT(Fs, 2);
    LPFL.setFc(300);
    
// -------------------------
    HPFR.setSRandFT(Fs, 1);
    HPFR.setFc(4e3);
    
    HPFL.setSRandFT(Fs, 1);
    HPFL.setFc(4e3);
    
// -------------------------
    HPFmL.setSRandFT(Fs, 1);
    HPFmL.setFc(300);
    
    LPFmL.setSRandFT(Fs, 2);
    LPFmL.setFc(4e3);
    
    HPFmR.setSRandFT(Fs, 1);
    HPFmR.setFc(300);
   
    LPFmR.setSRandFT(Fs, 2);
    LPFmR.setFc(4e3);
    
// --------------------------
    APFR.setSRandFT(Fs, 3);
    APFR.setFc(4e3);
    
    APFL.setSRandFT(Fs, 3);
    APFL.setFc(4e3);
    
}

void MultibandCompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MultibandCompressorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
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

void MultibandCompressorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            float inSamp = channelData[sample];

            if (channel == 0)
            {
                float firstPL = LPFL.processSample(APFL.processSample(inSamp));
                float secondPL = HPFmL.processSample(LPFmL.processSample(inSamp));
                float thirdPL = HPFL.processSample(inSamp);
                
                buffCirL_L.setSample(firstPL);
                auto deltaL = buffCirL_L.getdeltaRMS();
                if(buffCirL_L.getupdateRMS())
                {
                    auto rmsL = buffCirL_L.getRMS();
                    buffpdfL_L.setPDF(rmsL);
                    if (buffpdfL_L.getFlag())
                    {
                        auto meanL = buffpdfL_L.getMedia();
                        CompL_L.calculateMakeUp(meanL);
                    }
                }
                
                buffCirHL_L.setSample(secondPL);
                auto deltaHL = buffCirHL_L.getdeltaRMS();
                if(buffCirHL_L.getupdateRMS())
                {
                    auto rmsHL = buffCirHL_L.getRMS();
                    buffpdfHL_L.setPDF(rmsHL);
                    if (buffpdfHL_L.getFlag())
                    {
                        auto meanHL = buffpdfHL_L.getMedia();
                        CompHL_L.calculateMakeUp(meanHL);
                    }
                }
                //
                buffCirH_L.setSample(thirdPL);
                auto deltaH = buffCirH_L.getdeltaRMS();
                if(buffCirH_L.getupdateRMS())
                {
                    auto rmsH = buffCirH_L.getRMS();
                    buffpdfH_L.setPDF(rmsH);
                    if (buffpdfH_L.getFlag())
                    {
                        auto meanH = buffpdfH_L.getMedia();
                        CompH_L.calculateMakeUp(meanH);
                    }
                }
                
                CompL_L.setAutoM(AutoMGLow);
                CompHL_L.setAutoM(AutoMGMid);
                CompH_L.setAutoM(AutoMGHigh);
                
                CompL_L.setARtmax(TaMaxLow, TrMaxLow);
                CompHL_L.setARtmax(TaMaxMid, TrMaxMid);
                CompH_L.setARtmax(TaMaxMid, TrMaxMid);
                
                auto outL = CompL_L.Compressing(buffCirL_L.getSample(), deltaL);
                auto outHL = CompHL_L.Compressing(buffCirHL_L.getSample(), deltaHL);
                auto outH = CompH_L.Compressing(buffCirH_L.getSample(), deltaH);
                
                
                if (CBvalueL == 1)
                {
                    value_low = juce::Decibels::gainToDecibels(abs(firstPL));
                }
                else if (CBvalueL == 2)
                {
                    value_low = juce::Decibels::gainToDecibels(abs(1-CompL_L.c));
                }
                else if (CBvalueL == 3)
                {
                    value_low = juce::Decibels::gainToDecibels(abs(outL));
                }
                
                if (CBvalueM == 1)
                {
                    value_mid = juce::Decibels::gainToDecibels(abs(secondPL));
                }
                else if (CBvalueM == 2)
                {
                    value_mid = juce::Decibels::gainToDecibels(abs(1-CompHL_L.c));
                }
                else if (CBvalueM == 3)
                {
                    value_mid = juce::Decibels::gainToDecibels(abs(outHL));
                }
                
                if (CBvalueH == 1)
                {
                    value_high = juce::Decibels::gainToDecibels(abs(thirdPL));
                }
                else if (CBvalueH == 2)
                {
                    value_high = juce::Decibels::gainToDecibels(abs(1-CompH_L.c));
                }
                else if (CBvalueH == 3)
                {
                    value_high = juce::Decibels::gainToDecibels(abs(outH));
                }
                
                channelData[sample] = LimitL.Limiting(outL-outHL+outH);
                
               
   
            }

            else if (channel == 1)
            {
                float firstPR = LPFR.processSample(APFR.processSample(inSamp));
                float secondPR = HPFmR.processSample(LPFmR.processSample(inSamp));
                float thirdPR = HPFR.processSample(inSamp);
                
                buffCirL_R.setSample(firstPR);
                auto deltaL = buffCirL_R.getdeltaRMS();
                if(buffCirL_R.getupdateRMS())
                {
                    auto rmsL = buffCirL_R.getRMS();
                    buffpdfL_R.setPDF(rmsL);
                    if (buffpdfL_R.getFlag())
                    {
                        auto meanL = buffpdfL_R.getMedia();
                        CompL_R.calculateMakeUp(meanL);
                    }
                }

                buffCirHL_R.setSample(secondPR);
                auto deltaHL = buffCirHL_R.getdeltaRMS();
                if(buffCirHL_R.getupdateRMS())
                {
                    auto rmsHL = buffCirHL_R.getRMS();
                    buffpdfHL_R.setPDF(rmsHL);
                    if (buffpdfHL_R.getFlag())
                    {
                        auto meanHL = buffpdfHL_R.getMedia();
                        CompHL_R.calculateMakeUp(meanHL);
                    }
                }
//
                buffCirH_R.setSample(thirdPR);
                auto deltaH = buffCirH_R.getdeltaRMS();
                if(buffCirH_R.getupdateRMS())
                {
                    auto rmsH = buffCirH_R.getRMS();
                    buffpdfH_R.setPDF(rmsH);
                    if (buffpdfH_R.getFlag())
                    {
                        auto meanH = buffpdfH_R.getMedia();
                        CompH_R.calculateMakeUp(meanH);
                    }
                }

                CompL_R.setAutoM(AutoMGLow);
                CompHL_R.setAutoM(AutoMGMid);
                CompH_R.setAutoM(AutoMGHigh);
                
                CompL_R.setARtmax(TaMaxLow, TrMaxLow);
                CompHL_R.setARtmax(TaMaxMid, TrMaxMid);
                CompH_R.setARtmax(TaMaxHigh, TrMaxHigh);
                
                auto outL = CompL_R.Compressing(buffCirL_R.getSample(), deltaL);
                auto outHL = CompHL_R.Compressing(buffCirHL_R.getSample(), deltaHL);
                auto outH = CompH_R.Compressing(buffCirH_R.getSample(), deltaH);

                if (CBvalueL == 1)
                {
                    value_lowR = juce::Decibels::gainToDecibels(abs(firstPR));
                }
                else if (CBvalueL == 2)
                {
                    value_lowR = juce::Decibels::gainToDecibels(abs(1-CompL_R.c));
                }
                else if (CBvalueL == 3)
                {
                    value_lowR = juce::Decibels::gainToDecibels(abs(outL));
                }
                
                if (CBvalueM == 1)
                {
                    value_midR = juce::Decibels::gainToDecibels(abs(secondPR));
                }
                else if (CBvalueM == 2)
                {
                    value_midR = juce::Decibels::gainToDecibels(abs(1-CompHL_R.c));
                }
                else if (CBvalueM == 3)
                {
                    value_midR = juce::Decibels::gainToDecibels(abs(outHL));
                }
                
                if (CBvalueH == 1)
                {
                    value_highR = juce::Decibels::gainToDecibels(abs(thirdPR));
                }
                else if (CBvalueH == 2)
                {
                    value_highR = juce::Decibels::gainToDecibels(abs(1-CompH_R.c));
                }
                else if (CBvalueH == 3)
                {
                    value_highR = juce::Decibels::gainToDecibels(abs(outH));
                }
                channelData[sample] = LimitR.Limiting(outL-outHL+outH);
            }




        }
    
	
	}
}

//==============================================================================
bool MultibandCompressorAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MultibandCompressorAudioProcessor::createEditor()
{
	return new MultibandCompressorAudioProcessorEditor(*this);
}

//==============================================================================
void MultibandCompressorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void MultibandCompressorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MultibandCompressorAudioProcessor();
}

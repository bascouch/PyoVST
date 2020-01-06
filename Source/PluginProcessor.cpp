/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PyoJuceAudioProcessor::PyoJuceAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::octagonal(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::octagonal(), true)
                     #endif
                       )
, parameters (*this, nullptr, "PyoJuce", createParameterLayout())
#endif
{
    parameters.addParameterListener ("P1", this);
    parameters.addParameterListener ("P2", this);
    parameters.addParameterListener ("P3", this);
    parameters.addParameterListener ("P4", this);
    
    code_content = String(BinaryData::init_py);
}

PyoJuceAudioProcessor::~PyoJuceAudioProcessor()
{
}

//==============================================================================
const String PyoJuceAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PyoJuceAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PyoJuceAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PyoJuceAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PyoJuceAudioProcessor::getTailLengthSeconds() const
{
    return 10.;
}

int PyoJuceAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PyoJuceAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PyoJuceAudioProcessor::setCurrentProgram (int index)
{
}

const String PyoJuceAudioProcessor::getProgramName (int index)
{
    return {};
}

void PyoJuceAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PyoJuceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // assign static interpreter out
    
    //c_stdredirect =  &(this->getInterpreterOuptut);
    
    pyo.setup(getTotalNumOutputChannels(), samplesPerBlock, sampleRate);
    execCode();
    std::cout << " :::::: prepareToPlay ::::::" << std::endl;
}

void PyoJuceAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    
    std::cout << " :::::: releaseAfterStop ::::::" << std::endl;
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PyoJuceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
//    // This is the place where you check if the layout is supported.
//    // In this template code we only support mono or stereo.
//    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
//     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
//        return false;
//
//    // This checks if the input layout matches the output layout
//   #if ! JucePlugin_IsSynth
//    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
//        return false;
//   #endi]

    return true;
  #endif
}
#endif

void PyoJuceAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    
    AudioPlayHead* playHead = getPlayHead();
    auto sr = this->getSampleRate();
    
    
    
    if( playHead )
    {
        AudioPlayHead::CurrentPositionInfo position;
        playHead->getCurrentPosition (position);
        
        String output_string;
        
        output_string << " samples: " <<position.timeInSamples << " sec: " <<position.timeInSamples/sr << " bpm: " << position.bpm << " ppq: " << position.ppqPosition << " sig: " << position.timeSigDenominator << "/" << position.timeSigNumerator ;
        
        printf("%s\n",output_string.toRawUTF8());
        
        PyoJuceAudioProcessorEditor* editor = (PyoJuceAudioProcessorEditor*) getActiveEditor();
        if(editor != nullptr)
        {
            editor->setLabel(output_string);

            editor->appendOutput(PyoJuceAudioProcessor::output_content);
            PyoJuceAudioProcessor::output_content.clear();
            editor->postRepaint();
        }
    }
    
    // Process Midi Messages
    MidiBuffer::Iterator mid_buffer_iter(midiMessages);
    MidiMessage m(0xf0);
    const uint8 * mraw_data;
    int mraw_data_size;
    int sample;
    
    while(mid_buffer_iter.getNextEvent(m,sample))
    {
        mraw_data = m.getRawData();
        mraw_data_size = m.getRawDataSize();
        
        // only standard MIDI messages, no Sysex
        if(mraw_data_size>2)
            pyo.midi(mraw_data[0], mraw_data[1], mraw_data[2]);
        
    }

    
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
    pyo.process(buffer);
    
    //
    
}

//==============================================================================
bool PyoJuceAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PyoJuceAudioProcessor::createEditor()
{
    return new PyoJuceAudioProcessorEditor (*this);
}

//==============================================================================
void PyoJuceAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = parameters.copyState();
    state.setProperty ("code", var(code_content), nullptr);
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);

}

void PyoJuceAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    PyoJuceAudioProcessorEditor* editor = (PyoJuceAudioProcessorEditor*) getActiveEditor();
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
        {
            parameters.replaceState (ValueTree::fromXml (*xmlState));
            if (parameters.state.hasProperty ("code"))
            {
                code_content = parameters.state.getProperty ("code", var (String(BinaryData::init_py)));
            }
        }
    // unused normally
    if( code_content.isEmpty())
        code_content = String(BinaryData::init_py);
    
    if(editor)
        editor->setContent(code_content);
    
    execCode();
}

//==============================================================================
AudioProcessorValueTreeState::ParameterLayout PyoJuceAudioProcessor::createParameterLayout()
{
    // add your audio parameters here
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    
    params.push_back (std::make_unique<AudioProcessorValueTreeState::Parameter> ("P1", "P1", "",
                                                                                 NormalisableRange<float> (.0f, 1.0f, 0.00001f), 0.f,
                                                                                 [](float value) { return String(value, 4); }, nullptr));
    params.push_back (std::make_unique<AudioProcessorValueTreeState::Parameter> ("P2", "P2", "",
                                                                                 NormalisableRange<float> (.0f, 1.0f, 0.00001f), 0.f,
                                                                                 [](float value) { return String(value, 4); }, nullptr));
    params.push_back (std::make_unique<AudioProcessorValueTreeState::Parameter> ("P3", "P3", "",
                                                                                 NormalisableRange<float> (.0f, 1.0f, 0.00001f), 0.f,
                                                                                 [](float value) { return String(value, 4); }, nullptr));
    params.push_back (std::make_unique<AudioProcessorValueTreeState::Parameter> ("P4", "P4", "",
                                                                                 NormalisableRange<float> (.0f, 1.0f, 0.00001f), 0.f,
                                                                                 [](float value) { return String(value, 4); }, nullptr));
    
    
    return { params.begin(), params.end() };
}

//==============================================================================
// AudioProcessorValueTreeState::Listener interface
void PyoJuceAudioProcessor::parameterChanged (const String &parameterID, float newValue)
{
    // TODO : implement
    
    if( parameterID == "P1" || parameterID == "P2" || parameterID == "P3" || parameterID == "P4" )
    {
        pyo.value(parameterID, newValue);
    }

    
}

String PyoJuceAudioProcessor::getCode()
{
    return code_content;
}

void PyoJuceAudioProcessor::setCode(String content)
{
    code_content = content;
}

int PyoJuceAudioProcessor::execCode()
{
    pyo.clear();
    return pyo.exec(code_content);
}

int PyoJuceAudioProcessor::getInterpreterOutput(const char * content)
{
    PyoJuceAudioProcessor::output_content.append(String(content),1024);
}

int PyoJuceAudioProcessor::updateOutput()
{
    PyoJuceAudioProcessorEditor* editor = (PyoJuceAudioProcessorEditor*) getActiveEditor();
    editor->appendOutput(PyoJuceAudioProcessor::output_content);
    PyoJuceAudioProcessor::output_content.clear();
    
}

String PyoJuceAudioProcessor::output_content;

int getInterpreterOutput(const char * s)
{
    PyoJuceAudioProcessor::getInterpreterOutput(s);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PyoJuceAudioProcessor();
}

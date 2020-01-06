/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PyoJuceAudioProcessorEditor  : public AudioProcessorEditor,
                                    private CodeDocument::Listener,
                                    private Timer
{
public:
    PyoJuceAudioProcessorEditor (PyoJuceAudioProcessor&);
    ~PyoJuceAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void postRepaint();
    
    void update();
    void setContent(String content);
    void appendOutput(String content);
    
    void setLabel(String content);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PyoJuceAudioProcessor& processor;
    
    CodeDocument codeDocument;
    std::unique_ptr<CodeEditorComponent> editor;
    TextEditor outputDisplay;
    Label bottom_label { "hint", "test" };
    String time_text;
    String output_buffer;
    Boolean update_request = true;
    
    void codeDocumentTextInserted (const String&, int) override    { if(update_request) startTimer (300); }
    void codeDocumentTextDeleted (int, int) override               { if(update_request) startTimer (300); }
    
    void timerCallback() override
    {
        stopTimer();
        update();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PyoJuceAudioProcessorEditor)
};

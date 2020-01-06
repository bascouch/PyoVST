/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PyoJuceAudioProcessorEditor::PyoJuceAudioProcessorEditor (PyoJuceAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setOpaque (true);
    
    editor.reset (new CodeEditorComponent (codeDocument, nullptr));
    addAndMakeVisible (editor.get());
    editor->setFont ({ Font::getDefaultMonospacedFontName(), 14.0f, Font::plain });
    editor->setTabSize (4, true);
    
    outputDisplay.setMultiLine (true);
    outputDisplay.setReadOnly (true);
    outputDisplay.setCaretVisible (false);
    outputDisplay.setFont ({ Font::getDefaultMonospacedFontName(), 14.0f, Font::plain });
    addAndMakeVisible (outputDisplay);
    
    codeDocument.addListener (this);
    addAndMakeVisible(bottom_label);
    
    update_request = false;
    editor->loadContent ( p.getCode() );
    update_request = true;
    
    setSize (600, 460);
}

PyoJuceAudioProcessorEditor::~PyoJuceAudioProcessorEditor()
{
}

void PyoJuceAudioProcessorEditor::setContent(String content)
{
    editor->loadContent (content);
}

void PyoJuceAudioProcessorEditor::appendOutput(String content)
{
    output_buffer.append(content,1024);
    
}

void PyoJuceAudioProcessorEditor::setLabel(String content)
{
    time_text = content;
}


void PyoJuceAudioProcessorEditor::update()
{
    outputDisplay.clear();
    
    auto startTime = Time::getMillisecondCounterHiRes();
    
    char *msg;
    
    processor.setCode(codeDocument.getAllContent());
    auto error = processor.execCode();
    
    auto elapsedMs = Time::getMillisecondCounterHiRes() - startTime;

    if (error)
        outputDisplay.insertTextAtCaret (String(msg));
    else
        outputDisplay.insertTextAtCaret ("\n(Execution time: " + String (elapsedMs, 2) + " milliseconds)");
}
//==============================================================================
void PyoJuceAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    bottom_label.setText(time_text, NotificationType::dontSendNotification);
    if(output_buffer.isNotEmpty())
    {
        outputDisplay.insertTextAtCaret(output_buffer);
        output_buffer.clear();
    }
    

}

void PyoJuceAudioProcessorEditor::postRepaint()
{
    Component::SafePointer<PyoJuceAudioProcessorEditor> editor(this);
    Timer::callAfterDelay(30, [editor, this]()
                          {
                              if (editor != nullptr)
                              {
                                  repaint();
                              };
                          });
}

void PyoJuceAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto r = getLocalBounds().reduced (8);
    
    editor->setBounds       (r.removeFromTop (proportionOfHeight (0.6f)));
    bottom_label.setBounds (r.removeFromBottom(20));
    outputDisplay.setBounds (r.withTrimmedTop (8));
    
    
}

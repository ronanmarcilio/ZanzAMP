cmake_minimum_required(VERSION 3.15)

project(ZanzAMP VERSION 1.0.0)

# Add JUCE
# Nota: O usuário deve ter o JUCE baixado e apontar para ele
add_subdirectory(JUCE)

juce_add_plugin(ZanzAMP
    COMPANY_NAME "ZanzAudio"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Zanz
    PLUGIN_CODE Zamp
    FORMATS VST3
    PRODUCT_NAME "ZanzAMP")

target_sources(ZanzAMP PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h)

target_compile_definitions(ZanzAMP PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(ZanzAMP PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_audio_formats
    juce::juce_audio_devices
    juce::juce_audio_basics
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_dsp
    juce::juce_data_structures
    juce::juce_events
    juce::juce_core)

# Habilitar C++17 para o JUCE
set_target_properties(ZanzAMP PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)

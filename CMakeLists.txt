CMAKE_MINIMUM_REQUIRED (VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)
set(ATE_VERSION 1.0.0)
set(MANUFACTOR_ID DKDV)
set(DEV_NAME Devekuşu Devices)
set(PLUGIN_NAME "DelayPlugin")
set(PLUGIN_ID DLPG)

project (DELAYPLUGIN VERSION 1.0.0 DESCRIPTION "Atesh project")

ADD_SUBDIRECTORY (JUCE)

set (JUCE_ENABLE_MODULE_SOURCE_GROUPS ON CACHE BOOL "" FORCE)

juce_add_plugin(${PROJECT_NAME}
        COMPANY_NAME ${DEV_NAME}
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS TRUE
        JUCE_VST3_CAN_REPLACE_VST2 FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE ${MANUFACTOR_ID}
        PLUGIN_CODE ${PLUGIN_ID}
        FORMATS VST3 AU
        PRODUCT_NAME ${PLUGIN_NAME}
)

target_sources (${PROJECT_NAME}  PRIVATE Source/PluginProcessor.h
        Source/PluginEditor.h
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp)

# These are some toggleable options from the JUCE CMake API
target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries (${PROJECT_NAME} PRIVATE
        DELAY_CODE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_core
        juce::juce_data_structures
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_audio_plugin_client
        juce::juce_audio_formats
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
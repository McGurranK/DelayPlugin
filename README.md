# Another Delay Plugin?

Yes, yet another delay plugin — rebuilt using JUCE DSP to revamp an old project and introduce some eye-catching visualizations.

## Current Progress

- ✅ Custom visualizations implemented
- ✅ Initial GUI setup complete
- ⬜ Buffering code unit testing pending
- ⬜ Accessibility improvements for the UI
- ⬜ Comprehensive UI visual redesign

## Building Instructions

### Using Projucer

1. Build JUCE’s Projucer from the `extras` folder.
2. Open `DelayPlugin.jucer` in Projucer.
3. Export and open the project in Visual Studio (Windows) or Xcode (macOS).
4. Build a release version of the plugin.

### Using CMake

1. Clone the repository with all submodules initialized (`git clone --recurse-submodules`).
2. Open a terminal in the project directory.
3. Run `cmake -B Build` to generate the build files.
4. Build the project using your preferred build system (e.g., `make` or open the generated IDE project).

## Roadmap

- Refine and polish the UI for a modern, attractive look
- Implement full accessibility support for the interface
- Complete unit testing on buffering and core DSP components  

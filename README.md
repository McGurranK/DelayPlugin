# Not another delay plugin

I agree, everyone and there dad has a delay plugin these days... but I wanted to fix a very old and to be honest BAD juce project into just something simple and straight forward and have a look at writing some state loading/ custom UI code. To be honest, it has been thrown together and there area some issues that I will ammend when I get time.

# What's makes this different from other delays?

Nothing, just an excercise in using juce and a little bit of a playground for messing with delays.

# Accessibility

I will look into adding it simply when I finalise this plugin.

# Building

- Based on JUCE with a JUCER build setup and JUCE as a submodule in the repo.
- Open in either Visual Studio or Xcode (I've built in XCode, so let me know if you have any issues).
- Build the plugin, which should throw istelf into VST3 folder.
- (MacOs) Ensure you have privledges changed so you are able to copy into your plugin folder.
- Ps, Any issues let me know.

# Why no Installers?

It's something I could do in the future, but I would want to do it the right way and have it signed. I don't trust companies that don't sign their binaries, and it can be a massive headache dealing with both OS signing. Also, I deal with installers in my day job and depending on how custom you want to go it can be a heavy chunk of work. 

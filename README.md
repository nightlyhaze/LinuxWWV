Linux WWV Simulator
==================

Recreates the WWV Shortwave audio broadcast time signal. This syncronizes the Pi audio to a local Real-Time Clock (RTC) playing the correct time like WWV.

This is my first actual project here on Github, and it is a fork of @rgrokett's RaspiWWV. For this, I stripped out some components of the project such as the OLED clock, and only took the "audio clock" section of the project. Instead of using Python as the main code, I used C++ for this purpose, just out of experimentation (and yes, I used ChatGPT as my helping tool for this, such as translating the original Python code to C++), replaced and even added some audio segments such as my own "station identification".

ACCURACY:
The Pi's clock is set to the RTC so is as accurate as the RTC module. (Typically, +-1 second or so per day). For more accuracy, have the device actively synced to an NTP server. Additionally, the player itself may introduce some latency; this is why its first line after the #define lines has a compensation constant you can adjust as necessary:
`constexpr int AUDIO_LATENCY_MS = <number in milliseconds>;`
This also serves as the millisecond integer for the sleep() function that was in the original Python code.

Moreover, depending on the system load, the program may lag (or even malfunction) if you use your Linux machine aside from this program, so it's best to use this as some sort of "idle mode music", or, as the original program intends to, play this on a small single-board computer (SBC) such as a Raspberry Pi Zero or the likes of it.

Also Note: It can take up to two minutes before audio starts (for syncronization).

I might make some other projects out of this one at some point in time with some additions, such as adding a 100 Hz digital time code as that of an actual WWV broadcast. Also, feel free to contribute or even fork this project for improvement and further experimentation.

Credits to sir R. Grokett for the RaspiWWV project.

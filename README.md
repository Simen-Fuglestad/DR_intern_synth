# DR_intern_synth

### Welcome to the Data Repons STM32 Digital Synthesizer Project!

This project is an attempt at making a digital synth on a low-cost development board.

It currently sports the following features:
* Wavetable synthesis with sine, square, triangle and saw as basics
* Vibrato via FM
* Phase modulation
* ADSR envelope for both volume and modulation depth
* Freely selectable wavetable selections for both vibrato and PM
* Phase modulation depth sync
* Low pass resonant filter with adjustable sweep frequency
* Pulse width control

Check out this [video](https://youtu.be/JR3Jq1qJsE0) for a demo.

# Table of Contents
1. [Introduction: What's a synth?](#introduction-whats-a-synth?)
2. [Requirements and Prequisites](#requirements-and-prerequisites)
3. [Getting Started](#getting-started)
    *[A note on safety](#a-note-on-safety)
    *[Setting up](#setting-up)
4. [How it works](#how-it-works)
    * [System Overview](#sytem-overview)
    * [Implementation Details: The Nitty Gritties](#implementation-details-the-nitty-gritties)
6. [Proposed Improvals](#proposed-improvals)
7. [List of words](#list-of-words)
    * [Technical](#technical)
    * [Musical](#musical) 
8. [Further Reading](#further-reading)

## Introduction: What's a synth?
The term "synth" is short for synthesizer and is often used to describe musical instruments that produce sound electronically. Audio waves are generated using analog circuits and/or digital computers and then fed to a speaker element, often making noises you would be hard pressed to recreate on an acoustic instrument. That's the gist of it anyway. Check out the [Futher Reading](#further-reading) section if you want to learn more. 

## Requirements and Prerequisites
This project was made using [this](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) devkit for the STM32F07G. While it would be possible to get it up and running on another devkit, it is much less work to get started if you have one of these at hand(the Bergen office should have plenty).

If you simply want to try the synth out, then no prerequisites are required: Just follow the setup guide step by step. If you want to work on this thing, some preliminary knowlegde about C programming, basic signal processing and music is nice to have but not strictly required. I certainly learned a lot as I went along.

## Getting Started
This section deals with some important safety aspects and then how to proceed setting up the synth.

### A note on safety
**Please read and make sure you understand the following safety points before proceeding**:

* **Always listen on low volume**
* **If you make any alterations to the source code, always try to play a note with headphones and speakers disconnected**. Output should be measured via an oscilloscope to make sure it is within safe levels before connecting any listening devies. **Typical output should be less than 15mV**, listening to louder values can cause pain and potential damage to your hearing.
* Unless you know exactly what you are doing, **do not attempt to alter the volume levels of the DAC.**
* **Do not attempt to increase the audio sample size** (should be 12bit or less), this will cause audio spikes and clipping to potentially dangerous levels.

[Hearing loss can become permananet](https://www.cdc.gov/nceh/hearing_loss/how_does_loud_noise_cause_hearing_loss.html), **stay safe and keep loud noises away from your ears!**

### Setting Up

To set this thing up we are going to need a few things.

1. The [STM32Cube IDE](https://www.st.com/en/development-tools/stm32cubeide.html).
2. A devkit, preferably [this one](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) with a Mini-USB cable for programming/debugging.
3. Some prototyping components that lets us interact with the synth (all of these should be availble in the lab at the Bergen office).
    * 15 3-pin 10k potentiometers, either knobs or sliders
    * 5 click switches (buttons)
    * A whole bunch of jumper wires
    * As many breadboards as you need (I've used two long ones)
    * 5 Resistors, anything in the 1k-10k range should be fine
    * A MIDI keyboard, e.g like the [Korg MicroKey 25](#https://www.4sound.no/korg-microkey-25-usbmidi-ctr-2)
    * A micro-USB to USB-B cable. These are not very common, but you can make your own by stitching together two cables. An alternative would be to get an adapter, though this has not been tested
    * A headset with good 'ol fashinoed audio jack, either TRS or TRRS (or a loudspeaker if you have nice sound-proof office).

Once you have the IDE installed and the components neatly organized on your desk, do the following:
1. Clone or Download this repository
2. Open the STM32CubeIDE
3. Select *File* and then *Open Projects From File System*, then find the button labeled *Directory*.
4. Navigate to where you cloned/downloaded the project, then open the folder named *stm32_synth*.The project should now appear in your project explorer on the left.
5. Open the *DR_intern_synth.ioc* file by double-clicking it from the project explorer.
6. You should now be looking at a pin view of the MCU. On the devkit, all these pins have correspodning breakout pins which are labeled.
7. Connect your buttons and potmeters (aka slider and/or knobs) to the relevant pins. Note that everything labeled as "BTN_XXX" should be connected to push buttons while everything else should be connected to 10k sliders. For order's sake, here is the full list of the pins with a desctiption of what they do and if they need a 10k potmeter or button.


For usage with 10k potmeters
* PA1 -> Filter cutoff frequency    
* PA2 -> Filter sweep frequency     
* PA3 -> Gain (aka volume)          
* PA5 -> Phase shift                
* PA6 -> Envelope attack            
* PA7 -> Envelope decay            
* PB0 -> Envelope sustain           
* PB1 -> Envelope release           
* PC1 -> Vibrato depth              
* PC2 -> Pulse Width                
* PC3 -> Vibrato freuency           
* PC4 -> Modulation depth scaler    
* PC5 -> Modulation sync scaler

For usage with buttons
* PD10 -> Moduluation sync enable 
* PD12 -> Mod wavetable cycle       
* PD13 -> Vibrato wavetable cycle   
* PD14 -> Output wavetable cycle    

These pins are labeled but not in use
* PD11 -> Third oscillator cycle (button)
* PE11 -> GPIO for measurement, useful for testing with oscilloscope


Note that all mixer controls should be biased with VDD from the devkit. Below is a photo showing an example of how to wire the whole mixer on two large breadboards.


8. Connect your micro-usb to to usb-B cable. Chances are you don't have anything like this lying around, in which cas you can make one by cutting open to spearate cables and fixing them together. I used wago clips, but soldering is also an option.

9. Connect a mini-usb cable to the progarmmer port on the devkit, it should show up as a COM device on windows or as dev/ttyUSB in linux.

10. In the IDE, press either run or debug and you are good to go!

## How It Works
Here we will take a look at how the synth actually works, starting from a birdseye design view and eventually getting into some nitty gritties. This section is quite long, so feel free to skip to [Setting Up](#setting-up) if you want to get started right away.

Before we begin, let's get a few things cleared up:
* This synth is entirely digital: Every step of the actual *audio processing* is handled by the MCU. If you are an analog-only evangelist, this project is not for you (sorry).
* This is only <ins>one of many ways</ins> to build a synth. For other approaches and ideas, see [Further Reading](#further-reading).


### Sytem Overview
Check out this drawing to get a high-level understanding of the system:
![synth_system drawio(3)](https://user-images.githubusercontent.com/95622775/228760875-0077967c-4c74-497c-b0b4-3241bf20b27c.png)

We can summarize this as follows:
1. A budding musician presses a key on the MIDI keyboard
2. The keyboard transmits USB data containing MIDI information
3. The USB firmware driver picks up the data and passes it to the application
4. The application converts the MIDI data to tone data, which in our case is a stepping index (more on this later)
5. Our newly acquired stepping index is then added to a tracker, which is used to perform a lookup in a wavetable (more on this later also)
6. Next, data is read from our mixer to apply modulation, effects, envelopes, filter and volume controls.
7. The final sample is then put into a DMA buffer 
8. When the buffer is full, it is forwarded to the DAC via I2S
9. The DAC produces electronic signals for a speaker or headphone element to use

If you are unfamiliar with some of the jargon used here, check out the [List of Words](#list-of-words) section.

### Wavetables and Oscillation
The basic principle of all audio is alternating air pressure. When these alterations happens according to certain patterns, we humans can perceive them as tones or even music. To make our synth produce similar alterations at a controlled rate, we use something called a wavetable. This is essentially just a precomputed sample of a periodic wave that we store in working memory of our microcontroller. Playing back this sample at varying speeds will vary the pitch and tones we percieve accordingly, so we use a number to keep track of where we are in a current sample and increment it at different rates to get different sounds. I've elected to call this number a "stepping index" since it defines how far we are stepping ahead in the wavetable per sample we give to our output. Getting a correct magnitude for these steps will depend on the size of the data buffer we transmit to our output, the size of the wavetable, and the samplerate (how often we grab a sample from our wavetable and send it to our output). All these things are configurable in the source code, but care should be taken to prevent audio clipping and other undesirable side-effects.

## Proposed Improvals

* Fix the phase shifter, it is currently very clippy
* Add a preset save function (i.e save ADC buffer values in FLASH or similar)
* Add modulation for pulse width, current implementation has fixed width only
* Add envelope for the filter, would be way cooler than sweeping modulation

## List of Words
Short and (hopefully) simple explanations for all the jargon. Details are omitted on purpose to avoid making this README a mile long. I personally recommend going on a google-spree if you are more curious or alternatively check out the [Further Reading](#further-reading) section below.


#### Technical
<strong>DMA</strong>
Direct Memory access, a technique that allows a separate controller to access memeory data without loading the CPU. Useful in our case to send data to our DAC without using precious processing power.

#### Musical
<strong>MIDI</strong>
Musical Instrument Digital Interface, the most common standard that defines how musical gear and electronic instruments should communicate.
<strong>Wavetable</strong>
A predefined sample buffer that contains one period of a particular wave.
<strong>Modulation</strong>
Varying one periodic signal using another. In synths, this is commonly used to make sounds more interesting and complex.
<strong>Envelope</strong>
A process that alters a level of a signal according to a pattern over time. For example, an envelope can define our volume to rise slowly for a while and then suddenly go quiet very quickly.
<strong>PWM</strong>
Pulse Width Modulation: Varying the duty cycle of digital signal according to a modualtion source. While mostly a techincal term, it is more useful to consider it as a muscial effect for our purposes.

## Further Reading

* Practical overview of MIDI and note frequencies
https://www.inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies

* MIDI message format specification
https://www.songstuff.com/recording/article/midi_message_format/

* Very small design guide for resonant filter
https://www.music.mcgill.ca/~gary/618/week1/node13.html#fig:resonance

* Long, but very good series on synthesizers by Sound on Sound magazine
https://www.soundonsound.com/techniques/whats-sound

* Useful webpage for reading about anything DSP-related
https://www.dsprelated.com/index.php?tk=TwfcU4Rs

* Good explanantion of difference between tone, note and pitch
https://music.stackexchange.com/questions/3262/what-are-the-differences-between-tone-note-and-pitch

* Wiki page on Frequency Modulation
https://en.wikipedia.org/wiki/Frequency_modulation_synthesis

* Wiki page on Phase Modulation
https://en.wikipedia.org/wiki/Phase_modulation

* Wiki page on Synthesizers
https://en.wikipedia.org/wiki/Synthesizer
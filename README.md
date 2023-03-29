# DR_intern_synth

### Welcome to my STM32 Digital Synthesizer Project

This project is an attempt at making a digital synth on a low-cost development board.

[![Video here](https://img.https://youtu.be/JR3Jq1qJsE0/maxresdefault.jpg)](https://youtu.be/JR3Jq1qJsE0)

# Table of Contents
1. [Introduction: What's a synth?](#introduction-whats-a-synth?)
2. [Requirements and Prequisites](#requirements-and-prerequisites)
3. [Setting Up](#setting-up)
4. [How it works](#how-it-works)
5. [Proposed Improvals](#proposed-improvals)
6. [Furhter Reading](#further-reading)

## Introduction: What's a synth?
The term "synth" is short for synthesizer and is often used to describe musical instruments that produce sound electronically. Audio waves are generated using analog circuits and/or digital computers and then fed to a speaker element, often making noises you would hard pressed to recreate on an acoustic instrument. That's the gist of it anyway. Check out the [Futher Reading](#further-reading) section if you want to learn more. 

## Requirements and Prerequisites
This project was made using [this](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) devkit for the STM32F07G. While it would be possible to get it up and running on another devkit, it is much less work to get started if you have one of these at hand(the Bergen office should have plenty).

If you simply want to try the synth out, then no prerequisites are required: Just follow the setup guide step by step. If you want to work on this thing, some preliminary knowlegde about C programming, basic signal processing and music is nice to have but not strictly required. I certainly learned a lot as I went along.

## Setting Up
Now, to set this thing up we are going to need a few things.

1. The [STM32Cube IDE](https://www.st.com/en/development-tools/stm32cubeide.html).
2. A devkit, preferably [this one](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) with a Mini-USB cable for programming/debugging.
3. Some prototyping components that lets us interact with the synth (all of these should be availble in the lab at the Bergen office).
    * 15 potentiometers, either knobs or sliders
    * 5 click switches
    * A whole bunch of jumper wires
    * As many breadboards as you need (I've used two long ones)
    * 5 Resistors, anything in the 1k-10k range should be fine
    * A MIDI keyboard, e.g like the [Korg MicroKey 25](#https://www.4sound.no/korg-microkey-25-usbmidi-ctr-2)
    * A micro-USB to USB-B cable. These are not very common, but you can make your own by stitching together two cables. An alternative would be to get an adapter, though this has not been tested
    * A headset with good 'ol fashinoed audio jack, either TRS or TRRS (or a loudspeaker if you have nice sound-proof office).

## How It Works
Here we will take a look at how the synth actually works, starting from a birdseye design view and eventually getting into some nitty gritties. This section is quite long, so feel free to skip to [Setting Up](#setting-up) if you want to get started right away.

Before we begin, let's get a few things cleared up:
* This synth is entirely digital: Every step of the actual *audio processing* is handled by the MCU. If you are an analog-only evangelist, this project is not for you (sorry).
* This is only <ins>one of many ways</ins> to build a synth. For other approaches and ideas, see [Further Reading](#further-reading).

Ok, here we go.

### Sytem Overview
Check out this drawing to get a high-level grip:
![synth_system drawio](https://user-images.githubusercontent.com/95622775/228551135-a366a016-0961-4fec-97c9-755e227247ee.png)

## Proposed Improvals

## Further Reading

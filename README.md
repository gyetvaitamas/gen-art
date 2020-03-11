# gen-art
Making computer art with generative algorithms
###### This is a re-thinking of [dvalim/art-automata](https://github.com/dvalim/art-automata) _cell_ project.

## This project was built with
- [Visual Studio 2017](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=Community&rel=15)
- [openFrameworks v0.11.0](https://openframeworks.cc/versions/v0.11.0/of_v0.11.0_vs2017_release.zip)
- [K-Lite Codec Pack - Version 12.1.0 Full April 18th 2016](http://filehippo.com/download_klite_codec_pack/67445/)
- Windows 10 Pro (64-bit 1909)

## Purpose of re-thinking
In the original idea, the program generates almost everything based on random numbers, which results some very nice or very bad images. Sometimes I got really fantastic samples with good random numbers and I wondered how can I reproduce again the image. In the beginning I just sent out the values to the console, later I needed a panel where I can set up manually and test continuously the result.

## Installation
Please read the very detailed installation instruction on the official [openFrameworks website](https://openframeworks.cc/download/) for your OS. However, if you develop in VS 2017, I suggest you to use solution platform __x64__, otherwise you should download and fix the _missing FreeImage.dll_ common problem. It occurs only in __Win32__ mode. Also, disabling Automatic Precompiled Header is highly recommended on notebooks with small storage, instructions in the guide too.

## Usage
__Width__

Image width in pixels


__Height__

Image height in pixels


__Resolution__

Image sample(s) detail


__Noise seed__

For getNoise() funciton, it gives some extra simplicity or more details based on the value


__Gaussian noise__

For getNoise() funciton, it gives some extra simplicity or more details based on the value


__Coordinate scale__

Scale of the sample(s) on the image, less number results less scale, large number results large scale of sample(s)


__Rules count__

Pre-setup for states, less number results less complex sample(s), large number results more complex sample(s)


__Rules - minimum value__

Lower limit of the rule for generating random value


__Rules - maximum value__

Upper limit of the rule for generating random value


## Notes
It is saving the process configuration in the bin/data/configs.txt file __after__ you close the program.

The images will be saved in bin/images/ folder.

The base values of inputs are gave by me for processing images I like, tweak it as you wish but don't go crazy, therefore you will break the program.

## TODO
- More tweak options
- Add "Randomize" button
- Color management
- Real time rendering

# gen-art
Making computer art with generative algorithms
###### This is a re-thinking of [dvalim/art-automata](https://github.com/dvalim/art-automata) _cell_ project.

## This project was built with:
- [Visual Studio 2017](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=Community&rel=15)
- [openFrameworks v0.11.0](https://openframeworks.cc/versions/v0.11.0/of_v0.11.0_vs2017_release.zip)
- [K-Lite Codec Pack - Version 12.1.0 Full April 18th 2016](http://filehippo.com/download_klite_codec_pack/67445/)
- Windows 10 Pro (64-bit 1909)

## Installation:
Please read the very detailed installation instruction on the official [openFrameworks website](https://openframeworks.cc/download/) for your OS. However, if you develop in VS 2017, I suggest you to use solution platform __x64__, otherwise you should download and fix the _missing FreeImage.dll_ common problem. It occurs only in __Win32__ mode. Also, disabling Automatic Precompiled Header is highly recommended on notebooks with small storage, instructions in the guide too.

## Usage:
> Width
Image width in pixels

> Height
Image height in pixels

> Resolution
Image sample(s) detail

> Noise seed
For getNoise() funciton, it gives some extra simplicity or more details based on the value

> Gaussian noise
For getNoise() funciton, it gives some extra simplicity or more details based on the value

> Coordinate scale
Scale of the sample(s) on the image, less number results less scale, large number results large scale of sample(s)

> Rules count
Pre-setup for states, less number results less complex sample(s), large number results more complex sample(s)

> Rules - minimum value
Lower limit of the rule for generating random value

> Rules - maximum value
Upper limit of the rule for generating random value

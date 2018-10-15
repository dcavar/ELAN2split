# ELAN2split

(C) 2015-2018 by [Damir Cavar] <<dcavar@iu.edu>>

## License

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at:

[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


## Purpose

ELAN2split is a tool that creates pairs of audio/transcription files that correspond to time-aligned segments in an ELAN file. Each time-aligned segment is saved in two files, the trimmed WAVE-file from the original recording and the transcription or annotation text in the corresponding tier that can be selected via command line. This corpus is ideal to build and train a Forced Aligner and initial speech corpora and subsequently to train speech recognizers. I built this tool to work with the [Prosodylab-Aligner](http://prosodylab.org/tools/aligner/).

This is a command line tool. It does not come with a graphical interface.



## Download Binaries

The following binaries require only a *sox* binary in the system's PATH:

- [Mac OSX (El Capitan, 10.11.6) binary](https://bitbucket.org/dcavar/elan2split/downloads/MacOSX_elan2split.zip) (zipped binary)
- [Ubuntu 16.04 64-bit](https://bitbucket.org/dcavar/elan2split/downloads/Ubuntu-16.04-64bit-elan2split.zip)
- Fedora 28 64-bit
- Windows 10

On Mac OSX or Linux make sure that the unzipped binary has executable flags. You can set the executable flag using:

	chmod +x elan2split

On all systems (Windows since Windows 7) you can run the following command in the command line tool to test wether [SoX] is installed on your system:

	where sox

On Windows 10 with the optional Windows Subsystem for Linux and bash installed you can follow the same compile and build instructions of Ubuntu as described below in the Compilation section.


## Introduction

This code emerged out of the need to create data or corpora for the development of speech recognizers or other speech processing tools, as for example Forced Alignment for audio and textual transcription.

The program will perform various tasks on [ELAN] Annotation Files and corresponding audio media files. It has been tested only on [ELAN] version 4.x and WAV (Microsoft) signed 16 bit PCM audio files.

The program can perform the following tasks:

- list the labels of all annotation tiers
- list all associated media files and check whether the media files are accessible
- segment the audio based on the annotation intervals of any existing tier and generate pairs of audio and annotation text files in a specific location


## Command Line

To list all tier labels in an [ELAN] annotation file (here for example *test.eaf*):

	elan2split -l test.eaf

To list all associated media files in the eaf-file:

	elan2split -m test.eaf

To extract all annotation intervals as individual WAV- and text-files:

	elan2split test.eaf

This will use the first tier to perform the split. It will output all files in the current working directory.

If you want to specify the output directory (it will be automatically created, if not existing):

	elan2split -o ./hr-corpus test.eaf

More options are explained in the help text:

	elan2split -h

The putput files will have specific names. The names are using the [ELAN] tier ID, the start and end time of the annotation interval in milliseconds, and the ending *.wav* and *.txt* accordingly. The file endings for the text-files can be specified to ".lab" for example using the following option:

	elan2split -s .lab

The tier name can be added to the text-file name (preceding the suffix) using the following option:

	elan2split -n

In the current version all time metric is assumed to be using *milliseconds* in the [ELAN] annotation file. This will be more flexible in future. Make sure that your time scale in your [ELAN] annotation is using milliseconds.


## Requirements

### Runtime

The segmentation is based on [SoX]. Please make sure that the *[sox]* binary is accessible via command line and that the folder with the *[sox]* binary is in the system's PATH variable.

[ELAN] Annotation Files (eaf) are [XML] files. When processing eaf-files, the [XML] validator requires internet acces to fetch the eaf-schema specification (if specified in the eaf-file header). In some future release I will provide the option to specify some local copy of the schema for validation to remove the requirement to be online during runtime.


### Compilation

The code is common [C++11](https://en.wikipedia.org/wiki/C%2B%2B11). Besides the common compiler environment it is using the following extensions and libraries:

- [CMake] version 3.2.2 or newer (although 3.0 might work as well)
- [Boost] libraries version 1.47.0 or newer, and in particular the Boost components *program_options*, *filesystem*, *system*
- [Xerces C++] (tested with version 3.1.2)

To compile the binary run the following commands in the source folder on some *nix-type system with the necessary development environment and libraries installed:

    cmake .
    make

This should generate the binary *elan2split*.




[ELAN]: https://tla.mpi.nl/tools/tla-tools/elan/  (ELAN annotations of audio and video resources)
[CMake]: http://www.cmake.org/  (CMake, the cross-platform, open-source build system)
[Xerces C++]: https://xerces.apache.org/xerces-c/  (A validating XML parser)
[Boost]: http://www.boost.org/ (Portable C++ source libraries)
[SoX]: http://sox.sourceforge.net/ (Swiss Army knife of sound processing programs)
[sox]: http://sox.sourceforge.net/ (Swiss Army knife of sound processing programs)
[XML]: https://en.wikipedia.org/wiki/XML (Extensible Markup Language)
[Damir Cavar]: http://damir.cavar.me/ "Damir Cavar"

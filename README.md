OpenTimelineIO-C-bindings
=======
[![OpenTimelineIO](images/opentimelineio-color.svg)](http://opentimeline.io)
==============

[![Supported VFX Platform Versions](https://img.shields.io/badge/vfx%20platform-2016--2020-lightgrey.svg)](http://www.vfxplatform.com/)

Main web site: http://opentimeline.io/

Discussion group: https://lists.aswf.io/g/otio-discussion

Slack channel: https://academysoftwarefdn.slack.com/messages/CMQ9J4BQC
To join, create an account here first: https://slack.aswf.io/

PUBLIC BETA NOTICE
------------------

OpenTimelineIO is currently in Public Beta. That means that it may be missing
some essential features and there are large changes planned. During this phase
we actively encourage you to provide feedback, requests, comments, and/or
contributions.

Overview
--------

OpenTimelineIO is an interchange format and API for editorial cut information.
OTIO is not a container format for media, rather it contains information about
the order and length of cuts and references to external media. This repository 
contains the Java bindings built over the C++ core which you can find [here](https://github.com/PixarAnimationStudios/OpenTimelineIO).

OTIO includes both a file format and an API for manipulating that format.
It also implements a dependency-less library for dealing strictly with time, `opentime`.

You can provide adapters for your video editing tool or pipeline as needed.

The OpenTimelineIO core is written in C++. This repository hosts C bindings for OpenTimelineIO.

Prerequisites
-----------

- CMake 3.17.0+ (Windows) or CMake 3.9.4+ (Linux and macOS)
- Compiler with C++11 support

Dependencies
-----------

- [OpenTimelineIO](http://opentimeline.io/): OpenTimelineIO-C-Bindings is a C-wrapper over the OpenTimelineIO C++ core
- [cmocka](https://cmocka.org/): It is a unit testing framework for C used here to test the bindings.

Building OpenTimelineIO-C-Bindings
------------------------

OpenTimelineIO-C-Bindings have been built and tested on Ubuntu 18.04LTS, Windows 10 and macOS.
Follow these instructions to build the C bindings.

```console
git clone --recurse-submodules https://github.com/OpenTimelineIO/OpenTimelineIO-C-Bindings

cd OpenTimelineIO-C-Bindings

mkdir build

cd build

cmake .. {CMAKE OPTIONS}

cmake --build . --config {Debug/Release}
```

To run the tests from the `build` directory run:
```console
ctest
```

| CMAKE OPTION               | VALID VALUES |                                                                      |
|----------------------------|--------------|----------------------------------------------------------------------|
| CMAKE_INSTALL_PREFIX       | {SOME PATH}  | Install the libs to this path (`usr/lib/` by default on Linux/macOS) |
| COTIO_INSTALL              | ON/OFF       | Install the C bindings (ON by default)                               |
| COTIO_SHARED_LIBS          | ON/OFF       | Build shared if ON, static if OFF (ON by default)                    |
| COTIO_AUTOMATIC_SUBMODULES | ON/OFF       | Fetch submodules automatically (ON by default)                       |


Examples
--------

Looking through the unit tests is a great way to see what OTIO can do. 
See [here](https://github.com/OpenTimelineIO/OpenTimelineIO-C-Bindings/tree/master/tests).

Developing
----------

If you want to contribute to the project, please see: https://opentimelineio.readthedocs.io/en/latest/tutorials/contributing.html

You can get the latest development version via:

`git clone --recurse-submodules https://github.com/OpenTimelineIO/OpenTimelineIO-C-Bindings.git`

License
-------
OpenTimelineIO is open source software. Please see the [LICENSE](LICENSE) for details.

Nothing in the license file or this project grants any right to use Pixar or any other contributor’s trade names, trademarks, service marks, or product names.

Contact
-------

For more information, please visit http://opentimeline.io/
or https://github.com/PixarAnimationStudios/OpenTimelineIO
or join our discussion forum: https://lists.aswf.io/g/otio-discussion

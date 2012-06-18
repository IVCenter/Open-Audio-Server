Open-Audio-Server
=================

Installation and Set-Up
-----------------------

1.  Download the source code.
2.  Change to the build directory.
        cd build
3.  If you want to configure options yourself, type `ccmake ..`
    Otherwise, if you want to go with default options you can just type `cmake ..`
4.  Then, you can compile with `make` and install with `make install`
5.  To generate documentation with Doxygen, use `make docs`

Usage
-----

*   To get started as fast as possible, get the [client API](https://github.com/CalVR/Open-Audio-Server).
    Follow the directions and examples there.
*   Alternatively, if you want to roll your own client interface to communicate
    with the server, look at [this guide](http://ivl.calit2.net/wiki/index.php/OpenAL_Audio_Server) for the protocol specifications.


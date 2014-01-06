# What #

War Dance is a 2D top-down run-and-gun game.

The basic premise is that you run faster than your own bullets. What happens when enemies appear that are just as trigger-happy as you?

You dance.

![screenshot](hub/screenshot.png "Still in development!")

# Development #

A 32-bit Windows executable version of WarDance (built on 1/1/14) is available in this root git directory. To run it, simply make sure config, fonts, and images are all present.

War Dance is not really built on top off any proper game engine. Rather, it's used as a project for me to gain experience with game development in C++ and with the Simple and Fast Media Library (SFML). It is still very much a work in progress, and even most of the sprites are temporary and will eventually be replaced. Sounds may be added in the future, although music will be another beast of its own in regards to licensing, etc.

What it currently has:
- Simple in-game HUD
- Simple main, settings, and pause menus
- Functional and reasonably efficient collision detection
- Three enemy types: grunt, sprinkler, and alien
- Relatively smooth control and response of player movement and shooting

What it will have:
- Complete GUI including splash screen, transitions, and a pretty HUD
- Multiple levels
- More complex enemy types with the addition of bosses
- Player abilities and power-ups
- Sounds!

# Building #

If you want to build the project and try it out, you must have SFML 2.1 headers and libaries somewhere. Visit http://www.sfml-dev.org/download/sfml/2.1/ for those files.

### Visual Studio ###

To build on Visual Studio, simply make a new project and import all the source and header files into it. Then, follow the instructions at http://www.sfml-dev.org/tutorials/2.1/start-vc.php to configure SFML for your project. In summary, be sure you've specified: the include location, the link location, and the linked libararies. Also, make sure to copy the corresponding dlls to where your executable will be.

### g++ ###

Building on Linux is more complicated. The actual compilation and linking steps for WarDance are all automated in the Makefile that's included in the src folder, but the problem lies in how to get SFML to work in the first place.

First of all, if you're using a Debian system, *do not* apt-get install libsfml-dev. For now, that package contains SFML 1.6, which is incompatible with SFML 2.1.
What you want to do is to download the SDK in the page linked above. After untarring it, you have two options:

1. Copy the include and lib files to /usr/local
2. Move the files somewhere, and edit the Makefile in WarDance to have gcc include/link to that location. You can add something like -I/home/you/SFML-2.1/include to INCFLAGS and -L/home/you/SFML-2.1/lib and -Wl,-rpath=/home/you/SFML-2.1/lib to LNKFLAGS, and it should work.

After this, simply enter "make" inside WarDance's src directory. If you're really lucky, it will compile and link sucessfully. Most likely, gcc will complain that you're missing some libraries that SFML needs, and you'd have to look at the errors to discern exactly what. Fortunately, all of those additional libraries can be installed through apt-get.

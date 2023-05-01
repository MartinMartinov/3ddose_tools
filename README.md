# Compiling information for those with Qt5 dev tools installed:

To compile, go to the source directory and invoke the following 2 commands:

```
3ddose_tools/source> qmake
3ddose_tools/source> make
```

and an executable should appear in the parent directory.

# How to install from scratch

If you don't have Qt5 installed, then you will need to do it manually, or through a package manager (like apt-get in Ubuntu).  Here's an Ubuntu example:

```
sudo apt-get install qtbase5-dev qt5-qmake qtbase5-dev-tools
```

Navigate to the directory you intend to install in, and invoke the following commands:

```
> git clone https://github.com/MartinMartinov/3ddose_tools
> cd 3ddose_tools/source/
3ddose_tools/source> qmake
3ddose_tools/source> make
3ddose_tools/source> cd ..
3ddose_tools> ./3ddose_tools
```

and you should launch 3ddose_tools.

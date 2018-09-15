Code is setup for qmake project configuration tool, use the following to generate makefile:

> qmake

and the proper makefile and moc_* dependencies should be generated.

If the code does not compile, it could require you to manually include QtCore, QtWidgets or QtGui libraries to your make file (which are located at $QTDIR).

/***********************************************************************
************************************************************************
*    This code is part of a PRE-RELEASE VERSION of 3ddose_tools,       *
*    a *.3ddose file analysis code.                                    *
*    Copyright 2014 by Carleton University, Ottawa, Canada             *
*    GNU GENERAL PUBLIC LICENSE                                        *
*    Version 3, 29 June 2007                                           *
*                                                                      *
*    Please report all problems to:                                    *
*    Martin Martinov martinov@physics.carleton.ca                      *
*    Rowan Thomson rthomson@physics.carleton.ca                        *
************************************************************************
***********************************************************************/

#include "interface.h"
#include "dose.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    //QApplication::setFont(QFont("Liberation Sans", 12, QFont::Normal, false));
    //app.setStyle("CleanLooks"); // Give the app stylish interface

    Interface w;
    w.show();

    return app.exec();
}

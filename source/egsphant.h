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

#ifndef EGSPHANT_H
#define EGSPHANT_H

#include <QtWidgets>
#include <iostream>
#include <math.h>

class EGSPhant : public QObject {
    Q_OBJECT

signals:
    void progressMade(double n); // Update the progress bar

public:
    EGSPhant();

    int nx, ny, nz; // these hold the number of voxels
    QVector <double> x, y, z; // these hold the boundaries of the above voxels
    QVector <QVector <QVector <char> > > m; // this holds all the media
    QVector <QVector <QVector <double> > > d; // this holds all the densities
    QVector <QString> media; // this holds all the possible media
    double maxDensity;

    void loadEGSPhantFile(QString path);
    void loadEGSPhantFilePlus(QString path);
    void loadbEGSPhantFile(QString path);
    void loadbEGSPhantFilePlus(QString path);

    char getMedia(double px, double py, double pz);
    double getDensity(double px, double py, double pz);
    int getIndex(QString axis, double p);
    QImage getEGSPhantPicDen(QString axis, double ai, double af,
                             double bi, double bf, double d, int res);
    QImage getEGSPhantPicMed(QString axis, double ai, double af,
                             double bi, double bf, double d, int res);

    // Image Processing
    void loadMaps();

    // Progress bar resolution
    const static int MAX_PROGRESS = 1000000000;
};

#endif

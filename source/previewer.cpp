#include "previewer.h"

/*******************************************************************************
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Hover Label Class~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*******************************************************************************/
void HoverLabel::mouseMoveEvent(QMouseEvent *event) {
    if (event->x() > 0 && event->x() < width() && event->y() > 0 && event->y() < height()) {
        emit mouseMoved(event->x(),event->y());
    }
    event->accept();
}

void HoverLabel::wheelEvent(QWheelEvent *event) {
    if (event->delta() > 0 && event->x() > 0 && event->x() < width() && event->y() > 0 && event->y() < height()) {
        emit mouseWheelUp();
    }
    else if (event->x() > 0 && event->x() < width() && event->y() > 0 && event->y() < height()) {
        emit mouseWheelDown();
    }
    event->accept();
}

/*******************************************************************************
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Examine Window Structors~~~~~~~~~~~~~~~~~~~~~~~~~~
*******************************************************************************/
Previewer::Previewer(QWidget *parent, QVector <Dose *> *d)
    : QWidget(parent) { // Pass the parameter to the QWidget constructor
    mom = parent;
    data = d;
    createLayout();  // This creates the layout structure
    connectLayout();  // This connects the layout with the SLOTs
    window->resize(1000, 600);
}

Previewer::~Previewer() {
    for (int i = 0; i < colors->size(); i++) {
        delete (*colors)[i];
    }
    colors->clear();
    for (int i = 0; i < doses->size(); i++) {
        delete (*doses)[i];
    }
    doses->clear();
    for (int i = 0; i < distribs->size(); i++) {
        delete (*distribs)[i];
    }
    distribs->clear();
    for (int i = 0; i < lines->size(); i++) {
        delete (*lines)[i];
    }
    lines->clear();

    delete errors;
    delete layout;
    delete close;
    delete savePic;
    delete loadPhant;
    delete pose;
    delete image;
    delete imageFrame;
    delete picture;
    delete typeMed;
    delete typeDen;
    delete typeLayout;
    delete typeFrame;
    delete dimItems;
    delete dimBox;
    delete dimai;
    delete dimbi;
    delete dimaf;
    delete dimbf;
    delete depth;
    delete dimc;
    delete dimcLeft;
    delete dimcRight;
    delete dimLayout;
    delete dimFrame;
    delete resEdit;
    delete resLayout;
    delete resFrame;
    delete contourLayout;
    delete contourFrame;
    delete colors;
    delete doses;
    delete doseLayout;
    delete doseFrame;
    delete distribs;
    delete lines;
}

/*******************************************************************************
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Previewer Layout~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*******************************************************************************/
void Previewer::createLayout() {
    QString style;
    //style =  "QWidget {background-color: rgb(240, 240, 240)}";
    //style += "QTextEdit";
    //style += "{";
    //style += "background-color: rgb(250, 250, 255);";
    //style += "}";
    //style += "QLineEdit";
    //style += "{";
    //style += "background: rgb(250, 250, 255)";
    //style += "}";
    //style += "QListWidget";
    //style += "{";
    //style += "background-color: rgb(250, 250, 255);";
    //style += "}";

    errors = new QErrorMessage();
    errors->setHidden(TRUE);
    errors->setWindowTitle(tr("Error"));
    window = new QWidget();
    layout = new QGridLayout();
    close = new QPushButton("Close");
    savePic = new QPushButton("Save Image");
    loadPhant = new QPushButton("Load EGSPhant");

    picture = new QPixmap();
    image = new HoverLabel();
    image->setMouseTracking(true);
    image->setAlignment(Qt::AlignCenter);
    image->setPixmap(*picture);
    imageFrame = new QScrollArea();
    imageFrame->setFrameStyle(QFrame::Plain);
    imageFrame->setLineWidth(0);
    imageFrame->setWidget(image);

    typeFrame = new QGroupBox("Display");
    typeLayout = new QGridLayout();
    typeLayout->addWidget(typeMed = new QRadioButton("Media"),0,0);
    typeLayout->addWidget(typeDen = new QRadioButton("Density"),0,1);
    typeFrame->setLayout(typeLayout);
    typeMed->setChecked(1);

    dimItems = new QStringList();
    *dimItems << "z axis" << "y axis" << "x axis";
    dimBox = new QComboBox();
    dimBox->addItems(*dimItems);
    dimai = new LineInput(0, "Initial x", "-1.0");
    dimbi = new LineInput(0, "Initial y", "-1.0");
    dimaf = new LineInput(0, "Final x", "1.0");
    dimbf = new LineInput(0, "Final y", "1.0");
    depth = new QLabel("z Depth");
    dimc  = new QSlider(Qt::Horizontal);
    dimc->setSingleStep(1);
    dimcLeft = new QToolButton();
    dimcLeft->setArrowType(Qt::LeftArrow);
    dimcRight = new QToolButton();
    dimcRight->setArrowType(Qt::RightArrow);
    dimLayout = new QGridLayout();
    dimLayout->addWidget(dimBox,    0, 0, 1, 4);
    dimLayout->addWidget(dimai,     1, 0, 1, 2);
    dimLayout->addWidget(dimbi,     2, 0, 1, 2);
    dimLayout->addWidget(dimaf,     1, 2, 1, 2);
    dimLayout->addWidget(dimbf,     2, 2, 1, 2);
    dimLayout->addWidget(depth,     3, 0, 1, 4);
    dimLayout->addWidget(dimcLeft,  4, 0, 1, 1);
    dimLayout->addWidget(dimc,      4, 1, 1, 2);
    dimLayout->addWidget(dimcRight, 4, 3, 1, 1);
    dimFrame = new QGroupBox("Dimensions");
    dimFrame->setLayout(dimLayout);

    resEdit = new QLineEdit("20");
    resLayout = new QHBoxLayout();
    resLayout->addWidget(resEdit);
    resFrame = new QGroupBox("Resolution (Pixels per Centimeter)");
    resFrame->setLayout(resLayout);

    int num = 5;
    QStringList temp;
    QString cStyle;
    temp << "None";

    colors = new QVector <QPushButton *>;
    doses = new QVector <QLineEdit *>;
    colors->resize(num);
    doses->resize(num);
    contourLayout = new QGridLayout();
    for (int i = 0; i < num; i++) {
        (*colors)[i] = new QPushButton();
        cStyle = "QPushButton {background-color: rgb(";
        cStyle += QString::number(int(255/num*(i+1)));
        cStyle += ",0,";
        cStyle += QString::number(int(255/num*(num-i-1)));
        cStyle += ")}";
        (*colors)[i]->setStyleSheet(cStyle);
        (*doses)[i] = new QLineEdit(QString::number(int(100/num*(i+1))));
        contourLayout->addWidget((*colors)[i],i,0);
        contourLayout->addWidget((*doses)[i],i,1);
    }
    contourFrame = new QFrame;
    contourFrame->setLayout(contourLayout);


    distribs = new QVector <QComboBox *>;
    lines = new QVector <QLabel *>;
    distribs->resize(3);
    lines->resize(3);
    (*lines)[0] = new QLabel("solid");
    (*lines)[1] = new QLabel("dashed");
    (*lines)[2] = new QLabel("dotted");
    doseLayout = new QGridLayout();
    for (int i = 0; i < 3; i++) {
        (*distribs)[i] = new QComboBox();
        (*distribs)[i]->addItems(temp);
        doseLayout->addWidget((*lines)[i],i,0);
        doseLayout->addWidget((*distribs)[i],i,1);
    }
    doseFrame = new QFrame();
    doseFrame->setLayout(doseLayout);

    pose = new QLabel("x: 0.000 cm y: 0.000 cm z: 0.000 cm");
    pose->setFont(QFont("Monospace", 10, QFont::Normal, false));

    // Column 1
    layout->addWidget(dimFrame, 0, 0);
    layout->addWidget(resFrame, 1, 0);
    layout->addWidget(typeFrame, 2, 0);
    layout->addWidget(loadPhant, 3, 0);
    layout->addWidget(savePic, 4, 0);
    layout->setRowStretch(5,20);

    // Column 2-4
    layout->addWidget(imageFrame, 0, 1, 5, 3);
    layout->addWidget(pose, 5, 1);
    layout->addWidget(close, 5, 3);
    layout->setColumnStretch(2,20);

    // Column 5
    layout->addWidget(contourFrame, 0, 4);
    layout->addWidget(doseFrame, 1, 4);

    window->setStyleSheet(style);
    window->setLayout(layout);
    window->setHidden(TRUE);
    window->setWindowTitle(tr("Previewer"));

    // Progress Bar
    remainder = new double (0.0);
    progWin = new QWidget();
    progLayout = new QGridLayout();
    progress = new QProgressBar();
    progLayout->addWidget(progress, 0, 0);
    progWin->setLayout(progLayout);
    progWin->resize(300, 0);
    progress->setRange(0, Dose::MAX_PROGRESS);
    //style = "";
    //style += "QWidget {background-color: rgb(240, 240, 240)}";
    //style += "QProgressBar {";
    //style += "border: 1px solid black;";
    //style += "text-align: top;";
    //style += "padding: 1px;";
    //style += "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,";
    //style += "stop: 0.4 rgb(240, 240, 240), stop: 1.0 rgb(200, 200, 200),";
    //style += "stop: 0.4 rgb(240, 240, 240), stop: 1.0 rgb(200, 200, 200));";
    //style += "}";
    //style += "QProgressBar::chunk {";
    //style += "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,";
    //style += "stop: 0.4 rgb(176, 224, 230), stop: 1.0 rgb(65, 105, 225),";
    //style += "stop: 0.4 rgb(176, 224, 230), stop: 1.0 rgb(65, 105, 225));";
    //style += "}";
    progWin->setFont(QFont("Serif", 12, QFont::Normal, false));
    progWin->setStyleSheet(style);

    // Create a default (100 cm)^3 vacuum egsphant to be able to preview lines without
    // loading another one in
    phant.media.resize(1);
    phant.media[0]="Vacuum";
    phant.nx = phant.ny = 201;
    phant.nz=200;
    phant.x.resize(0);
    phant.y.resize(0);
    phant.z.resize(0);
    for (double n = -10.0; n < 10.0; n+=0.1) {
        phant.x.append(n);
        phant.y.append(n);
        phant.z.append(double(n)+0.05);
    }
    phant.x.append(10.0);
    phant.y.append(10.0);

    QVector <char> mz(200, '1');
    QVector <QVector <char> > my(201, mz);
    QVector <QVector <QVector <char> > > mx(201, my);
    phant.m = mx;
    QVector <double> dz(200, 0);
    QVector <QVector <double> > dy(201, dz);
    QVector <QVector <QVector <double> > > dx(201, dy);
    phant.d = dx;

    // Setup some arbitrary dimensions
    dimai->setText(phant.x[0]);
    dimaf->setText(phant.x[200]);
    dimbi->setText(phant.y[0]);
    dimbf->setText(phant.y[200]);
    dimc->setRange(0,phant.nz-1);
    dimc->setSliderPosition(99);
}

void Previewer::connectLayout() {
    connect(image, SIGNAL(mouseMoved(int, int)),
            this, SLOT(mouseGeom(int, int)));

    connect(dimBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeDim()));

    for (int i = 0; i < colors->size(); i++)
        connect((*doses)[i], SIGNAL(editingFinished()),
                this, SLOT(redraw()));

    for (int i = 0; i < colors->size(); i++)
        connect((*colors)[i], SIGNAL(clicked()),
                this, SLOT(changeColor()));

    for (int i = 0; i < 3; i++)
        connect((*distribs)[i], SIGNAL(currentIndexChanged(int)),
                this, SLOT(redraw()));

    connect(dimai->text, SIGNAL(editingFinished()),
            this, SLOT(checkBounds()));
    connect(dimaf->text, SIGNAL(editingFinished()),
            this, SLOT(checkBounds()));
    connect(dimbi->text, SIGNAL(editingFinished()),
            this, SLOT(checkBounds()));
    connect(dimbf->text, SIGNAL(editingFinished()),
            this, SLOT(checkBounds()));
    connect(dimc, SIGNAL(actionTriggered(int)),
            this, SLOT(checkBounds()));
    //connect(dimc, SIGNAL(sliderMoved(int)),
    //    this, SLOT(checkBounds()));
    //connect(dimc, SIGNAL(sliderReleased()),
    //    this, SLOT(checkBounds()));

    connect(dimcLeft, SIGNAL(clicked()),
            this, SLOT(changeDepthLeft()));
    connect(dimcRight, SIGNAL(clicked()),
            this, SLOT(changeDepthRight()));
    connect(image, SIGNAL(mouseWheelUp()),
            this, SLOT(changeDepthLeft()));
    connect(image, SIGNAL(mouseWheelDown()),
            this, SLOT(changeDepthRight()));

    connect(resEdit, SIGNAL(editingFinished()),
            this, SLOT(redraw()));
    connect(typeMed, SIGNAL(clicked()),
            this, SLOT(redraw()));
    connect(typeDen, SIGNAL(clicked()),
            this, SLOT(redraw()));

    connect(loadPhant, SIGNAL(clicked()),
            this, SLOT(loadPhantom()));
    connect(savePic, SIGNAL(clicked()),
            this, SLOT(saveImage()));
    connect(close, SIGNAL(clicked()),
            this, SLOT(closePreview()));
}

/*******************************************************************************
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Previewer Methods~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*******************************************************************************/
void Previewer::loadPhantom() {
    QString path =
        QFileDialog::getOpenFileName(0, tr("Open File"), 0,
                                     tr("EGSphant File (*.egsphant *begsphant)"));
    if (path == "") {
        return;
    }

    // Set up the progress bar
    progress->reset();
    *remainder = 0;
    progWin->setWindowTitle("Importing EGSPhant Data");
    progWin->show();
    progWin->activateWindow();
    progWin->raise();

    connect(&phant, SIGNAL(progressMade(double)),
            this, SLOT(updateProgress(double)));

    if (path.endsWith(".begsphant")) {
        phant.loadbEGSPhantFilePlus(path);
    }
    else {
        phant.loadEGSPhantFilePlus(path);
    }

    disconnect(&phant, SIGNAL(progressMade(double)),
               this, SLOT(updateProgress(double)));

    progWin->hide();

    changeDim();
}

void Previewer::checkBounds() {
    char flag = FALSE;
    if (phant.nx && phant.ny && phant.nz) {
        if (!dimBox->currentText().compare("x axis")) {
            if (dimai->getText().toDouble() < phant.y[0]) {
                dimai->setText(phant.y[0]);
                flag++;
            }
            if (dimaf->getText().toDouble() > phant.y[phant.ny]) {
                dimaf->setText(phant.y[phant.ny]);
                flag++;
            }
            if (dimbi->getText().toDouble() < phant.z[0]) {
                dimbi->setText(phant.z[0]);
                flag++;
            }
            if (dimaf->getText().toDouble() > phant.z[phant.nz]) {
                dimbf->setText(phant.z[phant.nz]);
                flag++;
            }
        }
        else if (!dimBox->currentText().compare("y axis")) {
            if (dimai->getText().toDouble() < phant.x[0]) {
                dimai->setText(phant.x[0]);
                flag++;
            }
            if (dimaf->getText().toDouble() > phant.x[phant.nx]) {
                dimaf->setText(phant.x[phant.nx]);
                flag++;
            }
            if (dimbi->getText().toDouble() < phant.z[0]) {
                dimbi->setText(phant.z[0]);
                flag++;
            }
            if (dimaf->getText().toDouble() > phant.z[phant.nz]) {
                dimbf->setText(phant.z[phant.nz]);
                flag++;
            }
        }
        else if (!dimBox->currentText().compare("z axis")) {
            if (dimai->getText().toDouble() < phant.x[0]) {
                dimai->setText(phant.x[0]);
                flag++;
            }
            if (dimaf->getText().toDouble() > phant.x[phant.nx]) {
                dimaf->setText(phant.x[phant.nx]);
                flag++;
            }
            if (dimbi->getText().toDouble() < phant.y[0]) {
                dimbi->setText(phant.y[0]);
                flag++;
            }
            if (dimbf->getText().toDouble() > phant.y[phant.ny]) {
                dimbf->setText(phant.y[phant.ny]);
                flag++;
            }
        }

        redraw();
    }
}

void Previewer::changeDepthLeft() {
    int temp = 0;
    if ((temp = dimc->sliderPosition()) == dimc->minimum()) {
        return;
    }
    dimc->setSliderPosition(temp-1);
    checkBounds();
}

void Previewer::changeDepthRight() {
    int temp = 0;
    if ((temp = dimc->sliderPosition()) == dimc->maximum()) {
        return;
    }
    dimc->setSliderPosition(temp+1);
    checkBounds();
}

void Previewer::changeColor() {
    QPushButton *b = (QPushButton *)(QObject::sender());
    QPalette p(b->palette());
    QColor c = QColorDialog::getColor(p.color(QPalette::Button), 0,
                                      "Select Contour Color");
    QString cStyle;
    cStyle = "QPushButton {background-color: rgb(";
    cStyle += QString::number(c.red()) + ",";
    cStyle += QString::number(c.green()) + ",";
    cStyle += QString::number(c.blue()) + ")}";
    b->setStyleSheet(cStyle);

    updateWash();
}

void Previewer::changeDim() {
    if (!dimBox->currentText().compare("x axis")) {
        dimai->title->setText("Initial y");
        dimaf->title->setText("Final y");
        dimbi->title->setText("Initial z");
        dimbf->title->setText("Final z");
        depth->setText("x Depth");
        if (phant.nx && phant.ny && phant.nz) {
            dimai->setText(phant.y[0]);
            dimaf->setText(phant.y[phant.ny]);
            dimbi->setText(phant.z[0]);
            dimbf->setText(phant.z[phant.nz]);
            dimc->setRange(0,phant.nx-1);
        }
    }
    else if (!dimBox->currentText().compare("y axis")) {
        dimai->title->setText("Initial x");
        dimaf->title->setText("Final x");
        dimbi->title->setText("Initial z");
        dimbf->title->setText("Final z");
        depth->setText("y Depth");
        if (phant.nx && phant.ny && phant.nz) {
            dimai->setText(phant.x[0]);
            dimaf->setText(phant.x[phant.nx]);
            dimbi->setText(phant.z[0]);
            dimbf->setText(phant.z[phant.nz]);
            dimc->setRange(0,phant.ny-1);
        }
    }
    else if (!dimBox->currentText().compare("z axis")) {
        dimai->title->setText("Initial x");
        dimaf->title->setText("Final x");
        dimbi->title->setText("Initial y");
        dimbf->title->setText("Final y");
        depth->setText("z Depth");
        if (phant.nx && phant.ny && phant.nz) {
            dimai->setText(phant.x[0]);
            dimaf->setText(phant.x[phant.nx]);
            dimbi->setText(phant.y[0]);
            dimbf->setText(phant.y[phant.ny]);
            dimc->setRange(0,phant.nz-1);
        }
    }

    redraw();
}

void Previewer::redraw() {
    if (!(phant.nx && phant.ny && phant.nz)) {
        return;
    }

    double depth = 0;
    if (!dimBox->currentText().compare("x axis")) {
        depth = (phant.x[dimc->value()]+phant.x[dimc->value()+1])/2.0;
    }
    else if (!dimBox->currentText().compare("y axis")) {
        depth = (phant.y[dimc->value()]+phant.y[dimc->value()+1])/2.0;
    }
    else if (!dimBox->currentText().compare("z axis")) {
        depth = (phant.z[dimc->value()]+phant.z[dimc->value()+1])/2.0;
    }

    if (typeMed->isChecked())
        phantPicture = QPixmap::fromImage(
                           phant.getEGSPhantPicMed(dimBox->currentText(),
                                   dimbi->getText().toDouble(),
                                   dimbf->getText().toDouble(),
                                   dimai->getText().toDouble(),
                                   dimaf->getText().toDouble(),
                                   depth,
                                   resEdit->text().toInt()));
    else if (typeDen->isChecked())
        phantPicture = QPixmap::fromImage(
                           phant.getEGSPhantPicDen(dimBox->currentText(),
                                   dimbi->getText().toDouble(),
                                   dimbf->getText().toDouble(),
                                   dimai->getText().toDouble(),
                                   dimaf->getText().toDouble(),
                                   depth,
                                   resEdit->text().toInt()));

    updateWash();
}

void Previewer::updateWash() {
    updateContour();
}

void Previewer::updateContour() {
    // Skip
    if ((*distribs)[0]->currentIndex() < 1 &&
            (*distribs)[1]->currentIndex() < 1 &&
            (*distribs)[2]->currentIndex() < 1) {
        updateImage();
        return;
    }

    *picture = phantPicture;
    QVector <double> conDose;
    for (int i = 0; i < colors->size(); i++) {
        conDose.append((*doses)[i]->text().toDouble());
    }

    double depth = 0;
    if (!dimBox->currentText().compare("x axis")) {
        depth = (phant.x[dimc->value()]+phant.x[dimc->value()+1])/2.0;
    }
    else if (!dimBox->currentText().compare("y axis")) {
        depth = (phant.y[dimc->value()]+phant.y[dimc->value()+1])/2.0;
    }
    else if (!dimBox->currentText().compare("z axis")) {
        depth = (phant.z[dimc->value()]+phant.z[dimc->value()+1])/2.0;
    }

    // Get the points for isodose contours
    // Solid
    if ((*distribs)[0]->currentText().compare("None")) {
        (*data)[(*distribs)[0]->currentIndex()-1]->getContour(&solid, conDose, dimBox->currentText().left(1).toUpper(), depth, dimbi->getText().toDouble(), dimbf->getText().toDouble(), dimai->getText().toDouble(), dimaf->getText().toDouble(),resEdit->text().toInt());
    }
    else {
        solid.clear();
    }

    // Dashed
    if ((*distribs)[1]->currentText().compare("None")) {
        (*data)[(*distribs)[1]->currentIndex()-1]->getContour(&dashed, conDose, dimBox->currentText().left(1).toUpper(), depth, dimbi->getText().toDouble(), dimbf->getText().toDouble(), dimai->getText().toDouble(), dimaf->getText().toDouble(),resEdit->text().toInt());
    }
    else {
        dashed.clear();
    }

    // Dotted
    if ((*distribs)[2]->currentText().compare("None")) {
        (*data)[(*distribs)[2]->currentIndex()-1]->getContour(&dotted, conDose, dimBox->currentText().left(1).toUpper(), depth, dimbi->getText().toDouble(), dimbf->getText().toDouble(), dimai->getText().toDouble(), dimaf->getText().toDouble(),resEdit->text().toInt());
    }
    else {
        dotted.clear();
    }

    // Draw the contours
    QPainter painter;
    QPen pen;
    pen.setWidth(2);
    painter.begin(picture);

    // For solid
    pen.setStyle(Qt::SolidLine);
    for (int i = 0; i < solid.size(); i++) {
        pen.setColor((*colors)[i]->palette().color(QPalette::Button));
        painter.setPen(pen);
        painter.drawLines(solid[i]);
    }

    // For dashed
    pen.setStyle(Qt::DashLine);
    for (int i = 0; i < dashed.size(); i++) {
        pen.setColor((*colors)[i]->palette().color(QPalette::Button));
        painter.setPen(pen);
        painter.drawLines(dashed[i]);
    }

    // For dotted
    pen.setStyle(Qt::DotLine);
    for (int i = 0; i < dotted.size(); i++) {
        pen.setColor((*colors)[i]->palette().color(QPalette::Button));
        painter.setPen(pen);
        painter.drawLines(dotted[i]);
    }

    painter.end();
    updateImage();
}

void Previewer::updateImage() {
    image->setPixmap(*picture);
    image->setFixedSize(picture->width(), picture->height());
    image->repaint();
}

void Previewer::saveImage() {
    QString path = QFileDialog::getSaveFileName(0, tr("Save File"), "",
                   tr("PNG (*.png)"));
    if (path == "") {
        return;
    }
    if (!path.endsWith(".png")) {
        path += ".png";
    }

    picture->save(path, "PNG");
}

void Previewer::updateDoses() {
    QStringList temp;
    temp << "None";
    for (int i = 0; i < data->size(); i++) {
        temp << ((*data)[i]->getTitle().left(15));
    }

    for (int i = 0; i < 3; i++) {
        (*distribs)[i]->clear();
        (*distribs)[i]->addItems(temp);
    }
}

void Previewer::closePreview() {
    this->setDisabled(TRUE);
    mom->setEnabled(TRUE);
    window->hide();
}

void Previewer::updateProgress(double n) {
    // The flooring function rounds down a real number to the nearest integer
    // In this line, we remove the remainder from the total number
    *remainder += n - floor(n);

    // Next we increment the progress bar by all of our whole numbers
    progress->setValue(int(progress->value() + floor(n) + floor(*remainder)));

    // We redraw the bar
    progress->update();

    // And if our remainder makes a whole number, remove it
    *remainder -= floor(*remainder);
}

void Previewer::mouseGeom(int width, int height) {
    QString temp = "";
    double min, res;
    QString axis = dimBox->currentText();
    res = resEdit->text().toInt();
    min = dimai->getText().toDouble();
    double a = width/res + min;
    min = dimbi->getText().toDouble();
    double b = min + height/res;
    double c;

    // Create the proper label for the proper axis
    if (!axis.compare("x axis")) {
        c = (phant.x[dimc->value()]+phant.x[dimc->value()+1])/2.0;
        temp += "x: ";
        temp += QString::number(c, 'f', 3);
        temp += " cm y: ";
        temp += QString::number(a, 'f', 3);
        temp += " cm z: ";
        temp += QString::number(b, 'f', 3);
        temp += " cm";
    }
    else if (!axis.compare("y axis")) {
        c = (phant.y[dimc->value()]+phant.y[dimc->value()+1])/2.0;
        temp += "x: ";
        temp += QString::number(a, 'f', 3);
        temp += " cm y: ";
        temp += QString::number(c, 'f', 3);
        temp += " cm z: ";
        temp += QString::number(b, 'f', 3);
        temp += " cm";
    }
    else if (!axis.compare("z axis")) {
        c = (phant.z[dimc->value()]+phant.z[dimc->value()+1])/2.0;
        temp += "x: ";
        temp += QString::number(a, 'f', 3);
        temp += " cm y: ";
        temp += QString::number(b, 'f', 3);
        temp += " cm z: ";
        temp += QString::number(c, 'f', 3);
        temp += " cm";
    }

    pose->setText(temp);
    pose->repaint();
}

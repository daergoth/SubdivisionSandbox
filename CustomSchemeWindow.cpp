#include "CustomSchemeWindow.h"

CustomSchemeWindow::CustomSchemeWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    this->setFixedSize(parent->size()*0.8);
    createActions();

    QLabel* labelLeft = new QLabel("Drawing of the fixed mesh");
    labelLeft->setFixedWidth(500);
    labelLeft->setFixedHeight(this->height());
    int labelWidth = 200;
    labelWeightsTitle = new QLabel("Set the weights:");
    labelWeightsTitle->setFixedWidth(labelWidth);

    sidebarVBox = new QVBoxLayout();

    vBoxWeights = new QVBoxLayout();
    container = new QWidget();

    groupBoxTypes = new QGroupBox(tr("Subdivision Type"));
    groupBoxShapes = new QGroupBox(tr("Mesh Type"));
    groupBoxNeighbours = new QGroupBox(tr("Level of Neighbours"));

    approxRB = new QRadioButton("Approximating");
    interpolRB = new QRadioButton("Interpolating");
    triRB = new QRadioButton("Triangular mesh");
    quadRB = new QRadioButton("Quad mesh");
    firstnRB = new QRadioButton("First neighbours");
    secondnRB = new QRadioButton("Second neighbours");

    QObject::connect(approxRB,SIGNAL(clicked(bool)),this,SLOT(onTriggered_Approx()));
    QObject::connect(interpolRB,SIGNAL(clicked(bool)),this,SLOT(onTriggered_Interp()));
    QObject::connect(triRB,SIGNAL(clicked(bool)),this,SLOT(onTriggered_Tri()));
    QObject::connect(quadRB,SIGNAL(clicked(bool)),this,SLOT(onTriggered_Quad()));
    QObject::connect(firstnRB,SIGNAL(clicked(bool)),this,SLOT(onTriggered_FirstNeighbours()));
    QObject::connect(secondnRB,SIGNAL(clicked(bool)),this,SLOT(onTriggered_SecondNeighbours()));

    approxRB->setChecked(true);
    interpolRB->setChecked(false);
    triRB->setChecked(true);
    quadRB->setChecked(false);
    firstnRB->setChecked(true);
    secondnRB->setChecked(false);

    QVBoxLayout *vboxTypes = new QVBoxLayout;
    vboxTypes->addWidget(approxRB);
    vboxTypes->addWidget(interpolRB);
    vboxTypes->addStretch(1);
    groupBoxTypes->setLayout(vboxTypes);

    QVBoxLayout *vboxShapes = new QVBoxLayout;
    vboxShapes->addWidget(triRB);
    vboxShapes->addWidget(quadRB);
    vboxShapes->addStretch(1);
    groupBoxShapes->setLayout(vboxShapes);

    QVBoxLayout *vboxNeighbours = new QVBoxLayout;
    vboxNeighbours->addWidget(firstnRB);
    vboxNeighbours->addWidget(secondnRB);
    vboxNeighbours->addStretch(1);
    groupBoxNeighbours->setLayout(vboxNeighbours);

    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");

    QObject::connect(okButton,SIGNAL(clicked(bool)),this,SLOT(onTriggered_okButton()));
    QObject::connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(onTriggered_cancelButton()));

    QHBoxLayout* hboxButtons = new QHBoxLayout();
    hboxButtons->addWidget(okButton);
    hboxButtons->addWidget(cancelButton);

    sidebarVBox->addWidget(groupBoxTypes);
    sidebarVBox->addWidget(groupBoxShapes);
    sidebarVBox->addWidget(groupBoxNeighbours);

    updateWeightsLayout();

    sidebarVBox->addLayout(vBoxWeights);
    sidebarVBox->addStretch(1);
    sidebarVBox->addLayout(hboxButtons);

    QHBoxLayout *mainlayout = new QHBoxLayout(this);
    mainlayout->addWidget(labelLeft);
    mainlayout->addLayout(sidebarVBox);


    widget->setLayout(mainlayout);
}

CustomSchemeWindow::~CustomSchemeWindow()
{

}

void CustomSchemeWindow::onTriggered_Approx()
{
    updateWeightsLayout();
}

void CustomSchemeWindow::onTriggered_Interp()
{
    updateWeightsLayout();
}

void CustomSchemeWindow::onTriggered_Tri()
{
    updateWeightsLayout();
}

void CustomSchemeWindow::onTriggered_Quad()
{
    updateWeightsLayout();
}

void CustomSchemeWindow::onTriggered_FirstNeighbours()
{
    updateWeightsLayout();
}

void CustomSchemeWindow::onTriggered_SecondNeighbours()
{
    updateWeightsLayout();
}

void CustomSchemeWindow::onTriggered_okButton()
{
    this->hide();
}

void CustomSchemeWindow::onTriggered_cancelButton()
{
    this->hide();
}

void CustomSchemeWindow::updateWeightsLayout()
{
    if(container != 0)
    {
        vBoxWeights->removeWidget(container);
        container->hide();
        container->deleteLater();
    }

    container = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(labelWeightsTitle);

    std::vector<QHBoxLayout*> hboxLineEdits = getVectorOfLineEdits();
    for(QHBoxLayout* hbox : hboxLineEdits){
        layout->addLayout(hbox);
    }

    container->setLayout(layout);
    vBoxWeights->addWidget(container);
}

std::vector<QHBoxLayout*> CustomSchemeWindow::getVectorOfLineEdits()
{
    std::vector<QHBoxLayout*> hboxVector;
    int numberOfWeights = getNumberOfWeights();

    for(int i = 0; i< numberOfWeights;i++){
        QHBoxLayout* hbox = new QHBoxLayout();

        std::string labelstr = "Weight "+std::to_string(i);
        QString labeltxt = QString::fromStdString(labelstr);
        QLabel* label = new QLabel(labeltxt);
        label->setFixedWidth(150);

        QLineEdit* lineEdit = new QLineEdit();
        lineEdit->setFixedWidth(50);

        hbox->addWidget(label);
        hbox->addWidget(lineEdit);
        hbox->addStretch(1);

        hboxVector.push_back(hbox);
    }

    return hboxVector;
}

int CustomSchemeWindow::getNumberOfWeights()
{
    //tri  1 = 2-1 = 1
    //tri  2 = 3-1 = 2
    //quad 1 = 2-1 + 1-1 = 1
    //quad 2 = 2-1 + 3-1 = 3
    return (firstnRB->isChecked())?1:((triRB->isChecked())?2:3);
}

void CustomSchemeWindow::createActions()
{
    approxAction = new QAction(tr("Approximating"), this);
    approxAction->setCheckable(true);
    connect(approxAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_Approx);

    interpolAction = new QAction(tr("Interpolating"), this);
    interpolAction->setCheckable(true);
    connect(interpolAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_Interp);

    triAction = new QAction(tr("Triangle Mesh"), this);
    triAction->setCheckable(true);
    connect(triAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_Tri);

    quadAction = new QAction(tr("Quad Mesh"), this);
    quadAction->setCheckable(true);
    connect(quadAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_Quad);

    firstNeighbourAction = new QAction(tr("First Neighbours"), this);
    firstNeighbourAction->setCheckable(true);
    connect(firstNeighbourAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_FirstNeighbours);

    secondNeighbourAction = new QAction(tr("Second Neighbours"), this);
    secondNeighbourAction->setCheckable(true);
    connect(secondNeighbourAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_SecondNeighbours);

    schemeTypesGroup = new QActionGroup(this);
    schemeTypesGroup->addAction(approxAction);
    schemeTypesGroup->addAction(interpolAction);
    approxAction->setChecked(true);

    shapeGroup = new QActionGroup(this);
    shapeGroup->addAction(triAction);
    shapeGroup->addAction(quadAction);
    triAction->setChecked(true);

    neighbourGroup = new QActionGroup(this);
    neighbourGroup->addAction(firstNeighbourAction);
    neighbourGroup->addAction(secondNeighbourAction);
    firstNeighbourAction->setChecked(true);
}

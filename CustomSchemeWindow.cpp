#include "CustomSchemeWindow.h"

CustomSchemeWindow::CustomSchemeWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QWidget *widget = new QWidget;
        setCentralWidget(widget);

    this->setFixedSize(parent->size()*0.6);
    createActions();
    createMenus();
    QLabel* labelLeft = new QLabel("Drawing of the fixed mesh");
    labelLeft->setFixedWidth(500);
    sidebarVBox = new QVBoxLayout();
    QLabel* label = new QLabel("Set the weights:");
    QHBoxLayout* firstNeighboursHBox = new QHBoxLayout();
    QLabel* firstNeighboursLabel = new QLabel("First neighbour weight:");
    firstNeighboursLabel->setFixedWidth(150);
    QLineEdit* lineEdit1 = new QLineEdit();
    lineEdit1->setFixedWidth(50);
    firstNeighboursHBox->addWidget(firstNeighboursLabel);
    firstNeighboursHBox->addWidget(lineEdit1);
    firstNeighboursHBox->addStretch(1);


    sidebarVBox->addWidget(label);
    sidebarVBox->addLayout(firstNeighboursHBox);
    sidebarVBox->addStretch(1);


    QHBoxLayout *mainlayout = new QHBoxLayout(this);
    mainlayout->addWidget(labelLeft);
    mainlayout->addLayout(sidebarVBox);
    labelLeft->setFixedHeight(this->height());
    int labelWidth = 200;
    label->setFixedWidth(labelWidth);
    widget->setLayout(mainlayout);
}

CustomSchemeWindow::~CustomSchemeWindow()
{

}

void CustomSchemeWindow::onTriggered_Approx()
{

}

void CustomSchemeWindow::onTriggered_Interp()
{

}

void CustomSchemeWindow::onTriggered_Tri()
{

}

void CustomSchemeWindow::onTriggered_Quad()
{

}

void CustomSchemeWindow::onTriggered_FirstNeighbours()
{

}

void CustomSchemeWindow::onTriggered_SecondNeighbours()
{

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

void CustomSchemeWindow::createMenus()
{
    schemetypeMenu = menuBar()->addMenu(tr("Scheme Types"));
    schemetypeMenu->addAction(approxAction);
    schemetypeMenu->addAction(interpolAction);

    shapeMenu = menuBar()->addMenu(tr("Mesh Shapes"));
    shapeMenu->addAction(triAction);
    shapeMenu->addAction(quadAction);


    neighbourMenu = menuBar()->addMenu(tr("Neighbours"));
    neighbourMenu->addAction(firstNeighbourAction);
    neighbourMenu->addAction(secondNeighbourAction);
}

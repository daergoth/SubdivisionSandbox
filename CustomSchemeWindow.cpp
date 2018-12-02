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

    sidebarVBox = new QVBoxLayout();

    vBoxWeights = new QVBoxLayout();
    container = new QWidget();

    //Radio Buttons
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

    //Weights
    vBoxWeights = new QVBoxLayout();

    int labelWidth = 200;
    QLabel* labelSchemeName = new QLabel("Name:");
    leSchemeName = new QLineEdit();
    leSchemeName->setFixedWidth(100);
    QObject::connect(leSchemeName,SIGNAL(textChanged(QString)),this,SLOT(onTextChange(QString)));

    QHBoxLayout* hBoxName = new QHBoxLayout();
    hBoxName->addWidget(labelSchemeName);
    hBoxName->addWidget(leSchemeName);
    hBoxName->addStretch(1);

    labelWeightsTitle = new QLabel("Set the weights:");
    labelWeightsTitle->setFixedWidth(labelWidth);

    setWeightGroupBoxes();

    vBoxWeights->addLayout(hBoxName);
    vBoxWeights->addWidget(labelWeightsTitle);
    vBoxWeights->addWidget(groupBoxFaceWeights);
    vBoxWeights->addWidget(groupBoxOddWeights);
    vBoxWeights->addWidget(groupBoxEvenWeights);

    //Buttons
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");

    okButton->setDisabled(true);

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

    CustomSchemeRefinementType refinement_type = static_cast<CustomSchemeRefinementType>(schemeTypesGroup->checkedAction()->data().toInt());
    CustomSchemeMeshType mesh_type = static_cast<CustomSchemeMeshType>(shapeGroup->checkedAction()->data().toInt());
    int neighbour_level = neighbourGroup->checkedAction()->data().toInt();

    CustomScheme custom_scheme;
    custom_scheme.refinement_type = refinement_type;
    custom_scheme.mesh_type = mesh_type;
    custom_scheme.neighbour_level = neighbour_level;

    custom_scheme.name = leSchemeName->text().toStdString();

    std::vector<OddWeight> odds;

    if(!leFace0->text().isEmpty()){
        OddWeight* odd = new OddWeight(0,CustomSchemeOddWeightType::Face,leFace0->text().toDouble());
        odds.push_back(*odd);
    }
    if(!leFace1->text().isEmpty()){
        OddWeight* odd = new OddWeight(1,CustomSchemeOddWeightType::Face,leFace1->text().toDouble());
        odds.push_back(*odd);
    }
    if(!leOdd0->text().isEmpty()){
        OddWeight* odd = new OddWeight(0,CustomSchemeOddWeightType::Edge,leOdd0->text().toDouble());
        odds.push_back(*odd);
    }
    if(!leOdd1->text().isEmpty()){
        OddWeight* odd = new OddWeight(1,CustomSchemeOddWeightType::Edge,leOdd1->text().toDouble());
        odds.push_back(*odd);
    }

    EvenWeights even{};

    if(!leEven0->text().isEmpty()){
        even.alfa = leEven0->text().toDouble();
    }else{
        even.alfa = 0;
    }

    if(!leEven1->text().isEmpty()){
        even.beta = leEven1->text().toDouble();
    }else{
        even.beta = 0;
    }

    custom_scheme.weights = {odds, even};

    CustomSchemeHandler& csh = CustomSchemeHandler::getInstance();
    csh.setCurrentCustomScheme(custom_scheme);

    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::Custom);

    this->hide();
}

void CustomSchemeWindow::onTriggered_cancelButton()
{
    this->hide();
}

void CustomSchemeWindow::updateWeightsLayout()
{
    mapLineEditsDisabled[leFace0] = true;
    mapLineEditsDisabled[leFace1] = true;
    mapLineEditsDisabled[leOdd0] = true;
    mapLineEditsDisabled[leOdd1] = true;
    mapLineEditsDisabled[leEven0] = true;
    mapLineEditsDisabled[leEven1] = true;

    if(triRB->isChecked()){
        if(approxRB->isChecked()){
            if(firstnRB->isChecked()){
                mapLineEditsDisabled[leOdd0] = false;
                mapLineEditsDisabled[leEven0] = false;
            }else{
                mapLineEditsDisabled[leOdd0] = false;
                mapLineEditsDisabled[leOdd1] = false;
                mapLineEditsDisabled[leEven0] = false;
            }
        }else{
            if(firstnRB->isChecked()){
                mapLineEditsDisabled[leOdd0] = false;
            }else{
                mapLineEditsDisabled[leOdd0] = false;
                mapLineEditsDisabled[leOdd1] = false;
            }
        }
    }else{
        if(approxRB->isChecked()){
            if(firstnRB->isChecked()){
                mapLineEditsDisabled[leOdd0] = false;
                mapLineEditsDisabled[leEven0] = false;
                mapLineEditsDisabled[leEven1] = false;
            }else{
                mapLineEditsDisabled[leFace0] = false;
                mapLineEditsDisabled[leFace1] = false;
                mapLineEditsDisabled[leOdd0] = false;
                mapLineEditsDisabled[leEven0] = false;
                mapLineEditsDisabled[leEven1] = false;
            }
        }else{
            if(firstnRB->isChecked()){
                mapLineEditsDisabled[leOdd0] = false;
            }else{
                mapLineEditsDisabled[leFace0] = false;
                mapLineEditsDisabled[leFace1] = false;
                mapLineEditsDisabled[leOdd0] = false;
            }
        }
    }
    updateLineEditsDisableSetting();
}

void CustomSchemeWindow::updateLineEditsDisableSetting(){
    for(auto item : mapLineEditsDisabled){
        item.first->setDisabled(item.second);
        if(item.second){
            item.first->clear();
        }
    }
}

void CustomSchemeWindow::setWeightGroupBoxes(){
    int labelWidth = 150;
    int lineEditWith = 50;

    groupBoxFaceWeights = new QGroupBox(tr("Face Vertex"));
    groupBoxOddWeights = new QGroupBox(tr("Edge Vertex"));
    groupBoxEvenWeights = new QGroupBox(tr("Even Vertex"));

    groupBoxFaceWeights->setFixedWidth(labelWidth+lineEditWith);
    groupBoxOddWeights->setFixedWidth(labelWidth+lineEditWith);
    groupBoxEvenWeights->setFixedWidth(labelWidth+lineEditWith);

    QVBoxLayout* layoutFace = new QVBoxLayout();
    QVBoxLayout* layoutOdd = new QVBoxLayout();
    QVBoxLayout* layoutEven = new QVBoxLayout();

    QHBoxLayout* hboxFace0 = new QHBoxLayout();
    QHBoxLayout* hboxFace1 = new QHBoxLayout();
    QHBoxLayout* hboxOdd0 = new QHBoxLayout();
    QHBoxLayout* hboxOdd1 = new QHBoxLayout();
    QHBoxLayout* hboxEven0 = new QHBoxLayout();
    QHBoxLayout* hboxEven1 = new QHBoxLayout();

    leFace0 = new QLineEdit();
    leFace1 = new QLineEdit();
    leOdd0 = new QLineEdit();
    leOdd1 = new QLineEdit();
    leEven0 = new QLineEdit();
    leEven1 = new QLineEdit();

    QObject::connect(leFace0,SIGNAL(textChanged(QString)),this,SLOT(onTextChange(QString)));
    QObject::connect(leFace1,SIGNAL(textChanged(QString)),this,SLOT(onTextChange(QString)));
    QObject::connect(leOdd0,SIGNAL(textChanged(QString)),this,SLOT(onTextChange(QString)));
    QObject::connect(leOdd1,SIGNAL(textChanged(QString)),this,SLOT(onTextChange(QString)));
    QObject::connect(leEven0,SIGNAL(textChanged(QString)),this,SLOT(onTextChange(QString)));
    QObject::connect(leEven1,SIGNAL(textChanged(QString)),this,SLOT(onTextChange(QString)));

    leFace0->setValidator( new QDoubleValidator(this) );
    leFace1->setValidator( new QDoubleValidator(this) );
    leOdd0->setValidator( new QDoubleValidator(this) );
    leOdd1->setValidator( new QDoubleValidator(this) );
    leEven0->setValidator( new QDoubleValidator(this) );
    leEven1->setValidator( new QDoubleValidator(this) );

    leFace0->setFixedWidth(lineEditWith);
    leFace1->setFixedWidth(lineEditWith);
    leOdd0->setFixedWidth(lineEditWith);
    leOdd1->setFixedWidth(lineEditWith);
    leEven0->setFixedWidth(lineEditWith);
    leEven1->setFixedWidth(lineEditWith);

    hboxFace0->addWidget(new QLabel("Weight 0:"));
    hboxFace1->addWidget(new QLabel("Weight 1:"));
    hboxOdd0->addWidget(new QLabel("Weight 0:"));
    hboxOdd1->addWidget(new QLabel("Weight 1:"));
    hboxEven0->addWidget(new QLabel("Weight 0:"));
    hboxEven1->addWidget(new QLabel("Weight 1:"));

    hboxFace0->addWidget(leFace0);
    hboxFace1->addWidget(leFace1);
    hboxOdd0->addWidget(leOdd0);
    hboxOdd1->addWidget(leOdd1);
    hboxEven0->addWidget(leEven0);
    hboxEven1->addWidget(leEven1);

    hboxFace0->addStretch(1);
    hboxFace1->addStretch(1);
    hboxOdd0->addStretch(1);
    hboxOdd1->addStretch(1);
    hboxEven0->addStretch(1);
    hboxEven1->addStretch(1);

    layoutFace->addLayout(hboxFace0);
    layoutFace->addLayout(hboxFace1);
    layoutOdd->addLayout(hboxOdd0);
    layoutOdd->addLayout(hboxOdd1);
    layoutEven->addLayout(hboxEven0);
    layoutEven->addLayout(hboxEven1);

    groupBoxFaceWeights->setLayout(layoutFace);
    groupBoxOddWeights->setLayout(layoutOdd);
    groupBoxEvenWeights->setLayout(layoutEven);

}

void CustomSchemeWindow::onTextChange(const QString &text){
    if(leSchemeName->text().isEmpty()){
        okButton->setDisabled(true);
        return;
    }
    for(auto item : mapLineEditsDisabled){
        //enabled & empty
        if(!item.second && item.first->text().isEmpty()){
            okButton->setDisabled(true);
            return;
        }
    }
    okButton->setDisabled(false);
}

void CustomSchemeWindow::createActions()
{
    approxAction = new QAction(tr("Approximating"), this);
    approxAction->setCheckable(true);
    approxAction->setData(CustomSchemeRefinementType::Approx);
    connect(approxAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_Approx);

    interpolAction = new QAction(tr("Interpolating"), this);
    interpolAction->setCheckable(true);
    interpolAction->setData(CustomSchemeRefinementType::Interp);
    connect(interpolAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_Interp);

    triAction = new QAction(tr("Triangle Mesh"), this);
    triAction->setCheckable(true);
    triAction->setData(CustomSchemeMeshType::Tri);
    connect(triAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_Tri);

    quadAction = new QAction(tr("Quad Mesh"), this);
    quadAction->setCheckable(true);
    quadAction->setData(CustomSchemeMeshType::Quad);
    connect(quadAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_Quad);

    firstNeighbourAction = new QAction(tr("First Neighbours"), this);
    firstNeighbourAction->setCheckable(true);
    firstNeighbourAction->setData(1);
    connect(firstNeighbourAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_FirstNeighbours);

    secondNeighbourAction = new QAction(tr("Second Neighbours"), this);
    secondNeighbourAction->setCheckable(true);
    secondNeighbourAction->setData(2);
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

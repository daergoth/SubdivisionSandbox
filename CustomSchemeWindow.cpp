#include "CustomSchemeWindow.h"

CustomSchemeWindow::CustomSchemeWindow(QWidget *parent) :
    QDialog(parent)
{
    this->setFixedSize(parent->size()*0.8);
    createActions();

    labelOddPng = new QLabel();
    labelEvenPng = new QLabel();
    labelOddPng->setAlignment(Qt::AlignCenter);
    labelEvenPng->setAlignment(Qt::AlignCenter);

    QVBoxLayout* vBoxPng = new QVBoxLayout();
    vBoxPng->addWidget(labelOddPng);
    vBoxPng->addWidget(labelEvenPng);

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
    mainlayout->addLayout(vBoxPng);
    mainlayout->addLayout(sidebarVBox);


    setLayout(mainlayout);
}

CustomSchemeWindow::~CustomSchemeWindow()
{

}

void CustomSchemeWindow::onTriggered_Approx()
{
    custom_scheme.refinement_type = CustomSchemeRefinementType::Approx;
    updateWeightsLayout();
}

void CustomSchemeWindow::onTriggered_Interp()
{
    custom_scheme.refinement_type = CustomSchemeRefinementType::Interp;
    updateWeightsLayout();
}

void CustomSchemeWindow::onTriggered_Tri()
{
    custom_scheme.mesh_type = CustomSchemeMeshType::Tri;
    updateWeightsLayout();
}

void CustomSchemeWindow::onTriggered_Quad()
{
    custom_scheme.mesh_type = CustomSchemeMeshType::Quad;
    updateWeightsLayout();
}

void CustomSchemeWindow::onTriggered_FirstNeighbours()
{
    custom_scheme.neighbour_level = 1;
    updateWeightsLayout();
}

void CustomSchemeWindow::onTriggered_SecondNeighbours()
{
    custom_scheme.neighbour_level = 2;
    updateWeightsLayout();
}

void CustomSchemeWindow::onTriggered_okButton()
{
    custom_scheme.name = leSchemeName->text().toStdString();

    QLocale locale;
    locale.setDefault(QLocale::Hungarian);
    locale.setNumberOptions(QLocale::OmitGroupSeparator);

    std::vector<OddWeight> odds;

    if(!leFace0->text().isEmpty()){
        OddWeight odd(0, CustomSchemeOddWeightType::Face, locale.toDouble(leFace0->text(), nullptr));
        odds.push_back(odd);
    }
    if(!leFace1->text().isEmpty()){
        OddWeight odd(1, CustomSchemeOddWeightType::Face, locale.toDouble(leFace1->text()));
        odds.push_back(odd);
    }
    if(!leOdd0->text().isEmpty()){
        OddWeight odd(0, CustomSchemeOddWeightType::Edge, locale.toDouble(leOdd0->text()));
        odds.push_back(odd);
    }
    if(!leOdd1->text().isEmpty()){
        OddWeight odd(1, CustomSchemeOddWeightType::Edge, locale.toDouble(leOdd1->text()));
        odds.push_back(odd);
    }

    EvenWeights even{};

    if(!leEven0->text().isEmpty()){
        even.alfa = locale.toDouble(leEven0->text());
    }else{
        even.alfa = 0;
    }

    if(!leEven1->text().isEmpty()){
        even.beta = locale.toDouble(leEven1->text());
    }else{
        even.beta = 0;
    }

    custom_scheme.weights = Weights{odds, even};

    std::cout << "Created custom scheme: " << std::endl << custom_scheme << std::endl;
    CustomSchemeHandler& csh = CustomSchemeHandler::getInstance();
    csh.setCurrentCustomScheme(custom_scheme);

    this->accept();
}

void CustomSchemeWindow::onTriggered_cancelButton()
{
    this->reject();
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
                updatePng(labelOddPng,":/odd_tri_1.png");
                updatePng(labelEvenPng,":/even_tri.png");
            }else{
                mapLineEditsDisabled[leOdd0] = false;
                mapLineEditsDisabled[leOdd1] = false;
                mapLineEditsDisabled[leEven0] = false;
                updatePng(labelOddPng,":/odd_tri_2.png");
                updatePng(labelEvenPng,":/even_tri.png");
            }
        }else{
            if(firstnRB->isChecked()){
                mapLineEditsDisabled[leOdd0] = false;
                updatePng(labelOddPng,":/odd_tri_1.png");
                labelEvenPng->clear();
            }else{
                mapLineEditsDisabled[leOdd0] = false;
                mapLineEditsDisabled[leOdd1] = false;
                updatePng(labelOddPng,":/odd_tri_2.png");
                labelEvenPng->clear();
            }
        }
    }else{
        if(approxRB->isChecked()){
            if(firstnRB->isChecked()){
                mapLineEditsDisabled[leOdd0] = false;
                mapLineEditsDisabled[leEven0] = false;
                mapLineEditsDisabled[leEven1] = false;
                updatePng(labelOddPng,":/odd_quad_1.png");
                updatePng(labelEvenPng,":/even_quad.png");
            }else{
                mapLineEditsDisabled[leFace0] = false;
                mapLineEditsDisabled[leFace1] = false;
                mapLineEditsDisabled[leOdd0] = false;
                mapLineEditsDisabled[leEven0] = false;
                mapLineEditsDisabled[leEven1] = false;
                updatePng(labelOddPng,":/odd_quad_2.png");
                updatePng(labelEvenPng,":/even_quad.png");
            }
        }else{
            if(firstnRB->isChecked()){
                mapLineEditsDisabled[leOdd0] = false;
                updatePng(labelOddPng,":/odd_quad_1.png");
                labelEvenPng->clear();
            }else{
                mapLineEditsDisabled[leFace0] = false;
                mapLineEditsDisabled[leFace1] = false;
                mapLineEditsDisabled[leOdd0] = false;
                updatePng(labelOddPng,":/odd_quad_2.png");
                labelEvenPng->clear();
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

void CustomSchemeWindow::updatePng(QLabel* label, const QString png){
    int height = 264;
    int width = 296;
    label->setFixedWidth(500);
    label->setFixedHeight(height);
    QPixmap mypix(png);
    mypix = mypix.scaled(width, height);
    label->setPixmap(mypix);
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

    QLabel* lFace0 = new QLabel(" Weight 0 ");
    QLabel* lFace1 = new QLabel(" Weight 1 ");
    QLabel* lOdd0 = new QLabel(" Weight 0 ");
    QLabel* lOdd1 = new QLabel(" Weight 1 ");
    QLabel* lEven0 = new QLabel(" Weight 0 ");
    QLabel* lEven1 = new QLabel(" Weight 1 ");

    lFace0->setStyleSheet("QLabel { background-color : blue; color : white; }");
    lFace1->setStyleSheet("QLabel { background-color : green; color : white; }");

    lOdd0->setStyleSheet("QLabel { background-color : blue; color : white; }");
    lOdd1->setStyleSheet("QLabel { background-color : green; color : white; }");

    lEven0->setStyleSheet("QLabel { background-color : blue; color : white; }");
    lEven1->setStyleSheet("QLabel { background-color : green; color : white; }");

    hboxFace0->addWidget(lFace0);
    hboxFace1->addWidget(lFace1);
    hboxOdd0->addWidget(lOdd0);
    hboxOdd1->addWidget(lOdd1);
    hboxEven0->addWidget(lEven0);
    hboxEven1->addWidget(lEven1);

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
    approxAction->setData(0);
    connect(approxAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_Approx);

    interpolAction = new QAction(tr("Interpolating"), this);
    interpolAction->setCheckable(true);
    interpolAction->setData(1);
    connect(interpolAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_Interp);

    triAction = new QAction(tr("Triangle Mesh"), this);
    triAction->setCheckable(true);
    triAction->setData(0);
    connect(triAction, &QAction::triggered, this, &CustomSchemeWindow::onTriggered_Tri);

    quadAction = new QAction(tr("Quad Mesh"), this);
    quadAction->setCheckable(true);
    quadAction->setData(1);
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
    custom_scheme.refinement_type = CustomSchemeRefinementType::Approx;

    shapeGroup = new QActionGroup(this);
    shapeGroup->addAction(triAction);
    shapeGroup->addAction(quadAction);
    triAction->setChecked(true);
    custom_scheme.mesh_type = CustomSchemeMeshType::Tri;

    neighbourGroup = new QActionGroup(this);
    neighbourGroup->addAction(firstNeighbourAction);
    neighbourGroup->addAction(secondNeighbourAction);
    firstNeighbourAction->setChecked(true);
    custom_scheme.neighbour_level = 1;

}

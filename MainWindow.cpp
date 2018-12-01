#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    createActions();
    createMenus();

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    vBoxSubdivControls = new QVBoxLayout();

    label = new QLabel();
    label->setFixedHeight(this->height()*0.3);
    int labelWidth = 200;
    label->setFixedWidth(labelWidth);
    label->setAlignment(Qt::AlignCenter);

    subdivisionButton = new QPushButton("Subdivision");
    revertButton = new QPushButton("Revert");
    revertButton->setDisabled(true);

    QObject::connect(subdivisionButton,SIGNAL(clicked(bool)),this,SLOT(doSubdivision()));
    QObject::connect(revertButton,SIGNAL(clicked(bool)),this,SLOT(revertSubdivision()));

    QHBoxLayout *hBoxButtons = new QHBoxLayout();
    hBoxButtons->addWidget(subdivisionButton);
    hBoxButtons->addWidget(revertButton);

    spinnerLabel = new QLabel();
    spinnerMovie = new QMovie(":/spinner.gif");
    spinnerLabel->setMovie(spinnerMovie);
    spinnerMovie->start();
    spinnerLabel->hide();
    spinnerLabel->setAlignment(Qt::AlignCenter);

    vBoxSubdivControls->addWidget(label);
    vBoxSubdivControls->addLayout(hBoxButtons);
    vBoxSubdivControls->addWidget(spinnerLabel);
    vBoxSubdivControls->addStretch(1);

    openglWidget = new MainOpenGLWidget(this);
    openglWidget->setFixedWidth(this->width()-labelWidth);
    openglWidget->setFixedHeight(this->height());

    mainLayout->addLayout(vBoxSubdivControls);
    mainLayout->addWidget(openglWidget);
    widget->setLayout(mainLayout);

    setLabelSubdivision("Loop Subdivision");

    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.setBaseMesh(Mesh::makeCube());
    sc.switchTo(SubdivisionScheme::Loop);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLabelSubdivision(QString name){
    label->setText(name);
    label->setFont(QFont("Purisa", 10));
}

void MainWindow::onTriggered_LoopSubdiv()
{
    setLabelSubdivision("Loop Subdivision");

    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::Loop);
}

void MainWindow::onTriggered_ButterflySubdiv()
{
    setLabelSubdivision("Butterfly Subdivision");

    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::Butterfly);
}

void MainWindow::onTriggered_CatmullClarkSubdiv()
{
    setLabelSubdivision("Catmull-Clark Subdivision");

    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::CatmullClark);

}

void MainWindow::onTriggered_KobbeltSubdiv()
{
    setLabelSubdivision("Kobbelt Subdivision");

    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::Kobbelt);
}

void MainWindow::onTriggered_CreateCustomScheme()
{
    // TODO: implement integration of SubdivisionController, CustomSchemeHandler and creation UI

    customSchemeWindow = new CustomSchemeWindow(this);
    customSchemeWindow->show();
}

void MainWindow::onTriggered_CubeObject()
{
    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.setBaseMesh(Mesh::makeCube());
    openglWidget->update();
}

void MainWindow::onTriggered_TetrahedronObject()
{
    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.setBaseMesh(Mesh::makeTetrahedron());
    openglWidget->update();
}

void MainWindow::onTriggered_OpenObjFile()
{
    // show file chooser dialog
    QString qfileName = QFileDialog::getOpenFileName(this, tr("Open Obj"), "/home", tr("*.obj"));
    //printf(qfileName.toUtf8().constData());
    std::string fileName = qfileName.toUtf8().constData();
    /* Assimp::Importer importer;
       const aiScene* scene = importer
                .ReadFile(fileName.c_str(),
                          aiProcess_Triangulate |
                          aiProcess_GenSmoothNormals |
                          aiProcess_FlipUVs);
         if(!scene){
            std::cout << "Mesh load failed!: " << fileName << std::endl;
            assert(0 == 0);
        }
                */

    openglWidget->update();
}

void MainWindow::on_actionOpen_scheme_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
            tr("Open Subdivision Scheme"), "",
            tr("Subdivision Scheme (*.sbs);;All Files (*)"));
    if (!filename.isEmpty()) {
        CustomSchemeHandler& csh = CustomSchemeHandler::getInstance();
        CustomScheme openedScheme = csh.openCustomScheme(filename);
        csh.setCurrentCustomScheme(openedScheme);
    }
}

void MainWindow::on_actionSave_scheme_triggered()
{
    CustomSchemeHandler& csh = CustomSchemeHandler::getInstance();
    std::shared_ptr<CustomScheme> current = csh.getCurrentCustomScheme();

    if (current) {
        QString filename = QFileDialog::getSaveFileName(this,
                tr("Save Subdivision Scheme"), "",
                tr("Subdivision Scheme (*.sbs);;All Files (*)"));
        if (!filename.isEmpty()) {
            csh.saveCustomScheme(filename, current);
        }
    } else {
        QMessageBox::information(this, tr("Nothing to save"),
                        tr("There is no active custom scheme to save, open one or create a new one first!"));
    }
}

void MainWindow::on_actionAbout_triggered()
{

}


void MainWindow::createActions()
{
    loopAction = new QAction(tr("Loop"), this);
    loopAction->setCheckable(true);
    connect(loopAction, &QAction::triggered, this, &MainWindow::onTriggered_LoopSubdiv);

    butterflyAction = new QAction(tr("Butterfly"), this);
    butterflyAction->setCheckable(true);
    connect(butterflyAction, &QAction::triggered, this, &MainWindow::onTriggered_ButterflySubdiv);

    catmullclarkAction = new QAction(tr("Catmull-Clark"), this);
    catmullclarkAction->setCheckable(true);
    connect(catmullclarkAction, &QAction::triggered, this, &MainWindow::onTriggered_CatmullClarkSubdiv);

    kobbeltAction = new QAction(tr("Kobbelt"), this);
    kobbeltAction->setCheckable(true);
    connect(kobbeltAction, &QAction::triggered, this, &MainWindow::onTriggered_KobbeltSubdiv);

    createCustomSchemeAction = new QAction(tr("Create custom scheme..."), this);
    connect(createCustomSchemeAction, &QAction::triggered, this, &MainWindow::onTriggered_CreateCustomScheme);

    cubeObjectAction = new QAction(tr("Cube"), this);
    cubeObjectAction->setCheckable(true);
    connect(cubeObjectAction, &QAction::triggered, this, &MainWindow::onTriggered_CubeObject);

    tetrahedronObjectAction = new QAction(tr("Tetrahedron"), this);
    tetrahedronObjectAction->setCheckable(true);
    connect(tetrahedronObjectAction, &QAction::triggered, this, &MainWindow::onTriggered_TetrahedronObject);

    openObjFileAction = new QAction(tr("Open OBJ file..."), this);
    connect(openObjFileAction, &QAction::triggered, this, &MainWindow::onTriggered_OpenObjFile);

    subdivisionSchemesGroup = new QActionGroup(this);
    subdivisionSchemesGroup->addAction(loopAction);
    subdivisionSchemesGroup->addAction(butterflyAction);
    subdivisionSchemesGroup->addAction(catmullclarkAction);
    subdivisionSchemesGroup->addAction(kobbeltAction);
    loopAction->setChecked(true);

    objectGroup = new QActionGroup(this);
    objectGroup->addAction(cubeObjectAction);
    objectGroup->addAction(tetrahedronObjectAction);
    cubeObjectAction->setChecked(true);
}

void MainWindow::createMenus()
{
    schemesMenu = menuBar()->addMenu(tr("Schemes"));
    schemesMenu->addAction(loopAction);
    schemesMenu->addAction(butterflyAction);
    schemesMenu->addAction(catmullclarkAction);
    schemesMenu->addAction(kobbeltAction);
    schemesMenu->addSeparator();
    schemesMenu->addAction(createCustomSchemeAction);

    objectsMenu = menuBar()->addMenu(tr("Objects"));
    objectsMenu->addAction(cubeObjectAction);
    objectsMenu->addAction(tetrahedronObjectAction);
    objectsMenu->addSeparator();
    objectsMenu->addAction(openObjFileAction);
}

void MainWindow::doSubdivision()
{
    spinnerMovie->start();
    spinnerLabel->show();

    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.doSubdivision();
    openglWidget->update();

    spinnerMovie->stop();
    spinnerLabel->hide();
    printf("Subdivision done\n");

    revertButton->setDisabled(false);
}

void MainWindow::revertSubdivision()
{
    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.doBackwardStep();
    openglWidget->update();
    printf("Revert done\n");

    revertButton->setDisabled(!sc.canDoBackwardStep());
}

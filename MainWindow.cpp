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

    openglWidget = new MainOpenGLWidget(this);

    label = new QLabel();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(openglWidget);
    label->setFixedHeight(this->height());
    int labelWidth = 200;
    label->setFixedWidth(labelWidth);
    openglWidget->setFixedWidth(this->width()-labelWidth);
    openglWidget->setFixedHeight(this->height());

    widget->setLayout(layout);

    Mesh baseMesh;
    SubdivisionController::getInstance().setBaseMesh(baseMesh);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLabelSubdivision(){
    label->setText("Loop Subdivision\nUse \"+\" key to perform subdivision\n and \"-\" key to revert it!");
    label->setFont(QFont("Purisa", 10));
}

void MainWindow::onTriggered_LoopSubdiv()
{
    setLabelSubdivision();

    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::Loop);
}

void MainWindow::onTriggered_ButterflySubdiv()
{
    setLabelSubdivision();

    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::Butterfly);
}

void MainWindow::onTriggered_CatmullClarkSubdiv()
{
    setLabelSubdivision();

    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::CatmullClark);
}

void MainWindow::onTriggered_KobbeltSubdiv()
{
    setLabelSubdivision();

    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::Kobbelt);
}

void MainWindow::onTriggered_CreateCustomScheme()
{
    // TODO: implement integration of SubdivisionController, CustomSchemeHandler and creation UI
    CustomSchemeHandler::getInstance().debug();

    customSchemeWindow = new CustomSchemeWindow(this);
    customSchemeWindow->show();
}

void MainWindow::onTriggered_CubeObject()
{

}

void MainWindow::onTriggered_TetrahedronObject()
{

}

void MainWindow::onTriggered_OpenObjFile()
{

    printf("Obj\n");

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

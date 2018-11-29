#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createMenus();

    openglWidget = new MainOpenGLWidget(this);
    setCentralWidget(openglWidget);

    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.setBaseMesh(Mesh::makeCube());
    sc.switchTo(SubdivisionScheme::Loop);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTriggered_LoopSubdiv()
{
    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::Loop);
}

void MainWindow::onTriggered_ButterflySubdiv()
{
    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::Butterfly);
}

void MainWindow::onTriggered_CatmullClarkSubdiv()
{
    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::CatmullClark);
}

void MainWindow::onTriggered_KobbeltSubdiv()
{
    SubdivisionController& sc = SubdivisionController::getInstance();
    sc.switchTo(SubdivisionScheme::Kobbelt);
}

void MainWindow::onTriggered_CreateCustomScheme()
{
    // TODO: implement integration of SubdivisionController, CustomSchemeHandler and creation UI
    CustomSchemeHandler::getInstance().debug();
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

#include "MainWindow.h"
#include "ui_mainwindow.h"


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


    typedef CGAL::Exact_predicates_exact_constructions_kernel kernel;

    CGAL::Polygon_2<kernel> current_polygon;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTriggered_LoopSubdiv()
{
    //Show label for help
    int level = 0;
    label->setText("Loop Subdivision\nUse \"+\" key to perform subdivision\n and \"-\" key to reverte it!");

    //label->setFont(QFont("Purisa", 10));

    // LoopSubdiv initialized with the current mesh
    // LoopSubdiv loopSubdiv = new LoopSubdiv(currentMesh);

    // Keep the old Mesh in an array to be able to go back
    // Mesh[] meshLevels = new Arraylist();
    // meshLevels[0] = currentMesh;

    //meshLevels[] loopSubdiv.nextLevel()
}

void MainWindow::onTriggered_ButterflySubdiv()
{

}

void MainWindow::onTriggered_CatmullClarkSubdiv()
{

}

void MainWindow::onTriggered_KobbeltSubdiv()
{

}

void MainWindow::onTriggered_CreateCustomScheme()
{

}

void MainWindow::onTriggered_CubeObject()
{
    openglWidget->drawCubeObject();
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

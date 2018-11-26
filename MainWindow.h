#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "MainOpenGLWidget.h"
#include "customschemehandler.h"

class QAction;
class QActionGroup;
class QMenu;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onTriggered_LoopSubdiv();
    void onTriggered_ButterflySubdiv();
    void onTriggered_CatmullClarkSubdiv();
    void onTriggered_KobbeltSubdiv();
    void onTriggered_CreateCustomScheme();

    void onTriggered_CubeObject();
    void onTriggered_TetrahedronObject();
    void onTriggered_OpenObjFile();

    void on_actionOpen_scheme_triggered();

private:
    Ui::MainWindow *ui;

    void createActions();
    void createMenus();

    QMenu* schemesMenu;
    QMenu* objectsMenu;

    QActionGroup * subdivisionSchemesGroup;
    QAction* loopAction;
    QAction* butterflyAction;
    QAction* catmullclarkAction;
    QAction* kobbeltAction;
    QAction* createCustomSchemeAction;

    QActionGroup * objectGroup;
    QAction* cubeObjectAction;
    QAction* tetrahedronObjectAction;
    QAction* openObjFileAction;

    MainOpenGLWidget* openglWidget;
};

#endif // MAINWINDOW_H

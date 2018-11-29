#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QFileDialog>
#include <fstream>
#include <iostream>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <QMessageBox>

#include "ui_mainwindow.h"

#include "MainOpenGLWidget.h"
#include "SubdivisionController.h"
#include "CustomSchemeHandler.h"
#include "customschemewindow.h"

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

    void on_actionSave_scheme_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    void setLabelSubdivision();

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

    CustomSchemeWindow* customSchemeWindow;

    QLabel* label;

};

#endif // MAINWINDOW_H

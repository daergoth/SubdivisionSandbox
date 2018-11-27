#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MainOpenGLWidget.h"

#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QFileDialog>
#include <fstream>
#include <iostream>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <CGAL/Polygon_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>


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
    QLabel* label;
};

#endif // MAINWINDOW_H

#ifndef CUSTOMSCHEMEWINDOW_H
#define CUSTOMSCHEMEWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QLineEdit>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>

#include "CustomSchemeHandler.h"

class QAction;
class QActionGroup;
class QMenu;

class CustomSchemeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CustomSchemeWindow(QWidget *parent = 0);
    ~CustomSchemeWindow();

private slots:
    void onTriggered_Approx();
    void onTriggered_Interp();
    void onTriggered_Tri();
    void onTriggered_Quad();
    void onTriggered_FirstNeighbours();
    void onTriggered_SecondNeighbours();

private:

    void createActions();
    void createMenus();

    QMenu* schemetypeMenu;
    QMenu* shapeMenu;
    QMenu* neighbourMenu;

    QActionGroup * schemeTypesGroup;
    QAction* approxAction;
    QAction* interpolAction;

    QActionGroup * shapeGroup;
    QAction* triAction;
    QAction* quadAction;

    QActionGroup * neighbourGroup;
    QAction* firstNeighbourAction;
    QAction* secondNeighbourAction;

    QVBoxLayout* sidebarVBox;
};

#endif // CUSTOMSCHEMEWINDOW_H

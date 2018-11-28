#ifndef CUSTOMSCHEMEWINDOW_H
#define CUSTOMSCHEMEWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QAction>
#include <QActionGroup>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>

#include "CustomSchemeHandler.h"

class QAction;
class QActionGroup;

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
    void onTriggered_okButton();
    void onTriggered_cancelButton();
    void updateWeightsLayout();
    int getNumberOfWeights();
    std::vector<QHBoxLayout*> getVectorOfLineEdits();

private:

    void createActions();

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
    QVBoxLayout* vBoxWeights;
    QWidget* container;
    QLabel* labelWeightsTitle;

    QGroupBox* groupBoxTypes;
    QGroupBox* groupBoxShapes;
    QGroupBox* groupBoxNeighbours;
    QRadioButton* approxRB;
    QRadioButton* interpolRB;
    QRadioButton* triRB;
    QRadioButton* quadRB;
    QRadioButton* firstnRB;
    QRadioButton* secondnRB;

    QPushButton* okButton;
    QPushButton* cancelButton;

    std::vector<QLineEdit> lineEdits;
};

#endif // CUSTOMSCHEMEWINDOW_H

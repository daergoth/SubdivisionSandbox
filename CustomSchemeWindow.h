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
#include <QDoubleValidator>

#include "CustomSchemeHandler.h"
#include "SubdivisionController.h"
#include "CustomScheme.h"

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
    void onTextChange(const QString &text);

private:

    void createActions();
    void updateWeightsLayout();
    void updateLineEditsDisableSetting();
    void setWeightGroupBoxes();

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

    QGroupBox* groupBoxTypes;
    QGroupBox* groupBoxShapes;
    QGroupBox* groupBoxNeighbours;
    QRadioButton* approxRB;
    QRadioButton* interpolRB;
    QRadioButton* triRB;
    QRadioButton* quadRB;
    QRadioButton* firstnRB;
    QRadioButton* secondnRB;

    QVBoxLayout* vBoxWeights;
    QWidget* container;
    QLabel* labelWeightsTitle;
    QLineEdit* leSchemeName;

    QGroupBox* groupBoxFaceWeights;
    QGroupBox* groupBoxOddWeights;
    QGroupBox* groupBoxEvenWeights;

    std::map<QLineEdit*,bool> mapLineEditsDisabled;

    QLineEdit* leFace0;
    QLineEdit* leFace1;

    QLineEdit* leOdd0;
    QLineEdit* leOdd1;

    QLineEdit* leEven0;
    QLineEdit* leEven1;

    QPushButton* okButton;
    QPushButton* cancelButton;
};

#endif // CUSTOMSCHEMEWINDOW_H

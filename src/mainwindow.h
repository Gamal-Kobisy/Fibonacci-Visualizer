#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "fibonaccitree.h"
#include "treevisualizer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void visualize();
    void onAnimationSpeedChanged(int value);
    void onStepChanged(int step, int total);
    void onAnimationFinished();

private:
    void setupUI();
    void updateStatistics();
    
    // UI Components
    QSpinBox* inputSpinBox;
    QPushButton* visualizeButton;
    QCheckBox* memoCheckBox;
    QSlider* speedSlider;
    QLabel* speedLabel;
    QLabel* statsLabel;
    QLabel* progressLabel;
    
    TreeVisualizer* treeVisualizer;
    FibonacciTree* fibTree;
    
    int animationSpeed;
};

#endif // MAINWINDOW_H

#ifndef TREEVISUALIZER_H
#define TREEVISUALIZER_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <unordered_map>
#include "fibonaccitree.h"

class TreeVisualizer : public QWidget {
    Q_OBJECT

public:
    explicit TreeVisualizer(QWidget *parent = nullptr);
    ~TreeVisualizer();
    
    void setTree(FibonacciTree* tree);
    void startAnimation(int delayMs = 500);
    void stopAnimation();
    void resetAnimation();
    bool isAnimating() const { return animating; }
    int getCurrentStep() const { return currentStep; }
    int getTotalSteps() const { return totalSteps; }
    
signals:
    void animationFinished();
    void stepChanged(int step, int total);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    
private slots:
    void animationStep();
    
private:
    void drawTree(QPainter& painter);
    void drawNode(QPainter& painter, TreeNode* node, int x, int y, int horizontalSpacing);
    void calculateNodePositions();
    int calculateSubtreeWidth(TreeNode* node);
    
    FibonacciTree* tree;
    QTimer* animationTimer;
    std::vector<TreeNode*> allNodes;
    std::unordered_map<TreeNode*, QPoint> nodePositions;
    
    bool animating;
    int currentStep;
    int totalSteps;
    int animationDelay;
    
    static const int NODE_RADIUS = 25;
    static const int VERTICAL_SPACING = 80;
    static const int MIN_HORIZONTAL_SPACING = 60;
};

#endif // TREEVISUALIZER_H

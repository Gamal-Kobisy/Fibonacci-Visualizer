#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QVariantAnimation>
#include <vector>
#include <unordered_map>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Data node for the call tree
struct FibNode {
    int n;
    long long value = 0;
    bool isCached = false;
    std::vector<FibNode*> children;
    double x = 0; // logical x (units)
    double y = 0; // pixel y
    int depth = 0;
    FibNode(int _n) : n(_n) {}
};

class NodeItem : public QGraphicsEllipseItem {
public:
    explicit NodeItem(FibNode* node, QGraphicsItem* parent = nullptr);

    FibNode* nodePtr;
    QGraphicsTextItem* label;
    QGraphicsTextItem* subLabel; // for value/extra

    void setHighlighted(bool on);
    void applyStyle();
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_btnDraw_clicked();
    void on_stepAnimation();
    void on_showNextNode();

    void on_btnStep_clicked();
    void on_btnSkip_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene = nullptr;
    std::vector<FibNode*> allNodes;
    std::unordered_map<FibNode*, NodeItem*> nodeItems;

    std::vector<QGraphicsLineItem*> edges;
    std::vector<std::pair<FibNode*, FibNode*>> edgePairs;
    std::unordered_map<FibNode*, FibNode*> parentMap;

    QTimer animTimer;        // kept for compatibility
    int animIndex = 0;
    std::vector<NodeItem*> visitOrder;

    QTimer nodesTimer;       // reveal timer
    int nodeRevealIndex = 0;

    int normalRevealInterval = 160; // ms
    int fastRevealInterval = 60;    // ms
    bool skipMode = false;

    // Build recursion trees
    FibNode* buildNaiveFib(int n);
    FibNode* buildMemoFib(int n, std::unordered_map<int, FibNode*>& memo);

    // Layout
    double computeWidths(FibNode* node, std::unordered_map<FibNode*, double>& widths);
    void assignPositions(FibNode* node, double &cursorX, int depth, double horizUnit, double vertGap);

    // Drawing & helpers
    void drawTree(FibNode* root);
    void clearSceneAndMemory();
    void collectVisitOrder(FibNode* node);

    unsigned long long fib_value_naive(int n);
    unsigned long long fib_value_dp(int n);

    void revealNodeAtIndex(size_t idx);
    void updateInfoForNode(FibNode* node, FibNode* parent);
    void updateStepSkipButtons();
    QString pathToRoot(FibNode* node);
};

#endif // MAINWINDOW_H

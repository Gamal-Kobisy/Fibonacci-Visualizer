#include "treevisualizer.h"
#include <QPainter>
#include <QResizeEvent>
#include <cmath>
#include <algorithm>

// Define static constants
const int TreeVisualizer::NODE_RADIUS;
const int TreeVisualizer::VERTICAL_SPACING;
const int TreeVisualizer::MIN_HORIZONTAL_SPACING;

TreeVisualizer::TreeVisualizer(QWidget *parent)
    : QWidget(parent), tree(nullptr), animationTimer(nullptr), 
      animating(false), currentStep(0), totalSteps(0), animationDelay(500) {
    
    setMinimumSize(800, 600);
    setStyleSheet("background-color: white;");
    
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &TreeVisualizer::animationStep);
}

TreeVisualizer::~TreeVisualizer() {
    stopAnimation();
}

void TreeVisualizer::setTree(FibonacciTree* t) {
    tree = t;
    resetAnimation();
    
    if (tree && tree->getRoot()) {
        allNodes = tree->getAllNodes();
        totalSteps = allNodes.size();
        calculateNodePositions();
    }
    
    update();
}

void TreeVisualizer::startAnimation(int delayMs) {
    if (!tree || !tree->getRoot()) return;
    
    animationDelay = delayMs;
    animating = true;
    currentStep = 0;
    
    animationTimer->start(animationDelay);
    emit stepChanged(currentStep, totalSteps);
}

void TreeVisualizer::stopAnimation() {
    animating = false;
    if (animationTimer) {
        animationTimer->stop();
    }
}

void TreeVisualizer::resetAnimation() {
    stopAnimation();
    currentStep = 0;
    allNodes.clear();
    nodePositions.clear();
    totalSteps = 0;
    update();
}

void TreeVisualizer::animationStep() {
    if (currentStep < totalSteps) {
        currentStep++;
        emit stepChanged(currentStep, totalSteps);
        update();
    } else {
        stopAnimation();
        emit animationFinished();
    }
}

void TreeVisualizer::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    if (tree && tree->getRoot()) {
        drawTree(painter);
    } else {
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, 
                        "Enter a number and click 'Visualize' to see the Fibonacci recursion tree");
    }
}

void TreeVisualizer::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    if (tree && tree->getRoot()) {
        calculateNodePositions();
    }
}

void TreeVisualizer::drawTree(QPainter& painter) {
    if (!tree || !tree->getRoot() || allNodes.empty()) return;
    
    // Draw edges first
    for (size_t i = 0; i < std::min(static_cast<size_t>(currentStep), allNodes.size()); ++i) {
        TreeNode* node = allNodes[i];
        
        if (nodePositions.find(node) == nodePositions.end()) continue;
        QPoint nodePos = nodePositions[node];
        
        // Draw edge to left child
        if (node->left && nodePositions.find(node->left) != nodePositions.end()) {
            QPoint leftPos = nodePositions[node->left];
            
            if (node->left->isMemoized) {
                painter.setPen(QPen(QColor(0, 150, 0), 2, Qt::DashLine));
            } else {
                painter.setPen(QPen(Qt::black, 2));
            }
            painter.drawLine(nodePos, leftPos);
        }
        
        // Draw edge to right child
        if (node->right && nodePositions.find(node->right) != nodePositions.end()) {
            QPoint rightPos = nodePositions[node->right];
            
            if (node->right->isMemoized) {
                painter.setPen(QPen(QColor(0, 150, 0), 2, Qt::DashLine));
            } else {
                painter.setPen(QPen(Qt::black, 2));
            }
            painter.drawLine(nodePos, rightPos);
        }
    }
    
    // Draw nodes
    for (size_t i = 0; i < std::min(static_cast<size_t>(currentStep), allNodes.size()); ++i) {
        TreeNode* node = allNodes[i];
        
        if (nodePositions.find(node) == nodePositions.end()) continue;
        QPoint pos = nodePositions[node];
        
        drawNode(painter, node, pos.x(), pos.y(), 0);
    }
}

void TreeVisualizer::drawNode(QPainter& painter, TreeNode* node, int x, int y, int horizontalSpacing) {
    Q_UNUSED(horizontalSpacing);
    
    // Choose color based on node state
    QColor nodeColor;
    if (node->isMemoized) {
        nodeColor = QColor(144, 238, 144); // Light green for memoized
    } else if (node->value <= 1) {
        nodeColor = QColor(173, 216, 230); // Light blue for base case
    } else if (node->isComputed) {
        nodeColor = QColor(255, 218, 185); // Peach for computed
    } else {
        nodeColor = QColor(220, 220, 220); // Gray for not computed
    }
    
    // Draw node circle
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(nodeColor);
    painter.drawEllipse(QPoint(x, y), NODE_RADIUS, NODE_RADIUS);
    
    // Draw node value
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);
    
    QString text = QString("F(%1)").arg(node->value);
    QRect textRect(x - NODE_RADIUS, y - NODE_RADIUS/2, NODE_RADIUS * 2, NODE_RADIUS);
    painter.drawText(textRect, Qt::AlignCenter, text);
    
    // Draw result if computed
    if (node->isComputed) {
        font.setBold(false);
        font.setPointSize(9);
        painter.setFont(font);
        QString resultText = QString("=%1").arg(node->result);
        QRect resultRect(x - NODE_RADIUS, y + 5, NODE_RADIUS * 2, NODE_RADIUS);
        painter.drawText(resultRect, Qt::AlignCenter, resultText);
    }
}

void TreeVisualizer::calculateNodePositions() {
    nodePositions.clear();
    
    if (!tree || !tree->getRoot()) return;
    
    TreeNode* root = tree->getRoot();
    int rootX = width() / 2;
    int rootY = 50;
    
    // Calculate the width needed for the tree
    int subtreeWidth = calculateSubtreeWidth(root);
    
    // Use a queue for level-order traversal with position calculation
    struct NodeWithPos {
        TreeNode* node;
        int x;
        int y;
        int spacing;
    };
    
    std::vector<NodeWithPos> queue;
    queue.push_back({root, rootX, rootY, subtreeWidth / 2});
    
    for (size_t i = 0; i < queue.size(); ++i) {
        NodeWithPos current = queue[i];
        nodePositions[current.node] = QPoint(current.x, current.y);
        
        int childY = current.y + VERTICAL_SPACING;
        int childSpacing = std::max(MIN_HORIZONTAL_SPACING, current.spacing / 2);
        
        if (current.node->left) {
            int leftX = current.x - childSpacing;
            queue.push_back({current.node->left, leftX, childY, childSpacing});
        }
        
        if (current.node->right) {
            int rightX = current.x + childSpacing;
            queue.push_back({current.node->right, rightX, childY, childSpacing});
        }
    }
}

int TreeVisualizer::calculateSubtreeWidth(TreeNode* node) {
    if (!node) return 0;
    
    if (!node->left && !node->right) {
        return MIN_HORIZONTAL_SPACING;
    }
    
    int leftWidth = calculateSubtreeWidth(node->left);
    int rightWidth = calculateSubtreeWidth(node->right);
    
    return leftWidth + rightWidth + MIN_HORIZONTAL_SPACING;
}

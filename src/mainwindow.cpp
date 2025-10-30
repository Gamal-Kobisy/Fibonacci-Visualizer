#include "mainwindow.h"
#include <QGroupBox>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), fibTree(nullptr), animationSpeed(500) {
    
    setupUI();
    setWindowTitle("Fibonacci Recursion Tree Visualizer");
    resize(1200, 800);
}

MainWindow::~MainWindow() {
    if (fibTree) {
        delete fibTree;
    }
}

void MainWindow::setupUI() {
    // Central widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Control panel
    QGroupBox* controlGroup = new QGroupBox("Controls", this);
    QHBoxLayout* controlLayout = new QHBoxLayout(controlGroup);
    
    // Input controls
    QLabel* inputLabel = new QLabel("Fibonacci N:", this);
    inputSpinBox = new QSpinBox(this);
    inputSpinBox->setMinimum(0);
    inputSpinBox->setMaximum(15);
    inputSpinBox->setValue(5);
    inputSpinBox->setToolTip("Enter a number between 0 and 15");
    
    // Memoization checkbox
    memoCheckBox = new QCheckBox("Use Memoization", this);
    memoCheckBox->setToolTip("Enable to see how memoization reduces redundant calculations");
    
    // Visualize button
    visualizeButton = new QPushButton("Visualize", this);
    visualizeButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; "
                                   "padding: 8px 16px; font-weight: bold; border-radius: 4px; }"
                                   "QPushButton:hover { background-color: #45a049; }");
    connect(visualizeButton, &QPushButton::clicked, this, &MainWindow::visualize);
    
    // Animation speed controls
    QLabel* speedTextLabel = new QLabel("Animation Speed:", this);
    speedLabel = new QLabel("Normal", this);
    speedLabel->setMinimumWidth(60);
    
    speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setMinimum(1);
    speedSlider->setMaximum(10);
    speedSlider->setValue(5);
    speedSlider->setTickPosition(QSlider::TicksBelow);
    speedSlider->setTickInterval(1);
    speedSlider->setToolTip("Adjust animation speed (1=Slow, 10=Fast)");
    connect(speedSlider, &QSlider::valueChanged, this, &MainWindow::onAnimationSpeedChanged);
    
    // Add controls to layout
    controlLayout->addWidget(inputLabel);
    controlLayout->addWidget(inputSpinBox);
    controlLayout->addWidget(memoCheckBox);
    controlLayout->addWidget(visualizeButton);
    controlLayout->addSpacing(20);
    controlLayout->addWidget(speedTextLabel);
    controlLayout->addWidget(speedSlider);
    controlLayout->addWidget(speedLabel);
    controlLayout->addStretch();
    
    mainLayout->addWidget(controlGroup);
    
    // Statistics panel
    QGroupBox* statsGroup = new QGroupBox("Statistics", this);
    QVBoxLayout* statsLayout = new QVBoxLayout(statsGroup);
    
    statsLabel = new QLabel("Waiting for visualization...", this);
    statsLabel->setWordWrap(true);
    statsLayout->addWidget(statsLabel);
    
    progressLabel = new QLabel("", this);
    progressLabel->setStyleSheet("QLabel { color: #2196F3; font-weight: bold; }");
    statsLayout->addWidget(progressLabel);
    
    mainLayout->addWidget(statsGroup);
    
    // Tree visualizer
    treeVisualizer = new TreeVisualizer(this);
    connect(treeVisualizer, &TreeVisualizer::stepChanged, this, &MainWindow::onStepChanged);
    connect(treeVisualizer, &TreeVisualizer::animationFinished, this, &MainWindow::onAnimationFinished);
    
    mainLayout->addWidget(treeVisualizer, 1);
    
    // Legend
    QGroupBox* legendGroup = new QGroupBox("Legend", this);
    QHBoxLayout* legendLayout = new QHBoxLayout(legendGroup);
    
    auto addLegendItem = [&](const QString& color, const QString& text) {
        QLabel* colorBox = new QLabel(this);
        colorBox->setFixedSize(20, 20);
        colorBox->setStyleSheet(QString("background-color: %1; border: 1px solid black;").arg(color));
        QLabel* label = new QLabel(text, this);
        legendLayout->addWidget(colorBox);
        legendLayout->addWidget(label);
        legendLayout->addSpacing(15);
    };
    
    addLegendItem("#ADD8E6", "Base Case (F(0), F(1))");
    addLegendItem("#FFDAB9", "Computed Node");
    addLegendItem("#90EE90", "Memoized (Cached Result)");
    legendLayout->addStretch();
    
    mainLayout->addWidget(legendGroup);
}

void MainWindow::visualize() {
    int n = inputSpinBox->value();
    bool useMemo = memoCheckBox->isChecked();
    
    // Stop any ongoing animation
    if (treeVisualizer->isAnimating()) {
        treeVisualizer->stopAnimation();
    }
    
    // Clean up old tree
    if (fibTree) {
        delete fibTree;
    }
    
    // Create new tree
    fibTree = new FibonacciTree();
    fibTree->buildTree(n, useMemo);
    
    // Update visualizer
    treeVisualizer->setTree(fibTree);
    updateStatistics();
    
    // Start animation
    treeVisualizer->startAnimation(animationSpeed);
    
    // Disable button during animation
    visualizeButton->setEnabled(false);
}

void MainWindow::onAnimationSpeedChanged(int value) {
    // Map slider value (1-10) to delay (1000-100ms)
    // Higher slider value = faster animation = lower delay
    animationSpeed = 1100 - (value * 100);
    
    QString speedText;
    if (value <= 3) {
        speedText = "Slow";
    } else if (value <= 7) {
        speedText = "Normal";
    } else {
        speedText = "Fast";
    }
    speedLabel->setText(speedText);
}

void MainWindow::onStepChanged(int step, int total) {
    progressLabel->setText(QString("Animation Progress: %1 / %2 nodes")
                          .arg(step).arg(total));
}

void MainWindow::onAnimationFinished() {
    progressLabel->setText(QString("Animation Complete! Showing all %1 nodes")
                          .arg(treeVisualizer->getTotalSteps()));
    visualizeButton->setEnabled(true);
}

void MainWindow::updateStatistics() {
    if (!fibTree || !fibTree->getRoot()) {
        statsLabel->setText("No tree to display");
        return;
    }
    
    TreeNode* root = fibTree->getRoot();
    int totalCalls = fibTree->getTotalCalls();
    int memoizedCalls = fibTree->getMemoizedCalls();
    
    QString statsText;
    statsText += QString("Computing F(%1) = %2\n").arg(root->value).arg(root->result);
    statsText += QString("Total function calls: %1\n").arg(totalCalls);
    
    if (memoCheckBox->isChecked()) {
        int savedCalls = memoizedCalls;
        statsText += QString("Memoized calls (reused results): %1\n").arg(savedCalls);
        statsText += QString("Unique computations: %1\n").arg(totalCalls - savedCalls);
        if (totalCalls > 0) {
            double efficiency = (double)savedCalls / totalCalls * 100.0;
            statsText += QString("Efficiency gain: %1%").arg(efficiency, 0, 'f', 1);
        }
    } else {
        // Calculate what it would be with memoization for comparison
        int n = root->value;
        int wouldBeCalls = (n + 1) * 2; // Approximation for memoized calls
        if (totalCalls > wouldBeCalls) {
            statsText += QString("With memoization, this would need only ~%1 calls\n").arg(wouldBeCalls);
            statsText += QString("Potential efficiency gain: ~%1%")
                        .arg((double)(totalCalls - wouldBeCalls) / totalCalls * 100.0, 0, 'f', 1);
        }
    }
    
    statsLabel->setText(statsText);
}

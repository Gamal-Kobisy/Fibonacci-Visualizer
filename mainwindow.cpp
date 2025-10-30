#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGraphicsLineItem>
#include <QFont>
#include <QWheelEvent>
#include <QTextEdit>
#include <QLinearGradient>
#include <QStringList>
#include <cmath>

// Tunable geometry
static const double H_GAP = 110.0; // pixel per logical x unit (horizontal spacing)
static const double V_GAP = 100.0; // vertical gap in pixels

// ---------------- NodeItem ----------------
NodeItem::NodeItem(FibNode* node, QGraphicsItem* parent)
    : QGraphicsEllipseItem(parent), nodePtr(node) {
    // rounded rect centered at (0,0)
    setRect(-50, -28, 100, 56);
    // modern off-white pen for subtle border
    setPen(QPen(QColor(240, 244, 249), 1));
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsScenePositionChanges);

    // label (main: F(n))
    label = new QGraphicsTextItem(this);
    label->setDefaultTextColor(QColor(250,250,252));
    label->setFont(QFont("Segoe UI", 10, QFont::Bold));
    label->setPos(-15, -25);

    // subLabel (value)
    subLabel = new QGraphicsTextItem(this);
    subLabel->setDefaultTextColor(QColor(200, 220, 235));
    subLabel->setFont(QFont("Segoe UI", 8));
    subLabel->setPos(-46, -4);

    applyStyle();
}

void NodeItem::applyStyle() {
    // modern purple -> teal gradient for nodes
    QLinearGradient grad(rect().topLeft(), rect().bottomRight());
    grad.setColorAt(0.0, QColor(51, 65, 151));   // deep indigo (#334197)
    grad.setColorAt(1.0, QColor(20, 184, 166));  // teal-accent (#14B8A6)
    setBrush(QBrush(grad));
}

void NodeItem::setHighlighted(bool on) {
    if (on) {
        // warm accent gradient for selection (coral -> amber)
        QLinearGradient grad(rect().topLeft(), rect().bottomRight());
        grad.setColorAt(0.0, QColor(255, 94, 98));
        grad.setColorAt(1.0, QColor(255, 159, 67));
        setBrush(grad);
    } else {
        if (nodePtr->isCached) {
            // muted slate gradient for cached nodes
            QLinearGradient grad(rect().topLeft(), rect().bottomRight());
            grad.setColorAt(0.0, QColor(95, 105, 125));
            grad.setColorAt(1.0, QColor(65, 75, 95));
            setBrush(grad);
            setPen(QPen(QColor(240, 244, 249), 1));
        } else {
            applyStyle();
        }
    }
}

// ---------------- MainWindow ----------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    // intercept wheel events for zoom (install on viewport)
    ui->graphicsView->viewport()->installEventFilter(this);

    // Hook up signals
    connect(ui->btnDraw, &QPushButton::clicked, this, &MainWindow::on_btnDraw_clicked);
    connect(&animTimer, &QTimer::timeout, this, &MainWindow::on_stepAnimation);
    connect(&nodesTimer, &QTimer::timeout, this, &MainWindow::on_showNextNode);
    connect(ui->btnSkip, &QPushButton::clicked, this, &MainWindow::on_btnSkip_clicked);

    // sensible defaults
    ui->spinBoxN->setRange(0, 40);
    ui->spinBoxN->setValue(6);
    ui->radioNaive->setChecked(true);

    // info area read-only + modern styling
    ui->infoText->setReadOnly(true);
    ui->infoText->setFont(QFont("Segoe UI", 16));
    ui->infoText->setStyleSheet("QTextEdit{ background: #0f1724; color: #E6EEF8; border-radius:8px; padding:8px; }");
    ui->infoText->setPlainText("Press Draw to build the tree. Use Step to reveal one node (root appears first), Skip to reveal the rest quickly.");

    // modernize buttons/colors via stylesheet
    QString btnStyle = R"(
        QPushButton {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #3A7BD5, stop:1 #00D2FF);
            color: white;
            border-radius:8px;
            padding:8px 14px;
            font-weight:600;
        }
        QPushButton:disabled { background: rgb(110, 115, 125); color: #d6d6d6; }
        QPushButton:hover { opacity: 0.95; }
        QRadioButton, QCheckBox { padding:4px; color: #E6EEF8; }
    )";
    ui->btnDraw->setStyleSheet(btnStyle);
    ui->btnStep->setStyleSheet(btnStyle);
    ui->btnSkip->setStyleSheet(btnStyle);

    // initially disable step/skip
    ui->btnStep->setEnabled(false);
    ui->btnSkip->setEnabled(false);
}

MainWindow::~MainWindow() {
    clearSceneAndMemory();
    delete ui;
}

// ---------------- build trees ----------------
FibNode* MainWindow::buildNaiveFib(int n) {
    FibNode* root = new FibNode(n);
    allNodes.push_back(root);
    if (n <= 1) {
        root->value = int(n);
        return root;
    }
    root->children.push_back(buildNaiveFib(n-1));
    root->children.push_back(buildNaiveFib(n-2));
    root->value = root->children[0]->value + root->children[1]->value;
    return root;
}

FibNode* MainWindow::buildMemoFib(int n, std::unordered_map<int, FibNode*>& memo) {
    auto it = memo.find(n);
    if (it != memo.end()) {
        FibNode* leaf = new FibNode(n);
        leaf->isCached = true;
        leaf->value = it->second->value;
        allNodes.push_back(leaf);
        return leaf;
    }

    FibNode* root = new FibNode(n);
    allNodes.push_back(root);
    if (n <= 1) {
        root->value = static_cast<unsigned long long>(n);
        memo[n] = root;
        return root;
    }
    FibNode* left = buildMemoFib(n-1, memo);
    FibNode* right = buildMemoFib(n-2, memo);
    root->children.push_back(left);
    root->children.push_back(right);
    root->value = left->value + right->value;
    memo[n] = root;
    return root;
}

// ---------------- layout ----------------
double MainWindow::computeWidths(FibNode* node, std::unordered_map<FibNode*, double>& widths) {
    if (!node) return 0.0;
    if (node->children.empty()) {
        widths[node] = 1.0;
        return 1.0;
    }
    double sum = 0.0;
    for (auto c : node->children) sum += computeWidths(c, widths);
    widths[node] = sum;
    return sum;
}

void MainWindow::assignPositions(FibNode* node, double &cursorX, int depth, double horizUnit, double vertGap) {
    if (!node) return;
    node->depth = depth;
    if (node->children.empty()) {
        node->x = cursorX;         // logical coordinate
        node->y = depth * vertGap; // pixel y
        cursorX += 1.0;            // advance leaf slot
        return;
    }
    for (auto c : node->children) assignPositions(c, cursorX, depth+1, horizUnit, vertGap);
    double left = node->children.front()->x;
    double right = node->children.back()->x;
    node->x = (left + right) / 2.0;
    node->y = depth * vertGap;
}

// ---------------- draw & helpers ----------------
void MainWindow::drawTree(FibNode* root) {
    scene->clear();
    nodeItems.clear();
    edges.clear();
    edgePairs.clear();
    parentMap.clear();

    for (FibNode* c : allNodes) {
        NodeItem* it = new NodeItem(c);
        it->label->setPlainText(QString("F(%1)").arg(c->n));
        it->subLabel->setPlainText(QString("= %1").arg(c->value));
        double px = c->x * H_GAP;
        double py = c->y;
        it->setPos(px, py);
        it->setVisible(false);   // start hidden, will reveal one by one
        it->setScale(0.2);       // small initial scale for a pop-in effect
        scene->addItem(it);
        nodeItems[c] = it;
    }

    // draw edges but hide them initially
    for (FibNode* p : allNodes) {
        for (FibNode* ch : p->children) {
            NodeItem* pit = nodeItems[p];
            NodeItem* cit = nodeItems[ch];
            if (!pit || !cit) continue;
            QGraphicsLineItem* line = scene->addLine(QLineF(pit->pos(), cit->pos()));
            line->setZValue(-1);
            line->setVisible(false);
            if (ch->isCached) {
                QPen pen = line->pen();
                pen.setStyle(Qt::DashLine);
                line->setPen(pen);
            }
            edges.push_back(line);
            edgePairs.push_back({p, ch});
            // store a parent mapping for the first parent seen
            if (parentMap.find(ch) == parentMap.end()) parentMap[ch] = p;
        }
    }

    // collect visit order (preorder) for reveal; could be any order you prefer
    visitOrder.clear();
    collectVisitOrder(root);

    // Prepare reveal state: if animate is checked, show root only and wait for Step clicks
    nodeRevealIndex = 0;
    skipMode = false;
    nodesTimer.stop();
    nodesTimer.setInterval(normalRevealInterval);

    if (ui->checkBoxAnimate->isChecked() && !visitOrder.empty()) {
        // reveal only root immediately (visitOrder[0]) and leave others hidden
        NodeItem* rootItem = visitOrder[0];
        rootItem->setVisible(true);
        rootItem->setScale(1.0);
        // make sure cached nodes styled
        rootItem->setHighlighted(false);

        // show edges only when both endpoints become visible (none yet)
        nodeRevealIndex = 1; // next reveal will be visitOrder[1]

        ui->btnStep->setEnabled(true);
        ui->btnSkip->setEnabled(true);
    } else {
        // reveal all immediately with a small scale effect
        for (auto &kv : nodeItems) {
            kv.second->setVisible(true);
            kv.second->setScale(1.0);
        }
        // show edges once both endpoints visible
        for (size_t i = 0; i < edges.size(); ++i) {
            FibNode* p = edgePairs[i].first;
            FibNode* c = edgePairs[i].second;
            if (nodeItems[p]->isVisible() && nodeItems[c]->isVisible()) edges[i]->setVisible(true);
        }
        ui->btnStep->setEnabled(false);
        ui->btnSkip->setEnabled(false);
    }

    // update info to mention root if visible
    if (!visitOrder.empty()) {
        FibNode* rootN = visitOrder[0]->nodePtr;
        updateInfoForNode(rootN, nullptr);
    }
}

void MainWindow::collectVisitOrder(FibNode* node) {
    if (!node) return;
    auto it = nodeItems.find(node);
    if (it != nodeItems.end()) visitOrder.push_back(it->second);
    for (auto c : node->children) collectVisitOrder(c);
}

void MainWindow::clearSceneAndMemory() {
    nodesTimer.stop();
    animTimer.stop();
    scene->clear();
    for (FibNode* p : allNodes) delete p;
    allNodes.clear();
    nodeItems.clear();
    edges.clear();
    edgePairs.clear();
    parentMap.clear();
    visitOrder.clear();
    nodeRevealIndex = 0;
    ui->btnStep->setEnabled(false);
    ui->btnSkip->setEnabled(false);
}

// ---------------- animation step for visit-highlighting (kept for compatibility) ----------------
void MainWindow::on_stepAnimation() {
    if (animIndex < visitOrder.size()) {
        visitOrder[animIndex]->setHighlighted(true);
        ui->graphicsView->centerOn(visitOrder[animIndex]);
        ++animIndex;
    } else {
        animTimer.stop();
    }
}

// ---------------- show nodes one by one (cool pop-in) ----------------
void MainWindow::on_showNextNode() {
    if (nodeRevealIndex >= visitOrder.size()) {
        nodesTimer.stop();
        ui->btnStep->setEnabled(false);
        ui->btnSkip->setEnabled(false);
        skipMode = false;
        return;
    }

    NodeItem* it = visitOrder[nodeRevealIndex++];
    // reveal node
    it->setVisible(true);

    // animate scale from small -> 1.0 for a pop effect
    QVariantAnimation *anim = new QVariantAnimation(this);
    anim->setDuration(220);
    anim->setStartValue(0.25);
    anim->setEndValue(1.0);
    // when anim value changes, update scale
    connect(anim, &QVariantAnimation::valueChanged, this, [it](const QVariant &v) {
        it->setScale(v.toDouble());
    });
    // delete animator when done
    connect(anim, &QVariantAnimation::finished, anim, &QObject::deleteLater);
    anim->start();

    // show edges that connect visible parent-child pairs
    for (size_t i = 0; i < edges.size(); ++i) {
        FibNode* p = edgePairs[i].first;
        FibNode* c = edgePairs[i].second;
        if (nodeItems[p]->isVisible() && nodeItems[c]->isVisible()) {
            edges[i]->setVisible(true);
        }
    }


    // update info text about this revealed node
    FibNode* thisNode = it->nodePtr;
    FibNode* parentNode = nullptr;
    auto pit = parentMap.find(thisNode);
    if (pit != parentMap.end()) parentNode = pit->second;
    updateInfoForNode(thisNode, parentNode);

    // if we've revealed all, disable step/skip
    updateStepSkipButtons();
}

// ---------------- helper to update info text ----------------
QString MainWindow::pathToRoot(FibNode* node) {
    QStringList parts;
    FibNode* cur = node;
    while (cur) {
        parts.prepend(QString("F(%1)").arg(cur->n));
        auto it = parentMap.find(cur);
        if (it == parentMap.end()) break;
        cur = it->second;
    }
    return parts.join(" -> ");
}

void MainWindow::updateInfoForNode(FibNode* node, FibNode* parent) {
    QString s;
    s += QString("Node: F(%1)\n").arg(node->n);
    s += QString("Value: %1\n").arg(node->value);
    s += QString("Depth: %1\n").arg(node->depth);
    s += QString("Cached: %1\n").arg(node->isCached ? "Yes" : "No");
    s += QString("Children: %1\n").arg((int)node->children.size());
    if (parent) s += QString("Parent: F(%1)\n").arg(parent->n);
    else s += QString("Parent: (root)\n");

    // Add a helpful path from root to this node
    s += QString("Path from root: %1\n").arg(pathToRoot(node));

    s += QString("\nNodes revealed: %1 / %2\n").arg((int)nodeRevealIndex).arg((int)visitOrder.size());

    // Provide a tiny performance hint for memo mode
    if (ui->radioMemo->isChecked()) {
        s += "\nNote: memoized mode uses cached subcalls.\n";
    }

    ui->infoText->setPlainText(s);
}

// ---------------- update Step/Skip button states ----------------
void MainWindow::updateStepSkipButtons() {
    bool hasRemaining = nodeRevealIndex < visitOrder.size();
    ui->btnStep->setEnabled(hasRemaining);
    ui->btnSkip->setEnabled(hasRemaining);
}

// ---------------- event filter for wheel (zoom) ----------------
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    // We installed the filter on the graphicsView viewport
    if (watched == ui->graphicsView->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *we = static_cast<QWheelEvent*>(event);
        QPoint numDegrees = we->angleDelta() / 8;
        QPoint numSteps = numDegrees / 15;
        int deltaSteps = numSteps.y();

        if (deltaSteps != 0) {
            const double factor = std::pow(1.125, deltaSteps);
            ui->graphicsView->scale(factor, factor);
            return true; // we've handled it
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

// ---------------- UI slot: Draw ----------------
void MainWindow::on_btnDraw_clicked() {
    clearSceneAndMemory();
    ui->infoText->clear();

    int n = ui->spinBoxN->value();
    bool isNaive = ui->radioNaive->isChecked();

    // Safety
    if (isNaive && n > 12) {
        QMessageBox::warning(this, "Too large",
                             "Naive recursion tree grows very fast. Please choose n <= 12 or use Memoized mode.");
        return;
    }

    FibNode* root = nullptr;
    if (isNaive) {
        root = buildNaiveFib(n);
    } else {
        std::unordered_map<int, FibNode*> memo;
        root = buildMemoFib(n, memo);
    }

    // layout
    std::unordered_map<FibNode*, double> widths;
    computeWidths(root, widths);
    double cursor = 0.0;
    assignPositions(root, cursor, 0, 1.0, V_GAP);

    drawTree(root);

    // fit view to content (if any)
    if (!scene->items().isEmpty()) {
        ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}

// ---------------- Step: reveal a single node ----------------
void MainWindow::on_btnStep_clicked() {
    // stop any running auto reveal
    nodesTimer.stop();
    skipMode = false;
    nodesTimer.setInterval(normalRevealInterval);
    on_showNextNode();
}

// ---------------- Skip: reveal all remaining (fast animation) ----------------
void MainWindow::on_btnSkip_clicked() {
    if (nodeRevealIndex >= visitOrder.size()) return;
    skipMode = true;
    nodesTimer.setInterval(fastRevealInterval);
    nodesTimer.start();
    // keep buttons enabled while running; they will be disabled when finished
}

// ---------------- small fib helpers ----------------
unsigned long long MainWindow::fib_value_naive(int n) {
    if (n <= 1) return n;
    return fib_value_naive(n-1) + fib_value_naive(n-2);
}

unsigned long long MainWindow::fib_value_dp(int n) {
    if (n <= 1) return n;
    unsigned long long a = 0, b = 1;
    for (int i = 2; i <= n; ++i) { unsigned long long c = a + b; a = b; b = c; }
    return b;
}

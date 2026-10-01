/* ================================================================
   SALES TRACKING SYSTEM — Complete Qt C++ Project
   Files included (clearly separated):
     1. main.cpp
     2. mainwindow.h
     3. mainwindow.cpp
   ================================================================ */


/* ================================================================
   FILE 1: main.cpp
   ================================================================ */
/*

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    MainWindow window;
    window.setWindowTitle("Sales Tracking System");
    window.resize(1000, 680);
    window.show();

    return app.exec();
}

*/


/* ================================================================
   FILE 2: mainwindow.h
   ================================================================ */
/*

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QVector>
#include <QDate>
#include <QString>

// ── Data structure for one sale record ──────────────────────────
struct Sale {
    int     id;
    QString date;
    QString product;
    QString category;
    int     quantity;
    double  unitPrice;
    double  totalAmount;
    QString status;     // "Completed", "Pending", "Cancelled"
};

// ── MainWindow ───────────────────────────────────────────────────
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onAddSale();
    void onDeleteSale();
    void onClearForm();
    void onTableRowSelected();
    void onSearchChanged(const QString &text);
    void onFilterCategory(const QString &cat);
    void refreshSummary();

private:
    // ── Form widgets ──────────────────────────────────────────────
    QDateEdit       *dateEdit;
    QLineEdit       *productInput;
    QComboBox       *categoryCombo;
    QSpinBox        *quantityInput;
    QDoubleSpinBox  *priceInput;
    QComboBox       *statusCombo;

    // ── Search / filter ───────────────────────────────────────────
    QLineEdit       *searchInput;
    QComboBox       *filterCombo;

    // ── Buttons ───────────────────────────────────────────────────
    QPushButton     *addButton;
    QPushButton     *deleteButton;
    QPushButton     *clearButton;

    // ── Table ─────────────────────────────────────────────────────
    QTableWidget    *salesTable;

    // ── Summary labels ────────────────────────────────────────────
    QLabel          *totalSalesLabel;
    QLabel          *totalRevenueLabel;
    QLabel          *pendingLabel;
    QLabel          *topProductLabel;

    // ── Data ──────────────────────────────────────────────────────
    QVector<Sale>   sales;
    int             nextId;

    // ── Helpers ───────────────────────────────────────────────────
    void setupUI();
    void setupStyleSheet();
    void loadSampleData();
    void populateTable(const QVector<Sale> &data);
    QVector<Sale> applyFilters();
    QLabel* makeSummaryCard(const QString &title, QLabel *&valueLabel,
                            const QString &color);
};

#endif // MAINWINDOW_H

*/


/* ================================================================
   FILE 3: mainwindow.cpp
   ================================================================ */
/*

#include "mainwindow.h"
#include <QFont>
#include <QFrame>
#include <QSplitter>
#include <QScrollArea>

// ════════════════════════════════════════════════════════════════
//  Constructor
// ════════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), nextId(1)
{
    setupUI();
    setupStyleSheet();
    loadSampleData();
    populateTable(sales);
    refreshSummary();
}

// ════════════════════════════════════════════════════════════════
//  UI SETUP
// ════════════════════════════════════════════════════════════════
void MainWindow::setupUI() {

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    // ── HEADER ────────────────────────────────────────────────────
    QLabel *header = new QLabel("Sales Tracking System");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    // ── SUMMARY CARDS ─────────────────────────────────────────────
    QHBoxLayout *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(12);

    auto makeCard = [&](const QString &title, QLabel *&valueRef,
                        const QString &color) -> QFrame* {
        QFrame *card = new QFrame();
        card->setObjectName("summaryCard");
        card->setStyleSheet(QString("QFrame#summaryCard { border-left: 4px solid %1; }").arg(color));
        QVBoxLayout *cl = new QVBoxLayout(card);
        cl->setContentsMargins(14, 10, 14, 10);
        QLabel *titleLbl = new QLabel(title);
        titleLbl->setObjectName("cardTitle");
        valueRef = new QLabel("0");
        valueRef->setObjectName("cardValue");
        valueRef->setStyleSheet(QString("color: %1;").arg(color));
        cl->addWidget(titleLbl);
        cl->addWidget(valueRef);
        return card;
    };

    cardsLayout->addWidget(makeCard("Total Sales",      totalSalesLabel,   "#2196F3"));
    cardsLayout->addWidget(makeCard("Total Revenue",    totalRevenueLabel, "#4CAF50"));
    cardsLayout->addWidget(makeCard("Pending Orders",   pendingLabel,      "#FF9800"));
    cardsLayout->addWidget(makeCard("Top Product",      topProductLabel,   "#9C27B0"));
    mainLayout->addLayout(cardsLayout);

    // ── CONTENT AREA: form (left) + table (right) ─────────────────
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(14);

    // ── LEFT: ADD SALE FORM ───────────────────────────────────────
    QGroupBox *formBox = new QGroupBox("Add New Sale");
    formBox->setObjectName("formBox");
    formBox->setFixedWidth(270);
    QGridLayout *formGrid = new QGridLayout(formBox);
    formGrid->setVerticalSpacing(10);
    formGrid->setHorizontalSpacing(8);

    auto makeLabel = [](const QString &t) {
        QLabel *l = new QLabel(t);
        l->setObjectName("formLabel");
        return l;
    };

    // Date
    formGrid->addWidget(makeLabel("Date:"), 0, 0);
    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    formGrid->addWidget(dateEdit, 0, 1);

    // Product
    formGrid->addWidget(makeLabel("Product:"), 1, 0);
    productInput = new QLineEdit();
    productInput->setPlaceholderText("Product name...");
    formGrid->addWidget(productInput, 1, 1);

    // Category
    formGrid->addWidget(makeLabel("Category:"), 2, 0);
    categoryCombo = new QComboBox();
    categoryCombo->addItems({"Electronics", "Clothing", "Food & Beverage",
                              "Home & Garden", "Sports", "Books", "Other"});
    formGrid->addWidget(categoryCombo, 2, 1);

    // Quantity
    formGrid->addWidget(makeLabel("Quantity:"), 3, 0);
    quantityInput = new QSpinBox();
    quantityInput->setRange(1, 9999);
    quantityInput->setValue(1);
    formGrid->addWidget(quantityInput, 3, 1);

    // Unit Price
    formGrid->addWidget(makeLabel("Unit Price (PHP):"), 4, 0);
    priceInput = new QDoubleSpinBox();
    priceInput->setRange(0.01, 9999999.99);
    priceInput->setDecimals(2);
    priceInput->setValue(100.00);
    priceInput->setPrefix("₱ ");
    formGrid->addWidget(priceInput, 4, 1);

    // Status
    formGrid->addWidget(makeLabel("Status:"), 5, 0);
    statusCombo = new QComboBox();
    statusCombo->addItems({"Completed", "Pending", "Cancelled"});
    formGrid->addWidget(statusCombo, 5, 1);

    // Buttons
    addButton = new QPushButton("+ Add Sale");
    addButton->setObjectName("addButton");
    clearButton = new QPushButton("Clear Form");
    clearButton->setObjectName("clearButton");
    deleteButton = new QPushButton("Delete Selected");
    deleteButton->setObjectName("deleteButton");

    formGrid->addWidget(addButton,    6, 0, 1, 2);
    formGrid->addWidget(clearButton,  7, 0, 1, 2);
    formGrid->addWidget(deleteButton, 8, 0, 1, 2);

    contentLayout->addWidget(formBox);

    // ── RIGHT: SEARCH + TABLE ─────────────────────────────────────
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(8);

    // Search / filter bar
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLabel *searchIcon = new QLabel("Search:");
    searchIcon->setObjectName("formLabel");
    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Search by product name...");
    searchInput->setClearButtonEnabled(true);

    QLabel *filterIcon = new QLabel("Category:");
    filterIcon->setObjectName("formLabel");
    filterCombo = new QComboBox();
    filterCombo->addItems({"All Categories", "Electronics", "Clothing",
                            "Food & Beverage", "Home & Garden",
                            "Sports", "Books", "Other"});

    searchLayout->addWidget(searchIcon);
    searchLayout->addWidget(searchInput, 2);
    searchLayout->addWidget(filterIcon);
    searchLayout->addWidget(filterCombo, 1);
    rightLayout->addLayout(searchLayout);

    // Sales table
    salesTable = new QTableWidget(0, 8);
    salesTable->setObjectName("salesTable");
    salesTable->setHorizontalHeaderLabels({
        "ID", "Date", "Product", "Category",
        "Qty", "Unit Price", "Total", "Status"
    });
    salesTable->horizontalHeader()->setStretchLastSection(true);
    salesTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    salesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    salesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    salesTable->setAlternatingRowColors(true);
    salesTable->verticalHeader()->setVisible(false);
    salesTable->setColumnWidth(0, 45);
    salesTable->setColumnWidth(1, 95);
    salesTable->setColumnWidth(3, 110);
    salesTable->setColumnWidth(4, 45);
    salesTable->setColumnWidth(5, 100);
    salesTable->setColumnWidth(6, 100);
    salesTable->setColumnWidth(7, 85);

    rightLayout->addWidget(salesTable);
    contentLayout->addLayout(rightLayout);
    mainLayout->addLayout(contentLayout);

    // ── CONNECT SIGNALS ───────────────────────────────────────────
    connect(addButton,    &QPushButton::clicked,
            this,         &MainWindow::onAddSale);
    connect(deleteButton, &QPushButton::clicked,
            this,         &MainWindow::onDeleteSale);
    connect(clearButton,  &QPushButton::clicked,
            this,         &MainWindow::onClearForm);
    connect(salesTable,   &QTableWidget::itemSelectionChanged,
            this,         &MainWindow::onTableRowSelected);
    connect(searchInput,  &QLineEdit::textChanged,
            this,         &MainWindow::onSearchChanged);
    connect(filterCombo,  QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            this,         &MainWindow::onFilterCategory);
}

// ════════════════════════════════════════════════════════════════
//  STYLESHEET
// ════════════════════════════════════════════════════════════════
void MainWindow::setupStyleSheet() {
    setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #F5F6FA;
            color: #2C3E50;
            font-family: "Segoe UI", Arial, sans-serif;
            font-size: 13px;
        }

        QLabel#headerLabel {
            font-size: 22px;
            font-weight: bold;
            color: #1A237E;
            padding: 8px 0;
            letter-spacing: 1px;
        }

        QFrame#summaryCard {
            background-color: #FFFFFF;
            border: 1px solid #E0E0E0;
            border-radius: 8px;
            padding: 6px;
        }
        QLabel#cardTitle {
            font-size: 11px;
            color: #888;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }
        QLabel#cardValue {
            font-size: 20px;
            font-weight: bold;
        }

        QGroupBox#formBox {
            background-color: #FFFFFF;
            border: 1px solid #E0E0E0;
            border-radius: 8px;
            padding-top: 20px;
            font-weight: bold;
            color: #1A237E;
        }
        QGroupBox#formBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 4px;
        }

        QLabel#formLabel {
            color: #555;
            font-size: 12px;
        }

        QLineEdit, QComboBox, QDateEdit,
        QDoubleSpinBox, QSpinBox {
            background: #FAFAFA;
            border: 1px solid #CFCFCF;
            border-radius: 5px;
            padding: 5px 8px;
            font-size: 12px;
            color: #2C3E50;
        }
        QLineEdit:focus, QComboBox:focus, QDateEdit:focus,
        QDoubleSpinBox:focus, QSpinBox:focus {
            border: 1.5px solid #2196F3;
            background: #FFFFFF;
        }

        QPushButton#addButton {
            background-color: #1565C0;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 9px;
            font-weight: bold;
            font-size: 13px;
        }
        QPushButton#addButton:hover { background-color: #1976D2; }
        QPushButton#addButton:pressed { background-color: #0D47A1; }

        QPushButton#clearButton {
            background-color: #ECEFF1;
            color: #37474F;
            border: 1px solid #CFD8DC;
            border-radius: 6px;
            padding: 8px;
            font-size: 12px;
        }
        QPushButton#clearButton:hover { background-color: #CFD8DC; }

        QPushButton#deleteButton {
            background-color: #C62828;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px;
            font-size: 12px;
        }
        QPushButton#deleteButton:hover { background-color: #E53935; }
        QPushButton#deleteButton:pressed { background-color: #B71C1C; }

        QTableWidget#salesTable {
            background-color: #FFFFFF;
            border: 1px solid #E0E0E0;
            border-radius: 8px;
            gridline-color: #F0F0F0;
            font-size: 12px;
            alternate-background-color: #F9FBFF;
        }
        QTableWidget#salesTable::item {
            padding: 6px 8px;
        }
        QTableWidget#salesTable::item:selected {
            background-color: #BBDEFB;
            color: #0D47A1;
        }
        QHeaderView::section {
            background-color: #1A237E;
            color: white;
            padding: 8px;
            font-weight: bold;
            font-size: 12px;
            border: none;
            border-right: 1px solid #3949AB;
        }
        QScrollBar:vertical {
            background: #F0F0F0;
            width: 8px;
        }
        QScrollBar::handle:vertical {
            background: #BDBDBD;
            border-radius: 4px;
        }
    )");
}

// ════════════════════════════════════════════════════════════════
//  SAMPLE DATA
// ════════════════════════════════════════════════════════════════
void MainWindow::loadSampleData() {
    auto addSale = [&](const QString &date, const QString &product,
                       const QString &cat, int qty, double price,
                       const QString &status) {
        Sale s;
        s.id          = nextId++;
        s.date        = date;
        s.product     = product;
        s.category    = cat;
        s.quantity    = qty;
        s.unitPrice   = price;
        s.totalAmount = qty * price;
        s.status      = status;
        sales.append(s);
    };

    addSale("2025-04-01", "Laptop Pro 15",       "Electronics",     2, 55000.00, "Completed");
    addSale("2025-04-02", "Wireless Mouse",       "Electronics",     5,  1200.00, "Completed");
    addSale("2025-04-02", "Running Shoes",        "Sports",          3,  3500.00, "Completed");
    addSale("2025-04-03", "Rice Cooker",          "Home & Garden",   1,  2800.00, "Pending");
    addSale("2025-04-04", "C++ Programming Book", "Books",           4,   850.00, "Completed");
    addSale("2025-04-05", "Gaming Headset",       "Electronics",     1,  4500.00, "Pending");
    addSale("2025-04-06", "Polo Shirt",           "Clothing",       10,   450.00, "Completed");
    addSale("2025-04-07", "Coffee Beans 500g",    "Food & Beverage", 8,   380.00, "Completed");
    addSale("2025-04-08", "USB-C Hub",            "Electronics",     3,  1800.00, "Cancelled");
    addSale("2025-04-09", "Yoga Mat",             "Sports",          2,  1200.00, "Completed");
}

// ════════════════════════════════════════════════════════════════
//  POPULATE TABLE
// ════════════════════════════════════════════════════════════════
void MainWindow::populateTable(const QVector<Sale> &data) {
    salesTable->setRowCount(0);

    for (const Sale &s : data) {
        int row = salesTable->rowCount();
        salesTable->insertRow(row);

        auto cell = [&](int col, const QString &text,
                        Qt::Alignment align = Qt::AlignLeft | Qt::AlignVCenter) {
            QTableWidgetItem *item = new QTableWidgetItem(text);
            item->setTextAlignment(align);
            salesTable->setItem(row, col, item);
        };

        cell(0, QString::number(s.id),    Qt::AlignCenter);
        cell(1, s.date,                   Qt::AlignCenter);
        cell(2, s.product);
        cell(3, s.category,               Qt::AlignCenter);
        cell(4, QString::number(s.quantity), Qt::AlignCenter);
        cell(5, QString("₱ %1").arg(s.unitPrice, 0, 'f', 2),   Qt::AlignRight | Qt::AlignVCenter);
        cell(6, QString("₱ %1").arg(s.totalAmount, 0, 'f', 2), Qt::AlignRight | Qt::AlignVCenter);

        // Status cell with colour coding
        QTableWidgetItem *statusItem = new QTableWidgetItem(s.status);
        statusItem->setTextAlignment(Qt::AlignCenter);
        if (s.status == "Completed")
            statusItem->setForeground(QColor("#1B5E20"));
        else if (s.status == "Pending")
            statusItem->setForeground(QColor("#E65100"));
        else
            statusItem->setForeground(QColor("#B71C1C"));
        salesTable->setItem(row, 7, statusItem);
    }
}

// ════════════════════════════════════════════════════════════════
//  SUMMARY CARDS
// ════════════════════════════════════════════════════════════════
void MainWindow::refreshSummary() {
    int    total   = sales.size();
    double revenue = 0.0;
    int    pending = 0;
    QMap<QString, double> productRevenue;

    for (const Sale &s : sales) {
        if (s.status != "Cancelled") {
            revenue += s.totalAmount;
            productRevenue[s.product] += s.totalAmount;
        }
        if (s.status == "Pending") pending++;
    }

    QString top = "—";
    double  topVal = 0;
    for (auto it = productRevenue.begin(); it != productRevenue.end(); ++it) {
        if (it.value() > topVal) { topVal = it.value(); top = it.key(); }
    }

    totalSalesLabel->setText(QString::number(total));
    totalRevenueLabel->setText(QString("₱ %1").arg(revenue, 0, 'f', 2));
    pendingLabel->setText(QString::number(pending));
    topProductLabel->setText(top);
}

// ════════════════════════════════════════════════════════════════
//  SLOT: ADD SALE
// ════════════════════════════════════════════════════════════════
void MainWindow::onAddSale() {
    if (productInput->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a product name.");
        productInput->setFocus();
        return;
    }

    Sale s;
    s.id          = nextId++;
    s.date        = dateEdit->date().toString("yyyy-MM-dd");
    s.product     = productInput->text().trimmed();
    s.category    = categoryCombo->currentText();
    s.quantity    = quantityInput->value();
    s.unitPrice   = priceInput->value();
    s.totalAmount = s.quantity * s.unitPrice;
    s.status      = statusCombo->currentText();

    sales.append(s);
    populateTable(applyFilters());
    refreshSummary();
    onClearForm();

    QMessageBox::information(this, "Success",
        QString("Sale added successfully!\nProduct: %1\nTotal: ₱%2")
            .arg(s.product)
            .arg(s.totalAmount, 0, 'f', 2));
}

// ════════════════════════════════════════════════════════════════
//  SLOT: DELETE SALE
// ════════════════════════════════════════════════════════════════
void MainWindow::onDeleteSale() {
    int row = salesTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a row to delete.");
        return;
    }

    int id = salesTable->item(row, 0)->text().toInt();
    QString product = salesTable->item(row, 2)->text();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Delete",
        QString("Delete sale #%1 (%2)?").arg(id).arg(product),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        for (int i = 0; i < sales.size(); i++) {
            if (sales[i].id == id) { sales.remove(i); break; }
        }
        populateTable(applyFilters());
        refreshSummary();
    }
}

// ════════════════════════════════════════════════════════════════
//  SLOT: CLEAR FORM
// ════════════════════════════════════════════════════════════════
void MainWindow::onClearForm() {
    productInput->clear();
    categoryCombo->setCurrentIndex(0);
    quantityInput->setValue(1);
    priceInput->setValue(100.00);
    statusCombo->setCurrentIndex(0);
    dateEdit->setDate(QDate::currentDate());
    productInput->setFocus();
}

// ════════════════════════════════════════════════════════════════
//  SLOT: TABLE ROW SELECTED
// ════════════════════════════════════════════════════════════════
void MainWindow::onTableRowSelected() {
    int row = salesTable->currentRow();
    if (row < 0) return;

    // Fill form with selected row's data
    dateEdit->setDate(QDate::fromString(
        salesTable->item(row, 1)->text(), "yyyy-MM-dd"));
    productInput->setText(salesTable->item(row, 2)->text());
    int catIdx = categoryCombo->findText(salesTable->item(row, 3)->text());
    if (catIdx >= 0) categoryCombo->setCurrentIndex(catIdx);
    quantityInput->setValue(salesTable->item(row, 4)->text().toInt());
    int stIdx = statusCombo->findText(salesTable->item(row, 7)->text());
    if (stIdx >= 0) statusCombo->setCurrentIndex(stIdx);
}

// ════════════════════════════════════════════════════════════════
//  SLOT: SEARCH & FILTER
// ════════════════════════════════════════════════════════════════
void MainWindow::onSearchChanged(const QString &) {
    populateTable(applyFilters());
}

void MainWindow::onFilterCategory(const QString &) {
    populateTable(applyFilters());
}

QVector<Sale> MainWindow::applyFilters() {
    QString keyword = searchInput->text().trimmed().toLower();
    QString cat     = filterCombo->currentText();

    QVector<Sale> result;
    for (const Sale &s : sales) {
        bool matchSearch = keyword.isEmpty() ||
                           s.product.toLower().contains(keyword);
        bool matchCat    = (cat == "All Categories") ||
                           (s.category == cat);
        if (matchSearch && matchCat)
            result.append(s);
    }
    return result;
}

*/

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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qboxlayout.h"
#include "qpushbutton.h"
#include "qtablewidget.h"
#include <QMainWindow>
#include<QHash>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


struct TableItem
{
    TableItem(QString name, QString compression, QString size, int depth, int resolution) {
        this->name = name;
        this->compression = compression;
        this->size = size;
        this->depth = depth;
        this->resolution = resolution;
    }
    bool operator==(const TableItem & other) const
    {
        return other.compression == this->compression &&
                other.depth == this->depth &&
                other.resolution == this->resolution &&
                other.name == this->name &&
                other.size == this->size;
    }

public:
    QString name;
    QString compression;
    int resolution;
    QString size;
    int depth;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void openFileDialog();
    void openFileDirectory();
    void removeItem();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    QList<TableItem> list;
    QString dirPath;
    void fillFromDirectory(QString);
    QString archive;
    QStringList suffixses;
    Ui::MainWindow *ui;
    QTableWidget* table;
    QPushButton* add_files_btn;
    QPushButton* add_folder_btn;
    QPushButton* remove_item_btn;
    QVBoxLayout* vlayout;
    QHBoxLayout* hlayout;
    QStringList dirFilter;
    void AppendToTableFiles(QStringList files);
    void AppendToTableFromDirectory(QString dir);

};
#endif // MAINWINDOW_H

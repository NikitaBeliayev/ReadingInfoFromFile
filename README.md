### Description:
<br>
The challenge is to write an application to view basic information about an image. The following information is displayed: name, size, resolution, color depth, compression type. The information is displayed in the form of a table. To implement the application, the C++ programming language and the Qt framework were chosen. In the application, you can select both a separate file and a folder where image files are stored. You can also remove a specific entry from the image information from the watch list. When you click on any of the specified columns, all image records are sorted by this column.
<br>

### Code examples:
<br>
Code from main method:
<br>

``` cpp
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
```
<br>

### MainWindow class declaration.

Here are declared handlers for clicking on the buttons for opening the file dialog for selecting a file or folder and deleting a record about the selected image. A list variable is also declared for storing a list of the contents of the records table and other graphical objects for creating an interface.

``` cpp
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
```
<br>
Implementation of the MainWindow constructor, where graphical objects are created, and button click events are associated with the corresponding methods for processing these events:
<br>

``` cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    table = new QTableWidget();
    table->setColumnCount(5);
    dirFilter << "jpg" << "gif" << "tif" << "bmp" << "png" << "pcx";
    table->setHorizontalHeaderLabels({"Name", "Size", "Resolution", "Color depth", "Compression"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vlayout = new QVBoxLayout();
    hlayout = new QHBoxLayout();
    add_files_btn = new QPushButton("Add files");
    add_folder_btn = new QPushButton("Add from folder");
    remove_item_btn = new QPushButton("Remove item");
    hlayout->addWidget(add_files_btn);
    hlayout->addWidget(add_folder_btn);
    hlayout->addWidget(remove_item_btn);
    vlayout->addItem(hlayout);
    vlayout->addWidget(table);
    QWidget* widget = new QWidget();
    widget->setLayout(vlayout);
    this->table->setSortingEnabled(true);
    this->setCentralWidget(widget);
    connect (this->add_files_btn,SIGNAL(clicked()),SLOT(openFileDialog()));
    connect (this->add_folder_btn,SIGNAL(clicked()),SLOT(openFileDirectory()));
    connect(this->remove_item_btn, SIGNAL(clicked()), SLOT(removeItem()));
}
```
<br>
Function for adding all files from chosen directory to table:
<br>

``` cpp
void MainWindow::AppendToTableFromDirectory(QString dirPath)
{
    QDir directory(dirPath);
    if(directory.exists())
    {
        QFileInfoList dirContent = directory.entryInfoList(QStringList()
        << "*.png" <<"*.jpg"<<"*.tif"<<"*.gif"
        <<"*.bmp"<<"*.pcx",QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        for (int i = 0 ; i < dirContent.size(); i++){
            QString compression ;
            QString path = dirContent[i].absoluteFilePath();
            QImage image(path);
            if (dirContent[i].fileName().endsWith("bmp") || dirContent[i].fileName().endsWith("BMP")){
                compression = "without compression";
            }
            if (dirContent[i].fileName().endsWith("jpg")){
                compression = "jpeg compression";
            }
            if (dirContent[i].fileName().endsWith("gif")){
                compression = "lzw compression";
            }
            if (dirContent[i].fileName().endsWith("png")){
                compression = "deflate compression";
            }
            if (dirContent[i].fileName().endsWith("pcx")){
                compression = "rle compression";
            }
            if (dirContent[i].fileName().endsWith("tif")){
                compression = "ZIP/LZW/JPEG compression";
            }
            TableItem item(dirContent[i].fileName(),
                           compression,
                           QString::number(image.height()) + "X" + QString::number(image.width()),
                           image.depth(),
                           static_cast<int>(std::min(image.dotsPerMeterX(), image.dotsPerMeterY()) / 39.37));
            bool check = true;
            for(auto i : list)
                    if (i.compression == item.compression && i.size == item.size && i.depth == item.depth && i.name == item.name && i.resolution == item.resolution){
                        check = false;
                        break;
                    }
            if (check){
                table->insertRow(table->rowCount());
                int pos = table->rowCount() - 1;
                table->setItem(pos, 0, new QTableWidgetItem(dirContent[i].fileName()));
                table->setItem(pos, 1, new QTableWidgetItem(QString::number(image.height()) + "X" + QString::number(image.width())));
                table->setItem(pos, 2, new QTableWidgetItem(QString::number(static_cast<int>(std::min(image.dotsPerMeterX(), image.dotsPerMeterY()) / 39.37))));
                table->setItem(pos, 3, new QTableWidgetItem(QString::number(image.depth())));
                list.append(item);
                table->setItem(pos, 4, new QTableWidgetItem(compression));
            }
        }
    }
}
```
<br>
Function for adding all chosen files to table:
<br>

``` cpp
void MainWindow::AppendToTableFiles(QStringList list_str)
{

    for (int i = 0; i < list_str.size(); i++)
    {
        QFileInfo info(list_str[i]);
        QImage image(list_str[i]);
        QString compression ;

        int pos = table->rowCount() - 1;
        if (info.fileName().endsWith("bmp") || info.fileName().endsWith("BMP")){
            compression = "without compression";
        }
        if (info.fileName().endsWith("jpg")){
            compression = "jpeg compression";
        }
        if (info.fileName().endsWith("gif")){
            compression = "lzw compression";
        }
        if (info.fileName().endsWith("png")){
            compression = "deflate compression";
        }
        if (info.fileName().endsWith("pcx")){
            compression = "rle compression";
        }
        if (info.fileName().endsWith("tif")){
            compression = "ZIP/LZW/JPEG compression";
        }
        TableItem item(info.fileName(),
                       compression,
                       QString::number(image.height()) + "X" + QString::number(image.width()),
                       image.depth(),
                       static_cast<int>(std::min(image.dotsPerMeterX(), image.dotsPerMeterY()) / 39.37));

        bool check = true;
        for(auto i : list)
                if (i.compression == item.compression && i.size == item.size && i.depth == item.depth && i.name == item.name && i.resolution == item.resolution){
                    check = false;
                    break;
                }
        if (check){
            table->insertRow(table->rowCount());
            list.append(item);
            table->setItem(pos, 0, new QTableWidgetItem(info.fileName()));
            table->setItem(pos, 1, new QTableWidgetItem(QString::number(image.height()) + "X" + QString::number(image.width())));
            table->setItem(pos, 2, new QTableWidgetItem(QString::number(static_cast<int>(std::min(image.dotsPerMeterX(), image.dotsPerMeterY()) / 39.37))));
            table->setItem(pos, 3, new QTableWidgetItem(QString::number(image.depth())));
            table->setItem(pos, 4, new QTableWidgetItem(compression));
        }
    }
}
```
<br>
Function for deleting records about image from table:
<br>

``` cpp
void MainWindow::removeItem(){
    int row = this->table->currentRow();
    if (row > -1){
        this->table->removeRow(row);
        this->table->selectionModel()->clearCurrentIndex();
    }
}
```
<br>
The result is the following application:

![](https://raw.githubusercontent.com/NikitaBeliayev/BSU-Computer-Graphic-Programming/development/Reading%20information%20from%20graphic%20files/.media/screenshot.png)

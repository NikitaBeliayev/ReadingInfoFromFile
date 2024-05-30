#include "mainwindow.h"
#include "qboxlayout.h"
#include "qheaderview.h"
#include "ui_mainwindow.h"
#include "QSet"
#include"QFileDialog"

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

MainWindow::~MainWindow()
{
    delete ui;
}



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


void MainWindow::openFileDirectory()
{
    QFileDialog* d = new QFileDialog(this);
    dirPath = d->getExistingDirectory();
    AppendToTableFromDirectory(dirPath);
    delete d;
}


void MainWindow::openFileDialog(){
    QStringList newFileNames = QFileDialog::getOpenFileNames(this, "Выбрать изображения", "/", "Chose image (*.jpg *.gif *.tif *.bmp *.png *.pcx)");
    if (!newFileNames.isEmpty())
    {
         AppendToTableFiles(newFileNames);
    }
}

void MainWindow::removeItem(){
    int row = this->table->currentRow();
    if (row > -1){
        this->table->removeRow(row);
        this->table->selectionModel()->clearCurrentIndex();
    }
}

void MainWindow::AppendToTableFromDirectory(QString dirPath)
{
    QDir directory(dirPath);
    if(directory.exists())
    {
        QFileInfoList dirContent = directory.entryInfoList(QStringList()
                                          << "*.png" <<"*.jpg"<<"*.tif"<<"*.gif"<<"*.bmp"<<"*.pcx",QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
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

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "cutandeditdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_testButton_clicked()
{
    if (!m_pCutAndEditDialog){
        m_pCutAndEditDialog = new CutAndEditDialog(this);
    }
    m_pCutAndEditDialog->reset();
    m_pCutAndEditDialog->show();
}


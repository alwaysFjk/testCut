#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CutAndEditDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_testButton_clicked();

private:
    Ui::MainWindow *ui;
    CutAndEditDialog *m_pCutAndEditDialog = nullptr;
};
#endif // MAINWINDOW_H

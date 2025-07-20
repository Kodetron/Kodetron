#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QFile>
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    void loadStyleSheet();

  private:
    // Private member variables and methods
};

#endif // MAINWINDOW_H
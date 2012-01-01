#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QDate>

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QDir currentDir;

    void    _updatePreview();
    void    _removeItemFromOrderList( const QString p_qsItem );

private slots:
    void on_listOrder_itemSelectionChanged();
    void on_pbDeselectAll_clicked();
    void on_pbSelectAll_clicked();
    void on_listFiles_itemSelectionChanged();
    void on_ledFixName_textChanged(QString );
    void on_chkFixName_stateChanged(int );
    void on_ledStep_textChanged(QString );
    void on_ledStart_textChanged(QString );
    void on_chkSerie_stateChanged(int );
    void on_dtDate_dateChanged(QDate date);
    void on_chkDate_stateChanged(int );
    void on_chkOrigName_stateChanged(int );
    void on_ledDirectoryTarget_textChanged(QString );
    void on_pbDirectoryBrowse_clicked();
};

#endif // MAINWINDOW_H

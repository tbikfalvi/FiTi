#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QDate>
#include <QListWidgetItem>

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

    QDir        currentDir;

    void        _updatePreview();
    void        _removeItemFromOrderList( const QString p_qsItem );
    QString     _getNameWithoutExtension( const QString p_qsFileName ) const;
    QString     _getExtension( const QString p_qsFileName ) const;

private slots:
    void on_listFiles_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void on_pbFileDown_clicked();
    void on_pbFileUp_clicked();
    void on_pbProcessAction_clicked();
    void on_pbDown_clicked();
    void on_pbUp_clicked();
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

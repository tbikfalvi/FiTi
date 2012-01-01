
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->ledDirectoryTarget->setText( QDir::currentPath() );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbDirectoryBrowse_clicked()
{
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Find Files"), QDir::currentPath());

    if (!qsDirectory.isEmpty())
    {
        ui->ledDirectoryTarget->setText( qsDirectory );
    }
}

void MainWindow::on_ledDirectoryTarget_textChanged(QString )
{
    ui->listFiles->clear();

    currentDir = QDir( ui->ledDirectoryTarget->text() );

    QStringList files;

    files = currentDir.entryList(QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < files.size(); ++i)
    {
        ui->listFiles->addItem( files.at(i) );
    }
}

void MainWindow::on_listFiles_itemSelectionChanged()
{
    _updatePreview();
}

void MainWindow::on_pbSelectAll_clicked()
{
    ui->listFiles->selectAll();
}

void MainWindow::on_pbDeselectAll_clicked()
{
    ui->listFiles->clearSelection();
}

void MainWindow::on_chkOrigName_stateChanged(int )
{
    if( ui->chkOrigName->isChecked() )
    {
        ui->listOrder->addItem( tr("Original name") );
    }
    else
    {
        _removeItemFromOrderList( tr("Original name") );
    }

    _updatePreview();
}

void MainWindow::on_chkDate_stateChanged(int )
{
    ui->dtDate->setEnabled( ui->chkDate->isChecked() );

    if( ui->chkDate->isChecked() )
    {
        ui->listOrder->addItem( tr("Date") );
    }
    else
    {
        _removeItemFromOrderList( tr("Date") );
    }

    _updatePreview();
}

void MainWindow::on_dtDate_dateChanged(QDate date)
{
    _updatePreview();
}

void MainWindow::on_chkSerie_stateChanged(int )
{
    ui->ledStart->setEnabled( ui->chkSerie->isChecked() );
    ui->ledStep->setEnabled( ui->chkSerie->isChecked() );

    if( ui->chkSerie->isChecked() )
    {
        ui->listOrder->addItem( tr("Serie") );
    }
    else
    {
        _removeItemFromOrderList( tr("Serie") );
    }

    _updatePreview();
}

void MainWindow::on_ledStart_textChanged(QString )
{
    _updatePreview();
}

void MainWindow::on_ledStep_textChanged(QString )
{
    _updatePreview();
}

void MainWindow::on_chkFixName_stateChanged(int )
{
    ui->ledFixName->setEnabled( ui->chkFixName->isChecked() );

    if( ui->chkFixName->isChecked() )
    {
        ui->listOrder->addItem( tr("Fix name") );
    }
    else
    {
        _removeItemFromOrderList( tr("Fix name") );
    }

    _updatePreview();
}

void MainWindow::on_ledFixName_textChanged(QString )
{
    _updatePreview();
}

void MainWindow::on_listOrder_itemSelectionChanged()
{
    ui->pbUp->setEnabled( ui->listOrder->selectedItems().size() > 0 ? true : false );
    ui->pbDown->setEnabled( ui->listOrder->selectedItems().size() > 0 ? true : false );
}

void MainWindow::_updatePreview()
{
    // Set process button state
    if( (ui->chkOrigName->isChecked() || ui->chkDate->isChecked() || ui->chkFixName->isChecked() || ui->chkSerie->isChecked()) &&
        ui->listFiles->selectedItems().size() > 0 )
    {
        ui->pbProcessAction->setEnabled( true );
    }
    else
    {
        ui->pbProcessAction->setEnabled( false );
    }

    QString     qsPreview = (ui->listFiles->selectedItems().count()>0?ui->listFiles->selectedItems()[0]->text():"");

    ui->ledPreview->setText( qsPreview );
}

void MainWindow::_removeItemFromOrderList( const QString p_qsItem )
{
    for( int i=0; i<ui->listOrder->count(); i++ )
    {
        if( ui->listOrder->item(i)->text().compare(p_qsItem) == 0 )
        {
            ui->listOrder->takeItem(i);
            break;
        }
    }
}

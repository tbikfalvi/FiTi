//QMessageBox::information( this, "DEBUG INFO", "" );

#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QStringList>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgpreview.h"

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

void MainWindow::on_listFiles_currentItemChanged(QListWidgetItem* current, QListWidgetItem*)
{
    ui->pbFileUp->setEnabled( current != NULL ? true : false );
    ui->pbFileDown->setEnabled( current != NULL ? true : false );
}

void MainWindow::on_pbSelectAll_clicked()
{
    ui->listFiles->selectAll();
}

void MainWindow::on_pbDeselectAll_clicked()
{
    ui->listFiles->clearSelection();
}

void MainWindow::on_pbFileUp_clicked()
{
    int     nRow = ui->listFiles->currentRow();

    if( nRow == 0 ) return;

    ui->listFiles->insertItem( nRow-1, ui->listFiles->takeItem(nRow) );
    ui->listFiles->setCurrentRow( nRow-1 );
}

void MainWindow::on_pbFileDown_clicked()
{
    int     nRow = ui->listFiles->currentRow();

    if( nRow == ui->listFiles->count()-1 ) return;

    ui->listFiles->insertItem( nRow+1, ui->listFiles->takeItem(nRow) );
    ui->listFiles->setCurrentRow( nRow+1 );
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

void MainWindow::on_pbUp_clicked()
{
    int     nRow = ui->listOrder->currentRow();

    if( nRow == 0 ) return;

    ui->listOrder->insertItem( nRow-1, ui->listOrder->takeItem(nRow) );
    ui->listOrder->setCurrentRow( nRow-1 );

    _updatePreview();
}

void MainWindow::on_pbDown_clicked()
{
    int     nRow = ui->listOrder->currentRow();

    if( nRow == ui->listOrder->count()-1 ) return;

    ui->listOrder->insertItem( nRow+1, ui->listOrder->takeItem(nRow) );
    ui->listOrder->setCurrentRow( nRow+1 );

    _updatePreview();
}


void MainWindow::on_pbPreviewAction_clicked()
{
    QStringList qslFiles;

    for( int nFile=0,nCount=0; nFile<ui->listFiles->count(); nFile++ )
    {
        qslFiles << ui->listFiles->item( nFile )->text();
        if( ui->listFiles->item(nFile)->isSelected() )
        {
            qslFiles << _createDestinationFile( ui->listFiles->item( nFile )->text(), nCount );
            nCount++;
        }
        else
        {
            qslFiles << ui->listFiles->item( nFile )->text();
        }
    }

    dlgPreview  obPreview( this, &qslFiles );
    obPreview.exec();
}

void MainWindow::on_pbProcessAction_clicked()
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to rename the selected files\n"
                                  "according to the selected settings?"),
                               QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes )
    {
        for( int nFile=0; nFile<ui->listFiles->selectedItems().size(); nFile++ )
        {
            QString     fileFrom = QString( "%1\\%2" ).arg(ui->ledDirectoryTarget->text()).arg(ui->listFiles->selectedItems()[nFile]->text());
            QString     fileTo = ui->ledDirectoryTarget->text();
            QString     fileTarget = _createDestinationFile( ui->listFiles->selectedItems()[nFile]->text(), nFile );

            fileFrom.replace( QChar('/'), QString("\\") );
            fileTo.replace( QChar('/'), QString("\\") );

            fileTo.append( "\\" );
            fileTo.append( fileTarget );

            if( !QFile::rename( fileFrom, fileTo ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("Unable to rename the selected file:\n\n%1\n\nto this name:\n\n%2").arg(fileFrom).arg(fileTo) );
            }
        }
        on_ledDirectoryTarget_textChanged("");
    }
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

    QString     qsPreview = _createDestinationFile( (ui->listFiles->selectedItems().count()>0?ui->listFiles->selectedItems()[0]->text():""), 0 );

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

QString MainWindow::_getNameWithoutExtension( const QString p_qsFileName ) const
{
    QString     qsFileName = "";

    if( p_qsFileName.indexOf(QChar('.')) > -1 )
    {
        qsFileName = p_qsFileName.left( p_qsFileName.indexOf(QChar('.')) );
    }
    else
    {
        qsFileName = p_qsFileName;
    }
    return qsFileName;
}

QString MainWindow::_getExtension( const QString p_qsFileName ) const
{
    QString     qsExtension = "";

    if( p_qsFileName.indexOf(QChar('.')) > -1 )
    {
        qsExtension = p_qsFileName.right( p_qsFileName.size()-p_qsFileName.indexOf(QChar('.')) );
    }
    return qsExtension;
}

QString MainWindow::_createDestinationFile( const QString p_qsFileName, const int nCount ) const
{
    QString     qsDestination = "";

    for( int i=0; i<ui->listOrder->count(); i++ )
    {
        if( ui->listOrder->item(i)->text().compare(tr("Original name")) == 0 )
        {
            qsDestination.append( _getNameWithoutExtension( p_qsFileName ) );
        }
        if( ui->listOrder->item(i)->text().compare(tr("Date")) == 0 )
        {
            qsDestination.append( ui->dtDate->date().toString("yyyyMMdd") );
        }
        if( ui->listOrder->item(i)->text().compare(tr("Serie")) == 0 )
        {
            qsDestination.append( QString::number( ui->ledStart->text().toInt() + nCount * ui->ledStep->text().toInt() ) );
        }
        if( ui->listOrder->item(i)->text().compare(tr("Fix name")) == 0 )
        {
            qsDestination.append( ui->ledFixName->text() );
        }
    }
    qsDestination.append( _getExtension(p_qsFileName) );

    return qsDestination;
}


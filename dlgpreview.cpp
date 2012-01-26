#include "dlgpreview.h"
#include "ui_dlgpreview.h"

dlgPreview::dlgPreview(QWidget *parent, QStringList *pqslFiles) : QDialog(parent), ui(new Ui::dlgPreview)
{
    ui->setupUi(this);

    for( int i=0; i<pqslFiles->size(); i++ )
    {
        QString qsLine = "";

        qsLine.append( pqslFiles->at(i) );
        qsLine.append( " -> ");
        i++;
        qsLine.append( pqslFiles->at(i) );
        ui->listFiles->addItem( qsLine );
    }
}

dlgPreview::~dlgPreview()
{
    delete ui;
}

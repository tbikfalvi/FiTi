#ifndef DLGPREVIEW_H
#define DLGPREVIEW_H

#include <QDialog>
#include <QStringList>

namespace Ui {
    class dlgPreview;
}

class dlgPreview : public QDialog
{
    Q_OBJECT

public:
    explicit dlgPreview(QWidget *parent = 0, QStringList *pqslFiles=NULL);
    ~dlgPreview();

private:
    Ui::dlgPreview *ui;
};

#endif // DLGPREVIEW_H

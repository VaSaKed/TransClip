#ifndef TCPOPUPWIDGET_H
#define TCPOPUPWIDGET_H

#include "TCPopup.h"
#include "TCLangBox.h"
#include "TCTabDictWidget.h"
#include "GoogleTranslateApi/GTApi.h"

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class TCPopupWidget : public TCPopup
{
    Q_OBJECT

    QLineEdit *lneInput;
    QLabel *lblOutput;
    TCLangBox *langBox;
    QPushButton *btnDialogPopup;

    TCTabDictWidget *dictsWgt;

public:
    TCPopupWidget();

signals:

public slots:

private slots:
    void onClipBoardChanged(QClipboard::Mode mode);
    void onInputLineEnterPressed();
    void onLanguageChanged();
    void onTranslationReady(const GTApiTranslation& gtApiTr);

private:
    void paintEvent(QPaintEvent *event);

private:
    GTApi   *gtApi;
};

#endif // TCPOPUPWIDGET_H

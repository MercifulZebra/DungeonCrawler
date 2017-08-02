#ifndef FLATBUTTON_H
#define FLATBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QPalette>

class FlatButton : public QPushButton
{
    Q_OBJECT
public:
    explicit FlatButton(QWidget *parent = 0);

    void setFlatStyle();
    void setStandardColor(QColor nColor);
    void setHoverColor(QColor nColor);
    void setPressedColor(QColor nColor);

signals:

public slots:
};

#endif // FLATBUTTON_H

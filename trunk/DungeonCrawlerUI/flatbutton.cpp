#include "flatbutton.h"

#include <QDebug>

FlatButton::FlatButton(QWidget *parent) : QPushButton(parent)
{
    setAutoFillBackground(true);
}

void FlatButton::setFlatStyle() {
    setStyleSheet(QString("QPushButton { border: 0px; } "));
}

void FlatButton::setStandardColor(QColor nColor) {
    setStyleSheet(styleSheet() + QString("QPushButton { background-color: %1 } ").arg(nColor.name()));
}

void FlatButton::setHoverColor(QColor nColor) {
    setStyleSheet(styleSheet() + QString("QPushButton:hover { background-color: %1 } ").arg(nColor.name()));
}

void FlatButton::setPressedColor(QColor nColor) {
    setStyleSheet(styleSheet() + QString("QPushButton:pressed { background-color: %1 } ").arg(nColor.name()));
}

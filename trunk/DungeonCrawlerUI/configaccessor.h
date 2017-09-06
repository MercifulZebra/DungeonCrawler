#ifndef CONFIGACCESSOR_H
#define CONFIGACCESSOR_H

#include <QObject>

class ConfigAccessor : public QObject
{
    Q_OBJECT
public:
    explicit ConfigAccessor(QObject *parent = nullptr);

    void clear();
    bool loadConfig(QString config_filename);

    QString moveToolImage_string;
    QString selectToolImage_string;
    QString paintToolImage_string;
signals:

public slots:
};

#endif // CONFIGACCESSOR_H

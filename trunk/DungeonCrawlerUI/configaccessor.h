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

signals:

public slots:
};

#endif // CONFIGACCESSOR_H

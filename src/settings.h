#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class QQmlEngine;
class QJSEngine;

class Settings : public QObject
{
    Q_OBJECT
    QSettings _settings;

public:
    explicit Settings(QObject *parent = 0);

    Q_INVOKABLE void setValue(const QString& key, const QVariant& value);
    Q_INVOKABLE QVariant value(const QString& key, const QVariant& defaultValue = QVariant());

    static QObject * provider(QQmlEngine*, QJSEngine*);
signals:

public slots:

};

#endif // SETTINGS_H

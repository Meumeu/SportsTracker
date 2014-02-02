#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    _settings.setValue(key, value);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue)
{
    return _settings.value(key, defaultValue);
}

QObject * Settings::provider(QQmlEngine*, QJSEngine*)
{
    return new Settings;
}

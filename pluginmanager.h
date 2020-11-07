/*
    SPDX-FileCopyrightText: 2020 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kwinglobals.h"

#include <QHash>
#include <QObject>

#include <KPluginMetaData>

namespace KWin
{

class Plugin;

class KWIN_EXPORT PluginManager : public QObject
{
    Q_OBJECT

public:
    ~PluginManager() override;

    QStringList loadedPlugins() const;
    QStringList availablePlugins() const;

public Q_SLOTS:
    bool loadPlugin(const QString &pluginId);
    bool loadPlugin(const KPluginMetaData &metadata);
    void unloadPlugin(const QString &pluginId);

private:
    QHash<QString, Plugin *> m_plugins;
    KWIN_SINGLETON(PluginManager)
};

} // namespace KWin

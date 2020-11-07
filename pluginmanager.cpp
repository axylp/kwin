/*
    SPDX-FileCopyrightText: 2020 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pluginmanager.h"
#include "main.h"
#include "plugin.h"
#include "utils.h"

#include <KConfigGroup>
#include <KPluginFactory>
#include <KPluginLoader>
#include <KPluginMetaData>

namespace KWin
{

KWIN_SINGLETON_FACTORY(PluginManager)

static const QString s_pluginDirectory = QStringLiteral("kwin/plugins");

static QVector<KPluginMetaData> enumerateAllPlugins()
{
    return KPluginLoader::findPlugins(s_pluginDirectory);
}

PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{
    const KConfigGroup config(kwinApp()->config(), QStringLiteral("Plugins"));

    const QVector<KPluginMetaData> plugins = enumerateAllPlugins();
    for (const KPluginMetaData &metadata : plugins) {
        bool tryLoad = false;

        const QString configKey = metadata.pluginId() + QLatin1String("Enabled");
        if (config.hasKey(configKey)) {
            tryLoad = config.readEntry(configKey, false);
        } else if (metadata.isEnabledByDefault()) {
            tryLoad = true;
        }

        if (tryLoad) {
            loadPlugin(metadata);
        }
    }
}

PluginManager::~PluginManager()
{
    s_self = nullptr;
}

QStringList PluginManager::loadedPlugins() const
{
    return m_plugins.keys();
}

QStringList PluginManager::availablePlugins() const
{
    const QVector<KPluginMetaData> plugins = enumerateAllPlugins();

    QStringList ret;
    for (const KPluginMetaData &metadata : plugins) {
        ret.append(metadata.pluginId());
    }

    return ret;
}

bool PluginManager::loadPlugin(const QString &pluginId)
{
    const auto offers = KPluginLoader::findPluginsById(s_pluginDirectory, pluginId);
    for (const KPluginMetaData &metadata : offers) {
        if (loadPlugin(metadata)) {
            return true;
        }
    }
    return false;
}

bool PluginManager::loadPlugin(const KPluginMetaData &metadata)
{
    if (!metadata.isValid()) {
        qCDebug(KWIN_CORE) << "PluginManager::loadPlugin needs a valid plugin metadata";
        return false;
    }

    const QString pluginId = metadata.pluginId();
    if (m_plugins.contains(pluginId)) {
        qCDebug(KWIN_CORE) << "Plugin" << pluginId << "is already loaded";
        return false;
    }

    KPluginLoader pluginLoader(metadata.fileName());
    if (pluginLoader.pluginVersion() != KWIN_PLUGIN_API_VERSION) {
        qCWarning(KWIN_CORE) << pluginId << "has mismatching plugin version";
        return false;
    }

    KPluginFactory *pluginFactory = pluginLoader.factory();
    if (!pluginFactory) {
        qCWarning(KWIN_CORE) << "Failed to get plugin factory for" << pluginId;
        return false;
    }

    Plugin *plugin = pluginFactory->create<Plugin>(this);
    if (!plugin) {
        qCWarning(KWIN_CORE) << "Failed to create plugin" << pluginId;
        return false;
    }

    m_plugins.insert(pluginId, plugin);

    connect(plugin, &QObject::destroyed, this, [this, pluginId]() {
        m_plugins.remove(pluginId);
    });

    return true;
}

void PluginManager::unloadPlugin(const QString &pluginId)
{
    Plugin *plugin = m_plugins.take(pluginId);
    if (!plugin) {
        qCWarning(KWIN_CORE) << "No plugin with the specified id:" << pluginId;
    }
    delete plugin;
}

} // namespace KWin

/*
    SPDX-FileCopyrightText: 2020 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "screencastmanager.h"

#include <KPluginFactory>

using namespace KWin;

K_PLUGIN_FACTORY_WITH_JSON(ScreencastManagerFactory,
                           "metadata.json",
                           registerPlugin<ScreencastManager>();)
K_EXPORT_PLUGIN_VERSION(KWIN_PLUGIN_API_VERSION)

#include "main.moc"

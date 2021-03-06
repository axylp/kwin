/*
    KWin - the KDE window manager
    This file is part of the KDE project.

    SPDX-FileCopyrightText: 2015 Martin Gräßlin <mgraesslin@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef KWIN_SCENE_QPAINTER_DRM_BACKEND_H
#define KWIN_SCENE_QPAINTER_DRM_BACKEND_H
#include <platformsupport/scenes/qpainter/backend.h>
#include <QObject>
#include <QVector>

namespace KWin
{

class DrmBackend;
class DrmDumbBuffer;
class DrmOutput;
class DrmGpu;

class DrmQPainterBackend : public QObject, public QPainterBackend
{
    Q_OBJECT
public:
    DrmQPainterBackend(DrmBackend *backend, DrmGpu *gpu);
    ~DrmQPainterBackend() override;

    QImage *bufferForScreen(int screenId) override;
    bool needsFullRepaint(int screenId) const override;
    void beginFrame(int screenId) override;
    void endFrame(int screenId, int mask, const QRegion &damage) override;
    bool perScreenRendering() const override;

private:
    void initOutput(DrmOutput *output);
    struct Output {
        DrmDumbBuffer *buffer[2];
        DrmOutput *output;
        int index = 0;
    };
    QVector<Output> m_outputs;
    DrmBackend *m_backend;
    DrmGpu *m_gpu;
};
}

#endif

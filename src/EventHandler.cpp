//
// Created by Danyil Pozniaokv on 2/20/26.
//

#include <QInputEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QQuickWindow>
#include <QWidget>
#include <QWindow>
#include <QCursor>
#include <QPoint>
#include <QTimer>
#include "EventHandler.h"

#include <bits/this_thread_sleep.h>


EventHandler::EventHandler(QObject *parent) {
    isHooking = false;
}

bool EventHandler::eventFilter(QObject *obj, QEvent *event) {

    if (!isHooking) {
        return QObject::eventFilter(obj, event);
    }
    auto elapsed = timer.elapsed();
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *key_event = static_cast<QKeyEvent *>(event); // Safe already
        auto ev = RecordedEvent(elapsed, key_event);
        recordedEvents.push_back(ev);
        qDebug() << "Recorded keyboard event";

        return QObject::eventFilter(obj, event);
    }
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease || event->type() == QEvent::MouseMove) {
        QMouseEvent *mouse_event = static_cast<QMouseEvent *>(event);
        auto ev = RecordedEvent(elapsed, mouse_event);
        recordedEvents.push_back(ev);
        qDebug() << "Recorded mouse event";
        return QObject::eventFilter(obj, event);
    }

    return QObject::eventFilter(obj, event);
}

void EventHandler::startHooking() {
    timer.restart();
    isHooking = true;
}

void EventHandler::stopHooking() {
    isHooking = false;
}

void EventHandler::playRecordedEvents() {
    if (recordedEvents.isEmpty() || playbackIndex >= recordedEvents.size()) {
        playbackIndex = 0;
        return;
    }
    if (isStopped) { //
        isStopped = false;
        return;
    }

    qDebug() << "Start playing" << recordedEvents.size() << "index:" << playbackIndex;
    if (auto *window = QGuiApplication::focusWindow()) {
        auto &eventData = recordedEvents[playbackIndex];

        if (eventData.type == QEvent::KeyPress || eventData.type == QEvent::KeyRelease) {
            QApplication::postEvent(window, eventData.toKeyEvent());
        } else {
            QPoint gPos = window->mapToGlobal(eventData.pos);
            QCursor::setPos(gPos);
            QMouseEvent *mouseEvent = new QMouseEvent(
                eventData.type,
                eventData.pos,
                eventData.pos,
                gPos,
                eventData.button,
                eventData.button,
                eventData.modifiers
            );
            QApplication::postEvent(window, mouseEvent);
        }
    }

    int nextIndex = playbackIndex + 1;
    qint64 delay = 10;
    qDebug() << "Playing event" << playbackIndex << "with delay" << delay;
    if (nextIndex < recordedEvents.size())
    {
        delay = recordedEvents[nextIndex].timestamp - recordedEvents[playbackIndex].timestamp;
        playbackIndex = nextIndex;
        QTimer::singleShot(delay, this, &EventHandler::playRecordedEvents);
    }
    else
    {
        playbackIndex = 0;
        delay = recordedEvents[playbackIndex + 1].timestamp - recordedEvents[playbackIndex].timestamp;
        if (loopPlayback) {
            QTimer::singleShot(delay, this, &EventHandler::playRecordedEvents);
        }
        qDebug() << "Finish playback";
    }
}

void EventHandler::clearRecordedEvents() {
        recordedEvents.clear();
}

void EventHandler::setLoopPlayback(bool loop) {
    loopPlayback = loop;
}

void EventHandler::stopPlayback() {
    playbackIndex = 0;
    isStopped = true;
}

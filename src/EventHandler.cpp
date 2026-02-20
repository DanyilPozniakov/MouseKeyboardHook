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
        qDebug() << "Recorded mouse event";

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
    auto eventsToPlay = recordedEvents[playbackIndex];
    QObject *receiver = QGuiApplication::focusWindow();
    if (!receiver) return;


    if (eventsToPlay.type == QEvent::KeyPress || eventsToPlay.type == QEvent::KeyRelease) {
        auto keyEvent = eventsToPlay.toKeyEvent();
        QApplication::postEvent(receiver, keyEvent);
    } else if (eventsToPlay.type == QEvent::MouseButtonPress ||
               eventsToPlay.type == QEvent::MouseButtonRelease ||
               eventsToPlay.type == QEvent::MouseMove) {
        QCursor::setPos(QApplication::activeWindow()->mapToGlobal(eventsToPlay.pos));

        auto mouseEvent = eventsToPlay.toMouseEvent();
        QApplication::postEvent(receiver, mouseEvent);
    }

        playbackIndex++;
        if (playbackIndex >= recordedEvents.size()) {
            playbackIndex = 0;
        }

    QTimer::singleShot(100, this, &EventHandler::playRecordedEvents);
}

void EventHandler::clearRecordedEvents() {
        recordedEvents.clear();
}

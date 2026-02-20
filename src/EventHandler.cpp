//
// Created by Danyil Pozniaokv on 2/20/26.
//

#include <QInputEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

#include "EventHandler.h"

EventHandler::EventHandler(QObject *parent): QObject(parent)
{
    isHooking  = false;
}

bool EventHandler::eventFilter(QObject *obj, QEvent *event) {

    if (!isHooking) {
        return QObject::eventFilter(obj, event);
    }

    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *key_event = static_cast<QKeyEvent *>(event); // Safe already
        auto ev = QSharedPointer<QKeyEvent>::create(event->type(), key_event->key(),
                                                    key_event->modifiers(), key_event->text(),
                                                    key_event->isAutoRepeat(), key_event->count());
        recordedEvents.push_back(ev);
        qDebug() << "Recorded key event: " << ev->key() << " with modifiers: " << ev->modifiers();

        return QObject::eventFilter(obj, event);
    }
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease || event->type() == QEvent::MouseMove) {
        QMouseEvent *mouse_event = static_cast<QMouseEvent *>(event);
        auto ev = QSharedPointer<QMouseEvent>::create(event->type(), mouse_event->localPos(),
                                                    mouse_event->windowPos(), mouse_event->screenPos(),
                                                    mouse_event->button(), mouse_event->buttons(),
                                                    mouse_event->modifiers());
        recordedEvents.push_back(ev);
        qDebug() << "Recorded mouse event: " << ev->button() << " at position: " << ev->localPos();
        return QObject::eventFilter(obj, event);
    }

    return QObject::eventFilter(obj, event);
}

void EventHandler::startHooking() {
        isHooking = true;
}

void EventHandler::stopHooking() {
        isHooking = false;
}

void EventHandler::clearRecordedEvents() {
        recordedEvents.clear();
}

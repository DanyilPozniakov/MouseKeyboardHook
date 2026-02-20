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
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

#include "EventHandler.h"

#include <bits/this_thread_sleep.h>

#define SCRIPT_FILE_PATH "recorded_events.json"


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
    saveEventsToJson();
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

void EventHandler::saveEventsToJson() {
        QJsonArray jsonArray;
        for (const auto& event : recordedEvents) {
            QJsonObject jsonEvent;
            jsonEvent["timestamp"] = event.timestamp;
            jsonEvent["type"] = event.type;
            jsonEvent["pos"] = QString("%1,%2").arg(event.pos.x()).arg(event.pos.y());
            jsonEvent["globalPos"] = QString("%1,%2").arg(event.globalPos.x()).arg(event.globalPos.y());
            jsonEvent["button"] = static_cast<int>(event.button);
            jsonEvent["key"] = event.key;
            jsonEvent["modifiers"] = static_cast<int>(event.modifiers);
            jsonEvent["text"] = event.text;
            jsonArray.append(jsonEvent);
        }

        QJsonDocument doc(jsonArray);
        QFile file(SCRIPT_FILE_PATH);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
            file.close();
        } else {
            qDebug() << "Failed to save events to JSON";
        }

}

void EventHandler::readEventsFromJson() {
    QFile file(SCRIPT_FILE_PATH);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            QJsonArray jsonArray = doc.array();
            recordedEvents.clear();
            for (const auto& jsonEvent : jsonArray) {
                if (jsonEvent.isObject()) {
                    QJsonObject obj = jsonEvent.toObject();
                    auto timestamp = obj["timestamp"].toVariant().toLongLong();
                    auto type = static_cast<QEvent::Type>(obj["type"].toInt());
                    auto posStr = obj["pos"].toString();
                    auto globalPosStr = obj["globalPos"].toString();
                    auto button = static_cast<Qt::MouseButton>(obj["button"].toInt());
                    auto key = obj["key"].toInt();
                    auto modifiers = static_cast<Qt::KeyboardModifiers>(obj["modifiers"].toInt());
                    auto text = obj["text"].toString();
                    QPoint pos, globalPos;
                    sscanf(posStr.toStdString().c_str(), "%d,%d", &pos.rx(), &pos.ry());
                    sscanf(globalPosStr.toStdString().c_str(), "%d,%d", &globalPos.rx(), &globalPos.ry());
                    RecordedEvent event(timestamp, type, pos, globalPos, button, key, modifiers, text);
                    recordedEvents.push_back(event);
                }
            }
        } else {
            qDebug() << "Invalid JSON format";
        }
    } else {
        qDebug() << "Failed to read events from JSON";
    }


}

//
// Created by Danyil Pozniaokv on 2/20/26.
//

#ifndef MOUSEKEYBOARDHOOK_EVENTHANDLER_H
#define MOUSEKEYBOARDHOOK_EVENTHANDLER_H

#include <QObject>
#include <QKeyEvent>
#include <QElapsedTimer>

struct RecordedEvent {
    qint64 timestamp;
    QEvent::Type type;
    QPoint pos;
    Qt::MouseButton button;
    int key;
    Qt::KeyboardModifiers modifiers;
    QString text;

    RecordedEvent(qint64 time, QKeyEvent* keyEvent)
        : timestamp(time), type(keyEvent->type()), pos(), button(Qt::NoButton),
          key(keyEvent->key()), modifiers(keyEvent->modifiers()), text(keyEvent->text()) {}

    RecordedEvent(qint64 time , QMouseEvent* mouseEvent)
        : timestamp(time), type(mouseEvent->type()), pos(mouseEvent->localPos().toPoint()),
          button(mouseEvent->button()), key(0), modifiers(mouseEvent->modifiers()) {}


    [[nodiscard]] QKeyEvent* toKeyEvent() const {
        return new QKeyEvent(type, key, modifiers, text);
    }

    [[nodiscard]] QMouseEvent* toMouseEvent() const {
        return new QMouseEvent(type, pos, pos, pos, button, Qt::NoButton, modifiers);
    }

};




class EventHandler
    : public QObject
{
    Q_OBJECT
public:
    explicit EventHandler(QObject* parent = nullptr);

    bool eventFilter(QObject* obj, QEvent* event) override;

    void startHooking();
    void stopHooking();
    void playRecordedEvents();

    void clearRecordedEvents();


private:
    int playbackIndex = 0;
    QElapsedTimer timer;
    std::atomic<bool> isHooking{false};

    QVector<RecordedEvent> recordedEvents;
};



#endif //MOUSEKEYBOARDHOOK_EVENTHANDLER_H

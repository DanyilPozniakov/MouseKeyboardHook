//
// Created by Danyil Pozniaokv on 2/20/26.
//

#ifndef MOUSEKEYBOARDHOOK_EVENTHANDLER_H
#define MOUSEKEYBOARDHOOK_EVENTHANDLER_H

#include <QObject>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QElapsedTimer>

struct RecordedEvent {
    qint64 timestamp;
    QEvent::Type type;
    QPoint pos;
    QPoint globalPos;
    Qt::MouseButton button;
    int key;
    Qt::KeyboardModifiers modifiers;
    QString text;

    RecordedEvent(qint64 time, QKeyEvent* keyEvent)
        : timestamp(time), type(keyEvent->type()), pos(), button(Qt::NoButton),
          key(keyEvent->key()), modifiers(keyEvent->modifiers()), text(keyEvent->text()) {}

    RecordedEvent(qint64 time , QMouseEvent* mouseEvent)
        : timestamp(time), type(mouseEvent->type()), pos(mouseEvent->position().toPoint()),
          globalPos(mouseEvent->globalPosition().toPoint()), button(mouseEvent->button()), key(0), modifiers(mouseEvent->modifiers()) {}


    [[nodiscard]] QKeyEvent* toKeyEvent() const {
        return new QKeyEvent(type, key, modifiers, text);
    }

    [[nodiscard]] QMouseEvent* toMouseEvent() const {
        QPointF pf = QPointF(pos);
        Qt::MouseButtons buttons = Qt::MouseButtons(button);
        return new QMouseEvent(type, pf, pf, pf, button, buttons, modifiers);
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
    void setLoopPlayback(bool loop);
    void stopPlayback();


private:
    int playbackIndex = 0;
    bool loopPlayback = false;
    QElapsedTimer timer;
    bool isHooking{false};
    bool isStopped{false};

    QVector<RecordedEvent> recordedEvents;
};



#endif //MOUSEKEYBOARDHOOK_EVENTHANDLER_H

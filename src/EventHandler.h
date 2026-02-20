//
// Created by Danyil Pozniaokv on 2/20/26.
//

#ifndef MOUSEKEYBOARDHOOK_EVENTHANDLER_H
#define MOUSEKEYBOARDHOOK_EVENTHANDLER_H

#include <QObject>
#include <QKeyEvent>

class EventHandler
    : public QObject
{
    Q_OBJECT
public:
    explicit EventHandler(QObject* parent = nullptr);

    bool eventFilter(QObject* obj, QEvent* event) override;

    void startHooking();
    void stopHooking();

    void clearRecordedEvents();


private:
    std::atomic<bool> isHooking{false};

    QVector<QSharedPointer<QInputEvent>> recordedEvents;
};



#endif //MOUSEKEYBOARDHOOK_EVENTHANDLER_H

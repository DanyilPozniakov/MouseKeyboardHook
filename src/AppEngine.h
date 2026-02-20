//
// Created by Danyil Pozniaokv on 2/20/26.
//

#ifndef MOUSEKEYBOARDHOOK_APPENGINE_H
#define MOUSEKEYBOARDHOOK_APPENGINE_H

#include <EventHandler.h>


class AppEngine : public QObject
{
    Q_OBJECT
public:
    AppEngine();
    ~AppEngine();

    QSharedPointer<EventHandler> getEventHandler() const;

    Q_INVOKABLE void startHooking();
    Q_INVOKABLE void stopHooking();
    Q_INVOKABLE void clearRecordedEvents();
    Q_INVOKABLE void playRecordedEvents();
    Q_INVOKABLE void stopPlaying();


private:
    QSharedPointer<EventHandler> eventHandler;
};



#endif //MOUSEKEYBOARDHOOK_APPENGINE_H

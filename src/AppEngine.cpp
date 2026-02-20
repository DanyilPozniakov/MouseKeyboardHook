//
// Created by Danyil Pozniaokv on 2/20/26.
//

#include "AppEngine.h"

AppEngine::AppEngine() {
        eventHandler = QSharedPointer<EventHandler>(new EventHandler(this));

}

AppEngine::~AppEngine() {
}

QSharedPointer<EventHandler> AppEngine::getEventHandler() const {
        return eventHandler;
}

void AppEngine::startHooking() {
        eventHandler->startHooking();
}

void AppEngine::stopHooking() {
        eventHandler->stopHooking();
}

void AppEngine::clearRecordedEvents() {
        eventHandler->clearRecordedEvents();
}

void AppEngine::playRecordedEvents() {
        eventHandler->readEventsFromJson();
        eventHandler->playRecordedEvents();
}

void AppEngine::clearEvents() {
        eventHandler->clearRecordedEvents();

}

void AppEngine::setLoopPlayback(bool loop) {
        eventHandler->setLoopPlayback(loop);
}

void AppEngine::stopPlayback() {
        eventHandler->stopPlayback();
}



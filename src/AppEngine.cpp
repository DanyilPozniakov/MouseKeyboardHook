//
// Created by Danyil Pozniaokv on 2/20/26.
//

#include "AppEngine.h"

AppEngine::AppEngine() {
        eventHandler = QSharedPointer<EventHandler>::create();

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
        eventHandler->playRecordedEvents();
}

void AppEngine::stopPlaying() {

}

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

}


void AppEngine::stopHooking() {
}

void AppEngine::clearRecordedEvents() {
}

void AppEngine::playRecordedEvents() {
}

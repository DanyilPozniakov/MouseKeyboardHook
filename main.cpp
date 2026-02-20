#include <QApplication>
#include <QQmlApplicationEngine>

#include <qqmlcontext.h>

#include "AppEngine.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    AppEngine EventAppEngine;
    engine.rootContext()->setContextProperty("EventAppEngine", &EventAppEngine);

    app.installEventFilter(EventAppEngine.getEventHandler().get());

    engine.load(QUrl(QStringLiteral("qrc:/MouseKeyboardHook/qml/main.qml")));

    return QApplication::exec();
}

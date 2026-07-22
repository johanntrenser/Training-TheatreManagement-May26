/*
 * File: main.cpp
 * Description: Entry point for the Theatre Management System QML application.
 *              Initializes the QGuiApplication and sets the global QQuickStyle.
 *              Creates and initializes the ControllerAdapter, which wires together
 *              all backend services (13 controllers). Logs success or failure of
 *              service initialization. Exposes the controller to QML via the
 *              root context property. Configures error handling for QML object
 *              creation failures. Loads the main QML module (Trenser_QuickBookUI)
 *              and starts the application event loop.
 * Author: Trenser
 * Created: 22 July 2026
 */
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QQuickStyle>
#include "controlleradapter.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Basic");
    ControllerAdapter controllerAdapter;
    if (controllerAdapter.initialize()) {
        qInfo() << "==================================================";
        qInfo() << "SUCCESS: All 13 Controller Services Initialized!";
        qInfo() << "==================================================";
    } else {
        qCritical() << "ERROR: Failed to initialize Controller Services!";
    }
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("controller", &controllerAdapter);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Trenser_QuickBookUI", "Main");

    return QGuiApplication::exec();
}

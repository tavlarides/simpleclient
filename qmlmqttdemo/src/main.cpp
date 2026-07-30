#include "appcontroller.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  app.setOrganizationName("IntracomDemo");
  app.setApplicationName("qmlmqttdemo");

  AppController controller;

  QQmlApplicationEngine engine;

  engine.setInitialProperties({ { "appController", QVariant::fromValue(&controller) },
                                { "messageModel", QVariant::fromValue(controller.messageModel()) } });

  engine.loadFromModule("DemoApp", "App");

  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}

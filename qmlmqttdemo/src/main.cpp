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
  // engine.rootContext()->setContextProperty("appController", &controller);
  // engine.rootContext()->setContextProperty("messageModel", controller.messageModel());

  engine.setInitialProperties({ { "appController", QVariant::fromValue(&controller) },
                                { "messageModel", QVariant::fromValue(controller.messageModel()) } });

  // const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
  engine.loadFromModule("DemoApp", "App");

  if (engine.rootObjects().isEmpty())
    return -1;

  // QObject::connect(
  //     &engine,
  //     &QQmlApplicationEngine::objectCreated,
  //     &app,
  //     [url](QObject *obj, const QUrl &objUrl) {
  //       if (!obj && url == objUrl)
  //         QCoreApplication::exit(-1);
  //     },
  //     Qt::QueuedConnection);

  // engine.load(url);
  return app.exec();
}

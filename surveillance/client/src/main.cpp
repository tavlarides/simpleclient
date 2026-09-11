#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
    QGuiApplication application(argc, argv);
    application.setApplicationName(QStringLiteral("operator-client"));

    QQmlApplicationEngine engine;
    engine.loadFromModule("Surveillance.Operator", "Main");
    if (engine.rootObjects().isEmpty()) {
        return 1;
    }
    return application.exec();
}

#include "securityserver.h"

#include <QCoreApplication>

int main(int argc, char *argv[]) {
    QCoreApplication application(argc, argv);
    application.setApplicationName(QStringLiteral("security-server"));

    SecurityServer server;
    server.start();
    return application.exec();
}

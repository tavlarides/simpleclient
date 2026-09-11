#pragma once

#include <QObject>
#include <QScopedPointer>

class SecurityServerPrivate;

class SecurityServer : public QObject {
    Q_OBJECT

public:
    explicit SecurityServer(QObject *parent = nullptr);
    ~SecurityServer() override;

    void start();

private:
    Q_DECLARE_PRIVATE(SecurityServer)
    QScopedPointer<SecurityServerPrivate> d_ptr;
};

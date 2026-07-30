#pragma once

#include <QAbstractItemModel>
#include <QObject>
#include <QQmlEngine>
#include <QScopedPointer>

class OperatorControllerPrivate;

class OperatorController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QAbstractItemModel *incidentModel READ incidentModel CONSTANT)
    Q_PROPERTY(QString connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)

public:
    explicit OperatorController(QObject *parent = nullptr);
    ~OperatorController() override;

    QAbstractItemModel *incidentModel();
    QString connectionStatus() const;

signals:
    void connectionStatusChanged();

private:
    Q_DECLARE_PRIVATE(OperatorController)
    QScopedPointer<OperatorControllerPrivate> d_ptr;
};

#ifndef CONTROLLERADAPTER_H
#define CONTROLLERADAPTER_H

#include <QObject>
#include <QString>
#include <qqmlintegration.h>
#include "Controller.h"
#include "EnumsAdapter.h"
#include "Enums.h"

class ControllerAdapter : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool isAuthenticated READ isAuthenticated NOTIFY authenticationChanged)
    Q_PROPERTY(EnumsAdapter::UserType userType READ userType NOTIFY authenticationChanged)
public:
    explicit ControllerAdapter(QObject *parent = nullptr);
    ~ControllerAdapter();
    Q_INVOKABLE bool initialize();
    Q_INVOKABLE int login(const QString &email, const QString &password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE int registerUser(const QString& name,
                                     const QString& email,
                                     const QString& password,
                                     const QString& phone,
                                     const EnumsAdapter::UserType userType);

    bool isAuthenticated() const { return m_authenticated; }
    EnumsAdapter::UserType userType() const { return m_currentUserType; }

signals:
    void authenticationChanged();

private:
    Controller* m_controller = nullptr;
    bool m_authenticated = false;
    EnumsAdapter::UserType m_currentUserType = EnumsAdapter::UserType::USER_NOT_FOUND;
    bool m_initialized = false;
};

#endif // CONTROLLERADAPTER_H
#ifndef REGISTERMANAGER_H
#define REGISTERMANAGER_H

#include <QObject>
#include <QHash>

#include "positionregister.h"

namespace Core {

class IEditor;

} // namespace Core

namespace ELPosRegisters {
namespace Internal {

class RegisterManager : public QObject
{
    Q_OBJECT

public:
    RegisterManager();

    RegisterManager(const RegisterManager &) = delete;
    RegisterManager & operator=(const RegisterManager &) = delete;
    RegisterManager(RegisterManager &&) = delete;
    RegisterManager & operator=(RegisterManager &&) = delete;

    ~RegisterManager();

    void fillRegister(const QChar & key, Core::IEditor &editor);
    void jumpToRegister(const QChar & key);

public Q_SLOTS:
    void editorAboutToClose(Core::IEditor *editor);

private Q_SLOTS:
    void onSessionLoaded(QString sessionName);

private:
    void loadRegisters();
    void saveRegisters();

private:
    QHash<QChar, PositionRegister> m_registers;
};

} // namespace Internal
} // namespace ELPosRegisters

#endif // REGISTERMANAGER_H

#include "registermanager.h"

#include <coreplugin/idocument.h>
#include <coreplugin/editormanager/ieditor.h>
#include <projectexplorer/session.h>
#include <utils/fileutils.h>

#include "positionregister.h"

namespace ELPosRegisters {
namespace Internal {

RegisterManager::RegisterManager()
    : m_registers()
{
    connect(ProjectExplorer::SessionManager::instance(), &ProjectExplorer::SessionManager::sessionLoaded,
            this, &RegisterManager::onSessionLoaded);
}

RegisterManager::~RegisterManager() = default;

void RegisterManager::fillRegister(const QChar &key, Core::IEditor &editor)
{
    const QString filePath = editor.document()->filePath().toString();
    const int line = editor.currentLine();
    // Work around:
    // Core::IEditor::currentColumn() is 1 based, while gotoLine is 0 based for the column.
    const int column = editor.currentColumn() - 1;

    PositionRegister reg(editor, filePath, line, column);
    m_registers.insert(key, reg);
    saveRegisters();
}

void RegisterManager::jumpToRegister(const QChar &key)
{
    if (m_registers.contains(key))
        m_registers[key].jumpTo();
}

void RegisterManager::editorAboutToClose(Core::IEditor *editor)
{
    for (PositionRegister & reg : m_registers) {
        if (reg.getEditor() == editor)
            reg.forgetEditor();
    }
}

void RegisterManager::onSessionLoaded(QString sessionName)
{
    Q_UNUSED(sessionName);

    loadRegisters();
}

void RegisterManager::loadRegisters()
{
    m_registers.clear();

    const QStringList list = ProjectExplorer::SessionManager::value(QLatin1String("ELPosRegisters")).toStringList();
    foreach (const QString & i, list) {
        const QChar key = i.at(0);
        const PositionRegister reg = PositionRegister::deserialize(i.mid(1));
        m_registers.insert(key, reg);
    }
}

void RegisterManager::saveRegisters()
{
    QStringList list;

    const auto end = m_registers.cend();
    for (auto iter = m_registers.cbegin(); iter != end; ++iter)
        list << iter.key() + iter.value().serialize();

    ProjectExplorer::SessionManager::setValue(QLatin1String("ELPosRegisters"), list);
}

} // namespace Internal
} // namespace ELPosRegister

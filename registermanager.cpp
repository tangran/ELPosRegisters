#include "registermanager.h"

#include <coreplugin/idocument.h>
#include <coreplugin/editormanager/ieditor.h>
#include <utils/fileutils.h>

namespace ELPosRegisters {
namespace Internal {

RegisterManager::RegisterManager()
    : m_registers()
{
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

} // namespace Internal
} // namespace ELPosRegister

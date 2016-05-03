#include "positionregister.h"

#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/editormanager/editormanager.h>

#include <QDebug>

namespace ELPosRegisters {
namespace Internal {

PositionRegister PositionRegister::deserialize(const QString &serializedRegister)
{
    const QLatin1Char colon(':');
    const int firstColon(serializedRegister.indexOf(colon));
    const int secondColon(serializedRegister.indexOf(colon, firstColon + 1));
    const int thirdColon(serializedRegister.indexOf(colon, secondColon + 1));

    if (firstColon == 0 && secondColon != -1 && thirdColon != -1) {
        const QString filePath = serializedRegister.mid(firstColon + 1, secondColon - firstColon - 1);
        const int line = serializedRegister.mid(secondColon + 1, thirdColon - secondColon - 1).toInt();
        const int column = serializedRegister.mid(thirdColon + 1).toInt();

        return PositionRegister(filePath, line, column);
    } else {
        qDebug() << "PositionRegister::deserialize() Invalid register string:" << serializedRegister;
        return PositionRegister();
    }
}

PositionRegister::PositionRegister()
    : m_editor(nullptr)
    , m_filePath()
    , m_line(-1)
    , m_column(-1)
{
}

PositionRegister::PositionRegister(const QString & filePath, int line, int column)
    : m_editor(nullptr)
    , m_filePath(filePath)
    , m_line(line)
    , m_column(column)
{
}

PositionRegister::PositionRegister(Core::IEditor & editor, const QString & filePath, int line, int column)
    : m_editor(&editor)
    , m_filePath(filePath)
    , m_line(line)
    , m_column(column)
{
}

PositionRegister::~PositionRegister() = default;

Core::IEditor *PositionRegister::getEditor() const
{
    return m_editor;
}

bool PositionRegister::isValid() const
{
    return (!m_filePath.isEmpty() && m_line != -1 && m_column != -1);
}

void PositionRegister::jumpTo()
{
    if (m_editor) {
        Core::EditorManager::activateEditor(m_editor);
        m_editor->gotoLine(m_line, m_column);
    } else {
        m_editor = Core::EditorManager::openEditorAt(m_filePath, m_line, m_column);
    }
}

QString PositionRegister::serialize() const
{
    const QLatin1Char colon(':');
    return (colon + m_filePath
            + colon + QString::number(m_line)
            + colon + QString::number(m_column));
}

void PositionRegister::forgetEditor()
{
    m_editor = nullptr;
}

} // namespace Internal
} // namespace ELPosRegisters

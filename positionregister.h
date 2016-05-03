#ifndef POSITIONREGISTER_H
#define POSITIONREGISTER_H

#include <QString>

namespace Core {

class IEditor;

} // namespace Core

namespace ELPosRegisters {
namespace Internal {

class PositionRegister
{
public:
    static PositionRegister deserialize(const QString & serializedRegister);

public:
    PositionRegister();
    PositionRegister(const QString &filePath, int line, int column);
    PositionRegister(Core::IEditor &editor, const QString &filePath, int line, int column);

    PositionRegister(const PositionRegister &) = default;
    PositionRegister & operator=(const PositionRegister &) = default;
    //	PositionRegister(PositionRegister &&) = default;
    //	PositionRegister & operator=(PositionRegister &&) = default;

    ~PositionRegister();

    Core::IEditor *getEditor() const ;
    bool isValid() const;

    void jumpTo();

    QString serialize() const;

    void forgetEditor();

private:
    Core::IEditor *m_editor;
    QString m_filePath;
    int m_line;
    int m_column;
};

} // namespace Internal
} // namespace ELPosRegisters

#endif // POSITIONREGISTER_H

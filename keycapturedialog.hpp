#ifndef KEYCAPTUREDIALOG_HPP
#define KEYCAPTUREDIALOG_HPP

#include <QDialog>

namespace ELPosRegisters {
namespace Internal {

class KeyCaptureDialog : public QDialog
{
public:
    KeyCaptureDialog(const QString & text, QWidget * parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    QChar getCapturedKey() const;

private:
    void keyPressEvent(QKeyEvent * event) override;

private:
    QChar m_capturedKey;
};

} // namespace Internal
} // namespace ELPosRegisters

#endif // KEYCAPTUREDIALOG_HPP

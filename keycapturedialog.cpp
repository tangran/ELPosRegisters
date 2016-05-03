#include "keycapturedialog.hpp"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <QKeyEvent>

#include <QDebug>

using namespace ELPosRegisters::Internal;

KeyCaptureDialog::KeyCaptureDialog(const QString & text, QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f)
    , m_capturedKey()
{
    QLabel * const label = new QLabel(tr("%1\n\nPress ECS to cancel.").arg(text), this);
    label->setWordWrap(true);

    QLayout * const layout = new QVBoxLayout(this);
    layout->addWidget(label);

    setLayout(layout);

    setWindowTitle(tr("Key capture"));
}

QChar KeyCaptureDialog::getCapturedKey() const
{
    return m_capturedKey;
}

void KeyCaptureDialog::keyPressEvent(QKeyEvent * event)
{
    const int key(event->key());

    const bool keyIsLatter(Qt::Key_A <= key && key <= Qt::Key_Z);
    const bool keyIsNumber(Qt::Key_0 <= key && key <= Qt::Key_9);

    if (event->modifiers() == Qt::NoModifier && (keyIsLatter || keyIsNumber))
    {
        m_capturedKey = key;
        accept();
    }
    else
    {
        QDialog::keyPressEvent(event);
    }
}

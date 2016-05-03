#include "elposregistersplugin.h"
#include "elposregistersconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/coreconstants.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
#include <QKeyEvent>

#include <QtPlugin>

using namespace ELPosRegisters::Internal;

ELPosRegistersPlugin::ELPosRegistersPlugin()
    : m_registerManager()
    , m_currentEditor(nullptr)
    , m_activeAction(NoAction)
{
}

ELPosRegistersPlugin::~ELPosRegistersPlugin()
{
}

bool ELPosRegistersPlugin::initialize(const QStringList &arguments, QString *errorString)
{
	Q_UNUSED(arguments)
	Q_UNUSED(errorString)

    QAction *bindRegister = new QAction(tr("Save position into register"), this);
    Core::Command *bindCmd = Core::ActionManager::registerAction(bindRegister, Constants::BIND_REGISTER_ACTION_ID,
                                                                 Core::Context(Core::Constants::C_GLOBAL));
    bindCmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+x,Ctrl+r,space")));
    connect(bindRegister, &QAction::triggered, this, &ELPosRegistersPlugin::triggerBindRegister);

    QAction *jumpRegister = new QAction(tr("Jump to position in register"), this);
    Core::Command *jumpCmd = Core::ActionManager::registerAction(jumpRegister, Constants::JUMP_REGISTER_ACTION_ID,
                                                                 Core::Context(Core::Constants::C_GLOBAL));
    jumpCmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+x,Ctrl+r,j")));
    connect(jumpRegister, &QAction::triggered, this, &ELPosRegistersPlugin::triggerJump);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::POSITION_REGISTER_MENU_ID);
    menu->menu()->setTitle(tr("Emacs like position registers"));
    menu->addAction(bindCmd);
    menu->addAction(jumpCmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    const Core::EditorManager *editorManager = Core::EditorManager::instance();
    connect(editorManager, &Core::EditorManager::currentEditorChanged,
            this, &ELPosRegistersPlugin::changeEditor);
    connect(editorManager, &Core::EditorManager::editorAboutToClose,
            this, &ELPosRegistersPlugin::editorAboutToClose);
    connect(editorManager, &Core::EditorManager::editorAboutToClose,
            &m_registerManager, &RegisterManager::editorAboutToClose);

	return true;
}

void ELPosRegistersPlugin::extensionsInitialized()
{
}

ExtensionSystem::IPlugin::ShutdownFlag ELPosRegistersPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

bool ELPosRegistersPlugin::eventFilter(QObject *watched, QEvent *event)
{
    Q_ASSERT(m_currentEditor);
    Q_ASSERT(watched == m_currentEditor->widget());

    if (event->type() == QEvent::KeyPress) {
        const QKeyEvent * const keyEvent = static_cast<QKeyEvent *>(event);
        const int key(keyEvent->key());
        const int modifiers(keyEvent->modifiers());

        if (modifiers == Qt::CTRL && key == Qt::Key_G)
        {
            stopAction();
            return true;
        }

        const bool keyIsLatter(Qt::Key_A <= key && key <= Qt::Key_Z);
        const bool keyIsNumber(Qt::Key_0 <= key && key <= Qt::Key_9);

        if (keyEvent->modifiers() == Qt::NoModifier && (keyIsLatter || keyIsNumber)) {
            stopAction();

            if (m_activeAction == BindAction)
                m_registerManager.fillRegister(key, *m_currentEditor);
            else if (m_activeAction == JumpAction)
                m_registerManager.jumpToRegister(key);
            else
                Q_ASSERT_X(false, "ELPosRegistersPlugin", "Unknown action");

            return true;
        }

        return true;
    }

    return false;
}

void ELPosRegistersPlugin::startAction(ELPosRegistersPlugin::RegisterAction action)
{
    Q_ASSERT(m_activeAction == NoAction);
    Q_ASSERT(action == BindAction || action == JumpAction);

    if (m_currentEditor && m_currentEditor->widget()) {
        m_currentEditor->widget()->installEventFilter(this);
        m_activeAction = action;

        QString help;
        if (m_activeAction == BindAction) {
            help = tr("ELPosRegister bind mode. Press any letter or number without modifier "
                      "to save current cursor position into corresponding register. "
                      "Or press ctrl+g to abort.");
        } else if (m_activeAction == JumpAction) {
            help = tr("ELPosRegister jump mode. Press any letter or number without modifier "
                      "to place cursor in position saved in corresponding register. "
                      "Or press ctrl+g to abort.");
        } else {
            Q_ASSERT_X(false, "ELPosRegistersPlugin", "Unknown action");
        }

        Core::EditorManager::showEditorStatusBar(
                    QLatin1String(Constants::M_STATUS_BUFFER),
                    help,
                    tr("Cancel"), this, SLOT(abortAction()));
    }
}

void ELPosRegistersPlugin::stopAction()
{
    if (m_currentEditor && m_currentEditor->widget())
        m_currentEditor->widget()->removeEventFilter(this);

    m_activeAction = NoAction;
    Core::EditorManager::hideEditorStatusBar(QLatin1String(Constants::M_STATUS_BUFFER));
}


void ELPosRegistersPlugin::resetCurrentEditor()
{
    stopAction();
    m_currentEditor = nullptr;
}

void ELPosRegistersPlugin::changeEditor(Core::IEditor *editor)
{
    stopAction();
    m_currentEditor = editor;
}

void ELPosRegistersPlugin::editorAboutToClose(Core::IEditor *editor)
{
    if (m_currentEditor == editor)
        resetCurrentEditor();
}

void ELPosRegistersPlugin::triggerBindRegister()
{
    startAction(BindAction);
}

void ELPosRegistersPlugin::triggerJump()
{
    startAction(JumpAction);
}

void ELPosRegistersPlugin::abortAction()
{
    stopAction();
}

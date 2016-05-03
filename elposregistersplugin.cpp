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

#include "keycapturedialog.hpp"

using namespace ELPosRegisters::Internal;

ELPosRegistersPlugin::ELPosRegistersPlugin()
    : m_registerManager()
    , m_currentEditor(nullptr)
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

void ELPosRegistersPlugin::changeEditor(Core::IEditor *editor)
{
    m_currentEditor = editor;
}

void ELPosRegistersPlugin::editorAboutToClose(Core::IEditor *editor)
{
    if (m_currentEditor == editor)
        m_currentEditor = nullptr;
}

void ELPosRegistersPlugin::triggerBindRegister()
{
    if (m_currentEditor) {
        KeyCaptureDialog dialog(tr("Press any letter or number without modifier "
                                   "to save current cursor position into corresponding register."),
                                Core::ICore::mainWindow());
        const int result = dialog.exec();
        if (result == QDialog::Accepted)
        {
            const int key = dialog.getCapturedKey();
            m_registerManager.fillRegister(key, *m_currentEditor);
        }
    }
}

void ELPosRegistersPlugin::triggerJump()
{
    if (m_currentEditor) {
        KeyCaptureDialog dialog(tr("Press any letter or number without modifier "
                                   "to place cursor in position saved in corresponding register."),
                                Core::ICore::mainWindow());
        const int result = dialog.exec();
        if (result == QDialog::Accepted)
        {
            const int key = dialog.getCapturedKey();
            m_registerManager.jumpToRegister(key);
        }
    }
}

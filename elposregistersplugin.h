#ifndef ELPOSREGISTERS_H
#define ELPOSREGISTERS_H

#include "elposregisters_global.h"

#include <extensionsystem/iplugin.h>

#include "registermanager.h"

namespace Core {

class IEditor;

} // namespace Core

namespace ELPosRegisters {
namespace Internal {

class ELPosRegistersPlugin : public ExtensionSystem::IPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "ELPosRegisters.json")

public:
	ELPosRegistersPlugin();
	~ELPosRegistersPlugin();

	bool initialize(const QStringList &arguments, QString *errorString);
	void extensionsInitialized();
	ShutdownFlag aboutToShutdown();

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    enum RegisterAction
    {
        NoAction,
        BindAction,
        JumpAction
    };

private:
    void startAction(RegisterAction action);
    void stopAction();

    void resetCurrentEditor();

private slots:
    void changeEditor(Core::IEditor *editor);
    void editorAboutToClose(Core::IEditor *editor);

    void triggerBindRegister();
    void triggerJump();

    void abortAction();

private:
    RegisterManager m_registerManager;

    Core::IEditor * m_currentEditor;
    RegisterAction m_activeAction;
};

} // namespace Internal
} // namespace ELPosRegisters

#endif // ELPOSREGISTERS_H

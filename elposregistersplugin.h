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

private slots:
    void changeEditor(Core::IEditor *editor);
    void editorAboutToClose(Core::IEditor *editor);

    void triggerBindRegister();
    void triggerJump();

private:
    RegisterManager m_registerManager;

    Core::IEditor * m_currentEditor;
};

} // namespace Internal
} // namespace ELPosRegisters

#endif // ELPOSREGISTERS_H

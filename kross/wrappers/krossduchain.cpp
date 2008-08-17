//This is file has been generated by xmltokross, you should not edit this file but the files used to generate it.

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <kross/core/manager.h>
#include <kross/core/wrapperinterface.h>
#include <language/duchain/duchain.h>

class KrossKDevelopDUChain : public QObject, public Kross::WrapperInterface
{
	Q_OBJECT
	public:
		KrossKDevelopDUChain(KDevelop::DUChain* obj, QObject* parent=0) : QObject(parent), wrapped(obj){ setObjectName("KDevelop::DUChain"); }
		void* wrappedObject() const { return wrapped; }

		Q_PROPERTY(const QMetaObject  staticMetaObject READ getstaticMetaObject SCRIPTABLE true)
		Q_SCRIPTABLE const QMetaObject  getstaticMetaObject() const { return wrapped->staticMetaObject; }
		Q_SCRIPTABLE QList< KDevelop::TopDUContext* > allChains() const { return wrapped->allChains(); }
		Q_SCRIPTABLE KDevelop::TopDUContext* chainForDocument(const KUrl& x0) const { return wrapped->chainForDocument(x0); }
		Q_SCRIPTABLE KDevelop::TopDUContext* chainForDocument(const KDevelop::IndexedString& x0) const { return wrapped->chainForDocument(x0); }
		Q_SCRIPTABLE QList< KDevelop::TopDUContext* > chainsForDocument(const KUrl& x0) const { return wrapped->chainsForDocument(x0); }
		Q_SCRIPTABLE QList< KDevelop::TopDUContext* > chainsForDocument(const KDevelop::IndexedString& x0) const { return wrapped->chainsForDocument(x0); }
		Q_SCRIPTABLE KDevelop::TopDUContext* chainForDocument(const KUrl& x0, const KDevelop::ParsingEnvironment* x1, KDevelop::TopDUContext::Flags x2=KDevelop::TopDUContext::AnyFlag) const { return wrapped->chainForDocument(x0, x1, x2); }
		Q_SCRIPTABLE KDevelop::TopDUContext* chainForDocument(const KDevelop::IndexedString& x0, const KDevelop::ParsingEnvironment* x1, KDevelop::TopDUContext::Flags x2=KDevelop::TopDUContext::AnyFlag) const { return wrapped->chainForDocument(x0, x1, x2); }
		Q_SCRIPTABLE KDevelop::TopDUContext* chainForIndex(int x0) const { return wrapped->chainForIndex(x0); }
		Q_SCRIPTABLE QList< KUrl > documents() const { return wrapped->documents(); }
		Q_SCRIPTABLE void addDocumentChain(KDevelop::TopDUContext* x0) { wrapped->addDocumentChain(x0); }
		Q_SCRIPTABLE KDevelop::DUChain* self() { return wrapped->self(); }
		Q_SCRIPTABLE KDevelop::Definitions* definitions() { return wrapped->definitions(); }
		Q_SCRIPTABLE KDevelop::Uses* uses() { return wrapped->uses(); }
		Q_SCRIPTABLE KDevelop::DUChainLock* lock() { return wrapped->lock(); }
		Q_SCRIPTABLE KDevelop::DUChainObserver* notifier() { return wrapped->notifier(); }
		Q_SCRIPTABLE void branchAdded(KDevelop::DUContext* x0) { wrapped->branchAdded(x0); }
		Q_SCRIPTABLE void branchModified(KDevelop::DUContext* x0) { wrapped->branchModified(x0); }
		Q_SCRIPTABLE void branchRemoved(KDevelop::DUContext* x0) { wrapped->branchRemoved(x0); }
		Q_SCRIPTABLE bool isInMemory(int x0) const { return wrapped->isInMemory(x0); }
		Q_SCRIPTABLE void updateContextEnvironment(KDevelop::TopDUContext* x0, KDevelop::ParsingEnvironmentFile* x1) { wrapped->updateContextEnvironment(x0, x1); }
		Q_SCRIPTABLE int newTopContextIndex() { return wrapped->newTopContextIndex(); }
		Q_SCRIPTABLE void removeDocumentChain(KDevelop::TopDUContext* x0) { wrapped->removeDocumentChain(x0); }
		Q_SCRIPTABLE void emitDeclarationSelected(KDevelop::DUChainPointer< KDevelop::Declaration > x0) { wrapped->emitDeclarationSelected(x0); }
	private:
		KDevelop::DUChain* wrapped;
};

bool krossduchain_registerHandler(const QByteArray& name, Kross::MetaTypeHandler::FunctionPtr* handler)
{ Kross::Manager::self().registerMetaTypeHandler(name, handler); return false; }

namespace Handlers
{
QVariant _kDevelopDUChainHandler(void* type)
{
	if(!type) return QVariant();
	KDevelop::DUChain* t=static_cast<KDevelop::DUChain*>(type);
	Q_ASSERT(dynamic_cast<KDevelop::DUChain*>(t));
	return qVariantFromValue((QObject*) new KrossKDevelopDUChain(t, 0));
}
bool b_KDevelopDUChain=krossduchain_registerHandler("KDevelop::DUChain*", _kDevelopDUChainHandler);
QVariant kDevelopDUChainHandler(KDevelop::DUChain* type){ return _kDevelopDUChainHandler(type); }
QVariant kDevelopDUChainHandler(const KDevelop::DUChain* type) { return _kDevelopDUChainHandler((void*) type); }

}
#include "krossduchain.moc"

//This is file has been generated by xmltokross, you should not edit this file but the files used to generate it.

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <kross/core/manager.h>
#include <kross/core/wrapperinterface.h>
#include <language/interfaces/iproblem.h>

class KrossKDevelopProblem : public QObject, public Kross::WrapperInterface
{
	Q_OBJECT
	public:
		KrossKDevelopProblem(KDevelop::Problem* obj, QObject* parent=0) : QObject(parent), wrapped(obj){ setObjectName("KDevelop::Problem"); }
		void* wrappedObject() const { return wrapped; }

		Q_SCRIPTABLE KDevelop::Problem::Source source() const { return wrapped->source(); }
		Q_SCRIPTABLE void setSource(KDevelop::Problem::Source x0) { wrapped->setSource(x0); }
		Q_SCRIPTABLE KDevelop::DocumentRange finalLocation() const { return wrapped->finalLocation(); }
		Q_SCRIPTABLE void setFinalLocation(const KDevelop::DocumentRange& x0) { wrapped->setFinalLocation(x0); }
		Q_SCRIPTABLE QStack< KDevelop::DocumentCursor > locationStack() const { return wrapped->locationStack(); }
		Q_SCRIPTABLE void addLocation(const KDevelop::DocumentCursor& x0) { wrapped->addLocation(x0); }
		Q_SCRIPTABLE void setLocationStack(const QStack< KDevelop::DocumentCursor >& x0) { wrapped->setLocationStack(x0); }
		Q_SCRIPTABLE void clearLocationStack() { wrapped->clearLocationStack(); }
		Q_SCRIPTABLE QString description() const { return wrapped->description(); }
		Q_SCRIPTABLE void setDescription(const QString& x0) { wrapped->setDescription(x0); }
		Q_SCRIPTABLE QString explanation() const { return wrapped->explanation(); }
		Q_SCRIPTABLE void setExplanation(const QString& x0) { wrapped->setExplanation(x0); }
		Q_SCRIPTABLE KDevelop::Problem operator=(const KDevelop::Problem& x0) { return wrapped->operator=(x0); }
	private:
		KDevelop::Problem* wrapped;
};

bool krossiproblem_registerHandler(const QByteArray& name, Kross::MetaTypeHandler::FunctionPtr* handler)
{ Kross::Manager::self().registerMetaTypeHandler(name, handler); return false; }

namespace Handlers
{
QVariant _kDevelopProblemHandler(void* type)
{
	if(!type) return QVariant();
	KDevelop::Problem* t=static_cast<KDevelop::Problem*>(type);
	Q_ASSERT(dynamic_cast<KDevelop::Problem*>(t));
	return qVariantFromValue((QObject*) new KrossKDevelopProblem(t, 0));
}
bool b_KDevelopProblem=krossiproblem_registerHandler("KDevelop::Problem*", _kDevelopProblemHandler);
QVariant kDevelopProblemHandler(KDevelop::Problem* type){ return _kDevelopProblemHandler(type); }
QVariant kDevelopProblemHandler(const KDevelop::Problem* type) { return _kDevelopProblemHandler((void*) type); }

}
#include "krossiproblem.moc"

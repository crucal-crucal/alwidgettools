#pragma once

#include "alwidgettoolsdef.hpp"
#include "alsingleton.hpp"

#define alApp AL::CALApplication::instance()

namespace AL {
class CALApplicationPrivate;

class CALWIDGETTOOLS_EXPORT CALApplication : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALApplication)
	Q_PROPERTY(bool isEnableMica READ getIsEnableMica WRITE setIsEnableMica NOTIFY sigIsEnableMicaChanged)
	Q_PROPERTY(QString micaImagePath READ getMicaImagePath WRITE setMicaImagePath NOTIFY sigMicaImagePathChanged)

public:
	static CALApplication* instance();

	void init();
	void syncMica(QWidget* widget, bool isSync = true);
	static bool containsCursorToItem(const QWidget* item);

	void setIsEnableMica(bool enable);
	[[nodiscard]] bool getIsEnableMica() const;

	void setMicaImagePath(const QString& path);
	[[nodiscard]] QString getMicaImagePath() const;

Q_SIGNALS:
	Q_SIGNAL void sigIsEnableMicaChanged();
	Q_SIGNAL void sigMicaImagePathChanged();

protected:
	const QScopedPointer<CALApplicationPrivate> d_ptr{ nullptr };

	friend class CALSingleton<CALApplication>;

private:
	explicit CALApplication(QObject* parent = nullptr);
	~CALApplication() override;
};
} // namespace AL

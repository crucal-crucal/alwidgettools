#pragma once

#include "uvwidgettoolsdef.hpp"
#include "uvsingleton.hpp"

#define uvApp CUVApplication::instance()

class CUVApplicationPrivate;

class CUVWIDGETTOOLS_EXPORT CUVApplication : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVApplication)
	Q_PROPERTY(bool isEnableMica READ getIsEnableMica WRITE setIsEnableMica NOTIFY sigIsEnableMicaChanged)
	Q_PROPERTY(QString micaImagePath READ getMicaImagePath WRITE setMicaImagePath NOTIFY sigMicaImagePathChanged)

public:
	static CUVApplication* instance();

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
	const QScopedPointer<CUVApplicationPrivate> d_ptr{ nullptr };

	friend class CUVSingleton<CUVApplication>;

private:
	explicit CUVApplication(QObject* parent = nullptr);
	~CUVApplication() override;
};

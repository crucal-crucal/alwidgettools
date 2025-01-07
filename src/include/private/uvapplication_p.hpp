#pragma once

#include <QIcon>
#include <QObject>

#include "alwidgettoolsdef.hpp"

class QTranslator;

class CUVApplication;

class CUVApplicationPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVApplicationPrivate)
	Q_DECLARE_PUBLIC(CUVApplication)

public:
	explicit CUVApplicationPrivate(CUVApplication* q, QObject* parent = nullptr);
	~CUVApplicationPrivate() override;
	Q_SLOT void slotThemeModeChanged(const ALThemeType::ThemeMode& mode);

Q_SIGNALS:
	Q_SIGNAL void sigInitMicaBase(const QImage& img);

protected:
	CUVApplication* const q_ptr{ nullptr };

	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	friend class CUVMicaBaseInitObject;

	QTranslator* translator{ nullptr };

	bool isEnableMica{};
	QString micaImagePath{};
	ALThemeType::ThemeMode themeMode{};
	QList<QWidget*> micaWidgetList{};
	QImage lightBaseImage{};
	QImage darkBaseImage{};

	void initMicaBaseImage(const QImage& img);
	QRect calculateWindowVirtualGeometry(const QWidget* widget) const;
	void updateMica(QWidget* widget, bool isProcessEvent = true) const;
	void updateAllMicaWidget();
};

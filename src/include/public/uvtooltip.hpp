#pragma once

#include <QWidget>

#include "uvsingleton.hpp"
#include "uvwidgettoolsdef.hpp"

class CUVToolTipHelper;
class CUVToolTipPrivate;

class CUVWIDGETTOOLS_EXPORT CUVToolTip : public QWidget {
	Q_OBJECT
	Q_DISABLE_COPY(CUVToolTip)
	Q_DECLARE_PRIVATE(CUVToolTip)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(int displayMsec READ getDisplayMsec WRITE setDisplayMsec NOTIFY sigDisplayMsecChanged)
	Q_PROPERTY(int showDelayMsec READ getShowDelayMsec WRITE setShowDelayMsec NOTIFY sigShowDelayMsecChanged)
	Q_PROPERTY(int hideDelayMsec READ getHideDelayMsec WRITE setHideDelayMsec NOTIFY sigHideDelayMsecChanged)
	Q_PROPERTY(QString toolTip READ getToolTip WRITE setToolTip NOTIFY sigToolTipChanged)
	Q_PROPERTY(QWidget* customWidget READ getCustomWidget WRITE setCustomWidget NOTIFY sigCustomWidgetChanged)

public:
	explicit CUVToolTip(QWidget* parent = nullptr);
	~CUVToolTip() override;

	void setBorderRadius(int borderRadius);
	[[nodiscard]] int getBorderRadius() const;

	void setDisplayMsec(int displayMsec);
	[[nodiscard]] int getDisplayMsec() const;

	void setShowDelayMsec(int delayMsec);
	[[nodiscard]] int getShowDelayMsec() const;

	void setHideDelayMsec(int delayMsec);
	[[nodiscard]] int getHideDelayMsec() const;

	void setToolTip(const QString& tooltip);
	[[nodiscard]] QString getToolTip() const;

	void setCustomWidget(QWidget* customWidget);
	[[nodiscard]] QWidget* getCustomWidget() const;

signals:
	void sigBorderRadiusChanged();
	void sigDisplayMsecChanged();
	void sigShowDelayMsecChanged();
	void sigHideDelayMsecChanged();
	void sigToolTipChanged();
	void sigCustomWidgetChanged();

protected:
	const QScopedPointer<CUVToolTipPrivate> d_ptr{ nullptr };

	friend CUVToolTipHelper;
	void paintEvent(QPaintEvent* event) override;
};

class CUVWIDGETTOOLS_EXPORT CUVToolTipHelper : public QObject {
	Q_OBJECT

public:
	static CUVToolTip* instance();

	static void showText(const QPoint& pos, const QString& text, QWidget* w = nullptr);
	static void showText(const QPoint& pos, const QString& text, QWidget* w, const QRect& rect);
	static void showText(const QPoint& pos, const QString& text, QWidget* w, const QRect& rect, int msecShowTime);
	static inline void hideText() { showText(QPoint(), QString()); }

	static bool isVisible();
	static QString text();

protected:
	friend class CUVToolTip;
	friend class CUVSingleton<CUVToolTip>;
};

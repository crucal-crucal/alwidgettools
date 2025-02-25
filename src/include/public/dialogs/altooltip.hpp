#pragma once

#include <QWidget>

#include "alsingleton.hpp"
#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALToolTipHelper;
class CALToolTipPrivate;

class CALWIDGETTOOLS_EXPORT CALToolTip : public QWidget {
	Q_OBJECT
	Q_DISABLE_COPY(CALToolTip)
	Q_DECLARE_PRIVATE(CALToolTip)
	Q_PROPERTY(int displayMsec READ getDisplayMsec WRITE setDisplayMsec NOTIFY sigDisplayMsecChanged)
	Q_PROPERTY(int showDelayMsec READ getShowDelayMsec WRITE setShowDelayMsec NOTIFY sigShowDelayMsecChanged)
	Q_PROPERTY(int hideDelayMsec READ getHideDelayMsec WRITE setHideDelayMsec NOTIFY sigHideDelayMsecChanged)
	Q_PROPERTY(qreal borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(QString toolTip READ getToolTip WRITE setToolTip NOTIFY sigToolTipChanged)
	Q_PROPERTY(QWidget* customWidget READ getCustomWidget WRITE setCustomWidget NOTIFY sigCustomWidgetChanged)

public:
	explicit CALToolTip(QWidget* parent = nullptr);
	~CALToolTip() override;

	void setBorderRadius(qreal borderRadius);
	[[nodiscard]] qreal getBorderRadius() const;

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

	void updatePos();

Q_SIGNALS:
	void sigBorderRadiusChanged();
	void sigDisplayMsecChanged();
	void sigShowDelayMsecChanged();
	void sigHideDelayMsecChanged();
	void sigToolTipChanged();
	void sigCustomWidgetChanged();

protected:
	const QScopedPointer<CALToolTipPrivate> d_ptr{ nullptr };

	friend CALToolTipHelper;
	void paintEvent(QPaintEvent* event) override;
};

class CALWIDGETTOOLS_EXPORT CALToolTipHelper : public QObject {
	Q_OBJECT

public:
	static CALToolTip* instance();

	static void showText(const QPoint& pos, const QString& text, QWidget* w = nullptr);
	static void showText(const QPoint& pos, const QString& text, QWidget* w, const QRect& rect);
	static void showText(const QPoint& pos, const QString& text, QWidget* w, const QRect& rect, int msecShowTime);
	static inline void hideText() { showText(QPoint(), QString()); }

	static bool isVisible();
	static QString text();

protected:
	friend class CALToolTip;
	friend class CALSingleton<CALToolTip>;
};
}

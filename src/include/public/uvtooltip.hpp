#pragma once

#include <QWidget>

#include "uvwidgettoolsdef.hpp"

class CUVToolTipPrivate;

class CUVWIDGETTOOLS_EXPORT CUVToolTip : public QWidget {
	Q_OBJECT
	Q_DISABLE_COPY(CUVToolTip)
	Q_DECLARE_PRIVATE(CUVToolTip)
	Q_PROPERTY(int pBorderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(int pDisplayMsec READ getDisplayMsec WRITE setDisplayMsec NOTIFY sigDisplayMsecChanged)
	Q_PROPERTY(int pShowDelayMsec READ getShowDelayMsec WRITE setShowDelayMsec NOTIFY sigShowDelayMsecChanged)
	Q_PROPERTY(int pHideDelayMsec READ getHideDelayMsec WRITE setHideDelayMsec NOTIFY sigHideDelayMsecChanged)
	Q_PROPERTY(QString pToolTip READ getToolTip WRITE setToolTip NOTIFY sigToolTipChanged)
	Q_PROPERTY(QWidget* pCustomWidget READ getCustomWidget WRITE setCustomWidget NOTIFY sigCustomWidgetChanged)

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

	void paintEvent(QPaintEvent* event) override;
};

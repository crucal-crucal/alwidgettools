#pragma once

#include <QObject>

#include "uvwidgettoolsdef.hpp"

class QVBoxLayout;
class CUVText;
class CUVToolTip;

class CUVToolTipPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVToolTipPrivate)
	Q_DECLARE_PUBLIC(CUVToolTip)
	Q_PROPERTY(qreal opacity MEMBER opacity NOTIFY sigOpacityChanged)

public:
	explicit CUVToolTipPrivate(CUVToolTip* q, QObject* parent = nullptr);
	~CUVToolTipPrivate() override;

	void setOpacity(qreal opacity);
	[[nodiscard]] qreal getOpacity() const;

signals:
	void sigOpacityChanged();

protected:
	CUVToolTip* const q_ptr{ nullptr };

	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	CUVText* toolTipText{ nullptr };
	QVBoxLayout* mainVLayout{ nullptr };
	QWidget* customWidget{ nullptr };
	UVThemeType::ThemeMode themeMode{};
	int shadowBorderWidth{ 6 };
	int borderRadius{};
	int displayMsec{};
	int showDelayMsec{};
	int hideDelayMsec{};
	QString toolTip{};
	qreal opacity{};

	void doShowAnimation();
};

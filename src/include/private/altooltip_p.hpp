#pragma once

#include <QObject>

#include "alwidgettoolsdef.hpp"

class QVBoxLayout;
class CALText;
class CALToolTip;

class CALToolTipPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALToolTipPrivate)
	Q_DECLARE_PUBLIC(CALToolTip)
	Q_PROPERTY(qreal opacity MEMBER opacity NOTIFY sigOpacityChanged)

public:
	explicit CALToolTipPrivate(CALToolTip* q, QObject* parent = nullptr);
	~CALToolTipPrivate() override;

	void setOpacity(qreal opacity);
	[[nodiscard]] qreal getOpacity() const;

signals:
	void sigOpacityChanged();

protected:
	CALToolTip* const q_ptr{ nullptr };

	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	CALText* toolTipText{ nullptr };
	QVBoxLayout* mainVLayout{ nullptr };
	QWidget* customWidget{ nullptr };
	ALThemeType::ThemeMode themeMode{};
	int shadowBorderWidth{ 6 };
	int displayMsec{};
	int showDelayMsec{};
	int hideDelayMsec{};
	QString toolTip{};
	qreal opacity{};
	qreal borderRadius{};

	void doShowAnimation();
};

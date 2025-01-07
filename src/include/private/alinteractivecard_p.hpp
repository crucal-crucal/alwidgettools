#pragma once

#include <QPixmap>

#include "alwidgettoolsdef.hpp"

class CALInteractiveCard;

class CALInteractiveCardPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALInteractiveCardPrivate)
	Q_DECLARE_PUBLIC(CALInteractiveCard)

public:
	explicit CALInteractiveCardPrivate(CALInteractiveCard* q, QObject* parent = nullptr);
	~CALInteractiveCardPrivate() override;

protected:
	CALInteractiveCard* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	ALCardPixType::PixMode cardPixMode{};
	qreal borderRadius{};
	qreal cardPixmapBorderRadius{};
	int titlePixelSize{};
	int subTitlePixelSize{};
	int titleSpacing{};
	QString title{};
	QString subTitle{};
	QPixmap cardPixmap{};
	QSize cardPixmapSize{};
};

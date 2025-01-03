#pragma once

#include <QPixmap>

#include "uvwidgettoolsdef.hpp"

class CUVInteractiveCard;

class CUVInteractiveCardPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVInteractiveCardPrivate)
	Q_DECLARE_PUBLIC(CUVInteractiveCard)

public:
	explicit CUVInteractiveCardPrivate(CUVInteractiveCard* q, QObject* parent = nullptr);
	~CUVInteractiveCardPrivate() override;

protected:
	CUVInteractiveCard* const q_ptr{ nullptr };

private:
	UVThemeType::ThemeMode themeMode{};
	UVCardPixType::PixMode cardPixMode{};
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

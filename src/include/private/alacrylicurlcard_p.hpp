#pragma once

#include <QPixmap>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALAcrylicUrlCard;

class CALAcrylicUrlCardPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALAcrylicUrlCardPrivate)
	Q_DECLARE_PUBLIC(CALAcrylicUrlCard)

public:
	explicit CALAcrylicUrlCardPrivate(CALAcrylicUrlCard* q, QObject* parent = nullptr);
	~CALAcrylicUrlCardPrivate() override;

	Q_SLOT void slotArcylicUrlCardClicked();

protected:
	CALAcrylicUrlCard* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	ALCardPixType::PixMode cardPixMode{};
	QPixmap noisePix{};
	int brushAlpha{};
	int titlePixelSize{};
	int subTitlePixelSize{};
	int titleSpacing{};
	int subTitleSpacing{};
	qreal borderRadius{};
	qreal mainOpacity{};
	qreal noiseOpacity{};
	qreal borderWidth{};
	qreal cardPixmapBorderRadius{};
	QString title{};
	QString subTitle{};
	QString url{};
	QPixmap cardPixmap{};
	QSize cardPixmapSize{};
};
}

#pragma once

#include <QPixmap>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPopularCardFloater;
class CALPopularCard;

class CALPopularCardPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALPopularCardPrivate)
	Q_DECLARE_PUBLIC(CALPopularCard)
	Q_PROPERTY(qreal hoverYOffset READ getHoverYOffset WRITE setHoverYOffset NOTIFY sigHoverYOffsetChanged)
	Q_PROPERTY(qreal hoverOpacity READ getHoverOpacity WRITE setHoverOpacity NOTIFY sigHoverOpacityChanged)

public:
	explicit CALPopularCardPrivate(CALPopularCard* q, QObject* parent = nullptr);
	~CALPopularCardPrivate() override;

	void setHoverYOffset(qreal offset);
	[[nodiscard]] qreal getHoverYOffset() const;

	void setHoverOpacity(qreal opacity);
	[[nodiscard]] qreal getHoverOpacity() const;

Q_SIGNALS:
	Q_SIGNAL void sigHoverYOffsetChanged();
	Q_SIGNAL void sigHoverOpacityChanged();

protected:
	CALPopularCard* const q_ptr{ nullptr };

	friend class CALPopularCardFloater;

private:
	ALThemeType::ThemeMode themeMode{};
	bool isFloating{};
	int textHSpacing{};
	int textVSpacing{};
	int shadowBorderWidth{};
	qreal borderRadius{};
	qreal hoverYOffset{};
	qreal hoverOpacity{};
	QString title{};
	QString subTitle{};
	QString interactiveTips{};
	QString detailedText{};
	QString cardButtonText{};
	QPixmap cardPixmap{};
	QPixmap cardFloatPixmap{};
	QRectF interactiveTipsBaseRect{};
	QRect buttonTargetRect{};
	CALPopularCardFloater* floater{ nullptr };
	QWidget* cardFloatArea{ nullptr };
	QTimer* floatTimer{ nullptr };

	void showFloater();
};
}

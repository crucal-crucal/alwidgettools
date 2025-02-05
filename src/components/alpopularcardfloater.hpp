#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class QGraphicsOpacityEffect;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPushButton;
class CALPopularCard;
class CALPopularCardPrivate;

class CALPopularCardFloater final : public QWidget {
	Q_OBJECT
	Q_PROPERTY(qreal hoverYOffset READ getHoverYOffset WRITE setHoverYOffset NOTIFY sigHoverYOffsetChanged)
	Q_PROPERTY(qreal hoverOpacity READ getHoverOpacity WRITE setHoverOpacity NOTIFY sigHoverOpacityChanged)

public:
	explicit CALPopularCardFloater(CALPopularCard* card, CALPopularCardPrivate* cardPrivate, QWidget* parent = nullptr);
	~CALPopularCardFloater() override;

	void showFloater();
	void hideFloater();

	void setHoverYOffset(qreal offset);
	[[nodiscard]] qreal getHoverYOffset() const;

	void setHoverOpacity(qreal opacity);
	[[nodiscard]] qreal getHoverOpacity() const;

Q_SIGNALS:
	 void sigHoverYOffsetChanged();
	 void sigHoverOpacityChanged();

protected:
	friend class CALPopularCard;

	bool event(QEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

private:
	ALThemeType::ThemeMode m_themeMode{};
	bool m_isHideAnimationFinished{};
	int m_floatGeometryOffset{};
	qreal m_hoverYOffset{};
	qreal m_hoverOpacity{};
	CALPushButton* m_overButton{ nullptr };
	CALPopularCard* m_card{ nullptr };
	CALPopularCardPrivate* m_cardPrivate{ nullptr };
	QGraphicsOpacityEffect* m_opacityEffect{ nullptr };

	[[nodiscard]] QRect calculateTargetGeometry(const QRect& cardGeometry) const;
};
}

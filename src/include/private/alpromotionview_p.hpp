#pragma once

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPromotionCard;
class CALPromotionView;

class CALPromotionViewPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALPromotionViewPrivate)
	Q_DECLARE_PUBLIC(CALPromotionView)

public:
	explicit CALPromotionViewPrivate(CALPromotionView* q, QObject* parent = nullptr);
	~CALPromotionViewPrivate() override;

	void onPromationCardClicked(CALPromotionCard* clickedCard);

protected:
	CALPromotionView* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	int cardExpandWidth{};
	int cardCollapseWidth{};
	int currentIndex{};
	int autoScrollInterval{};
	int leftPadding{};
	int cardSpacing{};
	int bottomMargin{};
	int indicatorSpacing{};
	bool isAutoScroll{};
	bool isAllowSwitch{};
	QTimer* autoScrollTimer{ nullptr };
	QList<CALPromotionCard*> listPromotionCard{};
	QVector<QRectF> allpoints{};

	static void startCardGeometryAnimation(CALPromotionCard* card, const QRect& start, const QRect& end);
	static void startHorizontalCardPixmapRatioAnimation(CALPromotionCard* card, const qreal& start, const qreal& end);
	void updatePromotionCardGrometry();
	[[nodiscard]] int getAdjacentIndex(const Qt::LayoutDirection& direction, const int& index) const;
	[[nodiscard]] int getRightLimitX() const;
};
}

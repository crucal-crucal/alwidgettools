#pragma once

#include <QPixmap>
#include <QObject>

class QRadialGradient;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPromotionCard;

class CALPromotionCardPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALPromotionCardPrivate)
	Q_DECLARE_PUBLIC(CALPromotionCard)
	Q_PROPERTY(qreal pressRadius READ getPressRadius WRITE setPressRadius NOTIFY sigPressRadiusChanged)
	Q_PROPERTY(qreal hoverOpacity READ getHoverOpacity WRITE setHoverOpacity NOTIFY sigHoverOpacityChanged)
	Q_PROPERTY(qreal pressOpacity READ getPressOpacity WRITE setPressOpacity NOTIFY sigPressOpacityChanged)

public:
	explicit CALPromotionCardPrivate(CALPromotionCard* q, QObject* parent = nullptr);
	~CALPromotionCardPrivate() override;

	void setPressRadius(qreal pressRadius);
	[[nodiscard]] qreal getPressRadius() const;

	void setHoverOpacity(qreal hoverOpacity);
	[[nodiscard]] qreal getHoverOpacity() const;

	void setPressOpacity(qreal pressOpacity);
	[[nodiscard]] qreal getPressOpacity() const;

Q_SIGNALS:
	void sigPressRadiusChanged();
	void sigHoverOpacityChanged();
	void sigPressOpacityChanged();

protected:
	CALPromotionCard* const q_ptr{ nullptr };

private:
	int cardTitlePixelSize{};
	int promotionTitlePixelSize{};
	int titlePixelSize{};
	int subTitlePixelSize{};
	int shadowBorderWidth{};
	qreal borderRadius{};
	qreal horizontalCardPixmapRatio{};
	qreal verticalCardPixmapRatio{};
	qreal pressRadius{};
	qreal hoverOpacity{};
	qreal pressOpacity{};
	bool isPressAnimationFinished{};
	QPixmap cardPixmap{};
	QString cardTitle{};
	QString promotionTitle{};
	QString title{};
	QString subTitle{};
	QColor cardTitleColor{};
	QColor promotionTitleColor{};
	QColor promotionTitleBaseColor{};
	QColor titleColor{};
	QColor subTitleColor{};
	QRadialGradient* hoverGradient{ nullptr };
	QRadialGradient* pressGradient{ nullptr };

	qreal getLongestDistance(const QPoint& point);
	static qreal distance(const QPoint& point1, const QPoint& point2);
	void startHoverOpacityAnimation(bool isVisiable);
};
}

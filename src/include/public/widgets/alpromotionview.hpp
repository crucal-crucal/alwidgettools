#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPromotionCard;
class CALPromotionViewPrivate;

class CALWIDGETTOOLS_EXPORT CALPromotionView : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALPromotionView)
	Q_PROPERTY(int cardExpandWidth READ getCardExpandWidth WRITE setCardExpandWidth NOTIFY sigCardExpandWidthChanged)
	Q_PROPERTY(int cardCollapseWidth READ getCardCollapseWidth WRITE setCardCollapseWidth NOTIFY sigCardCollapseWidthChanged)
	Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY sigCurrentIndexChanged)
	Q_PROPERTY(int autoScrollInterval READ getAutoScrollInterval WRITE setAutoScrollInterval NOTIFY sigAutoScrollIntervalChanged)
	Q_PROPERTY(bool isAutoScroll READ getIsAutoScroll WRITE setIsAutoScroll NOTIFY sigIsAutoScrollChanged)

public:
	explicit CALPromotionView(QWidget* parent = nullptr);
	~CALPromotionView() override;

	void setCardExpandWidth(int width);
	[[nodiscard]] int getCardExpandWidth() const;

	void setCardCollapseWidth(int width);
	[[nodiscard]] int getCardCollapseWidth() const;

	void setCurrentIndex(int index);
	[[nodiscard]] int getCurrentIndex() const;

	void setAutoScrollInterval(int interval);
	[[nodiscard]] int getAutoScrollInterval() const;

	void setIsAutoScroll(bool isAutoScroll);
	[[nodiscard]] bool getIsAutoScroll() const;

	void appendPromotionCard(CALPromotionCard* card);

Q_SIGNALS:
	Q_SIGNAL void sigCardExpandWidthChanged();
	Q_SIGNAL void sigCardCollapseWidthChanged();
	Q_SIGNAL void sigCurrentIndexChanged();
	Q_SIGNAL void sigAutoScrollIntervalChanged();
	Q_SIGNAL void sigIsAutoScrollChanged();

protected:
	const QScopedPointer<CALPromotionViewPrivate> d_ptr{ nullptr };

	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
};
}

#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class CALPromotionCardPrivate;

class CALWIDGETTOOLS_EXPORT CALPromotionCard : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALPromotionCard)
	Q_PROPERTY(int cardTitlePixelSize READ getCardTitlePixelSize WRITE setCardTitlePixelSize NOTIFY sigCardTitlePixelSizeChanged)
	Q_PROPERTY(int promotionTitlePixelSize READ getPromotionTitlePixelSize WRITE setPromotionTitlePixelSize NOTIFY sigPromotionTitlePixelSizeChanged)
	Q_PROPERTY(int titlePixelSize READ getTitlePixelSize WRITE setTitlePixelSize NOTIFY sigTitlePixelSizeChanged)
	Q_PROPERTY(int subTitlePixelSize READ getSubTitlePixelSize WRITE setSubTitlePixelSize NOTIFY sigSubTitlePixelSizeChanged)
	Q_PROPERTY(qreal borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(qreal horizontalCardPixmapRatio READ getHorizontalCardPixmapRatio WRITE setHorizontalCardPixmapRatio NOTIFY sigHorizontalCardPixmapRatioChanged)
	Q_PROPERTY(qreal verticalCardPixmapRatio READ getVerticalCardPixmapRatio WRITE setVerticalCardPixmapRatio NOTIFY sigVerticalCardPixmapRatioChanged)
	Q_PROPERTY(QPixmap cardPixmap READ getCardPixmap WRITE setCardPixmap NOTIFY sigCardPixmapChanged)
	Q_PROPERTY(QString cardTitle READ getCardTitle WRITE setCardTitle NOTIFY sigCardTitleChanged)
	Q_PROPERTY(QString promotionTitle READ getPromotionTitle WRITE setPromotionTitle NOTIFY sigPromotionTitleChanged)
	Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY sigTitleChanged)
	Q_PROPERTY(QString subTitle READ getSubTitle WRITE setSubTitle NOTIFY sigSubTitleChanged)
	Q_PROPERTY(QColor cardTitleColor READ getCardTitleColor WRITE setCardTitleColor NOTIFY sigCardTitleColorChanged)
	Q_PROPERTY(QColor promotionTitleColor READ getPromotionTitleColor WRITE setPromotionTitleColor NOTIFY sigPromotionTitleColorChanged)
	Q_PROPERTY(QColor promotionTitleBaseColor READ getPromotionTitleBaseColor WRITE setPromotionTitleBaseColor NOTIFY sigPromotionTitleBaseColorChanged)
	Q_PROPERTY(QColor titleColor READ getTitleColor WRITE setTitleColor NOTIFY sigTitleColorChanged)
	Q_PROPERTY(QColor subTitleColor READ getSubTitleColor WRITE setSubTitleColor NOTIFY sigSubTitleColorChanged)

public:
	explicit CALPromotionCard(QWidget* parent = nullptr);
	~CALPromotionCard() override;

	void setCardTitlePixelSize(int cardTitlePixelSize);
	[[nodiscard]] int getCardTitlePixelSize() const;

	void setPromotionTitlePixelSize(int promotionTitlePixelSize);
	[[nodiscard]] int getPromotionTitlePixelSize() const;

	void setTitlePixelSize(int titlePixelSize);
	[[nodiscard]] int getTitlePixelSize() const;

	void setSubTitlePixelSize(int subTitlePixelSize);
	[[nodiscard]] int getSubTitlePixelSize() const;

	void setBorderRadius(qreal borderRadius);
	[[nodiscard]] qreal getBorderRadius() const;

	void setHorizontalCardPixmapRatio(qreal horizontalCardPixmapRatio);
	[[nodiscard]] qreal getHorizontalCardPixmapRatio() const;

	void setVerticalCardPixmapRatio(qreal verticalCardPixmapRatio);
	[[nodiscard]] qreal getVerticalCardPixmapRatio() const;

	void setCardPixmap(const QPixmap& cardPixmap);
	[[nodiscard]] QPixmap getCardPixmap() const;

	void setCardTitle(const QString& cardTitle);
	[[nodiscard]] QString getCardTitle() const;

	void setPromotionTitle(const QString& promotionTitle);
	[[nodiscard]] QString getPromotionTitle() const;

	void setTitle(const QString& title);
	[[nodiscard]] QString getTitle() const;

	void setSubTitle(const QString& subTitle);
	[[nodiscard]] QString getSubTitle() const;

	void setCardTitleColor(const QColor& cardTitleColor);
	[[nodiscard]] QColor getCardTitleColor() const;

	void setPromotionTitleColor(const QColor& promotionTitleColor);
	[[nodiscard]] QColor getPromotionTitleColor() const;

	void setPromotionTitleBaseColor(const QColor& promotionTitleBaseColor);
	[[nodiscard]] QColor getPromotionTitleBaseColor() const;

	void setTitleColor(const QColor& titleColor);
	[[nodiscard]] QColor getTitleColor() const;

	void setSubTitleColor(const QColor& subTitleColor);
	[[nodiscard]] QColor getSubTitleColor() const;

Q_SIGNALS:
	Q_SIGNAL void sigPromotionCardClicked();
	Q_SIGNAL void sigCardTitlePixelSizeChanged();
	Q_SIGNAL void sigPromotionTitlePixelSizeChanged();
	Q_SIGNAL void sigTitlePixelSizeChanged();
	Q_SIGNAL void sigSubTitlePixelSizeChanged();
	Q_SIGNAL void sigBorderRadiusChanged();
	Q_SIGNAL void sigCardPixmapChanged();
	Q_SIGNAL void sigCardTitleChanged();
	Q_SIGNAL void sigPromotionTitleChanged();
	Q_SIGNAL void sigTitleChanged();
	Q_SIGNAL void sigSubTitleChanged();
	Q_SIGNAL void sigCardTitleColorChanged();
	Q_SIGNAL void sigPromotionTitleColorChanged();
	Q_SIGNAL void sigPromotionTitleBaseColorChanged();
	Q_SIGNAL void sigTitleColorChanged();
	Q_SIGNAL void sigSubTitleColorChanged();
	Q_SIGNAL void sigHorizontalCardPixmapRatioChanged();
	Q_SIGNAL void sigVerticalCardPixmapRatioChanged();

protected:
	const QScopedPointer<CALPromotionCardPrivate> d_ptr{ nullptr };

	bool event(QEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
};

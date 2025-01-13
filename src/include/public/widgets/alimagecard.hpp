#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALImageCardPrivate;

class CALWIDGETTOOLS_EXPORT CALImageCard : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALImageCard)
	Q_PROPERTY(bool isPreserveAspectCrop READ getIsPreserveAspectCrop WRITE setIsPreserveAspectCrop NOTIFY sigIsPreserveAspectCropChanged)
	Q_PROPERTY(qreal borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(qreal maximumAspectRatio READ getMaximumAspectRatio WRITE setMaximumAspectRatio NOTIFY sigMaximumAspectRatioChanged)
	Q_PROPERTY(QImage cardImage READ getCardImage WRITE setCardImage NOTIFY sigCardImageChanged)

public:
	explicit CALImageCard(QWidget* parent = nullptr);
	~CALImageCard() override;

	void setIsPreserveAspectCrop(bool isPreserveAspectCrop);
	[[nodiscard]] bool getIsPreserveAspectCrop() const;

	void setBorderRadius(qreal borderRadius);
	[[nodiscard]] qreal getBorderRadius() const;

	void setMaximumAspectRatio(qreal maximumAspectRatio);
	[[nodiscard]] qreal getMaximumAspectRatio() const;

	void setCardImage(const QImage& cardImage);
	[[nodiscard]] QImage getCardImage() const;

Q_SIGNALS:
	Q_SIGNAL void sigIsPreserveAspectCropChanged();
	Q_SIGNAL void sigBorderRadiusChanged();
	Q_SIGNAL void sigMaximumAspectRatioChanged();
	Q_SIGNAL void sigCardImageChanged();

protected:
	const QScopedPointer<CALImageCardPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
} // namespace AL
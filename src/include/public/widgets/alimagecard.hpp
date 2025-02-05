#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
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

	/**
	 * @brief 设置是否在裁剪时保持宽高比
	 * @param isPreserveAspectCrop 是否保持宽高比
	 */
	void setIsPreserveAspectCrop(bool isPreserveAspectCrop);
	[[nodiscard]] bool getIsPreserveAspectCrop() const;

	void setBorderRadius(qreal borderRadius);
	[[nodiscard]] qreal getBorderRadius() const;

	/**
	 * @brief 设置最大宽高比
	 * @param maximumAspectRatio 最大宽高比
	 */
	void setMaximumAspectRatio(qreal maximumAspectRatio);
	[[nodiscard]] qreal getMaximumAspectRatio() const;

	/**
	 * @brief 设置当前显示的图像
	 * @param cardImage 需要显示的图像
	 */
	void setCardImage(const QImage& cardImage);
	[[nodiscard]] QImage getCardImage() const;

Q_SIGNALS:
	void sigIsPreserveAspectCropChanged();
	void sigBorderRadiusChanged();
	void sigMaximumAspectRatioChanged();
	void sigCardImageChanged();

protected:
	const QScopedPointer<CALImageCardPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
}

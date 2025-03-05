#pragma once

#include <QPushButton>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALIconButtonPrivate;
class CALIconType;

class CALWIDGETTOOLS_EXPORT CALIconButton : public QPushButton {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALIconButton)
	Q_PROPERTY(qreal borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity NOTIFY sigOpacityChanged)
	Q_PROPERTY(QColor lightHoverColor READ getLightHoverColor WRITE setLightHoverColor NOTIFY sigLightHoverColorChanged)
	Q_PROPERTY(QColor darkHoverColor READ getDarkHoverColor WRITE setDarkHoverColor NOTIFY sigDarkHoverColorChanged)
	Q_PROPERTY(QColor lightIconColor READ getLightIconColor WRITE setLightIconColor NOTIFY sigLightIconColorChanged)
	Q_PROPERTY(QColor darkIconColor READ getDarkIconColor WRITE setDarkIconColor NOTIFY sigDarkIconColorChanged)
	Q_PROPERTY(QColor lightHoverIconColor READ getLightHoverIconColor WRITE setLightHoverIconColor NOTIFY sigLightHoverIconColorChanged)
	Q_PROPERTY(QColor darkHoverIconColor READ getDarkHoverIconColor WRITE setDarkHoverIconColor NOTIFY sigDarkHoverIconColorChanged)
	Q_PROPERTY(bool isSelected READ getIsSelected WRITE setIsSelected NOTIFY sigIsSelectedChanged)

public:
	explicit CALIconButton(const QPixmap& pix, QWidget* parent = nullptr);
	explicit CALIconButton(const std::shared_ptr<CALIconType>& icon_type, QWidget* parent = nullptr);
	explicit CALIconButton(const std::shared_ptr<CALIconType>& icon_type, const int& pixelSize, QWidget* parent = nullptr);
	explicit CALIconButton(const std::shared_ptr<CALIconType>& icon_type, const int& pixelSize, const int& fixedWidth, const int& fixedHeight, QWidget* parent = nullptr);
	~CALIconButton() override;

	void setALIcon(const std::shared_ptr<CALIconType>& icon_type);

	void setPixmap(const QPixmap& pix);

	void setBorderRadius(qreal borderRadius);
	[[nodiscard]] qreal getBorderRadius() const;

	void setOpacity(qreal opacity);
	[[nodiscard]] qreal getOpacity() const;

	void setLightHoverColor(const QColor& color);
	[[nodiscard]] QColor getLightHoverColor() const;

	void setDarkHoverColor(const QColor& color);
	[[nodiscard]] QColor getDarkHoverColor() const;

	void setLightIconColor(const QColor& color);
	[[nodiscard]] QColor getLightIconColor() const;

	void setDarkIconColor(const QColor& color);
	[[nodiscard]] QColor getDarkIconColor() const;

	void setLightHoverIconColor(const QColor& color);
	[[nodiscard]] QColor getLightHoverIconColor() const;

	void setDarkHoverIconColor(const QColor& color);
	[[nodiscard]] QColor getDarkHoverIconColor() const;

	void setIsSelected(bool isSelected);
	[[nodiscard]] bool getIsSelected() const;

	/**
	 * @brief 安装 CALToolTip 悬浮提示，但前提需要先 setToolTip
	 * @param showDelay 显示延时
	 * @param position \see \enum ALToolTipType::Position
	 */
	void installToolTipFilter(int showDelay, const ALToolTipType::Position& position);

Q_SIGNALS:
	void sigBorderRadiusChanged();
	void sigOpacityChanged();
	void sigLightHoverColorChanged();
	void sigDarkHoverColorChanged();
	void sigLightIconColorChanged();
	void sigDarkIconColorChanged();
	void sigLightHoverIconColorChanged();
	void sigDarkHoverIconColorChanged();
	void sigIsSelectedChanged();

protected:
	const QScopedPointer<CALIconButtonPrivate> d_ptr{ nullptr };

	bool event(QEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
};
}

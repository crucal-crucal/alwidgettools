#pragma once

#include <QPushButton>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPushButtonPrivate;

class CALWIDGETTOOLS_EXPORT CALPushButton : public QPushButton {
	Q_OBJECT
	Q_DISABLE_COPY(CALPushButton)
	Q_DECLARE_PRIVATE(CALPushButton)
	Q_PROPERTY(qreal borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(QColor lightDefaultColor READ getLightDefaultColor WRITE setLightDefaultColor NOTIFY sigLightDefaultColorChanged)
	Q_PROPERTY(QColor darkDefaultColor READ getDarkDefaultColor WRITE setDarkDefaultColor NOTIFY sigDarkDefaultColorChanged)
	Q_PROPERTY(QColor lightHoverColor READ getLightHoverColor WRITE setLightHoverColor NOTIFY sigLightHoverColorChanged)
	Q_PROPERTY(QColor darkHoverColor READ getDarkHoverColor WRITE setDarkHoverColor NOTIFY sigDarkHoverColorChanged)
	Q_PROPERTY(QColor lightPressedColor READ getLightPressedColor WRITE setLightPressedColor NOTIFY sigLightPressedColorChanged)
	Q_PROPERTY(QColor darkPressedColor READ getDarkPressedColor WRITE setDarkPressedColor NOTIFY sigDarkPressedColorChanged)

public:
	enum ButtonStyle {
		Normal     = 0x00,
		BottomLine = 0x01,
		Shadow     = 0x02,
		Border     = 0x04,
	};

	Q_DECLARE_FLAGS(ButtonStyles, ButtonStyle)
	Q_FLAGS(ButtonStyles)

	explicit CALPushButton(QWidget* parent = nullptr);
	explicit CALPushButton(const QString& text, QWidget* parent = nullptr);
	~CALPushButton() override;

	void setButtonStyle(const ButtonStyle& buttonStyle, bool isEnable = true);
	void setButtonStyles(const ButtonStyles& buttonStyles);
	[[nodiscard]] ButtonStyles getButtonStyles() const;

	void setBorderRadius(qreal borderRadius);
	[[nodiscard]] qreal getBorderRadius() const;

	void setLightDefaultColor(const QColor& defaultColor);
	[[nodiscard]] QColor getLightDefaultColor() const;

	void setDarkDefaultColor(const QColor& defaultColor);
	[[nodiscard]] QColor getDarkDefaultColor() const;

	void setLightHoverColor(const QColor& hoverColor);
	[[nodiscard]] QColor getLightHoverColor() const;

	void setDarkHoverColor(const QColor& hoverColor);
	[[nodiscard]] QColor getDarkHoverColor() const;

	void setLightPressedColor(const QColor& pressedColor);
	[[nodiscard]] QColor getLightPressedColor() const;

	void setDarkPressedColor(const QColor& pressedColor);
	[[nodiscard]] QColor getDarkPressedColor() const;

	void setLightTextColor(const QColor& textColor);
	[[nodiscard]] QColor getLightTextColor() const;

	void setDarkTextColor(const QColor& textColor);
	[[nodiscard]] QColor getDarkTextColor() const;

	void setShadowBorderWidth(int shadowBorderWidth);
	[[nodiscard]] int getShadowBorderWidth() const;

	void setBorderWidth(qreal borderwidth);
	[[nodiscard]] qreal getBorderWidth() const;

	void setBorderColor(const QColor& borderColor);
	[[nodiscard]] QColor getBorderColor() const;

	void setTextPixelSize(int pixelSize);
	[[nodiscard]] int getTextPixelSize() const;

	void setTextPointSize(int size);
	[[nodiscard]] int getTextPointSize() const;

	void setToolTip(const QString& tooltip);

Q_SIGNALS:
	void sigBorderRadiusChanged();
	void sigLightDefaultColorChanged();
	void sigDarkDefaultColorChanged();
	void sigLightHoverColorChanged();
	void sigDarkHoverColorChanged();
	void sigLightPressedColorChanged();
	void sigDarkPressedColorChanged();

protected:
	const QScopedPointer<CALPushButtonPrivate> d_ptr{ nullptr };

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CALPushButton::ButtonStyles)
}

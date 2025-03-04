#pragma once

#include <QPixmap>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALIconButton;

class CALIconButtonPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALIconButtonPrivate)
	Q_DECLARE_PUBLIC(CALIconButton)
	Q_PROPERTY(int hoverAlpha READ getHoverAlpha WRITE setHoverAlpha NOTIFY sigHoverAlphaChanged)

public:
	explicit CALIconButtonPrivate(CALIconButton* q, QObject* parent = nullptr);
	~CALIconButtonPrivate() override;

	void setHoverAlpha(int alpha);
	[[nodiscard]] int getHoverAlpha() const;

Q_SIGNALS:
	void sigHoverAlphaChanged();

protected:
	CALIconButton* const q_ptr{ nullptr };

private:
	qreal borderRadius{};
	qreal opacity{};
	ALIcon::IconType iconType{};
	QColor lightHoverColor{};
	QColor darkHoverColor{};
	QColor lightIconColor{};
	QColor darkIconColor{};
	QColor lightHoverIconColor{};
	QColor darkHoverIconColor{};
	bool isSelected{};
	int hoverAlpha{};
	QPixmap iconPix{};
	bool isAlphaAnimationFinished{};
	ALThemeType::ThemeMode themeMode{};
};
}

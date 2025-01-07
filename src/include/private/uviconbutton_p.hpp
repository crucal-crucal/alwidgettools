#pragma once

#include <QPixmap>

#include "alwidgettoolsdef.hpp"

class CUVIconButton;
class CUVToolTip;

class CUVIconButtonPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVIconButtonPrivate)
	Q_DECLARE_PUBLIC(CUVIconButton)
	Q_PROPERTY(int hoverAlpha READ getHoverAlpha WRITE setHoverAlpha NOTIFY sigHoverAlphaChanged)

public:
	explicit CUVIconButtonPrivate(CUVIconButton* q, QObject* parent = nullptr);
	~CUVIconButtonPrivate() override;

	void setHoverAlpha(int alpha);
	[[nodiscard]] int getHoverAlpha() const;

Q_SIGNALS:
	Q_SIGNAL void sigHoverAlphaChanged();

protected:
	CUVIconButton* const q_ptr{ nullptr };

private:
	CUVToolTip* tooltip{ nullptr };
	qreal borderRadius{};
	qreal opacity{};
	ALIcon::AweSomeIcon awesomeIcon{};
	QColor lightHoverColor{};
	QColor darkHoverColor{};
	QColor lightIconColor{};
	QColor darkIconColor{};
	QColor lightHoverIconColor{};
	QColor darkHoverIconColor{};
	bool isSelected{};
	int hoverAlpha{};
	QPixmap iconPix{};
	bool isAlphaAnimationFinished{ true };
	ALThemeType::ThemeMode themeMode{};
};

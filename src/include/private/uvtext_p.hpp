#pragma once

#include "alwidgettoolsdef.hpp"

class CUVText;

class CUVTextPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVTextPrivate)
	Q_DECLARE_PUBLIC(CUVText)

public:
	explicit CUVTextPrivate(CUVText* q, QObject* parent = nullptr);
	~CUVTextPrivate() override;

	Q_SLOT void slotThemeChanged(const ALThemeType::ThemeMode& mode);

protected:
	CUVText* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	bool isWrapAnyWhere{ false };
	ALTextType::TextStyle textStyle{ ALTextType::NoStyle };
	qreal textSpacing{ 0.5 };
};

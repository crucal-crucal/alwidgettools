#pragma once

#include <QColor>

#include "uvwidgettoolsdef.hpp"

class CUVThemeManager;

class CUVThemeManagerPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVThemeManagerPrivate)
	Q_DECLARE_PUBLIC(CUVThemeManager)

public:
	explicit CUVThemeManagerPrivate(CUVThemeManager* q, QObject* parent = nullptr);
	~CUVThemeManagerPrivate() override;

protected:
	CUVThemeManager* const q_ptr{ nullptr };

private:
	UVThemeType::ThemeMode themeMode{};
	QColor lightThemeColorList[40];
	QColor darkThemeColorList[40];

	void initColorList();
};

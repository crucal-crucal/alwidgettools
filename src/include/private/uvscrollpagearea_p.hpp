#pragma once

#include "uvwidgettoolsdef.hpp"

class CUVScrollPageArea;

class CUVScrollPageAreaPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVScrollPageAreaPrivate)
	Q_DECLARE_PUBLIC(CUVScrollPageArea)

public:
	explicit CUVScrollPageAreaPrivate(CUVScrollPageArea* q, QObject* parent = nullptr);
	~CUVScrollPageAreaPrivate() override;

protected:
	CUVScrollPageArea* const q_ptr{ nullptr };

private:
	UVThemeType::ThemeMode themeMode{};
	int borderRadius{};
};

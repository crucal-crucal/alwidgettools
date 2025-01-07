#pragma once

#include "alwidgettoolsdef.hpp"

class CALScrollPageArea;

class CALScrollPageAreaPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALScrollPageAreaPrivate)
	Q_DECLARE_PUBLIC(CALScrollPageArea)

public:
	explicit CALScrollPageAreaPrivate(CALScrollPageArea* q, QObject* parent = nullptr);
	~CALScrollPageAreaPrivate() override;

protected:
	CALScrollPageArea* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	int borderRadius{};
};

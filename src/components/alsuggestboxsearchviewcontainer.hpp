#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALSuggestBoxSearchViewContainer final : public QWidget {
	Q_OBJECT

public:
	explicit CALSuggestBoxSearchViewContainer(QWidget* parent = nullptr);
	~CALSuggestBoxSearchViewContainer() override;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	ALThemeType::ThemeMode themeMode{};
};
} // namespace AL
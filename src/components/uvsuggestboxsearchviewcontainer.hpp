#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class CUVSuggestBoxSearchViewContainer final : public QWidget {
	Q_OBJECT

public:
	explicit CUVSuggestBoxSearchViewContainer(QWidget *parent = nullptr);
	~CUVSuggestBoxSearchViewContainer() override;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	ALThemeType::ThemeMode themeMode{};
};

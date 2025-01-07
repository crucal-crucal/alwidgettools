#pragma once

#include <QDialog>

#include "alappbar.hpp"
#include "alwidgettoolsdef.hpp"

class QVBoxLayout;

class CUVCustomWidget final : public QDialog {
	Q_OBJECT
	Q_TAKEOVER_NATIVEEVENT_H

public:
	explicit CUVCustomWidget(QWidget* parent = nullptr);
	~CUVCustomWidget() override;

	void setCenterWidget(QWidget* widget);

protected:
	void paintEvent(QPaintEvent* event) override;

	QVBoxLayout* m_mainVLayout{ nullptr };
	CALAppBar* m_appBar{ nullptr };
	QWidget* m_centerWidget{ nullptr };

private:
	ALThemeType::ThemeMode m_themeMode{};
	bool m_isEnableMica{};
};

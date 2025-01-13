#pragma once

#include <QDialog>

#include "alappbar.hpp"
#include "alwidgettoolsdef.hpp"

class QVBoxLayout;

namespace AL {
class CALCustomWidget final : public QDialog {
	Q_OBJECT
	AL_TAKEOVER_NATIVEEVENT_H

public:
	explicit CALCustomWidget(QWidget* parent = nullptr);
	~CALCustomWidget() override;

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

} // namespace AL

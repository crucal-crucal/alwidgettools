#pragma once

#include "alcustomwidget.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALTabBar;
class CALTabWidget;

class CALCustomTabWidget final : public CALCustomWidget {
	Q_OBJECT

public:
	explicit CALCustomTabWidget(QWidget* parent = nullptr);
	~CALCustomTabWidget() override;

	void addTab(QWidget* widget, QIcon& tabIcon, const QString& tabTitle);
	[[nodiscard]] CALTabBar* getCustomTabBar() const;
	[[nodiscard]] CALTabWidget* getCustomTabWidget() const;

private:
	CALTabBar* m_customTabBar{ nullptr };
	CALTabWidget* m_customTabWidget{ nullptr };
};
}

#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class QVBoxLayout;
class QGraphicsOpacityEffect;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALDrawerContainer final : public QWidget {
	Q_OBJECT

public:
	explicit CALDrawerContainer(QWidget* parent = nullptr);
	~CALDrawerContainer() override;

	bool addDrawer(QWidget* widget);
	bool removeDrawer(QWidget* widget);

	void doDrawerAnimation(bool isExpand);

	void setBorderRadius(int radius);
	[[nodiscard]] int getBorderRadius() const;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	ALThemeType::ThemeMode m_themeMode{};
	QList<QWidget*> m_drawerWidgetList{};
	int m_borderRadius{};
	QGraphicsOpacityEffect* m_opacityEffect{ nullptr };
	QVBoxLayout* m_mainVLayout{ nullptr };
	QVBoxLayout* m_containerVLayout{ nullptr };
	QWidget* m_containerWidget{ nullptr };
};
}

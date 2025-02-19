#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class QVBoxLayout;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALDrawerHeader final : public QWidget {
	Q_OBJECT
	Q_PROPERTY(qreal expandIconRotate MEMBER m_expandIconRotate NOTIFY sigExpandIconRotateChanged)

public:
	explicit CALDrawerHeader(QWidget* parent = nullptr);
	~CALDrawerHeader() override;

	void setHeaderWidget(QWidget* widget);
	void doExpandOrCollpaseAnimation();

	void setBorderRadius(int radius);
	[[nodiscard]] int getBorderRadius() const;

	void setExpandIconRotate(qreal rotate);
	[[nodiscard]] qreal getExpandIconRotate() const;

	void setIsExpand(bool isExpand);
	[[nodiscard]] bool getIsExpand() const;

Q_SIGNALS:
	void sigDrawerHeaderClicked(bool isExpand);
	void sigExpandIconRotateChanged();

protected:
	bool event(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

private:
	ALThemeType::ThemeMode m_themeMode{};
	int m_borderRadius{};
	qreal m_expandIconRotate{};
	bool m_isExpand{};
	bool m_isPressed{};
	QWidget* m_headerWidget{ nullptr };
	QVBoxLayout* m_mainVLayout{ nullptr };
};
}

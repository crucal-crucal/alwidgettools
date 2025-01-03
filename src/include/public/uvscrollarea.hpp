#pragma once

#include <QScrollArea>
#include <QScrollBar>
#include <QWheelEvent>

#include "uvwidgettoolsdef.hpp"

class CUVScrollBar;
class CUVScrollAreaPrivate;

class CUVWIDGETTOOLS_EXPORT CUVScrollArea : public QScrollArea {
	Q_OBJECT
	Q_DISABLE_COPY(CUVScrollArea)
	Q_DECLARE_PRIVATE(CUVScrollArea)

public:
	explicit CUVScrollArea(QWidget* parent = nullptr);
	~CUVScrollArea() override;

	/**
	 * @brief 设置是否启用抓取手势（拖动操作），默认延迟为 0.5 s
	 * @param bIsEnable 是否启用
	 * @param mousePressEventDelay 鼠标按下事件延迟[s]
	 */
	void setIsGrabGesture(bool bIsEnable, qreal mousePressEventDelay = 0.5) const;

	/**
	 * @brief 设置是否启用超出滚动区域的弹性效果
	 * @param orientation 滚动方向
	 * @param bIsEnable 是否启用
	 */
	void setIsOverShoot(const Qt::Orientation& orientation, bool bIsEnable) const;
	[[nodiscard]] bool getIsOverShoot(const Qt::Orientation& orientation) const;

	/**
	 * @brief 设置是否启用滚动动画
	 * @param orientation 滚动方向
	 * @param bIsAnimation 是否启用
	 */
	void setIsAnimation(const Qt::Orientation& orientation, bool bIsAnimation);
	[[nodiscard]] bool getIsAnimation(const Qt::Orientation& orientation) const;

	void setWheelToScrollBarEnabled(bool bEnabled);
	[[nodiscard]] bool getWheelToScrollBarEnabled() const;

	[[nodiscard]] CUVScrollBar* getHorizontalScrollBar() const;
	[[nodiscard]] CUVScrollBar* getVerticalScrollBar() const;

	bool m_bScrollBarVisible{ true };
	void setScrollBarVisible(bool bVisible) { m_bScrollBarVisible = bVisible; }

protected:
	void mousePressEvent(QMouseEvent* ev) override;
	void mouseMoveEvent(QMouseEvent* ev) override;
	void mouseReleaseEvent(QMouseEvent* ev) override;
	void wheelEvent(QWheelEvent* ev) override;
	void resizeEvent(QResizeEvent* event) override;
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;

signals:
	void sigWheelDeltaValueChangedWithoutKey(int deltaValue, QPoint curMousePnt);

protected:
	const QScopedPointer<CUVScrollAreaPrivate> d_ptr{ nullptr };

	explicit CUVScrollArea(CUVScrollAreaPrivate& dd, CUVScrollArea* parent = nullptr);
};

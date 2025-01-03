#pragma once

#include <QSlider>

#include "uvwidgettoolsdef.hpp"

class CUVSliderPrivate;

class CUVWIDGETTOOLS_EXPORT CUVSlider : public QSlider {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVSlider)

public:
	explicit CUVSlider(QWidget* parent = nullptr);
	explicit CUVSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
	~CUVSlider() override;

	/**
	 * @brief 设置滑槽默认颜色
	 * @param color 滑槽默认颜色
	 */
	void setSliderTrackColor(const QColor& color);

	/**
	 * @brief 设置滑槽禁用时颜色
	 * @param color 滑槽禁用时颜色
	 */
	void setSliderTrackDisabledColor(const QColor& color);

	/**
	 * @brief 设置滑槽滑过位置颜色
	 * @param color 滑槽滑过位置颜色
	 */
	void setSliderTrackHoverColor(const QColor& color);

	/**
	 * @brief 设置滑槽滑过位置禁用时颜色
	 * @param color 滑槽滑过位置禁用时颜色
	 */
	void setSliderTrackHoverDisabledColor(const QColor& color);

	/**
	 * @brief 设置滑块边框默认颜色
	 * @param color 滑块边框默认颜色
	 */
	void setSliderHandleBorderColor(const QColor& color);

	/**
	 * @brief 设置滑块边框禁用时颜色
	 * @param color 滑块边框禁用时颜色
	 */
	void setSliderHandleBorderDisabledColor(const QColor& color);

	/**
	 * @brief 设置滑块内圆默认颜色
	 * @param color 滑块内圆默认颜色
	 */
	void setSliderHandleInnerColor(const QColor& color);

	/**
	 * @brief 设置滑块内圆禁用时颜色
	 * @param color 滑块内圆禁用时颜色
	 */
	void setSliderHandleInnerDisabledColor(const QColor& color);

	/**
	 * @brief 设置滑块外圆默认颜色
	 * @param color 滑块外圆默认颜色
	 */
	void setSliderHandleOuterColor(const QColor& color);

	/**
	 * @brief 设置滑块外圆禁用时颜色
	 * @param color 滑块外圆禁用时颜色
	 */
	void setSliderHandleOuterDisabledColor(const QColor& color);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

signals:
	void sigLeftMousePressed();
	void sigLeftMouseMoved();
	void sigLeftMouseReleased();

protected:
	const QScopedPointer<CUVSliderPrivate> d_ptr{ nullptr };
};

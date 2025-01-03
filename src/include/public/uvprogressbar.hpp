#pragma once

#include <QProgressBar>

#include "uvwidgettoolsdef.hpp"

class CUVProgressBarPrivate;

class CUVWIDGETTOOLS_EXPORT CUVProgressBar : public QProgressBar {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVProgressBar)
	Q_PROPERTY(QColor progressColor WRITE setProgressColor READ getProgressColor)
	Q_PROPERTY(QColor backgroundColor WRITE setBackgroundColor READ getBackgroundColor)

public:
	explicit CUVProgressBar(QWidget* parent = nullptr);
	~CUVProgressBar() override;

	/**
	 * @brief 设置进度条类型
	 * @param type \see \enum UVProgressType::ProgressType
	 */
	void setProgressType(const UVProgressType::ProgressType& type);
	[[nodiscard]] UVProgressType::ProgressType getProgressType() const;

	/**
	 * @brief 设置文本显示位置
	 * @param position \see \enum UVProgressType::ProgressTextPosition
	 */
	void setProgressTextPosition(const UVProgressType::ProgressTextPosition& position);
	[[nodiscard]] UVProgressType::ProgressTextPosition getProgressTextPosition() const;

	/**
	 * @brief 设置已滑过的进度颜色
	 * @param color 已滑过的进度颜色
	 */
	void setProgressColor(const QColor& color);
	[[nodiscard]] QColor getProgressColor() const;

	/**
	 * @brief 设置背景颜色
	 * @param color 背景颜色
	 */
	void setBackgroundColor(const QColor& color);
	[[nodiscard]] QColor getBackgroundColor() const;

	/**
	 * @brief 设置圆角大小
	 * @param borderRadius 圆角大小
	 */
	void setBorderRadius(const qreal& borderRadius);
	[[nodiscard]] qreal getBorderRadius() const;

	/**
	 * @brief 设置是否显示当前进度文字
	 * @param bShow 是否显示当前进度文字
	 */
	void setShowProgressText(bool bShow);
	[[nodiscard]] bool getShowProgressText() const;

protected:
	const QScopedPointer<CUVProgressBarPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};

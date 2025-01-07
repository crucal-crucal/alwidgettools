#pragma once

#include <QProgressBar>

#include "alwidgettoolsdef.hpp"

class CALProgressBarPrivate;

class CALWIDGETTOOLS_EXPORT CALProgressBar : public QProgressBar {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALProgressBar)
	Q_PROPERTY(QColor progressColor WRITE setProgressColor READ getProgressColor)
	Q_PROPERTY(QColor backgroundColor WRITE setBackgroundColor READ getBackgroundColor)

public:
	explicit CALProgressBar(QWidget* parent = nullptr);
	~CALProgressBar() override;

	/**
	 * @brief 设置进度条类型
	 * @param type \see \enum ALProgressType::ProgressType
	 */
	void setProgressType(const ALProgressType::ProgressType& type);
	[[nodiscard]] ALProgressType::ProgressType getProgressType() const;

	/**
	 * @brief 设置文本显示位置
	 * @param position \see \enum ALProgressType::ProgressTextPosition
	 */
	void setProgressTextPosition(const ALProgressType::ProgressTextPosition& position);
	[[nodiscard]] ALProgressType::ProgressTextPosition getProgressTextPosition() const;

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
	const QScopedPointer<CALProgressBarPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};

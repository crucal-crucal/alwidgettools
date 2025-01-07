#pragma once

#include <QProgressBar>

#include "alwidgettoolsdef.hpp"

class CALCircularProgressPrivate;

/**
 * @brief 圆环形状进度控件
 */
class CALWIDGETTOOLS_EXPORT CALCircularProgress : public QProgressBar {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALCircularProgress)
	Q_PROPERTY(qreal lineWidth WRITE setLineWidth READ getLineWidth)
	Q_PROPERTY(int size WRITE setSize READ getSize)
	Q_PROPERTY(QColor color WRITE setColor READ getColor)

public:
	explicit CALCircularProgress(QWidget* parent = nullptr);
	~CALCircularProgress() override;

	/**
	 * @brief 设置进度条类型
	 * @param type \see \enum ALProgressType::ProgressType
	 */
	void setProgressType(const ALProgressType::ProgressType& type);
	[[nodiscard]] ALProgressType::ProgressType getProgressType() const;

	/**
	 * @brief 设置进度条进度宽度(画笔宽度), 仅对圆环有效
	 * @param width 进度条进度宽度
	 */
	void setLineWidth(qreal width);
	[[nodiscard]] qreal getLineWidth() const;

	/**
	 * @brief 设置进度条大小
	 * @param size 进度条大小
	 */
	void setSize(int size);
	[[nodiscard]] int getSize() const;

	/**
	 * @brief 设置进度条滑过的颜色
	 * @param color 进度条滑过的颜色
	 */
	void setColor(const QColor& color);
	[[nodiscard]] QColor getColor() const;

	/**
	 * @brief 设置进度条禁用时颜色
	 * @param color 进度条禁用时颜色
	 */
	void setDisabledColor(const QColor& color);
	[[nodiscard]] QColor getDisabledColor() const;

	/**
	 * @brief 设置是否显示百分比
	 * @param bShow 是否显示百分比
	 */
	void setShowProgressText(bool bShow);
	[[nodiscard]] bool getShowProgressText() const;

	/**
	 * @brief 设置百分比文本颜色
	 * @param color 百分比文本颜色
	 */
	void setTextColor(const QColor& color);
	[[nodiscard]] QColor getTextColor() const;

	[[nodiscard]] QSize sizeHint() const override;

protected:
	const QScopedPointer<CALCircularProgressPrivate> d_ptr{ nullptr };

	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
};

#pragma once

#include "albasedialog.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALContentDialogPrivate;

class CALWIDGETTOOLS_EXPORT CALContentDialog : public CALBaseDialog {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALContentDialog)

public:
	explicit CALContentDialog(QWidget* parent);
	~CALContentDialog() override;

	/**
	 * @brief 关闭按钮被点击时的槽函数，用户可重写自定义后续操作
	 */
	Q_SLOT virtual void slotCloseButtonClicked();

	/**
	 * @brief 取消按钮被点击时的槽函数，用户可重写自定义后续操作
	 */
	Q_SLOT virtual void slotCancelButtonClicked();

	/**
	 * @brief 设置对话框的中心控件
	 * @param centerWidget 中心控件
	 */
	void setCenterWidget(QWidget* centerWidget);

	/**
	 * @brief 设置关闭按钮文本
	 * @param text 文本
	 */
	void setCloseButtonText(const QString& text);

	/**
	 * @brief 设置取消按钮文本
	 * @param text 文本
	 */
	void setCancelButtonText(const QString& text);

Q_SIGNALS:
	Q_SIGNAL void sigCloseButtonClicked();
	Q_SIGNAL void sigCancelButtonClicked();

protected:
	const QScopedPointer<CALContentDialogPrivate> d_ptr{ nullptr };

	void keyPressEvent(QKeyEvent* event) override;
};
}

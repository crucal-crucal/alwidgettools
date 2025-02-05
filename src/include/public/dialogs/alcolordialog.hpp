#pragma once

#include <QDialog>

#include "alappbar.hpp"
#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALColorDialogPrivate;

class CALWIDGETTOOLS_EXPORT CALColorDialog : public QDialog {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALColorDialog)
	Q_PROPERTY(QColor currentColor READ getCurrentColor WRITE setCurrentColor NOTIFY sigCurrentColorChanged)
	AL_TAKEOVER_NATIVEEVENT_H

public:
	explicit CALColorDialog(QWidget* parent = nullptr);
	~CALColorDialog() override;

	/**
	 * @brief 设置当前选中颜色
	 * @param color 选中颜色
	 */
	void setCurrentColor(const QColor& color);
	[[nodiscard]] QColor getCurrentColor() const;

	/**
	 * @brief 获取自定义颜色列表
	 * @return 颜色列表
	 */
	[[nodiscard]] QList<QColor> getCustomColorList() const;

	/**
	 * @brief 获取自定义颜色列表中指定颜色
	 * @param index 颜色索引
	 * @return 指定索引的颜色
	 */
	[[nodiscard]] QColor getCustomColor(int index) const;

	/**
	 * @brief 获取当前颜色的 RGB 字符串表示
	 * @return 当前颜色的 RGB 值（如 "#RRGGBB")
	 */
	[[nodiscard]] QString getCurrentColorRgb() const;

Q_SIGNALS:
	void sigCurrentColorChanged();
	void sigColorSelected(const QColor& color);

protected:
	const QScopedPointer<CALColorDialogPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
}

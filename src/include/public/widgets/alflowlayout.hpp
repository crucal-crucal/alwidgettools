#pragma once

#include <QLayout>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALFlowLayoutPrivate;

class CALWIDGETTOOLS_EXPORT CALFlowLayout : public QLayout {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALFlowLayout)

public:
	explicit CALFlowLayout(QWidget* parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
	explicit CALFlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
	~CALFlowLayout() override;

	void addItem(QLayoutItem* item) override;
	[[nodiscard]] int horizontalSpacing() const;
	[[nodiscard]] int verticalSpacing() const;
	[[nodiscard]] Qt::Orientations expandingDirections() const override;
	[[nodiscard]] bool hasHeightForWidth() const override;
	[[nodiscard]] int heightForWidth(int width) const override;
	[[nodiscard]] int count() const override;
	[[nodiscard]] QLayoutItem* itemAt(int index) const override;
	[[nodiscard]] QSize minimumSize() const override;
	void setGeometry(const QRect& rect) override;
	[[nodiscard]] QSize sizeHint() const override;
	QLayoutItem* takeAt(int index) override;

	void setIsAnimation(bool isAnimation);

protected:
	const QScopedPointer<CALFlowLayoutPrivate> d_ptr{ nullptr };
};
}

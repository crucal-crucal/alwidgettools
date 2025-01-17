#pragma once

#include <QTableView>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALTableViewPrivate;

class CALWIDGETTOOLS_EXPORT CALTableView : public QTableView {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALTableView)
	Q_PROPERTY(int headerMargin READ getHeaderMargin WRITE setHeaderMargin NOTIFY sigHeaderMarginChanged)

public:
	explicit CALTableView(QWidget* parent = nullptr);
	~CALTableView() override;

	void setHeaderMargin(int margin);
	[[nodiscard]] int getHeaderMargin() const;

Q_SIGNALS:
	Q_SIGNAL void sigTableViewShow();
	Q_SIGNAL void sigTableViewHide();
	Q_SIGNAL void sigHeaderMarginChanged();

protected:
	const QScopedPointer<CALTableViewPrivate> d_ptr{ nullptr };

	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void leaveEvent(QEvent* event) override;
};
}

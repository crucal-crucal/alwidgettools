#pragma once

#include <QAbstractScrollArea>
#include <QScrollBar>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALScrollBarPrivate;

class CALWIDGETTOOLS_EXPORT CALScrollBar : public QScrollBar {
	Q_OBJECT
	Q_DISABLE_COPY(CALScrollBar)
	Q_DECLARE_PRIVATE(CALScrollBar)

	Q_PROPERTY(bool pIsAnimation READ getIsAnimation WRITE setIsAnimation NOTIFY sigIsAnimationChanged)
	Q_PROPERTY(qreal pSpeedLimit READ getSpeedLimit WRITE setSpeedLimit NOTIFY sigSpeedLimitChanged)

public:
	enum ContextMenuFlag {
		NoFlag  = 0x00,
		HasIcon = 0x01,
	};

	Q_DECLARE_FLAGS(ContextMenuFlags, ContextMenuFlag)
	Q_FLAGS(ContextMenuFlags)

	explicit CALScrollBar(QWidget* parent = nullptr);
	explicit CALScrollBar(Qt::Orientation orientation, QWidget* parent = nullptr);
	explicit CALScrollBar(QScrollBar* originScrollBar, QAbstractScrollArea* parent = nullptr);
	~CALScrollBar() override;

	void setIsAnimation(bool isAnimation);
	[[nodiscard]] bool getIsAnimation() const;

	void setSpeedLimit(qreal speedLimit);
	[[nodiscard]] qreal getSpeedLimit() const;

	void setContextMenuFlags(const ContextMenuFlags& flags);
	[[nodiscard]] ContextMenuFlags getContextMenuFlags() const;
	[[nodiscard]] bool hasFlag(const ContextMenuFlag& flag) const;

signals:
	void sigIsAnimationChanged();
	void sigRangeAnimationFinished();
	void sigSpeedLimitChanged();

protected:
	const QScopedPointer<CALScrollBarPrivate> d_ptr{ nullptr };

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void contextMenuEvent(QContextMenuEvent* event) override;
	bool event(QEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CALScrollBar::ContextMenuFlags)
} // namespace AL

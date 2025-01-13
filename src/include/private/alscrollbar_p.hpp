#pragma once

#include <QAbstractScrollArea>
#include <QObject>
#include <QScrollBar>

#include "alscrollbar.hpp"

class QTimer;
class QPropertyAnimation;

namespace AL {
class CALScrollBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALScrollBarPrivate)
	Q_DECLARE_PUBLIC(CALScrollBar)
	Q_PROPERTY(int targetMaximum MEMBER targetMaximum NOTIFY sigTargetMaximumChanged)

public:
	explicit CALScrollBarPrivate(CALScrollBar* q, QObject* parent = nullptr);
	~CALScrollBarPrivate() override;

	Q_SLOT void slotRangeChanged(int min, int max);

	void setTargetMaximum(int targetMaximum);
	[[nodiscard]] int getTargetMaximum() const;

Q_SIGNALS:
	void sigTargetMaximumChanged();

protected:
	CALScrollBar* const q_ptr{ nullptr };

private:
	QScrollBar* originScrollBar{ nullptr };
	QAbstractScrollArea* originScrollArea{ nullptr };
	QTimer* expandTimer{ nullptr };
	QPropertyAnimation* slideSmoothAnimation{ nullptr };
	bool isExpand{ false };
	bool isAnimation{};
	CALScrollBar::ContextMenuFlags contextMenuFlags{};
	qreal speedLimit{};
	int targetMaximum{};
	int scrollValue{ -1 };

	void scroll(const Qt::KeyboardModifiers& modifiers, int value);
	[[nodiscard]] int pixelPosToRangeValue(int pos) const;

	void initAllConfig();
	static void handleScrollBarValueChanged(QScrollBar* scrollBar, int value);
	void handleScrollBarRangeChanged(int min, int max);
	void handleScrollBarGeometry();
};
} // namespace AL
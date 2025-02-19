#pragma once

#include "alwidgettoolsdef.hpp"

class QPropertyAnimation;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALRoller;

class CALRollerPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALRollerPrivate)
	Q_DECLARE_PUBLIC(CALRoller)
	Q_PROPERTY(qreal scrollOffset MEMBER scrollOffset NOTIFY sigScrollOffsetChanged)

public:
	explicit CALRollerPrivate(CALRoller* q, QObject* parent = nullptr);
	~CALRollerPrivate() override;

Q_SIGNALS:
	void sigScrollOffsetChanged();

protected:
	CALRoller* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	QPropertyAnimation* scrollAnimation{ nullptr };
	int penBorderWidth{};
	int indicatorMargin{};
	int borderRadius{};
	int itemHeight{};
	int maxVisibleItems{};
	int currentIndex{};
	qreal targetScrollOffset{};
	qreal scrollOffset{};
	QStringList itemList{};
	QPoint mousePoint{};

	void scroll(int delta);
	void drawItem(QPainter& painter, const int& index, const int& startIndexOffset, const int& yStart, const int& yEnd, const int& centerYStart, const int& centerYEnd);
};
}

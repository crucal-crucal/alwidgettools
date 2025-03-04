#pragma once

#include <QModelIndex>

#include "alwidgettoolsdef.hpp"

class QGraphicsDropShadowEffect;
class QVBoxLayout;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALText;
class CALToolTip;

class CALToolTipPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALToolTipPrivate)
	Q_DECLARE_PUBLIC(CALToolTip)
	Q_PROPERTY(qreal opacity MEMBER opacity NOTIFY sigOpacityChanged)

public:
	explicit CALToolTipPrivate(CALToolTip* q, QObject* parent = nullptr);
	~CALToolTipPrivate() override;

	void setOpacity(qreal opacity);
	[[nodiscard]] qreal getOpacity() const;

Q_SIGNALS:
	void sigOpacityChanged();

protected:
	CALToolTip* const q_ptr{ nullptr };

	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	CALText* toolTipText{ nullptr };
	QVBoxLayout* mainVLayout{ nullptr };
	QWidget* customWidget{ nullptr };
	QTimer* timer{ nullptr };
	QPropertyAnimation* opacityAnimation{ nullptr };
	ALThemeType::ThemeMode themeMode{};
	int shadowBorderWidth{};
	int duration{};
	QString text{};
	qreal opacity{};
	qreal borderRadius{};
	bool installFilterToShow{};

	void doShowAnimation();
	void updatePos();
};

class CALToolTipPositionManager {
public:
	explicit CALToolTipPositionManager() = default;
	virtual ~CALToolTipPositionManager() = default;

	QPoint position(CALToolTip* tooltip, QWidget* parent);

	static std::unique_ptr<CALToolTipPositionManager> create(const ALToolTipType::Position& position);

protected:
	virtual QPoint calculatePosition(CALToolTip* tooltip, QWidget* parent) = 0;
};

class CALTopToolTipManager final : public CALToolTipPositionManager {
protected:
	QPoint calculatePosition(CALToolTip* tooltip, QWidget* parent) override;
};

class CALBottomToolTipManager final : public CALToolTipPositionManager {
protected:
	QPoint calculatePosition(CALToolTip* tooltip, QWidget* parent) override;
};

class CALLeftToolTipManager final : public CALToolTipPositionManager {
protected:
	QPoint calculatePosition(CALToolTip* tooltip, QWidget* parent) override;
};

class CALRightToolTipManager final : public CALToolTipPositionManager {
protected:
	QPoint calculatePosition(CALToolTip* tooltip, QWidget* parent) override;
};

class CALTopRightToolTipManager final : public CALToolTipPositionManager {
protected:
	QPoint calculatePosition(CALToolTip* tooltip, QWidget* parent) override;
};

class CALTopLeftToolTipManager final : public CALToolTipPositionManager {
protected:
	QPoint calculatePosition(CALToolTip* tooltip, QWidget* parent) override;
};

class CALBottomRightToolTipManager final : public CALToolTipPositionManager {
protected:
	QPoint calculatePosition(CALToolTip* tooltip, QWidget* parent) override;
};

class CALBottomLeftToolTipManager final : public CALToolTipPositionManager {
protected:
	QPoint calculatePosition(CALToolTip* tooltip, QWidget* parent) override;
};

class CALToolTipFilter;

class CALToolTipFilterPrivate final : QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALToolTipFilterPrivate)
	Q_DECLARE_PUBLIC(CALToolTipFilter)

public:
	explicit CALToolTipFilterPrivate(CALToolTipFilter* q, QObject* parent = nullptr);
	~CALToolTipFilterPrivate() override;

protected:
	CALToolTipFilter* const q_ptr{ nullptr };

private:
	bool isEnter{};
	CALToolTip* toolTip{ nullptr };
	int toolTipDelay{ 300 };
	ALToolTipType::Position position{};
	QTimer* timer{ nullptr };
	QWidget* parentWidget{ nullptr };
};

// class ItemViewToolTip;
//
// class ItemViewToolTipPrivate final : QObject {
// 	Q_OBJECT
// 	Q_DISABLE_COPY(ItemViewToolTipPrivate)
// 	Q_DECLARE_PUBLIC(ItemViewToolTip)
//
// public:
// 	explicit ItemViewToolTipPrivate(ItemViewToolTip* q, QObject* parent = nullptr);
// 	~ItemViewToolTipPrivate() override;
//
// protected:
// 	ItemViewToolTip* const q_ptr{ nullptr };
//
// private:
// 	QString text{};
// 	QModelIndex currentIndex{};
// 	int toolTipDuration{};
// 	ALToolTipType::ItemViewToolTipType toolTipType{};
// 	QWidget* viewport{ nullptr };
// };
}

#pragma once

#include <QWidget>

#include "alsingleton.hpp"
#include "alwidgettoolsdef.hpp"

class QAbstractItemView;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALToolTipPrivate;

class CALWIDGETTOOLS_EXPORT CALToolTip : public QWidget {
	Q_OBJECT
	Q_DISABLE_COPY(CALToolTip)
	Q_DECLARE_PRIVATE(CALToolTip)
	Q_PROPERTY(qreal borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(QString text READ getText WRITE setText NOTIFY sigTextChanged)
	Q_PROPERTY(QWidget* customWidget READ getCustomWidget WRITE setCustomWidget NOTIFY sigCustomWidgetChanged)

public:
	explicit CALToolTip(QWidget* parent = nullptr, bool installFilterToShow = true);
	~CALToolTip() override;

	void setBorderRadius(qreal borderRadius);
	[[nodiscard]] qreal getBorderRadius() const;

	void setText(const QString& text);
	[[nodiscard]] QString getText() const;

	void setDuration(int duration);
	[[nodiscard]] int getDuration() const;

	void setCustomWidget(QWidget* customWidget);
	[[nodiscard]] QWidget* getCustomWidget() const;

	void updatePos();
	void adjustPos(QWidget* widget, const ALToolTipType::Position& position);

Q_SIGNALS:
	void sigBorderRadiusChanged();
	void sigTextChanged();
	void sigCustomWidgetChanged();

protected:
	const QScopedPointer<CALToolTipPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;
};

class CALToolTipFilterPrivate;

class CALWIDGETTOOLS_EXPORT CALToolTipFilter : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALToolTipFilter)
	Q_DECLARE_PRIVATE(CALToolTipFilter)

public:
	explicit CALToolTipFilter(QWidget* parent, int showDelay = 300, const ALToolTipType::Position& position = ALToolTipType::Position::Top);
	~CALToolTipFilter() override;

	void hideToolTip();
	void setToolTipDelay(int delay);

protected:
	const QScopedPointer<CALToolTipFilterPrivate> d_ptr{ nullptr };

	bool eventFilter(QObject* watched, QEvent* event) override;

	virtual CALToolTip* createToolTip();
	[[nodiscard]] virtual bool canShowToolTip() const;

private:
	Q_SLOT void slotShowToolTip();
};

// class ItemViewToolTipPrivate;
//
// class CALWIDGETTOOLS_EXPORT ItemViewToolTip : public ToolTip {
// 	Q_OBJECT
// 	Q_DISABLE_COPY(ItemViewToolTip)
// 	Q_DECLARE_PRIVATE(ItemViewToolTip)
//
// public:
// 	explicit ItemViewToolTip(const QString& text = QString(), QWidget* parent = nullptr);
// 	~ItemViewToolTip() override;
//
// 	void adjustPos(QAbstractItemView* view, const QRect& itemRect, const ALToolTipType::ItemViewToolTipType& toolTipType);
//
// protected:
// 	const QScopedPointer<ItemViewToolTipPrivate> d_ptr{ nullptr };
// };
}

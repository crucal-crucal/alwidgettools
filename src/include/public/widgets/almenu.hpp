#pragma once


#include <QMenu>
#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALMenuPrivate;
class CALIconType;

class CALWIDGETTOOLS_EXPORT CALMenu : public QMenu {
	Q_OBJECT
	Q_DISABLE_COPY(CALMenu)
	Q_DECLARE_PRIVATE(CALMenu)

public:
	explicit CALMenu(QWidget* parent = nullptr);
	explicit CALMenu(const QString& title, QWidget* parent = nullptr);
	~CALMenu() override;

	void setMenuItemHeight(int menuItemHeight);
	[[nodiscard]] int getMenuItemHeight() const;

	QAction* addMenu(QMenu* menu);
	CALMenu* addMenu(const QString& title);
	CALMenu* addMenu(const QIcon& icon, const QString& title);

	using QWidget::addAction;
	QAction* addAction(const QString& text);
	QAction* addAction(const QIcon& icon, const QString& text);
	QAction* addAction(const std::unique_ptr<CALIconType>& icon_type, const QString& text);
	QAction* addAction(const QIcon& icon, const QString& text, const QKeySequence& shortcut);
	QAction* addAction(const std::unique_ptr<CALIconType>& icon_type, const QString& text, const QKeySequence& shortcut);
	QAction* addAction(const QIcon& icon, const QString& text, const QObject* receiver, const char* member, const QKeySequence& shortcut = 0);
	QAction* addAction(const std::unique_ptr<CALIconType>& icon_type, const QString& text, const QObject* receiver, const char* member, const QKeySequence& shortcut = 0);


	[[nodiscard]] bool isHasChildMenu() const;
	[[nodiscard]] bool isHasIcon() const;

Q_SIGNALS:
	void sigMenuShow();

protected:
	const QScopedPointer<CALMenuPrivate> d_ptr{ nullptr };

	void showEvent(QShowEvent* event) override;;
	void paintEvent(QPaintEvent* event) override;
};
}

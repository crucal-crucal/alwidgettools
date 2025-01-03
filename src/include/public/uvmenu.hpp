#pragma once


#include <QMenu>
#include <QWidget>

#include "uvwidgettoolsdef.hpp"

class CUVMenuPrivate;

class CUVWIDGETTOOLS_EXPORT CUVMenu : public QMenu {
	Q_OBJECT
	Q_DISABLE_COPY(CUVMenu)
	Q_DECLARE_PRIVATE(CUVMenu)

public:
	explicit CUVMenu(QWidget* parent = nullptr);
	explicit CUVMenu(const QString& title, QWidget* parent = nullptr);
	~CUVMenu() override;

	void setMenuItemHeight(int menuItemHeight);
	[[nodiscard]] int getMenuItemHeight() const;

	QAction* addMenu(QMenu* menu);
	CUVMenu* addMenu(const QString& title);
	CUVMenu* addMenu(const QIcon& icon, const QString& title);
	CUVMenu* addMenu(const UVIcon::CUVAweSomeIcon& icon, const QString& title);

	using QWidget::addAction;
	QAction* addAction(const QString& text);
	QAction* addAction(const UVIcon::CUVAweSomeIcon& icon, const QString& text);
	QAction* addAction(const QIcon& icon, const QString& text);
	QAction* addAction(const UVIcon::CUVAweSomeIcon& icon, const QString& text, const QKeySequence& shortcut);
	QAction* addAction(const QIcon& icon, const QString& text, const QKeySequence& shortcut);
	QAction* addAction(const UVIcon::CUVAweSomeIcon& icon, const QString& text, const QObject* receiver, const char* member, const QKeySequence& shortcut = 0);
	QAction* addAction(const QIcon& icon, const QString& text, const QObject* receiver, const char* member, const QKeySequence& shortcut = 0);


	[[nodiscard]] bool isHasChildMenu() const;
	[[nodiscard]] bool isHasIcon() const;

signals:
	void sigMenuShow();

protected:
	const QScopedPointer<CUVMenuPrivate> d_ptr{ nullptr };

	void showEvent(QShowEvent* event) override;;
	void paintEvent(QPaintEvent* event) override;
};

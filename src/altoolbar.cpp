#include "include/public/altoolbar.hpp"

#include <QLayout>
#include <QPainter>
#include <QStyleOptionToolBar>

#include "alicon.hpp"
#include "althememanager.hpp"
#include "altoolbarstyle.hpp"
#include "altoolbar_p.hpp"

/**
 * @brief \class CALToolBarPrivate
 * Internal class for CALToolBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALToolBarPrivate::CALToolBarPrivate(CALToolBar* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALToolBarPrivate::~CALToolBarPrivate() = default;

/**
 * @brief \class CALToolBar
 * @param parent pointer to the parent class
 */
CALToolBar::CALToolBar(QWidget* parent): QToolBar(parent), d_ptr(new CALToolBarPrivate(this, this)) {
	Q_D(CALToolBar);

	d->shadowBorderWidth = 6;
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) {
		d->themeMode = mode;
		if (this->isFloating()) {
			update();
		}
	});
	d->style = new CALToolBarStyle(style());

	setObjectName("CALToolBar");
	setStyle(d->style);
	layout()->setSpacing(10);
	layout()->setContentsMargins(3, 3, 3, 3);
	setAttribute(Qt::WA_TranslucentBackground);
	connect(this, &CALToolBar::topLevelChanged, this, [=](const bool topLevel) {
		if (topLevel) {
			layout()->setContentsMargins(d->shadowBorderWidth + 3, d->shadowBorderWidth + 3, d->shadowBorderWidth + 3, d->shadowBorderWidth + 3);
		} else {
			layout()->setContentsMargins(3, 3, 3, 3);
		}
	});
}

CALToolBar::CALToolBar(const QString& title, QWidget* parent): CALToolBar(parent) {
	setWindowTitle(title);
}

CALToolBar::~CALToolBar() = default;

void CALToolBar::setToolBarSpacing(const int spacing) const {
	layout()->setSpacing(spacing);
}

int CALToolBar::getToolBarSpacing() const {
	return layout()->spacing();
}

QAction* CALToolBar::addAweIconAction(const ALIcon::AweSomeIcon& awesomeicon, const QString& text) {
	const auto action = new QAction(text, this);
	action->setProperty("CALIconType", QChar(static_cast<unsigned short>(awesomeicon)));
	action->setIcon(CALIcon::getQIconFromAwesomeIcon(ALIcon::AweSomeIcon::Broom, 1));
	addAction(action);
	return action;
}

QAction* CALToolBar::addAweIconAction(const ALIcon::AweSomeIcon& awesomeicon, const QString& text, const QKeySequence& shortcut) {
	const auto action = new QAction(text, this);
	action->setShortcut(shortcut);
	action->setProperty("CALIconType", QChar(static_cast<unsigned short>(awesomeicon)));
	action->setIcon(CALIcon::getQIconFromAwesomeIcon(ALIcon::AweSomeIcon::Broom, 1));
	addAction(action);
	return action;
}

void CALToolBar::paintEvent(QPaintEvent* event) {
	Q_D(CALToolBar);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	if (isFloating()) {
		// shadow
		ALTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, 6);
		// background
		painter.setPen(ALThemeColor(d->themeMode, ALThemeType::PopupBorder));
		painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::DialogBase));
		const QRect foregroundRect(d->shadowBorderWidth, d->shadowBorderWidth, width() - 2 * d->shadowBorderWidth, height() - 2 * d->shadowBorderWidth);
		painter.drawRoundedRect(foregroundRect, 5, 5);
		const QStyle* style = this->style();
		QStyleOptionToolBar opt;
		initStyleOption(&opt);
		opt.rect = style->subElementRect(QStyle::SE_ToolBarHandle, &opt, this);
		if (opt.rect.isValid()) {
			style->drawPrimitive(QStyle::PE_IndicatorToolBarHandle, &opt, &painter, this);
		}
	} else {
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::transparent);
		painter.drawRoundedRect(rect(), 5, 5);
		QToolBar::paintEvent(event);
	}
	painter.restore();
}

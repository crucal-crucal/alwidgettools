#include "alsuggestbox.hpp"

#include <QPropertyAnimation>
#include <QVBoxLayout>

#include "albaselistview.hpp"
#include "alicon.hpp"
#include "allineedit.hpp"
#include "alscrollbar.hpp"
#include "alsuggestboxsearchviewcontainer.hpp"
#include "alsuggestbox_p.hpp"
#include "alsuggestdelegate.hpp"
#include "alsuggestmodel.hpp"
#include "althememanager.hpp"

/**
 * @brief \class CALSuggestion
 * Internal class for CALSuggestBox
 * @param parent pointer to the parent class
 */
CALSuggestion::CALSuggestion(QObject* parent): QObject(parent) {
	awesomeIcon = ALIcon::AweSomeIcon::None;
	suggestText = "";
	suggestData = QVariantMap();
}

CALSuggestion::~CALSuggestion() = default;

void CALSuggestion::setAwesomeIcon(const ALIcon::AweSomeIcon& icon) {
	awesomeIcon = icon;
	Q_EMIT sigAwesomeIconChanged();
}

ALIcon::AweSomeIcon CALSuggestion::getAwesomeIcon() const {
	return awesomeIcon;
}

void CALSuggestion::setSuggestText(const QString& text) {
	suggestText = text;
	Q_EMIT sigSuggestTextChanged();
}

QString CALSuggestion::getSuggestText() const {
	return suggestText;
}

void CALSuggestion::setSuggestData(const QVariantMap& data) {
	suggestData = data;
	Q_EMIT sigSuggestDataChanged();
}

QVariantMap CALSuggestion::getSuggestData() const {
	return suggestData;
}

/**
 * @brief \class CALSuggestBoxPrivate
 * Internal class for CALSuggestBox
 * @param q pointer to the public class
 * @param parent pointer to the parent clss
 */
CALSuggestBoxPrivate::CALSuggestBoxPrivate(CALSuggestBox* q, QObject* parent): QObject(parent), q_ptr(q) {
	isExpandAnimationFinished = true;
	isCloseAnimationFinished = true;
}

CALSuggestBoxPrivate::~CALSuggestBoxPrivate() = default;

void CALSuggestBoxPrivate::slotThemeModeChanged(const ALThemeType::ThemeMode& mode) {
	themeMode = mode;
	searchLineEdit->removeAction(themeMode == ALThemeType::Light ? darkSearchAction : lightSearchAction);
	searchLineEdit->addAction(themeMode == ALThemeType::Light ? lightSearchAction : darkSearchAction, QLineEdit::TrailingPosition);
	searchLineEdit->update();
}

void CALSuggestBoxPrivate::slotSearchEditTextEdit(const QString& searchText) {
	Q_Q(CALSuggestBox);

	if (searchText.isEmpty()) {
		startCloseAnimation();
		return;
	}

	QVector<CALSuggestion*> suggestions;
	for (const auto& suggest : suggestionVector) {
		if (suggest->getSuggestText().contains(searchText, caseSensitivity)) {
			suggestions.append(suggest);
		}
	}

	if (!suggestions.isEmpty()) {
		searchModel->setSearchSuggestion(suggestions);
		const int rowCount = std::min(suggestions.count(), 4);
		if (!searchViewContainer->isVisible()) {
			q->raise();
			searchViewContainer->show();
			searchViewContainer->raise();
			if (const QPoint cyclePoint = searchViewContainer->mapFromGlobal(q->mapToGlobal(QPoint(-5, q->height()))); cyclePoint != QPoint(0, 0)) {
				searchViewContainer->move(cyclePoint);
			}
			startSizeAnimation(QSize(q->width() + 10, 0), QSize(q->width() + 10, 40 * rowCount + 16));
			searchView->move(searchView->x(), -(40 * rowCount + 16));
		} else {
			startSizeAnimation(searchViewContainer->size(), QSize(q->width() + 12, 40 * rowCount + 16));
		}
		startExpandAnimation();
	} else {
		startCloseAnimation();
	}
}

void CALSuggestBoxPrivate::slotSearchViewClicked(const QModelIndex& index) {
	Q_Q(CALSuggestBox);

	searchLineEdit->clear();
	searchView->clearSelection();
	if (!index.isValid()) {
		return;
	}
	const CALSuggestion* suggest = searchModel->getSearchSuggestion(index.row());
	Q_EMIT q->sigSuggestionClicked(suggest->getSuggestText(), suggest->getSuggestData());
	startCloseAnimation();
}

void CALSuggestBoxPrivate::startSizeAnimation(const QSize& oldSize, const QSize& newSize) {
	if (lastSize.isValid() && lastSize == newSize) {
		return;
	}

	shadowVLayout->removeWidget(searchView);
	const auto sizeAnimation = new QPropertyAnimation(searchViewContainer, "size");
	connect(sizeAnimation, &QPropertyAnimation::valueChanged, this, [=]() { searchView->resize(searchViewContainer->size()); });
	connect(sizeAnimation, &QPropertyAnimation::finished, this, [=]() { shadowVLayout->addWidget(searchView); });
	sizeAnimation->setDuration(300);
	sizeAnimation->setEasingCurve(QEasingCurve::InOutSine);
	sizeAnimation->setStartValue(oldSize);
	sizeAnimation->setEndValue(newSize);
	sizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	lastSize = newSize;
}

void CALSuggestBoxPrivate::startExpandAnimation() {
	if (!isExpandAnimationFinished) {
		return;
	}

	isCloseAnimationFinished = true;
	isExpandAnimationFinished = false;
	const auto expandAnimation = new QPropertyAnimation(searchView, "pos");
	connect(expandAnimation, &QPropertyAnimation::finished, this, [=]() {
		isExpandAnimationFinished = true;
		searchView->clearSelection();
	});
	expandAnimation->setDuration(300);
	expandAnimation->setEasingCurve(QEasingCurve::InOutSine);
	expandAnimation->setStartValue(searchView->pos());
	expandAnimation->setStartValue(QPoint(8, 8));
	expandAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALSuggestBoxPrivate::startCloseAnimation() {
	if (!isCloseAnimationFinished) {
		return;
	}

	isExpandAnimationFinished = true;
	isCloseAnimationFinished = false;
	const auto sizeAnimation = new QPropertyAnimation(searchViewContainer, "size");
	sizeAnimation->setDuration(300);
	sizeAnimation->setEasingCurve(QEasingCurve::InOutSine);
	sizeAnimation->setStartValue(searchViewContainer->size());
	sizeAnimation->setEndValue(QSize(searchViewContainer->width(), 0));
	sizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	const auto closeAnimation = new QPropertyAnimation(searchView, "pos");
	connect(closeAnimation, &QPropertyAnimation::finished, this, [=]() {
		isCloseAnimationFinished = true;
		searchModel->clearSearchNode();
		searchViewContainer->hide();
	});
	closeAnimation->setDuration(300);
	closeAnimation->setEasingCurve(QEasingCurve::InOutSine);
	closeAnimation->setStartValue(searchView->pos());
	closeAnimation->setEndValue(QPoint(searchView->pos().x(), -searchView->height()));
	closeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	lastSize = sizeAnimation->endValue().toSize();
}

/**
 * @brief \class CALSuggestBox
 * @param parent pointer to the parent class
 */
CALSuggestBox::CALSuggestBox(QWidget* parent): QWidget(parent), d_ptr(new CALSuggestBoxPrivate(this, this)) {
	Q_D(CALSuggestBox);

	setFixedSize(280, 35);
	d->borderRadius = 6;
	d->caseSensitivity = Qt::CaseInsensitive;

	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, d, &CALSuggestBoxPrivate::slotThemeModeChanged);

	d->lightSearchAction = new QAction(CALIcon::getQIconFromAwesomeIcon(ALIcon::AweSomeIcon::MagnifyingGlass), tr("search"), this);
	d->darkSearchAction = new QAction(CALIcon::getQIconFromAwesomeIcon(ALIcon::AweSomeIcon::MagnifyingGlass, QColor(0xFF, 0xFF, 0xFF)), tr("search"), this);

	d->searchLineEdit = new CALLineEdit(this);
	d->searchLineEdit->setFixedHeight(35);
	d->searchLineEdit->setPlaceholderText(tr("search Feature"));
	d->searchLineEdit->setClearButtonEnabled(true);
	d->searchLineEdit->addAction(d->themeMode == ALThemeType::Light ? d->lightSearchAction : d->darkSearchAction, QLineEdit::TrailingPosition);
	connect(d->searchLineEdit, &CALLineEdit::textChanged, d, &CALSuggestBoxPrivate::slotSearchEditTextEdit);
	connect(d->searchLineEdit, &CALLineEdit::sigFocusIn, d, &CALSuggestBoxPrivate::slotSearchEditTextEdit);
	connect(d->searchLineEdit, &CALLineEdit::sigWmFocusOut, this, [d]() { d->startCloseAnimation(); });

	const auto mainVlayout = new QVBoxLayout(this);
	mainVlayout->setSpacing(0);
	mainVlayout->setContentsMargins(0, 0, 0, 0);
	mainVlayout->addWidget(d->searchLineEdit);
	d->searchViewContainer = new CALSuggestBoxSearchViewContainer(window());
	d->searchViewContainer->resize(292, 300);

	d->searchModel = new CALSuggestModel(this);
	d->searchDelegate = new CALSuggestDelegate(this);

	d->searchView = new CALBaseListView(d->searchViewContainer);
	d->searchView->setModel(d->searchModel);
	d->searchView->setItemDelegate(d->searchDelegate);
	connect(d->searchView, &CALBaseListView::clicked, d, &CALSuggestBoxPrivate::slotSearchViewClicked);

	const auto floatVScrollBar = new CALScrollBar(d->searchView->verticalScrollBar(), d->searchView);
	floatVScrollBar->setIsAnimation(true);

	d->shadowVLayout = new QVBoxLayout(d->searchViewContainer);
	d->shadowVLayout->setContentsMargins(0, 0, 0, 0);
	d->shadowVLayout->addWidget(d->searchView);

	d->searchViewContainer->hide();
}

CALSuggestBox::~CALSuggestBox() = default;

void CALSuggestBox::setBorderRadius(const int borderRadius) {
	d_func()->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

int CALSuggestBox::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALSuggestBox::setCaseSensitivity(const Qt::CaseSensitivity& caseSensitivity) {
	d_func()->caseSensitivity = caseSensitivity;
	Q_EMIT sigCaseSensitivityChanged();
}

Qt::CaseSensitivity CALSuggestBox::getCaseSensitivity() const {
	return d_func()->caseSensitivity;
}

void CALSuggestBox::setPlaceholderText(const QString& placaHolderText) {
	d_func()->searchLineEdit->setPlaceholderText(placaHolderText);
}

void CALSuggestBox::addSuggestion(const QString& suggestText, const QVariantMap& suggestData) {
	const auto suggest = new CALSuggestion(this);
	suggest->setSuggestText(suggestText);
	suggest->setSuggestData(suggestData);
	d_func()->suggestionVector.append(suggest);
}

void CALSuggestBox::addSuggestion(const ALIcon::AweSomeIcon& awesomeIcon, const QString& suggestText, const QVariantMap& suggestData) {
	const auto suggest = new CALSuggestion(this);
	suggest->setAwesomeIcon(awesomeIcon);
	suggest->setSuggestText(suggestText);
	suggest->setSuggestData(suggestData);
	d_func()->suggestionVector.append(suggest);
}

void CALSuggestBox::removeSuggestion(const QString& suggestText) {
	Q_D(CALSuggestBox);

	for (const auto& suggest : d->suggestionVector) {
		if (suggest->getSuggestText() == suggestText) {
			d->suggestionVector.removeOne(suggest);
			suggest->deleteLater();
			break;
		}
	}
}

void CALSuggestBox::removeSuggestion(const int index) {
	Q_D(CALSuggestBox);

	if (index >= d->suggestionVector.count()) {
		return;
	}

	CALSuggestion* suggest = d->suggestionVector[index];
	d->suggestionVector.removeOne(suggest);
	suggest->deleteLater();
}

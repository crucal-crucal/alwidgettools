#include "uvsuggestbox.hpp"

#include <QPropertyAnimation>
#include <QVBoxLayout>

#include "uvbaselistview.hpp"
#include "uvicon.hpp"
#include "uvlineedit.hpp"
#include "uvscrollbar.hpp"
#include "uvsuggestboxsearchviewcontainer.hpp"
#include "uvsuggestbox_p.hpp"
#include "uvsuggestdelegate.hpp"
#include "uvsuggestmodel.hpp"
#include "uvthememanager.hpp"

/**
 * @brief \class CUVSuggestion
 * Internal class for CUVSuggestBox
 * @param parent pointer to the parent class
 */
CUVSuggestion::CUVSuggestion(QObject* parent): QObject(parent) {
	awesomeIcon = UVIcon::CUVAweSomeIcon::None;
	suggestText = "";
	suggestData = QVariantMap();
}

CUVSuggestion::~CUVSuggestion() = default;

void CUVSuggestion::setAwesomeIcon(const UVIcon::CUVAweSomeIcon& icon) {
	awesomeIcon = icon;
	Q_EMIT sigAwesomeIconChanged();
}

UVIcon::CUVAweSomeIcon CUVSuggestion::getAwesomeIcon() const {
	return awesomeIcon;
}

void CUVSuggestion::setSuggestText(const QString& text) {
	suggestText = text;
	Q_EMIT sigSuggestTextChanged();
}

QString CUVSuggestion::getSuggestText() const {
	return suggestText;
}

void CUVSuggestion::setSuggestData(const QVariantMap& data) {
	suggestData = data;
	Q_EMIT sigSuggestDataChanged();
}

QVariantMap CUVSuggestion::getSuggestData() const {
	return suggestData;
}

/**
 * @brief \class CUVSuggestBoxPrivate
 * Internal class for CUVSuggestBox
 * @param q pointer to the public class
 * @param parent pointer to the parent clss
 */
CUVSuggestBoxPrivate::CUVSuggestBoxPrivate(CUVSuggestBox* q, QObject* parent): QObject(parent), q_ptr(q) {
	isExpandAnimationFinished = true;
	isCloseAnimationFinished = true;
}

CUVSuggestBoxPrivate::~CUVSuggestBoxPrivate() = default;

void CUVSuggestBoxPrivate::slotThemeModeChanged(const UVThemeType::ThemeMode& mode) {
	themeMode = mode;
	searchLineEdit->removeAction(themeMode == UVThemeType::Light ? darkSearchAction : lightSearchAction);
	searchLineEdit->addAction(themeMode == UVThemeType::Light ? lightSearchAction : darkSearchAction, QLineEdit::TrailingPosition);
	searchLineEdit->update();
}

void CUVSuggestBoxPrivate::slotSearchEditTextEdit(const QString& searchText) {
	Q_Q(CUVSuggestBox);

	if (searchText.isEmpty()) {
		startCloseAnimation();
		return;
	}

	QVector<CUVSuggestion*> suggestions;
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

void CUVSuggestBoxPrivate::slotSearchViewClicked(const QModelIndex& index) {
	Q_Q(CUVSuggestBox);

	searchLineEdit->clear();
	searchView->clearSelection();
	if (!index.isValid()) {
		return;
	}
	const CUVSuggestion* suggest = searchModel->getSearchSuggestion(index.row());
	Q_EMIT q->sigSuggestionClicked(suggest->getSuggestText(), suggest->getSuggestData());
	startCloseAnimation();
}

void CUVSuggestBoxPrivate::startSizeAnimation(const QSize& oldSize, const QSize& newSize) {
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

void CUVSuggestBoxPrivate::startExpandAnimation() {
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

void CUVSuggestBoxPrivate::startCloseAnimation() {
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
 * @brief \class CUVSuggestBox
 * @param parent pointer to the parent class
 */
CUVSuggestBox::CUVSuggestBox(QWidget* parent): QWidget(parent), d_ptr(new CUVSuggestBoxPrivate(this, this)) {
	Q_D(CUVSuggestBox);

	setFixedSize(280, 35);
	d->borderRadius = 6;
	d->caseSensitivity = Qt::CaseInsensitive;

	d->themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, d, &CUVSuggestBoxPrivate::slotThemeModeChanged);

	d->lightSearchAction = new QAction(CUVIcon::getQIconFromAwesomeIcon(UVIcon::CUVAweSomeIcon::MagnifyingGlass), tr("search"), this);
	d->darkSearchAction = new QAction(CUVIcon::getQIconFromAwesomeIcon(UVIcon::CUVAweSomeIcon::MagnifyingGlass, QColor(0xFF, 0xFF, 0xFF)), tr("search"), this);

	d->searchLineEdit = new CUVLineEdit(this);
	d->searchLineEdit->setFixedHeight(35);
	d->searchLineEdit->setPlaceholderText(tr("search Feature"));
	d->searchLineEdit->setClearButtonEnabled(true);
	d->searchLineEdit->addAction(d->themeMode == UVThemeType::Light ? d->lightSearchAction : d->darkSearchAction, QLineEdit::TrailingPosition);
	connect(d->searchLineEdit, &CUVLineEdit::textChanged, d, &CUVSuggestBoxPrivate::slotSearchEditTextEdit);
	connect(d->searchLineEdit, &CUVLineEdit::sigFocusIn, d, &CUVSuggestBoxPrivate::slotSearchEditTextEdit);
	connect(d->searchLineEdit, &CUVLineEdit::sigWmFocusOut, this, [d]() { d->startCloseAnimation(); });

	const auto mainVlayout = new QVBoxLayout(this);
	mainVlayout->setSpacing(0);
	mainVlayout->setContentsMargins(0, 0, 0, 0);
	mainVlayout->addWidget(d->searchLineEdit);
	d->searchViewContainer = new CUVSuggestBoxSearchViewContainer(window());
	d->searchViewContainer->resize(292, 300);

	d->searchModel = new CUVSuggestModel(this);
	d->searchDelegate = new CUVSuggestDelegate(this);

	d->searchView = new CUVBaseListView(d->searchViewContainer);
	d->searchView->setModel(d->searchModel);
	d->searchView->setItemDelegate(d->searchDelegate);
	connect(d->searchView, &CUVBaseListView::clicked, d, &CUVSuggestBoxPrivate::slotSearchViewClicked);

	const auto floatVScrollBar = new CUVScrollBar(d->searchView->verticalScrollBar(), d->searchView);
	floatVScrollBar->setIsAnimation(true);

	d->shadowVLayout = new QVBoxLayout(d->searchViewContainer);
	d->shadowVLayout->setContentsMargins(0, 0, 0, 0);
	d->shadowVLayout->addWidget(d->searchView);

	d->searchViewContainer->hide();
}

CUVSuggestBox::~CUVSuggestBox() = default;

void CUVSuggestBox::setBorderRadius(const int borderRadius) {
	d_func()->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

int CUVSuggestBox::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CUVSuggestBox::setCaseSensitivity(const Qt::CaseSensitivity& caseSensitivity) {
	d_func()->caseSensitivity = caseSensitivity;
	Q_EMIT sigCaseSensitivityChanged();
}

Qt::CaseSensitivity CUVSuggestBox::getCaseSensitivity() const {
	return d_func()->caseSensitivity;
}

void CUVSuggestBox::setPlaceholderText(const QString& placaHolderText) {
	d_func()->searchLineEdit->setPlaceholderText(placaHolderText);
}

void CUVSuggestBox::addSuggestion(const QString& suggestText, const QVariantMap& suggestData) {
	const auto suggest = new CUVSuggestion(this);
	suggest->setSuggestText(suggestText);
	suggest->setSuggestData(suggestData);
	d_func()->suggestionVector.append(suggest);
}

void CUVSuggestBox::addSuggestion(const UVIcon::CUVAweSomeIcon& awesomeIcon, const QString& suggestText, const QVariantMap& suggestData) {
	const auto suggest = new CUVSuggestion(this);
	suggest->setAwesomeIcon(awesomeIcon);
	suggest->setSuggestText(suggestText);
	suggest->setSuggestData(suggestData);
	d_func()->suggestionVector.append(suggest);
}

void CUVSuggestBox::removeSuggestion(const QString& suggestText) {
	Q_D(CUVSuggestBox);

	for (const auto& suggest : d->suggestionVector) {
		if (suggest->getSuggestText() == suggestText) {
			d->suggestionVector.removeOne(suggest);
			suggest->deleteLater();
			break;
		}
	}
}

void CUVSuggestBox::removeSuggestion(const int index) {
	Q_D(CUVSuggestBox);

	if (index >= d->suggestionVector.count()) {
		return;
	}

	CUVSuggestion* suggest = d->suggestionVector[index];
	d->suggestionVector.removeOne(suggest);
	suggest->deleteLater();
}

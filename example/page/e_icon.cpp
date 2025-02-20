#include "e_icon.hpp"

#include <qapplication.h>
#include <QClipboard>
#include <QVBoxLayout>

#include "allineedit.hpp"
#include "allistview.hpp"
#include "almessagebar.hpp"
#include "altext.hpp"

#include "e_icondelegate.hpp"
#include "e_iconmodel.hpp"

using namespace AL;

E_Icon::E_Icon(const QMetaEnum& metaEnum, QWidget* parent): E_BasePage(parent), m_metaEnum(metaEnum) {
	createCustomWidget("some thing...");
	/// listView
	m_iconListView = new CALListView(this);
	m_iconListView->setIsTransparent(true);
	m_iconListView->setFlow(QListView::LeftToRight);
	m_iconListView->setViewMode(QListView::IconMode);
	m_iconListView->setResizeMode(QListView::Adjust);
	connect(m_iconListView, &CALListView::clicked, this, [this](const QModelIndex& index) {
		const QString iconName = m_iconModel->getIconNameFromModelIndex(index);
		if (iconName.isEmpty()) {
			return;
		}
		QApplication::clipboard()->setText(iconName);
		CALMessageBar::success(tr("copy finish"), iconName + tr("Copied to Clipboard"), 2000, ALMessageBarType::Top, this);
	});
	m_iconModel = new E_IconModel(metaEnum, this);
	m_iconDelegate = new E_IconDelegate(this);
	m_iconListView->setModel(m_iconModel);
	m_iconListView->setItemDelegate(m_iconDelegate);
	m_iconListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_searchEdit = new CALLineEdit(this);
	m_searchEdit->setPlaceholderText(tr("search icon"));
	m_searchEdit->setFixedSize(300, 35);
	connect(m_searchEdit, &CALLineEdit::textEdited, this, &E_Icon::slotSearchEditTextChanged);
	connect(m_searchEdit, &CALLineEdit::sigFocusIn, this, &E_Icon::slotSearchEditTextChanged);

	m_iconCountText = new CALText(tr("icon count: ") + QString::number(metaEnum.keyCount()), this);
	m_iconCountText->setTextPixelSize(17);
}

E_Icon::~E_Icon() = default;

void E_Icon::init() {
	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	const auto titleHLayout = new QHBoxLayout;
	titleHLayout->setContentsMargins(0, 0, 20, 0);
	titleHLayout->addWidget(m_searchEdit);
	titleHLayout->addStretch();
	titleHLayout->addWidget(m_iconCountText);
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->addSpacing(13);
	mainVLayout->addLayout(titleHLayout);
	mainVLayout->addWidget(m_iconListView);
	addCentralWidget(centralWidget, true, true, 0);
}

void E_Icon::slotSearchEditTextChanged(const QString& searchText) const {
	if (searchText.isEmpty()) {
		m_iconCountText->setText(tr("icon count: ") + QString::number(m_metaEnum.keyCount()));
		m_iconModel->setIsSearchMode(false);
		m_iconModel->setSearchKeyList({});
		m_iconListView->clearSelection();
		m_iconListView->viewport()->update();
		return;
	}

	QStringList searchKeyList{};
	for (int i = 1; i < m_metaEnum.keyCount(); ++i) {
		if (auto key = QString(m_metaEnum.key(i)); key.contains(searchText, Qt::CaseInsensitive)) {
			searchKeyList.append(key);
		}
	}
	m_iconCountText->setText(tr("icon count: ") + QString::number(searchKeyList.count()));
	m_iconModel->setSearchKeyList(searchKeyList);
	m_iconModel->setIsSearchMode(true);
	m_iconListView->clearSelection();
	m_iconListView->scrollTo(m_iconModel->index(0, 0));
	m_iconListView->viewport()->update();
}

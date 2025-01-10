#include "e_icon.hpp"

#include <qapplication.h>
#include <QClipboard>
#include <QVBoxLayout>

#include "allineedit.hpp"
#include "allistview.hpp"
#include "almessagebar.hpp"

#include "e_icondelegate.hpp"
#include "e_iconmodel.hpp"

E_Icon::E_Icon(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALIcon");
	createCustomWidget("some thing...");

	m_metaEnum = QMetaEnum::fromType<ALIcon::AweSomeIcon>();
	/// listView
	m_iconListView = new CALListView(this);
	m_iconListView->setIsTransparent(true);
	m_iconListView->setFlow(QListView::LeftToRight);
	m_iconListView->setViewMode(QListView::IconMode);
	m_iconListView->setResizeMode(QListView::Adjust);
	connect(m_iconListView, &CALListView::clicked, this, [=](const QModelIndex& index) {
		const QString iconName = m_iconModel->getIconNameFromModelIndex(index);
		if (iconName.isEmpty()) {
			return;
		}
		QApplication::clipboard()->setText(iconName);
		CALMessageBar::success(tr("copy finish"), iconName + tr("Copied to Clipboard"), 2000, ALMessageBarType::Top, this);
	});
	m_iconModel = new E_IconModel(this);
	m_iconDelegate = new E_IconDelegate(this);
	m_iconListView->setModel(m_iconModel);
	m_iconListView->setItemDelegate(m_iconDelegate);
	m_iconListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_searchEdit = new CALLineEdit(this);
	m_searchEdit->setPlaceholderText(tr("search icon"));
	m_searchEdit->setFixedSize(300, 35);
	connect(m_searchEdit, &CALLineEdit::textEdited, this, &E_Icon::slotSearchEditTextChanged);
	connect(m_searchEdit, &CALLineEdit::sigFocusIn, this, &E_Icon::slotSearchEditTextChanged);

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	const auto centralWidgetVLayout = new QVBoxLayout(centralWidget);
	centralWidgetVLayout->setContentsMargins(0, 0, 0, 0);
	centralWidgetVLayout->addSpacing(13);
	centralWidgetVLayout->addWidget(m_searchEdit);
	centralWidgetVLayout->addWidget(m_iconListView);
	addCentralWidget(centralWidget, true, true, 0);
}

E_Icon::~E_Icon() = default;

void E_Icon::slotSearchEditTextChanged(const QString& searchText) const {
	if (searchText.isEmpty()) {
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
	m_iconModel->setSearchKeyList(searchKeyList);
	m_iconModel->setIsSearchMode(true);
	m_iconListView->clearSelection();
	m_iconListView->scrollTo(m_iconModel->index(0, 0));
	m_iconListView->viewport()->update();
}

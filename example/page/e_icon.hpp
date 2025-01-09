#pragma once

#include <QMetaEnum>

#include "e_basepage.hpp"

class CALLineEdit;
class CALListView;

class E_IconModel;
class E_IconDelegate;

class E_Icon final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_Icon(QWidget* parent = nullptr);
	~E_Icon() override;

	Q_SLOT void slotSearchEditTextChanged(const QString& searchText) const;

private:
	QMetaEnum m_metaEnum{};
	CALLineEdit* m_searchEdit{ nullptr };
	CALListView* m_iconListView{ nullptr };
	E_IconModel* m_iconModel{ nullptr };
	E_IconDelegate* m_iconDelegate{ nullptr };
};

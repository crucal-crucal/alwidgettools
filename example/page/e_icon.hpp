#pragma once

#include <QMetaEnum>

#include "e_basepage.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALLineEdit;
class CALListView;
}

class E_IconModel;
class E_IconDelegate;

class E_Icon final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_Icon(const QMetaEnum& metaEnum, QWidget* parent = nullptr);
	~E_Icon() override;

	void init();

	Q_SLOT void slotSearchEditTextChanged(const QString& searchText) const;

private:
	QMetaEnum m_metaEnum{};
	AL::CALLineEdit* m_searchEdit{ nullptr };
	AL::CALListView* m_iconListView{ nullptr };
	E_IconModel* m_iconModel{ nullptr };
	E_IconDelegate* m_iconDelegate{ nullptr };
};

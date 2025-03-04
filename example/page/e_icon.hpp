#pragma once

#include <QMetaEnum>
#include <QModelIndex>

#include "e_basepage.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALLineEdit;
class CALListView;
class CALText;
class CALToolTip;
class CALIconButton;
class CALToolButton;
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
	AL::CALText* m_iconCountText{ nullptr };
	AL::CALToolTip* m_iconToolTip{ nullptr };
	AL::CALToolButton* m_icontoolButton{ nullptr };
	E_IconModel* m_iconModel{ nullptr };
	E_IconDelegate* m_iconDelegate{ nullptr };

	Q_SLOT void slotIconViewClicked(const QModelIndex& index);
	Q_SLOT void slotIconMouseOverItem(const QModelIndex& index) const;
};

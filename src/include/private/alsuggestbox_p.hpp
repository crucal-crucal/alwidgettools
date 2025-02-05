#pragma once

#include <QAction>

#include "alwidgettoolsdef.hpp"

class QVBoxLayout;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALSuggestion final : public QObject {
	Q_OBJECT
	Q_PROPERTY(ALIcon::AweSomeIcon awesomeIcon READ getAwesomeIcon WRITE setAwesomeIcon NOTIFY sigAwesomeIconChanged)
	Q_PROPERTY(ALIcon::FluentIcon fluentIcon READ getFluentIcon WRITE setFluentIcon NOTIFY sigFluentIconChanged)
	Q_PROPERTY(QString suggestText READ getSuggestText WRITE setSuggestText NOTIFY sigSuggestTextChanged)
	Q_PROPERTY(QVariantMap suggestData READ getSuggestData WRITE setSuggestData NOTIFY sigSuggestDataChanged)

public:
	explicit CALSuggestion(QObject* parent = nullptr);
	~CALSuggestion() override;

	void setAwesomeIcon(const ALIcon::AweSomeIcon& icon);
	[[nodiscard]] ALIcon::AweSomeIcon getAwesomeIcon() const;

	void setFluentIcon(const ALIcon::FluentIcon& icon);
	[[nodiscard]] ALIcon::FluentIcon getFluentIcon() const;

	void setSuggestText(const QString& text);
	[[nodiscard]] QString getSuggestText() const;

	void setSuggestData(const QVariantMap& data);
	[[nodiscard]] QVariantMap getSuggestData() const;

	[[nodiscard]] ALIcon::IconType getIconType() const;

Q_SIGNALS:
	void sigAwesomeIconChanged();
	void sigFluentIconChanged();
	void sigSuggestTextChanged();
	void sigSuggestDataChanged();

private:
	ALIcon::IconType m_iconType{};
	QString m_suggestText{};
	QVariantMap m_suggestData{};
};

class CALLineEdit;
class CALNavigationNode;
class CALSuggestModel;
class CALBaseListView;
class CALSuggestDelegate;
class CALSuggestBox;
class CALSuggestBoxSearchViewContainer;

class CALSuggestBoxPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALSuggestBoxPrivate)
	Q_DECLARE_PUBLIC(CALSuggestBox)

public:
	explicit CALSuggestBoxPrivate(CALSuggestBox* q, QObject* parent = nullptr);
	~CALSuggestBoxPrivate() override;

	Q_SLOT void slotThemeModeChanged(const ALThemeType::ThemeMode& mode);
	Q_SLOT void slotSearchEditTextEdit(const QString& searchText);
	Q_SLOT void slotSearchViewClicked(const QModelIndex& index);

protected:
	CALSuggestBox* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	int borderRadius{};
	Qt::CaseSensitivity caseSensitivity{};
	QSize lastSize{};
	bool isExpandAnimationFinished{};
	bool isCloseAnimationFinished{};

	QAction* lightSearchAction{ nullptr };
	QAction* darkSearchAction{ nullptr };
	CALSuggestBoxSearchViewContainer* searchViewContainer{ nullptr };
	CALLineEdit* searchLineEdit{ nullptr };
	CALSuggestModel* searchModel{ nullptr };
	CALBaseListView* searchView{ nullptr };
	CALSuggestDelegate* searchDelegate{ nullptr };
	QVBoxLayout* shadowVLayout{ nullptr };

	QVector<CALSuggestion*> suggestionVector{};

	void startSizeAnimation(const QSize& oldSize, const QSize& newSize);
	void startExpandAnimation();
	void startCloseAnimation();
};
}

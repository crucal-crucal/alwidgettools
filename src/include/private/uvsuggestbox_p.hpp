#pragma once

#include <QAction>

#include "uvwidgettoolsdef.hpp"

class CUVSuggestion final : public QObject {
	Q_OBJECT
	Q_PROPERTY(UVIcon::CUVAweSomeIcon awesomeIcon READ getAwesomeIcon WRITE setAwesomeIcon NOTIFY sigAwesomeIconChanged)
	Q_PROPERTY(QString suggestText READ getSuggestText WRITE setSuggestText NOTIFY sigSuggestTextChanged)
	Q_PROPERTY(QVariantMap suggestData READ getSuggestData WRITE setSuggestData NOTIFY sigSuggestDataChanged)

public:
	explicit CUVSuggestion(QObject* parent = nullptr);
	~CUVSuggestion() override;

	void setAwesomeIcon(const UVIcon::CUVAweSomeIcon& icon);
	[[nodiscard]] UVIcon::CUVAweSomeIcon getAwesomeIcon() const;

	void setSuggestText(const QString& text);
	[[nodiscard]] QString getSuggestText() const;

	void setSuggestData(const QVariantMap& data);
	[[nodiscard]] QVariantMap getSuggestData() const;

Q_SIGNALS:
	Q_SIGNAL void sigAwesomeIconChanged();
	Q_SIGNAL void sigSuggestTextChanged();
	Q_SIGNAL void sigSuggestDataChanged();

private:
	UVIcon::CUVAweSomeIcon awesomeIcon{};
	QString suggestText{};
	QVariantMap suggestData{};
};

class QVBoxLayout;

class CUVLineEdit;
class CUVNavigationNode;
class CUVSuggestModel;
class CUVBaseListView;
class CUVSuggestDelegate;
class CUVSuggestBox;
class CUVSuggestBoxSearchViewContainer;

class CUVSuggestBoxPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVSuggestBoxPrivate)
	Q_DECLARE_PUBLIC(CUVSuggestBox)

public:
	explicit CUVSuggestBoxPrivate(CUVSuggestBox* q, QObject* parent = nullptr);
	~CUVSuggestBoxPrivate() override;

	Q_SLOT void slotThemeModeChanged(const UVThemeType::ThemeMode& mode);
	Q_SLOT void slotSearchEditTextEdit(const QString& searchText);
	Q_SLOT void slotSearchViewClicked(const QModelIndex& index);

protected:
	CUVSuggestBox* const q_ptr{ nullptr };

private:
	UVThemeType::ThemeMode themeMode{};
	int borderRadius{};
	Qt::CaseSensitivity caseSensitivity{};
	QSize lastSize{};
	bool isExpandAnimationFinished{};
	bool isCloseAnimationFinished{};

	QAction* lightSearchAction{ nullptr };
	QAction* darkSearchAction{ nullptr };
	CUVSuggestBoxSearchViewContainer* searchViewContainer{ nullptr };
	CUVLineEdit* searchLineEdit{ nullptr };
	CUVSuggestModel* searchModel{ nullptr };
	CUVBaseListView* searchView{ nullptr };
	CUVSuggestDelegate* searchDelegate{ nullptr };
	QVBoxLayout* shadowVLayout{ nullptr };

	QVector<CUVSuggestion*> suggestionVector{};

	void startSizeAnimation(const QSize& oldSize, const QSize& newSize);
	void startExpandAnimation();
	void startCloseAnimation();
};

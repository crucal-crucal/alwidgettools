#pragma once

#include <QIntValidator>

class CALIntValidator final : public QIntValidator {
	Q_OBJECT

public:
	explicit CALIntValidator(QObject* parent = nullptr);
	explicit CALIntValidator(int bottom, int top, QObject* parent = nullptr);
	~CALIntValidator() override;

	QValidator::State validate(QString&, int&) const override;
	void fixup(QString&) const override;

	void setIsHexMode(bool isHexMode);
	[[nodiscard]] bool getIsHexMode() const;

private:
	bool m_isHexMode{};

	[[nodiscard]] static QString completeInput(const QString& input, int length);
};

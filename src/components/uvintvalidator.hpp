#pragma once

#include <QIntValidator>

class CUVIntValidator final : public QIntValidator {
	Q_OBJECT

public:
	explicit CUVIntValidator(QObject* parent = nullptr);
	explicit CUVIntValidator(int bottom, int top, QObject* parent = nullptr);
	~CUVIntValidator() override;

	QValidator::State validate(QString&, int&) const override;
	void fixup(QString&) const override;

	void setIsHexMode(bool isHexMode);
	[[nodiscard]] bool getIsHexMode() const;

private:
	bool m_isHexMode{};

	[[nodiscard]] static QString completeInput(const QString& input, int length);
};

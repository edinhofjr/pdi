#ifndef TRANSFORMDIALOG_H
#define TRANSFORMDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <vector>

class TransformDialog : public QDialog {
    Q_OBJECT

public:
    explicit TransformDialog(
        const std::vector<std::pair<QString, QString>>& fields,
        QWidget* parent = nullptr);

    std::vector<QString> getValues() const;

private:
    std::vector<QLineEdit*> inputs;
};

#endif // TRANSFORMDIALOG_H

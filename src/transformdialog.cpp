#include "transformdialog.h"

TransformDialog::TransformDialog(
        const std::vector<std::pair<QString, QString>>& fields,
        QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Transformação");

    QFormLayout* layout = new QFormLayout(this);

    for (const auto& f : fields) {
        auto* edit = new QLineEdit(this);
        edit->setText(f.second);      // valor inicial
        layout->addRow(f.first, edit);
        inputs.push_back(edit);
    }

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        this);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addRow(buttons);
}

std::vector<QString> TransformDialog::getValues() const {
    std::vector<QString> values;
    for (auto* edit : inputs) {
        values.push_back(edit->text());
    }
    return values;
}

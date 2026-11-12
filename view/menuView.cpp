#include "menuView.h"
#include "ui_menuView.h"

#include "menuItem.h"
#include "viewFormatting.h"

#include <QFont>
#include <QHeaderView>
#include <QStringList>
#include <QTreeWidgetItem>

namespace {
const int MenuItemIdRole = Qt::UserRole;
}

MenuView::MenuView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuView)
{
    ui->setupUi(this);
    ui->menuTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->menuTree->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->statusLabel->clear();
}

MenuView::~MenuView()
{
    delete ui;
}

void MenuView::clearMenu()
{
    ui->menuTree->clear();
    ui->quantitySpinBox->setValue(1);
    ui->statusLabel->clear();
}

void MenuView::addCategory(const QString& categoryName,
                           const std::vector<std::shared_ptr<MenuItem>>& items)
{
    QTreeWidgetItem *categoryItem = new QTreeWidgetItem(ui->menuTree);
    categoryItem->setText(0, categoryName);
    categoryItem->setFlags(Qt::ItemIsEnabled); // headings cannot be added to the cart

    QFont headingFont = categoryItem->font(0);
    headingFont.setBold(true);
    categoryItem->setFont(0, headingFont);

    for (const std::shared_ptr<MenuItem>& menuItem : items) {
        if (menuItem == nullptr) {
            continue;
        }

        QStringList ingredientNames;
        for (const IngredientRequirement& requirement : menuItem->getIngredientRequirements()) {
            if (requirement.getIngredient() != nullptr) {
                ingredientNames << requirement.getIngredient()->getName();
            }
        }

        QTreeWidgetItem *row = new QTreeWidgetItem(categoryItem);
        row->setText(0, menuItem->getName());
        row->setText(1, ViewFormatting::money(menuItem->getPriceCents()));
        row->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
        row->setData(0, MenuItemIdRole, menuItem->getId());
        row->setToolTip(0, tr("Made with: %1").arg(ingredientNames.join(QStringLiteral(", "))));
    }

    categoryItem->setExpanded(true);
}

void MenuView::on_addToCartButton_clicked()
{
    requestAdd(ui->menuTree->currentItem(), ui->quantitySpinBox->value());
}

void MenuView::on_menuTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    requestAdd(item, 1);
}

void MenuView::requestAdd(QTreeWidgetItem *item, int quantity)
{
    // category headings are top-level rows (no parent) and are not menu items
    if (item == nullptr || item->parent() == nullptr) {
        ui->statusLabel->setText(tr("Select a menu item first."));
        return;
    }

    emit addToCartRequested(item->data(0, MenuItemIdRole).toInt(), quantity);

    ui->statusLabel->setText(tr("Added %1 x %2 to your cart.").arg(quantity).arg(item->text(0)));
    ui->quantitySpinBox->setValue(1);
}

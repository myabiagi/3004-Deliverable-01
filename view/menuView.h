#ifndef MENUVIEW_H
#define MENUVIEW_H

#include <QWidget>
#include <QString>

#include <memory>
#include <vector>

class MenuItem;
class QTreeWidgetItem;

QT_BEGIN_NAMESPACE
namespace Ui { class MenuView; }
QT_END_NAMESPACE

// Café menu grouped by category. Emits addToCartRequested; CustomerWorkspace
// forwards it to the cart. Only menu item ids are kept in the tree.
class MenuView : public QWidget
{
    Q_OBJECT

public:
    explicit MenuView(QWidget *parent = nullptr);
    ~MenuView();

    void clearMenu();
    void addCategory(const QString& categoryName,
                     const std::vector<std::shared_ptr<MenuItem>>& items);

signals:
    void addToCartRequested(int menuItemId, int quantity);

private slots:
    void on_addToCartButton_clicked();
    void on_menuTree_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    void requestAdd(QTreeWidgetItem *item, int quantity);

    Ui::MenuView *ui;
};

#endif // MENUVIEW_H

#ifndef VIEWFORMATTING_H
#define VIEWFORMATTING_H

#include "order.h"
#include "user.h"

#include <QString>
#include <QStringList>
#include <QtGlobal>

// display-only helpers shared by the view classes (no business rules live here)
namespace ViewFormatting {

// 1234 -> "$12.34"
inline QString money(qint64 cents) {
    const QString sign = cents < 0 ? QStringLiteral("-") : QString();
    const qint64 absoluteCents = cents < 0 ? -cents : cents;
    return QStringLiteral("%1$%2.%3")
        .arg(sign)
        .arg(absoluteCents / 100)
        .arg(absoluteCents % 100, 2, 10, QLatin1Char('0'));
}

// human-readable user type for the GUI
inline QString roleName(UserRole role) {
    switch (role) {
    case UserRole::Customer:
        return QStringLiteral("Customer");
    case UserRole::KitchenStaff:
        return QStringLiteral("Kitchen Staff");
    case UserRole::Cashier:
        return QStringLiteral("Cashier");
    case UserRole::Manager:
        return QStringLiteral("Manager");
    case UserRole::Administrator:
        return QStringLiteral("System Administrator");
    }
    return QString();
}

// recipe quantities such as 2, 0.5 or 250
inline QString quantity(double value) {
    return QString::number(value, 'g', 6);
}

// "2 x Latte, 1 x BLT Sandwich"
inline QString orderItemsSummary(const Order& order) {
    QStringList parts;
    for (const OrderItem& item : order.getItems()) {
        const QString name = item.getMenuItem() ? item.getMenuItem()->getName()
                                                : QStringLiteral("(unknown item)");
        parts << QStringLiteral("%1 x %2").arg(item.getQuantity()).arg(name);
    }
    return parts.join(QStringLiteral(", "));
}

} // namespace ViewFormatting

#endif

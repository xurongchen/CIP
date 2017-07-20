#ifndef INSURANCE_H
#define INSURANCE_H

#include <QStandardItemModel>

void insurancelist_renew(QStandardItemModel *&tb);
void insurancelist_search(QStandardItemModel *&tb,QString str);
void insurancelist_delete(QStandardItemModel *&tb);

#endif // INSURANCE_H

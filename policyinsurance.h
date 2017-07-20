#ifndef POLICYINSURANCE_H
#define POLICYINSURANCE_H

#include <QStandardItemModel>
#include <set>

void insuranceget_init();
void insuranceget_insert(int _id);
bool insuranceget_count(int _id);
void insuranceget_remove(int _id);

void insuranceget_renew(QStandardItemModel *&tb);
void insuranceget_delete(QStandardItemModel *&tb);

void insuranceall_renew(QStandardItemModel *&tb);
void insuranceall_delete(QStandardItemModel *&tb);



#endif // POLICYINSURANCE_H

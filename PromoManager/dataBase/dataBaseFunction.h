#ifndef DATABASEFUNCTION_H
#define DATABASEFUNCTION_H

#include <QString>

#include "promoManagerStructs.h"

class QSqlDatabase;

const QString SQLITE_DRIVER_NAME = "QSQLITE";
const QString DEFAULT_DATABASE_NAME = "Resources/DB/dataBase.db";
const QString DEFAULT_DB_TABLE1_NAME = "saleHistory";
const QString DEFAULT_DB_TABLE2_NAME = "promoHistory";

void removeConnection ( QSqlDatabase &&dataBaseValue ) noexcept;
bool createConnection ( QSqlDatabase &&dataBaseValue ) noexcept;
void clearTable ( const QString &tableNameValue ) noexcept;

bool saveSaleHistory ( const QVector <saleStruct> &saleHistoryValue ) noexcept;
bool savePromoHistory ( const QVector <promoStruct> &promoHistoryValue ) noexcept;

QVector <int> getProductIdList () noexcept;
QVector <int> getShopIdList () noexcept;
QVector <int> getPromoIdList () noexcept;
QVector <double> getDataFromDataBase ( const QString &dataName ) noexcept;
QVector <promoParametersStruct> getPromoList () noexcept;

#endif // DATABASEFUNCTION_H

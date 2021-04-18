#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QStringList>
#include <QApplication>
#include <QVector>
#include <QDebug>

#include "dataBaseFunction.h"

void removeConnection( QSqlDatabase &&dataBaseValue ) noexcept{

    if ( !dataBaseValue.connectionNames().isEmpty() ){

        for ( int connectionNum = 0; connectionNum < dataBaseValue.connectionNames().count(); ++connectionNum ){

            dataBaseValue.removeDatabase( dataBaseValue.connectionNames().at(connectionNum) );
        }
    }

    return;
}

bool createConnection( QSqlDatabase &&dataBaseValue ) noexcept{

    dataBaseValue = QSqlDatabase::addDatabase( SQLITE_DRIVER_NAME );

    if ( !dataBaseValue.isValid() ) return false;

    dataBaseValue.setDatabaseName( QString( "%1/%2" ).arg( qApp->applicationDirPath() ).arg( DEFAULT_DATABASE_NAME ) );

    if ( !dataBaseValue.open() ) return false;

    return true;
}

void clearTable( const QString &tableNameValue ) noexcept{

    QSqlDatabase db;
    removeConnection( std::move(db) );
    if ( !createConnection( std::move(db) ) ) return;

    QSqlQuery query;

    query.exec( QString( "DELETE FROM %1" ).arg( tableNameValue ) );

    db.close();

    return;
}

bool saveSaleHistory( const QVector <saleStruct> &saleHistoryValue ) noexcept{

    if ( saleHistoryValue.isEmpty() ) return false;

    QSqlDatabase db;
    removeConnection( std::move(db) );
    if ( !createConnection( std::move(db) ) ) return false;

    db.transaction();
    QSqlQuery query;
    QString insertString = QString( "INSERT INTO saleHistory VALUES (?, ?, ?, ?, ?);" );

    for ( int saleNum = 0; saleNum < saleHistoryValue.size(); ++saleNum ){

        query.prepare( insertString );
        query.addBindValue( QString::number( saleHistoryValue[saleNum].productId ) );
        query.addBindValue( QString::number( saleHistoryValue[saleNum].productPrice ) );
        query.addBindValue( QString::number( saleHistoryValue[saleNum].productPiece ) );
        query.addBindValue( QString::number( saleHistoryValue[saleNum].shopId ) );
        query.addBindValue( QString( "%1" ).arg( saleHistoryValue[saleNum].date, 0, 'f', 10 ) );

        if ( !query.exec() ){

            qDebug () << "ERROR SAVE SALE HISTORY";
            db.commit();
            db.close();

            return false;
        }
    }

    db.commit();
    db.close();

    return true;
}

QVector <int> getProductIdList () noexcept{

    QSqlDatabase db;
    QVector <int> productIdList;
    removeConnection( std::move(db) );
    if ( !createConnection( std::move(db) ) ) return productIdList;

    QSqlQuery query;
    query.prepare( "SELECT DISTINCT productId FROM saleHistory" );

    if ( !query.exec() ){

        qDebug () << "ERROR GET PRODUCT ID";
        db.close();

        return productIdList;
    }

    query.last();
    int querySize = query.at() + 1;
    query.first();

    productIdList.resize( querySize );

    for ( int idNum = 0; idNum < querySize; ++idNum ){

        productIdList[idNum] = query.value(0).toInt();
        query.next();
    }

    return productIdList;
}

QVector<int> getShopIdList () noexcept{

    QSqlDatabase db;
    QVector <int> shopIdList;
    removeConnection( std::move(db) );
    if ( !createConnection( std::move(db) ) ) return shopIdList;

    QSqlQuery query;
    query.prepare( "SELECT DISTINCT shopId FROM saleHistory" );

    if ( !query.exec() ){

        qDebug () << "ERROR GET SHOP ID";
        db.close();

        return shopIdList;
    }

    query.last();
    int querySize = query.at() + 1;
    query.first();

    shopIdList.resize( querySize );

    for ( int idNum = 0; idNum < querySize; ++idNum ){

        shopIdList[idNum] = query.value(0).toInt();
        query.next();
    }

    return shopIdList;
}

bool savePromoHistory( const QVector<promoStruct> &promoHistoryValue ) noexcept{

    if ( promoHistoryValue.isEmpty() ) return false;

    QSqlDatabase db;
    removeConnection( std::move(db) );
    if ( !createConnection( std::move(db) ) ) return false;

    db.transaction();
    QSqlQuery query;
    QString insertString = QString( "INSERT INTO promoHistory VALUES (?, ?, ?, ?, ?);" );

    for ( int saleNum = 0; saleNum < promoHistoryValue.size(); ++saleNum ){

        query.prepare( insertString );
        query.addBindValue( QString::number( promoHistoryValue[saleNum].promoId ) );
        query.addBindValue( QString::number( promoHistoryValue[saleNum].productId ) );
        query.addBindValue( QString( "%1" ).arg( promoHistoryValue[saleNum].startDate, 0, 'f', 10 ) );
        query.addBindValue( QString( "%1" ).arg( promoHistoryValue[saleNum].finishDate, 0, 'f', 10 ) );
        query.addBindValue( QString( "%1" ).arg( promoHistoryValue[saleNum].percent, 0, 'f', 10 ) );

        if ( !query.exec() ){

            qDebug () << "ERROR SAVE PROMO HISTORY";
            db.commit();
            db.close();

            return false;
        }
    }

    db.commit();
    db.close();

    return true;
}

QVector<int> getPromoIdList () noexcept{

    QSqlDatabase db;
    QVector <int> promoIdList;
    removeConnection( std::move(db) );
    if ( !createConnection( std::move(db) ) ) return promoIdList;

    QSqlQuery query;
    query.prepare( "SELECT DISTINCT promoId FROM promoHistory" );

    if ( !query.exec() ){

        qDebug () << "ERROR GET PROMO ID";
        db.close();

        return promoIdList;
    }

    query.last();
    int querySize = query.at() + 1;
    query.first();

    promoIdList.resize( querySize );

    for ( int idNum = 0; idNum < querySize; ++idNum ){

        promoIdList[idNum] = query.value(0).toInt();
        query.next();
    }

    return promoIdList;
}

QVector<double> getDataFromDataBase ( const QString &dataName ) noexcept{

    QSqlDatabase db;
    QVector <double> data;
    removeConnection( std::move(db) );
    if ( !createConnection( std::move(db) ) ) return data;

    QSqlQuery query;
    QString insertString;

    if ( dataName == "Продано товара, шт." ){

        insertString = QString( "SELECT productPiece FROM saleHistory" );
    }
    else if ( dataName == "Продано товара, руб." ){

        insertString = QString( "SELECT productPrice FROM saleHistory" );
    }
    else if ( dataName == "Дата" ){

        insertString = QString( "SELECT date FROM saleHistory" );
    }
    else qDebug () << "ERROR GET DATA FROM DATABASE";

    query.prepare( insertString );

    if ( !query.exec() ){

        qDebug () << "ERROR GET DATA FROM DATABASE";
        db.close();

        return data;
    }

    query.last();
    int querySize = query.at() + 1;
    query.first();

    data.resize( querySize );

    for ( int idNum = 0; idNum < querySize; ++idNum ){

        data[idNum] = query.value(0).toDouble();
        query.next();
    }

    return data;
}

QVector<promoParametersStruct> getPromoList () noexcept{

    QVector <promoParametersStruct> promoList;

    QVector <int> promoIdList = getPromoIdList ();
    if ( promoIdList.isEmpty() ) return promoList;

    QSqlDatabase db;
    removeConnection( std::move(db) );
    if ( !createConnection( std::move(db) ) ) return promoList;

    QSqlQuery query;
    QString insertString;
    QVector <double> tmp;

    for ( int promoIdNum = 0; promoIdNum < promoIdList.size(); ++promoIdNum ){

        insertString = QString( "SELECT DISTINCT productId FROM promoHistory WHERE promoId = %1;" )
                       .arg( promoIdList[promoIdNum] );
        query.prepare( insertString );

        if ( !query.exec() ){

            qDebug () << "ERROR GET PROMO LIST FROM DATABASE";
            db.close();

            return promoList;
        }

        query.last();
        int querySize = query.at() + 1;
        query.first();

        for ( int idNum = 0; idNum < querySize; ++idNum ){

            promoList.append( promoParametersStruct( promoIdList[promoIdNum], query.value(0).toInt(), tmp, tmp, tmp ) );
            query.next();
        }
    }

    QSqlQuery querySales;
    for ( int promoNum = 0; promoNum < promoList.size(); ++promoNum ){

        insertString = QString( "SELECT startDate, finishDate, percent FROM promoHistory WHERE promoId = %1 AND productId = %2;" )
                       .arg( promoList[promoNum].promoId ).arg( promoList[promoNum].productId );
        query.prepare( insertString );

        if ( !query.exec() ){

            qDebug () << "ERROR GET PROMO LIST FROM DATABASE";
            db.close();

            return promoList;
        }

        query.last();
        int querySize = query.at() + 1;
        query.first();

        double startTmp,
               finishTmp;

        for ( int idNum = 0; idNum < querySize; ++idNum ){

            startTmp = query.value(0).toDouble();
            finishTmp = query.value(1).toDouble();
            promoList[promoNum].deltaTime.append( finishTmp - startTmp );
            promoList[promoNum].percent.append( query.value(2).toDouble() );

            insertString = QString( "SELECT productPiece FROM saleHistory WHERE productId = %1 AND date BETWEEN %2 AND %3;" )
                    .arg( promoList[promoNum].productId ).arg( startTmp ).arg( finishTmp );
            querySales.prepare( insertString );

            if ( !querySales.exec() ){

                qDebug () << "ERROR GET PROMO LIST FROM DATABASE";
                db.close();

                return promoList;
            }

            querySales.last();
            int querySalesSize = querySales.at() + 1;
            querySales.first();

            double productPiece = 0.0F;
            for ( int saleNum = 0; saleNum < querySalesSize; ++saleNum ){

                productPiece += querySales.value(0).toDouble();
                querySales.next();
            }

            promoList[promoNum].productPiece.append( productPiece );

            query.next();
        }
    }

    db.close();

    return promoList;
}

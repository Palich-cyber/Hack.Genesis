#ifndef PROMOMANAGERSTRUCTS_H
#define PROMOMANAGERSTRUCTS_H

#include <QString>

struct saleStruct{

    saleStruct(){}

    saleStruct( const double &productPriceValue, const int &productPieceValue, const int &productIdValue,
                const int &shopIdValue, const double &dateValue) :
    productPrice ( std::move( productPriceValue ) ), productPiece ( std::move( productPieceValue ) ),
    productId ( std::move( productIdValue ) ), shopId ( std::move( shopIdValue ) ), date ( std::move( dateValue ) ){}

    saleStruct( double &&productPriceValue, int &&productPieceValue, int &&productIdValue,
                int &&shopIdValue, double &&dateValue) :
    productPrice ( std::move( productPriceValue ) ), productPiece ( std::move( productPieceValue ) ),
    productId ( std::move( productIdValue ) ), shopId ( std::move( shopIdValue ) ), date ( std::move( dateValue ) ){}

    double  productPrice;
    int     productPiece,
            productId,
            shopId;
    double  date;
};

struct promoStruct{

    promoStruct(){}

    promoStruct( const double &startDateValue, const double &finishDateValue, const int &productIdValue,
                 const int &promoIdValue, const double &percentValue ) :
    startDate ( std::move( startDateValue ) ), finishDate ( std::move( finishDateValue ) ),
    productId ( std::move( productIdValue ) ), promoId ( std::move( promoIdValue ) ), percent ( std::move( percentValue ) ){}

    promoStruct( double &&startDateValue, double &&finishDateValue, int &&productIdValue,
                int &&promoIdValue, double &&percentValue) :
    startDate ( std::move( startDateValue ) ), finishDate ( std::move( finishDateValue ) ),
    productId ( std::move( productIdValue ) ), promoId ( std::move( promoIdValue ) ), percent ( std::move( percentValue ) ){}

    double  startDate,
            finishDate;
    int     productId,
            promoId;
    double  percent;
};

struct promoParametersStruct{

    promoParametersStruct(){}

    promoParametersStruct( const int &promoIdValue, const int &productIdValue, const QVector <double> &deltaTimeValue,
                           const QVector <double> &percentValue, const QVector <double> &productPieceValue ) :
    promoId ( std::move( promoIdValue ) ), productId ( std::move( productIdValue ) ),
    deltaTime ( std::move( deltaTimeValue ) ), percent ( std::move( percentValue ) ), productPiece ( std::move( productPieceValue ) ){}

    promoParametersStruct( int &&promoIdValue, int &&productIdValue, QVector <double> &&deltaTimeValue,
                           QVector <double> &&percentValue, QVector <double> &&productPieceValue) :
    promoId ( std::move( promoIdValue ) ), productId ( std::move( productIdValue ) ),
    deltaTime ( std::move( deltaTimeValue ) ), percent ( std::move( percentValue ) ), productPiece ( std::move( productPieceValue ) ){}

    int              promoId,
                     productId;
    QVector <double> deltaTime,
                     percent,
                     productPiece;
};

#endif // PROMOMANAGERSTRUCTS_H

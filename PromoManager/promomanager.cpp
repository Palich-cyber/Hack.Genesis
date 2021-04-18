#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

#include "promomanager.h"
#include "ui_promomanager.h"
#include "promoManagerConstants.h"
#include "dataBase/dataBaseFunction.h"
#include "dateConvertor/convert.h"

PromoManager::PromoManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PromoManager)
{
    ui->setupUi(this);

    // create promo list table
    createPromoListTable();

    // create promo parameters table
    createPromoParametersTable();

    // create promo parameters table
    ui->tW_promoDeltaTime->clear();
    ui->tW_promoDeltaTime->setColumnCount( PROMO_DELT_TABLE_HEADER.size() );
    ui->tW_promoDeltaTime->setHorizontalHeaderLabels( PROMO_DELT_TABLE_HEADER );
    for ( int colNum = 0; colNum < ui->tW_promoDeltaTime->columnCount(); ++colNum )
        ui->tW_promoDeltaTime->setColumnWidth( colNum, 130 );

    axisList << "Продано товара, шт."
             << "Продано товара, руб."
             << "Дата";

    currentAxisYList = axisList;
    for ( int rowNum = 0; rowNum < currentAxisYList.size(); ++rowNum ){

        ui->cB_axisY->addItem( currentAxisYList[rowNum] );
    }
    ui->cB_axisY->setCurrentIndex(1);

    currentAxisXList = axisList;
    ui->cB_axisX->clear();
    for ( int rowNum = 0; rowNum < currentAxisXList.size(); ++rowNum ){

        ui->cB_axisX->addItem( currentAxisXList[rowNum] );
    }
    ui->cB_axisX->setCurrentIndex(0);

}

PromoManager::~PromoManager()
{
    delete ui;
}

void PromoManager::on_pushButton_clicked() noexcept{

    QFile file( qApp->applicationDirPath() + "/Resources/sales_history.csv" );
    if ( !file.open( QIODevice::ReadOnly ) ){
        qDebug () << "Ошибка чтения файла: " << qApp->applicationDirPath() + "\Resources\sales_history.csv";
        return;
    }

    QTextStream stream( &file );
    QString str;
    QStringList list;
    int strCount = 0;

    saleHistory.reserve( 1000000 );

    while ( !stream.atEnd() ){

        ++strCount;
        str = stream.readLine();
        list = str.split(",");
        saleHistory.append( saleStruct( list[1].toDouble(), list[2].toInt(), list[3].toInt(), list[4].toInt(), dateHistoryToJD ( list[5] ) ) );
        if ( strCount >= 1000000 ){

            strCount = 0;
            if ( !saveSaleHistory( saleHistory ) ) qDebug () << "NOT SAVE TO DATABASE";

            saleHistory.clear();
            saleHistory.reserve( 1000000 );
            qDebug () << "SAVE TO DATABASE";
        }
    }

    saveSaleHistory( saleHistory );
    saleHistory.clear();

    return;
}

void PromoManager::on_pushButton_2_clicked () noexcept{

    QVector <int> productIdList = getProductIdList();
    int listSize = productIdList.size();

    ui->comboBox->clear();

    if ( listSize <= 0 ){

        ui->comboBox->addItem( "пусто" );
        ui->label_2->setText("0");
    }
    else{

        for ( int idNum = 0; idNum < productIdList.size(); ++idNum ){

            ui->comboBox->addItem( QString::number( productIdList[idNum] ) );
        }

        ui->comboBox->addItem( "все" );
        ui->comboBox->setCurrentIndex( 0 );
        ui->label_2->setText( QString::number( listSize ) );
    }

    listSize = 0;
    QVector <int> shopIdList = getShopIdList();
    listSize = shopIdList.size();

    ui->comboBox_2->clear();

    if ( listSize <= 0 ){

        ui->comboBox_2->addItem( "пусто" );
        ui->label_3->setText("0");
    }
    else{

        for ( int idNum = 0; idNum < shopIdList.size(); ++idNum ){

            ui->comboBox_2->addItem( QString::number( shopIdList[idNum] ) );
        }

        ui->comboBox_2->addItem( "все" );
        ui->comboBox_2->setCurrentIndex( 0 );
        ui->label_3->setText( QString::number( listSize ) );
    }

    listSize = 0;
    QVector <int> promoIdList = getPromoIdList();
    listSize = promoIdList.size();

    ui->cB_promo->clear();

    if ( listSize <= 0 ){

        ui->cB_promo->addItem( "пусто" );
        ui->l_promoCount->setText("0");
    }
    else{

        for ( int idNum = 0; idNum < promoIdList.size(); ++idNum ){

            ui->cB_promo->addItem( QString::number( promoIdList[idNum] ) );
        }

        ui->cB_promo->addItem( "все" );
        ui->cB_promo->setCurrentIndex( 0 );
        ui->l_promoCount->setText( QString::number( listSize ) );
    }

    promoList = getPromoList();

    // update promo list table
    updatePromoListTable();

    // update promo parameters table
    updatePromoParametersTable( 0 );

    return;
}

void PromoManager::on_cB_axisY_currentIndexChanged ( const QString &arg1 ){

    currentAxisXList.clear();
    currentAxisXList = axisList;

    for ( int rowNum = 0; rowNum < currentAxisXList.size(); ++rowNum ){

        if ( arg1 == currentAxisXList[rowNum] ){

            currentAxisXList.removeAt(rowNum);
        }
        else continue;
    }

    ui->cB_axisX->clear();
    for ( int rowNum = 0; rowNum < currentAxisXList.size(); ++rowNum ){

        ui->cB_axisX->addItem( currentAxisXList[rowNum] );
    }

    return;
}

void PromoManager::on_pushButton_3_clicked () noexcept{

    QFile file( qApp->applicationDirPath() + "/Resources/promo_history.txt" );
    if ( !file.open( QIODevice::ReadOnly ) ){
        qDebug () << "Ошибка чтения файла: " << qApp->applicationDirPath() + "\Resources\promo_history.txt";
        return;
    }

    QTextStream stream( &file );
    QString str;
    QStringList list;

    while ( !stream.atEnd() ){

        str = stream.readLine();
        list = str.split("\t");
        promoHistory.append( promoStruct( dateHistoryToJD ( list[0] ), dateHistoryToJD ( list[1] ),
                                          list[2].toInt(), list[3].toInt(), list[4].toDouble() ) );
    }

    if ( !savePromoHistory( promoHistory ) ) qDebug () << "NOT SAVE TO DATABASE";

    promoHistory.clear();
    qDebug () << "SAVE TO DATABASE";

    return;
}

void PromoManager::on_pB_saleHistoryClear_clicked () noexcept{

    clearTable( DEFAULT_DB_TABLE1_NAME );

    return;
}

void PromoManager::on_pB_promoHistoryClear_clicked () noexcept{

    clearTable( DEFAULT_DB_TABLE2_NAME );

    return;
}

void PromoManager::drawDataFromDataBase( const QString &dataNameValue ) noexcept{

    QVector <double> x, y;

    x.append( getDataFromDataBase( ui->cB_axisX->currentText() ) );
    y.append( getDataFromDataBase( ui->cB_axisY->currentText() ) );

    QVector <double> underX, underY;

    for ( int i = 0; i < 1000000; ++i){

        underX.append( x[i] );
        underY.append( y[i] );
    }
    int Idx = ui->gridWidget->graphCount();

    double maxX = max( underX );
    double maxY = max( underY );

    ui->gridWidget->xAxis->setRange( 2458231.5 - 0.5F, maxX + 0.5F );
    ui->gridWidget->yAxis->setRange( 0.0F, maxY * 1.1F );

    ui->gridWidget->addGraph();
    ui->gridWidget->graph( Idx )->setPen( QColor( 255, 0, 0 ) );
    ui->gridWidget->graph( Idx )->setLineStyle( QCPGraph::lsNone );
    ui->gridWidget->graph( Idx )->setScatterStyle( QCPScatterStyle( QCPScatterStyle::ssStar, QColor( 255, 0, 0 ), 14 ) );

    ui->gridWidget->graph( Idx )->setData( underX, underY );

    ui->gridWidget->replot();

    return;
}

double PromoManager::max( const QVector<double> &vectorValue ) noexcept{

    double max = 0.0F;

    for ( int num = 0; num < vectorValue.size(); ++num ){

        if ( vectorValue[num] > max ) max = vectorValue[num];
    }

    return max;
}

void PromoManager::createPromoListTable () noexcept{

    // create promo list table
    ui->tW_promoList->clear();
    ui->tW_promoList->setColumnCount( PROMO_LIST_TABLE_HEADER.size() );
    ui->tW_promoList->setHorizontalHeaderLabels( PROMO_LIST_TABLE_HEADER );
    for ( int colNum = 0; colNum < ui->tW_promoList->columnCount(); ++colNum )
        ui->tW_promoList->setColumnWidth( colNum, 90 );
}

void PromoManager::updatePromoListTable () noexcept{

    createPromoListTable();

    ui->tW_promoList->setRowCount( promoList.size() );
    for ( int rowNum = 0; rowNum < promoList.size(); ++rowNum ){

        ui->tW_promoList->setItem( rowNum, 0, new QTableWidgetItem( QString::number( promoList[rowNum].promoId ) ) );
        ui->tW_promoList->setItem( rowNum, 1, new QTableWidgetItem( QString::number( promoList[rowNum].productId ) ) );
    }

    return;
}

void PromoManager::createPromoParametersTable () noexcept{

    // create promo parameters table
    ui->tW_promoParameters->clear();
    ui->tW_promoParameters->setColumnCount( PROMO_PARS_TABLE_HEADER.size() );
    ui->tW_promoParameters->setHorizontalHeaderLabels( PROMO_PARS_TABLE_HEADER );
    for ( int colNum = 0; colNum < ui->tW_promoParameters->columnCount(); ++colNum )
        ui->tW_promoParameters->setColumnWidth( colNum, 130 );

    return;
}

void PromoManager::updatePromoParametersTable ( const int &promoNum ) noexcept{

    createPromoParametersTable();

    ui->tW_promoParameters->setRowCount( promoList[promoNum].deltaTime.size() );
    for ( int rowNum = 0; rowNum < promoList[promoNum].deltaTime.size(); ++rowNum ){

        ui->tW_promoParameters->setItem( rowNum, 0, new QTableWidgetItem( QString::number( promoList[promoNum].deltaTime[rowNum] ) ) );
        ui->tW_promoParameters->setItem( rowNum, 1, new QTableWidgetItem( QString::number( promoList[promoNum].percent[rowNum] ) ) );
        ui->tW_promoParameters->setItem( rowNum, 2, new QTableWidgetItem( QString::number( promoList[promoNum].productPiece[rowNum] ) ) );
    }

    return;
}

void PromoManager::on_pB_draw_clicked () noexcept{

    drawDataFromDataBase( "" );

    return;
}

void PromoManager::on_tW_promoList_cellClicked ( int row, int column ) noexcept{

    Q_UNUSED ( column );

    updatePromoParametersTable( row );

    return;
}

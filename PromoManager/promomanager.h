#ifndef PROMOMANAGER_H
#define PROMOMANAGER_H

#include <QMainWindow>

#include "promoManagerStructs.h"

namespace Ui {
class PromoManager;
}

class PromoManager : public QMainWindow
{
    Q_OBJECT

public:

    explicit PromoManager(QWidget *parent = 0);

    ~PromoManager();

private slots:

    void on_pushButton_clicked () noexcept;

    void on_pushButton_2_clicked () noexcept;

    void on_cB_axisY_currentIndexChanged ( const QString &arg1 );

    void on_pushButton_3_clicked () noexcept;

    void on_pB_saleHistoryClear_clicked () noexcept;

    void on_pB_promoHistoryClear_clicked () noexcept;

    void on_pB_draw_clicked () noexcept;

    void on_tW_promoList_cellClicked (int row, int column) noexcept;

private:

    void drawDataFromDataBase ( const QString &dataNameValue ) noexcept;

    double max ( const QVector <double> &vectorValue ) noexcept;

    void createPromoListTable () noexcept;

    void updatePromoListTable () noexcept;

    void createPromoParametersTable () noexcept;

    void updatePromoParametersTable ( const int &promoNum ) noexcept;

private:

    Ui::PromoManager *ui;
    QVector < saleStruct > saleHistory;
    QVector < promoStruct > promoHistory;
    QVector < promoParametersStruct > promoList;
    QStringList axisList,
                currentAxisXList,
                currentAxisYList;

};

#endif // PROMOMANAGER_H

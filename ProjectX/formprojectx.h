#ifndef FORMPROJECTX_H
#define FORMPROJECTX_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QObject>
#include <QGraphicsScene>
#include <QTreeWidget>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QPen>
#include <QBrush>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <string>
#include <QVector>
#include <QTimer>
#include <sceneprojectx.h>
#include "QtScript/QScriptEngine"
using namespace std;


namespace Ui {
class FormProjectX;
}

struct MoveMultItem{
    QString vx;
    QString vy;
    double x0;
    double y0;
    double xm;
    double ym;
    int item_k;
};

struct MoveMultItemCoord{
    QString xt;
    QString yt;
    QScriptEngine xtExpression;
    QScriptEngine ytExpression;
    double x0;
    double y0;
    double xm;
    double ym;
    int item_k;
};

class FormProjectX : public QMainWindow
{
    Q_OBJECT

public:
    explicit FormProjectX(QWidget *parent = 0);
    ~FormProjectX();

private slots:
    //Материальная точка
    void on_inP_clicked();

    //Колесо
    void on_inW_clicked();

    void MovePB_clicked();
    void CoordMeth_clicked();

public slots:
    //Создание материальной точки
    void addPoint_okPoint_clicked();

    //Создание колеса
    void addWheel_okWheel_clicked();

    //Слот срабатывающий при нажатии на объект в дереве
    void trWi_clicked();

    //Действие
    void delE_clicked();
    void change_trWi_clicked();
    void AddNewDetail_clicked();
    void AddNewDetailCoord_clicked();
    void Play_clicked();
    void Play_clicked_coord();
    void Stop_clicked();
    void Stop_clickedCoord();
    void update_xy_formove();
    void update_xy_forcoord();
    void contact_trWi_clicked();
    void timeSetting_clicked();
    void ChangeTime_clicked();
    void omega_clicked();
    void ChangeOmega_clicked();
    void Pause_clicked();
    void change_MoveMultItem();
    void change_MoveMultItemCoord();

private:
    Ui::FormProjectX *ui;
    SceneProjectX *scene;

    QVBoxLayout *lay;
    QVBoxLayout *laynowcoor;
    QVBoxLayout *laycontact;
    QVBoxLayout *layeffect;

    //Дерево проекта
    QTreeWidget *treeProjectD;
    QVBoxLayout *treelay;
    QTreeWidgetItem *trWi;

    //Векторы объектов
    QVector<QGraphicsItem*> *vItem;
    QVector<QTreeWidgetItem*> *vTree;

    //Действия над деталаями
    QPushButton *delE;
    QPushButton *chanE;

    //on_inP
    QLineEdit *x, *y, *name;
    QLabel *xl1, *yl1;
    QPushButton *okPoint;
    QHBoxLayout *v1, *v2;

    //on_inR
    QLineEdit *xR1, *yR1, *xR2, *yR2, *nameR;
    QLabel *xl2, *yl2;
    QPushButton *okRod;
    QHBoxLayout *v3, *v4;

    //on_inW
    QLineEdit *xW, *yW, *rW, *nameW;
    QLabel *rl;
    QPushButton *okWheel;

    //addPoint
    QGraphicsEllipseItem *Point;

    //addRod
    QGraphicsItem *Rod;

    //addWheel
    QGraphicsItemGroup *Wheel;

    //Текущие координаты
    QHBoxLayout *vnc1, *vnc2, *vnc3;

    //laynowccor
    QLineEdit *lnc_t;
    QLabel *lncLabel_t;
    double tShow;

    //Движение точки
    QVector<MoveMultItem*> *vMovePB;
    QVector<MoveMultItemCoord*> *vMoveCoord;
    QHBoxLayout *layCoor1, *layCoor2;
    QLineEdit *v_x, *v_y, *x_t, *y_t;
    QLineEdit *vA_x, *vA_y, *aA_x, *aA_y, *xA_0, *yA_0, *vV_x, *vV_y, *xV_0, *yV_0, *time_m, *time_h, *omegaEdit;
    QLineEdit *xtC, *ytC, *xC_0, *yC_0;
    QLabel *vxl1, *vyl1, *axl1, *ayl1, *timel, *timehl, *xl0, *yl0, *xtlabel, *ytlabel, *omegaLabel;
    QPushButton *MovePB, *Accel, *CoordMeth;
    QPushButton *AddNewDetail, *AddNewDetailAccel, *AddNewDetailCoord;
    QPushButton *ChangeTime, *ChangeOmega;
    QHBoxLayout *vm, *layDelE;
    QTimer *timerv, *timerc;
    QHBoxLayout *v5, *v6;
    MoveMultItem *Detail;
    MoveMultItemCoord *DetailCoord;
    //
    QScriptEngine xtExpression, ytExpression, vxtExpression, vytExpression;
    int testCount;
    //
    int CountItemMovePB;
    int CountItemAccel;
    int CountItemCoord;
    int statusAnimation;
    double xm;
    double ym;
    double vx;
    double vy;
    double ax;
    double ay;
    double h, helph, helphAccel, helphCoord;
    double vxh, vyh, th, thCoor, dx, dy, vxp, vyp;
    double tlimit, tAnim;
    double dtlimit, helpdtlimit, helpdtlimitAccel, helpdtlimitCoord;
    double item_k;
    bool boolv, boola;
    double omega, dfiCoor, dfiMove, angleCoor, anglePB;
};


#endif // FORMPROJECTX_H

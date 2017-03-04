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
using namespace std;


namespace Ui {
class FormProjectX;
}

struct MoveMultItem{
    double vx;
    double vy;
    double x0m;
    double y0m;
    double xm;
    double ym;
    int item_k;
};

struct MoveMultItemAccel{
    double vx;
    double vy;
    double ax;
    double ay;
    double dx;
    double dy;
    double x0m;
    double y0m;
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

    //Стержень
    void on_inR_clicked();

    //Колесо
    void on_inW_clicked();

    void MovePB_clicked();
    void Accel_clicked();

public slots:
    //Создание материальной точки
    void addPoint_okPoint_clicked();

    //Создание стержня
    void addRod_okRod_clicked();

    //Создание колеса
    void addWheel_okWheel_clicked();

    //Слот срабатывающий при нажатии на объект в дереве
    void trWi_clicked();

    //Действие
    void delE_clicked();
    void change_trWi_clicked();
    void change_scene_clicked();
    void AddNewDetail_clicked();
    void AddNewDetailAccel_clicked();
    void Play_clicked();
    void Play_clicked_accel();
    void Stop_clicked();
    void Stop_clickedAccel();
    void update_xy_formove();
    void update_xy_foraccel();
    void contact_trWi_clicked();
    void timeSetting_clicked();
    void ChangeTime_clicked();
    void Pause_clicked();
    void change_MoveMultItem();
    void change_MoveMultItemAccel();

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
    QHBoxLayout *vnc1, *vnc2;

    //Движение точки
    QVector<MoveMultItem*> *vMovePB;
    QVector<MoveMultItemAccel*> *vMoveAccel;
    QLineEdit *v_x, *v_y, *a_x, *a_y;
    QLineEdit *vA_x, *vA_y, *aA_x, *aA_y, *xA_0, *yA_0, *vV_x, *vV_y, *xV_0, *yV_0, *time_m, *time_h;
    QLabel *vxl1, *vyl1, *axl1, *ayl1, *timel, *timehl, *xl0, *yl0;
    QPushButton *MovePB, *Accel, *CoordMeth;
    QPushButton *AddNewDetail, *AddNewDetailAccel;
    QPushButton *ChangeTime;
    QHBoxLayout *vm, *layDelE;
    QTimer *timerv, *timera;
    QHBoxLayout *v5, *v6;
    MoveMultItem *Detail;
    MoveMultItemAccel *DetailAccel;
    int CountItemMovePB;
    int CountItemAccel;
    int statusAnimation;
    double xm;
    double ym;
    double vx;
    double vy;
    double ax;
    double ay;
    double h, helph, helphAccel;
    double vxh, vyh, th, dx, dy, vxp, vyp;
    double tlimit, tAnim;
    double dtlimit, helpdtlimit, helpdtlimitAccel;
    double item_k;
    bool boolv, boola;
};


#endif // FORMPROJECTX_H

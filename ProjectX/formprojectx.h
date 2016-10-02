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
    void restructure_graphicitem_fromtree();
    void change_scene_clicked();
    void AddNewDetail_clicked();
    void Anim_clicked();
    void Anim_clicked_accel();
    void stopAnim_clicked();
    void stopAnim_clickedaccel();
    void update_xy_formove();
    void update_xy_foraccel();
    void contact_trWi_clicked();
    void timeSetting_clicked();
    void ChangeTime_clicked();

private:
    Ui::FormProjectX *ui;
    SceneProjectX *scene;

    QVBoxLayout *lay;
    QVBoxLayout *laycontact;
    QHBoxLayout *layeffect;

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

    //Движение точки
    QVector<MoveMultItem*> *vMovePB;
    QLineEdit *v_x, *v_y, *a_x, *a_y, *time_m, *name_m;
    QLabel *vxl1, *vyl1, *axl1, *ayl1, *timel;
    QPushButton *MovePB;
    QPushButton *AddNewDetail;
    QPushButton *Accel;
    QPushButton *ChangeTime;
    QHBoxLayout *vm;
    QTimer *timerv, *timera;
    QHBoxLayout *v5;
    MoveMultItem *Detail;
    int CountItemMovePB;
    double xm;
    double ym;
    double vx;
    double vy;
    double ax;
    double ay;
    double h, helph;
    double vxh, vyh, th, dx, dy, vxp, vyp;
    double tlimit;
    double dtlimit, helpdtlimit;
    double item_k;
    bool boolv, boola;
};


#endif // FORMPROJECTX_H

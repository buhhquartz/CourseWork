#include "formprojectx.h"
#include "ui_formprojectx.h"

FormProjectX::FormProjectX(QWidget *parent) : QMainWindow(parent), ui(new Ui::FormProjectX)
{
    ui->setupUi(this);
    ui->centralwidget->scroll(20, 20);
    lay = new QVBoxLayout;
    laynowcoor = new QVBoxLayout;
    laycontact = new QVBoxLayout;
    layeffect = new QVBoxLayout;
    v1 = new QHBoxLayout;
    v2 = new QHBoxLayout;
    layCoor1 = new QHBoxLayout;
    layCoor2 = new QHBoxLayout;
    CountItemMovePB = 0;
    CountItemCoord = 0;
    statusAnimation = 0;
    th = 0;
    angleCoor = 0;

    //
    testCount = 0;
    //

    //Создаем таймер для задания равномерного движения мат. точки
    timerv = new QTimer(this);
    connect(timerv, SIGNAL(timeout()), this, SLOT(update_xy_formove()));

    //Создаем таймер для задания координатного движения мат. точки
    timerc = new QTimer(this);
    connect(timerc,SIGNAL(timeout()),this,SLOT(update_xy_forcoord()));

    ui->inP->setToolTip("Материальная точка");
    ui->inW->setToolTip("Колесо");
    ui->Play->setToolTip("Анимировать");
    ui->Pause->setToolTip("Пауза");
    ui->Stop->setToolTip("Стоп");
    ui->CoordMeth->setToolTip("Координатный способ задания движения");
    ui->MovePB->setToolTip("Движение по заданной скорости");

    ui->Play->setEnabled(false);
    ui->Stop->setEnabled(false);
    ui->Pause->setEnabled(false);

    //Сигналы по функциональным кнопкам
    //Меню -> Выход
    connect(ui->ExitMenu, SIGNAL(triggered(bool)), this, SLOT(close()));
    //Настройки -> Время анимации
    dtlimit = 5;
    tlimit = dtlimit;
    tAnim = 5;
    h = 0.005;
    connect(ui->timeSetting, SIGNAL(triggered(bool)), this, SLOT(timeSetting_clicked()));

    //Угловая скорость
    omega = 1;
    connect(ui->omega, SIGNAL(triggered(bool)), this, SLOT(omega_clicked()));


    scene = new SceneProjectX(this);
    ui->View->setScene(scene);

    //Введение системы координат
    //scene->setSceneRect(-ui->View->width()/2, -ui->View->height()/2, ui->View->width(), ui->View->height());
    scene->setSceneRect(-3000.0/2, -3000.0/2, 3000.0, 3000.0);
    scene->coordinate_system();

    //Дерево проекта
    treeProjectD = new QTreeWidget;
    treeProjectD->setHeaderLabel("Список деталей");
    treelay = new QVBoxLayout;
    treelay->addWidget(treeProjectD);
    ui->groupBox_3->setLayout(treelay);

    //Векторы объектов
    vItem = new QVector<QGraphicsItem*>;//Вектор деталей
    vTree = new QVector<QTreeWidgetItem*>;//Вектор элементов дерева
    vMovePB = new QVector<MoveMultItem*>;
    vMoveCoord = new QVector<MoveMultItemCoord*>;

    //Слот срабатывающий при нажатии на элемент в дереве
    connect(treeProjectD, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(trWi_clicked()));
    //Слот показывающий координаты при нажатии
    connect(treeProjectD, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(change_trWi_clicked()));
    //Слот создающий кнопку движения v-const
    connect(treeProjectD, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(contact_trWi_clicked()));
    //Слот срабатывающий при нажатии кнопки "Стоп"
    connect(ui->Stop, SIGNAL(clicked(bool)), this, SLOT(Stop_clicked()));
    connect(ui->Stop, SIGNAL(clicked(bool)), this, SLOT(Stop_clickedCoord()));
    connect(ui->Pause, SIGNAL(clicked(bool)), this, SLOT(Pause_clicked()));

    connect(ui->MovePB, SIGNAL(clicked(bool)), this, SLOT(MovePB_clicked()));
    connect(ui->CoordMeth, SIGNAL(clicked(bool)), this, SLOT(CoordMeth_clicked()));
}

FormProjectX::~FormProjectX()
{
    delete ui;
}

void FormProjectX::on_inP_clicked()
{
    if (lay->isEmpty()){
        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        xl1 = new QLabel("x=");
        x = new QLineEdit("Координата x");
        v1->addWidget(xl1);
        v1->addWidget(x);
        yl1 = new QLabel("y=");
        y = new QLineEdit("Координата y");
        v2->addWidget(yl1);
        v2->addWidget(y);
        name = new QLineEdit("Название объекта");
        okPoint = new QPushButton("Построить");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addWidget(name);
        lay->addWidget(okPoint);
        ui->groupBox_2->setLayout(lay);
        qDebug() << "1p";
    }
    else{
        delete(lay);
        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        xl1 = new QLabel("x=");
        x = new QLineEdit("Координата x");
        v1->addWidget(xl1);
        v1->addWidget(x);
        yl1 = new QLabel("y=");
        y = new QLineEdit("Координата y");
        v2->addWidget(yl1);
        v2->addWidget(y);
        name = new QLineEdit("Название объекта");
        okPoint = new QPushButton("Построить");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addWidget(name);
        lay->addWidget(okPoint);
        ui->groupBox_2->setLayout(lay);
        qDebug() << "2p";
    }
    connect(okPoint, SIGNAL(clicked(bool)), this, SLOT(addPoint_okPoint_clicked()));
}

void FormProjectX::addPoint_okPoint_clicked()
{
    double x1, y1;
    x1 = x->text().toDouble() * 30;
    y1 = y->text().toDouble() * 30;
    QBrush blackBrush(Qt::black);
    QPen blackPen(Qt::black);

    Point = scene->addEllipse(-5, (-1)*(5), 10, 10, blackPen, blackBrush);
    Point->setPos(x1, (-1)*y1);

    trWi = new QTreeWidgetItem(treeProjectD);
    trWi->setText(0, name->text());

    vItem->push_back(Point);
    vTree->push_back(trWi);

    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;
}

void FormProjectX::on_inW_clicked()
{
    if (lay->isEmpty()){
        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        v3 = new QHBoxLayout;
        xl1 = new QLabel("x=");
        xW = new QLineEdit("Координата x");
        v1->addWidget(xl1);
        v1->addWidget(xW);
        yl1 = new QLabel("y=");
        yW = new QLineEdit("Координата y");
        v2->addWidget(yl1);
        v2->addWidget(yW);
        rl = new QLabel("r=");
        rW = new QLineEdit("Радиус");
        v3->addWidget(rl);
        v3->addWidget(rW);
        nameW = new QLineEdit("Название объекта");
        okWheel = new QPushButton("Построить");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addLayout(v3);
        lay->addWidget(nameW);
        lay->addWidget(okWheel);
        ui->groupBox_2->setLayout(lay);
    }
    else{
        delete(lay);
        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        v3 = new QHBoxLayout;
        xl1 = new QLabel("x=");
        xW = new QLineEdit("Координата x");
        v1->addWidget(xl1);
        v1->addWidget(xW);
        yl1 = new QLabel("y=");
        yW = new QLineEdit("Координата y");
        v2->addWidget(yl1);
        v2->addWidget(yW);
        rl = new QLabel("r=");
        rW = new QLineEdit("Радиус");
        v3->addWidget(rl);
        v3->addWidget(rW);
        nameW = new QLineEdit("Название объекта");
        okWheel = new QPushButton("Построить");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addLayout(v3);
        lay->addWidget(nameW);
        lay->addWidget(okWheel);
        ui->groupBox_2->setLayout(lay);
    }
    connect(okWheel, SIGNAL(clicked(bool)), this, SLOT(addWheel_okWheel_clicked()));
}

void FormProjectX::addWheel_okWheel_clicked()
{
    double x1, y1, r;
    x1 = xW->text().toDouble() * 30;
    y1 = yW->text().toDouble() * 30;
    r = rW->text().toDouble() * 30;
    QBrush blackBrush(Qt::black);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    Wheel = new QGraphicsItemGroup;
    scene->addItem(Wheel);
    QGraphicsItem *a, *b, *c, *d;
    a = scene->addEllipse((0-r), (-1)*(0+r), 2*r, 2*r, blackPen);
    blackPen.setWidth(4);
    b = scene->addEllipse(0-1, (-1)*(0+1), 2, 2, blackPen, blackBrush);
    blackPen.setWidth(2);
    c = scene->addLine(0, (-1)*(0+r), 0, (-1)*(0-r));
    d = scene->addLine(0-r, (-1)*0, 0+r, (-1)*0);
    Wheel->addToGroup(a);
    Wheel->addToGroup(b);
    Wheel->addToGroup(c);
    Wheel->addToGroup(d);
    Wheel->setPos(x1, (-1)*y1);
    Wheel->setFlag(QGraphicsItem::ItemIsMovable);

    trWi = new QTreeWidgetItem(treeProjectD);
    trWi->setText(0, nameW->text());   

    vItem->push_back(Wheel);
    vTree->push_back(trWi);

    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;
}

void FormProjectX::trWi_clicked()
{
    if (layeffect->isEmpty()){
        layDelE = new QHBoxLayout;
        delE = new QPushButton;
        delE->setIcon(QIcon("E:/CourseWork.git/ProjectX/Del.png"));
        delE->setIconSize(QSize(25, 25));
        delE->setToolTip("Удалить");
        delE->setFixedHeight(33);
        delE->setFixedWidth(33);
        layDelE->addWidget(delE);
        layeffect = new QVBoxLayout;
        layeffect->addLayout(layDelE);
        ui->groupBox_7->setLayout(layeffect);
    }
    else{
        delete(layeffect);
        qDeleteAll( ui->groupBox_7->findChildren<QWidget*>() );

        layDelE = new QHBoxLayout;
        delE = new QPushButton;
        delE->setIcon(QIcon("E:/CourseWork.git/ProjectX/Del.png"));
        delE->setIconSize(QSize(25, 25));
        delE->setToolTip("Удалить");
        delE->setFixedHeight(33);
        delE->setFixedWidth(33);
        layDelE->addWidget(delE);
        layeffect = new QVBoxLayout;
        layeffect->addLayout(layDelE);
        ui->groupBox_7->setLayout(layeffect);
    }
    connect(delE, SIGNAL(clicked(bool)), this, SLOT(delE_clicked()));
}

void FormProjectX::contact_trWi_clicked()
{
    ui->CoordMeth->setEnabled(true);
    ui->MovePB->setEnabled(true);
}

void FormProjectX::delE_clicked()
{   
    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            for(int j = 0; j < vMovePB->size(); j++){
                if(i == vMovePB->at(j)->item_k){
                    delete(vMovePB->at(j));
                    vMovePB->removeAt(j);
                    CountItemMovePB--;
                    for(int k = j; k < vMovePB->size(); k++){
                        vMovePB->at(k)->item_k--;
                    }
                }
            }
            for(int j = 0; j < vMoveCoord->size(); j++){
                if(i == vMoveCoord->at(j)->item_k){
                    delete(vMoveCoord->at(j));
                    vMoveCoord->removeAt(j);
                    CountItemCoord--;
                    for(int k = j; k < vMoveCoord->size(); k++){
                        vMoveCoord->at(k)->item_k--;
                    }
                }
            }
        }
    }

    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            delete(vItem->at(i));
            vItem->removeAt(i);
            delete(vTree->at(i));
            vTree->removeAt(i);
        }
    }
}

void FormProjectX::change_trWi_clicked()
{
    if (laynowcoor->isEmpty()){
        vnc1 = new QHBoxLayout;
        vnc2 = new QHBoxLayout;
        vnc3 = new QHBoxLayout;

        xl1 = new QLabel("x = ");
        x = new QLineEdit();
        x->setDisabled(true);
        vnc1->addWidget(xl1);
        vnc1->addWidget(x);

        yl1 = new QLabel("y = ");
        y = new QLineEdit();
        y->setDisabled(true);
        vnc2->addWidget(yl1);
        vnc2->addWidget(y);

        lncLabel_t = new QLabel("t = ");
        lnc_t = new QLineEdit();
        lnc_t->setDisabled(true);
        vnc3->addWidget(lncLabel_t);
        vnc3->addWidget(lnc_t);

        for(int i = 0; i < vTree->size(); i++){
            if(vTree->at(i)->isSelected()){
                QString xpos = QString::number(vItem->at(i)->pos().x() / 30);
                QString ypos = QString::number((-1) * vItem->at(i)->pos().y() / 30);
                QString tnow = QString::number(tShow);
                x->setText(xpos);
                y->setText(ypos);
                lnc_t->setText(tnow);
            }
        }

        laynowcoor = new QVBoxLayout;
        laynowcoor->addLayout(vnc1);
        laynowcoor->addLayout(vnc2);
        laynowcoor->addLayout(vnc3);
        ui->groupBox_8->setLayout(laynowcoor);
    }
    else{
        delete(laynowcoor);
        qDeleteAll( ui->groupBox_8->findChildren<QWidget*>() );

        vnc1 = new QHBoxLayout;
        vnc2 = new QHBoxLayout;
        vnc3 = new QHBoxLayout;

        xl1 = new QLabel("x = ");
        x = new QLineEdit();
        x->setDisabled(true);
        vnc1->addWidget(xl1);
        vnc1->addWidget(x);

        yl1 = new QLabel("y = ");
        y = new QLineEdit();
        y->setDisabled(true);
        vnc2->addWidget(yl1);
        vnc2->addWidget(y);

        lncLabel_t = new QLabel("t = ");
        lnc_t = new QLineEdit();
        lnc_t->setDisabled(true);
        vnc3->addWidget(lncLabel_t);
        vnc3->addWidget(lnc_t);

        for(int i = 0; i < vTree->size(); i++){
            if(vTree->at(i)->isSelected()){
                QString xpos = QString::number(vItem->at(i)->pos().x() / 30);
                QString ypos = QString::number((-1) * vItem->at(i)->pos().y() / 30);
                QString tnow = QString::number(tShow);
                x->setText(xpos);
                y->setText(ypos);
                lnc_t->setText(tnow);
            }
        }

        laynowcoor = new QVBoxLayout;
        laynowcoor->addLayout(vnc1);
        laynowcoor->addLayout(vnc2);
        laynowcoor->addLayout(vnc3);
        ui->groupBox_8->setLayout(laynowcoor);
    }

    xV_0 = new QLineEdit;
    yV_0 = new QLineEdit;
    vV_x = new QLineEdit;
    vV_y = new QLineEdit;

    xA_0 = new QLineEdit;
    yA_0 = new QLineEdit;
    vA_x = new QLineEdit;
    vA_y = new QLineEdit;
    aA_x = new QLineEdit;
    aA_y = new QLineEdit;

    xC_0 = new QLineEdit();
    yC_0 = new QLineEdit();
    xtC = new QLineEdit();
    ytC = new QLineEdit();

    if(statusAnimation == 0){
        xV_0->setEnabled(true);
        yV_0->setEnabled(true);
        vV_x->setEnabled(true);
        vV_y->setEnabled(true);

        xA_0->setEnabled(true);
        yA_0->setEnabled(true);
        vA_x->setEnabled(true);
        vA_y->setEnabled(true);
        aA_x->setEnabled(true);
        aA_y->setEnabled(true);

        xC_0->setEnabled(true);
        yC_0->setEnabled(true);
        xtC->setEnabled(true);
        ytC->setEnabled(true);
    }
    else{
        xV_0->setDisabled(true);
        yV_0->setDisabled(true);
        vV_x->setDisabled(true);
        vV_y->setDisabled(true);

        xA_0->setDisabled(true);
        yA_0->setDisabled(true);
        vA_x->setDisabled(true);
        vA_y->setDisabled(true);
        aA_x->setDisabled(true);
        aA_y->setDisabled(true);

        xC_0->setDisabled(true);
        yC_0->setDisabled(true);
        xtC->setDisabled(true);
        ytC->setDisabled(true);
    }

    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            for (int j = 0; j < vMovePB->size(); j++){
                if (vMovePB->at(j)->item_k == i){
                    if (lay->isEmpty()){
                        v1 = new QHBoxLayout;
                        v2 = new QHBoxLayout;
                        v3 = new QHBoxLayout;
                        v4 = new QHBoxLayout;
                        QString x0 = QString::number(vMovePB->at(j)->x0 / 30);
                        QString y0 = QString::number(vMovePB->at(j)->y0 / 30);
                        QString vx = vMovePB->at(j)->vx;
                        QString vy = vMovePB->at(j)->vy;

                        xl0 = new QLabel("x0=");
                        xV_0->setText(x0);

                        yl0 = new QLabel("y0=");
                        yV_0->setText(y0);

                        vxl1 = new QLabel("v_x=");                    
                        vV_x->setText(vx);

                        vyl1 = new QLabel("v_y=");
                        vV_y->setText(vy);

                        v1->addWidget(xl0);
                        v1->addWidget(xV_0);
                        v2->addWidget(yl0);
                        v2->addWidget(yV_0);
                        v3->addWidget(vxl1);
                        v3->addWidget(vV_x);
                        v4->addWidget(vyl1);
                        v4->addWidget(vV_y);

                        lay = new QVBoxLayout;
                        lay->addLayout(v1);
                        lay->addLayout(v2);
                        lay->addLayout(v3);
                        lay->addLayout(v4);
                        ui->groupBox_2->setLayout(lay);
                    }
                    else{
                        delete(lay);
                        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

                        v1 = new QHBoxLayout;
                        v2 = new QHBoxLayout;
                        v3 = new QHBoxLayout;
                        v4 = new QHBoxLayout;
                        QString x0 = QString::number(vMovePB->at(j)->x0 / 30);
                        QString y0 = QString::number(vMovePB->at(j)->y0 / 30);
                        QString vx = vMovePB->at(j)->vx;
                        QString vy = vMovePB->at(j)->vy;

                        xl0 = new QLabel("x0=");
                        xV_0->setText(x0);

                        yl0 = new QLabel("y0=");
                        yV_0->setText(y0);

                        vxl1 = new QLabel("v_x=");
                        vV_x->setText(vx);

                        vyl1 = new QLabel("v_y=");
                        vV_y->setText(vy);

                        v1->addWidget(xl0);
                        v1->addWidget(xV_0);
                        v2->addWidget(yl0);
                        v2->addWidget(yV_0);
                        v3->addWidget(vxl1);
                        v3->addWidget(vV_x);
                        v4->addWidget(vyl1);
                        v4->addWidget(vV_y);

                        lay = new QVBoxLayout;
                        lay->addLayout(v1);
                        lay->addLayout(v2);
                        lay->addLayout(v3);
                        lay->addLayout(v4);
                        ui->groupBox_2->setLayout(lay);
                    }
                }
            }
        }
    }

    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            for (int j = 0; j < vMoveCoord->size(); j++){
                if (vMoveCoord->at(j)->item_k == i){
                    if (lay->isEmpty()){
                        v1 = new QHBoxLayout;
                        v2 = new QHBoxLayout;
                        v3 = new QHBoxLayout;
                        v4 = new QHBoxLayout;

                        QString x0 = QString::number(vMoveCoord->at(j)->x0 / 30);
                        QString y0 = QString::number(vMoveCoord->at(j)->y0 / 30);
                        QString xt = vMoveCoord->at(j)->xt;
                        QString yt = vMoveCoord->at(j)->yt;

                        xl0 = new QLabel("x0=");
                        xC_0->setText(x0);

                        yl0 = new QLabel("y0=");
                        yC_0->setText(y0);

                        vxl1 = new QLabel("x(t)=");
                        xtC->setText(xt);

                        vyl1 = new QLabel("y(t)=");
                        ytC->setText(yt);

                        v1->addWidget(xl0);
                        v1->addWidget(xC_0);
                        v2->addWidget(yl0);
                        v2->addWidget(yC_0);
                        v3->addWidget(vxl1);
                        v3->addWidget(xtC);
                        v4->addWidget(vyl1);
                        v4->addWidget(ytC);

                        lay = new QVBoxLayout;
                        lay->addLayout(v1);
                        lay->addLayout(v2);
                        lay->addLayout(v3);
                        lay->addLayout(v4);

                        ui->groupBox_2->setLayout(lay);
                    }
                    else{
                        delete(lay);
                        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

                        v1 = new QHBoxLayout;
                        v2 = new QHBoxLayout;
                        v3 = new QHBoxLayout;
                        v4 = new QHBoxLayout;

                        QString x0 = QString::number(vMoveCoord->at(j)->x0 / 30);
                        QString y0 = QString::number(vMoveCoord->at(j)->y0 / 30);
                        QString xt = vMoveCoord->at(j)->xt;
                        QString yt = vMoveCoord->at(j)->yt;

                        xl0 = new QLabel("x0=");
                        xC_0->setText(x0);

                        yl0 = new QLabel("y0=");
                        yC_0->setText(y0);

                        vxl1 = new QLabel("x(t)=");
                        xtC->setText(xt);

                        vyl1 = new QLabel("y(t)=");
                        ytC->setText(yt);

                        v1->addWidget(xl0);
                        v1->addWidget(xC_0);
                        v2->addWidget(yl0);
                        v2->addWidget(yC_0);
                        v3->addWidget(vxl1);
                        v3->addWidget(xtC);
                        v4->addWidget(vyl1);
                        v4->addWidget(ytC);

                        lay = new QVBoxLayout;
                        lay->addLayout(v1);
                        lay->addLayout(v2);
                        lay->addLayout(v3);
                        lay->addLayout(v4);

                        ui->groupBox_2->setLayout(lay);
                    }
                }
            }
        }
    }

    connect(xV_0, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItem()));
    connect(yV_0, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItem()));
    connect(vV_x, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItem()));
    connect(vV_y, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItem()));

    connect(xC_0, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItemCoord()));
    connect(yC_0, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItemCoord()));
    connect(xtC, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItemCoord()));
    connect(ytC, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItemCoord()));
}

void FormProjectX::change_MoveMultItem()
{
    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            for (int j = 0; j < vMovePB->size(); j++){
                if (vMovePB->at(j)->item_k == i){
                    vMovePB->at(j)->x0 = xV_0->text().toDouble() * 30;
                    vMovePB->at(j)->y0 = yV_0->text().toDouble() * 30;
                    vMovePB->at(j)->xm = vMovePB->at(j)->x0;
                    vMovePB->at(j)->ym = vMovePB->at(j)->y0;
                    vMovePB->at(j)->vx = vV_x->text();
                    vMovePB->at(j)->vy = vV_y->text();
                }
            }
        }
    }
}

void FormProjectX::change_MoveMultItemCoord()
{
    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            for (int j = 0; j < vMoveCoord->size(); j++){
                if (vMoveCoord->at(j)->item_k == i){
                    vMoveCoord->at(j)->x0 = xC_0->text().toDouble() * 30;
                    vMoveCoord->at(j)->y0 = yC_0->text().toDouble() * 30;
                    vMoveCoord->at(j)->xt = xtC->text();
                    vMoveCoord->at(j)->yt = ytC->text();
                }
            }
        }
    }
}

void FormProjectX::timeSetting_clicked()
{
    ui->groupBox_2->setTitle("Настройки");
    if(lay->isEmpty()){
        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        v3 = new QHBoxLayout;
        timel = new QLabel("t max = ");
        time_m = new QLineEdit();
        timehl = new QLabel("Время анимации, у. е. = ");
        time_h = new QLineEdit();
        QString helpT = QString::number(dtlimit);
        time_m->setText(helpT);
        QString helpH = QString::number(tAnim);
        time_h->setText(helpH);
        ChangeTime = new QPushButton("Изменить");
        v1->addWidget(timel);
        v1->addWidget(time_m);
        v2->addWidget(timehl);
        v2->addWidget(time_h);
        v3->addWidget(ChangeTime);

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addLayout(v3);
        ui->groupBox_2->setLayout(lay);
    }
    else{
        delete(lay);
        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        v3 = new QHBoxLayout;
        timel = new QLabel("t max = ");
        time_m = new QLineEdit();
        timehl = new QLabel("Время анимации, у. е. = ");
        time_h = new QLineEdit();
        QString helpT = QString::number(dtlimit);
        time_m->setText(helpT);
        QString helpH = QString::number(tAnim);
        time_h->setText(helpH);
        ChangeTime = new QPushButton("Изменить");
        v1->addWidget(timel);
        v1->addWidget(time_m);
        v2->addWidget(timehl);
        v2->addWidget(time_h);
        v3->addWidget(ChangeTime);

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addLayout(v3);
        ui->groupBox_2->setLayout(lay);
    }
    connect(ChangeTime, SIGNAL(clicked(bool)), this, SLOT(ChangeTime_clicked()));
}

void FormProjectX::ChangeTime_clicked()
{
    dtlimit = time_m->text().toDouble();
    tlimit = dtlimit;
    tAnim = time_h->text().toDouble();
    //h = dtlimit / time_h->text().toDouble() * 0.025;

    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;
    ui->groupBox_2->setTitle("Свойства");
}

void FormProjectX::omega_clicked()
{
    ui->groupBox_2->setTitle("Настройки");
    if(lay->isEmpty()){

        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        omegaLabel = new QLabel("Угл. скорость = ");
        omegaEdit = new QLineEdit();
        qDebug() << "Ok";
        QString helpOmega = QString::number(omega);
        qDebug() << "Ok";
        omegaEdit->setText(helpOmega);
        ChangeOmega = new QPushButton("Изменить");
        qDebug() << "Ok";
        v1->addWidget(omegaLabel);
        v1->addWidget(omegaEdit);
        v2->addWidget(ChangeOmega);

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        ui->groupBox_2->setLayout(lay);
        qDebug() << "Ok";
    }
    else{
        delete(lay);
        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

        qDebug() << "Ok1";
        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        omegaLabel = new QLabel("Угл. скорость = ");
        omegaEdit = new QLineEdit();
        QString helpOmega = QString::number(omega);
        omegaEdit->setText(helpOmega);
        ChangeOmega = new QPushButton("Изменить");
        v1->addWidget(omegaLabel);
        v1->addWidget(omegaEdit);
        v2->addWidget(ChangeOmega);

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        ui->groupBox_2->setLayout(lay);
    }
    connect(ChangeOmega, SIGNAL(clicked(bool)), this, SLOT(ChangeOmega_clicked()));
}

void FormProjectX::ChangeOmega_clicked()
{
    omega = omegaEdit->text().toDouble();
    //dfiCoor = omega * h;
    //dfiMove = omega * h;

    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;
    ui->groupBox_2->setTitle("Свойства");
}

void FormProjectX::MovePB_clicked()
{
    if (lay->isEmpty()){
        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        vxl1 = new QLabel("v_x=");
        v_x = new QLineEdit("Функция v_x(t)");
        v1->addWidget(vxl1);
        v1->addWidget(v_x);
        vyl1 = new QLabel("v_y=");
        v_y = new QLineEdit("Функция v_y(t)");
        v2->addWidget(vyl1);
        v2->addWidget(v_y);
        AddNewDetail = new QPushButton("Запомнить");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addWidget(AddNewDetail);
        ui->groupBox_2->setLayout(lay);
    }
    else{
        delete(lay);
        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        vxl1 = new QLabel("v_x=");
        v_x = new QLineEdit("Функция v_x(t)");
        v1->addWidget(vxl1);
        v1->addWidget(v_x);
        vyl1 = new QLabel("v_y=");
        v_y = new QLineEdit("Функция v_y(t)");
        v2->addWidget(vyl1);
        v2->addWidget(v_y);
        AddNewDetail = new QPushButton("Запомнить");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addWidget(AddNewDetail);
        ui->groupBox_2->setLayout(lay);
    }

    connect(AddNewDetail, SIGNAL(clicked(bool)), this, SLOT(AddNewDetail_clicked()));
    connect(ui->Play, SIGNAL(pressed()), this, SLOT(Play_clicked()));

}

void FormProjectX::AddNewDetail_clicked()
{
    ui->Play->setEnabled(true);
    ui->Stop->setEnabled(false);

    Detail = new MoveMultItem;

    helph = h;
    helpdtlimit = dtlimit;

    Detail->vx = v_x->text();
    Detail->vy = v_y->text();

    //Поиск элемента выбранного для анимации
    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            Detail->x0 = vItem->at(i)->pos().x();
            Detail->xm = vItem->at(i)->pos().x();
            Detail->y0 = -vItem->at(i)->pos().y();
            Detail->ym = -vItem->at(i)->pos().y();
            Detail->item_k = i;
        }
    }

    vMovePB->push_back(Detail);
    CountItemMovePB++;

    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;

    ui->MovePB->setDisabled(true);
    ui->CoordMeth->setDisabled(true);
}

void FormProjectX::Play_clicked()
{
    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;

    statusAnimation = 2;
    timerv->start(int(tAnim));
    ui->Play->setEnabled(false);
    ui->Stop->setEnabled(true);
    ui->Pause->setEnabled(true);
}

void FormProjectX::update_xy_formove()
{
    int i = 0;
    tShow = th;
    th += h;
    helpdtlimit -= helph;
    anglePB += omega;

    if (helpdtlimit < -3.39808e-14){
        timerv->stop();
        ui->Play->setEnabled(true);
        ui->Stop->setEnabled(false);
        ui->Pause->setEnabled(false);

        statusAnimation = 0;
        th = 0;
        helph = h;
        helpdtlimit = dtlimit;
        anglePB = 0;
        for (int i = 0; i < vMovePB->size(); i++){
            vMovePB->at(i)->xm = vMovePB->at(i)->x0;
            vMovePB->at(i)->ym = vMovePB->at(i)->y0;
        }
    }
    else{
        while(i != CountItemMovePB){
            vxtExpression.globalObject().setProperty("t", th);
            vytExpression.globalObject().setProperty("t", th);

            vMovePB->at(i)->xm += vxtExpression.evaluate(vMovePB->at(i)->vx).toNumber() * 30 * h;
            vMovePB->at(i)->ym += vytExpression.evaluate(vMovePB->at(i)->vy).toNumber() * 30 * h;

            vItem->at(vMovePB->at(i)->item_k)->setPos(vMovePB->at(i)->xm, -vMovePB->at(i)->ym);
            vItem->at(vMovePB->at(i)->item_k)->setRotation(anglePB);
            vItem->at(vMovePB->at(i)->item_k)->rotation();
            i++;
        }
    }
}

void FormProjectX::Stop_clicked()
{
    //Действие при нажатии на кнопку "Стоп" во время равномерного движения  
    statusAnimation = 0;
    helph = h;
    helpdtlimit = dtlimit;
    th = 0;
    anglePB = 0;
    for (int i = 0; i < vMovePB->size(); i++){
        vMovePB->at(i)->xm = vMovePB->at(i)->x0;
        vMovePB->at(i)->ym = vMovePB->at(i)->y0;
    }

    timerv->stop();
    ui->Play->setEnabled(true);
    ui->Stop->setEnabled(false);
    ui->Pause->setEnabled(false);
}

void FormProjectX::Pause_clicked()
{
    statusAnimation = 1;
    timerv->stop();
    timerc->stop();

    ui->Stop->setEnabled(false);
    ui->Pause->setEnabled(false);
    ui->Play->setEnabled(true);
}

void FormProjectX::CoordMeth_clicked()
{
    if (lay->isEmpty()){
        layCoor1 = new QHBoxLayout;
        layCoor2 = new QHBoxLayout;
        xtlabel = new QLabel("x(t)=");
        x_t = new QLineEdit("Функция x(t)");
        //x_t = new QLineEdit("t");
        layCoor1->addWidget(xtlabel);
        layCoor1->addWidget(x_t);
        ytlabel = new QLabel("y(t)=");
        y_t = new QLineEdit("Функция y(t)");
        //y_t = new QLineEdit("t");
        layCoor2->addWidget(ytlabel);
        layCoor2->addWidget(y_t);
        AddNewDetailCoord = new QPushButton("Запомнить");

        lay = new QVBoxLayout;
        lay->addLayout(layCoor1);
        lay->addLayout(layCoor2);
        lay->addWidget(AddNewDetailCoord);
        ui->groupBox_2->setLayout(lay);
        qDebug() << "1";
    }
    else{
        delete(lay);
        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

        layCoor1 = new QHBoxLayout;
        layCoor2 = new QHBoxLayout;
        xtlabel = new QLabel("x(t)=");
        x_t = new QLineEdit("Функция x(t)");
        layCoor1->addWidget(xtlabel);
        layCoor1->addWidget(x_t);
        ytlabel = new QLabel("y(t)=");
        y_t = new QLineEdit("Функция y(t)");
        layCoor2->addWidget(ytlabel);
        layCoor2->addWidget(y_t);
        AddNewDetailCoord = new QPushButton("Запомнить");

        lay = new QVBoxLayout;
        lay->addLayout(layCoor1);
        lay->addLayout(layCoor2);
        lay->addWidget(AddNewDetailCoord);
        ui->groupBox_2->setLayout(lay);
        qDebug() << "2";
    }

    connect(AddNewDetailCoord, SIGNAL(clicked(bool)), this, SLOT(AddNewDetailCoord_clicked()));
    connect(ui->Play, SIGNAL(pressed()), this, SLOT(Play_clicked_coord()));
}

void FormProjectX::AddNewDetailCoord_clicked()
{
    ui->Play->setEnabled(true);
    ui->Stop->setEnabled(false);
    DetailCoord = new MoveMultItemCoord;

    helpdtlimitCoord = dtlimit;
    helphCoord = h;

    DetailCoord->xt = x_t->text();
    DetailCoord->yt = y_t->text();

    //Поиск элемента выбранного для анимации
    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            DetailCoord->x0 = vItem->at(i)->pos().x();
            DetailCoord->y0 = -vItem->at(i)->pos().y();
            DetailCoord->item_k = i;
        }
    }
    DetailCoord->xm = DetailCoord->x0;
    DetailCoord->ym = DetailCoord->y0;

    vMoveCoord->push_back(DetailCoord);
    CountItemCoord++;

    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;

    ui->MovePB->setDisabled(true);
    ui->CoordMeth->setDisabled(true);
}

void FormProjectX::Play_clicked_coord()
{
    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;

    statusAnimation = 2;
    timerc->start(int(tAnim));
    ui->Play->setEnabled(false);
    ui->Stop->setEnabled(true);
    ui->Pause->setEnabled(true);
}

void FormProjectX::update_xy_forcoord()
{
    //Обновление координат при движении объекта
    int j = 0;
    tShow = th;
    thCoor += h;
    angleCoor += omega;
    helpdtlimitCoord -= helphCoord;

    if (helpdtlimitCoord < -3.39808e-14){
        timerc->stop();
        ui->Play->setEnabled(true);
        ui->Stop->setEnabled(false);
        ui->Pause->setEnabled(false);

        statusAnimation = 0;
        helphCoord = h;
        helpdtlimitCoord = dtlimit;
        thCoor = 0;
        angleCoor = 0;
        for (int i = 0; i < vMoveCoord->size(); i++){
            vMoveCoord->at(i)->xm = vMoveCoord->at(i)->x0;
            vMoveCoord->at(i)->ym = vMoveCoord->at(i)->y0;
        }
    }
    else{
        while(j != CountItemCoord){
            vMoveCoord->at(j)->xtExpression.globalObject().setProperty("t", thCoor);
            vMoveCoord->at(j)->ytExpression.globalObject().setProperty("t", thCoor);

            vMoveCoord->at(j)->xm = vMoveCoord->at(j)->x0 + vMoveCoord->at(j)->xtExpression.evaluate(vMoveCoord->at(j)->xt).toNumber() * 30;
            vMoveCoord->at(j)->ym = vMoveCoord->at(j)->y0 + vMoveCoord->at(j)->ytExpression.evaluate(vMoveCoord->at(j)->yt).toNumber() * 30;
            vItem->at(vMoveCoord->at(j)->item_k)->setPos(vMoveCoord->at(j)->xm, -vMoveCoord->at(j)->ym);
            vItem->at(vMoveCoord->at(j)->item_k)->setRotation(angleCoor);
            vItem->at(vMoveCoord->at(j)->item_k)->rotation();
            j++;
        }
    }
}

void FormProjectX::Stop_clickedCoord()
{
    //Действие при нажатии на кнопку "Стоп" во время координатного движения
    statusAnimation = 0;

    helphCoord = h;
    helpdtlimitCoord = dtlimit;
    thCoor = 0;
    angleCoor = 0;
    for (int i = 0; i < vMoveCoord->size(); i++){
        vMoveCoord->at(i)->xm = vMoveCoord->at(i)->x0;
        vMoveCoord->at(i)->ym = vMoveCoord->at(i)->y0;
    }

    timerc->stop();
    ui->Play->setEnabled(true);
    ui->Stop->setEnabled(false);
    ui->Pause->setEnabled(false);
}

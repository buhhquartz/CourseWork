#include "formprojectx.h"
#include "ui_formprojectx.h"

FormProjectX::FormProjectX(QWidget *parent) : QMainWindow(parent), ui(new Ui::FormProjectX)
{
    ui->setupUi(this);
    lay = new QVBoxLayout;
    laynowcoor = new QVBoxLayout;
    laycontact = new QVBoxLayout;
    layeffect = new QVBoxLayout;
    v1 = new QHBoxLayout;
    v2 = new QHBoxLayout;
    CountItemMovePB = 0;
    CountItemAccel = 0;
    statusAnimation = 0;
    th = 0;

    //Создаем таймер для задания равномерного движения мат. точки
    timerv = new QTimer(this);
    connect(timerv, SIGNAL(timeout()), this, SLOT(update_xy_formove()));

    //Создаем таймер для задания равноускоренного движения мат. точки
    timera = new QTimer(this);
    connect(timera,SIGNAL(timeout()),this,SLOT(update_xy_foraccel()));

    ui->inP->setToolTip("Материальная точка");
    ui->inR->setToolTip("Стержень");
    ui->inW->setToolTip("Колесо");
    ui->Play->setToolTip("Анимировать");
    ui->Pause->setToolTip("Пауза");
    ui->Stop->setToolTip("Стоп");

    ui->Play->setEnabled(false);
    ui->Stop->setEnabled(false);
    ui->Pause->setEnabled(false);

    //Сигналы по функциональным кнопкам
    //Меню -> Выход
    connect(ui->ExitMenu, SIGNAL(triggered(bool)), this, SLOT(close()));
    //Настройки -> Время анимации
    dtlimit = 5;
    tlimit = dtlimit;
    h = 0.05;
    connect(ui->timeSetting, SIGNAL(triggered(bool)), this, SLOT(timeSetting_clicked()));


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
    vMoveAccel = new QVector<MoveMultItemAccel*>;

    //Слот срабатывающий при нажатии на элемент в дереве
    connect(treeProjectD, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(trWi_clicked()));
    //Слот показывающий координаты при нажатии
    connect(treeProjectD, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(change_trWi_clicked()));
    //Слот создающий кнопку движения v-const
    connect(treeProjectD, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(contact_trWi_clicked()));
    //Слот показывающий изменение координат при перемещении объекта сцены
    connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(change_scene_clicked()));
    //Слот срабатывающий при нажатии кнопки "Стоп"
    connect(ui->Stop, SIGNAL(clicked(bool)), this, SLOT(Stop_clicked()));
    connect(ui->Stop, SIGNAL(clicked(bool)), this, SLOT(Stop_clickedAccel()));
    connect(ui->Pause, SIGNAL(clicked(bool)), this, SLOT(Pause_clicked()));
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
    Point->setFlag(QGraphicsItem::ItemIsMovable);

    trWi = new QTreeWidgetItem(treeProjectD);
    trWi->setText(0, name->text());

    vItem->push_back(Point);
    vTree->push_back(trWi);

    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;
}


void FormProjectX::on_inR_clicked()
{
    if (lay->isEmpty()){
        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        v3 = new QHBoxLayout;
        v4 = new QHBoxLayout;
        xl1 = new QLabel("x1=");
        xR1 = new QLineEdit("Координата x1");
        v1->addWidget(xl1);
        v1->addWidget(xR1);
        yl1 = new QLabel("y1=");
        yR1 = new QLineEdit("Координата y1");
        v2->addWidget(yl1);
        v2->addWidget(yR1);
        xl2 = new QLabel("x2=");
        xR2 = new QLineEdit("Координата x2");
        v3->addWidget(xl2);
        v3->addWidget(xR2);
        yl2 = new QLabel("y2=");
        yR2 = new QLineEdit("Координата y2");
        v4->addWidget(yl2);
        v4->addWidget(yR2);
        nameR = new QLineEdit("Название объекта");
        okRod = new QPushButton("Построить");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addLayout(v3);
        lay->addLayout(v4);
        lay->addWidget(nameR);
        lay->addWidget(okRod);
        ui->groupBox_2->setLayout(lay);
    }
    else{
        delete(lay);
        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        v3 = new QHBoxLayout;
        v4 = new QHBoxLayout;
        xl1 = new QLabel("x1=");
        xR1 = new QLineEdit("Координата x1");
        v1->addWidget(xl1);
        v1->addWidget(xR1);
        yl1 = new QLabel("y1=");
        yR1 = new QLineEdit("Координата y1");
        v2->addWidget(yl1);
        v2->addWidget(yR1);
        xl2 = new QLabel("x2=");
        xR2 = new QLineEdit("Координата x2");
        v3->addWidget(xl2);
        v3->addWidget(xR2);
        yl2 = new QLabel("y2=");
        yR2 = new QLineEdit("Координата y2");
        v4->addWidget(yl2);
        v4->addWidget(yR2);
        nameR = new QLineEdit("Название объекта");
        okRod = new QPushButton("Построить");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addLayout(v3);
        lay->addLayout(v4);
        lay->addWidget(nameR);
        lay->addWidget(okRod);
        ui->groupBox_2->setLayout(lay);
    }
    connect(okRod, SIGNAL(clicked(bool)), this, SLOT(addRod_okRod_clicked()));
}

void FormProjectX::addRod_okRod_clicked()
{
    double x1, y1, x2, y2;
    x1 = xR1->text().toDouble() * 30;
    y1 = yR1->text().toDouble() * 30;
    x2 = xR2->text().toDouble() * 30;
    y2 = yR2->text().toDouble() * 30;
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    Rod = scene->addLine(0, (-1)*0, x2-x1, (-1)*(y2-y1), blackPen);
    Rod->setPos(x1, (-1)*y1);
    Rod->setFlag(QGraphicsItem::ItemIsMovable);

    trWi = new QTreeWidgetItem(treeProjectD);
    trWi->setText(0, nameR->text());

    vItem->push_back(Rod);
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
        delE->setIcon(QIcon(":/images/Del.png"));
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
        delE->setIcon(QIcon(":/images/Del.png"));
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
    if(laycontact->isEmpty()){
        vm = new QHBoxLayout;
        MovePB = new QPushButton;
        MovePB->setIcon(QIcon(":/images/Speed.png"));
        MovePB->setIconSize(QSize(25, 25));
        MovePB->setToolTip("Равномерное движение");
        MovePB->setFixedHeight(33);
        MovePB->setFixedWidth(33);
        Accel = new QPushButton;
        Accel->setIcon(QIcon(":/images/Accel.png"));
        Accel->setIconSize(QSize(25, 25));
        Accel->setToolTip("Равноускоренное движение");
        Accel->setFixedHeight(33);
        Accel->setFixedWidth(33);
        CoordMeth = new QPushButton;
        CoordMeth->setIcon(QIcon(":/images/Coord.png"));
        CoordMeth->setIconSize(QSize(25, 25));
        CoordMeth->setToolTip("Координатный способ задания движения");
        CoordMeth->setFixedHeight(33);
        CoordMeth->setFixedWidth(33);
        vm->addWidget(CoordMeth);
        vm->addWidget(MovePB);
        vm->addWidget(Accel);  
        laycontact = new QVBoxLayout;
        laycontact->addLayout(vm);
        ui->groupBox_6->setLayout(laycontact);
    }
    else{
        delete(laycontact);
        qDeleteAll( ui->groupBox_6->findChildren<QWidget*>() );

        vm = new QHBoxLayout;
        MovePB = new QPushButton;
        MovePB->setIcon(QIcon(":/images/Speed.png"));
        MovePB->setIconSize(QSize(25, 25));
        MovePB->setToolTip("Равномерное движение");
        MovePB->setFixedHeight(33);
        MovePB->setFixedWidth(33);
        Accel = new QPushButton;
        Accel->setIcon(QIcon(":/images/Accel.png"));
        Accel->setIconSize(QSize(25, 25));
        Accel->setToolTip("Равноускоренное движение");
        Accel->setFixedHeight(33);
        Accel->setFixedWidth(33);
        CoordMeth = new QPushButton;
        CoordMeth->setIcon(QIcon(":/images/Coord.png"));
        CoordMeth->setIconSize(QSize(25, 25));
        CoordMeth->setToolTip("Координатный способ задания движения");
        CoordMeth->setFixedHeight(33);
        CoordMeth->setFixedWidth(33);
        vm->addWidget(CoordMeth);
        vm->addWidget(MovePB);
        vm->addWidget(Accel);
        laycontact = new QVBoxLayout;
        laycontact->addLayout(vm);
        ui->groupBox_6->setLayout(laycontact);
    }
    connect(MovePB, SIGNAL(clicked(bool)), this, SLOT(MovePB_clicked()));
    connect(Accel, SIGNAL(clicked(bool)), this, SLOT(Accel_clicked()));
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
            for(int j = 0; j < vMoveAccel->size(); j++){
                if(i == vMoveAccel->at(j)->item_k){
                    delete(vMoveAccel->at(j));
                    vMoveAccel->removeAt(j);
                    CountItemAccel--;
                    for(int k = j; k < vMoveAccel->size(); k++){
                        vMoveAccel->at(k)->item_k--;
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
        xl1 = new QLabel("x=");
        x = new QLineEdit();
        x->setDisabled(true);
        vnc1->addWidget(xl1);
        vnc1->addWidget(x);
        yl1 = new QLabel("y=");
        y = new QLineEdit();
        y->setDisabled(true);
        vnc2->addWidget(yl1);
        vnc2->addWidget(y);

        for(int i = 0; i < vTree->size(); i++){
            if(vTree->at(i)->isSelected()){
                QString xpos = QString::number(vItem->at(i)->pos().x() / 30);
                QString ypos = QString::number((-1) * vItem->at(i)->pos().y() / 30);
                x->setText(xpos);
                y->setText(ypos);
            }
        }

        laynowcoor = new QVBoxLayout;
        laynowcoor->addLayout(vnc1);
        laynowcoor->addLayout(vnc2);
        ui->groupBox_8->setLayout(laynowcoor);
    }
    else{
        delete(laynowcoor);
        qDeleteAll( ui->groupBox_8->findChildren<QWidget*>() );

        vnc1 = new QHBoxLayout;
        vnc2 = new QHBoxLayout;
        xl1 = new QLabel("x=");
        x = new QLineEdit();
        x->setDisabled(true);
        vnc1->addWidget(xl1);
        vnc1->addWidget(x);
        yl1 = new QLabel("y=");
        y = new QLineEdit();
        y->setDisabled(true);
        vnc2->addWidget(yl1);
        vnc2->addWidget(y);

        for(int i = 0; i < vTree->size(); i++){
            if(vTree->at(i)->isSelected()){
                QString xpos = QString::number(vItem->at(i)->pos().x() / 30);
                QString ypos = QString::number((-1) * vItem->at(i)->pos().y() / 30);
                x->setText(xpos);
                y->setText(ypos);
            }
        }

        laynowcoor = new QVBoxLayout;
        laynowcoor->addLayout(vnc1);
        laynowcoor->addLayout(vnc2);
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
                        QString x0 = QString::number(vMovePB->at(j)->x0m / 30);
                        QString y0 = QString::number(vMovePB->at(j)->y0m / 30);
                        QString vx = QString::number(vMovePB->at(j)->vx / 30);
                        QString vy = QString::number(vMovePB->at(j)->vy / 30);

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
                        QString x0 = QString::number(vMovePB->at(j)->x0m / 30);
                        QString y0 = QString::number(vMovePB->at(j)->y0m / 30);
                        QString vx = QString::number(vMovePB->at(j)->vx / 30);
                        QString vy = QString::number(vMovePB->at(j)->vy / 30);

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
            for (int j = 0; j < vMoveAccel->size(); j++){
                if (vMoveAccel->at(j)->item_k == i){
                    if (lay->isEmpty()){
                        v1 = new QHBoxLayout;
                        v2 = new QHBoxLayout;
                        v3 = new QHBoxLayout;
                        v4 = new QHBoxLayout;
                        v5 = new QHBoxLayout;
                        v6 = new QHBoxLayout;
                        QString x0 = QString::number(vMoveAccel->at(j)->x0m / 30);
                        QString y0 = QString::number(vMoveAccel->at(j)->y0m / 30);
                        QString vx = QString::number(vMoveAccel->at(j)->vx / 30);
                        QString vy = QString::number(vMoveAccel->at(j)->vy / 30);
                        QString ax = QString::number(vMoveAccel->at(j)->ax / 30);
                        QString ay = QString::number(vMoveAccel->at(j)->ay / 30);

                        xl0 = new QLabel("x0=");
                        xA_0->setText(x0);

                        yl0 = new QLabel("y0=");
                        yA_0->setText(y0);

                        vxl1 = new QLabel("v0_x=");
                        vA_x->setText(vx);

                        vyl1 = new QLabel("v0_y=");
                        vA_y->setText(vy);

                        axl1 = new QLabel("a_x=");
                        aA_x->setText(ax);

                        ayl1 = new QLabel("a_y=");
                        aA_y->setText(ay);

                        v1->addWidget(xl0);
                        v1->addWidget(xA_0);
                        v2->addWidget(yl0);
                        v2->addWidget(yA_0);
                        v3->addWidget(vxl1);
                        v3->addWidget(vA_x);
                        v4->addWidget(vyl1);
                        v4->addWidget(vA_y);
                        v5->addWidget(axl1);
                        v5->addWidget(aA_x);
                        v6->addWidget(ayl1);
                        v6->addWidget(aA_y);

                        lay = new QVBoxLayout;
                        lay->addLayout(v1);
                        lay->addLayout(v2);
                        lay->addLayout(v3);
                        lay->addLayout(v4);
                        lay->addLayout(v5);
                        lay->addLayout(v6);
                        ui->groupBox_2->setLayout(lay);
                    }
                    else{
                        delete(lay);
                        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

                        v1 = new QHBoxLayout;
                        v2 = new QHBoxLayout;
                        v3 = new QHBoxLayout;
                        v4 = new QHBoxLayout;
                        v5 = new QHBoxLayout;
                        v6 = new QHBoxLayout;
                        QString x0 = QString::number(vMoveAccel->at(j)->x0m / 30);
                        QString y0 = QString::number(vMoveAccel->at(j)->y0m / 30);
                        QString vx = QString::number(vMoveAccel->at(j)->vx / 30);
                        QString vy = QString::number(vMoveAccel->at(j)->vy / 30);
                        QString ax = QString::number(vMoveAccel->at(j)->ax / 30);
                        QString ay = QString::number(vMoveAccel->at(j)->ay / 30);

                        xl0 = new QLabel("x0=");
                        xA_0->setText(x0);

                        yl0 = new QLabel("y0=");
                        yA_0->setText(y0);

                        vxl1 = new QLabel("v0_x=");
                        vA_x->setText(vx);

                        vyl1 = new QLabel("v0_y=");
                        vA_y->setText(vy);

                        axl1 = new QLabel("a_x=");
                        aA_x->setText(ax);

                        ayl1 = new QLabel("a_y=");
                        aA_y->setText(ay);

                        v1->addWidget(xl0);
                        v1->addWidget(xA_0);
                        v2->addWidget(yl0);
                        v2->addWidget(yA_0);
                        v3->addWidget(vxl1);
                        v3->addWidget(vA_x);
                        v4->addWidget(vyl1);
                        v4->addWidget(vA_y);
                        v5->addWidget(axl1);
                        v5->addWidget(aA_x);
                        v6->addWidget(ayl1);
                        v6->addWidget(aA_y);

                        lay = new QVBoxLayout;
                        lay->addLayout(v1);
                        lay->addLayout(v2);
                        lay->addLayout(v3);
                        lay->addLayout(v4);
                        lay->addLayout(v5);
                        lay->addLayout(v6);
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

    connect(xA_0, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItemAccel()));
    connect(yA_0, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItemAccel()));
    connect(vA_x, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItemAccel()));
    connect(vA_y, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItemAccel()));
    connect(aA_x, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItemAccel()));
    connect(aA_y, SIGNAL(textChanged(QString)), this, SLOT(change_MoveMultItemAccel()));

}

void FormProjectX::change_MoveMultItem()
{
    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            for (int j = 0; j < vMovePB->size(); j++){
                if (vMovePB->at(j)->item_k == i){
                    vMovePB->at(j)->x0m = xV_0->text().toDouble() * 30;
                    vMovePB->at(j)->y0m = yV_0->text().toDouble() * 30;
                    vMovePB->at(j)->xm = vMovePB->at(j)->x0m;
                    vMovePB->at(j)->ym = vMovePB->at(j)->y0m;
                    vMovePB->at(j)->vx = vV_x->text().toDouble() * 30;
                    vMovePB->at(j)->vy = vV_y->text().toDouble() * 30;
                }
            }
        }
    }
}

void FormProjectX::change_MoveMultItemAccel()
{
    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            for (int j = 0; j < vMoveAccel->size(); j++){
                if (vMoveAccel->at(j)->item_k == i){
                    vMoveAccel->at(j)->x0m = xA_0->text().toDouble() * 30;
                    vMoveAccel->at(j)->y0m = yA_0->text().toDouble() * 30;
                    vMoveAccel->at(j)->dx = vMoveAccel->at(j)->x0m;
                    vMoveAccel->at(j)->dy = vMoveAccel->at(j)->y0m;
                    vMoveAccel->at(j)->vx = vA_x->text().toDouble() * 30;
                    vMoveAccel->at(j)->vy = vA_y->text().toDouble() * 30;
                    vMoveAccel->at(j)->ax = aA_x->text().toDouble() * 30;
                    vMoveAccel->at(j)->ay = aA_y->text().toDouble() * 30;
                }
            }
        }
    }
}

void FormProjectX::change_scene_clicked(){

    if (laynowcoor->isEmpty()){
        vnc1 = new QHBoxLayout;
        vnc2 = new QHBoxLayout;
        xl1 = new QLabel("x=");
        x = new QLineEdit();
        x->setDisabled(true);
        vnc1->addWidget(xl1);
        vnc1->addWidget(x);
        yl1 = new QLabel("y=");
        y = new QLineEdit();
        y->setDisabled(true);
        vnc2->addWidget(yl1);
        vnc2->addWidget(y);

        laynowcoor = new QVBoxLayout;
        laynowcoor->addLayout(vnc1);
        laynowcoor->addLayout(vnc2);

        for (int i = 0; i < vItem->size(); i++){
            if (vItem->at(i)->isUnderMouse()){
                QString xpos = QString::number(vItem->at(i)->pos().x() / 30);
                QString ypos = QString::number((-1) * vItem->at(i)->pos().y() / 30);
                x->setText(xpos);
                y->setText(ypos);

                vTree->at(i)->setSelected(true);
                for (int j = 0; j < vTree->size(); j++){
                    if (j != i){
                        vTree->at(j)->setSelected(false);
                    }
                }
                ui->groupBox_8->setLayout(laynowcoor);
            }
        }
    }
    else{
        delete(laynowcoor);
        qDeleteAll( ui->groupBox_8->findChildren<QWidget*>() );

        vnc1 = new QHBoxLayout;
        vnc2 = new QHBoxLayout;
        xl1 = new QLabel("x=");
        x = new QLineEdit();
        x->setDisabled(true);
        vnc1->addWidget(xl1);
        vnc1->addWidget(x);
        yl1 = new QLabel("y=");
        y = new QLineEdit();
        y->setDisabled(true);
        vnc2->addWidget(yl1);
        vnc2->addWidget(y);

        laynowcoor = new QVBoxLayout;
        laynowcoor->addLayout(vnc1);
        laynowcoor->addLayout(vnc2);

        for (int i = 0; i < vItem->size(); i++){
            if (vItem->at(i)->isUnderMouse()){
                QString xpos = QString::number(vItem->at(i)->pos().x() / 30);
                QString ypos = QString::number((-1) * vItem->at(i)->pos().y() / 30);
                x->setText(xpos);
                y->setText(ypos);

                vTree->at(i)->setSelected(true);
                for (int j = 0; j < vTree->size(); j++){
                    if (j != i){
                        vTree->at(j)->setSelected(false);
                    }
                }
                ui->groupBox_8->setLayout(laynowcoor);
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
        timel = new QLabel("t = ");
        time_m = new QLineEdit();
        timehl = new QLabel("dt = ");
        time_h = new QLineEdit();
        QString helpT = QString::number(dtlimit);
        time_m->setText(helpT);
        QString helpH = QString::number(h);
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
        timel = new QLabel("t = ");
        time_m = new QLineEdit();
        timehl = new QLabel("dt = ");
        time_h = new QLineEdit();
        QString helpT = QString::number(dtlimit);
        time_m->setText(helpT);
        QString helpH = QString::number(h);
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
    h = time_h->text().toDouble();

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
        v_x = new QLineEdit("Скорость x");
        v1->addWidget(vxl1);
        v1->addWidget(v_x);
        vyl1 = new QLabel("v_y=");
        v_y = new QLineEdit("Скорость y");
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
        v_x = new QLineEdit("Скорость x");
        v1->addWidget(vxl1);
        v1->addWidget(v_x);
        vyl1 = new QLabel("v_y=");
        v_y = new QLineEdit("Скорость y");
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

    Detail->vx = v_x->text().toDouble() * 30;
    Detail->vy = v_y->text().toDouble() * 30;

    //Поиск элемента выбранного для анимации
    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            Detail->x0m = vItem->at(i)->pos().x();
            Detail->xm = vItem->at(i)->pos().x();
            Detail->y0m = -vItem->at(i)->pos().y();
            Detail->ym = -vItem->at(i)->pos().y();
            Detail->item_k = i;
        }
    }

    vMovePB->push_back(Detail);
    CountItemMovePB++;

    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;

    MovePB->setDisabled(true);
    Accel->setDisabled(true);
}

void FormProjectX::Play_clicked()
{
    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;

    statusAnimation = 2;
    timerv->start(30);
    ui->Play->setEnabled(false);
    ui->Stop->setEnabled(true);
    ui->Pause->setEnabled(true);
}

void FormProjectX::update_xy_formove()
{
    int i = 0;
    helpdtlimit -= helph;

    if (helpdtlimit < -3.39808e-14){
        timerv->stop();
        ui->Play->setEnabled(true);
        ui->Stop->setEnabled(false);
        ui->Pause->setEnabled(false);

        statusAnimation = 0;
        helph = h;
        helpdtlimit = dtlimit;
        for (int i = 0; i < vMovePB->size(); i++){
            vMovePB->at(i)->xm = vMovePB->at(i)->x0m;
            vMovePB->at(i)->ym = vMovePB->at(i)->y0m;
        }
    }
    else{
        while(i != CountItemMovePB){
            vMovePB->at(i)->xm += vMovePB->at(i)->vx * helph;
            vMovePB->at(i)->ym += vMovePB->at(i)->vy * helph;
            vItem->at(vMovePB->at(i)->item_k)->setPos(vMovePB->at(i)->xm, -vMovePB->at(i)->ym);
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
    for (int i = 0; i < vMovePB->size(); i++){
        vMovePB->at(i)->xm = vMovePB->at(i)->x0m;
        vMovePB->at(i)->ym = vMovePB->at(i)->y0m;
    }

    timerv->stop();
    ui->Play->setEnabled(true);
    ui->Stop->setEnabled(false);
    ui->Pause->setEnabled(false);
}

void FormProjectX::Accel_clicked()
{
    if (lay->isEmpty()){
        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        v3 = new QHBoxLayout;
        v4 = new QHBoxLayout;
        vxl1 = new QLabel("v0_x=");
        v_x = new QLineEdit("Скорость x0");
        v1->addWidget(vxl1);
        v1->addWidget(v_x);
        vyl1 = new QLabel("v0_y=");
        v_y = new QLineEdit("Скорость y0");
        v2->addWidget(vyl1);
        v2->addWidget(v_y);
        axl1 = new QLabel("a_x=");
        a_x = new QLineEdit("Ускорение x");
        v3->addWidget(axl1);
        v3->addWidget(a_x);
        ayl1 = new QLabel("a_y=");
        a_y = new QLineEdit("Ускорение y");
        v4->addWidget(ayl1);
        v4->addWidget(a_y);
        AddNewDetailAccel = new QPushButton("Запомнить");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addLayout(v3);
        lay->addLayout(v4);
        lay->addWidget(AddNewDetailAccel);
        ui->groupBox_2->setLayout(lay);
    }
    else{
        delete(lay);
        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        v3 = new QHBoxLayout;
        v4 = new QHBoxLayout;
        vxl1 = new QLabel("v0_x=");
        v_x = new QLineEdit("Скорость x0");
        v1->addWidget(vxl1);
        v1->addWidget(v_x);
        vyl1 = new QLabel("v0_y=");
        v_y = new QLineEdit("Скорость y0");
        v2->addWidget(vyl1);
        v2->addWidget(v_y);
        axl1 = new QLabel("a_x=");
        a_x = new QLineEdit("Ускорение x");
        v3->addWidget(axl1);
        v3->addWidget(a_x);
        ayl1 = new QLabel("a_y=");
        a_y = new QLineEdit("Ускорение y");
        v4->addWidget(ayl1);
        v4->addWidget(a_y);
        AddNewDetailAccel = new QPushButton("Запомнить");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addLayout(v3);
        lay->addLayout(v4);
        lay->addWidget(AddNewDetailAccel);
        ui->groupBox_2->setLayout(lay);
    }

    connect(AddNewDetailAccel, SIGNAL(clicked(bool)), this, SLOT(AddNewDetailAccel_clicked()));
    connect(ui->Play, SIGNAL(pressed()), this, SLOT(Play_clicked_accel()));
}

void FormProjectX::AddNewDetailAccel_clicked()
{
    ui->Play->setEnabled(true);
    ui->Stop->setEnabled(false);
    DetailAccel = new MoveMultItemAccel;

    helpdtlimitAccel = dtlimit;
    helphAccel = h;

    DetailAccel->vx = v_x->text().toDouble() * 30;
    DetailAccel->vy = v_y->text().toDouble() * 30;
    DetailAccel->ax = a_x->text().toDouble() * 30;
    DetailAccel->ay = a_y->text().toDouble() * 30;

    //Поиск элемента выбранного для анимации
    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            DetailAccel->x0m = vItem->at(i)->pos().x();
            DetailAccel->dx = vItem->at(i)->pos().x();
            DetailAccel->y0m = -vItem->at(i)->pos().y();
            DetailAccel->dy = -vItem->at(i)->pos().y();
            DetailAccel->item_k = i;
        }
    }
    vMoveAccel->push_back(DetailAccel);
    CountItemAccel++;

    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;

    MovePB->setDisabled(true);
    Accel->setDisabled(true);
}

void FormProjectX::Play_clicked_accel()
{  
    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;

    statusAnimation = 2;
    timera->start(30);
    ui->Play->setEnabled(false);
    ui->Stop->setEnabled(true);
    ui->Pause->setEnabled(true);
}

void FormProjectX::update_xy_foraccel()
{
    //Обновление координат при движении объекта
    int j = 0;
    th += h;
    helpdtlimitAccel -= helphAccel;

    if (helpdtlimitAccel < -3.39808e-14){
        timera->stop();
        ui->Play->setEnabled(true);
        ui->Stop->setEnabled(false);
        ui->Pause->setEnabled(false);

        statusAnimation = 0;
        helphAccel = h;
        helpdtlimitAccel = dtlimit;
        th = 0;
        for (int i = 0; i < vMoveAccel->size(); i++){
            vMoveAccel->at(i)->dx = vMoveAccel->at(i)->x0m;
            vMoveAccel->at(i)->dy = vMoveAccel->at(i)->y0m;
            vMoveAccel->at(i)->xm = 0;
            vMoveAccel->at(i)->ym = 0;
        }
    }
    else{
        while(j != CountItemAccel){
            vMoveAccel->at(j)->xm = vMoveAccel->at(j)->dx + vMoveAccel->at(j)->vx*th + vMoveAccel->at(j)->ax*th*th/2;
            vMoveAccel->at(j)->ym = vMoveAccel->at(j)->dy + vMoveAccel->at(j)->vy*th + vMoveAccel->at(j)->ay*th*th/2;
            vItem->at(vMoveAccel->at(j)->item_k)->setPos(vMoveAccel->at(j)->xm, -vMoveAccel->at(j)->ym);
            j++;
        }
    }
}

void FormProjectX::Stop_clickedAccel()
{
    //Действие при нажатии на кнопку "Стоп" во время равноускоренного движения
    statusAnimation = 0;

    helphAccel = h;
    helpdtlimitAccel = dtlimit;
    th = 0;
    for (int i = 0; i < vMoveAccel->size(); i++){
        vMoveAccel->at(i)->dx = vMoveAccel->at(i)->x0m;
        vMoveAccel->at(i)->dy = vMoveAccel->at(i)->y0m;
        vMoveAccel->at(i)->xm = 0;
        vMoveAccel->at(i)->ym = 0;
    }

    timera->stop();
    ui->Play->setEnabled(true);
    ui->Stop->setEnabled(false);
    ui->Pause->setEnabled(false);
}

void FormProjectX::Pause_clicked()
{
    statusAnimation = 1;
    timerv->stop();
    timera->stop();

    ui->Stop->setEnabled(false);
    ui->Pause->setEnabled(false);
    ui->Play->setEnabled(true);
}



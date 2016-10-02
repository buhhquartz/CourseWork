#include "formprojectx.h"
#include "ui_formprojectx.h"

FormProjectX::FormProjectX(QWidget *parent) : QMainWindow(parent), ui(new Ui::FormProjectX)
{
    ui->setupUi(this);
    lay = new QVBoxLayout;
    laycontact = new QVBoxLayout;
    layeffect = new QHBoxLayout;
    v1 = new QHBoxLayout;
    v2 = new QHBoxLayout;
    CountItemMovePB = 0;
    CountNow = 0;

    //Создаем таймер для задания равномерного движения мат. точки
    timerv = new QTimer(this);
    connect(timerv, SIGNAL(timeout()), this, SLOT(update_xy_formove()));

    //Создаем таймер для задания равноускоренного движения мат. точки
    timera = new QTimer(this);
    //connect(timera,SIGNAL(timeout()),this,SLOT(update_xy_foraccel()));

    boolv = false;
    boola = false;

    ui->Anim->setEnabled(false);
    ui->stopAnim->setEnabled(false);

    //Сигналы по функциональным кнопкам
    connect(ui->menuFile, SIGNAL(triggered(QAction*)), this, SLOT(close()));

    scene = new SceneProjectX(this);
    ui->View->setScene(scene);

    //Введение системы координат
    scene->setSceneRect(-ui->View->width()/2, -ui->View->height()/2, ui->View->width(), ui->View->height());
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

    //Слот срабатывающий при нажатии на элемент в дереве
    connect(treeProjectD, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(trWi_clicked()));
    //Слот показывающий координаты при нажатии
    connect(treeProjectD, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(change_trWi_clicked()));
    //Слот создающий кнопку движения v-const
    connect(treeProjectD, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(contact_trWi_clicked()));
    //Слот показывающий изменение координат при перемещении объекта сцены
    connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(change_scene_clicked()));
    //Слот срабатывающий при нажатии кнопки "Стоп"
    connect(ui->stopAnim, SIGNAL(clicked(bool)), this, SLOT(stopAnim_clicked()));
    connect(ui->stopAnim, SIGNAL(clicked(bool)), this, SLOT(stopAnim_clickedaccel()));

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
        delE = new QPushButton("Удалить");
        delE->setFixedHeight(30);
        delE->setFixedWidth(65);
        layeffect = new QHBoxLayout;
        layeffect->addWidget(delE);
        ui->groupBox_7->setLayout(layeffect);
    }
    else{
        delete(layeffect);
        qDeleteAll( ui->groupBox_7->findChildren<QWidget*>() );

        delE = new QPushButton("Удалить");
        delE->setFixedHeight(30);
        delE->setFixedWidth(65);
        layeffect = new QHBoxLayout;
        layeffect->addWidget(delE);
        ui->groupBox_7->setLayout(layeffect);
    }
    connect(delE, SIGNAL(clicked(bool)), this, SLOT(delE_clicked()));
}

void FormProjectX::contact_trWi_clicked()
{
    if(laycontact->isEmpty()){
        vm = new QHBoxLayout;
        MovePB = new QPushButton("V");
        MovePB->setFixedHeight(21);
        MovePB->setFixedWidth(21);
        Accel = new QPushButton("A");
        Accel->setFixedHeight(21);
        Accel->setFixedWidth(21);
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
        MovePB = new QPushButton("V");
        MovePB->setFixedHeight(21);
        MovePB->setFixedWidth(21);
        Accel = new QPushButton("A");
        Accel->setFixedHeight(21);
        Accel->setFixedWidth(21);
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
            delete(vItem->at(i));
            vItem->removeAt(i);
            delete(vTree->at(i));
            vTree->removeAt(i);
        }
    }
}

void FormProjectX::change_trWi_clicked()
{
    if (lay->isEmpty()){
        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        xl1 = new QLabel("x=");
        x = new QLineEdit();
        v1->addWidget(xl1);
        v1->addWidget(x);
        yl1 = new QLabel("y=");
        y = new QLineEdit();
        v2->addWidget(yl1);
        v2->addWidget(y);

        for(int i = 0; i < vTree->size(); i++){
            if(vTree->at(i)->isSelected()){
                QString xpos = QString::number(vItem->at(i)->pos().x() / 30);
                QString ypos = QString::number((-1) * vItem->at(i)->pos().y() / 30);
                x->setText(xpos);
                y->setText(ypos);
            }
        }

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        ui->groupBox_2->setLayout(lay);
    }
    else{
        delete(lay);
        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        xl1 = new QLabel("x=");
        x = new QLineEdit();
        v1->addWidget(xl1);
        v1->addWidget(x);
        yl1 = new QLabel("y=");
        y = new QLineEdit();
        v2->addWidget(yl1);
        v2->addWidget(y);

        for(int i = 0; i < vTree->size(); i++){
            if(vTree->at(i)->isSelected()){
                QString xpos = QString::number(vItem->at(i)->pos().x() / 30);
                QString ypos = QString::number((-1) * vItem->at(i)->pos().y() / 30);
                x->setText(xpos);
                y->setText(ypos);
            }
        }

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        ui->groupBox_2->setLayout(lay);
    }
    connect(x, SIGNAL(editingFinished()), this, SLOT(restructure_graphicitem_fromtree()));
    connect(y, SIGNAL(editingFinished()), this, SLOT(restructure_graphicitem_fromtree()));
}

void FormProjectX::restructure_graphicitem_fromtree()
{
    for(int i = 0; i < vTree->size(); i++){
        if(vTree->at(i)->isSelected()){
            double x1, y1;
            x1 = x->text().toDouble() * 30;
            y1 = y->text().toDouble() * 30;
            vItem->at(i)->setPos(x1, (-1)*y1);
        }
    }
}

void FormProjectX::change_scene_clicked(){

    if (lay->isEmpty()){
        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        xl1 = new QLabel("x=");
        x = new QLineEdit();
        v1->addWidget(xl1);
        v1->addWidget(x);
        yl1 = new QLabel("y=");
        y = new QLineEdit();
        v2->addWidget(yl1);
        v2->addWidget(y);

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);

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
                ui->groupBox_2->setLayout(lay);
            }
        }
    }
    else{
        delete(lay);
        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        xl1 = new QLabel("x=");
        x = new QLineEdit();
        v1->addWidget(xl1);
        v1->addWidget(x);
        yl1 = new QLabel("y=");
        y = new QLineEdit();
        v2->addWidget(yl1);
        v2->addWidget(y);

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);

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
                ui->groupBox_2->setLayout(lay);
            }
        }
    }
    connect(x, SIGNAL(editingFinished()), this, SLOT(restructure_graphicitem_fromtree()));
    connect(y, SIGNAL(editingFinished()), this, SLOT(restructure_graphicitem_fromtree()));
}

void FormProjectX::MovePB_clicked()
{
    ui->Anim->setEnabled(true);
    ui->stopAnim->setEnabled(true);
    vx = 0;
    vy = 0;
    boolv = true;
    boola = false;
    if (lay->isEmpty()){
        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        v3 = new QHBoxLayout;
        vxl1 = new QLabel("v_x=");
        v_x = new QLineEdit("Скорость x");
        v1->addWidget(vxl1);
        v1->addWidget(v_x);
        vyl1 = new QLabel("v_y=");
        v_y = new QLineEdit("Скорость y");
        v2->addWidget(vyl1);
        v2->addWidget(v_y);
        timel = new QLabel("t=");
        time_m = new QLineEdit("Время t");
        v3->addWidget(timel);
        v3->addWidget(time_m);
        AddNewDetail = new QPushButton("Добавить деталь");
        name_m = new QLineEdit("Название движения");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addLayout(v3);
        lay->addWidget(AddNewDetail);
        lay->addWidget(name_m);
        ui->groupBox_2->setLayout(lay);
    }
    else{
        delete(lay);
        qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );

        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        v3 = new QHBoxLayout;
        vxl1 = new QLabel("v_x=");
        v_x = new QLineEdit("Скорость x");
        v1->addWidget(vxl1);
        v1->addWidget(v_x);
        vyl1 = new QLabel("v_y=");
        v_y = new QLineEdit("Скорость y");
        v2->addWidget(vyl1);
        v2->addWidget(v_y);
        timel = new QLabel("t=");
        time_m = new QLineEdit("Время t");
        v3->addWidget(timel);
        v3->addWidget(time_m);
        AddNewDetail = new QPushButton("Добавить деталь");
        name_m = new QLineEdit("Название движения");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addLayout(v3);
        lay->addWidget(AddNewDetail);
        lay->addWidget(name_m);
        ui->groupBox_2->setLayout(lay);
    }
    if (boolv){
        connect(AddNewDetail, SIGNAL(clicked(bool)), this, SLOT(AddNewDetail_clicked()));
        connect(ui->Anim, SIGNAL(pressed()), this, SLOT(Anim_clicked()));
    }
}

void FormProjectX::AddNewDetail_clicked()
{
    Detail = new MoveMultItem;
    if(vx == 0 && vy == 0 && boolv){
        Detail->h = time_m->text().toDouble() / 100;
        Detail->dtlimit  = time_m->text().toDouble();
        Detail->vx = v_x->text().toDouble() * 30;
        Detail->vy = v_y->text().toDouble() * 30;

        //Поиск элемента выбранного для анимации
        for(int i = 0; i < vTree->size(); i++){
            if(vTree->at(i)->isSelected()){
                Detail->xm = vItem->at(i)->pos().x();
                Detail->ym = -vItem->at(i)->pos().y();
                Detail->item_k = i;
            }
        }
        Detail->end = true;
        vMovePB->push_back(Detail);
        CountItemMovePB++;
    }
    delete(lay);
    qDeleteAll( ui->groupBox_2->findChildren<QWidget*>() );
    lay = new QVBoxLayout;
}

void FormProjectX::Anim_clicked()
{
    if(boolv){
        timerv->start(30);
    }
}

void FormProjectX::update_xy_formove()
{
    if(boolv){
        int i = 0;
        while(i != CountItemMovePB){
            if (vMovePB->at(i)->end){
                vMovePB->at(i)->xm += vMovePB->at(i)->vx * vMovePB->at(i)->h;
                vMovePB->at(i)->ym += vMovePB->at(i)->vy * vMovePB->at(i)->h;
                vMovePB->at(i)->dtlimit -= vMovePB->at(i)->h;
                vItem->at(vMovePB->at(i)->item_k)->setPos(vMovePB->at(i)->xm, -vMovePB->at(i)->ym);
                qDebug() << i;
            }
            if (vMovePB->at(i)->dtlimit <= 0){
                qDebug() << "Вышло время:" << i;
                if (vMovePB->at(i)->end){
                    vMovePB->at(i)->end = false;
                    CountNow++;
                }
            }
            i++;
        }
        if (CountItemMovePB == CountNow){
            qDebug() << "Yeah!";
            timerv->stop();
            ui->Anim->setEnabled(false);
            ui->stopAnim->setEnabled(false);
            CountItemMovePB = 0;
            CountNow = 0;
        }
    }
}

void FormProjectX::stopAnim_clicked()
{
    //Действие при нажатии на кнопку "Стоп" во время равномерного движения
    if (boolv){
        timerv->stop();
    }
}

void FormProjectX::Accel_clicked()
{
    ui->Anim->setEnabled(true);
    ui->stopAnim->setEnabled(true);
    vx = 0;
    vy = 0;
    ax = 0;
    ay = 0;
    th = 0;
    dx = 0;
    dy = 0;
    boola = true;
    boolv = false;
    if (lay->isEmpty()){
        v1 = new QHBoxLayout;
        v2 = new QHBoxLayout;
        v3 = new QHBoxLayout;
        v4 = new QHBoxLayout;
        v5 = new QHBoxLayout;
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
        timel = new QLabel("t=");
        time_m = new QLineEdit("Время t");
        v5->addWidget(timel);
        v5->addWidget(time_m);
        name_m = new QLineEdit("Название движения");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addLayout(v3);
        lay->addLayout(v4);
        lay->addLayout(v5);
        lay->addWidget(name_m);
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
        timel = new QLabel("t=");
        time_m = new QLineEdit("Время t");
        v5->addWidget(timel);
        v5->addWidget(time_m);
        name_m = new QLineEdit("Название движения");

        lay = new QVBoxLayout;
        lay->addLayout(v1);
        lay->addLayout(v2);
        lay->addLayout(v3);
        lay->addLayout(v4);
        lay->addLayout(v5);
        lay->addWidget(name_m);
        ui->groupBox_2->setLayout(lay);
    }
    if (boola){
        connect(ui->Anim, SIGNAL(pressed()), this, SLOT(Anim_clicked_accel()));
    }
}

void FormProjectX::Anim_clicked_accel()
{
    if(vx == 0 && vy == 0 && boola){
        h = time_m->text().toDouble() / 100;
        tlimit  = time_m->text().toDouble();
        dtlimit  = time_m->text().toDouble();
        vx = v_x->text().toDouble() * 30;
        vy = v_y->text().toDouble() * 30;
        ax = a_x->text().toDouble() * 30;
        ay = a_y->text().toDouble() * 30;

        //Поиск элемента выбранного для анимации
        for(int i = 0; i < vTree->size(); i++){
            if(vTree->at(i)->isSelected()){
                dx = vItem->at(i)->pos().x();
                dy = -vItem->at(i)->pos().y();
                item_k = i;
            }
        }
    }
    timera->start(tlimit * 10);
}

void FormProjectX::update_xy_foraccel()
{
    //Обновление координат при движении объекта
    if (boola){
        th += h;
        xm = dx + vx*th + ax*th*th/2;
        ym = dy + vy*th + ay*th*th/2;
        dtlimit -= h;
        vItem->at(item_k)->setPos(xm, -ym);

        //Проверка окончания времени анимации
        if(dtlimit <= 0){
            timera->stop();
            ui->Anim->setEnabled(false);
            ui->stopAnim->setEnabled(false);
        }
    }
}

void FormProjectX::stopAnim_clickedaccel()
{
    //Действие при нажатии на кнопку "Стоп" во время равноускоренного движения
    if(boola){
        timera->stop();
    }
}

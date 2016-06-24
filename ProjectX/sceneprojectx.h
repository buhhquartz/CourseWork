#ifndef SCENEPROJECTX_H
#define SCENEPROJECTX_H
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>


class SceneProjectX : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit SceneProjectX(QObject *parent = 0) : QGraphicsScene(parent) {}
    ~SceneProjectX() {}

    //Установка системы координат
    void coordinate_system();
};

#endif // SCENEPROJECTX_H

#include "sceneprojectx.h"

void SceneProjectX::coordinate_system()
{

    this->addLine(0.0, (-1)*-(height()/2), 0.0, (-1)*(height()/2));
    this->addLine(-(width()/2), 0.0, (width()/2), 0.0);
    this->addText("O")->setPos(-15.0, (-1)*0.0);
    this->addText("X")->setPos((width()/2)-20, 0.0);
    this->addText("Y")->setPos(0.0, (-1)*(height()/2));
    this->addLine(-5.0, (-1)*30.0, 5.0, (-1)*30.0);
    this->addText("1")->setPos(5.0, (-1)*40.0);
    this->addLine(30.0, (-1)*-5.0, 30.0, (-1)*5.0);
    this->addText("1")->setPos(23.0, (-1)*-5.0);
}

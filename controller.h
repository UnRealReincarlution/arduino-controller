#pragma once

#include <string.h>
#include <Vector.h>

#include "HardwareSerial.h"
#include "SoftwareSerial.h"

// #include "json.hpp"
// using json = nlohmann::json;

struct Node
{
private:
    void updateValue(String type); // Update the value on the hardware
    int getValue(); // Get Hardware Value

public:
    int pin;
    int value;
    String name;
    String unit;

    Node(int pin, int value, String name, String unit);
    int readValue(String type);
    void setValue(int value);
};

struct Motor {
private:
    void updateValue();
    void getValue();

public:
    int pin0; // First pin connected PIN
    int pin1; // Second pin connected PIN
    int pwm; // Pulse Width Modulated PIN

    int state; 

    /* 
                    POSSIBLE    STATES
        0 stationary (technically braking (0,0))
        1 MOTOR movingForwards (1,0) (-> spinning forwards)
        2 MOTOR movingBackwards (0,1) (-> spinning backwards)
        3 braking (1,1)

        + with rotational value to get where the robot is moving
    */

    String name;

    Motor(int pin0, int pin1, int pwm, String name);
    String getState(void);
    int getStateInt(void);

    void move(int pin0_state, int pin1_state);
    void setVelocity(int newVelocity);
};

// Move Nodes to private and make getNode a editable refrence Node element instead;
class Controller {
private:
    bool console;
    bool noNodes = true;

    bool noNodesMessage = true;
    bool firstItteration = true;

    String logFormat;

public:
    Vector<Node> nodes;
    Vector<Motor> l_motors;
    Vector<Motor> r_motors;

    String boardName = "Robotoc";

    Controller(int serialHz);
    void init();
    // void logNodes(void);
    void addNode(Node node);
    Node getNode(int nodeId);

    void addMotor(Motor motor, String type);
    void move(int leftDirection, int rightDirection, int leftVelocity, int rightVelocity = -1);
};
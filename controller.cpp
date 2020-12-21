#include "controller.h"

Node::Node(int _pin, int _value, String _name, String _unit) {
    value = _value;
    pin = _pin;
    name = _name;
    unit = _unit;

    pinMode(pin, INPUT);

    Serial.println("[+] Node " + name + " Created.")
}

void Node::updateValue(String type) {
    if(type == "DSTD") {
        value = digitalRead(pin);
    }else if(type == "ASTD") {
        value = analongRead(pin);
    }else if (type == "DDIS") {
        value = pulseIn(pin, HIGH);
        int inches = ( value / 147 ) * 2.54;
        int cm = inches * 2.54;
        value = cm; 
    }
}

int Node::getValue(void) {
    return value;

    // Faster Method Reserved for constant type sensors.
    // or re-review functionality
}

int Node::readValue(String type) {
    if(type == "DSTD") {
        value = digitalRead(pin);
    }else if(type == "ASTD") {
        value = analongRead(pin);
    }else if (type == "DDIS") {
        value = pulseIn(pin, HIGH);
        int inches = ( value / 147 ) * 2.54;
        int cm = inches * 2.54;
        value = cm; 
    }
    
    return value;
}

void Node::setValue(int _value) {
    value = _value;
    updateValue();
}

Motor::Motor(int _pin0, int _pin1, int _pwm, String _name) {
    pin0 = _pin0;
    pin1 = _pin1;
    name = _name;
    pwm = _pwm;

    // Initialize Motor
    pinMode(pin0, OUTPUT);
    pinMode(pin1, OUTPUT);
    pinMode(pwm, OUTPUT);

    Serial.println("[+] Motor " + name + " Created.");
}

int Motor::getStateInt() {
    return state;
}

String Motor::getState() {
    if(state == 0) return "SBrake";
    else if(state == 1) return "Forwards";
    else if(state == 2) return "Backwards";
    else return "Brake";
}

void Motor::move(int _pin0_state, int _pin1_state) {
    digitalWrite(pin0, (_pin0_state == 1) ? HIGH : LOW);
    digitalWrite(pin1, (_pin1_state == 1) ? HIGH : LOW);

    if(_pin0_state == 0 && _pin1_state == 0) {
        state = 0;
    }else if(_pin0_state == 1 && _pin1_state == 0) {
        state = 1;
    }else if(_pin0_state == 0 && _pin1_state == 1) {
        state = 2;
    }else {
        state = 3;
    }
}

void Motor::setVelocity(int newVelocity) {
    if(newVelocity >= 0 && newVelocity <= 255) {
        analogWrite(pwm, newVelocity);
    }
}

Controller::Controller(int serialHz) {
    Serial.begin(serialHz);
    Serial.println("\n\nLibrary Declared, No Issues Found.");
    /*
    time_t t = time(0); // get time now
    struct tm * _now = localtime(& t);

    char buffer [80];
    strftime (buffer,80,"%Y-%m-%d_%H-%M",_now);
    String str(buffer);

    s = str;
    */
};

/*
void Controller::logNodes(void) {
    time_t now = time(0);
    char* dt = ctime(&now);

    //std::fstream logFile;
    //logFile.open("./logs/rl-" + s +".txt", std::fstream::app);

    if(firstItteration) {
        // logFile << "Log for " << boardName << " starting at " << dt << " \n" << "Format: " << logFormat << std::endl << "Connected Nodes: " << nodes.size() << " " << std::endl << "Log Delay: " << logDelay << std::endl  << "\nFlight Systems Nominal, we are go for flight."  << std::endl << std::endl;
        Serial.println("Log for " + boardName + " starting at " + dt + " \nFormat: " + logFormat + "Connected Nodes: " + (char) nodes.size() + " Log Delay: " + (char) logDelay + "\nFlight Systems Nominal, we are go for flight. \n\n");
        firstItteration = false;
    }

    //if (logFile.is_open()) {
        for (int i = 0; i < nodes.size(); i++) {
            if(logFormat == "minimal") {
                //logFile << nodes[i].readValue() << " "; //logFile
                if(console) Serial.println(nodes[i].readValue()); // ConsoleOutput
            }else {
                //logFile << "\nPIN: " << nodes[i].pin << " >> " << nodes[i].readValue() << " " << nodes[i].unit << " (" << nodes[i].name << ")"; //logFile
                if(console) Serial.println("\nPIN: " + (char) (nodes[i].pin) + " >> " + nodes[i].readValue() + " " + nodes[i].unit + " (" + nodes[i].name + ")"); // ConsoleOutput
            }
        }

        if(nodes.size() > 1 && noNodes) {
            noNodes = false;
        }else if(noNodesMessage && noNodes) {
            noNodesMessage = false;
            Serial.println("\n\nNo Nodes established. \n Hint: They may be connected to the board but are not initialized in the software. \nTo intialize it; use <ControllerName>.addNode(Node(int: pinValue, int: defaultValue, string: vehicleName)) \n\n"); //Checking again in " << logDelay << ((logDelay == 1) ? " seccond... " : " secconds...");
        }

        //logFile << std::endl;
        if(console) Serial.println("\n");

        //logFile.close();
    // }
    // else {
    //     Serial.println("Unable to open file");
    // }
}
*/

void Controller::init() {
    Serial.println("\nROBOTOC INITIALIZED.\n\n\n");

    // Run initialization functions...
}

void Controller::addNode(Node node) {
    nodes.push_back(node);
}

Node Controller::getNode(int nodePin) {
    for (int i = 0; i < nodes.size(); i++) {
        if(nodes[i].pin == nodePin) return nodes[i]; //return &nodes.at(i);
    }

    return Node(-1, 0, "No Node Found", "N/A");
}

void Controller::addMotor(Motor motor, String type) {
    if(type == "Left") {
        l_motors.push_back(motor);
    }else if(type == "Right") {
        r_motors.push_back(motor);
    }else{
        Serial.println("No valid type was presented for the motor. Valid types are 'Left' and 'Right'.")
        return;
    }
}

void Controller::move(int leftDirection, int rightDirection, int leftVelocity, int rightVelocity = -1) {
    if(rightVelocity == -1) rightVelocity = leftVelocity;

    if(l_motors.size() < 1) { 
        Serial.println("[ERR] Insufficent number of left motors");
        return;
    }

    for(int i = 0; i < l_motors.size() - 1; i++) {
        if(leftDirection == 1) l_motors[i].move(1, 0); // RIGHT
        else if(leftDirection == 0) l_motors[i].move(0, 0); // NO MOV - LOCK
        else if(leftDirection == -1) l_motors[i].move(0, 1); // LEFT

        l_motors[i].setVelocity(leftVelocity);
    }

    if(r_motors.size() < 1) {
        Serial.println("[ERR] Insufficent number of right motors");
        return;
    }

    for(int i = 0; i < r_motors.size() - 1; i++) {
        if(rightDirection == 1) r_motors[i].move(1, 0); // RIGHT
        else if(rightDirection == 0) r_motors[i].move(0, 0); // NO MOV - LOCK
        else if(rightDirection == -1) r_motors[i].move(0, 1); // LEFT

        r_motors[i].setVelocity(rightVelocity);
    }
}
#include "include/SDLController.h"

SDL_GameController* SDLController::mController = nullptr;
BleGattClient* SDLController::mBleGattClient = nullptr;
QTimer* SDLController::mTimer = nullptr;


void SDLController::init(QObject* parent, BleGattClient *bleGattClient)
{
    qDebug() << "Initialiesiere Controller";
    if(bleGattClient && bleGattClient != SDLController::mBleGattClient)
    {
        SDLController::mBleGattClient = bleGattClient;
        qDebug() << "Bluetooth Client wird verbunden";
    }

    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        qDebug() << "SDL konnte nicht initialisiert werden:" << SDL_GetError();
        return;
    }
}

void SDLController::connectToController()
{
    SDLController::mTimer = new QTimer();
    if (SDL_NumJoysticks() < 1) {
        qDebug() << "Kein Controller angeschlossen.";
    } else {
        if (SDL_IsGameController(0)) {
            SDLController::mController = SDL_GameControllerOpen(0);
            if (SDLController::mController) {
                qDebug() << "Controller verbunden:" << SDL_GameControllerName(SDLController::mController);
            } else {
                qDebug() << "Konnte Controller nicht öffnen:" << SDL_GetError();
            }
        }
    }


    connect(mTimer, &QTimer::timeout, &SDLController::instance(), &SDLController::onUpdate);
    SDLController::mTimer->start(100);
}

void SDLController::onUpdate()
{
    SDL_GameControllerUpdate();
    constexpr int DEADZONE = 1000;


    Sint16 leftY = SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_LEFTY);
    if (std::abs(leftY) > DEADZONE) {
        onLeftStickMoved();
    }

    // --- Right Stick X (Left/Right only) ---
    Sint16 rightX = SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_RIGHTX);
    if (std::abs(rightX) > DEADZONE) {
        if (rightX < 0) {
            onLeftStickMoved();
            steeringCycle = 0;
        } else if (rightX > 0) {
            onRightStickMoved();
            steeringCycle = 0;
        }
    } else {
        if(steeringCycle <= 6)
            steeringCycle ++;
        if(steeringCycle >= 5) {
            onNoDirectionMoved();
        }
    }
}

void SDLController::onNoDirectionMoved()
{
    if(mBleGattClient)
    {
        if(lastDirection == 1) { // left
            mBleGattClient->sendCommand(2);
            qDebug() << "Back to Mid";
            lastDirection = 0;
        } else if(lastDirection == 2) {
            mBleGattClient->sendCommand(1);
            qDebug() << "Back to Mid";
            lastDirection = 0;
        }
    }
}

void SDLController::onRightStickMoved()
{
    if(mBleGattClient && lastDirection != 2)
    {
        lastDirection = 2;
        mBleGattClient->sendCommand(2);
        qDebug() << "Right";
    }
}

void SDLController::onLeftStickMoved()
{
    if(mBleGattClient && lastDirection != 1)
    {
        lastDirection = 1;
        mBleGattClient->sendCommand(1);
        qDebug() << "Left";
    }
}


void SDLController::onButtonPressed()
{

}

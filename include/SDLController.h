#ifndef SDLCONTROLLER_H
#define SDLCONTROLLER_H

#include "BleGattClient.h"
#include <SDL.h>
#include <QObject>
#include <QTimer>

class SDLController : public QObject {
    Q_OBJECT
public:

    static constexpr Sint16 HIGHEST_STATE = 32767;
    static constexpr Sint16 LOWEST_STATE = -32767;


    static SDLController& instance() {
        static SDLController instance;
        return instance;
    }

    // Add your public methods here

    static void init(QObject* parent = nullptr, BleGattClient *bleGattClient = nullptr);
    Q_INVOKABLE void connectToController();


private:
    SDLController(QObject *parent = nullptr) : QObject(parent) {}
    ~SDLController() = default;
    SDLController(const SDLController&) = delete;
    SDLController& operator=(const SDLController&) = delete;

    static SDL_GameController* mController;
    static BleGattClient* mBleGattClient;
    static QTimer* mTimer;

    static constexpr int toPercent(Sint16 value) {
        return static_cast<int>((value * 100.0f) / 32767.0f);
    }

    void onRightStickMoved();
    void onLeftStickMoved();
    void onButtonPressed();
    void onNoDirectionMoved();

    uint8_t steeringCycle = 0;
    uint8_t lastDirection = 0;

private slots:
    void onUpdate();
};

#endif // SDLCONTROLLER_H

#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#include <QObject>

class ButtonManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool State READ State WRITE setMState NOTIFY mStateChanged FINAL)
public:
    ButtonManager(QObject *parent = nullptr) : QObject(parent){};

    bool State() const
    {
        return mState;
    }
    void setMState(bool state)
    {
        if (mState != state) {
            mState = state;
            emit mStateChanged(mState);
        }
    }

signals:
    void mStateChanged(bool state);


private:
    bool mState = false; // Default state is false



};

#endif // BUTTONMANAGER_H

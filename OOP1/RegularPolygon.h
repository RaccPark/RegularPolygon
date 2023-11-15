#pragma once

#include "GameObject.h"
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

#define PI 3.14159265f

enum class SelectState {
    Idle,
    isSelecting,
    Selected,
};

class RegularPolygon :
    public GameObject
{
    vector<Vector2> points;
    float   radius;
    float   rotationSpeed;

    SelectState state;

    int n_frames;

    void configure(unsigned int n) {
        if (n == 0 || n == 1) return;
        points.clear();
        double angleInRadian = 2.0 * PI / n;

        for (unsigned int i = 0; i < n; i++) {
            Debug::Log("shape[%d] (%f, %f)", i, cos(angleInRadian * i), cos(angleInRadian * i));
            points.push_back({ (float)cos(angleInRadian * i), (float)sin(angleInRadian * i) });
        }
    }

    

public:
    // set the center position to its game object position.
    RegularPolygon(int n, const Vector2& center, float radius) : GameObject("polygon", center, { (int)radius, (int)radius }, true),
        radius(radius), rotationSpeed(PI / 180.0f), // rotation speed by one degree
        n_frames(0),
        state(SelectState::Idle)
    {
        configure(n);
    }
    void rotate();
    void update(InputManager& input) override;
    void draw() override;

    void setState(SelectState state) { this->state = state; }

    void setBlinkingPeriod(int n_frames) { this->n_frames = n_frames; }
    float getRotationSpeed() const { return this->rotationSpeed; }
    void setRotationSpeed(const float speed) { this->rotationSpeed = speed; }
    float getRadius() const { return this->radius; }
    void setRadius(const float radius) { this->radius = radius; }




    RegularPolygon& operator++() {
        configure((int)points.size() + 1);
        return *this;
    }
    RegularPolygon& operator--(int unused) {
        if (points.size() <= 2) return *this;
        configure((int)points.size() - 1);
        return *this;
    }
};


/*
    This is an adaptation of work from Franciszek Szewczyk, who made an intuitive article
    explaining 2D rope physics and methods of implementation.
    The gist: https://gist.github.com/fszewczyk/46915c02a34a1833d83a3c2fd851b7a0
    The article: https://medium.com/@szewczyk.franciszek02/rope-simulator-in-c-a595a3ef956c
    
    The main changes here are the removal of gravity, the adaptation of types to IsaacRepentance ones,
    and handling rendering through BeamRenderer.
*/

#pragma once

#include "IsaacRepentance.h"
#include <math.h>
#include <stdlib.h>
#include <vector>

class Rope {
  public:
    Rope(unsigned int nPoints, float timestep, unsigned int iterations, float spriteStretchHeight, float spriteStretchWidth) {
        printf("Rope: constructing\n");
        _numPoints = nPoints;
        _timestep = timestep;
        _iterations = iterations;
        _spriteStretch = spriteStretchHeight;
        _spriteWidth = spriteStretchWidth;
        _desiredDistance = 0.0f;
        Vector const zero;

        for (unsigned int i = 0; i < 2; i++) {
            printf("Rope: making point #%d\n", i);
            Point p(zero, (Entity*)nullptr, 0.0f, spriteStretchWidth, false);
            printf("Rope: pushing point #%d\n", i);
            _points.deque.push_back(p);
        }
        printf("Rope: points %p, %p\n", &_points.deque.front(), &_points.deque.back());
        printf("Rope: done constructing\n");
    }

    void Init() {
        printf("Rope::Init starting, using deque %p\n", &_points);
        if (_points.deque.size() < 2) {
            printf("not enough points\n");
            return;
        }
        Point* startPoint = &_points.deque.front();
        Point endPoint = _points.deque.back();
        Vector* startPos = startPoint->_target ? startPoint->_target->GetPosition() : &startPoint->_pos;
        Vector* endPos = endPoint._target ? endPoint._target->GetPosition() : &endPoint._pos;

        printf("startPos: %f %f, endPos: %f %f\n", startPos->x, startPos->y, endPos->x, endPos->y);

        float ropeLength = (float)sqrt(pow(startPos->x - endPos->x, 2) + pow(startPos->y - endPos->y, 2));

        // There is one less segment than there are Points
        unsigned int numSegments = _numPoints - 1;

        _desiredDistance = ropeLength / numSegments;

        printf("length: %f, segments : %d, avg distance: %f\n", ropeLength, numSegments, _desiredDistance);

        _points.deque.pop_back();
        for (unsigned int i = 1; i < _numPoints; i++) {
            // percent distance from start to end, 0.0->1.0
            float w = (float)i / (_numPoints - 1);

            Vector pos(w * endPos->x + (1 - w) * startPos->x, w * endPos->y + (1 - w) * startPos->y);
            float spritesheetCoord = ropeLength * w * _spriteStretch;

            if (i == _numPoints - 1) {
                endPoint._pos = pos;
                endPoint._spritesheetCoordinate = spritesheetCoord;
                _points.deque.push_back(endPoint);
                printf("readded end point at pos %f, %f with coord %f and fixed %d\n", endPoint._pos.x, endPoint._pos.y, endPoint._spritesheetCoordinate, endPoint._target != nullptr);
            }
            else
            {
                Point p(pos, nullptr, spritesheetCoord, _spriteWidth, false);
                _points.deque.push_back(p);
                printf("created point at pos %f, %f with coord %f and fixed %d\n", p._pos.x, p._pos.y, p._spritesheetCoordinate, p._fixed);
            }
        }
        _initialized = true;
        printf("Rope::Init ending, size %d\n", _points.deque.size());
    }

    void Update() {
        //printf("Rope::Update starting, using deque %p\n", &_points);
        if (!_initialized) {
            printf("Cord::Update: not initialzed\n");
            return;
        }
        verletIntegration();
        enforceConstraints();
        //printf("Rope::Update finished\n");
    }

    void Render(ANM2* anm2, unsigned int layerID, bool useOverlay, bool unk) {
        //printf("Rope::Render starting, using deque %p\n", &_points);
        if (_points.deque.size() < 2) {
            printf("Rope::Render: < 2 points, size is %d\n", _points.deque.size());
            return;
        }
        if (!_initialized) {
            printf("Rope::Render: not initialzed\n");
            return;
        }  

        g_BeamRenderer->Begin(anm2, layerID, useOverlay, unk);

        #pragma warning(suppress:4533) 
        ColorMod color;
        Vector buffer;
        for (auto it = _points.deque.begin(); it != _points.deque.end(); ++it) {
            Isaac::WorldToScreen(&buffer, &it->_pos);
            g_BeamRenderer->Add(&buffer, &color, it->_width, it->_spritesheetCoordinate);
        }

        g_BeamRenderer->End();
    }

    // We need to store our Points somewhere
    PointDeque _points;

    // Target distance of a single segment
    float _desiredDistance;
    float _timestep = 0.01f;
    float _spriteStretch, _spriteWidth = 1;
    unsigned int _numPoints = 2;
    unsigned int _iterations = 10;
    bool _initialized = false;

  private:
    void verletIntegration() {
        printf("verletIntegration: deque size %d\n", _points.deque.size());
        int i = 0;
        for (Point& p : _points.deque) {
            if (p._target) {
                p._pos = *p._target->GetPosition();
                p._lastPos = p._pos;
                continue;
            }
            // We do not want to move fixed Points
            if (p._fixed)
                continue; 

            Vector copy = p._pos;

            // Calculating previous velocity
            Vector velocity((p._pos.x - p._lastPos.x) / _timestep, (p._pos.y - p._lastPos.y) / _timestep);

            // Updating Point's position
            p._pos += velocity * _timestep;

            p._lastPos = copy;
            i++;
        }
        printf("verletIntegration: ran %d times, deque size %d\n", i, _points.deque.size());
    }

    void enforceConstraints() {
        printf("enforceConstraints: iterations %d, deque size %d\n", _iterations, _points.deque.size());
        // We perform the enforcement multiple times
        for (unsigned int iteration = 0; iteration < _iterations; iteration++) {
            //printf("iteration %d: ", iteration);
            // We iterate over each pair of points
            for (size_t i = 1; i < _points.deque.size(); i++) {
                Point &p1 = _points.deque[i - 1];
                Point &p2 = _points.deque[i];

                // Calculating distance between the Points
                float distance = (float)sqrt(pow(p1._pos.x - p2._pos.x, 2) + pow(p1._pos.y - p2._pos.y, 2));
                float distanceError = distance - _desiredDistance;

                //printf("distance %f, error %f, ", distance, distanceError);

                 // The direction in which Points should be pulled or pushed
                Vector difference(p2._pos.x - p1._pos.x, p2._pos.y - p1._pos.y);
                //printf("difference %f, %f, ", difference.x, difference.y);

                // We need to make it a unit vector
                // This will allow us to easily scale the impact we have
                // on each Point's position.
                Vector direction(difference.x / (float)sqrt(pow(difference.x, 2) + pow(difference.y, 2)), difference.y / (float)sqrt(pow(difference.x, 2) + pow(difference.y, 2)));
                //printf("direction %f, %f\n", difference.x, difference.y);

                // Finally, we can update Points' positions
                // We need to remember that fixed Points should stay in place
                if (p1._fixed && !p2._fixed) {
                    p2._pos -= direction * distanceError;
                    //printf("moved p2, now %f, %f\n", p2._pos.x, p2._pos.y);
                } else if (p2._fixed && !p1._fixed) {
                    p1._pos += direction * distanceError;
                    //printf("moved p1, now %f, %f\n", p1._pos.x, p1._pos.y);
                // keeping this comparison in case both points are fixed
                } else if (!p1._fixed && !p2._fixed) {
                    p2._pos -= direction * distanceError * 0.5;
                    //printf("moved p2 halfway, now %f, %f\n", p2._pos.x, p2._pos.y);
                    p1._pos += direction * distanceError * 0.5;
                    //printf("moved p1 halfway, now %f, %f\n", p1._pos.x, p1._pos.y);
                }
                //__debugbreak();
            }
        }
    }
};

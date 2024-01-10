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

float TIMESTEP = 0.01f;             // Δt in our equations
unsigned int JAKOBSEN_ITERATIONS = 50; // number of times we will enforce the distance constraint

class Rope {
  public:
    Rope(Entity* origin, Vector* end, unsigned int nPoints, float spriteStretchHeight, float spriteStretchWidth) {
        printf("constructing rope: entity %p, start %f %f, end %f %f, nPoints %d, spriteStretchHeight %f, float spriteStretchWidth %f\n", origin, origin->GetPosition()->x, origin->GetPosition()->y, end->x, end->y, nPoints, spriteStretchHeight, spriteStretchWidth);
        _originEntity = origin;
        _target = *end;
        Vector* start = origin->GetPosition();

        float ropeLength = (float)sqrt(pow(start->x - _target.x, 2) + pow(start->y - _target.y, 2));
       
        // There is one less segment than there are Points
        unsigned int numSegments = nPoints - 1;

        _desiredDistance = ropeLength / numSegments;

        printf("length: %f, segments : %d, avg distance: %f\n", ropeLength, numSegments, _desiredDistance);

        for (unsigned int i = 0; i < nPoints; i++) {
            // percent distance from start to end, 0.0->1.0
            float w = (float)i / (nPoints - 1);

            Vector pos(w * _target.x + (1 - w) * start->x, w * _target.y + (1 - w) * start->y);
            float spritesheetCoord = ropeLength * w * spriteStretchHeight;

            Point p(pos, spritesheetCoord, spriteStretchWidth, i == 0);
            printf("created point at pos %f, %f with coord %f\n", p._pos.x, p._pos.y, p._spritesheetCoordinate);

            _points.push_back(p);
        }
    }

    void Update() {
        verletIntegration();
        enforceConstraints();
    }

    void Render(ANM2* anm2, unsigned int layerID, bool useOverlay, bool unk) {
        __debugbreak();
        if (_points.size() < 2)
            return;

        g_BeamRenderer->Begin(anm2, layerID, useOverlay, unk);

        #pragma warning(suppress:4533) 
        ColorMod color;
        Vector buffer;
        for (auto it = _points.begin(); it != _points.end(); ++it) {
            Isaac::WorldToScreen(&buffer, &it->_pos);
            g_BeamRenderer->Add(&buffer, &color, it->_width, it->_spritesheetCoordinate);
        }

        g_BeamRenderer->End();
    }

    // We need to store our Points somewhere
    std::deque<Point> _points;

    // Target distance of a single segment
    float _desiredDistance;

    Entity * _originEntity;
    Vector _target;

  private:
    void verletIntegration() {
        if (_originEntity != nullptr) {
            _points.begin()->_pos = *_originEntity->GetPosition();
            _points.begin()->_lastPos = _points.begin()->_pos;
        }

        for (auto p = _points.begin()+1; p != _points.end(); ++p) {
            // We do not want to move fixed Points
            if (p->_fixed)
                continue; 

            Vector copy = p->_pos;

            // Calculating previous velocity
            Vector velocity((p->_pos.x - p->_lastPos.x) / TIMESTEP, (p->_pos.y - p->_lastPos.y) / TIMESTEP);

            // Updating Point's position
            p->_pos += velocity * TIMESTEP;

            p->_lastPos = copy;
        }
    }

    void enforceConstraints() {
        // We perform the enforcement multiple times
        for (unsigned int iteration = 0; iteration < JAKOBSEN_ITERATIONS; iteration++) {
            // We iterate over each pair of points
            for (size_t i = 1; i < _points.size(); i++) {
                Point &p1 = _points[i - 1];
                Point &p2 = _points[i];

                // Calculating distance between the Points
                float distance = (float)sqrt(pow(p1._pos.x - p2._pos.x, 2) + pow(p1._pos.y - p2._pos.y, 2));
                float distanceError = distance - _desiredDistance;

                 // The direction in which Points should be pulled or pushed
                Vector difference(p2._pos.x - p1._pos.x, p2._pos.y - p1._pos.y);

                // We need to make it a unit vector
                // This will allow us to easily scale the impact we have
                // on each Point's position.
                Vector direction(difference.x / (float)sqrt(pow(difference.x, 2) + pow(difference.y, 2)), difference.y / (float)sqrt(pow(difference.x, 2) + pow(difference.y, 2)));

                // Finally, we can update Points' positions
                // We need to remember that fixed Points should stay in place
                if (p1._fixed && !p2._fixed) {
                    p2._pos -= direction * distanceError;
                } else if (p2._fixed && !p1._fixed) {
                    p1._pos += direction * distanceError;
                // keeping this comparison in case both points are fixed
                } else if (!p1._fixed && !p2._fixed) {  
                    p2._pos -= direction * distanceError * 0.5;
                    p2._pos += direction * distanceError * 0.5;
                }
            }
        }
    }
};

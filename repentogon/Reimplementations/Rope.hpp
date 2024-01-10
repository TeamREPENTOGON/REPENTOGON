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
    Rope(Vector* start, Vector* end, unsigned int nPoints, float spriteStretchHeight, float spriteStretchWidth) {
        _origin = *start;
        _target = *end;

        float ropeLength = (float)sqrt(pow(_origin.x - _target.x, 2) + pow(_origin.y - _target.y, 2));
       
        // There is one less segment than there are Points
        unsigned int numSegments = nPoints - 1;

        _desiredDistance = ropeLength / numSegments;

        for (unsigned int i = 0; i < nPoints; i++) {
            // percent distance from start to end, 0.0->1.0
            float w = (float)i / (nPoints - 1);

            Vector pos(w * _target.x + (1 - w) * _origin.x, w * _target.y + (1 - w) * _origin.y);
            float spritesheetCoord = ropeLength * w * spriteStretchHeight;

            Point p(pos, spritesheetCoord, spriteStretchWidth, false);

            _points.push_back(p);
        }
    }

    void Update(Vector * newOrigin, Vector * newTarget) {
        verletIntegration(newOrigin, newTarget);
        enforceConstraints();
    }

    void Render(ANM2* anm2, unsigned int layerID, bool useOverlay, bool unk) {
        g_BeamRenderer->Begin(anm2, layerID, useOverlay, unk);

        #pragma warning(suppress:4533) 
        ColorMod color;
        for (auto it = _points.begin(); it != _points.end(); ++it) {
            g_BeamRenderer->Add(&it->_pos, &color, it->_width, it->_spritesheetCoordinate);
        }

        g_BeamRenderer->End();
    }

    // We need to store our Points somewhere
    std::deque<Point> _points;

    // Target distance of a single segment
    float _desiredDistance;

    Vector _origin;
    Vector _target;

  private:
    void verletIntegration(Vector* newOrigin, Vector* newTarget) {
        for (auto p = _points.begin(); p != _points.end(); ++p) {
            if (p->_fixed)
                continue; // We do not want to move fixed Points
            if (p == _points.begin()) {
                p->_pos = *newOrigin;
                continue;
            }
            if (p == _points.end() - 1) {
                p->_pos = *newTarget;
                continue;
            }
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

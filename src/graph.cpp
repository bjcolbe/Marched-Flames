#include "graph.h"

#include <algorithm>
#include <cmath>

using namespace std;

Model Graph::create(float resolution, float width, float height, float length, function<float(float, float)> function) {
    Model model;

    for(auto x = -width / 2.0f; x <= width / 2.0f; x += 1.0f / resolution)
        for(auto z = -length / 2.0f; z <= length / 2.0f; z += 1.0f / resolution) {
            auto y = function(x, z);

            model.vertices.push_back({x, y, z});
            model.colors.push_back({
                min(1.0f, 0.1f + x / (width / 2)),
                min(1.0f, 0.1f + y / (height / 2)),
                min(1.0f, 0.1f + z / (length / 2))
            });
        }

    return model;
}
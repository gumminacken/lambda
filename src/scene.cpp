#include "lambda.h"

Lt::Circle Li::Circle(float radius, Lt::Vec3f center) {
    return Lt::Circle {
        radius,
        center
    };
}

bool Li::intersect(Lt::Circle *object, Lt::Sample2D sample) {
    Lt::Vec3f sample_vector = Li::Vec3f(sample.x, sample.y, 0);
    return (Li::Vec3f_length(sample_vector - object->center) <= object->radius);
}

bool Li::intersect(Lt::Scene scene, Lt::Sample2D sample) {
    Lt::Grid *grid = Li::get_intersected_grid(&scene, sample.x, sample.y);
    for (size_t i = 0; i < grid->num_circles; ++i) {
        if (Li::intersect(grid->circles + i, sample)) {
            return true;
        }
    }
    return false;
}

Lt::Grid *Li::get_intersected_grid(Lt::Scene *scene, int x, int y) {
    Lt::Grid *grid = scene->grids + ((x / scene->gridsize) + ((y / scene->gridsize) * (WIDTH / scene->gridsize)));
    return grid;
}

int *Li::get_intersected_grid_indices(Lt::Scene *scene, Lt::Circle *circle) {
    Lt::Grid *grid = get_intersected_grid(scene, circle->center.x, circle->center.y);
    int offset_right = (int)ceilf((circle->radius - (grid->maxx - circle->center.x)) / scene->gridsize);
    int offset_bottom = (int)ceilf((circle->radius - (grid->maxy - circle->center.y)) / scene->gridsize);
    int offset_left = (int)ceilf((circle->radius - (circle->center.x - grid->minx)) / scene->gridsize);
    int offset_top = (int)ceilf((circle->radius - (circle->center.y - grid->miny)) / scene->gridsize);
    
    offset_right = offset_right < 0 ? 0 : offset_right;
    offset_left = offset_left < 0 ? 0 : offset_left;
    offset_top = offset_top < 0 ? 0 : offset_top;
    offset_bottom = offset_bottom < 0 ? 0 : offset_bottom;

    int num_indices = (offset_right + offset_left + 1) * (offset_top + offset_bottom + 1);
    int *indices = (int *)malloc(sizeof(int) * (num_indices + 1));
    *(indices + num_indices) = -1;

    for (int y = grid->index - (offset_top * grid->stride); y <= grid->index + (offset_bottom * grid->stride); y += grid->stride) {
        for (int x = y - offset_left; x <= y + offset_right; ++x) {
            *indices = x < 0 ? 0 : (x > scene->num_grids - 1 ? scene->num_grids - 1 : x);
            ++indices;
        }
    }
    indices = indices - num_indices;
    return indices;
}

Lt::Scene Li::scene_create(size_t num_circles, size_t gridsize, float minsize, float maxsize) {
    size_t num_grids = ((WIDTH / gridsize) + 1) * ((HEIGHT / gridsize) + 1);

    Lt::Scene scene = {
        num_circles,
        num_grids,
        gridsize,
        (Lt::Circle *)malloc(sizeof(Lt::Circle) * num_circles),
        (Lt::Grid *)malloc(sizeof(Lt::Grid) * num_grids)
    };

    Lt::Sample2D *samples = Li::sample2D_random(12345, num_circles);

    for (size_t i = 0; i < num_grids; ++i) {
        scene.grids[i].num_circles = 0;
        scene.grids[i].index = i;
        scene.grids[i].stride = WIDTH / gridsize;
        scene.grids[i].minx = (gridsize * i) % WIDTH;
        scene.grids[i].maxx = scene.grids[i].minx + gridsize;
        scene.grids[i].miny = ((gridsize * i) / WIDTH) * gridsize;
        scene.grids[i].maxy = scene.grids[i].miny + gridsize;
        scene.grids[i].circles = (Lt::Circle *)malloc(sizeof(Lt::Circle) * num_circles);
    }

    for (size_t i = 0; i < num_circles; ++i) {
        float size = minsize + ((rand() / float(RAND_MAX)) * (maxsize - minsize));
        Lt::Circle *circle = (Lt::Circle *)malloc(sizeof(Lt::Circle));
        *circle = Li::Circle(
            size,
            Li::Vec3f(samples[i].x * WIDTH, samples[i].y * HEIGHT, 0)
        );

        int *indices = Li::get_intersected_grid_indices(&scene, circle);

        while (*indices != -1) {
            Lt::Grid *grid = scene.grids + *indices;
            memcpy(grid->circles + grid->num_circles, circle, sizeof(Lt::Circle));
            grid->num_circles += 1;
            ++indices;
        }
    }

    return scene;
}

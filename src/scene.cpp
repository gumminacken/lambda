#include <malloc.h>
#include <stdlib.h>

#include "lambda.h"

Lt_Circle Li_Circle(float radius, Lt_Vec3f center) {
    return Lt_Circle {
        radius,
        center
    };
}

bool intersect(Lt_Circle *object, Lt_Sample2D sample) {
    Lt_Vec3f sample_vector = Li_Vec3f(sample.x, sample.y, 0);
    return (Vec3f_length(sample_vector - object->center) <= object->radius);
}

bool intersect(Lt_Scene scene, Lt_Sample2D sample) {
    Lt_Grid *grid = get_intersected_grid(&scene, sample.x, sample.y);
    for (size_t i = 0; i < grid->num_circles; ++i) {
        if (intersect(grid->circles + i, sample)) {
            return true;
        }
    }
    return false;
}

Lt_Grid *get_intersected_grid(Lt_Scene *scene, int x, int y) {
    Lt_Grid *grid = scene->grids + ((x / scene->gridsize) + ((y / scene->gridsize) * (WIDTH / scene->gridsize)));
    return grid;
}

int *get_intersected_grid_indices(Lt_Scene *scene, Lt_Circle *circle) {
    Lt_Grid *grid = get_intersected_grid(scene, circle->center.x, circle->center.y);
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

Lt_Scene create_scene(size_t num_circles, size_t gridsize, float minsize, float maxsize) {
    size_t num_grids = ((WIDTH / gridsize) + 1) * ((HEIGHT / gridsize) + 1);

    Lt_Scene scene = {
        num_circles,
        num_grids,
        gridsize,
        (Lt_Circle *)malloc(sizeof(Lt_Circle) * num_circles),
        (Lt_Grid *)malloc(sizeof(Lt_Grid) * num_grids)
    };

    Lt_Sample2D *samples = sample2D_random(12345, num_circles);

    for (size_t i = 0; i < num_grids; ++i) {
        scene.grids[i].num_circles = 0;
        scene.grids[i].index = i;
        scene.grids[i].stride = WIDTH / gridsize + 1;
        scene.grids[i].minx = (gridsize * i) % WIDTH;
        scene.grids[i].maxx = scene.grids[i].minx + gridsize;
        scene.grids[i].miny = ((gridsize * i) / WIDTH) * gridsize;
        scene.grids[i].maxy = scene.grids[i].miny + gridsize;
        scene.grids[i].circles = (Lt_Circle *)malloc(sizeof(Lt_Circle) * num_circles);
    }

    for (size_t i = 0; i < num_circles; ++i) {
        float size = minsize + ((rand() / float(RAND_MAX)) * (maxsize - minsize));
        Lt_Circle *circle = (Lt_Circle *)malloc(sizeof(Lt_Circle));
        *circle = {size, Li_Vec3f(samples[i].x * WIDTH, samples[i].y * HEIGHT, 0)};
        int *indices = get_intersected_grid_indices(&scene, circle);

        while (*indices != -1) {
            Lt_Grid *grid = scene.grids + *indices;
            memcpy(grid->circles + grid->num_circles, circle, sizeof(Lt_Circle));
            grid->num_circles += 1;
            ++indices;
        }
    }

    return scene;
}

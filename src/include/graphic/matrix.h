#ifndef __MATRIX_H__
#define __MATRIX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>
#include <math.h>

/*
 * | [a] [b] [x] |
 * | [c] [d] [y] |
 * | [0] [0] [1] |
 */
struct matrix_transform_t {
	double a, b, x;
	double c, d, y;
};

void matrix_transform_init(struct matrix_transform_t * m, double a, double b, double x, double c, double d, double y);
void matrix_transform_init_identity(struct matrix_transform_t * m);
void matrix_transform_init_translate(struct matrix_transform_t * m, double tx, double ty);
void matrix_transform_init_scale(struct matrix_transform_t * m, double sx, double sy);
void matrix_transform_init_rotate(struct matrix_transform_t * m, double c, double s);
void matrix_transform_init_shear(struct matrix_transform_t * m, double x, double y);
void matrix_transform_multiply(struct matrix_transform_t * m, const struct matrix_transform_t * m1, const struct matrix_transform_t * m2);
void matrix_transform_invert(struct matrix_transform_t * m, const struct matrix_transform_t * i);
void matrix_transform_translate(struct matrix_transform_t * m, double tx, double ty);
void matrix_transform_scale(struct matrix_transform_t * m, double sx, double sy);
void matrix_transform_rotate(struct matrix_transform_t * m, double c, double s);
void matrix_transform_shear(struct matrix_transform_t * m, double x, double y);
void matrix_transform_distance(const struct matrix_transform_t * m, double * dx, double * dy);
void matrix_transform_point(const struct matrix_transform_t * m, double * x, double * y);

#ifdef __cplusplus
}
#endif

#endif /* __MATRIX_H__ */

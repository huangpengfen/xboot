/*
 * kernel/graphic/matrix.c
 *
 * Copyright(c) 2007-2018 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <xboot.h>
#include <graphic/matrix.h>

void matrix_transform_init(struct matrix_transform_t * m,
		double a, double b, double x,
		double c, double d, double y)
{
	m->a = a;	m->b = b;	m->x = x;
	m->c = c;	m->d = d;	m->y = y;
}

void matrix_transform_init_identity(struct matrix_transform_t * m)
{
	m->a = 1;	m->b = 0;	m->x = 0;
	m->c = 0;	m->d = 1;	m->y = 0;
}

void matrix_transform_init_translate(struct matrix_transform_t * m,
		double tx, double ty)
{
	m->a = 1;	m->b = 0;	m->x = tx;
	m->c = 0;	m->d = 1;	m->y = ty;
}

void matrix_transform_init_scale(struct matrix_transform_t * m,
		double sx, double sy)
{
	m->a = sx;	m->b = 0;	m->x = 0;
	m->c = 0;	m->d = sy;	m->y = 0;
}

void matrix_transform_init_rotate(struct matrix_transform_t * m,
		double c, double s)
{
	m->a = c;	m->b = -s;	m->x = 0;
	m->c = s;	m->d = c;	m->y = 0;
}

void matrix_transform_init_shear(struct matrix_transform_t * m,
		double x, double y)
{
	m->a = 1;	m->b = x;	m->x = 0;
	m->c = y;	m->d = 1;	m->y = 0;
}

/*
 * | [m->a] [m->b] [m->x] |   | [m1->a] [m1->b] [m1->x] |   | [m2->a] [m2->b] [m2->x] |
 * | [m->c] [m->d] [m->y] | = | [m1->c] [m1->d] [m1->y] | x | [m2->c] [m2->d] [m2->y] |
 * | [0]    [0]    [1]    |   | [0]     [0]     [1]     |   | [0]     [0]     [1]     |
 */
void matrix_transform_multiply(struct matrix_transform_t * m,
		const struct matrix_transform_t * m1,
		const struct matrix_transform_t * m2)
{
	struct matrix_transform_t t;

	t.a = m1->a * m2->a + m1->b * m2->c;
	t.b = m1->a * m2->b + m1->b * m2->d;
	t.x = m1->a * m2->x + m1->b * m2->y + m1->x;

	t.c = m1->c * m2->a + m1->d * m2->c;
	t.d = m1->c * m2->b + m1->d * m2->d;
	t.y = m1->c * m2->x + m1->d * m2->y + m1->y;

	memcpy(m, &t, sizeof(struct matrix_transform_t));
}

void matrix_transform_invert(struct matrix_transform_t * m, const struct matrix_transform_t * i)
{
	struct matrix_transform_t t;
	double det;

	det = i->a * i->d - i->b * i->c;

	if(det == 0.0)
	{
		matrix_transform_init_identity(m);
		return;
	}

	t.a = i->d / det;
	t.b = -i->b / det;
	t.c = -i->c / det;
	t.d = i->a / det;
	t.x = (i->c * i->y - i->d * i->x) / det;
	t.y = (i->b * i->x - i->a * i->y) / det;

	memcpy(m, &t, sizeof(struct matrix_transform_t));
}

void matrix_transform_translate(struct matrix_transform_t * m, double tx, double ty)
{
	struct matrix_transform_t t;

	matrix_transform_init_translate(&t, tx, ty);
	matrix_transform_multiply(m, &t, m);
}

void matrix_transform_scale(struct matrix_transform_t * m, double sx, double sy)
{
	struct matrix_transform_t t;

	matrix_transform_init_scale(&t, sx, sy);
	matrix_transform_multiply(m, &t, m);
}

void matrix_transform_rotate(struct matrix_transform_t * m, double c, double s)
{
	struct matrix_transform_t t;

	matrix_transform_init_rotate(&t, c, s);
	matrix_transform_multiply(m, &t, m);
}

void matrix_transform_shear(struct matrix_transform_t * m, double x, double y)
{
	struct matrix_transform_t t;

	matrix_transform_init_shear(&t, x, y);
	matrix_transform_multiply(m, &t, m);
}

void matrix_transform_distance(const struct matrix_transform_t * m, double * dx, double * dy)
{
	double nx, ny;

	nx = m->a * (*dx) + m->b * (*dy);
	ny = m->c * (*dx) + m->d * (*dy);

	*dx = nx;
	*dy = ny;
}

void matrix_transform_point(const struct matrix_transform_t * m, double * x, double * y)
{
	matrix_transform_distance(m, x, y);

	*x += m->x;
	*y += m->y;
}

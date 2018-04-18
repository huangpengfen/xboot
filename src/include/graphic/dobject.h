#ifndef __GRAPHIC_DOBJECT_H__
#define __GRAPHIC_DOBJECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <xboot.h>
#include <graphic/matrix.h>
#include <graphic/display.h>

enum dobject_type_t {
	DOBJECT_TYPE_CONTAINER,
	DOBJECT_TYPE_NODE,
};

enum alignment_t {
	ALIGN_NONE				= 0,
	ALIGN_LEFT				= 1,
	ALIGN_TOP				= 2,
	ALIGN_RIGHT				= 3,
	ALIGN_BOTTOM			= 4,
	ALIGN_LEFT_TOP			= 5,
	ALIGN_RIGHT_TOP			= 6,
	ALIGN_LEFT_BOTTOM		= 7,
	ALIGN_RIGHT_BOTTOM		= 8,
	ALIGN_LEFT_CENTER		= 9,
	ALIGN_TOP_CENTER		= 10,
	ALIGN_RIGHT_CENTER		= 11,
	ALIGN_BOTTOM_CENTER		= 12,
	ALIGN_HORIZONTAL_CENTER	= 13,
	ALIGN_VERTICAL_CENTER	= 14,
	ALIGN_CENTER			= 15,
	ALIGN_LEFT_FILL			= 16,
	ALIGN_TOP_FILL			= 17,
	ALIGN_RIGHT_FILL		= 18,
	ALIGN_BOTTOM_FILL		= 19,
	ALIGN_HORIZONTAL_FILL	= 20,
	ALIGN_VERTICAL_FILL		= 21,
	ALIGN_CENTER_FILL		= 22,
};

struct dobject_t
{
	enum dobject_type_t type;
	struct dobject_t * parent;
	struct list_head entry;
	struct list_head children;

	double width, height;
	double x, y;
	double rotation;
	double scalex, scaley;
	double anchorx, anchory;
	double alpha;
	enum alignment_t alignment;
	int visible;
	int touchable;
	struct matrix_t m, matrix;

	void (*draw)(struct dobject_t * o, struct display_t * d);
	void (*bbox)(struct dobject_t * o, double * x1, double * y1, double * x2, double * y2);
	void * priv;
};

typedef void (*dobject_draw_t)(struct dobject_t * o, struct display_t * d);
typedef void (*dobject_bbox_t)(struct dobject_t * o, double * x1, double * y1, double * x2, double * y2);

#ifdef __cplusplus
}
#endif

#endif /* __GRAPHIC_DOBJECT_H__ */

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <xboot.h>

enum display_type_t {
	DISPLAY_TYPE_CONTAINER,
	DISPLAY_TYPE_NODE,
};

struct display_t
{
	char * name;
	enum display_type_t type;
	struct display_t * parent;
	struct list_head entry;
	struct list_head children;

	double width, height;
	double x, y;
	double rotation;
	double scalex, scaley;
	double anchorx, anchory;
	double alpha;
//	enum alignment_t alignment;
//	int visible;
//	int touchable;

	ssize_t (*read)(struct display_t * dobj, void * buf, size_t size);
	ssize_t (*write)(struct display_t * dobj, void * buf, size_t size);
	void * priv;
};

#ifdef __cplusplus
}
#endif

#endif /* __DISPLAY_H__ */

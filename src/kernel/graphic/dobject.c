/*
 * kernel/graphic/dobject.c
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
#include <graphic/dobject.h>

static struct dobject_t * __dobject_alloc(enum dobject_type_t type, dobject_draw_t draw, dobject_bbox_t bbox, void * priv)
{
	struct dobject_t * o;

	o = malloc(sizeof(struct dobject_t));
	if(!o)
		return NULL;

	o->type = type;
	o->parent = o;
	init_list_head(&o->entry);
	init_list_head(&o->children);

	o->width = 0;
	o->height = 0;
	o->x = 0;
	o->y = 0;
	o->rotation = 0;
	o->scalex = 1;
	o->scaley = 1;
	o->anchorx = 0;
	o->anchory = 0;
	o->alpha = 1;
	o->alignment = ALIGN_NONE;
	o->visible = 1;
	o->touchable = 1;
	matrix_init_identity(&o->m);
	matrix_init_identity(&o->matrix);

	o->draw = draw;
	o->bbox = bbox;
	o->priv = priv;

	return o;
}

struct dobject_t * dobject_alloc_container(void)
{
	return __dobject_alloc(DOBJECT_TYPE_CONTAINER, NULL, NULL, NULL);
}

struct dobject_t * dobject_alloc_node(dobject_draw_t draw, dobject_bbox_t bbox, void * priv)
{
	return __dobject_alloc(DOBJECT_TYPE_NODE, draw, bbox, priv);
}

bool_t dobject_free(struct dobject_t * o)
{
	if(!o)
		return FALSE;
	free(o);
	return TRUE;
}

bool_t dobject_add(struct dobject_t * parent, struct dobject_t * o)
{
	if(!parent || !o)
		return FALSE;

	if(parent->type != DOBJECT_TYPE_CONTAINER)
		return FALSE;

	o->parent = parent;
	list_add_tail(&o->entry, &parent->children);
	return TRUE;
}

bool_t dobject_remove(struct dobject_t * parent, struct dobject_t * o)
{
	struct dobject_t * pos, * n;

	if(!parent || !o)
		return FALSE;

	if(parent->type != DOBJECT_TYPE_CONTAINER)
		return FALSE;

	list_for_each_entry_safe(pos, n, &(parent->children), entry)
	{
		if(pos == o)
		{
			pos->parent = pos;
			list_del(&(pos->entry));
			return TRUE;
		}
	}
	return FALSE;
}

bool_t dobject_remove_self(struct dobject_t * o)
{
	struct dobject_t * parent;
	struct dobject_t * pos, * n;
	bool_t ret;

	if(!o)
		return FALSE;

	if(o->type == DOBJECT_TYPE_CONTAINER)
	{
		list_for_each_entry_safe(pos, n, &(o->children), entry)
		{
			dobject_remove_self(pos);
		}
	}

	parent = o->parent;
	if(parent && (parent != o))
	{
		ret = dobject_remove(parent, o);
		if(ret)
			dobject_free(o);
		return ret;
	}

	dobject_free(o);
	return TRUE;
}

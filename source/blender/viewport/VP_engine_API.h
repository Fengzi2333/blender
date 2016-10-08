/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file VP_engine_API.h
 *  \ingroup viewport
 */

#ifndef __VP_ENGINE_H__
#define __VP_ENGINE_H__

#include "BLI_compiler_attrs.h"

#include "DNA_listBase.h"

struct bContext;
struct ViewportEngine;

extern ListBase ViewportEngineTypes;

typedef struct ViewportEngineType {
	struct ViewportEngineType *next, *prev;

	char idname[64];
	char name[64]; /* MAX_NAME */

	void (*draw)(const struct bContext *C);
} ViewportEngineType;

/* Engine Types */
void VP_enginetypes_init(void);
void VP_enginetypes_exit(void);

/* Engines */
struct ViewportEngine *VP_engine_create(ViewportEngineType *engine_type) ATTR_NONNULL();
void VP_engine_free(struct ViewportEngine *engine) ATTR_NONNULL();

void VP_engine_render(const struct ViewportEngine *engine, const struct bContext *C);

#endif /* __VP_ENGINE_H__ */

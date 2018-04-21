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

/** \file blender/python/intern/bpy_app_icons.c
 *  \ingroup pythonintern
 *
 * Runtime defined icons.
 */

#include <Python.h>

#include "MEM_guardedalloc.h"

#include "BLI_utildefines.h"

#include "BKE_icons.h"

#include "../generic/py_capi_utils.h"

#include "bpy_app_icons.h"

/* We may want to load direct from file. */
PyDoc_STRVAR(bpy_app_icons_new_triangles_doc,
".. function:: new_triangles(coords, colors)"
"\n"
"   Create a new icon from triangle geometry.\n"
"\n"
"   :arg coords: Sequence of bytes (6 floats for one triangle) for (X, Y) coordinates.\n"
"   :type coords: byte sequence\n"
"   :arg colors: Sequence of ints (12 for one triangles) for RGBA.\n"
"   :type colors: byte sequence\n"
"   :return: Unique icon value (pass to interface ``icon_value`` argument).\n"
"   :rtype: int\n"
);
static PyObject *bpy_app_icons_new_triangles(PyObject *UNUSED(self), PyObject *args)
{
	/* bytes */
	PyObject *py_coords, *py_colors;
	if (!PyArg_ParseTuple(args, "SS:new_triangles", &py_coords, &py_colors)) {
		return NULL;
	}

	const int coords_len = PyBytes_GET_SIZE(py_coords);
	const int tris_len = coords_len / 6;
	if (tris_len * 6 != coords_len) {
		PyErr_SetString(PyExc_ValueError, "coords must be multiple of 6");
		return NULL;
	}
	if (PyBytes_GET_SIZE(py_colors) != 2 * coords_len) {
		PyErr_SetString(PyExc_ValueError, "colors must be twice size of coords");
		return NULL;
	}

	int coords_size = sizeof(uchar[2]) * tris_len * 3;
	int colors_size = sizeof(uchar[4]) * tris_len * 3;
	uchar (*coords)[2] = MEM_mallocN(coords_size, __func__);
	uchar (*colors)[4] = MEM_mallocN(colors_size, __func__);

	memcpy(coords, PyBytes_AS_STRING(py_coords), coords_size);
	memcpy(colors, PyBytes_AS_STRING(py_colors), colors_size);

	struct Icon_Geom *geom = MEM_mallocN(sizeof(*geom), __func__);
	geom->coords_len = tris_len;
	geom->coords = coords;
	geom->colors = colors;
	geom->icon_id = 0;
	int icon_id = BKE_icon_geom_ensure(geom);
	return PyLong_FromLong(icon_id);
}

PyDoc_STRVAR(bpy_app_icons_release_doc,
".. function:: release(icon_id)"
"\n"
"   Release the icon.\n"
);
static PyObject *bpy_app_icons_release(PyObject *UNUSED(self), PyObject *args)
{
	int icon_id;
	static _PyArg_Parser _parser = {"i:release", NULL, 0};
	if (!_PyArg_ParseTupleAndKeywordsFast(
	        args, NULL, &_parser,
	        &icon_id))
	{
		return NULL;
	}

	if (!BKE_icon_delete_unmanaged(icon_id)) {
		PyErr_SetString(PyExc_ValueError, "invalid icon_id");
		return NULL;
	}
	Py_RETURN_NONE;
}

static struct PyMethodDef M_AppIcons_methods[] = {
	{"new_triangles", (PyCFunction)bpy_app_icons_new_triangles, METH_VARARGS, bpy_app_icons_new_triangles_doc},
	{"release", (PyCFunction)bpy_app_icons_release, METH_VARARGS, bpy_app_icons_release_doc},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef M_AppIcons_module_def = {
	PyModuleDef_HEAD_INIT,
	"bpy.app.icons",  /* m_name */
	NULL,  /* m_doc */
	0,     /* m_size */
	M_AppIcons_methods,  /* m_methods */
	NULL,  /* m_reload */
	NULL,  /* m_traverse */
	NULL,  /* m_clear */
	NULL,  /* m_free */
};

PyObject *BPY_app_icons_module(void)
{
	PyObject *mod = PyModule_Create(&M_AppIcons_module_def);

	return mod;
}

#include <cmath>
#include <Python.h>
#include "structmember.h"

#include "Utils.h"
#include "Layers.h"
#include "Renderable.h"

// https://docs.python.org/3.6/extending/newtypes.html
struct PyRenderable
{
	PyObject_HEAD
};

class PyRenderableWrapper: public Renderable
{
	PyObject* wrapped;
public:
	PyRenderableWrapper(PyObject* renderable)
	{
		this->wrapped = renderable;
	}

	virtual bool Update()
	{
		PyObject* update = PyObject_CallMethod(this->wrapped, "Update", "");
		
		return PyObject_IsTrue(update);
	}
	
	virtual void Render()
	{
		PyObject_CallMethod(this->wrapped, "Render", "");
	}
};

static void PyRenderable_dealloc(PyRenderable* self)
{
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyRenderable_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PyRenderable* self;

	self = (PyRenderable*)type->tp_alloc(type, 0);

	return (PyObject*)self;
}

static int PyRenderable_init(PyRenderable *self, PyObject *args, PyObject *kwds)
{
	return 0;
}

// Set Members
static PyMemberDef PyRenderable_members[] = {
	{ NULL }  /* Sentinel */
};

// Class Methods
static PyObject* PyRenderable_Update(PyRenderable* self)
{
	Py_RETURN_TRUE;
}

static PyObject* PyRenderable_Render(PyRenderable* self)
{
	Py_RETURN_NONE;
}

// Declare Class Methods
static PyMethodDef PyRenderable_methods[] = {
	{ "Update", (PyCFunction)PyRenderable_Update, METH_NOARGS, "" },
	{ "Render", (PyCFunction)PyRenderable_Render, METH_NOARGS, "" },
	{ NULL }  /* Sentinel */
};

// Set Type Info
static PyTypeObject PyRenderableType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"PyRenderable.PyRenderable",        /* tp_name */
	sizeof(PyRenderable),		        /* tp_basicsize */
	0,						            /* tp_itemsize */
	(destructor)PyRenderable_dealloc, /* tp_dealloc */
	0,						 /* tp_print */
	0,						 /* tp_getattr */
	0,						 /* tp_setattr */
	0,						 /* tp_reserved */
	0,						 /* tp_repr */
	0,						 /* tp_as_number */
	0,						 /* tp_as_sequence */
	0,						 /* tp_as_mapping */
	0,						 /* tp_hash  */
	0,						 /* tp_call */
	0,						 /* tp_str */
	0,						 /* tp_getattro */
	0,						 /* tp_setattro */
	0,						 /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT |
		Py_TPFLAGS_BASETYPE, /* tp_flags */
	"PyRenderable objects",	 /* tp_doc */
	0,						 /* tp_traverse */
	0,						 /* tp_clear */
	0,						 /* tp_richcompare */
	0,						 /* tp_weaklistoffset */
	0,						 /* tp_iter */
	0,						 /* tp_iternext */
	PyRenderable_methods,	 /* tp_methods */
	PyRenderable_members,    /* tp_members */
	0,						 /* tp_getset */
	0,						 /* tp_base */
	0,						 /* tp_dict */
	0,						 /* tp_descr_get */
	0,						 /* tp_descr_set */
	0,						 /* tp_dictoffset */
	(initproc)PyRenderable_init,	 /* tp_init */
	0,						 /* tp_alloc */
	PyRenderable_new,				 /* tp_new */
};

// Module Functions
static PyObject* PyRenderable_AddRenderable(PyObject *self, PyObject *args)
{
	int layer = Layers::LEVEL;
	
	PyObject* pyRenderable = NULL;
	
	if(PyArg_ParseTuple(args, "O|I", &pyRenderable, &layer))
	{
		// Managed by layers
		Renderable* renderable = new PyRenderableWrapper(pyRenderable);
		
		if (isBounded(layer, Layers::BACKGROUND, Layers::NUM_LAYERS))
		{
			Layers::AddToLayer(renderable, layer);
		}
	}
	
	Py_RETURN_NONE;
}

// Declare Module Functions
static PyMethodDef ModuleFuncs[] = {
	{"AddRenderable", PyRenderable_AddRenderable, METH_VARARGS, ""},
	{NULL}  /* Sentinel */
};

// Create Module
static PyModuleDef PyRenderablemodule = {
	PyModuleDef_HEAD_INIT, "", "Example module that creates an extension type.", -1, ModuleFuncs, NULL, NULL, NULL, NULL
};

// Module Init Func
PyMODINIT_FUNC PyInit_PyRenderable()
{
	PyObject* m;
	
	PyObject* layers = PyDict_New();
	
	PyDict_SetItemString(layers, "BACKGROUND", PyLong_FromLong(Layers::BACKGROUND));
	PyDict_SetItemString(layers, "BACKGROUND_2", PyLong_FromLong(Layers::BACKGROUND_2));
	PyDict_SetItemString(layers, "LEVEL", PyLong_FromLong(Layers::LEVEL));
	PyDict_SetItemString(layers, "ABOVE", PyLong_FromLong(Layers::ABOVE));
	PyDict_SetItemString(layers, "WEATHER", PyLong_FromLong(Layers::WEATHER));
	PyDict_SetItemString(layers, "HUD", PyLong_FromLong(Layers::HUD));
	PyDict_SetItemString(layers, "NUM_LAYERS", PyLong_FromLong(Layers::NUM_LAYERS));

	if (PyType_Ready(&PyRenderableType) < 0)
	{
		return NULL;
	}

	m = PyModule_Create(&PyRenderablemodule);
	if (m == NULL)
	{
		return NULL;
	}

	Py_INCREF(&PyRenderableType);
	PyModule_AddObject(m, "Renderable", (PyObject*)&PyRenderableType);
	PyModule_AddObject(m, "Layers", layers);
	return m;
}


void InitModules()
{
	PyImport_AppendInittab("GameEngine", &PyInit_PyRenderable);
}
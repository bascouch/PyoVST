//
//  m_pyo_redirect.h
//  PyoJUCE
//
//  Created by charles on 19/12/2019.
//

#ifndef m_pyo_redirect_h
#define m_pyo_redirect_h

#include <Python.h>
//#include "PluginProcessor.h"

extern int getInterpreterOutput(const char * s);

extern "C" {
    
/* STDOUT STDERR readback function prototype */
//typedef const int stdfun(const char*);

static PyObject*
stdredirect_write(PyObject *self, PyObject *args)
{
    const char *what;
    if(!PyArg_ParseTuple(args, "s", &what))
        return NULL;
    
    getInterpreterOutput(what);
    return Py_BuildValue("s",what);
}

static PyMethodDef stdredirectMethods[] = {
    {"write", stdredirect_write, METH_VARARGS,
        "doc for write stdout to C"},
    {NULL, NULL, 0, NULL}
};
    
    
}

#endif /* m_pyo_redirect_h */

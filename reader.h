#ifndef READER
#define READER

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <fstream>
#include <iostream>

using namespace std;

bool RunPythonScript()
{
    Py_Initialize();

    PyObject* pValue;
    PyObject* pName = PyUnicode_DecodeFSDefault("reader");
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL)
    {
        PyObject* pFunc = PyObject_GetAttrString(pModule, "read");

        if (pFunc && PyCallable_Check(pFunc))
        {
            pValue = PyObject_CallObject(pFunc, NULL);

            if (pValue == NULL || PyLong_AsLong(pValue) == 0)
            {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr, "Call failed\n");
                return false;
            }
            Py_DECREF(pValue);
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"read\"\n");
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"reader.py\"\n");
        return false;
    }
    if (Py_FinalizeEx() < 0) {
        return false;
    }
    return true;
}

void Read(int sudoku[81])
{
    if (RunPythonScript())
    {
        ifstream is("data.txt");
        if (!is)
        {
            cerr << "Failed to load data" << endl;
            exit(1);
        }
        for (int i = 0; i < 81; i++)
            is >> sudoku[i];
        system("cls");
    }
    else
    {
        cerr << "Failed to run python script" << endl;
        exit(2);
    }
}
#endif // !READER
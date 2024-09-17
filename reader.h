#ifndef READER
#define READER

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

bool RunPythonScript(string filename)
{
    Py_Initialize();

    PyObject* pValue;
    PyObject* pName = PyUnicode_DecodeFSDefault("reader");
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL)
    {
        PyObject* pFunc = PyObject_GetAttrString(pModule, "read");
        PyObject* pArg = PyUnicode_DecodeFSDefault(filename.c_str());

        if (pFunc && PyCallable_Check(pFunc))
        {
            //pValue = PyObject_CallObject(pFunc, "test");
            pValue = _PyObject_CallMethod(pModule, PyUnicode_DecodeFSDefault("read"), "s", filename.c_str());

            if (pValue == NULL || PyLong_AsLong(pValue) == 0)
            {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr, "Call failed\n");
                return false;
            }
            Py_DECREF(pValue);
            Py_DECREF(pArg);
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
    string filename = "";
    while (true)
    {
        cout << "Enter Sudoku image file name: ";
        getline(cin, filename);
        if (fs::exists("images/" + filename))
            break;
        cout << "File \"" << filename << "\" doesn't exit in images directory." << endl;
    }
    cout << "Loading..." << endl;

    if (RunPythonScript(filename))
    {
        ifstream is("data.txt");
        if (!is)
        {
            cerr << "Failed to load data" << endl;
            return;
            exit(1);
        }
        for (int i = 0; i < 81; i++)
            is >> sudoku[i];
        system("cls");
    }
    else
    {
        cerr << "Failed to run python script" << endl;
        return;
        exit(2);
    }
}
#endif // !READER
#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include <chrono>
#include <string>
#include <vector>

#include "preprocess.h"
#include "msa.h"
#include "postprocess.h"
#include "align.h"

std::vector<std::string> string_list_to_vector(PyObject *py_list) {
    /*
     * Parse python list of strings to c++ vector of strings
     */
    long long size = PyList_Size(py_list);
    std::vector<std::string> string_vector;
    for (int i = 0; i < size; ++i) {
        PyObject *py_string = PyList_GetItem(py_list, i);
        const char *token = PyUnicode_AsUTF8(py_string);
        string_vector.emplace_back(token);
    }
    return string_vector;
}

PyObject *string_vector_to_list(const std::vector<std::string> &string_vector) {
    /*
     * Parse c++ vector of strings to python list of strings
     */
    PyObject *py_list = PyList_New(string_vector.size());
    if (!py_list) {
        return NULL;
    }
    for (int i = 0; i < string_vector.size(); ++i) {
        PyObject *py_string = PyUnicode_FromString(string_vector[i].c_str());
        if (!py_string) {
            Py_DECREF(py_list);
            return NULL;
        }
        if (PyList_SetItem(py_list, i, py_string) != 0) {
            Py_DECREF(py_string);
            Py_DECREF(py_list);
            return NULL;
        }
    }
    return py_list;
}

PyObject *int_vector_to_list(const std::vector<int>& int_vector) {
    /*
     * Parse c++ vector of ints to python list of ints
     */
    PyObject *py_list = PyList_New(int_vector.size());
    if (!py_list) {
        return NULL;
    }
    for (int i = 0; i < int_vector.size(); ++i) {
        PyObject *py_int = PyLong_FromLong(int_vector[i]);
        if (!py_int) {
            Py_DECREF(py_int);
            return NULL;
        }
        if (PyList_SetItem(py_list, i, py_int) != 0) {
            Py_DECREF(py_int);
            Py_DECREF(py_list);
            return NULL;
        }
    }
    return py_list;
}

std::vector<std::vector<std::string>> nested_str_list_to_vector(PyObject *py_list) {
    std::vector<std::vector<std::string>> result;
    long long size = PyList_Size(py_list);
    for (int i = 0; i < size; ++i) {
        PyObject *string_list = PyList_GetItem(py_list, i);
        std::vector<std::string> string_vector = string_list_to_vector(string_list);
        result.emplace_back(string_vector);
    }
    return result;
}

PyObject *nested_str_vector_to_list(const std::vector<std::vector<std::string>> &sequences) {
    PyObject *py_list = PyList_New(sequences.size());
    if (!py_list) {
        return NULL;
    }
    for (int i = 0; i < sequences.size(); ++i) {
        PyObject *py_string_list = string_vector_to_list(sequences[i]);
        if (!py_string_list) {
            Py_DECREF(py_list);
            return NULL;
        }
        if (PyList_SetItem(py_list, i, py_string_list) != 0) {
            Py_DECREF(py_list);
            return NULL;
        }
    }
    return py_list;
}

PyObject *nested_int_vector_to_list(const std::vector<std::vector<int>> &align_indices) {
    PyObject *py_list = PyList_New(align_indices.size());
    if (!py_list) {
        return NULL;
    }
    for (int i = 0; i < align_indices.size(); ++i) {
        PyObject *py_int_list = int_vector_to_list(align_indices[i]);
        if (!py_int_list) {
            Py_DECREF(py_list);
            return NULL;
        }
        if (PyList_SetItem(py_list, i, py_int_list) != 0) {
            Py_DECREF(py_list);
            return NULL;
        }
    }
    return py_list;
}

static PyObject *align_without_segment(PyObject *self, PyObject *args) {
    PyObject *hypothesis_list;
    PyObject *reference_list;
    PyObject *reference_label_list;
    int partial_bound = 2;

    if (!PyArg_ParseTuple(args, "O!O!O!|i", &PyList_Type, &hypothesis_list, &PyList_Type, &reference_list, &PyList_Type, &reference_label_list, &partial_bound)) {
        return NULL;
    }

    std::vector<std::string> hypothesis = string_list_to_vector(hypothesis_list);
    std::vector<std::string> reference = string_list_to_vector(reference_list);
    std::vector<std::string> reference_label = string_list_to_vector(reference_label_list);

    std::vector<std::vector<std::string>> align_result = align_without_segment(hypothesis, reference, reference_label, partial_bound);
    PyObject *py_align_result = nested_str_vector_to_list(align_result);
    return Py_BuildValue("O", py_align_result);
}

static PyObject *align_with_auto_segment(PyObject *self, PyObject *args) {
    PyObject *hypothesis_list;
    PyObject *reference_list;
    PyObject *reference_label_list;
    int partial_bound = 2;

    if (!PyArg_ParseTuple(args, "O!O!O!|i", &PyList_Type, &hypothesis_list, &PyList_Type, &reference_list, &PyList_Type, &reference_label_list, &partial_bound)) {
        return NULL;
    }

    std::vector<std::string> hypothesis = string_list_to_vector(hypothesis_list);
    std::vector<std::string> reference = string_list_to_vector(reference_list);
    std::vector<std::string> reference_label = string_list_to_vector(reference_label_list);

    std::vector<std::vector<std::string>> align_result = align_with_auto_segment(hypothesis, reference, reference_label, partial_bound);
    PyObject *py_align_result = nested_str_vector_to_list(align_result);
    return Py_BuildValue("O", py_align_result);
}

static PyObject *align_with_manual_segment(PyObject *self, PyObject *args) {
    PyObject *hypothesis_list;
    PyObject *reference_list;
    PyObject *reference_label_list;
    int segment_length = 0;
    int barrier_length = 0;
    int partial_bound = 2;

    if (!PyArg_ParseTuple(args, "O!O!O!ii|i", &PyList_Type, &hypothesis_list, &PyList_Type, &reference_list, &PyList_Type, &reference_label_list, &segment_length, &barrier_length, &partial_bound)) {
        return NULL;
    }

    std::vector<std::string> hypothesis = string_list_to_vector(hypothesis_list);
    std::vector<std::string> reference = string_list_to_vector(reference_list);
    std::vector<std::string> reference_label = string_list_to_vector(reference_label_list);

    std::vector<std::vector<std::string>> align_result = align_with_manual_segment(hypothesis, reference, reference_label, segment_length, barrier_length, partial_bound);
    PyObject *py_align_result = nested_str_vector_to_list(align_result);
    return Py_BuildValue("O", py_align_result);
}

static PyObject *get_token_match_result(PyObject *self, PyObject *args) {
    PyObject *py_align_result;
    int partial_bound = 2;

    if (!PyArg_ParseTuple(args, "O!|i", &PyList_Type, &py_align_result, &partial_bound)) {
        return NULL;
    }

    std::vector<std::vector<std::string>> align_result = nested_str_list_to_vector(py_align_result);
    std::vector<std::string> token_match_result = get_token_match_result(align_result, partial_bound);
    PyObject *py_token_match_result = string_vector_to_list(token_match_result);
    return Py_BuildValue("O", py_token_match_result);
}

static PyObject *get_align_indices(PyObject *self, PyObject *args) {
    PyObject *py_align_result;

    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &py_align_result)) {
        return NULL;
    }

    std::vector<std::vector<std::string>> align_result = nested_str_list_to_vector(py_align_result);
    std::vector<std::vector<int>> align_indices = get_align_indices(align_result);
    PyObject *py_align_indices = nested_int_vector_to_list(align_indices);
    return Py_BuildValue("O", py_align_indices);
}

static PyObject *get_ref_original_indices(PyObject *self, PyObject *args) {
    PyObject *py_reference_list;
    PyObject *py_speaker_label_list;

    if (!PyArg_ParseTuple(args, "O!O!", &PyList_Type, &py_reference_list, &PyList_Type, &py_speaker_label_list)) {
        return NULL;
    }

    std::vector<std::string> reference = string_list_to_vector(py_reference_list);
    std::vector<std::string> speaker_label = string_list_to_vector(py_speaker_label_list);
    std::vector<std::vector<int>> ref_original_indices = get_ref_original_indices(reference, speaker_label);
    PyObject *py_ref_original_indices_list = nested_int_vector_to_list(ref_original_indices);
    return Py_BuildValue("O", py_ref_original_indices_list);
}

static PyObject *get_unique_speaker_label(PyObject *self, PyObject *args) {
    PyObject *py_speaker_label_list;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &py_speaker_label_list)) {
        return NULL;
    }
    std::vector<std::string> speaker_label = string_list_to_vector(py_speaker_label_list);
    std::vector<std::string> unique_speaker_label = get_unique_speaker_label(speaker_label);
    PyObject *py_unique_speaker_label_list = string_vector_to_list(unique_speaker_label);
    return Py_BuildValue("O", py_unique_speaker_label_list);
}

static PyObject *get_aligned_hypo_speaker_label(PyObject *self, PyObject *args) {
    PyObject *py_align_result;
    PyObject *py_hypo_speaker_label_list;
    if (!PyArg_ParseTuple(args, "O!O!", &PyList_Type, &py_align_result, &PyList_Type, &py_hypo_speaker_label_list)) {
        return NULL;
    }
    std::vector<std::vector<std::string>> align_result = nested_str_list_to_vector(py_align_result);
    std::vector<std::string> hypo_speaker_label = string_list_to_vector(py_hypo_speaker_label_list);
    std::vector<std::string> aligned_hypo_speaker_label = get_aligned_hypo_speaker_label(align_result, hypo_speaker_label);
    PyObject *py_aligned_hypo_speaker_label_list = string_vector_to_list(aligned_hypo_speaker_label);
    return Py_BuildValue("O", py_aligned_hypo_speaker_label_list);
}

static PyMethodDef align4d_funcs[] = {
        {"align_without_segment",     align_without_segment,     METH_VARARGS, "multi-sequence alignment without segmentation."},
        {"align_with_auto_segment",   align_with_auto_segment,   METH_VARARGS, "multi-sequence alignment with automatic segmentation."},
        {"align_with_manual_segment", align_with_manual_segment, METH_VARARGS, "multi-sequence alignment with manual segmentation."},
        {"get_token_match_result",    get_token_match_result,    METH_VARARGS, "get token match result from alignment result."},
        {"get_align_indices",         get_align_indices,         METH_VARARGS, "get indices map from separated references to hypothesis."},
        {"get_ref_original_indices",  get_ref_original_indices,  METH_VARARGS, "get indices map from separated references to original combined reference."},
        {"get_unique_speaker_label", get_unique_speaker_label, METH_VARARGS, "get unique speaker label from total speaker labels."},
        {"get_aligned_hypo_speaker_label", get_aligned_hypo_speaker_label, METH_VARARGS, "get hypothesis speaker labels after alignment (with gap)."},
        {NULL, NULL, 0, NULL}
};

static PyModuleDef align4d = {
        PyModuleDef_HEAD_INIT,
        "align4d",
        NULL,
        -1,
        align4d_funcs
};

PyMODINIT_FUNC
PyInit_align4d(void) {
    return PyModule_Create(&align4d);
}
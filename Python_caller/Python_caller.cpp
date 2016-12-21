// Python_caller.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Python.h>

int main()
{
	//python���ü���ֵ��C��ӳ�� python - >
	// s ->char*
	// i -> int
	// u -> wchar_t* //����δȷ��
	// b/B -> unsigned char
	// h -> short int
	// H -> unsigned short int
	// l -> long
	// k -> unsigned long
	// c -> char
	// f -> float
	// d -> double
	// o -> python object
	// w -> char* �ɶ���д


	Py_Initialize();
	PyRun_SimpleString("import sys");//��ͨ����python
	PyRun_SimpleString("sys.path.append('./')");//��λ��Ŀ¼

	PyObject *pFile, *pRes, *pFunc;
	pFile = PyImport_ImportModule("hihiPython");//��python�ļ�, ��ס��� .py
	pFunc = PyObject_GetAttrString(pFile, "func");//ȡ��������

	PyObject *pArg1, *pArg2, *pArg3;
	pArg1 = Py_BuildValue("i", 99);
	pArg2 = Py_BuildValue("i", 100);
	pArg3 = Py_BuildValue("s", "HiHiChina");
	PyObject* pParamTuple = PyTuple_New(3);//������tuple��ʽ���
	PyTuple_SetItem(pParamTuple, 0, pArg1);
	PyTuple_SetItem(pParamTuple, 1, pArg2);
	PyTuple_SetItem(pParamTuple, 2, pArg3);
	pRes = PyEval_CallObject(pFunc, pParamTuple);//���ú���
	//���pRes��һ�� python_tuple, [99, "Hello"];
	int nRes = 0;
	char* pHello = 0;
	PyArg_ParseTuple(pRes, "is", &nRes, &pHello);//ȡֵ, ��ߵ� is ����¥�ϵ�ӳ���ϵ

	//������һ�ֵ��÷�ʽ
	pRes = PyObject_CallMethod(pFile, "func", "(ssi)", "99", "100", 300);//��ߵ�  ssi����¥�ϵ�ӳ���ϵ

	pRes = PyObject_CallMethod(pFile, "GetDict", "");
	//��ߵ�pRes��һ��map, {1:'A', 2:'B', 3:'C'}
	if (PyDict_Check(pRes) == true) {
		printf("Is Dict");
		int nSize = PyDict_Size(pRes);//size
		PyObject* pKeys = PyDict_Items(pRes);//��ȡkeys, �Ǹ� python list [1, 2, 3]
		int nListSize = PyList_Size(pKeys);//list size
		for (int iList = 0; iList < nListSize; iList++) {
			int nNumber;//key
			PyObject* pTuple = PyList_GetItem(pKeys, iList);
			char* psz;//value
			PyArg_ParseTuple(pTuple, "is", &nNumber, &psz);//��ȡ key, value
		}
	}

	Py_Finalize();

    return 0;
}


# distutils: language = c++
from libcpp.vector cimport vector
from libcpp.string cimport string

cdef extern from "../src/NeuroNet.hpp":
    cdef cppclass GrafoCSR:
        GrafoCSR()
        void cargarDatos(string)
        vector[int] bfs(int, int)
        int obtenerNodoCritico()
        int getN()
        int getM()

cdef class Motor:
    cdef GrafoCSR* g
    def __cinit__(self): self.g = new GrafoCSR()
    def __dealloc__(self): del self.g
    def cargar(self, f): self.g.cargarDatos(f.encode('utf-8'))
    def critico(self): return self.g.obtenerNodoCritico()
    def stats(self): return (self.g.getN(), self.g.getM())
    def simular(self, s, d): return self.g.bfs(s, d)

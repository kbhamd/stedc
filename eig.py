import cupy as cp
import time
from contextlib import contextmanager
import numpy as np

from cupy.linalg._eigenvalue import _syevd
 
@contextmanager
def timing():
    start = time.time()
    yield
    end = time.time()
    print(end-start,'s')

n=4096
A = cp.random.rand(n,n,dtype=np.float32)
cp.cuda.runtime.deviceSynchronize()
B = A.get()
 
print('cupy ', end='')
with timing():
    #_syevd(A, 'U', True)
    La,V=cp.linalg.eigh(A)
    cp.cuda.runtime.deviceSynchronize()
    pass
 
print('numpy ', end='')
with timing():
    Lb,V=np.linalg.eigh(B)

print(La[:4])
print(Lb[:4])

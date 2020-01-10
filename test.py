#!/bin/python3
import argparse
import time

from numpy import allclose
from scipy.io import mmread
from scipy.sparse import tril
from scipy.sparse.linalg import spsolve_triangular


def response_time(function, output_prefix=''):
    TIMES = 1
    start_time = time.time()
    for _ in range(TIMES):
        function()
    end_time = time.time()
    print(f'{output_prefix} response time: {(end_time - start_time) / TIMES}')

def run(base_address):
    l = mmread(base_address + "/af_0_k101.mtx")
    l = tril(l).tocsr()

    b_dense = mmread(base_address + "/b_dense_af_0_k101.mtx")
    def call_x_dense():
        global x_dense
        x_dense = spsolve_triangular(l, b_dense)

    response_time(call_x_dense, 'dense')

    me_x_dense_naive = mmread(base_address + "/x_dense_naive.mtx")
    me_x_dense_parallel1 = mmread(base_address + "/x_dense_parallel1.mtx")

    print(f'allclose(me_x_dense_naive, x_dense)? {allclose(x_dense, me_x_dense_naive)}')
    print(f'allclose(me_x_dense_parallel1, x_dense)? {allclose(x_dense, me_x_dense_parallel1)}')

    b_sparse = mmread(base_address + "/b_sparse_af_0_k101.mtx").toarray()
    def call_x_sparse():
        global x_sparse
        x_sparse = spsolve_triangular(l, b_sparse)

    response_time(call_x_sparse, 'spaese')

    me_x_dense_naive = mmread(base_address + "/x_dense_naive.mtx")
    me_x_sparse_parallel1 = mmread(base_address + "/x_sparse_parallel1.mtx")
    me_x_sparse_parallel2 = mmread(base_address + "/x_sparse_parallel2.mtx")

    print(f'allclose(me_x_dense_naive, x_dense)? {allclose(x_dense, me_x_dense_naive)}')
    print(f'allclose(me_x_sparse_parallel1, x_dense)? {allclose(x_sparse, me_x_sparse_parallel1)}')
    print(f'allclose(me_x_sparse_parallel12 x_dense)? {allclose(x_sparse, me_x_sparse_parallel2)}')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--base-address", dest='base_address')
    args = parser.parse_args()
    run(args.base_address)

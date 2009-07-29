/*
Iterative CUDA is licensed to you under the MIT/X Consortium license:

Copyright (c) 2009 Andreas Kloeckner.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the Software), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/




#ifndef _AAFADFJ_ITERATIVE_CUDA_HPP_SEEN
#define _AAFADFJ_ITERATIVE_CUDA_HPP_SEEN




#include <utility>
#include <cstdint>




namespace iterative_cuda
{
  template <typename ValueType, typename IndexType>
  class gpu_vector
  {
    public:
      typedef IndexType index_type;
      typedef ValueType value_type;

    private:
      std::auto_ptr<gpu_vector_pimpl<value_type, index_type> > pimpl;

    public:
      gpu_vector(index_type size);
      gpu_vector(gpu_vector const &src);
      ~gpu_vector();

      index_type size() const;

      void from_cpu(value_type *cpu);
      void to_cpu(value_type *cpu);

      value_type *ptr();
  };




  template <typename ValueType, typename IndexType>
  class gpu_sparse_pkt_matrix
  {
    public:
      typedef IndexType index_type;
      typedef ValueType value_type;

    private:
      std::auto_ptr<
        gpu_sparse_pkt_matrix_pimpl<value_type, index_type>
        > pimpl;

    public:
      gpu_sparse_pkt_matrix(
          index_type row_count,
          index_type column_count,
          const index_type *csr_row_pointers,
          const index_type *csr_column_indices,
          const value_type *csr_nonzeros);
      ~gpu_sparse_pkt_matrix();

      index_type row_count() const;
      index_type column_count() const;

      void permute(
          gpu_vector const &dest,
          gpu_vector const &src) const;
      void unpermute(
          gpu_vector const &dest,
          gpu_vector const &src) const;

      void operator()(
          gpu_vector const &dest,
          gpu_vector const &src) const;
  };




  template <class GpuVector>
  class diagonal_preconditioner
  {
    public:
      typedef GpuVector gpu_vector;
      typedef typename GpuVector gpu_vector::index_type index_type;
      typedef typename GpuVector gpu_vector::value_type value_type;

    private:
      std::auto_ptr<diagonal_preconditioner_pimpl> m_pimpl;

    public:
      diagonal_preconditioner(gpu_vector const &vec);

      void operator()(gpu_vector const &op, gpu_vector& result);
  };




  template <typename ValueType, typename IndexType, typename Operator, typename Preconditioner>
  void run_cg(
      const Operator &a,
      const Preconditioner &m_inv,
      gpu_vector<ValueType, IndexType> const &x,
      gpu_vector<ValueType, IndexType> const &b,
      ValueType tol = 1e-8);
}




#endif
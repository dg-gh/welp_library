// welp_matrix.hpp - last update : 25 / 03 / 2021
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_MATRIX_HPP
#define WELP_MATRIX_HPP


////////////////////////   I N C L U D E S   ////////////////////////

#include <cstdlib>
#include <cstring>
#include <cmath>


#ifdef WELP_MATRIX_AVX_EXT
#include <immintrin.h>
#endif // WELP_MATRIX_AVX_EXT


// include all in one line with #define WELP_MATRIX_INCLUDE_ALL
#if defined(WELP_MATRIX_INCLUDE_ALL) || defined(WELP_ALWAYS_INCLUDE_ALL)

#ifndef WELP_MATRIX_INCLUDE_FUNCTIONAL
#define WELP_MATRIX_INCLUDE_FUNCTIONAL
#endif
#ifndef WELP_MATRIX_INCLUDE_INITLIST
#define WELP_MATRIX_INCLUDE_INITLIST
#endif
#ifndef WELP_MATRIX_INCLUDE_UTILITY
#define WELP_MATRIX_INCLUDE_UTILITY
#endif
#ifndef WELP_MATRIX_INCLUDE_IOSTREAM
#define WELP_MATRIX_INCLUDE_IOSTREAM
#endif
#ifndef WELP_MATRIX_INCLUDE_STRING
#define WELP_MATRIX_INCLUDE_STRING
#endif
#ifndef WELP_MATRIX_INCLUDE_FSTREAM
#define WELP_MATRIX_INCLUDE_FSTREAM
#endif
#ifndef WELP_MATRIX_INCLUDE_ALGORITHM
#define WELP_MATRIX_INCLUDE_ALGORITHM
#endif

#endif // WELP_MATRIX_INCLUDE_ALL


#if defined(WELP_ALWAYS_DEBUG_MODE) && !defined(WELP_MATRIX_DEBUG_MODE)
#define WELP_MATRIX_DEBUG_MODE
#endif // WELP_ALWAYS_DEBUG_MODE

#ifdef WELP_MATRIX_DEBUG_MODE
#include <cassert>
#ifndef WELP_MATRIX_INCLUDE_IOSTREAM
#define WELP_MATRIX_INCLUDE_IOSTREAM
#endif // WELP_MATRIX_INCLUDE_IOSTREAM
#endif // WELP_MATRIX_DEBUG_MODE

#ifdef WELP_MATRIX_INCLUDE_FUNCTIONAL
#include <functional>
#endif // WELP_MATRIX_INCLUDE_FUNCTIONAL

#ifdef WELP_MATRIX_INCLUDE_INITLIST
#include <initializer_list>
#endif // WELP_MATRIX_INCLUDE_INITLIST

#ifdef WELP_MATRIX_INCLUDE_UTILITY
#include <utility>
#endif // WELP_MATRIX_INCLUDE_UTILITY

#ifdef WELP_MATRIX_INCLUDE_IOSTREAM
#include <iostream>
#endif // WELP_MATRIX_INCLUDE_IOSTREAM

#ifdef WELP_MATRIX_INCLUDE_STRING
#include <string>
#endif // WELP_MATRIX_INCLUDE_STRING

#ifdef WELP_MATRIX_INCLUDE_FSTREAM
#include <fstream>
#endif // WELP_MATRIX_INCLUDE_FSTREAM

#ifdef WELP_MATRIX_INCLUDE_ALGORITHM
#include <algorithm>
#endif // WELP_MATRIX_INCLUDE_ALGORITHM

////////////////////////   O P T I O N S   ////////////////////////

// #define WELP_MATRIX_UNROLL_WITH_CLANG will make Clang compilers unroll loops in matrix multiplication
// #define WELP_MATRIX_UNROLL_WITH_GCC will make GCC compilers unroll loops in matrix multiplication

// #define WELP_MATRIX_DEFAULT_ALLOCATOR defines the name of the allocator used by default
#ifndef WELP_MATRIX_DEFAULT_ALLOCATOR
#include <memory>
#define WELP_MATRIX_DEFAULT_ALLOCATOR std::allocator
#endif // WELP_MATRIX_DEFAULT_ALLOCATOR

// #define WELP_MATRIX_DEFAULT_STREAM_LENGTH defines the capacity of a default matrix stream
#ifndef WELP_MATRIX_DEFAULT_STREAM_LENGTH
#define WELP_MATRIX_DEFAULT_STREAM_LENGTH 256
#endif // WELP_MATRIX_DEFAULT_STREAM_LENGTH

#ifndef WELP_MATRIX_AVX_ps_elim_T
#define WELP_MATRIX_AVX_ps_elim_T 65536
#endif // WELP_MATRIX_AVX_ps_elim_T
#ifndef WELP_MATRIX_AVX_pd_elim_T
#define WELP_MATRIX_AVX_pd_elim_T 32768
#endif // WELP_MATRIX_AVX_pd_elim_T

#ifndef WELP_MATRIX_AVX_ps_trisolve_Ti
#define WELP_MATRIX_AVX_ps_trisolve_Ti 32
#endif // WELP_MATRIX_AVX_ps_trisolve_Ti
#ifndef WELP_MATRIX_AVX_pd_trisolve_Ti
#define WELP_MATRIX_AVX_pd_trisolve_Ti 16
#endif // WELP_MATRIX_AVX_pd_trisolve_Ti

#ifdef WELP_MATRIX_AVX_EXT

// tile sizes for tiled matrix multiplication single precision with AVX
#ifndef WELP_MATRIX_AVX_ps_mm_Ti
#define WELP_MATRIX_AVX_ps_mm_Ti 512 // must be a multiple of 8
#endif // WELP_MATRIX_AVX_ps_mm_Ti
#ifndef WELP_MATRIX_AVX_ps_mm_Tj
#define WELP_MATRIX_AVX_ps_mm_Tj 512 // must be a multiple of 8
#endif // WELP_MATRIX_AVX_ps_mm_Tj
#ifndef WELP_MATRIX_AVX_ps_mm_Tk
#define WELP_MATRIX_AVX_ps_mm_Tk 32 // must be a multiple of 8
#endif // WELP_MATRIX_AVX_ps_mm_Tk

// tile sizes for tiled matrix multiplication double precision with AVX
#ifndef WELP_MATRIX_AVX_pd_mm_Ti
#define WELP_MATRIX_AVX_pd_mm_Ti 512 // must be a multiple of 8
#endif // WELP_MATRIX_AVX_pd_mm_Ti
#ifndef WELP_MATRIX_AVX_pd_mm_Tj
#define WELP_MATRIX_AVX_pd_mm_Tj 256 // must be a multiple of 4
#endif // WELP_MATRIX_AVX_pd_mm_Tj
#ifndef WELP_MATRIX_AVX_pd_mm_Tk
#define WELP_MATRIX_AVX_pd_mm_Tk 32 // must be a multiple of 8
#endif // WELP_MATRIX_AVX_pd_mm_Tk

#endif // defined WELP_MATRIX_AVX_EXT


////////////////////////   D E S C R I P T I O N S   ////////////////////////

////// SUBROUTINES //////

namespace welp
{
	namespace matrix_subroutines // generic kernel
	{
		// notations : pfA = &A[0], pfB = &B[0], pfC = &C[0], pfL = &L[0], pfU = &U[0] etc ...

		// fills C with x, C has n elements
		template <typename Ty> inline void fill(Ty* const pfC, const Ty x, const std::size_t n);

		// C <- row i0 of A, A has Ar rows and Ac columns
		template <typename Ty> inline void get_row(Ty* const pfC, const Ty* const pfA, const std::size_t i, const std::size_t Ac);

		// C <- column j0 of A, A has Ar rows and Ac columns
		template <typename Ty> inline void get_col(Ty* const pfC, const Ty* const pfA, const std::size_t j, const std::size_t Ar, const std::size_t Ac);

		// C <- submatrix of A starting from position (i0, j0), C has Cr rows and Cc columns, A has Ar rows and Ac columns
		template <typename Ty> inline void get_blk(Ty* const pfC, const Ty* const pfA, const std::size_t i0, const std::size_t j0,
			const std::size_t Cr, const std::size_t Cc, const std::size_t Ac);

		// inserts A into C starting from position (i0, j0) in C, C has Cr rows and Cc columns, A has Ar rows and Ac columns
		template <typename Ty> inline void insert(Ty* const pfC, const Ty* const pfA, const std::size_t i0, const std::size_t j0,
			const std::size_t Cr, const std::size_t Cc, const std::size_t Ac);

		// set diagonal to x of matrix C with Cr rows and Cc columns starting from element (r_offset, c_offset)
		template <typename Ty> inline void diag(Ty* const pfC, const Ty x, const std::size_t Cr,
			const std::size_t Cc, const std::size_t r_offset, const std::size_t c_offset) noexcept;

		// C = A', A has Ar rows and Ac columns
		template <typename Ty> void adj(Ty* const pfC, const Ty* const pfA, const std::size_t Ar, const std::size_t Ac);

		// C <- C', C is a square matrix with Cc rows and Cc columns
		template <typename Ty> void adj_sqm(Ty* const pfC, const std::size_t Cc);

		// returns <A, B>, A has n elements
		template <typename Ty> inline Ty dot(const Ty* const pfA, const Ty* const pfB, const std::size_t n) noexcept;

		// returns <A, A>, A has n elements
		template <typename Ty> inline Ty norm2(const Ty* const pfA, const std::size_t n) noexcept;

		// ("-matrix") C = -A, A has n elements
		template <typename Ty> inline void _m(Ty* const pfC, const Ty* const pfA, const std::size_t n) noexcept;

		// ("plus scalar") C <- C + x, C has n elements
		template <typename Ty> inline void ps(Ty* const pfC, const Ty x, const std::size_t n) noexcept;

		// ("scalar plus matrix") C[k] = x + A[k], C has n elements, x is a scalar
		template <typename Ty> inline void spm(Ty* const pfC, const Ty x, const Ty* const pfA, const std::size_t n) noexcept;

		// ("scalar plus -matrix") C[k] = x - A[k], C has n elements, x is a scalar
		template <typename Ty> inline void sp_m(Ty* const pfC, const Ty x, const Ty* const pfA, const std::size_t n) noexcept;

		// ("x scalar") C <- x * C, C has n elements, x is a scalar
		template <typename Ty> inline void pm(Ty* const pfC, const Ty* const pfA, const std::size_t n) noexcept;

		// ("plus -matrix") C <- C - A, C has n elements
		template <typename Ty> inline void p_m(Ty* const pfC, const Ty* const pfA, const std::size_t n) noexcept;

		// ("matrix plus matrix") C = A + B, C has n elements
		template <typename Ty> inline void mpm(Ty* const pfC, const Ty* const pfA, const Ty* const pfB, const std::size_t n) noexcept;

		// ("matrix plus -matrix") C = A - B, C has n elements
		template <typename Ty> inline void mp_m(Ty* const pfC, const Ty* const pfA, const Ty* const pfB, const std::size_t n) noexcept;

		// ("plus scalar") C <- x + C, C has n elements, x is a scalar
		template <typename Ty> inline void xs(Ty* const pfC, const Ty x, const std::size_t n) noexcept;

		// ("scalar x matrix") C = x * A, C has n elements, x is a scalar
		template <typename Ty> inline void sxm(Ty* const pfC, const Ty x, const Ty* const pfA, const std::size_t n) noexcept;

		// ("plus scalar x matrix") C <- C + x * A, C has n elements, x is a scalar
		template <typename Ty> inline void psxm(Ty* const pfC, const Ty x, const Ty* const pfA, const std::size_t n) noexcept;

		// ("plus matrix x matrix") C <- C + A * B, A has Ar rows and Ac columns, B has Bc columns
		template <typename Ty> void pmxm(Ty* const pfC, const Ty* const pfA, const Ty* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept;

		// ("plus -matrix x matrix") C <- C - A * B, A has Ar rows and Ac columns, B has Bc columns
		template <typename Ty> void p_mxm(Ty* const pfC, const Ty* const pfA, const Ty* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept;

		// ("plus diagonal") C <- C + x * I, C has Cr rows and Cc columns, I is a diagonal matrix of 1 starting at (r_offset, c_offset)
		template <typename Ty> inline void pdiag(Ty* const pfC, const Ty x, const std::size_t Cr,
			const std::size_t Cc, const std::size_t r_offset, const std::size_t c_offset) noexcept;

		// A <- Gaussian elim of A, A has Ar rows and Ac columns
		template <typename Ty> void elim_gauss(Ty* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t) noexcept;
		// A <- elim of A using Householder reflections, A has Ar rows and Ac columns
		// Nc is the number of columns being zeroed
		// pfu is an array of Ar elements, pfv is an array of Ac elements
		template <typename Ty> void elim_householder(Ty* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t Nc,
			Ty* const pfu, Ty* const pfv, const std::size_t) noexcept;
		// A <- elim of A using Givens rotations, A has Ar rows and Ac columns
		// Nc is the number of columns being zeroed
		template <typename Ty> void elim_givens(Ty* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t Nc, const std::size_t) noexcept;

		// finds X of triangular system A * X = B where U = [A B], U has Ur rows, X has Xc columns
		template <typename Ty> void trisolve(Ty* const pfX, const Ty* const pfU, const std::size_t Ur, const std::size_t Xc, const std::size_t) noexcept;

		// returns smallest ratio of diagonal of upper triangular matrix U
		template <typename Ty> Ty diagonal_ratio(const Ty* const pfU, const std::size_t Ur, const std::size_t Uc) noexcept;
		// sorts A row-wise with respect to the col column, A has Ar rows and Ac columns
		// pfnum0 and pfnum1 are arrays of Ar std::size_t, pfB is an array of Ar * max(2, Ac) * sizeof(Ty) Ty
		template <typename Ty> void sort(Ty* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t col,
			Ty* const pfB, std::size_t* const pfnum0, std::size_t* const pfnum1);

		// sorts A row-wise with respect to the col column, A has Ar rows and Ac columns
		// pfnum0 and pfnum1 are arrays of Ar std::size_t, pfB is an array of Ar * max(2, Ac) * sizeof(Ty) Ty
		// less_Predicate is a predicate with less_Predicate(a, b) true iff a < b
		template <typename Ty, class Predicate> void sort(Ty* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t col,
			Ty* const pfB, std::size_t* const pfnum0, std::size_t* const pfnum1, Predicate less_Predicate);

#ifdef WELP_MATRIX_INCLUDE_IOSTREAM
		// display A row-major, A has Ar rows and Ac columns, cmax columns displayed on screen, each column is cwidth wide
		template <typename Ty> void say_rm(const Ty* const pfA, const std::size_t Ar, const std::size_t Ac,
			const std::size_t cmax, const std::size_t rblock, std::streamsize cwidth);

		// display A column-major, A has Ar rows and Ac columns, cmax columns displayed on screen, each column is cwidth wide
		template <typename Ty> void say_cm(const Ty* const pfA, const std::size_t Ar, const std::size_t Ac,
			const std::size_t cmax, const std::size_t rblock, std::streamsize cwidth);
#endif // WELP_MATRIX_INCLUDE_IOSTREAM

		// C = A^n, A has Ac rows and columns, pfpool0 and pfpool1 are arrays of Ac * Ac elements
		// DEPENDENCIES : calls the function welp::matrix_subroutines::pmxm
		template <typename Ty, typename Tn> void pow(Ty* const pfC, const Ty* const pfA, const Tn n,
			const std::size_t Ac, Ty* const pfpool0, Ty* const pfpool1) noexcept;

		// returns true if A(i, j) == B(i, j) for all i, j
		template <typename Ty> inline bool compare_equal_mm(const Ty* const pfA, const Ty* const pfB, const std::size_t n);

		// returns false if A(i, j) < B(i, j) for all i, j
		template <typename Ty> inline bool compare_strictly_smaller_mm(const Ty* const pfA, const Ty* const pfB, const std::size_t n);

		// returns true if A(i, j) > B(i, j) for all i, j
		template <typename Ty> inline bool compare_strictly_greater_mm(const Ty* const pfA, const Ty* const pfB, const std::size_t n);

		// returns true if A(i, j) <= B(i, j) for all i, j
		template <typename Ty> inline bool compare_smaller_mm(const Ty* const pfA, const Ty* const pfB, const std::size_t n);

		// returns true if A(i, j) >= B(i, j) for all i, j
		template <typename Ty> inline bool compare_greater_mm(const Ty* const pfA, const Ty* const pfB, const std::size_t n);

		// returns true if Pr(A(i, j), B(i, j)) is true for all i, j
		template <typename Ty1, typename Ty2, class Predicate> inline bool compare_mm(const Ty1* const pfA, const Ty2* const pfB,
			const std::size_t n, Predicate Pr);

		// returns true if A(i, j) == x for all i, j
		template <typename Ty> inline bool compare_equal_ms(const Ty* const pfA, const Ty x, const std::size_t n);

		// returns false if A(i, j) < x for all i, j
		template <typename Ty> inline bool compare_strictly_smaller_ms(const Ty* const pfA, const Ty x, const std::size_t n);

		// returns true if A(i, j) > x for all i, j
		template <typename Ty> inline bool compare_strictly_greater_ms(const Ty* const pfA, const Ty x, const std::size_t n);

		// returns true if A(i, j) <= x for all i, j
		template <typename Ty> inline bool compare_smaller_ms(const Ty* const pfA, const Ty x, const std::size_t n);

		// returns true if A(i, j) >= x for all i, j
		template <typename Ty> inline bool compare_greater_ms(const Ty* const pfA, const Ty x, const std::size_t n);

		// returns true if Pr(A(i, j), x) is true for all i, j
		template <typename Ty1, typename Ty2, class Predicate> inline bool compare_ms(const Ty1* const pfA,
			const Ty2 x, const std::size_t n, Predicate Pr);
	}

#ifdef WELP_MATRIX_AVX_EXT
	namespace matrix_subroutines // kernel with AVX intrinsics for single precision numbers
	{
		inline void fill(float* const pfC, const float x, const std::size_t n) noexcept;
		inline float dot(const float* const pfA, const float* const pfB, const std::size_t n) noexcept;
		inline float norm2(const float* const pfA, const std::size_t n) noexcept;
		inline void _m(float* const pfC, const float* const pfA, const std::size_t n) noexcept;
		inline void ps(float* const pfC, const float x, const std::size_t n) noexcept;
		inline void spm(float* const pfC, const float x, const float* const pfA, const std::size_t n) noexcept;
		inline void sp_m(float* const pfC, const float x, const float* const pfA, const std::size_t n) noexcept;
		inline void pm(float* const pfC, const float* const pfA, const std::size_t n) noexcept;
		inline void p_m(float* const pfC, const float* const pfA, const std::size_t n) noexcept;
		inline void mpm(float* const pfC, const float* const pfA, const float* const pfB, const std::size_t n) noexcept;
		inline void mp_m(float* const pfC, const float* const pfA, const float* const pfB, const std::size_t n) noexcept;
		inline void xs(float* const pfC, const float x, const std::size_t n) noexcept;
		inline void sxm(float* const pfC, const float x, const float* const pfA, const std::size_t n) noexcept;
		inline void psxm(float* const pfC, const float x, const float* const pfA, const std::size_t n) noexcept;
		void pmxm(float* const pfC, const float* const pfA, const float* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept;
		void p_mxm(float* const pfC, const float* const pfA, const float* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept;
		void elim_gauss(float* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t slice) noexcept;
		void elim_householder(float* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t Nc,
			float* const pfu, float* const pfv, const std::size_t slice) noexcept;
		void elim_givens(float* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t Nc, const std::size_t slice) noexcept;
		void trisolve(float* const pfX, const float* const pfU, const std::size_t Ur, const std::size_t Xc, const std::size_t slice) noexcept;

		void pmxm(int* const pfC, const int* const pfA, const int* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept;
		void p_mxm(int* const pfC, const int* const pfA, const int* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept;
	}

	namespace matrix_subroutines // kernel with AVX intrinsics for double precision numbers
	{
		inline void fill(double* const pfC, const double x, const std::size_t n) noexcept;
		inline double dot(const double* const pfA, const double* const pfB, const std::size_t n) noexcept;
		inline double norm2(const double* const pfA, const std::size_t n) noexcept;
		inline void _m(double* const pfC, const double* const pfA, const std::size_t n) noexcept;
		inline void ps(double* const pfC, const double x, const std::size_t n) noexcept;
		inline void spm(double* const pfC, const double x, const double* const pfA, const std::size_t n) noexcept;
		inline void sp_m(double* const pfC, const double x, const double* const pfA, const std::size_t n) noexcept;
		inline void pm(double* const pfC, const double* const pfA, const std::size_t n) noexcept;
		inline void p_m(double* const pfC, const double* const pfA, const std::size_t n) noexcept;
		inline void mpm(double* const pfC, const double* const pfA, const double* const pfB, const std::size_t n) noexcept;
		inline void mp_m(double* const pfC, const double* const pfA, const double* const pfB, const std::size_t n) noexcept;
		inline void xs(double* const pfC, const double x, const std::size_t n) noexcept;
		inline void sxm(double* const pfC, const double x, const double* const pfA, const std::size_t n) noexcept;
		inline void psxm(double* const pfC, const double x, const double* const pfA, const std::size_t n) noexcept;
		void pmxm(double* const pfC, const double* const pfA, const double* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept;
		void p_mxm(double* const pfC, const double* const pfA, const double* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept;
		void elim_gauss(double* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t slice) noexcept;
		void elim_householder(double* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t Nc,
			double* const pfu, double* const pfv, const std::size_t slice) noexcept;
		void elim_givens(double* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t Nc, const std::size_t slice) noexcept;
		void trisolve(double* const pfX, const double* const pfU, const std::size_t Ur, const std::size_t Xc, const std::size_t slice) noexcept;
	}
#endif // WELP_MATRIX_AVX_EXT
}


////// CONTAINER //////

namespace welp
{
	template <typename Ty, class _Allocator> class _matrix_container : private _Allocator
	{

	public:

		// infos

		// returns the number of rows of *this
		inline std::size_t r() const noexcept { return rows; }
		// returns the number of columns of *this
		inline std::size_t c() const noexcept { return cols; }
		// returns the pointer to the array owned by *this
		inline const Ty* data() const noexcept { return data_ptr; }
		inline Ty* data() noexcept { return data_ptr; }
		// returns the number of elements of *this
		inline std::size_t size() const noexcept { return rows * cols; }
		// returns the number of elements that can be stored in the memory owned by *this
		inline std::size_t capacity() const noexcept { return storage_capacity; }

		inline const Ty* begin() const noexcept { return data_ptr; }
		inline const Ty* end() const noexcept { return end_ptr; }
		inline Ty* begin() noexcept { return data_ptr; }
		inline Ty* end() noexcept { return end_ptr; }
		inline const Ty* cbegin() const noexcept { return data_ptr; }
		inline const Ty* cend() const noexcept { return end_ptr; }

		// constructors, assign, copy, destructor

		_matrix_container() noexcept;
		_matrix_container(std::size_t new_capacity);
		_matrix_container(std::size_t new_r, std::size_t new_c);
		_matrix_container(std::size_t new_capacity, std::size_t new_r, std::size_t new_c);

		_matrix_container(const _matrix_container<Ty, _Allocator>& rhs_container);
		_matrix_container<Ty, _Allocator>& operator=(const _matrix_container<Ty, _Allocator>& rhs_container);
		_matrix_container(_matrix_container<Ty, _Allocator>&& rhs_container) noexcept;
		_matrix_container<Ty, _Allocator>& operator=(_matrix_container<Ty, _Allocator>&& rhs_container) noexcept;

		~_matrix_container() noexcept;

		// accessors

		inline const Ty& operator[](std::size_t offset) const noexcept;
		inline Ty& operator[](std::size_t offset) noexcept;
		inline const Ty& operator()(std::size_t row_index, std::size_t col_index) const noexcept;
		inline Ty& operator()(std::size_t row_index, std::size_t col_index) noexcept;

		// memory and dimension management

		// reserve memory for the number of elements
		void reserve(std::size_t new_capacity_value);
		// reserve memory for the number of rows and columnns
		void reserve(std::size_t new_r, std::size_t new_c);
		// resize the matrix for the number of rows and columnns
		void resize(std::size_t new_r, std::size_t new_c);
		// reduces the amount of memory owned to the minimum
		void shrink_to_fit();
		// deallocates the memory currently owned
		void clear() noexcept;

	private:

		Ty* data_ptr; // pointer to the beginning of the storage
		Ty* end_ptr; // pointer to the end of the storage
		std::size_t rows; // number of rows
		std::size_t cols; // number of columns

		std::size_t storage_capacity; // capacity of the allocated memory block

		inline void construct_all();
		inline void construct_all(Ty* ptr, std::size_t instances);
		inline void destroy_all() noexcept;
		inline void destroy_all(Ty* ptr, std::size_t instances) noexcept;
		inline void copy_all(Ty* copy_to_ptr, Ty* copy_from_ptr, std::size_t instances);
		inline void move_all(Ty* copy_to_ptr, Ty* copy_from_ptr, std::size_t instances);

		class _matrix_temp : private _Allocator
		{

		public:

			Ty* data_ptr; // pointer to the beginning of the storage
			std::size_t storage_capacity;

			_matrix_temp(std::size_t new_capacity)
			{
				data_ptr = this->allocate(new_capacity); // constructions by _matrix_container for non POD types
			}
			~_matrix_temp() noexcept
			{
				if (data_ptr != nullptr) // in case of an exception
				{
					Ty* p = data_ptr + (storage_capacity - 1);
					std::size_t r = storage_capacity & 3;
					std::size_t k;

					for (k = storage_capacity - r; k > 0; k -= 4)
					{
						p->~Ty();
						(p - 1)->~Ty();
						(p - 2)->~Ty();
						(p - 3)->~Ty();
						p -= 4;
					}

					switch (r)
					{

					case 0:
						break;

					case 1:
						p->~Ty();
						break;

					case 2:
						p->~Ty();
						(p - 1)->~Ty();
						break;

					case 3:
						p->~Ty();
						(p - 1)->~Ty();
						(p - 2)->~Ty();
						break;
					}
					this->deallocate(data_ptr, storage_capacity);
				}
			}

			_matrix_temp(const welp::_matrix_container<Ty, _Allocator>::_matrix_temp& rhs_temp) = delete;
			welp::_matrix_container<Ty, _Allocator>::_matrix_temp& operator=(const welp::_matrix_container<Ty, _Allocator>::_matrix_temp& rhs_temp) = delete;
			_matrix_temp(welp::_matrix_container<Ty, _Allocator>::_matrix_temp&& rhs_temp) = delete;
			welp::_matrix_container<Ty, _Allocator>::_matrix_temp& operator=(welp::_matrix_container<Ty, _Allocator>::_matrix_temp&& rhs_temp) = delete;
		};
	};
}


////// MATRIX CLASS //////

namespace welp
{
	// generic implementation
	template <typename Ty, class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty>> class matrix : public welp::_matrix_container<Ty, _Allocator>
	{

	public:

		// constructors and destructors

		matrix() noexcept = default;
		// initializes the matrix with a given capacity
		matrix(std::size_t new_capacity);
		// initializes the matrix with a given numbers of rows and colums
		matrix(std::size_t new_r, std::size_t new_c);
		// initializes the matrix with a given numbers of rows and colums filled with a specific value
		matrix(std::size_t new_r, std::size_t new_c, Ty x);
#ifdef WELP_MATRIX_INCLUDE_INITLIST
		// initializes the matrix with a given numbers of rows and colums filled with elements from initializer list
		matrix(std::size_t new_r, std::size_t new_c, std::initializer_list<Ty> L);
#endif // WELP_MATRIX_INCLUDE_INITLIST

		matrix(const welp::matrix<Ty, _Allocator>&) = default;
		welp::matrix<Ty, _Allocator>& operator=(const welp::matrix<Ty, _Allocator>&) = default;
		matrix(welp::matrix<Ty, _Allocator>&&) = default;
		welp::matrix<Ty, _Allocator>& operator=(welp::matrix<Ty, _Allocator>&&) = default;

		~matrix();

		// set

		// copies from pointer p into *this
		welp::matrix<Ty, _Allocator>& cpy(const Ty* const p);
		// copies from iterator iter into *this
		template <class iter> welp::matrix<Ty, _Allocator>& cpy(iter p);
		// fills *this with value x
		welp::matrix<Ty, _Allocator>& fill(Ty x);
		// fills diagonal of *this with value x
		welp::matrix<Ty, _Allocator>& diag(Ty x);
		// fills diagonal of *this starting at position (i0, j0) with value x
		welp::matrix<Ty, _Allocator>& diag(Ty x, std::size_t i0, std::size_t j0);

		// submatrix operations

		// extracts the row i0
		welp::matrix<Ty, _Allocator> get_row(std::size_t i0) const;
		// extracts the column j0
		welp::matrix<Ty, _Allocator> get_col(std::size_t j0) const;
		// extracts a Cr x Cc submatrix of the elements starting at position (i0, j0)
		welp::matrix<Ty, _Allocator> get_blk(std::size_t i0, std::size_t j0, std::size_t Cr, std::size_t Cc) const;
		// fills matrix C with the elements of *this starting at position (i0, j0) from *this
		template <class _Allocator_C> void get_into(std::size_t i0, std::size_t j0, welp::matrix<Ty, _Allocator_C>& C) const;
		// inserts matrix A into *this starting at position (i0, j0) from *this
		template <class _Allocator_A> welp::matrix<Ty, _Allocator>& insert(std::size_t i0, std::size_t j0, const welp::matrix<Ty, _Allocator_A>& A);

		// linear algebra operations

		// *this <- *this'
		welp::matrix<Ty, _Allocator>& adj();
		template <class _Allocator_A> welp::matrix<Ty, _Allocator>& adj(const welp::matrix<Ty, _Allocator_A>& A);
		// *this <- *this + x
		welp::matrix<Ty, _Allocator>& operator+=(Ty x) noexcept;
		// *this <- *this - x
		welp::matrix<Ty, _Allocator>& operator-=(Ty x) noexcept;
		// *this <- *this + A
		template <class _Allocator_A> welp::matrix<Ty, _Allocator>& operator+=(const welp::matrix<Ty, _Allocator_A>& A) noexcept;
		// *this <- *this - A
		template <class _Allocator_A> welp::matrix<Ty, _Allocator>& operator-=(const welp::matrix<Ty, _Allocator_A>& A) noexcept;
		// *this <- *this * x
		welp::matrix<Ty, _Allocator>& operator*=(Ty x) noexcept;
		// *this <- x * A
		template <class _Allocator_A> welp::matrix<Ty, _Allocator>& sxm(Ty x, const welp::matrix<Ty, _Allocator_A>& A) noexcept;
		// *this <- *this + x * A
		template <class _Allocator_A> welp::matrix<Ty, _Allocator>& psxm(Ty x, const welp::matrix<Ty, _Allocator_A>& A) noexcept;
		// *this <- *this - x * A
		template <class _Allocator_A> welp::matrix<Ty, _Allocator>& p_sxm(Ty x, const welp::matrix<Ty, _Allocator_A>& A) noexcept;
		// *this <- *this + A * B
		template <class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator>& pmxm(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept;
		// *this <- *this - A * B
		template <class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator>& p_mxm(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept;
		// *this <- *this + v * w'
		template <class _Allocator_v, class _Allocator_w> welp::matrix<Ty, _Allocator>& pvxv(const welp::matrix<Ty, _Allocator_v>& v, const welp::matrix<Ty, _Allocator_w>& w) noexcept;
		// *this <- *this - v * w'
		template <class _Allocator_v, class _Allocator_w> welp::matrix<Ty, _Allocator>& p_vxv(const welp::matrix<Ty, _Allocator_v>& v, const welp::matrix<Ty, _Allocator_w>& w) noexcept;
		// *this <- *this + x * I
		welp::matrix<Ty, _Allocator>& pdiag(Ty x) noexcept;
		// *this <- *this - x * I
		welp::matrix<Ty, _Allocator>& p_diag(Ty x) noexcept;
		// *this <- *this + diagonal of x starting at (i0, j0)
		welp::matrix<Ty, _Allocator>& pdiag(Ty x, std::size_t i0, std::size_t j0) noexcept;
		// *this <- *this - diagonal of x starting at (i0, j0)
		welp::matrix<Ty, _Allocator>& p_diag(Ty x, std::size_t i0, std::size_t j0) noexcept;

		// *this <- Gaussian elim of *this
		welp::matrix<Ty, _Allocator>& elim_gauss() noexcept;
		// *this <- elim of *this using Householder reflections, Nc is the number of columns being zeroed
		welp::matrix<Ty, _Allocator>& elim_householder(std::size_t Nc);
		// *this <- elim of *this using Givens rotations, Nc is the number of columns being zeroed
		welp::matrix<Ty, _Allocator>& elim_givens(std::size_t Nc) noexcept;
		// *this <- X, X of triangular system A * X = B where U = [A B]
		template <class _Allocator_U> welp::matrix<Ty, _Allocator>& trisolve(const welp::matrix<Ty, _Allocator_U>& U) noexcept;

		// sorts *this row-wise with respect to the col column
		welp::matrix<Ty, _Allocator>& sort(std::size_t col);
		// sorts *this row-wise with respect to the col column using predicate Pr
		template <class Predicate> welp::matrix<Ty, _Allocator>& sort(std::size_t col, Predicate Pr);
		// sorts *this row-wise with respect to the col column between rows from row_begin to row_end
		welp::matrix<Ty, _Allocator>& sort(std::size_t col, std::size_t row_begin, std::size_t row_end);
		// sorts *this row-wise with respect to the col column between rows from row_begin to row_end using predicate less_Predicate
		template <class Predicate> welp::matrix<Ty, _Allocator>& sort(std::size_t col, std::size_t row_begin, std::size_t row_end, Predicate less_Predicate);

#ifdef WELP_MATRIX_INCLUDE_IOSTREAM
		// prints *this matrix in console
		template <std::size_t cmax = 10, std::size_t rblock = 20, std::size_t cwidth = 12> const welp::matrix<Ty, _Allocator>& say() const;
		template <std::size_t cmax = 10, std::size_t rblock = 20, std::size_t cwidth = 12, typename msg_Ty> const welp::matrix<Ty, _Allocator>& say
		(const msg_Ty& msg) const;
		template <std::size_t cmax = 10, std::size_t rblock = 20, std::size_t cwidth = 12,
			typename msg_Ty1, typename msg_Ty2> const welp::matrix<Ty, _Allocator>& say
			(const msg_Ty1& msg1, const msg_Ty2& msg2) const;
		template <std::size_t cmax = 10, std::size_t rblock = 20, std::size_t cwidth = 12,
			typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> const welp::matrix<Ty, _Allocator>& say
			(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3) const;
		template <std::size_t cmax = 10, std::size_t rblock = 20, std::size_t cwidth = 12,
			typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> const welp::matrix<Ty, _Allocator>& say
			(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4) const;
		// prints *this matrix in console
		template <std::size_t cmax = 10, std::size_t rblock = 20, std::size_t cwidth = 12> welp::matrix<Ty, _Allocator>& say();
		template <std::size_t cmax = 10, std::size_t rblock = 20, std::size_t cwidth = 12, typename msg_Ty> welp::matrix<Ty, _Allocator>& say
		(const msg_Ty& msg);
		template <std::size_t cmax = 10, std::size_t rblock = 20, std::size_t cwidth = 12,
			typename msg_Ty1, typename msg_Ty2> welp::matrix<Ty, _Allocator>& say
			(const msg_Ty1& msg1, const msg_Ty2& msg2);
		template <std::size_t cmax = 10, std::size_t rblock = 20, std::size_t cwidth = 12,
			typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> welp::matrix<Ty, _Allocator>& say
			(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3);
		template <std::size_t cmax = 10, std::size_t rblock = 20, std::size_t cwidth = 12,
			typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> welp::matrix<Ty, _Allocator>& say
			(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4);
#endif // WELP_MATRIX_INCLUDE_IOSTREAM
	};

	template <typename Ty, class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty>> using matrix_ptr = welp::matrix<Ty, _Allocator>*;
	template <typename Ty, class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty>> using const_matrix_ptr = const welp::matrix<Ty, _Allocator>*;
	template <typename Ty, class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty>> using matrix_ref = welp::matrix<Ty, _Allocator>&;
	template <typename Ty, class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty>> using const_matrix_ref = const welp::matrix<Ty, _Allocator>&;

	template <std::size_t length, typename Ty, class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty>> class matrix_stream
	{

	private:

		welp::const_matrix_ptr<Ty, _Allocator> arr[length] = { nullptr };

	public:

		std::size_t count = 0;

		inline const welp::const_matrix_ptr<Ty, _Allocator>& operator[](std::size_t offset) const noexcept;
		inline welp::const_matrix_ptr<Ty, _Allocator>& operator[](std::size_t offset) noexcept;

		inline welp::matrix_stream<length, Ty, _Allocator>& operator<<(const welp::matrix<Ty, _Allocator>& A) noexcept;
		inline welp::matrix_stream<length, Ty, _Allocator>& operator<<(std::size_t repeat) noexcept;

		matrix_stream() = default;
		matrix_stream(const welp::matrix_stream<length, Ty, _Allocator>&) = default;
		welp::matrix_stream<length, Ty, _Allocator>& operator=(const welp::matrix_stream<length, Ty, _Allocator>&) = default;
		matrix_stream(welp::matrix_stream<length, Ty, _Allocator>&&) = default;
		welp::matrix_stream<length, Ty, _Allocator>& operator=(welp::matrix_stream<length, Ty, _Allocator>&&) = default;
		~matrix_stream() = default;

		matrix_stream(const welp::matrix<Ty, _Allocator>& A) : count(1) { arr[0] = &A; }
	};

	// operators

	template <typename Ty, class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator_B> operator+(
		const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B);

	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> operator-(const welp::matrix<Ty, _Allocator>& A);

	template <typename Ty, class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator_B> operator-(
		const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B);

	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> operator*(Ty x, const welp::matrix<Ty, _Allocator>& A);

	template <typename Ty, class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator_B> operator*(
		const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B);

	template <typename Ty, class _Allocator, typename Tn> welp::matrix<Ty, _Allocator> pow(const welp::matrix<Ty, _Allocator>& A, Tn n);

	// comparisons

	template <typename Ty, class _Allocator_A, class _Allocator_B> bool operator==(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept;
	template <typename Ty, class _Allocator_A, class _Allocator_B> bool operator!=(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept;
	template <typename Ty, class _Allocator_A, class _Allocator_B> bool operator<(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept;
	template <typename Ty, class _Allocator_A, class _Allocator_B> bool operator>(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept;
	template <typename Ty, class _Allocator_A, class _Allocator_B> bool operator<=(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept;
	template <typename Ty, class _Allocator_A, class _Allocator_B> bool operator>=(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept;
	// returns true if and only if _relation(Aij, Bij) is true for all i, j
	template <typename Ty, class relation_function, class _Allocator_A, class _Allocator_B> bool relation(
		relation_function _relation, const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B);

	template <typename Ty, class _Allocator> bool operator==(const welp::matrix<Ty, _Allocator>& A, const Ty& x) noexcept;
	template <typename Ty, class _Allocator> bool operator!=(const welp::matrix<Ty, _Allocator>& A, const Ty& x) noexcept;
	template <typename Ty, class _Allocator> bool operator<(const welp::matrix<Ty, _Allocator>& A, const Ty& x) noexcept;
	template <typename Ty, class _Allocator> bool operator>(const welp::matrix<Ty, _Allocator>& A, const Ty& x) noexcept;
	template <typename Ty, class _Allocator> bool operator<=(const welp::matrix<Ty, _Allocator>& A, const Ty& x) noexcept;
	template <typename Ty, class _Allocator> bool operator>=(const welp::matrix<Ty, _Allocator>& A, const Ty& x) noexcept;

	template <typename Ty, class _Allocator> bool operator==(const Ty& x, const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> bool operator!=(const Ty& x, const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> bool operator<(const Ty& x, const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> bool operator>(const Ty& x, const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> bool operator<=(const Ty& x, const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> bool operator>=(const Ty& x, const welp::matrix<Ty, _Allocator>& A) noexcept;
	// returns true if and only if _relation(Aij, x) is true for all i, j
	template <typename Ty, class relation_function, class _Allocator> bool relation(relation_function _relation, const welp::matrix<Ty, _Allocator>& A, const Ty& x);

	// returns true iff A does not contain any nan
	template <typename Ty, class _Allocator> bool well_defined(const welp::matrix<Ty, _Allocator>& A) noexcept;

	template <std::size_t length = WELP_MATRIX_DEFAULT_STREAM_LENGTH, typename Ty, class _Allocator>
	inline welp::matrix_stream<length, Ty, _Allocator> operator<<(const welp::matrix<Ty, _Allocator>& A, const welp::matrix<Ty, _Allocator>& B);
	template <std::size_t length = WELP_MATRIX_DEFAULT_STREAM_LENGTH, typename Ty, class _Allocator>
	inline welp::matrix_stream<length, Ty, _Allocator> operator<<(const welp::matrix<Ty, _Allocator>& A, std::size_t n);
}

#ifdef WELP_MATRIX_INCLUDE_IOSTREAM
template <typename Ty, class _Allocator> std::ostream& operator << (std::ostream& out, const welp::matrix<Ty, _Allocator>& A);
#endif // WELP_MATRIX_INCLUDE_IOSTREAM

////// commons //////

namespace welp
{
	// returns a N x N matrix with x for the diagonal elements
	template <typename Ty, class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty>> welp::matrix<Ty, _Allocator> id(std::size_t N, Ty x);
	// returns a M x N matrix with random numbers between a and b
	template <typename Ty, class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty>> welp::matrix<Ty, _Allocator> random_matrix(std::size_t M, std::size_t N, Ty a, Ty b);
	template <typename Ty, class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty>> welp::matrix<Ty, _Allocator> linspace_length(Ty x, Ty length, Ty y);
	template <typename Ty, class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty>> welp::matrix<Ty, _Allocator> linspace_number(Ty x, std::size_t number, Ty y);

	template <typename Ty, class _Allocator> Ty min(const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> Ty max(const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> Ty sum(const welp::matrix<Ty, _Allocator>& A) noexcept;

	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> abs(const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> Ty min_abs(const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> Ty max_abs(const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> Ty sum_abs(const welp::matrix<Ty, _Allocator>& A) noexcept;

	template <typename Ty, class _Allocator> std::size_t arg_min(const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> std::size_t arg_max(const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> std::size_t arg_min_abs(const welp::matrix<Ty, _Allocator>& A) noexcept;
	template <typename Ty, class _Allocator> std::size_t arg_max_abs(const welp::matrix<Ty, _Allocator>& A) noexcept;

	template <typename Ty, class _Allocator> Ty trace(const welp::matrix<Ty, _Allocator>& A) noexcept;

	// returns A'
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> adj(const welp::matrix<Ty, _Allocator>& A);
	// returns A flipped horizontally 
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> flip_lr(const welp::matrix<Ty, _Allocator>& A);
	// returns A flipped vertically 
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> flip_ud(const welp::matrix<Ty, _Allocator>& A);

	// welp::blk_hor( A0 << A1 << ... << An ) returns A0, A1, ... , An concatenated horizontally
	template <typename Ty, std::size_t length, class _Allocator> welp::matrix<Ty, _Allocator> blk_hor(const welp::matrix_stream<length, Ty, _Allocator>& blk);
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> blk_hor(const welp::matrix<Ty, _Allocator>& A);
	// welp::blk_vert( A0 << A1 << ... << An ) returns A0, A1, ... , An concatenated vertically
	template <typename Ty, std::size_t length, class _Allocator> welp::matrix<Ty, _Allocator> blk_vert(const welp::matrix_stream<length, Ty, _Allocator>& blk);
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> blk_vert(const welp::matrix<Ty, _Allocator>& A);
	// welp::blk_diag( A0 << A1 << ... << An ) returns A0, A1, ... , An concatenated in a diagonal
	template <typename Ty, std::size_t length, class _Allocator> welp::matrix<Ty, _Allocator> blk_diag(const welp::matrix_stream<length, Ty, _Allocator>& blk);
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> blk_diag(const welp::matrix<Ty, _Allocator>& A);
}


////// dot product, norm //////

namespace welp
{
	// returns the dot product of A and B
	template <typename Ty, class _Allocator_A, class _Allocator_B> Ty dot(
		const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept;
	// returns the 2-norm of A
	template <typename Ty, class _Allocator> Ty norm(const welp::matrix<Ty, _Allocator>& A) noexcept;
	// returns the squared 2-norm of A
	template <typename Ty, class _Allocator> Ty norm2(const welp::matrix<Ty, _Allocator>& A) noexcept;
}


////// solve, inverse, determinant //////

namespace welp
{
	namespace solve
	{
		// returns X such that A * X = B using Gaussian eliminations
		template <typename Ty, class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator_B> gauss(
			const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B);

		// returns X such that A * X = B using Householder reflections, uses least squares if A is non-square
		template <typename Ty, class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator_B> householder(
			const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B);

		// returns X such that A * X = B using Givens rotations, uses least squares if A is non-square
		template <typename Ty, class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator_B> givens(
			const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B);

		// uses the Newton's method to find x such that f(x) = b, with J being the Jacobian function of f, x0 being the initial point
		// max_iter being the maximum number of iterations, tol being the precision
		template <typename Ty, class function_f, class function_J, class _Allocator> welp::matrix<Ty, _Allocator> newton(const function_f& f,
			const function_J& J, const welp::matrix<Ty, _Allocator>& x0, const welp::matrix<Ty, _Allocator>& b, int max_iter, Ty tol);

		// uses the broydens's method to update x and Jinv such that f(x) = b, with Jinv starting as the inverse Jacobian of f at x,
		// x starting as the initial point, max_iter being the maximum number of iterations, tol being the precision
		template <typename Ty, class function_f, class _Allocator> void broyden_update(const function_f& f, welp::matrix<Ty, _Allocator>& Jinv,
			welp::matrix<Ty, _Allocator>& x, const welp::matrix<Ty, _Allocator>& b, int max_iter, Ty tol);

		// uses the broydens's method to find x such that f(x) = b, with Jx0innv being the inverse Jacobian of f at x0, x0 being the initial point,
		// max_iter being the maximum number of iterations, tol being the precision
		template <typename Ty, class function_f, class _Allocator> inline welp::matrix<Ty, _Allocator> broyden(
			const function_f& f, const welp::matrix<Ty, _Allocator>& Jx0inv, const welp::matrix<Ty, _Allocator>& x0,
			const welp::matrix<Ty, _Allocator>& b, int max_iter, Ty tol);
	}

	namespace inv
	{
		// returns A^-1 using Gaussian eliminations
		template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> gauss(const welp::matrix<Ty, _Allocator>& A);
		// returns A^-1 using Householder reflections
		template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> householder(const welp::matrix<Ty, _Allocator>& A);
		// returns A^-1 using Givens rotations
		template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> givens(const welp::matrix<Ty, _Allocator>& A);
	}

	// returns determinant of A
	template <typename Ty, class _Allocator> Ty det(const welp::matrix<Ty, _Allocator>& A);
	// returns the greatest absolute element on the diagonal divided by smallest absolute elemet on the diagonal of A
	template <typename Ty, class _Allocator> Ty diagonal_ratio(const welp::matrix<Ty, _Allocator>& A);
}


////// optimization //////

namespace welp
{
	namespace opti
	{
		// uses gradient method for differentiable quasi convex objective function obj, gradient function grad to update x,
		// backtrack parameters alpha and beta, max numbers of iterations max_iter and stop when |grad(x)| < tol, track_forward allows tracking forward
		template <typename Ty, class function_obj, class function_grd, class _Allocator> void quasi_convex_update(
			const function_obj& obj, const function_grd& grad, welp::matrix<Ty, _Allocator>& x,
			Ty alpha, Ty beta, int max_iter, Ty tol, bool track_forward);

		// uses gradient method for differentialbe quasi convex objective function obj, gradient function grad, initial point x0,
		// backtrack parameters alpha and beta, max numbers of iterations max_iter and stop when |grad(x)| < tol, track_forward allows tracking forward
		template <typename Ty, class function_obj, class function_grd, class _Allocator> inline welp::matrix<Ty, _Allocator> quasi_convex(
			const function_obj& obj, const function_grd& grad, const welp::matrix<Ty, _Allocator>& x0,
			Ty alpha, Ty beta, int max_iter, Ty tol, bool track_forward);

		// uses BFGS method for twice differentiable strictly convex objective function obj, gradient function grad to update x and inverse of Jacobian Jxinv at x
		// backtrack parameters alpha and beta, max numbers of iterations max_iter and stop when |grad(x)| < tol
		template <typename Ty, class function_obj, class function_grd, class _Allocator> void strict_convex_update(
			const function_obj& obj, const function_grd& grad, welp::matrix<Ty, _Allocator>& x, welp::matrix<Ty, _Allocator>& Binv,
			Ty alpha, Ty beta, int max_iter, Ty tol);

		// uses BFGS method for twice differentiable strictly convex objective function obj, gradient function grad, initial point x0,
		// inverse of Hessian Bx0inv at x0, backtrack parameters alpha and beta, max numbers of iterations max_iter and stop when |grad(x)| < tol
		template <typename Ty, class function_obj, class function_grd, class _Allocator> inline welp::matrix<Ty, _Allocator> strict_convex(
			const function_obj& obj, const function_grd& grad, const welp::matrix<Ty, _Allocator>& x0, const welp::matrix<Ty, _Allocator>& Bx0inv,
			Ty alpha, Ty beta, int max_iter, Ty tol);
	}
}


////// gradient and jacobian //////

namespace welp
{
	// return the gradient of f at x with finite stepsize h
	template <typename Ty, class function_f, class _Allocator> inline welp::matrix<Ty, _Allocator> gradient(
		function_f f, const welp::matrix<Ty, _Allocator>& x, Ty h);

	// return the jacobian of f at x with finite stepsize h
	template <typename Ty, class function_f, class _Allocator> inline welp::matrix<Ty, _Allocator> jacobian(
		function_f f, const welp::matrix<Ty, _Allocator>& x, Ty h);
}


////// element-wise and row-wise functional operations //////

namespace welp
{
	// returns matrix C such that Cij =  f(Aij) for all i, j
	template <typename Ty2, class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty2>, typename Ty1, class _Allocator>
	welp::matrix<Ty2, _return_Allocator> eval(Ty2(*f)(Ty1), const welp::matrix<Ty1, _Allocator>& A);

	// returns column matrix C such that Ci = f(row i of A) for all i
	template <typename Ty2, class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty2>, typename Ty1, class _Allocator>
	welp::matrix<Ty2, _return_Allocator> eval(Ty2(*f)(welp::matrix<Ty1, _Allocator>), const welp::matrix<Ty1, _Allocator>& A);

	// returns matrix C such that row i of C = f(Ai) for all i, A being a colum matrix
	template <typename Ty2, class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty2>,
		typename Ty1, class _Allocator, class _temp_Allocator> welp::matrix<Ty2, _return_Allocator> eval(
			welp::matrix<Ty2, _temp_Allocator>(*f)(Ty1), const welp::matrix<Ty1, _Allocator>& A);

	// returns matrix C such that row i of C = f(row i of A) for all i
	template <typename Ty2, class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty2>,
		typename Ty1, class _Allocator, class _temp_Allocator> welp::matrix<Ty2, _return_Allocator> eval(
			welp::matrix<Ty2, _temp_Allocator>(*f)(welp::matrix<Ty1, _Allocator>), const welp::matrix<Ty1, _Allocator>& A);

#ifdef WELP_MATRIX_INCLUDE_FUNCTIONAL
	// returns matrix C such that Cij =  f(Aij) for all i, j
	template <typename Ty2, class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty2>, typename Ty1, class _Allocator>
	welp::matrix<Ty2, _return_Allocator> eval(const std::function<Ty2(Ty1)>& f, const welp::matrix<Ty1, _Allocator>& A);

	// returns column matrix C such that Ci = f(row i of A) for all i
	template <typename Ty2, class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty2>, typename Ty1, class _Allocator>
	welp::matrix<Ty2, _return_Allocator> eval(const std::function<Ty2(welp::matrix<Ty1, _Allocator>)>& f, const welp::matrix<Ty1, _Allocator>& A);

	// returns matrix C such that row i of C = f(Ai) for all i, A being a colum matrix
	template <typename Ty2, class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty2>,
		typename Ty1, class _Allocator, class _temp_Allocator> welp::matrix<Ty2, _return_Allocator> eval(
			const std::function<welp::matrix<Ty2, _temp_Allocator>(Ty1)>& f, const welp::matrix<Ty1, _Allocator>& A);

	// returns matrix C such that row i of C = f(row i of A) for all i
	template <typename Ty2, class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty2>,
		typename Ty1, class _Allocator, class _temp_Allocator> welp::matrix<Ty2, _return_Allocator> eval(
			const std::function<welp::matrix<Ty2, _temp_Allocator>(welp::matrix<Ty1, _Allocator>)>& f, const welp::matrix<Ty1, _Allocator>& A);
#endif // WELP_MATRIX_INCLUDE_FUNCTIONAL

	namespace elem
	{
		// returns first index between elem_begin and elem_end of the element that satisfies predicate Pr
		template <typename Ty, class Predicate, class _Allocator> std::size_t find_first(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, std::size_t elem_begin, std::size_t elem_end);

		// returns first index of the element that satisfies predicate Pr
		template <typename Ty, class Predicate, class _Allocator> std::size_t find_first(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A);

		// returns last index between elem_begin and elem_end of the element that satisfies predicate Pr
		template <typename Ty, class Predicate, class _Allocator> std::size_t find_last(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, std::size_t elem_begin, std::size_t elem_end);

		// returns last index of the element that satisfies predicate Pr
		template <typename Ty, class Predicate, class _Allocator> std::size_t find_last(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A);

		// returns matrix of all the indices between elem_begin and elem_end of the elements that satisty predicate Pr
		// shrink_to_fit set to true will resize the memory capacity of the matrix to fit the data
		template <class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<std::size_t>, typename Ty, class Predicate, class _Allocator>
		welp::matrix<std::size_t, _return_Allocator> find_all(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A,
			std::size_t elem_begin, std::size_t elem_end, bool shrink_to_fit = true);

		// returns matrix of all the indices of the elements that satisty predicate Pr
		// shrink_to_fit set to true will resize the memory capacity of the matrix to fit the data
		template <class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<std::size_t>, typename Ty, class Predicate, class _Allocator>
		welp::matrix<std::size_t, _return_Allocator> find_all(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, bool shrink_to_fit = true);

		// returns matrix of all elements of indices between elem_begin and elem_end that satisty predicate Pr
		// shrink_to_fit set to true will resize the memory capacity of the matrix to fit the data
		template <typename Ty, class Predicate, class _Allocator> welp::matrix<Ty, _Allocator> select(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, std::size_t elem_begin, std::size_t elem_end, bool shrink_to_fit = true);

		// returns matrix of all elements that satisty predicate Pr
		// shrink_to_fit set to true will resize the memory capacity of the matrix to fit the data
		template <typename Ty, class Predicate, class _Allocator> welp::matrix<Ty, _Allocator> select(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, bool shrink_to_fit = true);

		// returns number of elements of indices between elem_begin and elem_end that satisty predicate Pr
		template <typename Ty, class Predicate, class _Allocator> std::size_t count(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, std::size_t elem_begin, std::size_t elem_end);

		// returns number of elements that satisty predicate Pr
		template <typename Ty, class Predicate, class _Allocator> std::size_t count(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A);
	}

	namespace row
	{
		// returns first index between row_begin and row_end of the row that satisfies predicate Pr
		template <typename Ty, class Predicate, class _Allocator>
		std::size_t find_first(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A,
			std::size_t row_begin, std::size_t row_end);

		// returns first index of the fow that satisfies predicate Pr
		template <typename Ty, class Predicate, class _Allocator>
		std::size_t find_first(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A);

		// returns last index between row_begin and row_end of the row that satisfies predicate Pr
		template <typename Ty, class Predicate, class _Allocator>
		std::size_t find_last(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A,
			std::size_t row_begin, std::size_t row_end);

		// returns last index of the fow that satisfies predicate Pr
		template <typename Ty, class Predicate, class _Allocator>
		std::size_t find_last(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A);

		// returns matrix of all the indices between elem_begin and elem_end of the rows that satisty predicate Pr
		// shrink_to_fit set to true will resize the memory capacity of the matrix to fit the data
		template <class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<std::size_t>, typename Ty, class Predicate, class _Allocator>
		welp::matrix<std::size_t, _return_Allocator> find_all(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A,
			std::size_t row_begin, std::size_t row_end, bool shrink_to_fit = true);

		// returns matrix of all the indices of the rows that satisty predicate Pr
		// shrink_to_fit set to true will resize the memory capacity of the matrix to fit the data
		template <class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<std::size_t>, typename Ty, class Predicate, class _Allocator>
		welp::matrix<std::size_t, _return_Allocator> find_all(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, bool shrink_to_fit = true);

		// returns matrix of all rows of indices between elem_begin and elem_end that satisty predicate Pr
		// shrink_to_fit set to true will resize the memory capacity of the matrix to fit the data
		template <typename Ty, class Predicate, class _Allocator>
		welp::matrix<Ty, _Allocator> select(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A,
			std::size_t row_begin, std::size_t row_end, bool shrink_to_fit = true);

		// returns matrix of all rows that satisty predicate Pr
		// shrink_to_fit set to true will resize the memory capacity of the matrix to fit the data
		template <typename Ty, class Predicate, class _Allocator>
		welp::matrix<Ty, _Allocator> select(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, bool shrink_to_fit = true);

		// returns number of rows of indices between elem_begin and elem_end that satisty predicate Pr
		template <typename Ty, class Predicate, class _Allocator>
		std::size_t count(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A,
			std::size_t row_begin, std::size_t row_end);

		// returns number of rows that satisty predicate Pr
		template <typename Ty, class Predicate, class _Allocator>
		std::size_t count(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A);
	}
}


////// textual matrices //////

#ifdef WELP_MATRIX_INCLUDE_STRING
namespace welp
{
	// parses column matrix A, with rows_max as the maximal amount of rows, skips the first rows_skipped rows of A
	// delim as the delimiter
	template <class _Allocator, class _string_Allocator>
	welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> parse(
		const welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator>& A,
		std::size_t rows_max, std::size_t rows_skipped, char delim);

	// parses column matrix A, with rows_max as the maximal amount of rows, skips the first rows_skipped rows of A
	// delim as the delimiter, will apply function f to every element of the matrix
	template <typename Ty, class _return_Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty>, class _Allocator, class _string_Allocator, class function_f>
	welp::matrix<Ty, _return_Allocator> parse(
		const welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator>& A,
		std::size_t rows_max, std::size_t rows_skipped, char delim, function_f f);

	// concatenates all the columns of matrix A into a single column matrix of strings with delim as the delimiter
	template <typename Ty, class _Allocator, class _string_Allocator>
	welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> concatenate(
		const welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator>& A, Ty delim);

	// welp::concatenate( A0 << A1 << ... << An, delim ) concatenates all the columns of matrices A0 << A1 << ... << An
	// into a single column matrix of strings with delim as the delimiter
	template <typename Ty, std::size_t length, class _Allocator, class _string_Allocator>
	welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> concatenate(
		const welp::matrix_stream<length, std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> S, Ty delim);

#ifdef WELP_MATRIX_INCLUDE_FSTREAM
	namespace file
	{
		// loads matrix from filename, with rows_max as the maximal amount of rows, skips the first rows_skipped rows at the beginning of the file,
		// shrink_to_fit set to true will resize the memory capacity of the matrix to fit the data
		template <class _string_Allocator = std::allocator<char>,
			class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<std::basic_string<char, std::char_traits<char>, _string_Allocator>>>
			welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> load(
				const std::string& filename, std::size_t rows_max, std::size_t rows_skipped);

#ifdef WELP_MATRIX_INCLUDE_ALGORITHM
		// loads matrix from filename, with rows_max as the maximal amount of rows, skips the first rows_skipped rows at the beginning of the file,
		// delim as the delimiter, shrink_to_fit set to true will resize the memory capacity of the matrix to fit the data
		template <class _string_Allocator = std::allocator<char>,
			class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<std::basic_string<char, std::char_traits<char>, _string_Allocator>>>
			welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> load(
				const std::string& filename, std::size_t rows_max, std::size_t rows_skipped, char delim);

		// loads matrix from filename, with rows_max as the maximal amount of rows, skips the first rows_skipped rows at the beginning of the file,
		// delim as the delimiter, shrink_to_fit set to true will resize the memory capacity of the matrix to fit the data,
		// will apply function f to every element of the matrix
		template <typename Ty, class _string_Allocator = std::allocator<char>,
			class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<Ty>, class function_f> welp::matrix<Ty, _Allocator> load(
				const std::string& filename, std::size_t rows_max, std::size_t rows_skipped,
				char delim, function_f f);
#endif // WELP_MATRIX_INCLUDE_ALGORITHM

		// writes A as file filename with delim as the delimiter
		template <typename Ty1, typename Ty2, class _Allocator_A> void write(
			const std::string& filename, const welp::matrix<Ty1, _Allocator_A>& A, Ty2 delim);

		// writes [A B] as file filename with delim as the delimiter
		template <typename Ty1, typename Ty2, typename Ty3, class _Allocator_A, class _Allocator_B> void write(
			const std::string& filename, const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B,
			const Ty3& delim);

		// writes [A B C] as file filename with delim as the delimiter
		template <typename Ty1, typename Ty2, typename Ty3, typename Ty4, class _Allocator_A, class _Allocator_B, class _Allocator_C>
		void write(const std::string& filename, const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B,
			const welp::matrix<Ty3, _Allocator_C>& C, const Ty4& delim);

		// writes [A B C D] as file filename with delim as the delimiter
		template <typename Ty1, typename Ty2, typename Ty3, typename Ty4, typename Ty5,
			class _Allocator_A, class _Allocator_B, class _Allocator_C, class _Allocator_D> void write(const std::string& filename,
				const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B,
				const welp::matrix<Ty3, _Allocator_C>& C, const welp::matrix<Ty4, _Allocator_D>& D, const Ty5& delim);

		// writes [A B C D E] as file filename with delim as the delimiter
		template <typename Ty1, typename Ty2, typename Ty3, typename Ty4, typename Ty5, typename Ty6,
			class _Allocator_A, class _Allocator_B, class _Allocator_C, class _Allocator_D, class _Allocator_E> void write(const std::string& filename,
				const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B, const welp::matrix<Ty3, _Allocator_C>& C,
				const welp::matrix<Ty4, _Allocator_D>& D, const welp::matrix<Ty5, _Allocator_E>& E, const Ty6& delim);

		// appends A to file filename with delim as the delimiter
		template <typename Ty1, typename Ty2, class _Allocator_A> void append(
			const std::string& filename, const welp::matrix<Ty1, _Allocator_A>& A, const Ty2& delim);

		// appends [A B] to file filename with delim as the delimiter
		template <typename Ty1, typename Ty2, typename Ty3, class _Allocator_A, class _Allocator_B> void append(
			const std::string& filename, const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B,
			const Ty3& delim);

		// appends [A B C] to file filename with delim as the delimiter
		template <typename Ty1, typename Ty2, typename Ty3, typename Ty4, class _Allocator_A, class _Allocator_B, class _Allocator_C>
		void append(const std::string& filename, const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B,
			const welp::matrix<Ty3, _Allocator_C>& C, const Ty4& delim);

		// appends [A B C D] to file filename with delim as the delimiter
		template <typename Ty1, typename Ty2, typename Ty3, typename Ty4, typename Ty5,
			class _Allocator_A, class _Allocator_B, class _Allocator_C, class _Allocator_D> void append(const std::string& filename,
				const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B,
				const welp::matrix<Ty3, _Allocator_C>& C, const welp::matrix<Ty4, _Allocator_D>& D, const Ty5& delim);

		// appends [A B C D E] to file filename with delim as the delimiter
		template <typename Ty1, typename Ty2, typename Ty3, typename Ty4, typename Ty5, typename Ty6,
			class _Allocator_A, class _Allocator_B, class _Allocator_C, class _Allocator_D, class _Allocator_E> void append(const std::string& filename,
				const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B, const welp::matrix<Ty3, _Allocator_C>& C,
				const welp::matrix<Ty4, _Allocator_D>& D, const welp::matrix<Ty4, _Allocator_E>& E, const Ty6& delim);

		std::string file_path(const std::string& str);
		std::string file_name(const std::string& str);
		std::string file_name_without_type(const std::string& str);
		std::string file_type(const std::string& str);
	}
#endif // WELP_MATRIX_INCLUDE_FSTREAM
}
#endif // WELP_MATRIX_INCLUDE_STRING



////////////////////////   I M P L E M E N T A T I O N S   ////////////////////////

////// SUBROUTINES //////

namespace welp
{
	namespace matrix_subroutines
	{
		template <typename Ty> inline void fill(Ty* const pfC, const Ty x, const std::size_t n)
		{
			std::size_t r = n & 3;
			Ty* pC = pfC;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC = x;
				*(pC + 1) = x;
				*(pC + 2) = x;
				*(pC + 3) = x;
				pC += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = x;
				break;

			case 2:
				*pC = x;
				*(pC + 1) = x;
				break;

			case 3:
				*pC = x;
				*(pC + 1) = x;
				*(pC + 2) = x;
				break;
			}
		}
		template <typename Ty> inline void cpy(Ty* const pfC, const Ty* const pfA, const std::size_t n)
		{
			std::size_t r = n & 3;
			Ty* pC = pfC; const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC = *pA;
				*(pC + 1) = *(pA + 1);
				*(pC + 2) = *(pA + 2);
				*(pC + 3) = *(pA + 3);
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = *pA;
				break;

			case 2:
				*pC = *pA;
				*(pC + 1) = *(pA + 1);
				break;

			case 3:
				*pC = *pA;
				*(pC + 1) = *(pA + 1);
				*(pC + 2) = *(pA + 2);
				break;
			}
		}
		inline void cpy(float* const pfC, const float* const pfA, const std::size_t n) noexcept
		{
			std::memcpy(pfC, pfA, n * sizeof(float));
		}
		inline void cpy(double* const pfC, const double* const pfA, const std::size_t n) noexcept
		{
			std::memcpy(pfC, pfA, n * sizeof(double));
		}
		template <typename Ty> inline void get_row(Ty* const pfC, const Ty* const pfA, const std::size_t i0, const std::size_t Ac)
		{
			Ty* pC = pfC; const Ty* pA = pfA + (Ac * i0);

			for (std::size_t k = Ac; k > 0; k--)
			{
				*pC++ = *pA++;
			}
		}
		template <> inline void get_row(float* const pfC, const float* const pfA, const std::size_t i0, const std::size_t Ac)
		{
			std::memcpy(pfC, pfA + (Ac * i0), Ac * sizeof(float));
		}
		template <> inline void get_row(double* const pfC, const double* const pfA, const std::size_t i0, const std::size_t Ac)
		{
			std::memcpy(pfC, pfA + (Ac * i0), Ac * sizeof(double));
		}
		template <typename Ty> inline void get_col(Ty* const pfC, const Ty* const pfA, const std::size_t j0,
			const std::size_t Ar, const std::size_t Ac)
		{
			Ty* pC = pfC; const Ty* pA = pfA + j0;
			for (std::size_t i = Ar; i > 0; i--) { *pC++ = *pA; pA += Ac; }
		}
		template <typename Ty> inline void get_blk(Ty* const pfC, const Ty* const pfA, const std::size_t i0, const std::size_t j0,
			const std::size_t Cr, const std::size_t Cc, const std::size_t Ac)
		{
			Ty* pC = pfC; const Ty* pA = (pfA + j0) + (Ac * i0);
			std::size_t mem = Ac - Cc;
			std::size_t j;
			for (std::size_t i = Cr; i > 0; i--)
			{
				for (j = Cc; j > 0; j--)
				{
					*pC++ = *pA++;
				}
				pA += mem;
			}
		}
		template <> inline void get_blk(float* const pfC, const float* const pfA, const std::size_t i0, const std::size_t j0,
			const std::size_t Cr, const std::size_t Cc, const std::size_t Ac)
		{
			float* pC = pfC; const float* pA = (pfA + j0) + (Ac * i0);
			std::size_t mem = Cc * sizeof(float);
			for (std::size_t i = Cr; i > 0; i--)
			{
				std::memcpy(pC, pA, mem);
				pA += Ac; pC += Cc;
			}
		}
		template <> inline void get_blk(double* const pfC, const double* const pfA, const std::size_t i0, const std::size_t j0,
			const std::size_t Cr, const std::size_t Cc, const std::size_t Ac)
		{
			double* pC = pfC; const double* pA = (pfA + j0) + (Ac * i0);
			std::size_t mem = Cc * sizeof(double);
			for (std::size_t i = Cr; i > 0; i--)
			{
				std::memcpy(pC, pA, mem);
				pA += Ac; pC += Cc;
			}
		}
		template <typename Ty> inline void insert(Ty* const pfC, const Ty* const pfA, const std::size_t i0, const std::size_t j0,
			const std::size_t Cc, const std::size_t Ar, const std::size_t Ac)
		{
			Ty* pC = (pfC + j0) + (Cc * i0); const Ty* pA = pfA;
			if (Ac == 1)
			{
				for (std::size_t i = Ar; i > 0; i--)
				{
					*pC = *pA++;
					pC += Cc;
				}
			}
			else
			{
				std::size_t memA = Cc - Ac;
				std::size_t j;
				for (std::size_t i = Ar; i > 0; i--)
				{
					for (j = Ac; j > 0; j--)
					{
						*pC++ = *pA++;
					}
					pC += memA;
				}
			}
		}
		template <> inline void insert(float* const pfC, const float* const pfA, const std::size_t i0, const std::size_t j0,
			const std::size_t Cc, const std::size_t Ar, const std::size_t Ac)
		{
			float* pC = (pfC + j0) + (Cc * i0); const float* pA = pfA;
			if (Ac == 1)
			{
				for (std::size_t i = Ar; i > 0; i--)
				{
					*pC = *pA++;
					pC += Cc;
				}
			}
			else
			{
				std::size_t memA = Ac * sizeof(float);
				for (std::size_t i = Ar; i > 0; i--)
				{
					std::memcpy(pC, pA, memA);
					pC += Cc; pA += Ac;
				}
			}
		}
		template <> inline void insert(double* const pfC, const double* const pfA, const std::size_t i0, const std::size_t j0,
			const std::size_t Cc, const std::size_t Ar, const std::size_t Ac)
		{
			double* pC = (pfC + j0) + (Cc * i0); const double* pA = pfA;
			if (Ac == 1)
			{
				for (std::size_t i = Ar; i > 0; i--)
				{
					*pC = *pA++;
					pC += Cc;
				}
			}
			else
			{
				std::size_t memA = Ac * sizeof(double);
				for (std::size_t i = Ar; i > 0; i--)
				{
					std::memcpy(pC, pA, memA);
					pC += Cc; pA += Ac;
				}
			}
		}
		template <typename Ty> inline void diag(Ty* const pfC, const Ty x, const std::size_t Cr,
			const std::size_t Cc, const std::size_t r_offset, const std::size_t c_offset) noexcept
		{
			std::size_t jump = Cc + 1;
			Ty* p = (pfC + c_offset) + (Cc * r_offset);
			for (std::size_t n = (Cr - r_offset < Cc - c_offset) ?
				Cr - r_offset : Cc - c_offset; n > 0; n--)
			{
				*p = x; p += jump;
			}
		}
		template <typename Ty> void adj(Ty* const pfC, const Ty* const pfA, const std::size_t Ar, const std::size_t Ac)
		{
			Ty* pC0; Ty* pC1; Ty* pC2; Ty* pC3; const Ty* pA;
			std::size_t N = Ac - (Ac & 3);

			for (std::size_t j = 0; j < N; j += 4)
			{
				pA = pfA + j;
				pC0 = pfC + (Ar * j);
				pC1 = pC0 + Ar;
				pC2 = pC0 + 2 * Ar;
				pC3 = pC0 + 3 * Ar;

				for (std::size_t i = 0; i < Ar; i++)
				{
					*pC0++ = *pA;
					*pC1++ = *(pA + 1);
					*pC2++ = *(pA + 2);
					*pC3++ = *(pA + 3);
					pA += Ac;
				}
			}

			switch (Ac & 3)
			{

			case 0:
				break;

			case 1:
				pA = pfA + N;
				pC0 = pfC + (Ar * N);
				for (std::size_t i = 0; i < Ar; i++)
				{
					*pC0++ = *pA;
					pA += Ac;
				}
				break;

			case 2:
				pA = pfA + N;
				pC0 = pfC + (Ar * N);
				pC1 = pC0 + Ar;
				for (std::size_t i = 0; i < Ar; i++)
				{
					*pC0++ = *pA;
					*pC1++ = *(pA + 1);
					pA += Ac;
				}
				break;

			case 3:
				pA = pfA + N;
				pC0 = pfC + (Ar * N);
				pC1 = pC0 + Ar;
				pC2 = pC0 + 2 * Ar;
				for (std::size_t i = 0; i < Ar; i++)
				{
					*pC0++ = *pA;
					*pC1++ = *(pA + 1);
					*pC2++ = *(pA + 2);
					pA += Ac;
				}
				break;
			}
		}
		template <typename Ty> void adj_sqm(Ty* const pfC, const std::size_t Cc)
		{
			Ty temp0; Ty temp1; Ty temp2; Ty temp3;
			Ty* p; Ty* q0; Ty* q1; Ty* q2; Ty* q3;
			std::size_t N;
			std::size_t M = 4 * Cc;
			std::size_t i, j;

			for (i = 0; i < Cc; i++)
			{
				p = (pfC + i + 1) + (Cc * i);
				q0 = (pfC + i) + (Cc * (i + 1));
				q1 = q0 + Cc;
				q2 = q0 + 2 * Cc;
				q3 = q0 + 3 * Cc;

				for (j = i + 1; j < Cc; j += 4)
				{
					N = (Cc - j < 4) ? Cc - j : 4;

					switch (N)
					{

					case 4:
						temp0 = *p;
						temp1 = *(p + 1);
						temp2 = *(p + 2);
						temp3 = *(p + 3);
						*p = *q0;
						*(p + 1) = *q1;
						*(p + 2) = *q2;
						*(p + 3) = *q3;
						*q0 = temp0;
						*q1 = temp1;
						*q2 = temp2;
						*q3 = temp3;

						p += 4;
						q0 += M;
						q1 += M;
						q2 += M;
						q3 += M;
						break;

					case 3:
						temp0 = *p;
						temp1 = *(p + 1);
						temp2 = *(p + 2);
						*p = *q0;
						*(p + 1) = *q1;
						*(p + 2) = *q2;
						*q0 = temp0;
						*q1 = temp1;
						*q2 = temp2;
						break;

					case 2:
						temp0 = *p;
						temp1 = *(p + 1);
						*p = *q0;
						*(p + 1) = *q1;
						*q0 = temp0;
						*q1 = temp1;
						break;

					case 1:
						temp0 = *p;
						*p = *q0;
						*q0 = temp0;
						break;

					case 0:
						break;
					}
				}
			}
		}
		template <typename Ty> inline void _m(Ty* const pfC, const Ty* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			Ty* pC = pfC; const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC = -(*pA);
				*(pC + 1) = -(*(pA + 1));
				*(pC + 2) = -(*(pA + 2));
				*(pC + 3) = -(*(pA + 3));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = -(*pA);
				break;

			case 2:
				*pC = -(*pA);
				*(pC + 1) = -(*(pA + 1));
				break;

			case 3:
				*pC = -(*pA);
				*(pC + 1) = -(*(pA + 1));
				*(pC + 2) = -(*(pA + 2));
				break;
			}
		}
		template <typename Ty> inline void ps(Ty* const pfC, const Ty x, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			Ty* pC = pfC;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC += x;
				*(pC + 1) += x;
				*(pC + 2) += x;
				*(pC + 3) += x;
				pC += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC += x;
				break;

			case 2:
				*pC += x;
				*(pC + 1) += x;
				break;

			case 3:
				*pC += x;
				*(pC + 1) += x;
				*(pC + 2) += x;
				break;
			}
		}
		template <typename Ty> inline void spm(Ty* const pfC, const Ty x, const Ty* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			Ty* pC = pfC; const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC = x + (*pA);
				*(pC + 1) = x + (*(pA + 1));
				*(pC + 2) = x + (*(pA + 2));
				*(pC + 3) = x + (*(pA + 3));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = x + (*pA);
				break;

			case 2:
				*pC += x + (*pA);
				*(pC + 1) = x + (*(pA + 1));
				break;

			case 3:
				*pC = x + (*pA);
				*(pC + 1) = x + (*(pA + 1));
				*(pC + 2) = x + (*(pA + 2));
				break;
			}
		}
		template <typename Ty> inline void sp_m(Ty* const pfC, const Ty x, const Ty* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			Ty* pC = pfC; const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC = x - (*pA);
				*(pC + 1) = x - (*(pA + 1));
				*(pC + 2) = x - (*(pA + 2));
				*(pC + 3) = x - (*(pA + 3));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = x - (*pA);
				break;

			case 2:
				*pC += x - (*pA);
				*(pC + 1) = x - (*(pA + 1));
				break;

			case 3:
				*pC = x - (*pA);
				*(pC + 1) = x - (*(pA + 1));
				*(pC + 2) = x - (*(pA + 2));
				break;
			}
		}
		template <typename Ty> inline void pm(Ty* const pfC, const Ty* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			Ty* pC = pfC; const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC += (*pA);
				*(pC + 1) += (*(pA + 1));
				*(pC + 2) += (*(pA + 2));
				*(pC + 3) += (*(pA + 3));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC += (*pA);
				break;

			case 2:
				*pC += (*pA);
				*(pC + 1) += (*(pA + 1));
				break;

			case 3:
				*pC += (*pA);
				*(pC + 1) += (*(pA + 1));
				*(pC + 2) += (*(pA + 2));
				break;
			}
		}
		template <typename Ty> inline void p_m(Ty* const pfC, const Ty* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			Ty* pC = pfC; const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC -= (*pA);
				*(pC + 1) -= (*(pA + 1));
				*(pC + 2) -= (*(pA + 2));
				*(pC + 3) -= (*(pA + 3));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC -= (*pA);
				break;

			case 2:
				*pC -= (*pA);
				*(pC + 1) -= (*(pA + 1));
				break;

			case 3:
				*pC -= (*pA);
				*(pC + 1) -= (*(pA + 1));
				*(pC + 2) -= (*(pA + 2));
				break;
			}
		}
		template <typename Ty> inline void mpm(Ty* const pfC, const Ty* const pfA, const Ty* const pfB, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			Ty* pC = pfC; const Ty* pA = pfA; const Ty* pB = pfB;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC = (*pA) + (*pB);
				*(pC + 1) = (*(pA + 1)) + (*(pB + 1));
				*(pC + 2) = (*(pA + 2)) + (*(pB + 2));
				*(pC + 3) = (*(pA + 3)) + (*(pB + 3));
				pC += 4; pA += 4; pB += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = (*pA) + (*pB);
				break;

			case 2:
				*pC = (*pA) + (*pB);
				*(pC + 1) = (*(pA + 1)) + (*(pB + 1));
				break;

			case 3:
				*pC = (*pA) + (*pB);
				*(pC + 1) = (*(pA + 1)) + (*(pB + 1));
				*(pC + 2) = (*(pA + 2)) + (*(pB + 2));
				break;
			}
		}
		template <typename Ty> inline void mp_m(Ty* const pfC, const Ty* const pfA, const Ty* const pfB, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			Ty* pC = pfC; const Ty* pA = pfA; const Ty* pB = pfB;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC = (*pA) - (*pB);
				*(pC + 1) = (*(pA + 1)) - (*(pB + 1));
				*(pC + 2) = (*(pA + 2)) - (*(pB + 2));
				*(pC + 3) = (*(pA + 3)) - (*(pB + 3));
				pC += 4; pA += 4; pB += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = (*pA) - (*pB);
				break;

			case 2:
				*pC = (*pA) - (*pB);
				*(pC + 1) = (*(pA + 1)) - (*(pB + 1));
				break;

			case 3:
				*pC = (*pA) - (*pB);
				*(pC + 1) = (*(pA + 1)) - (*(pB + 1));
				*(pC + 2) = (*(pA + 2)) - (*(pB + 2));
				break;
			}
		}
		template <typename Ty> inline void xs(Ty* const pfC, const Ty x, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			Ty* pC = pfC;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC *= x;
				*(pC + 1) *= x;
				*(pC + 2) *= x;
				*(pC + 3) *= x;
				pC += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC *= x;
				break;

			case 2:
				*pC *= x;
				*(pC + 1) *= x;
				break;

			case 3:
				*pC *= x;
				*(pC + 1) *= x;
				*(pC + 2) *= x;
				break;
			}
		}
		template <typename Ty> inline void sxm(Ty* const pfC, const Ty x, const Ty* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			Ty* pC = pfC; const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC = x * (*pA);
				*(pC + 1) = x * (*(pA + 1));
				*(pC + 2) = x * (*(pA + 2));
				*(pC + 3) = x * (*(pA + 3));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = x * (*pA);
				break;

			case 2:
				*pC = x * (*pA);
				*(pC + 1) = x * (*(pA + 1));
				break;

			case 3:
				*pC = x * (*pA);
				*(pC + 1) = x * (*(pA + 1));
				*(pC + 2) = x * (*(pA + 2));
				break;
			}
		}
		template <typename Ty> inline void psxm(Ty* const pfC, const Ty x, const Ty* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			Ty* pC = pfC; const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				*pC += x * (*pA);
				*(pC + 1) += x * (*(pA + 1));
				*(pC + 2) += x * (*(pA + 2));
				*(pC + 3) += x * (*(pA + 3));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC += x * (*pA);
				break;

			case 2:
				*pC += x * (*pA);
				*(pC + 1) += x * (*(pA + 1));
				break;

			case 3:
				*pC += x * (*pA);
				*(pC + 1) += x * (*(pA + 1));
				*(pC + 2) += x * (*(pA + 2));
				break;
			}
		}
		template <typename Ty> inline Ty dot(const Ty* const pfA, const Ty* const pfB, const std::size_t n) noexcept
		{
			Ty acc0 = static_cast<Ty>(0); Ty acc1 = static_cast<Ty>(0);
			const Ty* pA = pfA; const Ty* pB = pfB;
			std::size_t r = n & 1;

			for (std::size_t k = n - r; k > 0; k -= 2)
			{
				acc0 += (*pA) * (*pB);
				acc1 += (*(pA + 1)) * (*(pB + 1));
				pA += 2; pB += 2;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				acc0 += (*pA) * (*pB);
				break;
			}

			return acc0 + acc1;
		}
		template <typename Ty> inline Ty norm2(const Ty* const pfA, const std::size_t n) noexcept
		{
			Ty acc0 = static_cast<Ty>(0); Ty acc1 = static_cast<Ty>(0);
			const Ty* pA = pfA;
			std::size_t r = n & 1;

			for (std::size_t k = n - r; k > 0; k -= 2)
			{
				acc0 += (*pA) * (*pA);
				acc1 += (*(pA + 1)) * (*(pA + 1));
				pA += 2;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				acc0 += (*pA) * (*pA);
				break;
			}

			return acc0 + acc1;
		}
		template <typename Ty> void pmxm(Ty* const pfC, const Ty* const pfA, const Ty* const pfB,
			const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept
		{
			if (Bc == 1)
			{
				Ty acc0; Ty acc1; Ty acc2; Ty acc3; Ty regB;
				const Ty* pA0 = pfA;
				const Ty* pA1 = pfA + Ac;
				const Ty* pA2 = pfA + 2 * Ac;
				const Ty* pA3 = pfA + 3 * Ac;
				const Ty* pB; Ty* pC = pfC;
				std::size_t N = Ar - (Ar & 3);
				std::size_t jump = 3 * Ac;

				std::size_t i, k;

				// major upper part of C
				for (i = N; i > 0; i -= 4)
				{
					acc0 = static_cast<Ty>(0);
					acc1 = static_cast<Ty>(0);
					acc2 = static_cast<Ty>(0);
					acc3 = static_cast<Ty>(0);
					pB = pfB;

					for (k = Ac; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
						acc3 += (*pA3++) * regB;
					}
					*pC += acc0;
					*(pC + 1) += acc1;
					*(pC + 2) += acc2;
					*(pC + 3) += acc3;
					pC += 4;

					pA0 += jump;
					pA1 += jump;
					pA2 += jump;
					pA3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					acc0 = static_cast<Ty>(0);
					pB = pfB;

					for (k = Ac; k > 0; k--)
					{
						acc0 += (*pA0++) * (*pB++);
					}
					*pC += acc0;
					break;

				case 2:
					acc0 = static_cast<Ty>(0);
					acc1 = static_cast<Ty>(0);
					pB = pfB;

					for (k = Ac; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
					}
					*pC += acc0;
					*(pC + 1) += acc1;
					break;

				case 3:
					acc0 = static_cast<Ty>(0);
					acc1 = static_cast<Ty>(0);
					acc2 = static_cast<Ty>(0);
					pB = pfB;

					for (k = Ac; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
					}
					*pC += acc0;
					*(pC + 1) += acc1;
					*(pC + 2) += acc2;
					break;
				}
				return;
			}

			else if (Ar == 1)
			{
				Ty regA0; Ty regA1; Ty regA2; Ty regA3;
				const Ty* pA = pfA;
				const Ty* pB0 = pfB;
				const Ty* pB1 = pfB + Bc;
				const Ty* pB2 = pfB + 2 * Bc;
				const Ty* pB3 = pfB + 3 * Bc;
				Ty* pC;
				std::size_t N = Ac - (Ac & 3);
				std::size_t jump = 3 * Bc;

				std::size_t j, k;

				// major upper part of B
				for (k = N; k > 0; k -= 4)
				{
					pC = pfC;

					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc; j > 0; j--)
					{
						*pC++ += (regA0 * (*pB0++) + regA1 * (*pB1++))
							+ (regA2 * (*pB2++) + regA3 * (*pB3++));
					}
					pB0 += jump;
					pB1 += jump;
					pB2 += jump;
					pB3 += jump;
				}

				// bottom fringe of B
				switch (Ac & 3)
				{

				case 0:
					break;

				case 1:
					pC = pfC;
					regA0 = *pA;

					for (j = Bc; j > 0; j--)
					{
						*pC++ += regA0 * (*pB0++);
					}
					break;

				case 2:
					pC = pfC;
					regA0 = *pA;
					regA1 = *(pA + 1);

					for (j = Bc; j > 0; j--)
					{
						*pC++ += regA0 * (*pB0++) + regA1 * (*pB1++);
					}
					break;

				case 3:
					pC = pfC;
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);

					for (j = Bc; j > 0; j--)
					{
						*pC++ += (regA0 * (*pB0++) + regA1 * (*pB1++)) + regA2 * (*pB2++);
					}
					break;
				}
				return;
			}

			// if A and B are full matrices
			else
			{
				Ty regA0; Ty regA1; Ty regA2; Ty regA3; Ty regB;
				const Ty* pA; const Ty* pB; Ty* pC0; Ty* pC1; Ty* pC2; Ty* pC3;
				std::size_t N = Ar - (Ar & 3);

				std::size_t i, j, k;

				// major upper part of C
				for (i = 0; i < N; i += 4)
				{
					for (k = 0; k < Ac; k++)
					{
						pB = pfB + (Bc * k);

						pC0 = pfC + (Bc * i);
						pC1 = pC0 + Bc;
						pC2 = pC0 + 2 * Bc;
						pC3 = pC0 + 3 * Bc;

						pA = (pfA + k) + (Ac * i);
						regA0 = *pA;
						regA1 = *(pA + Ac);
						regA2 = *(pA + 2 * Ac);
						regA3 = *(pA + 3 * Ac);

						for (j = Bc; j > 0; j--)
						{
							regB = *pB++;
							*pC0++ += regA0 * regB;
							*pC1++ += regA1 * regB;
							*pC2++ += regA2 * regB;
							*pC3++ += regA3 * regB;
						}
					}
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					for (k = 0; k < Ac; k++)
					{
						pB = pfB + (Bc * k);
						pC0 = pfC + (Bc * N);
						regA0 = *((pfA + k) + (Ac * N));

						for (j = Bc; j > 0; j--)
						{
							*pC0++ += regA0 * (*pB++);
						}
					}
					break;

				case 2:
					for (k = 0; k < Ac; k++)
					{
						pB = pfB + (Bc * k);

						pC0 = pfC + (Bc * N);
						pC1 = pC0 + Bc;

						pA = (pfA + k) + (Ac * N);
						regA0 = *pA;
						regA1 = *(pA + Ac);

						for (j = Bc; j > 0; j--)
						{
							regB = *pB++;
							*pC0++ += regA0 * regB;
							*pC1++ += regA1 * regB;
						}
					}
					break;

				case 3:
					for (k = 0; k < Ac; k++)
					{
						pB = pfB + (Bc * k);

						pC0 = pfC + (Bc * N);
						pC1 = pC0 + Bc;
						pC2 = pC0 + 2 * Bc;

						pA = (pfA + k) + (Ac * N);
						regA0 = *pA;
						regA1 = *(pA + Ac);
						regA2 = *(pA + 2 * Ac);

						for (j = Bc; j > 0; j--)
						{
							regB = *pB++;
							*pC0++ += regA0 * regB;
							*pC1++ += regA1 * regB;
							*pC2++ += regA2 * regB;
						}
					}
					break;
				}
			}
		}
		template <typename Ty> void p_mxm(Ty* const pfC, const Ty* const pfA, const Ty* const pfB,
			const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept
		{
			if (Bc == 1)
			{
				Ty acc0; Ty acc1; Ty acc2; Ty acc3; Ty regB;
				const Ty* pA0 = pfA;
				const Ty* pA1 = pfA + Ac;
				const Ty* pA2 = pfA + 2 * Ac;
				const Ty* pA3 = pfA + 3 * Ac;
				const Ty* pB; Ty* pC = pfC;
				std::size_t N = Ar - (Ar & 3);
				std::size_t jump = 3 * Ac;

				std::size_t i, k;

				// major upper part of C
				for (i = N; i > 0; i -= 4)
				{
					acc0 = static_cast<Ty>(0);
					acc1 = static_cast<Ty>(0);
					acc2 = static_cast<Ty>(0);
					acc3 = static_cast<Ty>(0);
					pB = pfB;

					for (k = Ac; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
						acc3 += (*pA3++) * regB;
					}
					*pC -= acc0;
					*(pC + 1) -= acc1;
					*(pC + 2) -= acc2;
					*(pC + 3) -= acc3;
					pC += 4;

					pA0 += jump;
					pA1 += jump;
					pA2 += jump;
					pA3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					acc0 = static_cast<Ty>(0);
					pB = pfB;

					for (k = Ac; k > 0; k--)
					{
						acc0 += (*pA0++) * (*pB++);
					}
					*pC -= acc0;
					break;

				case 2:
					acc0 = static_cast<Ty>(0);
					acc1 = static_cast<Ty>(0);
					pB = pfB;

					for (k = Ac; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
					}
					*pC -= acc0;
					*(pC + 1) -= acc1;
					break;

				case 3:
					acc0 = static_cast<Ty>(0);
					acc1 = static_cast<Ty>(0);
					acc2 = static_cast<Ty>(0);
					pB = pfB;

					for (k = Ac; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
					}
					*pC -= acc0;
					*(pC + 1) -= acc1;
					*(pC + 2) -= acc2;
					break;
				}
				return;
			}

			else if (Ar == 1)
			{
				Ty regA0; Ty regA1; Ty regA2; Ty regA3;
				const Ty* pA = pfA;
				const Ty* pB0 = pfB;
				const Ty* pB1 = pfB + Bc;
				const Ty* pB2 = pfB + 2 * Bc;
				const Ty* pB3 = pfB + 3 * Bc;
				Ty* pC;
				std::size_t N = Ac - (Ac & 3);
				std::size_t jump = 3 * Bc;

				std::size_t j, k;

				// major upper part of B
				for (k = N; k > 0; k -= 4)
				{
					pC = pfC;

					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc; j > 0; j--)
					{
						*pC++ -= (regA0 * (*pB0++) + regA1 * (*pB1++))
							+ (regA2 * (*pB2++) + regA3 * (*pB3++));
					}
					pB0 += jump;
					pB1 += jump;
					pB2 += jump;
					pB3 += jump;
				}

				// bottom fringe of B
				switch (Ac & 3)
				{

				case 0:
					break;

				case 1:
					pC = pfC;
					regA0 = *pA;

					for (j = Bc; j > 0; j--)
					{
						*pC++ -= regA0 * (*pB0++);
					}
					break;

				case 2:
					pC = pfC;
					regA0 = *pA;
					regA1 = *(pA + 1);

					for (j = Bc; j > 0; j--)
					{
						*pC++ -= regA0 * (*pB0++) + regA1 * (*pB1++);
					}
					break;

				case 3:
					pC = pfC;
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);

					for (j = Bc; j > 0; j--)
					{
						*pC++ -= (regA0 * (*pB0++) + regA1 * (*pB1++)) + regA2 * (*pB2++);
					}
					break;
				}
				return;
			}

			// if A and B are full matrices
			else
			{
				Ty regA0; Ty regA1; Ty regA2; Ty regA3; Ty regB;
				const Ty* pA; const Ty* pB; Ty* pC0; Ty* pC1; Ty* pC2; Ty* pC3;
				std::size_t N = Ar - (Ar & 3);

				std::size_t i, j, k;

				// major upper part of C
				for (i = 0; i < N; i += 4)
				{
					for (k = 0; k < Ac; k++)
					{
						pB = pfB + (Bc * k);

						pC0 = pfC + (Bc * i);
						pC1 = pC0 + Bc;
						pC2 = pC0 + 2 * Bc;
						pC3 = pC0 + 3 * Bc;

						pA = pfA + (Ac * i);
						regA0 = *pA;
						regA1 = *(pA + Ac);
						regA2 = *(pA + 2 * Ac);
						regA3 = *(pA + 3 * Ac);

						for (j = Bc; j > 0; j--)
						{
							regB = *pB++;
							*pC0++ -= regA0 * regB;
							*pC1++ -= regA1 * regB;
							*pC2++ -= regA2 * regB;
							*pC3++ -= regA3 * regB;
						}
					}
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					for (k = 0; k < Ac; k++)
					{
						pB = pfB + (Bc * k);
						pC0 = pfC + (Bc * N);
						regA0 = *((pfA + k) + (Ac * N));

						for (j = Bc; j > 0; j--)
						{
							*pC0++ -= regA0 * (*pB++);
						}
					}
					break;

				case 2:
					for (k = 0; k < Ac; k++)
					{
						pB = pfB + (Bc * k);

						pC0 = pfC + (Bc * N);
						pC1 = pC0 + Bc;

						pA = (pfA + k) + (Ac * N);
						regA0 = *pA;
						regA1 = *(pA + Ac);

						for (j = Bc; j > 0; j--)
						{
							regB = *pB++;
							*pC0++ -= regA0 * regB;
							*pC1++ -= regA1 * regB;
						}
					}
					break;

				case 3:
					for (k = 0; k < Ac; k++)
					{
						pB = pfB + (Bc * k);

						pC0 = pfC + (Bc * N);
						pC1 = pC0 + Bc;
						pC2 = pC0 + 2 * Bc;

						pA = (pfA + k) + (Ac * N);
						regA0 = *pA;
						regA1 = *(pA + Ac);
						regA2 = *(pA + 2 * Ac);

						for (j = Bc; j > 0; j--)
						{
							regB = *pB++;
							*pC0++ -= regA0 * regB;
							*pC1++ -= regA1 * regB;
							*pC2++ -= regA2 * regB;
						}
					}
					break;
				}
			}
		}
		template <typename Ty> inline void pdiag(Ty* const pfC, const Ty x, const std::size_t Cr,
			const std::size_t Cc, const std::size_t r_offset, const std::size_t c_offset) noexcept
		{
			std::size_t jump = Cc + 1;
			Ty* p = (pfC + c_offset) + (Cc * r_offset);
			for (std::size_t n = (Cr - r_offset < Cc - c_offset) ?
				Cr - r_offset : Cc - c_offset; n > 0; n--)
			{
				*p += x; p += jump;
			}
		}
		template <typename Ty> void elim_gauss(Ty* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t) noexcept
		{
			Ty* p; Ty* q0; Ty* q1; Ty* q2; Ty* q3;
			Ty reg0; Ty reg1; Ty reg2; Ty reg3; Ty reg;
			Ty maxelem; Ty temp; bool pivot;
			std::size_t ndiag = (Ar - 1 < Ac) ? Ar - 1 : Ac;
			std::size_t ur; std::size_t  M; std::size_t jump;
			std::size_t i, ii, jj;

			for (i = 0; i < ndiag; i++)
			{
				ur = Ar - 1 - i;
				p = (pfA + i) + (Ac * i);
				q0 = p + Ac;
				q1 = p + Ac;
				maxelem = std::abs(*p);
				pivot = false;
				for (jj = 0; jj < ur; jj++)
				{
					temp = std::abs(*q1);
					if (temp > maxelem)
					{
						pivot = true;
						maxelem = temp;
						q0 = q1;
					}
					q1 += Ac;
				}

				M = Ac - i;

				if (pivot)
				{
					for (jj = M - (M & 3); jj > 0; jj -= 4)
					{
						reg0 = *p;
						reg1 = *(p + 1);
						reg2 = *(p + 2);
						reg3 = *(p + 3);

						std::memcpy(p, q0, 4 * sizeof(Ty));

						*q0 = reg0;
						*(q0 + 1) = reg1;
						*(q0 + 2) = reg2;
						*(q0 + 3) = reg3;

						p += 4; q0 += 4;
					}
					for (jj = M & 3; jj > 0; jj--)
					{
						temp = *p;
						*p = *q0;
						*q0 = temp;
						p++; q0++;
					}
				}

				p = (pfA + i) + (Ac * i);

				if (*p != static_cast<Ty>(0))
				{
					temp = (static_cast<Ty>(1)) / *p;
					q0 = p + Ac;
					q1 = p + 2 * Ac;
					q2 = p + 3 * Ac;
					q3 = p + 4 * Ac;
					jump = i + 3 * Ac;
					for (ii = ur - (ur & 3); ii > 0; ii -= 4)
					{
						p = (pfA + i) + (Ac * i);
						reg0 = temp * (*q0);
						reg1 = temp * (*q1);
						reg2 = temp * (*q2);
						reg3 = temp * (*q3);
						for (jj = M; jj > 0; jj--)
						{
							reg = *p++;
							*q0++ -= reg0 * reg;
							*q1++ -= reg1 * reg;
							*q2++ -= reg2 * reg;
							*q3++ -= reg3 * reg;
						}
						q0 += jump;
						q1 += jump;
						q2 += jump;
						q3 += jump;
					}

					switch (ur & 3)
					{

					case 0:
						break;

					case 1:
						p = (pfA + i) + (Ac * i);
						reg0 = temp * (*q0);
						for (jj = M; jj > 0; jj--)
						{
							*q0++ -= reg0 * (*p++);
						}
						break;

					case 2:
						p = (pfA + i) + (Ac * i);
						reg0 = temp * (*q0);
						reg1 = temp * (*q1);
						for (jj = M; jj > 0; jj--)
						{
							reg = *p++;
							*q0++ -= reg0 * reg;
							*q1++ -= reg1 * reg;
						}
						break;

					case 3:
						p = (pfA + i) + (Ac * i);
						reg0 = temp * (*q0);
						reg1 = temp * (*q1);
						reg2 = temp * (*q2);
						for (jj = M; jj > 0; jj--)
						{
							reg = *p++;
							*q0++ -= reg0 * reg;
							*q1++ -= reg1 * reg;
							*q2++ -= reg2 * reg;
						}
						break;
					}
				}
			}

			ndiag = (Ar < Ac) ? Ar : Ac;
			p = pfA + Ac;
			for (i = 1; i < ndiag; i++)
			{
				for (ii = i; ii > 0; ii--)
				{
					*p++ = static_cast<Ty>(0);
				}
				p += Ac - i;
			}
			if (Ar > Ac)
			{
				for (i = Ac * (Ar - Ac); i > 0; i--)
				{
					*p++ = static_cast<Ty>(0);
				}
			}
		}
		template <typename Ty> void elim_householder(Ty* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t Nc,
			Ty* const pfu, Ty* const pfv, const std::size_t) noexcept
		{
			Ty reg0; Ty reg1; Ty reg2; Ty reg3; Ty duu; Ty regv;
			Ty* pA0; Ty* pA1; Ty* pA2; Ty* pA3; Ty* pu; Ty* pv;
			std::size_t ndiag = (Ar - 1 < Nc) ? Ar - 1 : Nc;
			std::size_t N; std::size_t ur = Ar; std::size_t jump;
			std::size_t i, ii, jj;

			for (i = 0; i < ndiag; i++)
			{
				pA0 = (pfA + i) + (Ac * i);
				pu = pfu;
				for (ii = ur; ii > 0; ii--)
				{
					*pu++ = *pA0; pA0 += Ac;
				}

				duu = static_cast<Ty>(0);
				pu = pfu;
				for (ii = ur; ii > 0; ii--)
				{
					duu += (*pu) * (*pu); pu++;
				}

				pu = pfu;
				if (*pu > static_cast<Ty>(0)) { *pu += std::sqrt(duu); }
				else { *pu -= std::sqrt(duu); }

				duu = static_cast<Ty>(0);
				for (ii = ur; ii > 0; ii--)
				{
					duu += (*pu) * (*pu); pu++;
				}

				if (duu != static_cast<Ty>(0))
				{
					duu = (-static_cast<Ty>(2) / duu);

					pv = pfv;
					for (ii = Ac; ii > 0; ii--)
					{
						*pv++ = static_cast<Ty>(0);
					}
					pA0 = (pfA + i) + (Ac * i);
					pA1 = pA0 + Ac;
					pA2 = pA0 + 2 * Ac;
					pA3 = pA0 + 3 * Ac;
					pu = pfu;
					N = Ar - i;
					jump = i + 3 * Ac;
					for (ii = N - (N & 3); ii > 0; ii -= 4)
					{
						pv = pfv;
						reg0 = *pu;
						reg1 = *(pu + 1);
						reg2 = *(pu + 2);
						reg3 = *(pu + 3);
						pu += 4;
						for (jj = Ac - i; jj > 0; jj--)
						{
							*pv++ += (reg0 * (*pA0++) + reg1 * (*pA1++))
								+ (reg2 * (*pA2++) + reg3 * (*pA3++));
						}
						pA0 += jump;
						pA1 += jump;
						pA2 += jump;
						pA3 += jump;
					}

					switch (N & 3)
					{

					case 0:
						break;

					case 1:
						pv = pfv;
						reg0 = *pu++;
						for (jj = Ac - i; jj > 0; jj--)
						{
							*pv++ += reg0 * (*pA0++);
						}
						break;

					case 2:
						pv = pfv;
						reg0 = *pu;
						reg1 = *(pu + 1);
						for (jj = Ac - i; jj > 0; jj--)
						{
							*pv++ += (reg0 * (*pA0++) + reg1 * (*pA1++));
						}
						break;

					case 3:
						pv = pfv;
						reg0 = *pu;
						reg1 = *(pu + 1);
						reg2 = *(pu + 2);
						for (jj = Ac - i; jj > 0; jj--)
						{
							*pv++ += (reg0 * (*pA0++) + reg1 * (*pA1++))
								+ (reg2 * (*pA2++));
						}
						break;
					}

					pv = pfv;
					for (ii = Ac - i; ii > 0; ii--)
					{
						*pv++ *= duu;
					}

					pA0 = (pfA + i) + (Ac * i);
					pA1 = pA0 + Ac;
					pA2 = pA0 + 2 * Ac;
					pA3 = pA0 + 3 * Ac;
					pu = pfu;
					N = Ar - i;
					jump = i + 3 * Ac;
					for (ii = N - (N & 3); ii > 0; ii -= 4)
					{
						pv = pfv;
						reg0 = *pu;
						reg1 = *(pu + 1);
						reg2 = *(pu + 2);
						reg3 = *(pu + 3);
						pu += 4;
						for (jj = Ac - i; jj > 0; jj--)
						{
							regv = *pv++;
							*pA0++ += reg0 * regv;
							*pA1++ += reg1 * regv;
							*pA2++ += reg2 * regv;
							*pA3++ += reg3 * regv;
						}
						pA0 += jump;
						pA1 += jump;
						pA2 += jump;
						pA3 += jump;
					}

					switch (N & 3)
					{

					case 0:
						break;

					case 1:
						pv = pfv;
						reg0 = *pu++;
						for (jj = Ac - i; jj > 0; jj--)
						{
							*pA0++ += reg0 * (*pv++);
						}
						break;

					case 2:
						pv = pfv;
						reg0 = *pu;
						reg1 = *(pu + 1);
						for (jj = Ac - i; jj > 0; jj--)
						{
							regv = *pv++;
							*pA0++ += reg0 * regv;
							*pA1++ += reg1 * regv;
						}
						break;

					case 3:
						pv = pfv;
						reg0 = *pu;
						reg1 = *(pu + 1);
						reg2 = *(pu + 2);
						for (jj = Ac - i; jj > 0; jj--)
						{
							regv = *pv++;
							*pA0++ += reg0 * regv;
							*pA1++ += reg1 * regv;
							*pA2++ += reg2 * regv;
						}
						break;
					}
				}
				ur--;
			}

			ndiag = (Ar < Nc) ? Ar : Nc;
			pA0 = pfA + Ac;
			for (i = 1; i < ndiag; i++)
			{
				for (ii = i; ii > 0; ii--)
				{
					*pA0++ = static_cast<Ty>(0);
				}
				pA0 += Ac - i;
			}
			if (Ar > Nc)
			{
				for (i = Ar - Nc; i > 0; i--)
				{
					for (ii = ndiag; ii > 0; ii--)
					{
						*pA0++ = static_cast<Ty>(0);
					}
					pA0 += Ac - ndiag;
				}
			}
		}
		template <typename Ty> void elim_givens(Ty* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t Nc, const std::size_t) noexcept
		{
			Ty C; Ty S; Ty temp0; Ty temp1; Ty temp2; Ty temp3;
			Ty* p; Ty* q;
			std::size_t N; std::size_t ndiag = (Ar - 1 < Nc) ? Ar - 1 : Nc;
			std::size_t i, j, jj;

			for (j = 0; j < ndiag; j++)
			{
				for (i = Ar - 1; i > j; i--)
				{
					q = (pfA + j) + (Ac * i);
					if (*q != static_cast<Ty>(0))
					{
						p = q - Ac;

						C = *p;
						S = *q;
						temp0 = static_cast<Ty>(1) / std::sqrt(C * C + S * S);
						C *= temp0;
						S *= temp0;

						temp0 = *q;
						*q = static_cast<Ty>(0);
						*p = S * temp0 + C * (*p);
						q++; p++;
						N = Ac - (j + 1);

						for (jj = N - (N & 3); jj > 0; jj -= 4)
						{
							temp0 = *p;
							temp1 = *(p + 1);
							temp2 = *(p + 2);
							temp3 = *(p + 3);

							*p = C * temp0 + S * (*q);
							*(p + 1) = C * temp1 + S * (*(q + 1));
							*(p + 2) = C * temp2 + S * (*(q + 2));
							*(p + 3) = C * temp3 + S * (*(q + 3));

							*q = C * (*q) - S * temp0;
							*(q + 1) = C * (*(q + 1)) - S * temp1;
							*(q + 2) = C * (*(q + 2)) - S * temp2;
							*(q + 3) = C * (*(q + 3)) - S * temp3;

							q += 4; p += 4;
						}
						for (jj = N & 3; jj > 0; jj--)
						{
							temp0 = *p; temp1 = *q;
							*p++ = C * temp0 + S * temp1;
							*q++ = C * temp1 - S * temp0;
						}
					}
				}
			}
		}
		template <typename Ty> void trisolve(Ty* const pfX, const Ty* const pfU, const std::size_t Ur, const std::size_t Xc, const std::size_t) noexcept
		{
			std::size_t Uc = Ur + Xc;

			if (Xc == 1)
			{
				Ty acc;
				const Ty* pU; Ty* pX1;
				std::size_t k;

				for (std::size_t i = Ur - 1; i + 1 > 0; i--)
				{
					pU = (pfU + (i + 1)) + (Uc * i);
					pX1 = pfX + (i + 1);
					acc = static_cast<Ty>(0);

					for (k = i + 1; k < Ur; k++)
					{
						acc -= (*pU++) * (*pX1++);
					}

					acc += *pU;
					*(pfX + i) = acc / (*((pfU + i) + (Uc * i)));
				}
				return;
			}

			else
			{
				Ty temp;
				const Ty* pU; Ty* pX1; Ty* pX2;
				std::size_t i, j, ii;

				for (i = Ur - 1; i + 1 > 0; i--)
				{
					std::memcpy(pfX + (Xc * i), (pfU + Ur) + (Uc * i), Xc * sizeof(Ty));

					pU = (pfU + (i + 1)) + (Uc * i);
					pX2 = pfX + (Xc * (i + 1));
					for (ii = i + 1; ii < Ur; ii++)
					{
						temp = *pU++;
						pX1 = pfX + (Xc * i);
						for (j = Xc; j > 0; j--)
						{
							*pX1++ -= temp * (*pX2++);
						}
					}

					temp = static_cast<Ty>(1) / *((pfU + i) + (Uc * i));
					pX1 = pfX + (Xc * i);
					for (j = Xc; j > 0; j--)
					{
						(*pX1++) *= temp;
					}
				}
			}
		}
		template <typename Ty> Ty diagonal_ratio(const Ty* const pfU, const std::size_t Ur, const std::size_t Uc) noexcept
		{
			const Ty* pU = pfU;
			std::size_t N = Uc + 1;
			std::size_t n = (Ur < Uc) ? Ur : Uc;
			Ty R = std::abs(*pU); Ty P = R; Ty Q = R;

			for (std::size_t k = n; k > 1; k--)
			{
				pU += N;
				R = std::abs(*pU);
				if (R < P) { P = R; }
				if (R > Q) { Q = R; }
			}

			if (Q == static_cast<Ty>(0)) { return static_cast<Ty>(0); }
			else { return P / Q; }
		}
		template <typename Ty> void sort(Ty* const pfA, std::size_t Ar, std::size_t Ac, std::size_t col,
			Ty* const pfB, std::size_t* const pfnum0, std::size_t* const pfnum1)
		{
			Ty* p0 = pfA + col; Ty* p1 = pfB; Ty* p0end; Ty* p1end; Ty* p; Ty* q; Ty* pmerge;
			std::size_t* pnmerge; std::size_t* pn0 = pfnum0; std::size_t* pn1 = pfnum1; std::size_t* pn; std::size_t* qn;
			std::size_t k; std::size_t kmax; std::size_t kmax2;
			std::size_t i; std::size_t j = 1; std::size_t j2;

			for (i = Ar; i > 0; i--) { *p1++ = *p0; p0 += Ac; }
			for (i = 0; i < Ar; i++) { *pn0++ = i; *pn1++ = i; }

			bool dir = 1;

			while (j < Ar)
			{
				j2 = j + j;
				kmax = Ar - (Ar & (j - 1));
				kmax2 = Ar - (Ar & (j2 - 1));

				if (dir)
				{
					p = pfB; q = pfB + Ar; dir = false;
					pn = pfnum0; qn = pfnum1;
				}
				else
				{
					p = pfB + Ar; q = pfB; dir = true;
					pn = pfnum1; qn = pfnum0;
				}

				for (k = 0; k < kmax2; k += j2)
				{
					p0 = p + k; p1 = p + (k + j);
					p0end = p1; p1end = p + (k + j2);
					pmerge = q + k;

					pn0 = pn + k; pn1 = pn + (k + j);
					pnmerge = qn + k;

					for (i = j2; i > 0; i--)
					{
						if (p0 == p0end) { *pmerge++ = std::move(*p1++); *pnmerge++ = *pn1++; }
						else if (p1 == p1end) { *pmerge++ = std::move(*p0++); *pnmerge++ = *pn0++; }
						else if (*p0 > * p1) { *pmerge++ = std::move(*p1++); *pnmerge++ = *pn1++; }
						else { *pmerge++ = std::move(*p0++); *pnmerge++ = *pn0++; }
					}
				}
				if ((kmax != kmax2) && (kmax < Ar))
				{
					p0 = p + kmax2; p1 = p + kmax;
					p0end = p1; p1end = p + Ar;
					pmerge = q + kmax2;

					pn0 = pn + kmax2; pn1 = pn + kmax;
					pnmerge = qn + kmax2;

					for (i = Ar - kmax2; i > 0; i--)
					{
						if (p0 == p0end) { *pmerge++ = std::move(*p1++); *pnmerge++ = *pn1++; }
						else if (p1 == p1end) { *pmerge++ = std::move(*p0++); *pnmerge++ = *pn0++; }
						else if (*p0 > * p1) { *pmerge++ = std::move(*p1++); *pnmerge++ = *pn1++; }
						else { *pmerge++ = std::move(*p0++); *pnmerge++ = *pn0++; }
					}
				}
				else
				{
					p0 = p + kmax2; pmerge = q + kmax2;
					pn0 = pn + kmax2; pnmerge = qn + kmax2;
					for (i = Ar - kmax2; i > 0; i--) { *pmerge++ = std::move(*p0++); *pnmerge++ = *pn0++; }
				}
				j = j2;
			}

			i = Ar * Ac; j = i & 3;
			p0 = pfA; p1 = pfB;
			for (k = i - j; k > 0; k -= 4)
			{
				*p1 = std::move(*p0);
				*(p1 + 1) = std::move(*(p0 + 1));
				*(p1 + 2) = std::move(*(p0 + 2));
				*(p1 + 3) = std::move(*(p0 + 3));
				p0 += 4; p1 += 4;
			}

			switch (j)
			{

			case 0:
				break;

			case 1:
				*p1 = std::move(*p0);
				break;

			case 2:
				*p1 = std::move(*p0);
				*(p1 + 1) = std::move(*(p0 + 1));
				break;

			case 3:
				*p1 = std::move(*p0);
				*(p1 + 1) = std::move(*(p0 + 1));
				*(p1 + 2) = std::move(*(p0 + 2));
				break;
			}

			j = Ac & 3; j2 = Ac - j;
			p0 = pfA;

			if (dir) { pn = pfnum0; }
			else { pn = pfnum1; }

			for (i = Ar; i > 0; i--)
			{
				p1 = pfB + Ac * (*pn++);
				for (k = j2; k > 0; k -= 4)
				{
					*p0 = std::move(*p1);
					*(p0 + 1) = std::move(*(p1 + 1));
					*(p0 + 2) = std::move(*(p1 + 2));
					*(p0 + 3) = std::move(*(p1 + 3));
					p0 += 4; p1 += 4;
				}

				switch (j)
				{

				case 0:
					break;

				case 1:
					*p0 = std::move(*p1);
					p0++;
					break;

				case 2:
					*p0 = std::move(*p1);
					*(p0 + 1) = std::move(*(p1 + 1));
					p0 += 2;
					break;

				case 3:
					*p0 = std::move(*p1);
					*(p0 + 1) = std::move(*(p1 + 1));
					*(p0 + 2) = std::move(*(p1 + 2));
					p0 += 3;
					break;
				}
			}
		}
		template <typename Ty, class Predicate> void sort(Ty* const pfA, std::size_t Ar, std::size_t Ac, std::size_t col,
			Ty* const pfB, std::size_t* const pfnum0, std::size_t* const pfnum1, Predicate less_Predicate)
		{
			Ty* p0 = pfA + col; Ty* p1 = pfB; Ty* p0end; Ty* p1end; Ty* p; Ty* q; Ty* pmerge;
			std::size_t* pnmerge; std::size_t* pn0 = pfnum0; std::size_t* pn1 = pfnum1; std::size_t* pn; std::size_t* qn;
			std::size_t k; std::size_t kmax; std::size_t kmax2;
			std::size_t i; std::size_t j = 1; std::size_t j2;

			for (i = Ar; i > 0; i--) { *p1++ = *p0; p0 += Ac; }
			for (i = 0; i < Ar; i++) { *pn0++ = i; *pn1++ = i; }

			bool dir = 1;

			while (j < Ar)
			{
				j2 = j + j;
				kmax = Ar - (Ar & (j - 1));
				kmax2 = Ar - (Ar & (j2 - 1));

				if (dir)
				{
					p = pfB; q = pfB + Ar; dir = false;
					pn = pfnum0; qn = pfnum1;
				}
				else
				{
					p = pfB + Ar; q = pfB; dir = true;
					pn = pfnum1; qn = pfnum0;
				}

				for (k = 0; k < kmax2; k += j2)
				{
					p0 = p + k; p1 = p + (k + j);
					p0end = p1; p1end = p + (k + j2);
					pmerge = q + k;

					pn0 = pn + k; pn1 = pn + (k + j);
					pnmerge = qn + k;

					for (i = j2; i > 0; i--)
					{
						if (p0 == p0end) { *pmerge++ = std::move(*p1++); *pnmerge++ = *pn1++; }
						else if (p1 == p1end) { *pmerge++ = std::move(*p0++); *pnmerge++ = *pn0++; }
						else if (less_Predicate(*p1, *p0)) { *pmerge++ = std::move(*p1++); *pnmerge++ = *pn1++; }
						else { *pmerge++ = std::move(*p0++); *pnmerge++ = *pn0++; }
					}
				}
				if ((kmax != kmax2) && (kmax < Ar))
				{
					p0 = p + kmax2; p1 = p + kmax;
					p0end = p1; p1end = p + Ar;
					pmerge = q + kmax2;

					pn0 = pn + kmax2; pn1 = pn + kmax;
					pnmerge = qn + kmax2;

					for (i = Ar - kmax2; i > 0; i--)
					{
						if (p0 == p0end) { *pmerge++ = std::move(*p1++); *pnmerge++ = *pn1++; }
						else if (p1 == p1end) { *pmerge++ = std::move(*p0++); *pnmerge++ = *pn0++; }
						else if (less_Predicate(*p1, *p0)) { *pmerge++ = std::move(*p1++); *pnmerge++ = *pn1++; }
						else { *pmerge++ = std::move(*p0++); *pnmerge++ = *pn0++; }
					}
				}
				else
				{
					p0 = p + kmax2; pmerge = q + kmax2;
					pn0 = pn + kmax2; pnmerge = qn + kmax2;
					for (i = Ar - kmax2; i > 0; i--) { *pmerge++ = std::move(*p0++); *pnmerge++ = *pn0++; }
				}
				j = j2;
			}

			i = Ar * Ac; j = i & 3;
			p0 = pfA; p1 = pfB;
			for (k = i - j; k > 0; k -= 4)
			{
				*p1 = std::move(*p0);
				*(p1 + 1) = std::move(*(p0 + 1));
				*(p1 + 2) = std::move(*(p0 + 2));
				*(p1 + 3) = std::move(*(p0 + 3));
				p0 += 4; p1 += 4;
			}

			switch (j)
			{

			case 0:
				break;

			case 1:
				*p1 = std::move(*p0);
				break;

			case 2:
				*p1 = std::move(*p0);
				*(p1 + 1) = std::move(*(p0 + 1));
				break;

			case 3:
				*p1 = std::move(*p0);
				*(p1 + 1) = std::move(*(p0 + 1));
				*(p1 + 2) = std::move(*(p0 + 2));
				break;
			}

			j = Ac & 3; j2 = Ac - j;
			p0 = pfA;

			if (dir) { pn = pfnum0; }
			else { pn = pfnum1; }

			for (i = Ar; i > 0; i--)
			{
				p1 = pfB + Ac * (*pn++);
				for (k = j2; k > 0; k -= 4)
				{
					*p0 = std::move(*p1);
					*(p0 + 1) = std::move(*(p1 + 1));
					*(p0 + 2) = std::move(*(p1 + 2));
					*(p0 + 3) = std::move(*(p1 + 3));
					p0 += 4; p1 += 4;
				}

				switch (j)
				{

				case 0:
					break;

				case 1:
					*p0 = std::move(*p1);
					p0++;
					break;

				case 2:
					*p0 = *p1;
					*(p0 + 1) = std::move(*(p1 + 1));
					p0 += 2;
					break;

				case 3:
					*p0 = std::move(*p1);
					*(p0 + 1) = std::move(*(p1 + 1));
					*(p0 + 2) = std::move(*(p1 + 2));
					p0 += 3;
					break;
				}
			}
		}
#ifdef WELP_MATRIX_INCLUDE_IOSTREAM
		template <typename Ty> void say_rm(const Ty* const pfA, std::size_t Ar, std::size_t Ac,
			std::size_t cmax, std::size_t rblock, std::streamsize cwidth)
		{
			std::size_t i, j, iOut, jOut, imax, jmax;

			if (rblock == 0) { rblock = Ar; }

			for (iOut = 0; iOut < Ar; iOut += rblock)
			{
				imax = (iOut + rblock < Ar) ? iOut + rblock : Ar;
				for (jOut = 0; jOut < Ac; jOut += cmax)
				{
					jmax = (jOut + cmax < Ac) ? jOut + cmax : Ac;

					if ((iOut == 0) && (jOut == 0)) { std::cout << ">>>         "; }
					else { std::cout << "            "; }

					for (j = jOut; j < jmax; j++)
					{
						std::cout << "col ";
						std::cout.fill(' '); std::cout.width(cwidth);
						std::cout << std::left << j;
					}

					for (i = iOut; i < imax; i++)
					{
						for (j = jOut; j < jmax; j++)
						{
							if (j == jOut)
							{
								std::cout << "\n";
								std::cout << "row ";
								std::cout.fill(' '); std::cout.width(8);
								std::cout << std::left << i;
							}
							std::cout << "> ";
							std::cout.fill(' '); std::cout.width(cwidth + 2);
							std::cout << std::left << *((pfA + j) + (Ac * i));
						}
					}

					std::cout << "\n" << std::endl;
				}
			}
		}
		template <typename Ty> void say_cm(const Ty* const pfA, std::size_t Ar, std::size_t Ac,
			std::size_t cmax, std::size_t rblock, std::streamsize cwidth)
		{
			std::size_t i, j, iOut, jOut, imax, jmax;

			if (rblock == 0) { rblock = Ar; }

			for (iOut = 0; iOut < Ar; iOut += rblock)
			{
				imax = (iOut + rblock < Ar) ? iOut + rblock : Ar;
				for (jOut = 0; jOut < Ac; jOut += cmax)
				{
					jmax = (jOut + cmax < Ac) ? jOut + cmax : Ac;

					if ((iOut == 0) && (jOut == 0)) { std::cout << ">>>         "; }
					else { std::cout << "            "; }

					for (j = jOut; j < jmax; j++)
					{
						std::cout << "col ";
						std::cout.fill(' '); std::cout.width(cwidth);
						std::cout << std::left << j;
					}

					for (i = iOut; i < imax; i++)
					{
						for (j = jOut; j < jmax; j++)
						{
							if (j == jOut)
							{
								std::cout << "\n";
								std::cout << "row ";
								std::cout.fill(' '); std::cout.width(8);
								std::cout << std::left << i;
							}
							std::cout << "> ";
							std::cout.fill(' '); std::cout.width(cwidth + 2);
							std::cout << std::left << *((pfA + i) + (Ar * j));
						}
					}

					std::cout << "\n" << std::endl;
				}
			}
		}
#endif // WELP_MATRIX_INCLUDE_IOSTREAM

		template <typename Ty, typename Tn> void pow(Ty* const pfC, const Ty* const pfA, const Tn n,
			const std::size_t Ac, Ty* const pfpool0, Ty* const pfpool1) noexcept
		{
			if (n == 0)
			{
				std::size_t N = Ac + 1;
				Ty* pC = pfC;
				for (std::size_t k = Ac; k > 0; k--) { *pC = 1; pC += N; }
			}

			else
			{
				std::size_t N = Ac * Ac;
				Tn r = n;
				Tn m;
				bool b;
				bool first = true;

				while (r != 0)
				{
					std::memcpy(pfpool0, pfA, N * sizeof(Ty));
					m = 1;
					b = 0;

					while (m + m <= r)
					{
						m += m;
						if (b == 0)
						{
							Ty* p = pfpool1;
							for (std::size_t k = N; k > 0; k--) { *p++ = 0; }
							welp::matrix_subroutines::pmxm(pfpool1, pfpool0, pfpool0, Ac, Ac, Ac);
							b = 1;
						}
						else
						{
							Ty* p = pfpool0;
							for (std::size_t k = N; k > 0; k--) { *p++ = 0; }
							welp::matrix_subroutines::pmxm(pfpool0, pfpool1, pfpool1, Ac, Ac, Ac);
							b = 0;
						}
					}

					r -= m;

					if (first == false)
					{
						if (b == 0)
						{
							Ty* p = pfpool1;
							for (std::size_t k = N; k > 0; k--) { *p++ = 0; }
							welp::matrix_subroutines::pmxm(pfpool1, pfpool0, pfC, Ac, Ac, Ac);
							std::memcpy(pfC, pfpool1, N * sizeof(Ty));
						}
						else
						{
							Ty* p = pfpool0;
							for (std::size_t k = N; k > 0; k--) { *p++ = 0; }
							welp::matrix_subroutines::pmxm(pfpool0, pfpool1, pfC, Ac, Ac, Ac);
							std::memcpy(pfC, pfpool0, N * sizeof(Ty));
						}
					}

					else
					{
						if (b == 0) { std::memcpy(pfC, pfpool0, N * sizeof(Ty)); }
						else { std::memcpy(pfC, pfpool1, N * sizeof(Ty)); }
						first = false;
					}
				}
			}
		}

		template <typename Ty> inline bool compare_equal_mm(const Ty* const pfA, const Ty* const pfB, const std::size_t n)
		{
			std::size_t r = n & 3;
			const Ty* pA = pfA; const Ty* pB = pfB;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				if (!(((*pA) == (*pB))
					&& ((*(pA + 1)) == (*(pB + 1)))
					&& ((*(pA + 2)) == (*(pB + 2)))
					&& ((*(pA + 3)) == (*(pB + 3)))))
				{
					return false;
				}
				pA += 4; pB += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				if (!((*pA) == (*pB)))
				{
					return false;
				}
				break;

			case 2:
				if (!(((*pA) == (*pB))
					&& ((*(pA + 1)) == (*(pB + 1)))))
				{
					return false;
				}
				break;

			case 3:
				if (!(((*pA) == (*pB))
					&& ((*(pA + 1)) == (*(pB + 1)))
					&& ((*(pA + 2)) == (*(pB + 2)))))
				{
					return false;
				}
				break;
			}
			return true;
		}
		template <typename Ty> inline bool compare_strictly_smaller_mm(const Ty* const pfA, const Ty* const pfB, const std::size_t n)
		{
			std::size_t r = n & 3;
			const Ty* pA = pfA; const Ty* pB = pfB;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				if (!(((*pA) < (*pB))
					&& ((*(pA + 1)) < (*(pB + 1)))
					&& ((*(pA + 2)) < (*(pB + 2)))
					&& ((*(pA + 3)) < (*(pB + 3)))))
				{
					return false;
				}
				pA += 4; pB += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				if (!((*pA) < (*pB)))
				{
					return false;
				}
				break;

			case 2:
				if (!(((*pA) < (*pB))
					&& ((*(pA + 1)) < (*(pB + 1)))))
				{
					return false;
				}
				break;

			case 3:
				if (!(((*pA) < (*pB))
					&& ((*(pA + 1)) < (*(pB + 1)))
					&& ((*(pA + 2)) < (*(pB + 2)))))
				{
					return false;
				}
				break;
			}
			return true;
		}
		template <typename Ty> inline bool compare_strictly_greater_mm(const Ty* const pfA, const Ty* const pfB, const std::size_t n)
		{
			std::size_t r = n & 3;
			const Ty* pA = pfA; const Ty* pB = pfB;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				if (!(((*pA) > (*pB))
					&& ((*(pA + 1)) > (*(pB + 1)))
					&& ((*(pA + 2)) > (*(pB + 2)))
					&& ((*(pA + 3)) > (*(pB + 3)))))
				{
					return false;
				}
				pA += 4; pB += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				if (!((*pA) > (*pB)))
				{
					return false;
				}
				break;

			case 2:
				if (!(((*pA) > (*pB))
					&& ((*(pA + 1)) > (*(pB + 1)))))
				{
					return false;
				}
				break;

			case 3:
				if (!(((*pA) > (*pB))
					&& ((*(pA + 1)) > (*(pB + 1)))
					&& ((*(pA + 2)) > (*(pB + 2)))))
				{
					return false;
				}
				break;
			}
			return true;
		}
		template <typename Ty> inline bool compare_smaller_mm(const Ty* const pfA, const Ty* const pfB, const std::size_t n)
		{
			std::size_t r = n & 3;
			const Ty* pA = pfA; const Ty* pB = pfB;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				if (!(((*pA) <= (*pB))
					&& ((*(pA + 1)) <= (*(pB + 1)))
					&& ((*(pA + 2)) <= (*(pB + 2)))
					&& ((*(pA + 3)) <= (*(pB + 3)))))
				{
					return false;
				}
				pA += 4; pB += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				if (!((*pA) <= (*pB)))
				{
					return false;
				}
				break;

			case 2:
				if (!(((*pA) <= (*pB))
					&& ((*(pA + 1)) <= (*(pB + 1)))))
				{
					return false;
				}
				break;

			case 3:
				if (!(((*pA) <= (*pB))
					&& ((*(pA + 1)) <= (*(pB + 1)))
					&& ((*(pA + 2)) <= (*(pB + 2)))))
				{
					return false;
				}
				break;
			}
			return true;
		}
		template <typename Ty> inline bool compare_greater_mm(const Ty* const pfA, const Ty* const pfB, const std::size_t n)
		{
			std::size_t r = n & 3;
			const Ty* pA = pfA; const Ty* pB = pfB;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				if (!(((*pA) >= (*pB))
					&& ((*(pA + 1)) >= (*(pB + 1)))
					&& ((*(pA + 2)) >= (*(pB + 2)))
					&& ((*(pA + 3)) >= (*(pB + 3)))))
				{
					return false;
				}
				pA += 4; pB += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				if (!((*pA) >= (*pB)))
				{
					return false;
				}
				break;

			case 2:
				if (!(((*pA) >= (*pB))
					&& ((*(pA + 1)) >= (*(pB + 1)))))
				{
					return false;
				}
				break;

			case 3:
				if (!(((*pA) >= (*pB))
					&& ((*(pA + 1)) >= (*(pB + 1)))
					&& ((*(pA + 2)) >= (*(pB + 2)))))
				{
					return false;
				}
				break;
			}
			return true;
		}
		template <typename Ty1, typename Ty2, class Predicate> inline bool compare_mm(const Ty1* const pfA, const Ty2* const pfB,
			const std::size_t n, Predicate Pr)
		{
			std::size_t r = n & 3;
			const Ty1* pA = pfA; const Ty2* pB = pfB;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				if (!(Pr(*pA, *pB)
					&& Pr(*(pA + 1), *(pB + 1))
					&& Pr(*(pA + 2), *(pB + 2))
					&& Pr(*(pA + 3), *(pB + 3))))
				{
					return false;
				}
				pA += 4; pB += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				if (!Pr(*pA, *pB))
				{
					return false;
				}
				break;

			case 2:
				if (!(Pr(*pA, *pB)
					&& Pr(*(pA + 1), *(pB + 1))))
				{
					return false;
				}
				break;

			case 3:
				if (!(Pr(*pA, *pB)
					&& Pr(*(pA + 1), *(pB + 1))
					&& Pr(*(pA + 2), *(pB + 2))))
				{
					return false;
				}
				break;
			}
			return true;
		}

		template <typename Ty> inline bool compare_equal_ms(const Ty* const pfA, const Ty x, const std::size_t n)
		{
			std::size_t r = n & 3;
			const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				if (!(((*pA) == x)
					&& ((*(pA + 1)) == x)
					&& ((*(pA + 2)) == x)
					&& ((*(pA + 3)) == x)))
				{
					return false;
				}
				pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				if (!((*pA) == x))
				{
					return false;
				}
				break;

			case 2:
				if (!(((*pA) == x)
					&& ((*(pA + 1)) == x)))
				{
					return false;
				}
				break;

			case 3:
				if (!(((*pA) == x)
					&& ((*(pA + 1)) == x)
					&& ((*(pA + 2)) == x)))
				{
					return false;
				}
				break;
			}
			return true;
		}
		template <typename Ty> inline bool compare_strictly_smaller_ms(const Ty* const pfA, const Ty x, const std::size_t n)
		{
			std::size_t r = n & 3;
			const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				if (!(((*pA) < x)
					&& ((*(pA + 1)) < x)
					&& ((*(pA + 2)) < x)
					&& ((*(pA + 3)) < x)))
				{
					return false;
				}
				pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				if (!((*pA) < x))
				{
					return false;
				}
				break;

			case 2:
				if (!(((*pA) < x)
					&& ((*(pA + 1)) < x)))
				{
					return false;
				}
				break;

			case 3:
				if (!(((*pA) < x)
					&& ((*(pA + 1)) < x)
					&& ((*(pA + 2)) < x)))
				{
					return false;
				}
				break;
			}
			return true;
		}
		template <typename Ty> inline bool compare_strictly_greater_ms(const Ty* const pfA, const Ty x, const std::size_t n)
		{
			std::size_t r = n & 3;
			const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				if (!(((*pA) > x)
					&& ((*(pA + 1)) > x)
					&& ((*(pA + 2)) > x)
					&& ((*(pA + 3)) > x)))
				{
					return false;
				}
				pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				if (!((*pA) > x))
				{
					return false;
				}
				break;

			case 2:
				if (!(((*pA) > x)
					&& ((*(pA + 1)) > x)))
				{
					return false;
				}
				break;

			case 3:
				if (!(((*pA) > x)
					&& ((*(pA + 1)) > x)
					&& ((*(pA + 2)) > x)))
				{
					return false;
				}
				break;
			}
			return true;
		}
		template <typename Ty> inline bool compare_smaller_ms(const Ty* const pfA, const Ty x, const std::size_t n)
		{
			std::size_t r = n & 3;
			const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				if (!(((*pA) <= x)
					&& ((*(pA + 1)) <= x)
					&& ((*(pA + 2)) <= x)
					&& ((*(pA + 3)) <= x)))
				{
					return false;
				}
				pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				if (!((*pA) <= x))
				{
					return false;
				}
				break;

			case 2:
				if (!(((*pA) <= x)
					&& ((*(pA + 1)) <= x)))
				{
					return false;
				}
				break;

			case 3:
				if (!(((*pA) <= x)
					&& ((*(pA + 1)) <= x)
					&& ((*(pA + 2)) <= x)))
				{
					return false;
				}
				break;
			}
			return true;
		}
		template <typename Ty> inline bool compare_greater_ms(const Ty* const pfA, const Ty x, const std::size_t n)
		{
			std::size_t r = n & 3;
			const Ty* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				if (!(((*pA) >= x)
					&& ((*(pA + 1)) >= x)
					&& ((*(pA + 2)) >= x)
					&& ((*(pA + 3)) >= x)))
				{
					return false;
				}
				pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				if (!((*pA) >= x))
				{
					return false;
				}
				break;

			case 2:
				if (!(((*pA) >= x)
					&& ((*(pA + 1)) >= x)))
				{
					return false;
				}
				break;

			case 3:
				if (!(((*pA) >= x)
					&& ((*(pA + 1)) >= x)
					&& ((*(pA + 2)) >= x)))
				{
					return false;
				}
				break;
			}
			return true;
		}
		template <typename Ty1, typename Ty2, class Predicate> inline bool compare_ms(const Ty1* const pfA,
			const Ty2 x, const std::size_t n, Predicate Pr)
		{
			std::size_t r = n & 3;
			const Ty1* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				if (!(Pr(*pA, x)
					&& Pr(*(pA + 1), x)
					&& Pr(*(pA + 2), x)
					&& Pr(*(pA + 3), x)))
				{
					return false;
				}
				pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				if (!Pr(*pA, x))
				{
					return false;
				}
				break;

			case 2:
				if (!(Pr(*pA, x)
					&& Pr(*(pA + 1), x)))
				{
					return false;
				}
				break;

			case 3:
				if (!(Pr(*pA, x)
					&& Pr(*(pA + 1), x)
					&& Pr(*(pA + 2), x)))
				{
					return false;
				}
				break;
			}
			return true;
		}
	}


#ifdef WELP_MATRIX_AVX_EXT
	namespace matrix_subroutines
	{
		inline void fill(float* const pfC, const float x, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			float* pC = pfC;

			__m256 vs = _mm256_broadcast_ss(&x);

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				_mm256_storeu_ps(pC, vs);
				pC += 8;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = x;
				break;

			case 2:
				*pC = x;
				*(pC + 1) = x;
				break;

			case 3:
				*pC = x;
				*(pC + 1) = x;
				*(pC + 2) = x;
				break;

			case 4:
				_mm_storeu_ps(pC, _mm_broadcast_ss(&x));
				break;

			case 5:
				_mm_storeu_ps(pC, _mm_broadcast_ss(&x));
				*(pC + 4) = x;
				break;

			case 6:
				_mm_storeu_ps(pC, _mm_broadcast_ss(&x));
				*(pC + 4) = x;
				*(pC + 5) = x;
				break;

			case 7:
				_mm_storeu_ps(pC, _mm_broadcast_ss(&x));
				*(pC + 4) = x;
				*(pC + 5) = x;
				*(pC + 6) = x;
				break;
			}
		}
		inline float dot(const float* const pfA, const float* const pfB, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			const float* pA = pfA; const float* pB = pfB;

			__m256 v = _mm256_setzero_ps();
			union { __m256 v; float arr[8]; } varr;

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				v = _mm256_fmadd_ps(_mm256_loadu_ps(pA), _mm256_loadu_ps(pB), v);
				pA += 8; pB += 8;
			}
			v = _mm256_hadd_ps(v, v); varr.v = _mm256_hadd_ps(v, v);
			float acc = varr.arr[0] + varr.arr[4];

			switch (r)
			{

			case 0:
				break;

			case 1:
				acc += (*pA) * (*pB);
				break;

			case 2:
				acc += (*pA) * (*pB) + (*(pA + 1)) * (*(pB + 1));
				break;

			case 3:
				union { __m128 v; float arr[4]; } varr128;
				varr128.v = _mm_dp_ps(_mm_loadu_ps(pA), _mm_loadu_ps(pB), 0x7f);
				acc += varr128.arr[0];
				break;

			case 4:
				varr128.v = _mm_dp_ps(_mm_loadu_ps(pA), _mm_loadu_ps(pB), 0xff);
				acc += varr128.arr[0];
				break;

			case 5:
				varr128.v = _mm_dp_ps(_mm_loadu_ps(pA), _mm_loadu_ps(pB), 0xff);
				acc += varr128.arr[0] + (*(pA + 4)) * (*(pB + 4));
				break;

			case 6:
				varr128.v = _mm_dp_ps(_mm_loadu_ps(pA), _mm_loadu_ps(pB), 0xff);
				acc += varr128.arr[0] + (*(pA + 4)) * (*(pB + 4)) + (*(pA + 5)) * (*(pB + 5));
				break;

			case 7:
				varr128.v = _mm_dp_ps(_mm_loadu_ps(pA), _mm_loadu_ps(pB), 0xff);
				acc += varr128.arr[0];
				varr128.v = _mm_dp_ps(_mm_loadu_ps(pA + 4), _mm_loadu_ps(pB + 4), 0x7f);
				acc += varr128.arr[0];
				break;
			}

			return acc;
		}
		inline float norm2(const float* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			const float* pA = pfA;

			__m256 v = _mm256_setzero_ps();
			union { __m256 v; float arr[8]; } varr;

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				v = _mm256_fmadd_ps(_mm256_loadu_ps(pA), _mm256_loadu_ps(pA), v);
				pA += 8;
			}
			v = _mm256_hadd_ps(v, v); varr.v = _mm256_hadd_ps(v, v);
			float acc = varr.arr[0] + varr.arr[4];

			switch (r)
			{

			case 0:
				break;

			case 1:
				acc += (*pA) * (*pA);
				break;

			case 2:
				acc += (*pA) * (*pA) + (*(pA + 1)) * (*(pA + 1));
				break;

			case 3:
				union { __m128 v; float arr[4]; } varr128;
				varr128.v = _mm_dp_ps(_mm_loadu_ps(pA), _mm_loadu_ps(pA), 0x7f);
				acc += varr128.arr[0];
				break;

			case 4:
				varr128.v = _mm_dp_ps(_mm_loadu_ps(pA), _mm_loadu_ps(pA), 0xff);
				acc += varr128.arr[0];
				break;

			case 5:
				varr128.v = _mm_dp_ps(_mm_loadu_ps(pA), _mm_loadu_ps(pA), 0xff);
				acc += varr128.arr[0] + (*(pA + 4)) * (*(pA + 4));
				break;

			case 6:
				varr128.v = _mm_dp_ps(_mm_loadu_ps(pA), _mm_loadu_ps(pA), 0xff);
				acc += varr128.arr[0] + (*(pA + 4)) * (*(pA + 4)) + (*(pA + 5)) * (*(pA + 5));
				break;

			case 7:
				varr128.v = _mm_dp_ps(_mm_loadu_ps(pA), _mm_loadu_ps(pA), 0xff);
				acc += varr128.arr[0];
				varr128.v = _mm_dp_ps(_mm_loadu_ps(pA + 4), _mm_loadu_ps(pA + 4), 0x7f);
				acc += varr128.arr[0];
				break;
			}

			return acc;
		}
		inline void _m(float* const pfC, const float* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			float* pC = pfC; const float* pA = pfA;

			__m256 vs = _mm256_setzero_ps();

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				_mm256_storeu_ps(pC, _mm256_sub_ps(vs, _mm256_loadu_ps(pA)));
				pC += 8; pA += 8;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = -(*pA);
				break;

			case 2:
				*pC = -(*pA);
				*(pC + 1) = -(*(pA + 1));
				break;

			case 3:
				*pC = -(*pA);
				*(pC + 1) = -(*(pA + 1));
				*(pC + 2) = -(*(pA + 2));
				break;

			case 4:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_setzero_ps(),
					_mm_loadu_ps(pA)));
				break;

			case 5:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_setzero_ps(),
					_mm_loadu_ps(pA)));
				*(pC + 4) = -(*(pA + 4));
				break;

			case 6:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_setzero_ps(),
					_mm_loadu_ps(pA)));
				*(pC + 4) = -(*(pA + 4));
				*(pC + 5) = -(*(pA + 5));
				break;

			case 7:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_setzero_ps(),
					_mm_loadu_ps(pA)));
				*(pC + 4) = -(*(pA + 4));
				*(pC + 5) = -(*(pA + 5));
				*(pC + 6) = -(*(pA + 6));
				break;
			}
		}
		inline void ps(float* const pfC, const float x, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			float* pC = pfC;

			__m256 vs = _mm256_broadcast_ss(&x);

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				_mm256_storeu_ps(pC, _mm256_add_ps(vs, _mm256_loadu_ps(pC)));
				pC += 8;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC += x;
				break;

			case 2:
				*pC += x;
				*(pC + 1) += x;
				break;

			case 3:
				*pC += x;
				*(pC + 1) += x;
				*(pC + 2) += x;
				break;

			case 4:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pC)));
				break;

			case 5:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pC)));
				*(pC + 4) += x;
				break;

			case 6:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pC)));
				*(pC + 4) += x;
				*(pC + 5) += x;
				break;

			case 7:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pC)));
				*(pC + 4) += x;
				*(pC + 5) += x;
				*(pC + 6) += x;
				break;
			}
		}
		inline void spm(float* const pfC, const float x, const float* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			float* pC = pfC; const float* pA = pfA;

			__m256 vs = _mm256_broadcast_ss(&x);

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				_mm256_storeu_ps(pC, _mm256_add_ps(vs, _mm256_loadu_ps(pA)));
				pC += 8; pA += 8;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = x + (*pA);
				break;

			case 2:
				*pC += x + (*pA);
				*(pC + 1) = x + (*(pA + 1));
				break;

			case 3:
				*pC = x + (*pA);
				*(pC + 1) = x + (*(pA + 1));
				*(pC + 2) = x + (*(pA + 2));
				break;

			case 4:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA)));
				break;

			case 5:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA)));
				*(pC + 4) = x + (*(pA + 4));
				break;

			case 6:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA)));
				*(pC + 4) = x + (*(pA + 4));
				*(pC + 5) = x + (*(pA + 5));
				break;

			case 7:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA)));
				*(pC + 4) = x + (*(pA + 4));
				*(pC + 5) = x + (*(pA + 5));
				*(pC + 6) = x + (*(pA + 6));
				break;
			}
		}
		inline void sp_m(float* const pfC, const float x, const float* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			float* pC = pfC; const float* pA = pfA;

			__m256 vs = _mm256_broadcast_ss(&x);

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				_mm256_storeu_ps(pC, _mm256_sub_ps(vs, _mm256_loadu_ps(pA)));
				pC += 8; pA += 8;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = x - (*pA);
				break;

			case 2:
				*pC += x - (*pA);
				*(pC + 1) = x - (*(pA + 1));
				break;

			case 3:
				*pC = x - (*pA);
				*(pC + 1) = x - (*(pA + 1));
				*(pC + 2) = x - (*(pA + 2));
				break;

			case 4:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA)));
				break;

			case 5:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA)));
				*(pC + 4) = x - (*(pA + 4));
				break;

			case 6:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA)));
				*(pC + 4) = x - (*(pA + 4));
				*(pC + 5) = x - (*(pA + 5));
				break;

			case 7:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA)));
				*(pC + 4) = x - (*(pA + 4));
				*(pC + 5) = x - (*(pA + 5));
				*(pC + 6) = x - (*(pA + 6));
				break;
			}
		}
		inline void pm(float* const pfC, const float* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			float* pC = pfC; const float* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				_mm256_storeu_ps(pC, _mm256_add_ps(_mm256_loadu_ps(pC),
					_mm256_loadu_ps(pA)));
				pC += 8; pA += 8;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC += (*pA);
				break;

			case 2:
				*pC += (*pA);
				*(pC + 1) += (*(pA + 1));
				break;

			case 3:
				*pC += (*pA);
				*(pC + 1) += (*(pA + 1));
				*(pC + 2) += (*(pA + 2));
				break;

			case 4:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_loadu_ps(pC),
					_mm_loadu_ps(pA)));
				break;

			case 5:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_loadu_ps(pC),
					_mm_loadu_ps(pA)));
				*(pC + 4) += (*(pA + 4));
				break;

			case 6:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_loadu_ps(pC),
					_mm_loadu_ps(pA)));
				*(pC + 4) += (*(pA + 4));
				*(pC + 5) += (*(pA + 5));
				break;

			case 7:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_loadu_ps(pC),
					_mm_loadu_ps(pA)));
				*(pC + 4) += (*(pA + 4));
				*(pC + 5) += (*(pA + 5));
				*(pC + 6) += (*(pA + 6));
				break;
			}
		}
		inline void p_m(float* const pfC, const float* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			float* pC = pfC; const float* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				_mm256_storeu_ps(pC, _mm256_sub_ps(_mm256_loadu_ps(pC),
					_mm256_loadu_ps(pA)));
				pC += 8; pA += 8;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC -= (*pA);
				break;

			case 2:
				*pC -= (*pA);
				*(pC + 1) -= (*(pA + 1));
				break;

			case 3:
				*pC -= (*pA);
				*(pC + 1) -= (*(pA + 1));
				*(pC + 2) -= (*(pA + 2));
				break;

			case 4:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_loadu_ps(pC),
					_mm_loadu_ps(pA)));
				break;

			case 5:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_loadu_ps(pC),
					_mm_loadu_ps(pA)));
				*(pC + 4) -= (*(pA + 4));
				break;

			case 6:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_loadu_ps(pC),
					_mm_loadu_ps(pA)));
				*(pC + 4) -= (*(pA + 4));
				*(pC + 5) -= (*(pA + 5));
				break;

			case 7:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_loadu_ps(pC),
					_mm_loadu_ps(pA)));
				*(pC + 4) -= (*(pA + 4));
				*(pC + 5) -= (*(pA + 5));
				*(pC + 6) -= (*(pA + 6));
				break;
			}
		}
		inline void mpm(float* const pfC, const float* const pfA, const float* const pfB, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			float* pC = pfC; const float* pA = pfA; const float* pB = pfB;

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				_mm256_storeu_ps(pC, _mm256_add_ps(_mm256_loadu_ps(pA),
					_mm256_loadu_ps(pB)));
				pC += 8; pA += 8; pB += 8;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = (*pA) + (*pB);
				break;

			case 2:
				*pC = (*pA) + (*pB);
				*(pC + 1) = (*(pA + 1)) + (*(pB + 1));
				break;

			case 3:
				*pC = (*pA) + (*pB);
				*(pC + 1) = (*(pA + 1)) + (*(pB + 1));
				*(pC + 2) = (*(pA + 2)) + (*(pB + 2));
				break;

			case 4:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_loadu_ps(pA),
					_mm_loadu_ps(pB)));
				break;

			case 5:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_loadu_ps(pA),
					_mm_loadu_ps(pB)));
				*(pC + 4) = (*(pA + 4)) + (*(pB + 4));
				break;

			case 6:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_loadu_ps(pA),
					_mm_loadu_ps(pB)));
				*(pC + 4) = (*(pA + 4)) + (*(pB + 4));
				*(pC + 5) = (*(pA + 5)) + (*(pB + 5));
				break;

			case 7:
				_mm_storeu_ps(pC, _mm_add_ps(_mm_loadu_ps(pA),
					_mm_loadu_ps(pB)));
				*(pC + 4) = (*(pA + 4)) + (*(pB + 4));
				*(pC + 5) = (*(pA + 5)) + (*(pB + 5));
				*(pC + 6) = (*(pA + 6)) + (*(pB + 6));
				break;
			}
		}
		inline void mp_m(float* const pfC, const float* const pfA, const float* const pfB, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			float* pC = pfC; const float* pA = pfA; const float* pB = pfB;

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				_mm256_storeu_ps(pC, _mm256_sub_ps(_mm256_loadu_ps(pA),
					_mm256_loadu_ps(pB)));
				pC += 8; pA += 8; pB += 8;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = (*pA) - (*pB);
				break;

			case 2:
				*pC = (*pA) - (*pB);
				*(pC + 1) = (*(pA + 1)) - (*(pB + 1));
				break;

			case 3:
				*pC = (*pA) - (*pB);
				*(pC + 1) = (*(pA + 1)) - (*(pB + 1));
				*(pC + 2) = (*(pA + 2)) - (*(pB + 2));
				break;

			case 4:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_loadu_ps(pA),
					_mm_loadu_ps(pB)));
				break;

			case 5:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_loadu_ps(pA),
					_mm_loadu_ps(pB)));
				*(pC + 4) = (*(pA + 4)) - (*(pB + 4));
				break;

			case 6:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_loadu_ps(pA),
					_mm_loadu_ps(pB)));
				*(pC + 4) = (*(pA + 4)) - (*(pB + 4));
				*(pC + 5) = (*(pA + 5)) - (*(pB + 5));
				break;

			case 7:
				_mm_storeu_ps(pC, _mm_sub_ps(_mm_loadu_ps(pA),
					_mm_loadu_ps(pB)));
				*(pC + 4) = (*(pA + 4)) - (*(pB + 4));
				*(pC + 5) = (*(pA + 5)) - (*(pB + 5));
				*(pC + 6) = (*(pA + 6)) - (*(pB + 6));
				break;
			}
		}
		inline void xs(float* const pfC, const float x, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			float* pC = pfC;

			__m256 vs = _mm256_broadcast_ss(&x);

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				_mm256_storeu_ps(pC, _mm256_mul_ps(vs, _mm256_loadu_ps(pC)));
				pC += 8;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC *= x;
				break;

			case 2:
				*pC *= x;
				*(pC + 1) *= x;
				break;

			case 3:
				*pC *= x;
				*(pC + 1) *= x;
				*(pC + 2) *= x;
				break;

			case 4:
				_mm_storeu_ps(pC, _mm_mul_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pC)));
				break;

			case 5:
				_mm_storeu_ps(pC, _mm_mul_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pC)));
				*(pC + 4) *= x;
				break;

			case 6:
				_mm_storeu_ps(pC, _mm_mul_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pC)));
				*(pC + 4) *= x;
				*(pC + 5) *= x;
				break;

			case 7:
				_mm_storeu_ps(pC, _mm_mul_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pC)));
				*(pC + 4) *= x;
				*(pC + 5) *= x;
				*(pC + 6) *= x;
				break;
			}
		}
		inline void sxm(float* const pfC, const float x, const float* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			float* pC = pfC; const float* pA = pfA;

			__m256 vs = _mm256_broadcast_ss(&x);

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				_mm256_storeu_ps(pC, _mm256_mul_ps(vs, _mm256_loadu_ps(pA)));
				pC += 8; pA += 8;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = x * (*pA);
				break;

			case 2:
				*pC = x * (*pA);
				*(pC + 1) = x * (*(pA + 1));
				break;

			case 3:
				*pC = x * (*pA);
				*(pC + 1) = x * (*(pA + 1));
				*(pC + 2) = x * (*(pA + 2));
				break;

			case 4:
				_mm_storeu_ps(pC, _mm_mul_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA)));
				break;

			case 5:
				_mm_storeu_ps(pC, _mm_mul_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA)));
				*(pC + 4) = x * (*(pA + 4));
				break;

			case 6:
				_mm_storeu_ps(pC, _mm_mul_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA)));
				*(pC + 4) = x * (*(pA + 4));
				*(pC + 5) = x * (*(pA + 5));
				break;

			case 7:
				_mm_storeu_ps(pC, _mm_mul_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA)));
				*(pC + 4) = x * (*(pA + 4));
				*(pC + 5) = x * (*(pA + 5));
				*(pC + 6) = x * (*(pA + 6));
				break;
			}
		}
		inline void psxm(float* const pfC, const float x, const float* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 7;
			float* pC = pfC; const float* pA = pfA;

			__m256 vs = _mm256_broadcast_ss(&x);

			for (std::size_t k = n - r; k > 0; k -= 8)
			{
				_mm256_storeu_ps(pC, _mm256_fmadd_ps(vs, _mm256_loadu_ps(pA), _mm256_loadu_ps(pC)));
				pC += 8; pA += 8;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC += x * (*pA);
				break;

			case 2:
				*pC += x * (*pA);
				*(pC + 1) += x * (*(pA + 1));
				break;

			case 3:
				*pC += x * (*pA);
				*(pC + 1) += x * (*(pA + 1));
				*(pC + 2) += x * (*(pA + 2));
				break;

			case 4:
				_mm_storeu_ps(pC, _mm_fmadd_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA), _mm_loadu_ps(pC)));
				break;

			case 5:
				_mm_storeu_ps(pC, _mm_fmadd_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA), _mm_loadu_ps(pC)));
				*(pC + 4) += x * (*(pA + 4));
				break;

			case 6:
				_mm_storeu_ps(pC, _mm_fmadd_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA), _mm_loadu_ps(pC)));
				*(pC + 4) += x * (*(pA + 4));
				*(pC + 5) += x * (*(pA + 5));
				break;

			case 7:
				_mm_storeu_ps(pC, _mm_fmadd_ps(_mm_broadcast_ss(&x),
					_mm_loadu_ps(pA), _mm_loadu_ps(pC)));
				*(pC + 4) += x * (*(pA + 4));
				*(pC + 5) += x * (*(pA + 5));
				*(pC + 6) += x * (*(pA + 6));
				break;
			}
		}
		void pmxm(float* const pfC, const float* const pfA, const float* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept
		{
			if (Bc == 1)
			{
				float acc0; float acc1; float acc2; float acc3; float regB;
				const float* pA0 = pfA;
				const float* pA1 = pfA + Ac;
				const float* pA2 = pfA + 2 * Ac;
				const float* pA3 = pfA + 3 * Ac;
				const float* pB; float* pC = pfC;
				std::size_t N = Ar - (Ar & 3);
				std::size_t M = Ac - (Ac & 7);
				std::size_t jump = 3 * Ac;

				std::size_t i, k;

				__m256 vacc0; __m256 vacc1; __m256 vacc2; __m256 vacc3; __m256 vB;
				union { __m256 v; float arr[8]; } varr;

				// major upper part of C
				for (i = N; i > 0; i -= 4)
				{
					acc0 = 0.0f; acc1 = 0.0f; acc2 = 0.0f; acc3 = 0.0f;
					vacc0 = _mm256_setzero_ps();
					vacc1 = _mm256_setzero_ps();
					vacc2 = _mm256_setzero_ps();
					vacc3 = _mm256_setzero_ps();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vB = _mm256_loadu_ps(pB); pB += 8;
						vacc0 = _mm256_fmadd_ps(_mm256_loadu_ps(pA0), vB, vacc0); pA0 += 8;
						vacc1 = _mm256_fmadd_ps(_mm256_loadu_ps(pA1), vB, vacc1); pA1 += 8;
						vacc2 = _mm256_fmadd_ps(_mm256_loadu_ps(pA2), vB, vacc2); pA2 += 8;
						vacc3 = _mm256_fmadd_ps(_mm256_loadu_ps(pA3), vB, vacc3); pA3 += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
						acc3 += (*pA3++) * regB;
					}
					vacc0 = _mm256_hadd_ps(vacc0, vacc0); varr.v = _mm256_hadd_ps(vacc0, vacc0);
					*pC += acc0 + varr.arr[0] + varr.arr[4];
					vacc1 = _mm256_hadd_ps(vacc1, vacc1); varr.v = _mm256_hadd_ps(vacc1, vacc1);
					*(pC + 1) += acc1 + varr.arr[0] + varr.arr[4];
					vacc2 = _mm256_hadd_ps(vacc2, vacc2); varr.v = _mm256_hadd_ps(vacc2, vacc2);
					*(pC + 2) += acc2 + varr.arr[0] + varr.arr[4];
					vacc3 = _mm256_hadd_ps(vacc3, vacc3); varr.v = _mm256_hadd_ps(vacc3, vacc3);
					*(pC + 3) += acc3 + varr.arr[0] + varr.arr[4];
					pC += 4;

					pA0 += jump;
					pA1 += jump;
					pA2 += jump;
					pA3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					acc0 = 0.0f;
					vacc0 = _mm256_setzero_ps();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vacc0 = _mm256_fmadd_ps(_mm256_loadu_ps(pA0), _mm256_loadu_ps(pB), vacc0);
						pA0 += 8; pB += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
					}
					vacc0 = _mm256_hadd_ps(vacc0, vacc0); varr.v = _mm256_hadd_ps(vacc0, vacc0);
					*pC += acc0 + varr.arr[0] + varr.arr[4];
					break;

				case 2:
					acc0 = 0.0f; acc1 = 0.0f;
					vacc0 = _mm256_setzero_ps();
					vacc1 = _mm256_setzero_ps();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vB = _mm256_loadu_ps(pB); pB += 8;
						vacc0 = _mm256_fmadd_ps(_mm256_loadu_ps(pA0), vB, vacc0); pA0 += 8;
						vacc1 = _mm256_fmadd_ps(_mm256_loadu_ps(pA1), vB, vacc1); pA1 += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
					}
					vacc0 = _mm256_hadd_ps(vacc0, vacc0); varr.v = _mm256_hadd_ps(vacc0, vacc0);
					*pC += acc0 + varr.arr[0] + varr.arr[4];
					vacc1 = _mm256_hadd_ps(vacc1, vacc1); varr.v = _mm256_hadd_ps(vacc1, vacc1);
					*(pC + 1) += acc1 + varr.arr[0] + varr.arr[4];
					break;

				case 3:
					acc0 = 0.0f; acc1 = 0.0f; acc2 = 0.0f;
					vacc0 = _mm256_setzero_ps();
					vacc1 = _mm256_setzero_ps();
					vacc2 = _mm256_setzero_ps();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vB = _mm256_loadu_ps(pB); pB += 8;
						vacc0 = _mm256_fmadd_ps(_mm256_loadu_ps(pA0), vB, vacc0); pA0 += 8;
						vacc1 = _mm256_fmadd_ps(_mm256_loadu_ps(pA1), vB, vacc1); pA1 += 8;
						vacc2 = _mm256_fmadd_ps(_mm256_loadu_ps(pA2), vB, vacc2); pA2 += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
					}
					vacc0 = _mm256_hadd_ps(vacc0, vacc0); varr.v = _mm256_hadd_ps(vacc0, vacc0);
					*pC += acc0 + varr.arr[0] + varr.arr[4];
					vacc1 = _mm256_hadd_ps(vacc1, vacc1); varr.v = _mm256_hadd_ps(vacc1, vacc1);
					*(pC + 1) += acc1 + varr.arr[0] + varr.arr[4];
					vacc2 = _mm256_hadd_ps(vacc2, vacc2); varr.v = _mm256_hadd_ps(vacc2, vacc2);
					*(pC + 2) += acc2 + varr.arr[0] + varr.arr[4];
					break;
				}
			}

			else if (Ar == 1)
			{
				float regA0; float regA1; float regA2; float regA3;
				const float* pA = pfA;
				const float* pB0 = pfB;
				const float* pB1 = pfB + Bc;
				const float* pB2 = pfB + 2 * Bc;
				const float* pB3 = pfB + 3 * Bc;
				float* pC;

				std::size_t N = Ac - (Ac & 3);
				std::size_t M = Bc - (Bc & 7);
				std::size_t jump = 3 * Bc;

				std::size_t j, k;

				__m256 vregA0; __m256 vregA1; __m256 vregA2; __m256 vregA3; __m256 vregC;

				// major upper part of B
				for (k = N; k > 0; k -= 4)
				{
					pC = pfC;

					vregA0 = _mm256_broadcast_ss(pA);
					vregA1 = _mm256_broadcast_ss(pA + 1);
					vregA2 = _mm256_broadcast_ss(pA + 2);
					vregA3 = _mm256_broadcast_ss(pA + 3);

					for (j = M; j > 0; j -= 8)
					{
						vregC = _mm256_loadu_ps(pC);
						vregC = _mm256_fmadd_ps(vregA0, _mm256_loadu_ps(pB0), vregC); pB0 += 8;
						vregC = _mm256_fmadd_ps(vregA1, _mm256_loadu_ps(pB1), vregC); pB1 += 8;
						vregC = _mm256_fmadd_ps(vregA2, _mm256_loadu_ps(pB2), vregC); pB2 += 8;
						vregC = _mm256_fmadd_ps(vregA3, _mm256_loadu_ps(pB3), vregC); pB3 += 8;
						_mm256_storeu_ps(pC, vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ += (regA0 * (*pB0++) + regA1 * (*pB1++))
							+ (regA2 * (*pB2++) + regA3 * (*pB3++));
					}
					pB0 += jump;
					pB1 += jump;
					pB2 += jump;
					pB3 += jump;
				}

				// bottom fringe of B
				switch (Ac & 3)
				{

				case 0:
					break;

				case 1:
					pC = pfC;
					vregA0 = _mm256_broadcast_ss(pA);

					for (j = M; j > 0; j -= 8)
					{
						_mm256_storeu_ps(pC, _mm256_fmadd_ps(vregA0,
							_mm256_loadu_ps(pB0), _mm256_loadu_ps(pC)));
						pB0 += 8; pC += 8;
					}
					regA0 = *pA;
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ += regA0 * (*pB0++);
					}
					break;

				case 2:
					pC = pfC;
					vregA0 = _mm256_broadcast_ss(pA);
					vregA1 = _mm256_broadcast_ss(pA + 1);

					for (j = M; j > 0; j -= 8)
					{
						vregC = _mm256_loadu_ps(pC);
						vregC = _mm256_fmadd_ps(vregA0, _mm256_loadu_ps(pB0), vregC); pB0 += 8;
						vregC = _mm256_fmadd_ps(vregA1, _mm256_loadu_ps(pB1), vregC); pB1 += 8;
						_mm256_storeu_ps(pC, vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ += regA0 * (*pB0++) + regA1 * (*pB1++);
					}
					break;

				case 3:
					pC = pfC;
					vregA0 = _mm256_broadcast_ss(pA);
					vregA1 = _mm256_broadcast_ss(pA + 1);
					vregA2 = _mm256_broadcast_ss(pA + 2);

					for (j = M; j > 0; j -= 8)
					{
						vregC = _mm256_loadu_ps(pC);
						vregC = _mm256_fmadd_ps(vregA0, _mm256_loadu_ps(pB0), vregC); pB0 += 8;
						vregC = _mm256_fmadd_ps(vregA1, _mm256_loadu_ps(pB1), vregC); pB1 += 8;
						vregC = _mm256_fmadd_ps(vregA2, _mm256_loadu_ps(pB2), vregC); pB2 += 8;
						_mm256_storeu_ps(pC, vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ += (regA0 * (*pB0++) + regA1 * (*pB1++)) + regA2 * (*pB2++);
					}
					break;
				}
				return;
			}

			else if (Ac == 1)
			{
				float regA0; float regA1; float regA2; float regA3; float regB;
				const float* pA = pfA;
				const float* pB;
				float* pC0 = pfC;
				float* pC1 = pfC + Bc;
				float* pC2 = pfC + 2 * Bc;
				float* pC3 = pfC + 3 * Bc;

				std::size_t N = Ar - (Ar & 3);
				std::size_t M = Bc - (Bc & 7);
				std::size_t jump = 3 * Bc;

				std::size_t i, j;

				__m256 vregA0; __m256 vregA1; __m256 vregA2; __m256 vregA3; __m256 vregB;

				// major upper part of B
				for (i = N; i > 0; i -= 4)
				{
					pB = pfB;
					vregA0 = _mm256_broadcast_ss(pA);
					vregA1 = _mm256_broadcast_ss(pA + 1);
					vregA2 = _mm256_broadcast_ss(pA + 2);
					vregA3 = _mm256_broadcast_ss(pA + 3);
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_ps(pB); pB += 8;
						_mm256_storeu_ps(pC0, _mm256_fmadd_ps(vregA0, vregB, _mm256_loadu_ps(pC0))); pC0 += 8;
						_mm256_storeu_ps(pC1, _mm256_fmadd_ps(vregA1, vregB, _mm256_loadu_ps(pC1))); pC1 += 8;
						_mm256_storeu_ps(pC2, _mm256_fmadd_ps(vregA2, vregB, _mm256_loadu_ps(pC2))); pC2 += 8;
						_mm256_storeu_ps(pC3, _mm256_fmadd_ps(vregA3, vregB, _mm256_loadu_ps(pC3))); pC3 += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ += regA0 * regB;
						*pC1++ += regA1 * regB;
						*pC2++ += regA2 * regB;
						*pC3++ += regA3 * regB;
					}
					pC0 += jump;
					pC1 += jump;
					pC2 += jump;
					pC3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					pB = pfB;
					vregA0 = _mm256_broadcast_ss(pA);
					for (j = M; j > 0; j -= 8)
					{
						_mm256_storeu_ps(pC0, _mm256_fmadd_ps(vregA0, _mm256_loadu_ps(pB), _mm256_loadu_ps(pC0)));
						pB += 8; pC0 += 8;
					}
					regA0 = *pA;
					for (j = Bc - M; j > 0; j--)
					{
						*pC0++ += regA0 * (*pB++);
					}
					break;

				case 2:
					pB = pfB;
					vregA0 = _mm256_broadcast_ss(pA);
					vregA1 = _mm256_broadcast_ss(pA + 1);
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_ps(pB); pB += 8;
						_mm256_storeu_ps(pC0, _mm256_fmadd_ps(vregA0, vregB, _mm256_loadu_ps(pC0))); pC0 += 8;
						_mm256_storeu_ps(pC1, _mm256_fmadd_ps(vregA1, vregB, _mm256_loadu_ps(pC1))); pC1 += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ += regA0 * regB;
						*pC1++ += regA1 * regB;
					}
					break;

				case 3:
					pB = pfB;
					vregA0 = _mm256_broadcast_ss(pA);
					vregA1 = _mm256_broadcast_ss(pA + 1);
					vregA2 = _mm256_broadcast_ss(pA + 2);
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_ps(pB); pB += 8;
						_mm256_storeu_ps(pC0, _mm256_fmadd_ps(vregA0, vregB, _mm256_loadu_ps(pC0))); pC0 += 8;
						_mm256_storeu_ps(pC1, _mm256_fmadd_ps(vregA1, vregB, _mm256_loadu_ps(pC1))); pC1 += 8;
						_mm256_storeu_ps(pC2, _mm256_fmadd_ps(vregA2, vregB, _mm256_loadu_ps(pC2))); pC2 += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ += regA0 * regB;
						*pC1++ += regA1 * regB;
						*pC2++ += regA2 * regB;
					}
					break;
				}
				return;
			}

			// if A and B are full matrices
			else
			{
				const float* pB; float* pC;

				const float* pA0; const float* pA1; const float* pA2; const float* pA3;
				const float* pA4; const float* pA5; const float* pA6; const float* pA7;

				float temp[8];

				std::size_t M = Ar - (Ar & 7);
				std::size_t N = Bc - (Bc & 7);
				std::size_t r = (Bc & 7) * sizeof(float);

				std::size_t i, j, k, iOut, jOut, kOut, imax, jmax, kmax;

				__m256 vacc0; __m256 vacc1; __m256 vacc2; __m256 vacc3;
				__m256 vacc4; __m256 vacc5; __m256 vacc6; __m256 vacc7;
				__m256 vregB;

				// major upper part of C
				for (iOut = 0; iOut < Ar; iOut += WELP_MATRIX_AVX_ps_mm_Ti)
				{
					imax = (iOut + WELP_MATRIX_AVX_ps_mm_Ti < Ar) ? iOut + WELP_MATRIX_AVX_ps_mm_Ti : M;
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (i = iOut; i < imax; i += 8)
							{
								for (j = jOut; j < jmax; j += 8)
								{
									pB = (pfB + j) + (Bc * kOut);
									pC = (pfC + j) + (Bc * i);

									pA0 = (pfA + kOut) + (Ac * i); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
									pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac; pA7 = pA0 + 7 * Ac;

									vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
									vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
									vacc4 = _mm256_loadu_ps(pC + 4 * Bc); vacc5 = _mm256_loadu_ps(pC + 5 * Bc);
									vacc6 = _mm256_loadu_ps(pC + 6 * Bc); vacc7 = _mm256_loadu_ps(pC + 7 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
									for (k = kOut; k < kmax; k++)
									{
										vregB = _mm256_loadu_ps(pB); pB += Bc;
										vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
										vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
										vacc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
										vacc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
										vacc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
										vacc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA5++), vregB, vacc5);
										vacc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA6++), vregB, vacc6);
										vacc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA7++), vregB, vacc7);
									}

									_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
									_mm256_storeu_ps(pC + 2 * Bc, vacc2); _mm256_storeu_ps(pC + 3 * Bc, vacc3);
									_mm256_storeu_ps(pC + 4 * Bc, vacc4); _mm256_storeu_ps(pC + 5 * Bc, vacc5);
									_mm256_storeu_ps(pC + 6 * Bc, vacc6); _mm256_storeu_ps(pC + 7 * Bc, vacc7);
								}
								if ((jmax == N) && (r != 0))
								{
									pB = (pfB + N) + (Bc * kOut);
									pC = (pfC + N) + (Bc * i);

									pA0 = (pfA + kOut) + (Ac * i); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
									pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac; pA7 = pA0 + 7 * Ac;

									vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
									vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
									vacc4 = _mm256_loadu_ps(pC + 4 * Bc); vacc5 = _mm256_loadu_ps(pC + 5 * Bc);
									vacc6 = _mm256_loadu_ps(pC + 6 * Bc); vacc7 = _mm256_loadu_ps(pC + 7 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
									for (k = kOut; k < kmax; k++)
									{
										vregB = _mm256_loadu_ps(pB); pB += Bc;
										vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
										vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
										vacc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
										vacc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
										vacc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
										vacc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA5++), vregB, vacc5);
										vacc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA6++), vregB, vacc6);
										vacc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA7++), vregB, vacc7);
									}

									_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc6); std::memcpy(pC + 6 * Bc, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc7); std::memcpy(pC + 7 * Bc, static_cast<float*>(temp), r);
								}
							}
						}
					}
				}

				// bottom fringe of C
				switch (Ar & 7)
				{

				case 0:
					break;

				case 1:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M);

								vacc0 = _mm256_loadu_ps(pC);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
								}

								_mm256_storeu_ps(pC, vacc0);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M);

								vacc0 = _mm256_loadu_ps(pC);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
							}
						}
					}
					break;

				case 2:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
								}

								_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
							}
						}
					}
					break;

				case 3:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
								}

								_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
								_mm256_storeu_ps(pC + 2 * Bc, vacc2);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<float*>(temp), r);
							}
						}
					}
					break;

				case 4:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
								}

								_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
								_mm256_storeu_ps(pC + 2 * Bc, vacc2); _mm256_storeu_ps(pC + 3 * Bc, vacc3);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<float*>(temp), r);
							}
						}
					}
					break;

				case 5:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
								vacc4 = _mm256_loadu_ps(pC + 4 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
									vacc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
								}

								_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
								_mm256_storeu_ps(pC + 2 * Bc, vacc2); _mm256_storeu_ps(pC + 3 * Bc, vacc3);
								_mm256_storeu_ps(pC + 4 * Bc, vacc4);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
								vacc4 = _mm256_loadu_ps(pC + 4 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
									vacc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<float*>(temp), r);
							}
						}
					}
					break;

				case 6:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
								vacc4 = _mm256_loadu_ps(pC + 4 * Bc); vacc5 = _mm256_loadu_ps(pC + 5 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
									vacc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
									vacc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA5++), vregB, vacc5);
								}

								_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
								_mm256_storeu_ps(pC + 2 * Bc, vacc2); _mm256_storeu_ps(pC + 3 * Bc, vacc3);
								_mm256_storeu_ps(pC + 4 * Bc, vacc4); _mm256_storeu_ps(pC + 5 * Bc, vacc5);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
								vacc4 = _mm256_loadu_ps(pC + 4 * Bc); vacc5 = _mm256_loadu_ps(pC + 5 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
									vacc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
									vacc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA5++), vregB, vacc5);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<float*>(temp), r);
							}
						}
					}
					break;

				case 7:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
								vacc4 = _mm256_loadu_ps(pC + 4 * Bc); vacc5 = _mm256_loadu_ps(pC + 5 * Bc);
								vacc6 = _mm256_loadu_ps(pC + 6 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
									vacc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
									vacc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA5++), vregB, vacc5);
									vacc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA6++), vregB, vacc6);
								}

								_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
								_mm256_storeu_ps(pC + 2 * Bc, vacc2); _mm256_storeu_ps(pC + 3 * Bc, vacc3);
								_mm256_storeu_ps(pC + 4 * Bc, vacc4); _mm256_storeu_ps(pC + 5 * Bc, vacc5);
								_mm256_storeu_ps(pC + 6 * Bc, vacc6);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
								vacc4 = _mm256_loadu_ps(pC + 4 * Bc); vacc5 = _mm256_loadu_ps(pC + 5 * Bc);
								vacc6 = _mm256_loadu_ps(pC + 6 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
									vacc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
									vacc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA5++), vregB, vacc5);
									vacc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(pA6++), vregB, vacc6);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc6); std::memcpy(pC + 6 * Bc, static_cast<float*>(temp), r);
							}
						}
					}
					break;
				}
			}
		}
		void p_mxm(float* const pfC, const float* const pfA, const float* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept
		{
			if (Bc == 1)
			{
				float acc0; float acc1; float acc2; float acc3; float regB;
				const float* pA0 = pfA;
				const float* pA1 = pfA + Ac;
				const float* pA2 = pfA + 2 * Ac;
				const float* pA3 = pfA + 3 * Ac;
				const float* pB; float* pC = pfC;
				std::size_t N = Ar - (Ar & 3);
				std::size_t M = Ac - (Ac & 7);
				std::size_t jump = 3 * Ac;

				std::size_t i, k;

				__m256 vacc0; __m256 vacc1; __m256 vacc2; __m256 vacc3; __m256 vB;
				union { __m256 v; float arr[8]; } varr;

				// major upper part of C
				for (i = N; i > 0; i -= 4)
				{
					acc0 = 0.0f; acc1 = 0.0f; acc2 = 0.0f; acc3 = 0.0f;
					vacc0 = _mm256_setzero_ps();
					vacc1 = _mm256_setzero_ps();
					vacc2 = _mm256_setzero_ps();
					vacc3 = _mm256_setzero_ps();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vB = _mm256_loadu_ps(pB); pB += 8;
						vacc0 = _mm256_fmadd_ps(_mm256_loadu_ps(pA0), vB, vacc0); pA0 += 8;
						vacc1 = _mm256_fmadd_ps(_mm256_loadu_ps(pA1), vB, vacc1); pA1 += 8;
						vacc2 = _mm256_fmadd_ps(_mm256_loadu_ps(pA2), vB, vacc2); pA2 += 8;
						vacc3 = _mm256_fmadd_ps(_mm256_loadu_ps(pA3), vB, vacc3); pA3 += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
						acc3 += (*pA3++) * regB;
					}
					vacc0 = _mm256_hadd_ps(vacc0, vacc0); varr.v = _mm256_hadd_ps(vacc0, vacc0);
					*pC -= acc0 + varr.arr[0] + varr.arr[4];
					vacc1 = _mm256_hadd_ps(vacc1, vacc1); varr.v = _mm256_hadd_ps(vacc1, vacc1);
					*(pC + 1) -= acc1 + varr.arr[0] + varr.arr[4];
					vacc2 = _mm256_hadd_ps(vacc2, vacc2); varr.v = _mm256_hadd_ps(vacc2, vacc2);
					*(pC + 2) -= acc2 + varr.arr[0] + varr.arr[4];
					vacc3 = _mm256_hadd_ps(vacc3, vacc3); varr.v = _mm256_hadd_ps(vacc3, vacc3);
					*(pC + 3) -= acc3 + varr.arr[0] + varr.arr[4];
					pC += 4;

					pA0 += jump;
					pA1 += jump;
					pA2 += jump;
					pA3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					acc0 = 0.0f;
					vacc0 = _mm256_setzero_ps();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vacc0 = _mm256_fmadd_ps(_mm256_loadu_ps(pA0), _mm256_loadu_ps(pB), vacc0);
						pA0 += 8; pB += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
					}
					vacc0 = _mm256_hadd_ps(vacc0, vacc0); varr.v = _mm256_hadd_ps(vacc0, vacc0);
					*pC -= acc0 + varr.arr[0] + varr.arr[4];
					break;

				case 2:
					acc0 = 0.0f; acc1 = 0.0f;
					vacc0 = _mm256_setzero_ps();
					vacc1 = _mm256_setzero_ps();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vB = _mm256_loadu_ps(pB); pB += 8;
						vacc0 = _mm256_fmadd_ps(_mm256_loadu_ps(pA0), vB, vacc0); pA0 += 8;
						vacc1 = _mm256_fmadd_ps(_mm256_loadu_ps(pA1), vB, vacc1); pA1 += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
					}
					vacc0 = _mm256_hadd_ps(vacc0, vacc0); varr.v = _mm256_hadd_ps(vacc0, vacc0);
					*pC -= acc0 + varr.arr[0] + varr.arr[4];
					vacc1 = _mm256_hadd_ps(vacc1, vacc1); varr.v = _mm256_hadd_ps(vacc1, vacc1);
					*(pC + 1) -= acc1 + varr.arr[0] + varr.arr[4];
					break;

				case 3:
					acc0 = 0.0f; acc1 = 0.0f; acc2 = 0.0f;
					vacc0 = _mm256_setzero_ps();
					vacc1 = _mm256_setzero_ps();
					vacc2 = _mm256_setzero_ps();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vB = _mm256_loadu_ps(pB); pB += 8;
						vacc0 = _mm256_fmadd_ps(_mm256_loadu_ps(pA0), vB, vacc0); pA0 += 8;
						vacc1 = _mm256_fmadd_ps(_mm256_loadu_ps(pA1), vB, vacc1); pA1 += 8;
						vacc2 = _mm256_fmadd_ps(_mm256_loadu_ps(pA2), vB, vacc2); pA2 += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
					}
					vacc0 = _mm256_hadd_ps(vacc0, vacc0); varr.v = _mm256_hadd_ps(vacc0, vacc0);
					*pC -= acc0 + varr.arr[0] + varr.arr[4];
					vacc1 = _mm256_hadd_ps(vacc1, vacc1); varr.v = _mm256_hadd_ps(vacc1, vacc1);
					*(pC + 1) -= acc1 + varr.arr[0] + varr.arr[4];
					vacc2 = _mm256_hadd_ps(vacc2, vacc2); varr.v = _mm256_hadd_ps(vacc2, vacc2);
					*(pC + 2) -= acc2 + varr.arr[0] + varr.arr[4];
					break;
				}
				return;
			}

			else if (Ar == 1)
			{
				float regA0; float regA1; float regA2; float regA3;
				const float* pA = pfA;
				const float* pB0 = pfB;
				const float* pB1 = pB0 + Bc;
				const float* pB2 = pB0 + 2 * Bc;
				const float* pB3 = pB0 + 3 * Bc;
				float* pC;

				std::size_t N = Ac - (Ac & 3);
				std::size_t M = Bc - (Bc & 7);
				std::size_t jump = 3 * Bc;

				std::size_t j, k;

				__m256 vregA0; __m256 vregA1; __m256 vregA2; __m256 vregA3; __m256 vregC;

				// major upper part of B
				for (k = N; k > 0; k -= 4)
				{
					pC = pfC;

					vregA0 = _mm256_broadcast_ss(pA);
					vregA1 = _mm256_broadcast_ss(pA + 1);
					vregA2 = _mm256_broadcast_ss(pA + 2);
					vregA3 = _mm256_broadcast_ss(pA + 3);

					for (j = M; j > 0; j -= 8)
					{
						vregC = _mm256_loadu_ps(pC);
						vregC = _mm256_fnmadd_ps(vregA0, _mm256_loadu_ps(pB0), vregC); pB0 += 8;
						vregC = _mm256_fnmadd_ps(vregA1, _mm256_loadu_ps(pB1), vregC); pB1 += 8;
						vregC = _mm256_fnmadd_ps(vregA2, _mm256_loadu_ps(pB2), vregC); pB2 += 8;
						vregC = _mm256_fnmadd_ps(vregA3, _mm256_loadu_ps(pB3), vregC); pB3 += 8;
						_mm256_storeu_ps(pC, vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ -= (regA0 * (*pB0++) + regA1 * (*pB1++))
							+ (regA2 * (*pB2++) + regA3 * (*pB3++));
					}
					pB0 += jump;
					pB1 += jump;
					pB2 += jump;
					pB3 += jump;
				}

				// bottom fringe of B
				switch (Ac & 3)
				{

				case 0:
					break;

				case 1:
					pC = pfC;
					vregA0 = _mm256_broadcast_ss(pA);

					for (j = M; j > 0; j -= 8)
					{
						_mm256_storeu_ps(pC, _mm256_fnmadd_ps(vregA0,
							_mm256_loadu_ps(pB0), _mm256_loadu_ps(pC)));
						pB0 += 8; pC += 8;
					}
					regA0 = *pA;
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ -= regA0 * (*pB0++);
					}
					break;

				case 2:
					pC = pfC;
					vregA0 = _mm256_broadcast_ss(pA);
					vregA1 = _mm256_broadcast_ss(pA + 1);

					for (j = M; j > 0; j -= 8)
					{
						vregC = _mm256_loadu_ps(pC);
						vregC = _mm256_fnmadd_ps(vregA0, _mm256_loadu_ps(pB0), vregC); pB0 += 8;
						vregC = _mm256_fnmadd_ps(vregA1, _mm256_loadu_ps(pB1), vregC); pB1 += 8;
						_mm256_storeu_ps(pC, vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ -= regA0 * (*pB0++) + regA1 * (*pB1++);
					}
					break;

				case 3:
					pC = pfC;
					vregA0 = _mm256_broadcast_ss(pA);
					vregA1 = _mm256_broadcast_ss(pA + 1);
					vregA2 = _mm256_broadcast_ss(pA + 2);

					for (j = M; j > 0; j -= 8)
					{
						vregC = _mm256_loadu_ps(pC);
						vregC = _mm256_fnmadd_ps(vregA0, _mm256_loadu_ps(pB0), vregC); pB0 += 8;
						vregC = _mm256_fnmadd_ps(vregA1, _mm256_loadu_ps(pB1), vregC); pB1 += 8;
						vregC = _mm256_fnmadd_ps(vregA2, _mm256_loadu_ps(pB2), vregC); pB2 += 8;
						_mm256_storeu_ps(pC, vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ -= (regA0 * (*pB0++) + regA1 * (*pB1++)) + regA2 * (*pB2++);
					}
					break;
				}
				return;
			}

			else if (Ac == 1)
			{
				float regA0; float regA1; float regA2; float regA3; float regB;
				const float* pA = pfA;
				const float* pB;
				float* pC0 = pfC;
				float* pC1 = pfC + Bc;
				float* pC2 = pfC + 2 * Bc;
				float* pC3 = pfC + 3 * Bc;

				std::size_t N = Ar - (Ar & 3);
				std::size_t M = Bc - (Bc & 7);
				std::size_t jump = 3 * Bc;

				std::size_t i, j;

				__m256 vregA0; __m256 vregA1; __m256 vregA2; __m256 vregA3; __m256 vregB;

				// major upper part of B
				for (i = N; i > 0; i -= 4)
				{
					pB = pfB;
					vregA0 = _mm256_broadcast_ss(pA);
					vregA1 = _mm256_broadcast_ss(pA + 1);
					vregA2 = _mm256_broadcast_ss(pA + 2);
					vregA3 = _mm256_broadcast_ss(pA + 3);
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_ps(pB); pB += 8;
						_mm256_storeu_ps(pC0, _mm256_fnmadd_ps(vregA0, vregB, _mm256_loadu_ps(pC0))); pC0 += 8;
						_mm256_storeu_ps(pC1, _mm256_fnmadd_ps(vregA1, vregB, _mm256_loadu_ps(pC1))); pC1 += 8;
						_mm256_storeu_ps(pC2, _mm256_fnmadd_ps(vregA2, vregB, _mm256_loadu_ps(pC2))); pC2 += 8;
						_mm256_storeu_ps(pC3, _mm256_fnmadd_ps(vregA3, vregB, _mm256_loadu_ps(pC3))); pC3 += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ -= regA0 * regB;
						*pC1++ -= regA1 * regB;
						*pC2++ -= regA2 * regB;
						*pC3++ -= regA3 * regB;
					}
					pC0 += jump;
					pC1 += jump;
					pC2 += jump;
					pC3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					pB = pfB;
					vregA0 = _mm256_broadcast_ss(pA);
					for (j = M; j > 0; j -= 8)
					{
						_mm256_storeu_ps(pC0, _mm256_fnmadd_ps(vregA0, _mm256_loadu_ps(pB), _mm256_loadu_ps(pC0)));
						pB += 8; pC0 += 8;
					}
					regA0 = *pA;
					for (j = Bc - M; j > 0; j--)
					{
						*pC0++ -= regA0 * (*pB++);
					}
					break;

				case 2:
					pB = pfB;
					vregA0 = _mm256_broadcast_ss(pA);
					vregA1 = _mm256_broadcast_ss(pA + 1);
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_ps(pB); pB += 8;
						_mm256_storeu_ps(pC0, _mm256_fnmadd_ps(vregA0, vregB, _mm256_loadu_ps(pC0))); pC0 += 8;
						_mm256_storeu_ps(pC1, _mm256_fnmadd_ps(vregA1, vregB, _mm256_loadu_ps(pC1))); pC1 += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ -= regA0 * regB;
						*pC1++ -= regA1 * regB;
					}
					break;

				case 3:
					pB = pfB;
					vregA0 = _mm256_broadcast_ss(pA);
					vregA1 = _mm256_broadcast_ss(pA + 1);
					vregA2 = _mm256_broadcast_ss(pA + 2);
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_ps(pB); pB += 8;
						_mm256_storeu_ps(pC0, _mm256_fnmadd_ps(vregA0, vregB, _mm256_loadu_ps(pC0))); pC0 += 8;
						_mm256_storeu_ps(pC1, _mm256_fnmadd_ps(vregA1, vregB, _mm256_loadu_ps(pC1))); pC1 += 8;
						_mm256_storeu_ps(pC2, _mm256_fnmadd_ps(vregA2, vregB, _mm256_loadu_ps(pC2))); pC2 += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ -= regA0 * regB;
						*pC1++ -= regA1 * regB;
						*pC2++ -= regA2 * regB;
					}
					break;
				}
				return;
			}

			// if A and B are full matrices
			else
			{
				const float* pB; float* pC;

				const float* pA0; const float* pA1; const float* pA2; const float* pA3;
				const float* pA4; const float* pA5; const float* pA6; const float* pA7;

				float temp[8];

				std::size_t M = Ar - (Ar & 7);
				std::size_t N = Bc - (Bc & 7);
				std::size_t r = (Bc & 7) * sizeof(float);

				std::size_t i, j, k, iOut, jOut, kOut, imax, jmax, kmax;

				__m256 vacc0; __m256 vacc1; __m256 vacc2; __m256 vacc3;
				__m256 vacc4; __m256 vacc5; __m256 vacc6; __m256 vacc7;
				__m256 vregB;

				// major upper part of C
				for (iOut = 0; iOut < Ar; iOut += WELP_MATRIX_AVX_ps_mm_Ti)
				{
					imax = (iOut + WELP_MATRIX_AVX_ps_mm_Ti < Ar) ? iOut + WELP_MATRIX_AVX_ps_mm_Ti : M;
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (i = iOut; i < imax; i += 8)
							{
								for (j = jOut; j < jmax; j += 8)
								{
									pB = (pfB + j) + (Bc * kOut);
									pC = (pfC + j) + (Bc * i);

									pA0 = (pfA + kOut) + (Ac * i); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
									pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac; pA7 = pA0 + 7 * Ac;

									vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
									vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
									vacc4 = _mm256_loadu_ps(pC + 4 * Bc); vacc5 = _mm256_loadu_ps(pC + 5 * Bc);
									vacc6 = _mm256_loadu_ps(pC + 6 * Bc); vacc7 = _mm256_loadu_ps(pC + 7 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
									for (k = kOut; k < kmax; k++)
									{
										vregB = _mm256_loadu_ps(pB); pB += Bc;
										vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
										vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
										vacc2 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
										vacc3 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
										vacc4 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
										vacc5 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA5++), vregB, vacc5);
										vacc6 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA6++), vregB, vacc6);
										vacc7 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA7++), vregB, vacc7);
									}

									_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
									_mm256_storeu_ps(pC + 2 * Bc, vacc2); _mm256_storeu_ps(pC + 3 * Bc, vacc3);
									_mm256_storeu_ps(pC + 4 * Bc, vacc4); _mm256_storeu_ps(pC + 5 * Bc, vacc5);
									_mm256_storeu_ps(pC + 6 * Bc, vacc6); _mm256_storeu_ps(pC + 7 * Bc, vacc7);
								}
								if ((jmax == N) && (r != 0))
								{
									pB = (pfB + N) + (Bc * kOut);
									pC = (pfC + N) + (Bc * i);

									pA0 = (pfA + kOut) + (Ac * i); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
									pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac; pA7 = pA0 + 7 * Ac;

									vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
									vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
									vacc4 = _mm256_loadu_ps(pC + 4 * Bc); vacc5 = _mm256_loadu_ps(pC + 5 * Bc);
									vacc6 = _mm256_loadu_ps(pC + 6 * Bc); vacc7 = _mm256_loadu_ps(pC + 7 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
									for (k = kOut; k < kmax; k++)
									{
										vregB = _mm256_loadu_ps(pB); pB += Bc;
										vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
										vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
										vacc2 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
										vacc3 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
										vacc4 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
										vacc5 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA5++), vregB, vacc5);
										vacc6 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA6++), vregB, vacc6);
										vacc7 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA7++), vregB, vacc7);
									}

									_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc6); std::memcpy(pC + 6 * Bc, static_cast<float*>(temp), r);
									_mm256_storeu_ps(static_cast<float*>(temp), vacc7); std::memcpy(pC + 7 * Bc, static_cast<float*>(temp), r);
								}
							}
						}
					}
				}

				// bottom fringe of C
				switch (Ar & 7)
				{

				case 0:
					break;

				case 1:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M);

								vacc0 = _mm256_loadu_ps(pC);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
								}

								_mm256_storeu_ps(pC, vacc0);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M);

								vacc0 = _mm256_loadu_ps(pC);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
							}
						}
					}
					break;

				case 2:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
								}

								_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
							}
						}
					}
					break;

				case 3:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
								}

								_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
								_mm256_storeu_ps(pC + 2 * Bc, vacc2);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<float*>(temp), r);
							}
						}
					}
					break;

				case 4:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
								}

								_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
								_mm256_storeu_ps(pC + 2 * Bc, vacc2); _mm256_storeu_ps(pC + 3 * Bc, vacc3);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<float*>(temp), r);
							}
						}
					}
					break;

				case 5:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
								vacc4 = _mm256_loadu_ps(pC + 4 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
									vacc4 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
								}

								_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
								_mm256_storeu_ps(pC + 2 * Bc, vacc2); _mm256_storeu_ps(pC + 3 * Bc, vacc3);
								_mm256_storeu_ps(pC + 4 * Bc, vacc4);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
								vacc4 = _mm256_loadu_ps(pC + 4 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
									vacc4 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<float*>(temp), r);
							}
						}
					}
					break;

				case 6:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
								vacc4 = _mm256_loadu_ps(pC + 4 * Bc); vacc5 = _mm256_loadu_ps(pC + 5 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
									vacc4 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
									vacc5 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA5++), vregB, vacc5);
								}

								_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
								_mm256_storeu_ps(pC + 2 * Bc, vacc2); _mm256_storeu_ps(pC + 3 * Bc, vacc3);
								_mm256_storeu_ps(pC + 4 * Bc, vacc4); _mm256_storeu_ps(pC + 5 * Bc, vacc5);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
								vacc4 = _mm256_loadu_ps(pC + 4 * Bc); vacc5 = _mm256_loadu_ps(pC + 5 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
									vacc4 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
									vacc5 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA5++), vregB, vacc5);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<float*>(temp), r);
							}
						}
					}
					break;

				case 7:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
								vacc4 = _mm256_loadu_ps(pC + 4 * Bc); vacc5 = _mm256_loadu_ps(pC + 5 * Bc);
								vacc6 = _mm256_loadu_ps(pC + 6 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
									vacc4 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
									vacc5 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA5++), vregB, vacc5);
									vacc6 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA6++), vregB, vacc6);
								}

								_mm256_storeu_ps(pC, vacc0); _mm256_storeu_ps(pC + Bc, vacc1);
								_mm256_storeu_ps(pC + 2 * Bc, vacc2); _mm256_storeu_ps(pC + 3 * Bc, vacc3);
								_mm256_storeu_ps(pC + 4 * Bc, vacc4); _mm256_storeu_ps(pC + 5 * Bc, vacc5);
								_mm256_storeu_ps(pC + 6 * Bc, vacc6);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac;

								vacc0 = _mm256_loadu_ps(pC); vacc1 = _mm256_loadu_ps(pC + Bc);
								vacc2 = _mm256_loadu_ps(pC + 2 * Bc); vacc3 = _mm256_loadu_ps(pC + 3 * Bc);
								vacc4 = _mm256_loadu_ps(pC + 4 * Bc); vacc5 = _mm256_loadu_ps(pC + 5 * Bc);
								vacc6 = _mm256_loadu_ps(pC + 6 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_ps(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA3++), vregB, vacc3);
									vacc4 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA4++), vregB, vacc4);
									vacc5 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA5++), vregB, vacc5);
									vacc6 = _mm256_fnmadd_ps(_mm256_broadcast_ss(pA6++), vregB, vacc6);
								}

								_mm256_storeu_ps(static_cast<float*>(temp), vacc0); std::memcpy(pC, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<float*>(temp), r);
								_mm256_storeu_ps(static_cast<float*>(temp), vacc6); std::memcpy(pC + 6 * Bc, static_cast<float*>(temp), r);
							}
						}
					}
					break;
				}
			}
		}
		void elim_gauss(float* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t slice) noexcept
		{
			float* p; float* q0; float* q1; float* q2; float* q3;
			float reg0; float reg1; float reg2; float reg3; float reg;
			float maxelem; float temp; bool pivot;
			std::size_t iOut = Ar - (Ar % slice);
			if (iOut != Ar) { iOut += slice; }
			std::size_t ur; std::size_t M; std::size_t jump;
			std::size_t i, ii, j, jj;

			__m256 vreg0; __m256 vreg1; __m256 vreg2; __m256 vreg3; __m256 vreg;

			while (iOut > 0)
			{
				iOut -= slice; j = 0;
				for (i = iOut; i < Ar - 1; i++)
				{
					if (j < Ac)
					{
						ur = (slice < Ar - 1 - i) ? slice : Ar - 1 - i;

						p = (pfA + j) + (Ac * i);
						q0 = p + Ac;
						q1 = p + Ac;
						maxelem = std::abs(*p);
						pivot = false;
						for (jj = 0; jj < ur; jj++)
						{
							temp = std::abs(*q1);
							if (temp > maxelem)
							{
								pivot = true;
								maxelem = temp;
								q0 = q1;
							}
							q1 += Ac;
						}

						M = Ac - j;

						if (pivot)
						{
							for (jj = M - (M & 7); jj > 0; jj -= 8)
							{
								vreg = _mm256_loadu_ps(p);
								_mm256_storeu_ps(p, _mm256_loadu_ps(q0));
								_mm256_storeu_ps(q0, vreg);
								p += 8; q0 += 8;
							}
							for (jj = M & 7; jj > 0; jj--)
							{
								temp = *p;
								*p = *q0;
								*q0 = temp;
								p++; q0++;
							}
						}

						p = (pfA + j) + (Ac * i);

						if (*p != 0.0f)
						{
							temp = 1.0f / *p;
							q0 = p + Ac;
							q1 = p + 2 * Ac;
							q2 = p + 3 * Ac;
							q3 = p + 4 * Ac;
							jump = j + 3 * Ac;
							for (ii = ur - (ur & 3); ii > 0; ii -= 4)
							{
								p = (pfA + j) + (Ac * i);
								reg0 = temp * (*q0);
								reg1 = temp * (*q1);
								reg2 = temp * (*q2);
								reg3 = temp * (*q3);
								vreg0 = _mm256_broadcast_ss(&reg0);
								vreg1 = _mm256_broadcast_ss(&reg1);
								vreg2 = _mm256_broadcast_ss(&reg2);
								vreg3 = _mm256_broadcast_ss(&reg3);
								for (jj = M - (M & 7); jj > 0; jj -= 8)
								{
									vreg = _mm256_loadu_ps(p); p += 8;
									_mm256_storeu_ps(q0, _mm256_fnmadd_ps(vreg0, vreg, _mm256_loadu_ps(q0))); q0 += 8;
									_mm256_storeu_ps(q1, _mm256_fnmadd_ps(vreg1, vreg, _mm256_loadu_ps(q1))); q1 += 8;
									_mm256_storeu_ps(q2, _mm256_fnmadd_ps(vreg2, vreg, _mm256_loadu_ps(q2))); q2 += 8;
									_mm256_storeu_ps(q3, _mm256_fnmadd_ps(vreg3, vreg, _mm256_loadu_ps(q3))); q3 += 8;
								}
								for (jj = M & 7; jj > 0; jj--)
								{
									reg = *p++;
									*q0++ -= reg0 * reg;
									*q1++ -= reg1 * reg;
									*q2++ -= reg2 * reg;
									*q3++ -= reg3 * reg;
								}
								q0 += jump;
								q1 += jump;
								q2 += jump;
								q3 += jump;
							}

							switch (ur & 3)
							{

							case 0:
								break;

							case 1:
								p = (pfA + j) + (Ac * i);
								reg0 = temp * (*q0);
								vreg0 = _mm256_broadcast_ss(&reg0);
								for (jj = M - (M & 7); jj > 0; jj -= 8)
								{
									_mm256_storeu_ps(q0, _mm256_fnmadd_ps(vreg0,
										_mm256_loadu_ps(p), _mm256_loadu_ps(q0)));
									p += 8; q0 += 8;
								}
								for (jj = M & 7; jj > 0; jj--)
								{
									*q0++ -= reg0 * (*p++);
								}
								break;

							case 2:
								p = (pfA + j) + (Ac * i);
								reg0 = temp * (*q0);
								reg1 = temp * (*q1);
								vreg0 = _mm256_broadcast_ss(&reg0);
								vreg1 = _mm256_broadcast_ss(&reg1);
								for (jj = M - (M & 7); jj > 0; jj -= 8)
								{
									vreg = _mm256_loadu_ps(p); p += 8;
									_mm256_storeu_ps(q0, _mm256_fnmadd_ps(vreg0, vreg, _mm256_loadu_ps(q0))); q0 += 8;
									_mm256_storeu_ps(q1, _mm256_fnmadd_ps(vreg1, vreg, _mm256_loadu_ps(q1))); q1 += 8;
								}
								for (jj = M & 7; jj > 0; jj--)
								{
									reg = *p++;
									*q0++ -= reg0 * reg;
									*q1++ -= reg1 * reg;
								}
								break;

							case 3:
								p = (pfA + j) + (Ac * i);
								reg0 = temp * (*q0);
								reg1 = temp * (*q1);
								reg2 = temp * (*q2);
								vreg0 = _mm256_broadcast_ss(&reg0);
								vreg1 = _mm256_broadcast_ss(&reg1);
								vreg2 = _mm256_broadcast_ss(&reg2);
								for (jj = M - (M & 7); jj > 0; jj -= 8)
								{
									vreg = _mm256_loadu_ps(p); p += 8;
									_mm256_storeu_ps(q0, _mm256_fnmadd_ps(vreg0, vreg, _mm256_loadu_ps(q0))); q0 += 8;
									_mm256_storeu_ps(q1, _mm256_fnmadd_ps(vreg1, vreg, _mm256_loadu_ps(q1))); q1 += 8;
									_mm256_storeu_ps(q2, _mm256_fnmadd_ps(vreg2, vreg, _mm256_loadu_ps(q2))); q2 += 8;
								}
								for (jj = M & 7; jj > 0; jj--)
								{
									reg = *p++;
									*q0++ -= reg0 * reg;
									*q1++ -= reg1 * reg;
									*q2++ -= reg2 * reg;
								}
								break;
							}
						}
						j++;
					}
					else { break; }
				}
			}

			std::size_t ndiag = (Ar < Ac) ? Ar : Ac;
			p = pfA + Ac;
			for (i = 1; i < ndiag; i++)
			{
				std::memset(p, 0, i * sizeof(float));
				p += Ac;
			}
			if (Ar > Ac)
			{
				std::memset(p, 0, Ac * (Ar - Ac) * sizeof(float));
			}
		}
		void elim_householder(float* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t Nc,
			float* const pfu, float* const pfv, const std::size_t slice) noexcept
		{
			float reg0; float reg1; float reg2; float reg3; float duu; float regv;
			float* pA0; float* pA1; float* pA2; float* pA3; float* pu; float* pv;
			std::size_t iOut = Ar - (Ar % slice);
			if (iOut != Ar) { iOut += slice; }
			std::size_t M; std::size_t jump;
			std::size_t i, j, ii, jj, ur;

			__m256 vreg; __m256 vreg0; __m256 vreg1; __m256 vreg2; __m256 vreg3;
			union { __m256 v; float arr[8]; } varr;

			while (iOut > 0)
			{
				iOut -= slice; j = 0;
				for (i = iOut; i < Ar - 1; i++)
				{
					if (j < Nc)
					{
						ur = (slice + 1 < Ar - i) ? slice + 1 : Ar - i;

						pA0 = (pfA + j) + (Ac * i);
						pu = pfu;
						for (ii = ur; ii > 0; ii--)
						{
							*pu++ = *pA0; pA0 += Ac;
						}

						duu = 0.0f;
						pu = pfu;
						vreg0 = _mm256_setzero_ps();
						for (ii = ur - (ur & 7); ii > 0; ii -= 8)
						{
							vreg0 = _mm256_fmadd_ps(_mm256_loadu_ps(pu), _mm256_loadu_ps(pu), vreg0);
							pu += 8;
						}
						vreg0 = _mm256_hadd_ps(vreg0, vreg0); varr.v = _mm256_hadd_ps(vreg0, vreg0);
						duu = varr.arr[0] + varr.arr[4];
						for (ii = ur & 7; ii > 0; ii--)
						{
							duu += (*pu) * (*pu); pu++;
						}

						pu = pfu;
						if (*pu > 0.0f) { *pu += std::sqrt(duu); }
						else { *pu -= std::sqrt(duu); }

						duu = 0.0f;
						vreg0 = _mm256_setzero_ps();
						for (ii = ur - (ur & 7); ii > 0; ii -= 8)
						{
							vreg0 = _mm256_fmadd_ps(_mm256_loadu_ps(pu), _mm256_loadu_ps(pu), vreg0);
							pu += 8;
						}
						vreg0 = _mm256_hadd_ps(vreg0, vreg0); varr.v = _mm256_hadd_ps(vreg0, vreg0);
						duu = varr.arr[0] + varr.arr[4];
						for (ii = ur & 7; ii > 0; ii--)
						{
							duu += (*pu) * (*pu); pu++;
						}

						if (duu != 0.0f)
						{
							duu = (-2.0f / duu);
							M = Ac - j;

							std::memset(pfv, 0, Ac * sizeof(float));
							pA0 = (pfA + j) + (Ac * i);
							pA1 = pA0 + Ac;
							pA2 = pA0 + 2 * Ac;
							pA3 = pA0 + 3 * Ac;
							pu = pfu;
							jump = j + 3 * Ac;
							for (ii = ur - (ur & 3); ii > 0; ii -= 4)
							{
								pv = pfv;
								vreg0 = _mm256_broadcast_ss(pu);
								vreg1 = _mm256_broadcast_ss(pu + 1);
								vreg2 = _mm256_broadcast_ss(pu + 2);
								vreg3 = _mm256_broadcast_ss(pu + 3);
								for (jj = M - (M & 7); jj > 0; jj -= 8)
								{
									vreg = _mm256_loadu_ps(pv);
									vreg = _mm256_fmadd_ps(vreg0, _mm256_loadu_ps(pA0), vreg); pA0 += 8;
									vreg = _mm256_fmadd_ps(vreg1, _mm256_loadu_ps(pA1), vreg); pA1 += 8;
									vreg = _mm256_fmadd_ps(vreg2, _mm256_loadu_ps(pA2), vreg); pA2 += 8;
									vreg = _mm256_fmadd_ps(vreg3, _mm256_loadu_ps(pA3), vreg); pA3 += 8;
									_mm256_storeu_ps(pv, vreg); pv += 8;
								}
								reg0 = *pu;
								reg1 = *(pu + 1);
								reg2 = *(pu + 2);
								reg3 = *(pu + 3);
								pu += 4;
								for (jj = M & 7; jj > 0; jj--)
								{
									*pv++ += (reg0 * (*pA0++) + reg1 * (*pA1++))
										+ (reg2 * (*pA2++) + reg3 * (*pA3++));
								}
								pA0 += jump;
								pA1 += jump;
								pA2 += jump;
								pA3 += jump;
							}

							switch (ur & 3)
							{

							case 0:
								break;

							case 1:
								pv = pfv;
								vreg0 = _mm256_broadcast_ss(pu);
								for (jj = M - (M & 7); jj > 0; jj -= 8)
								{
									_mm256_storeu_ps(pv, _mm256_fmadd_ps(vreg0,
										_mm256_loadu_ps(pA0), _mm256_loadu_ps(pv)));
									pA0 += 8; pv += 8;
								}
								reg0 = *pu;
								for (jj = M & 7; jj > 0; jj--)
								{
									*pv++ += reg0 * (*pA0++);
								}
								break;

							case 2:
								pv = pfv;
								vreg0 = _mm256_broadcast_ss(pu);
								vreg1 = _mm256_broadcast_ss(pu + 1);
								for (jj = M - (M & 7); jj > 0; jj -= 8)
								{
									vreg = _mm256_loadu_ps(pv);
									vreg = _mm256_fmadd_ps(vreg0, _mm256_loadu_ps(pA0), vreg); pA0 += 8;
									vreg = _mm256_fmadd_ps(vreg1, _mm256_loadu_ps(pA1), vreg); pA1 += 8;
									_mm256_storeu_ps(pv, vreg); pv += 8;
								}
								reg0 = *pu;
								reg1 = *(pu + 1);
								for (jj = M & 7; jj > 0; jj--)
								{
									*pv++ += (reg0 * (*pA0++) + reg1 * (*pA1++));
								}
								break;

							case 3:
								pv = pfv;
								vreg0 = _mm256_broadcast_ss(pu);
								vreg1 = _mm256_broadcast_ss(pu + 1);
								vreg2 = _mm256_broadcast_ss(pu + 2);
								for (jj = M - (M & 7); jj > 0; jj -= 8)
								{
									vreg = _mm256_loadu_ps(pv);
									vreg = _mm256_fmadd_ps(vreg0, _mm256_loadu_ps(pA0), vreg); pA0 += 8;
									vreg = _mm256_fmadd_ps(vreg1, _mm256_loadu_ps(pA1), vreg); pA1 += 8;
									vreg = _mm256_fmadd_ps(vreg2, _mm256_loadu_ps(pA2), vreg); pA2 += 8;
									_mm256_storeu_ps(pv, vreg); pv += 8;
								}
								reg0 = *pu;
								reg1 = *(pu + 1);
								reg2 = *(pu + 2);
								for (jj = M & 7; jj > 0; jj--)
								{
									*pv++ += (reg0 * (*pA0++) + reg1 * (*pA1++))
										+ (reg2 * (*pA2++));
								}
								break;
							}

							pv = pfv;
							vreg0 = _mm256_broadcast_ss(&duu);
							for (jj = M - (M & 7); jj > 0; jj -= 8)
							{
								_mm256_storeu_ps(pv, _mm256_mul_ps(vreg0, _mm256_loadu_ps(pv)));
								pv += 8;
							}
							for (jj = M & 7; jj > 0; jj--)
							{
								*pv++ *= duu;
							}

							pA0 = (pfA + j) + (Ac * i);
							pA1 = pA0 + Ac;
							pA2 = pA0 + 2 * Ac;
							pA3 = pA0 + 3 * Ac;
							pu = pfu;
							jump = j + 3 * Ac;
							for (ii = ur - (ur & 3); ii > 0; ii -= 4)
							{
								pv = pfv;
								vreg0 = _mm256_broadcast_ss(pu);
								vreg1 = _mm256_broadcast_ss(pu + 1);
								vreg2 = _mm256_broadcast_ss(pu + 2);
								vreg3 = _mm256_broadcast_ss(pu + 3);
								for (jj = M - (M & 7); jj > 0; jj -= 8)
								{
									vreg = _mm256_loadu_ps(pv); pv += 8;
									_mm256_storeu_ps(pA0, _mm256_fmadd_ps(vreg0, vreg, _mm256_loadu_ps(pA0))); pA0 += 8;
									_mm256_storeu_ps(pA1, _mm256_fmadd_ps(vreg1, vreg, _mm256_loadu_ps(pA1))); pA1 += 8;
									_mm256_storeu_ps(pA2, _mm256_fmadd_ps(vreg2, vreg, _mm256_loadu_ps(pA2))); pA2 += 8;
									_mm256_storeu_ps(pA3, _mm256_fmadd_ps(vreg3, vreg, _mm256_loadu_ps(pA3))); pA3 += 8;
								}
								reg0 = *pu;
								reg1 = *(pu + 1);
								reg2 = *(pu + 2);
								reg3 = *(pu + 3);
								pu += 4;
								for (jj = M & 7; jj > 0; jj--)
								{
									regv = *pv++;
									*pA0++ += reg0 * regv;
									*pA1++ += reg1 * regv;
									*pA2++ += reg2 * regv;
									*pA3++ += reg3 * regv;
								}
								pA0 += jump;
								pA1 += jump;
								pA2 += jump;
								pA3 += jump;
							}

							switch (ur & 3)
							{

							case 0:
								break;

							case 1:
								pv = pfv;
								vreg0 = _mm256_broadcast_ss(pu);
								for (jj = M - (M & 7); jj > 0; jj -= 8)
								{
									_mm256_storeu_ps(pA0, _mm256_fmadd_ps(vreg0,
										_mm256_loadu_ps(pv), _mm256_loadu_ps(pA0)));
									pA0 += 8; pv += 8;
								}
								reg0 = *pu;
								for (jj = M & 7; jj > 0; jj--)
								{
									*pA0++ += reg0 * (*pv++);
								}
								break;

							case 2:
								pv = pfv;
								vreg0 = _mm256_broadcast_ss(pu);
								vreg1 = _mm256_broadcast_ss(pu + 1);
								for (jj = M - (M & 7); jj > 0; jj -= 8)
								{
									vreg = _mm256_loadu_ps(pv); pv += 8;
									_mm256_storeu_ps(pA0, _mm256_fmadd_ps(vreg0, vreg, _mm256_loadu_ps(pA0))); pA0 += 8;
									_mm256_storeu_ps(pA1, _mm256_fmadd_ps(vreg1, vreg, _mm256_loadu_ps(pA1))); pA1 += 8;
								}
								reg0 = *pu;
								reg1 = *(pu + 1);
								for (jj = M & 7; jj > 0; jj--)
								{
									regv = *pv++;
									*pA0++ += reg0 * regv;
									*pA1++ += reg1 * regv;
								}
								break;

							case 3:
								pv = pfv;
								vreg0 = _mm256_broadcast_ss(pu);
								vreg1 = _mm256_broadcast_ss(pu + 1);
								vreg2 = _mm256_broadcast_ss(pu + 2);
								for (jj = M - (M & 7); jj > 0; jj -= 8)
								{
									vreg = _mm256_loadu_ps(pv); pv += 8;
									_mm256_storeu_ps(pA0, _mm256_fmadd_ps(vreg0, vreg, _mm256_loadu_ps(pA0))); pA0 += 8;
									_mm256_storeu_ps(pA1, _mm256_fmadd_ps(vreg1, vreg, _mm256_loadu_ps(pA1))); pA1 += 8;
									_mm256_storeu_ps(pA2, _mm256_fmadd_ps(vreg2, vreg, _mm256_loadu_ps(pA2))); pA2 += 8;
								}
								reg0 = *pu;
								reg1 = *(pu + 1);
								reg2 = *(pu + 2);
								for (jj = M & 7; jj > 0; jj--)
								{
									regv = *pv++;
									*pA0++ += reg0 * regv;
									*pA1++ += reg1 * regv;
									*pA2++ += reg2 * regv;
								}
								break;
							}
						}
						j++;
					}
					else { break; }
				}
			}

			std::size_t ndiag = (Ar < Nc) ? Ar : Nc;
			pA0 = pfA + Ac;
			for (i = 1; i < ndiag; i++)
			{
				std::memset(pA0, 0, i * sizeof(float));
				pA0 += Ac;
			}
			if (Ar > Nc)
			{
				for (i = Ar - Nc; i > 0; i--)
				{
					std::memset(pA0, 0, ndiag * sizeof(float));
					pA0 += Ac;
				}
			}
		}
		void elim_givens(float* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t Nc, const std::size_t slice) noexcept
		{
			float C; float S; float temp0; float temp1;
			float* p; float* q;
			std::size_t N;
			std::size_t i, j, iOut, jOut, jmax, jj;

			__m256 vp; __m256 vq; __m256 vC; __m256 vS;

			for (jOut = 0; jOut < Nc; jOut += slice)
			{
				jmax = (jOut + slice < Nc) ? jOut + slice : Nc;
				for (iOut = Ar - 1; iOut > jOut; iOut--)
				{
					for (j = jOut; j < jmax; j++)
					{
						i = iOut + (j - jOut);

						if (i < Ar)
						{
							q = (pfA + j) + (Ac * i);
							if (*q != 0.0f)
							{
								p = q - Ac;

								C = *p;
								S = *q;
								temp0 = 1.0f / std::sqrt(C * C + S * S);
								C *= temp0;
								S *= temp0;

								temp1 = *q;
								*q = 0.0f;
								*p = C * (*p) + S * temp1;
								q++; p++;
								N = Ac - (j + 1);
								vC = _mm256_broadcast_ss(&C);
								vS = _mm256_broadcast_ss(&S);

								for (jj = N - (N & 7); jj > 0; jj -= 8)
								{
									vp = _mm256_loadu_ps(p); vq = _mm256_loadu_ps(q);
									_mm256_storeu_ps(p, _mm256_fmadd_ps(vC, vp, _mm256_mul_ps(vS, vq))); p += 8;
									_mm256_storeu_ps(q, _mm256_fnmadd_ps(vS, vp, _mm256_mul_ps(vC, vq))); q += 8;
								}
								for (jj = N & 7; jj > 0; jj--)
								{
									temp0 = *p; temp1 = *q;
									*p++ = C * temp0 + S * temp1;
									*q++ = C * temp1 - S * temp0;
								}
							}
						}
					}
				}
			}
		}
		void trisolve(float* const pfX, const float* const pfU, const std::size_t Ur, const std::size_t Xc, const std::size_t slice) noexcept
		{
			std::size_t Uc = Ur + Xc;

			if (Xc == 1)
			{
				float acc;
				const float* pU; float* pX1;
				std::size_t k;
				std::size_t M;

				__m256 vreg;
				union { __m256 v; float arr[8]; } varr;

				for (std::size_t i = Ur - 1; i + 1 > 0; i--)
				{
					pU = (pfU + (i + 1)) + (Uc * i);
					pX1 = pfX + (i + 1);
					M = Ur - (i + 1);
					acc = 0.0f;
					vreg = _mm256_setzero_ps();

					for (k = M - (M & 7); k > 0; k -= 8)
					{
						vreg = _mm256_fnmadd_ps(_mm256_loadu_ps(pU),
							_mm256_loadu_ps(pX1), vreg);
						pU += 8; pX1 += 8;
					}
					vreg = _mm256_hadd_ps(vreg, vreg); varr.v = _mm256_hadd_ps(vreg, vreg);
					acc = varr.arr[0] + varr.arr[4];
					for (k = M & 7; k > 0; k--)
					{
						acc -= (*pU++) * (*pX1++);
					}

					acc += *pU;
					*(pfX + i) = acc / *((pfU + i) + (Uc * i));
				}
				return;
			}

			else
			{
				float temp;
				std::memset(pfX, 0, Ur * Xc * sizeof(float));
				const float* pU; float* pX1; float* pX2;
				std::size_t M = Xc - (Xc & 7);
				std::size_t i, j, ii, iOut, imin, ip;

				__m256 vtemp;

				for (iOut = Ur - 1; iOut < Ur; iOut -= slice)
				{
					imin = (iOut - slice < Ur) ? iOut - slice + 1 : 0;
					for (ip = iOut + 1; ip > imin; ip--)
					{
						i = ip - 1;
						pX1 = pfX + (Xc * i);
						pU = (pfU + Ur) + (Uc * i);
						for (j = M; j > 0; j -= 8)
						{
							_mm256_storeu_ps(pX1, _mm256_add_ps(_mm256_loadu_ps(pX1),
								_mm256_loadu_ps(pU)));
							pU += 8; pX1 += 8;
						}
						for (j = Xc - M; j > 0; j--)
						{
							*pX1++ += *pU++;
						}

						pX1 = pfX + (Xc * i);
						temp = 1.0f / *((pfU + i) + (Uc * i));
						vtemp = _mm256_broadcast_ss(&temp);
						for (j = M; j > 0; j -= 8)
						{
							_mm256_storeu_ps(pX1, _mm256_mul_ps(_mm256_loadu_ps(pX1), vtemp));
							pX1 += 8;
						}
						for (j = Xc - M; j > 0; j--)
						{
							(*pX1++) *= temp;
						}

						for (ii = imin; ii < i; ii++)
						{
							pX1 = pfX + (Xc * i);
							pX2 = pfX + (Xc * ii);
							temp = *((pfU + i) + (Uc * ii));
							vtemp = _mm256_broadcast_ss(&temp);
							for (j = M; j > 0; j -= 8)
							{
								_mm256_storeu_ps(pX2, _mm256_fnmadd_ps(vtemp,
									_mm256_loadu_ps(pX1), _mm256_loadu_ps(pX2)));
								pX1 += 8; pX2 += 8;
							}
							for (j = Xc - M; j > 0; j--)
							{
								*pX2++ -= temp * (*pX1++);
							}
						}
					}

					for (ii = 0; ii < imin; ii++)
					{
						for (i = imin; i < iOut + 1; i++)
						{
							pX1 = pfX + (Xc * i);
							pX2 = pfX + (Xc * ii);
							temp = *((pfU + i) + (Uc * ii));
							vtemp = _mm256_broadcast_ss(&temp);
							for (j = M; j > 0; j -= 8)
							{
								_mm256_storeu_ps(pX2, _mm256_fnmadd_ps(vtemp,
									_mm256_loadu_ps(pX1), _mm256_loadu_ps(pX2)));
								pX1 += 8; pX2 += 8;
							}
							for (j = Xc - M; j > 0; j--)
							{
								*pX2++ -= temp * (*pX1++);
							}
						}
					}
				}
			}
		}

		void pmxm(int* const pfC, const int* const pfA, const int* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept
		{
			if (Bc == 1)
			{
				int acc0; int acc1; int acc2; int acc3; int regB;
				const int* pA0 = pfA;
				const int* pA1 = pfA + Ac;
				const int* pA2 = pfA + 2 * Ac;
				const int* pA3 = pfA + 3 * Ac;
				const int* pB; int* pC = pfC;
				std::size_t N = Ar - (Ar & 3);
				std::size_t M = Ac - (Ac & 7);
				std::size_t jump = 3 * Ac;

				std::size_t i, k;

				__m256i vacc0; __m256i vacc1; __m256i vacc2; __m256i vacc3; __m256i vB;
				union { __m256i v; int arr[8]; } varr;

				// major upper part of C
				for (i = N; i > 0; i -= 4)
				{
					acc0 = 0; acc1 = 0; acc2 = 0; acc3 = 0;
					vacc0 = _mm256_setzero_si256();
					vacc1 = _mm256_setzero_si256();
					vacc2 = _mm256_setzero_si256();
					vacc3 = _mm256_setzero_si256();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB));
						vacc0 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA0)), vB), vacc0);
						vacc1 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA1)), vB), vacc1);
						vacc2 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA2)), vB), vacc2);
						vacc3 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA3)), vB), vacc3);
						pA0 += 8; pA1 += 8; pA2 += 8; pA3 += 8; pB += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
						acc3 += (*pA3++) * regB;
					}
					vacc0 = _mm256_hadd_epi32(vacc0, vacc0); varr.v = _mm256_hadd_epi32(vacc0, vacc0);
					*pC += acc0 + varr.arr[0] + varr.arr[4];
					vacc1 = _mm256_hadd_epi32(vacc1, vacc1); varr.v = _mm256_hadd_epi32(vacc1, vacc1);
					*(pC + 1) += acc1 + varr.arr[0] + varr.arr[4];
					vacc2 = _mm256_hadd_epi32(vacc2, vacc2); varr.v = _mm256_hadd_epi32(vacc2, vacc2);
					*(pC + 2) += acc2 + varr.arr[0] + varr.arr[4];
					vacc3 = _mm256_hadd_epi32(vacc3, vacc3); varr.v = _mm256_hadd_epi32(vacc3, vacc3);
					*(pC + 3) += acc3 + varr.arr[0] + varr.arr[4];
					pC += 4;

					pA0 += jump;
					pA1 += jump;
					pA2 += jump;
					pA3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					acc0 = 0;
					vacc0 = _mm256_setzero_si256();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vacc0 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA0)),
							_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB))), vacc0);
						pA0 += 8; pB += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
					}
					vacc0 = _mm256_hadd_epi32(vacc0, vacc0); varr.v = _mm256_hadd_epi32(vacc0, vacc0);
					*pC += acc0 + varr.arr[0] + varr.arr[4];
					break;

				case 2:
					acc0 = 0; acc1 = 0;
					vacc0 = _mm256_setzero_si256();
					vacc1 = _mm256_setzero_si256();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						vacc0 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA0)), vB), vacc0); pA0 += 8;
						vacc1 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA1)), vB), vacc1); pA1 += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
					}
					vacc0 = _mm256_hadd_epi32(vacc0, vacc0); varr.v = _mm256_hadd_epi32(vacc0, vacc0);
					*pC += acc0 + varr.arr[0] + varr.arr[4];
					vacc1 = _mm256_hadd_epi32(vacc1, vacc1); varr.v = _mm256_hadd_epi32(vacc1, vacc1);
					*(pC + 1) += acc1 + varr.arr[0] + varr.arr[4];
					break;

				case 3:
					acc0 = 0; acc1 = 0; acc2 = 0;
					vacc0 = _mm256_setzero_si256();
					vacc1 = _mm256_setzero_si256();
					vacc2 = _mm256_setzero_si256();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						vacc0 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA0)), vB), vacc0); pA0 += 8;
						vacc1 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA1)), vB), vacc1); pA1 += 8;
						vacc2 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA2)), vB), vacc2); pA2 += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
					}
					vacc0 = _mm256_hadd_epi32(vacc0, vacc0); varr.v = _mm256_hadd_epi32(vacc0, vacc0);
					*pC += acc0 + varr.arr[0] + varr.arr[4];
					vacc1 = _mm256_hadd_epi32(vacc1, vacc1); varr.v = _mm256_hadd_epi32(vacc1, vacc1);
					*(pC + 1) += acc1 + varr.arr[0] + varr.arr[4];
					vacc2 = _mm256_hadd_epi32(vacc2, vacc2); varr.v = _mm256_hadd_epi32(vacc2, vacc2);
					*(pC + 2) += acc2 + varr.arr[0] + varr.arr[4];
					break;
				}
			}

			else if (Ar == 1)
			{
				int regA0; int regA1; int regA2; int regA3;
				const int* pA = pfA;
				const int* pB0 = pfB;
				const int* pB1 = pfB + Bc;
				const int* pB2 = pfB + 2 * Bc;
				const int* pB3 = pfB + 3 * Bc;
				int* pC;

				std::size_t N = Ac - (Ac & 3);
				std::size_t M = Bc - (Bc & 7);
				std::size_t jump = 3 * Bc;

				std::size_t j, k;

				__m256i vregA0; __m256i vregA1; __m256i vregA2; __m256i vregA3; __m256i vregC;

				// major upper part of B
				for (k = N; k > 0; k -= 4)
				{
					pC = pfC;

					vregA0 = _mm256_set1_epi32(*pA);
					vregA1 = _mm256_set1_epi32(*(pA + 1));
					vregA2 = _mm256_set1_epi32(*(pA + 2));
					vregA3 = _mm256_set1_epi32(*(pA + 3));

					for (j = M; j > 0; j -= 8)
					{
						vregC = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC));
						vregC = _mm256_add_epi32(vregC, _mm256_mullo_epi32(vregA0, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB0)))); pB0 += 8;
						vregC = _mm256_add_epi32(vregC, _mm256_mullo_epi32(vregA1, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB1)))); pB1 += 8;
						vregC = _mm256_add_epi32(vregC, _mm256_mullo_epi32(vregA2, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB2)))); pB2 += 8;
						vregC = _mm256_add_epi32(vregC, _mm256_mullo_epi32(vregA3, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB3)))); pB3 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ += (regA0 * (*pB0++) + regA1 * (*pB1++))
							+ (regA2 * (*pB2++) + regA3 * (*pB3++));
					}
					pB0 += jump;
					pB1 += jump;
					pB2 += jump;
					pB3 += jump;
				}

				// bottom fringe of B
				switch (Ac & 3)
				{

				case 0:
					break;

				case 1:
					pC = pfC;
					vregA0 = _mm256_set1_epi32(*pA);

					for (j = M; j > 0; j -= 8)
					{
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), _mm256_add_epi32(_mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)),
							_mm256_mullo_epi32(vregA0, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB0)))));
						pB0 += 8; pC += 8;
					}
					regA0 = *pA;
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ += regA0 * (*pB0++);
					}
					break;

				case 2:
					pC = pfC;
					vregA0 = _mm256_set1_epi32(*pA);
					vregA1 = _mm256_set1_epi32(*(pA + 1));

					for (j = M; j > 0; j -= 8)
					{
						vregC = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC));
						vregC = _mm256_add_epi32(vregC, _mm256_mullo_epi32(vregA0, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB0)))); pB0 += 8;
						vregC = _mm256_add_epi32(vregC, _mm256_mullo_epi32(vregA1, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB1)))); pB1 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ += regA0 * (*pB0++) + regA1 * (*pB1++);
					}
					break;

				case 3:
					pC = pfC;
					vregA0 = _mm256_set1_epi32(*pA);
					vregA1 = _mm256_set1_epi32(*(pA + 1));
					vregA2 = _mm256_set1_epi32(*(pA + 2));

					for (j = M; j > 0; j -= 8)
					{
						vregC = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC));
						vregC = _mm256_add_epi32(vregC, _mm256_mullo_epi32(vregA0, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB0)))); pB0 += 8;
						vregC = _mm256_add_epi32(vregC, _mm256_mullo_epi32(vregA1, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB1)))); pB1 += 8;
						vregC = _mm256_add_epi32(vregC, _mm256_mullo_epi32(vregA2, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB2)))); pB2 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ += (regA0 * (*pB0++) + regA1 * (*pB1++)) + regA2 * (*pB2++);
					}
					break;
				}
				return;
			}

			else if (Ac == 1)
			{
				int regA0; int regA1; int regA2; int regA3; int regB;
				const int* pA = pfA;
				const int* pB;
				int* pC0 = pfC;
				int* pC1 = pfC + Bc;
				int* pC2 = pfC + 2 * Bc;
				int* pC3 = pfC + 3 * Bc;

				std::size_t N = Ar - (Ar & 3);
				std::size_t M = Bc - (Bc & 7);
				std::size_t jump = 3 * Bc;

				std::size_t i, j;

				__m256i vregA0; __m256i vregA1; __m256i vregA2; __m256i vregA3; __m256i vregB;

				// major upper part of B
				for (i = N; i > 0; i -= 4)
				{
					pB = pfB;
					vregA0 = _mm256_set1_epi32(*pA);
					vregA1 = _mm256_set1_epi32(*(pA + 1));
					vregA2 = _mm256_set1_epi32(*(pA + 2));
					vregA3 = _mm256_set1_epi32(*(pA + 3));
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC0), _mm256_add_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC0)), _mm256_mullo_epi32(vregA0, vregB))); pC0 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC1), _mm256_add_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC1)), _mm256_mullo_epi32(vregA1, vregB))); pC1 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC2), _mm256_add_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC2)), _mm256_mullo_epi32(vregA2, vregB))); pC2 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC3), _mm256_add_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC3)), _mm256_mullo_epi32(vregA3, vregB))); pC3 += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ += regA0 * regB;
						*pC1++ += regA1 * regB;
						*pC2++ += regA2 * regB;
						*pC3++ += regA3 * regB;
					}
					pC0 += jump;
					pC1 += jump;
					pC2 += jump;
					pC3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					pB = pfB;
					vregA0 = _mm256_set1_epi32(*pA);
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC0), _mm256_add_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC0)), _mm256_mullo_epi32(vregA0, vregB))); pC0 += 8;
					}
					regA0 = *pA;
					for (j = Bc - M; j > 0; j--)
					{
						*pC0++ += regA0 * (*pB++);
					}
					break;

				case 2:
					pB = pfB;
					vregA0 = _mm256_set1_epi32(*pA);
					vregA1 = _mm256_set1_epi32(*(pA + 1));
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC0), _mm256_add_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC0)), _mm256_mullo_epi32(vregA0, vregB))); pC0 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC1), _mm256_add_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC1)), _mm256_mullo_epi32(vregA1, vregB))); pC1 += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ += regA0 * regB;
						*pC1++ += regA1 * regB;
					}
					break;

				case 3:
					pB = pfB;
					vregA0 = _mm256_set1_epi32(*pA);
					vregA1 = _mm256_set1_epi32(*pA + 1);
					vregA2 = _mm256_set1_epi32(*pA + 2);
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC0), _mm256_add_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC0)), _mm256_mullo_epi32(vregA0, vregB))); pC0 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC1), _mm256_add_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC1)), _mm256_mullo_epi32(vregA1, vregB))); pC1 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC2), _mm256_add_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC2)), _mm256_mullo_epi32(vregA2, vregB))); pC2 += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ += regA0 * regB;
						*pC1++ += regA1 * regB;
						*pC2++ += regA2 * regB;
					}
					break;
				}
				return;
			}

			// if A and B are full matrices
			else
			{
				const int* pB; int* pC;

				const int* pA0; const int* pA1; const int* pA2; const int* pA3;
				const int* pA4; const int* pA5; const int* pA6; const int* pA7;

				int temp[8];

				std::size_t M = Ar - (Ar & 7);
				std::size_t N = Bc - (Bc & 7);
				std::size_t r = (Bc & 7) * sizeof(int);

				std::size_t i, j, k, iOut, jOut, kOut, imax, jmax, kmax;

				__m256i vacc0; __m256i vacc1; __m256i vacc2; __m256i vacc3;
				__m256i vacc4; __m256i vacc5; __m256i vacc6; __m256i vacc7;
				__m256i vregB;

				// major upper part of C
				for (iOut = 0; iOut < Ar; iOut += WELP_MATRIX_AVX_ps_mm_Ti)
				{
					imax = (iOut + WELP_MATRIX_AVX_ps_mm_Ti < Ar) ? iOut + WELP_MATRIX_AVX_ps_mm_Ti : M;
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (i = iOut; i < imax; i += 8)
							{
								for (j = jOut; j < jmax; j += 8)
								{
									pB = (pfB + j) + (Bc * kOut);
									pC = (pfC + j) + (Bc * i);

									pA0 = (pfA + kOut) + (Ac * i); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
									pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac; pA7 = pA0 + 7 * Ac;

									vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
									vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
									vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc)); vacc5 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc));
									vacc6 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 6 * Bc)); vacc7 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 7 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
									for (k = kOut; k < kmax; k++)
									{
										vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
										vacc0 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB), vacc0);
										vacc1 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB), vacc1);
										vacc2 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB), vacc2);
										vacc3 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB), vacc3);
										vacc4 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB), vacc4);
										vacc5 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA5++), vregB), vacc5);
										vacc6 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA6++), vregB), vacc6);
										vacc7 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA7++), vregB), vacc7);
									}

									_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc), vacc2); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc), vacc3);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc), vacc4); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc), vacc5);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 6 * Bc), vacc6); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 7 * Bc), vacc7);
								}
								if ((jmax == N) && (r != 0))
								{
									pB = (pfB + N) + (Bc * kOut);
									pC = (pfC + N) + (Bc * i);

									pA0 = (pfA + kOut) + (Ac * i); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
									pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac; pA7 = pA0 + 7 * Ac;

									vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
									vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
									vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc)); vacc5 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc));
									vacc6 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 6 * Bc)); vacc7 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 7 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
									for (k = kOut; k < kmax; k++)
									{
										vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
										vacc0 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB), vacc0);
										vacc1 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB), vacc1);
										vacc2 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB), vacc2);
										vacc3 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB), vacc3);
										vacc4 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB), vacc4);
										vacc5 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA5++), vregB), vacc5);
										vacc6 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA6++), vregB), vacc6);
										vacc7 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_set1_epi32(*pA7++), vregB), vacc7);
									}

									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc6); std::memcpy(pC + 6 * Bc, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc7); std::memcpy(pC + 7 * Bc, static_cast<int*>(temp), r);
								}
							}
						}
					}
				}

				// bottom fringe of C
				switch (Ar & 7)
				{

				case 0:
					break;

				case 1:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M);

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M);

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
							}
						}
					}
					break;

				case 2:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_add_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_add_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
							}
						}
					}
					break;

				case 3:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_add_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_add_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc), vacc2);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_add_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_add_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<int*>(temp), r);
							}
						}
					}
					break;

				case 4:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_add_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_add_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_add_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc), vacc2); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc), vacc3);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_add_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_add_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_add_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<int*>(temp), r);
							}
						}
					}
					break;

				case 5:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
								vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_add_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_add_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_add_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
									vacc4 = _mm256_add_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc), vacc2); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc), vacc3);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc), vacc4);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
								vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_add_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_add_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_add_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
									vacc4 = _mm256_add_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<int*>(temp), r);
							}
						}
					}
					break;

				case 6:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
								vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc)); vacc5 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_add_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_add_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_add_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
									vacc4 = _mm256_add_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
									vacc5 = _mm256_add_epi32(vacc5, _mm256_mullo_epi32(_mm256_set1_epi32(*pA5++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc), vacc2); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc), vacc3);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc), vacc4); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc), vacc5);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
								vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc)); vacc5 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_add_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_add_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_add_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
									vacc4 = _mm256_add_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
									vacc5 = _mm256_add_epi32(vacc5, _mm256_mullo_epi32(_mm256_set1_epi32(*pA5++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<int*>(temp), r);
							}
						}
					}
					break;

				case 7:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
								vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc)); vacc5 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc));
								vacc6 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 6 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_add_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_add_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_add_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
									vacc4 = _mm256_add_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
									vacc5 = _mm256_add_epi32(vacc5, _mm256_mullo_epi32(_mm256_set1_epi32(*pA5++), vregB));
									vacc6 = _mm256_add_epi32(vacc6, _mm256_mullo_epi32(_mm256_set1_epi32(*pA6++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc), vacc2); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc), vacc3);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc), vacc4); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc), vacc5);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 6 * Bc), vacc6);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
								vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc)); vacc5 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc));
								vacc6 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 6 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_add_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_add_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_add_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_add_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
									vacc4 = _mm256_add_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
									vacc5 = _mm256_add_epi32(vacc5, _mm256_mullo_epi32(_mm256_set1_epi32(*pA5++), vregB));
									vacc6 = _mm256_add_epi32(vacc6, _mm256_mullo_epi32(_mm256_set1_epi32(*pA6++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc6); std::memcpy(pC + 6 * Bc, static_cast<int*>(temp), r);
							}
						}
					}
					break;
				}
			}
		}
		void p_mxm(int* const pfC, const int* const pfA, const int* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept
		{
			if (Bc == 1)
			{
				int acc0; int acc1; int acc2; int acc3; int regB;
				const int* pA0 = pfA;
				const int* pA1 = pfA + Ac;
				const int* pA2 = pfA + 2 * Ac;
				const int* pA3 = pfA + 3 * Ac;
				const int* pB; int* pC = pfC;
				std::size_t N = Ar - (Ar & 3);
				std::size_t M = Ac - (Ac & 7);
				std::size_t jump = 3 * Ac;

				std::size_t i, k;

				__m256i vacc0; __m256i vacc1; __m256i vacc2; __m256i vacc3; __m256i vB;
				union { __m256i v; int arr[8]; } varr;

				// major upper part of C
				for (i = N; i > 0; i -= 4)
				{
					acc0 = 0; acc1 = 0; acc2 = 0; acc3 = 0;
					vacc0 = _mm256_setzero_si256();
					vacc1 = _mm256_setzero_si256();
					vacc2 = _mm256_setzero_si256();
					vacc3 = _mm256_setzero_si256();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						vacc0 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA0)), vB), vacc0); pA0 += 8;
						vacc1 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA1)), vB), vacc1); pA1 += 8;
						vacc2 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA2)), vB), vacc2); pA2 += 8;
						vacc3 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA3)), vB), vacc3); pA3 += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
						acc3 += (*pA3++) * regB;
					}
					vacc0 = _mm256_hadd_epi32(vacc0, vacc0); varr.v = _mm256_hadd_epi32(vacc0, vacc0);
					*pC -= acc0 + varr.arr[0] + varr.arr[4];
					vacc1 = _mm256_hadd_epi32(vacc1, vacc1); varr.v = _mm256_hadd_epi32(vacc1, vacc1);
					*(pC + 1) -= acc1 + varr.arr[0] + varr.arr[4];
					vacc2 = _mm256_hadd_epi32(vacc2, vacc2); varr.v = _mm256_hadd_epi32(vacc2, vacc2);
					*(pC + 2) -= acc2 + varr.arr[0] + varr.arr[4];
					vacc3 = _mm256_hadd_epi32(vacc3, vacc3); varr.v = _mm256_hadd_epi32(vacc3, vacc3);
					*(pC + 3) -= acc3 + varr.arr[0] + varr.arr[4];
					pC += 4;

					pA0 += jump;
					pA1 += jump;
					pA2 += jump;
					pA3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					acc0 = 0;
					vacc0 = _mm256_setzero_si256();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vacc0 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA0)),
							_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB))), vacc0);
						pA0 += 8; pB += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
					}
					vacc0 = _mm256_hadd_epi32(vacc0, vacc0); varr.v = _mm256_hadd_epi32(vacc0, vacc0);
					*pC -= acc0 + varr.arr[0] + varr.arr[4];
					break;

				case 2:
					acc0 = 0; acc1 = 0;
					vacc0 = _mm256_setzero_si256();
					vacc1 = _mm256_setzero_si256();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						vacc0 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA0)), vB), vacc0); pA0 += 8;
						vacc1 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA1)), vB), vacc1); pA1 += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
					}
					vacc0 = _mm256_hadd_epi32(vacc0, vacc0); varr.v = _mm256_hadd_epi32(vacc0, vacc0);
					*pC -= acc0 + varr.arr[0] + varr.arr[4];
					vacc1 = _mm256_hadd_epi32(vacc1, vacc1); varr.v = _mm256_hadd_epi32(vacc1, vacc1);
					*(pC + 1) -= acc1 + varr.arr[0] + varr.arr[4];
					break;

				case 3:
					acc0 = 0; acc1 = 0; acc2 = 0;
					vacc0 = _mm256_setzero_si256();
					vacc1 = _mm256_setzero_si256();
					vacc2 = _mm256_setzero_si256();
					pB = pfB;

					for (k = M; k > 0; k -= 8)
					{
						vB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						vacc0 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA0)), vB), vacc0); pA0 += 8;
						vacc1 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA1)), vB), vacc1); pA1 += 8;
						vacc2 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(pA2)), vB), vacc2); pA2 += 8;
					}
					for (k = Ac & 7; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
					}
					vacc0 = _mm256_hadd_epi32(vacc0, vacc0); varr.v = _mm256_hadd_epi32(vacc0, vacc0);
					*pC -= acc0 + varr.arr[0] + varr.arr[4];
					vacc1 = _mm256_hadd_epi32(vacc1, vacc1); varr.v = _mm256_hadd_epi32(vacc1, vacc1);
					*(pC + 1) -= acc1 + varr.arr[0] + varr.arr[4];
					vacc2 = _mm256_hadd_epi32(vacc2, vacc2); varr.v = _mm256_hadd_epi32(vacc2, vacc2);
					*(pC + 2) -= acc2 + varr.arr[0] + varr.arr[4];
					break;
				}
			}

			else if (Ar == 1)
			{
				int regA0; int regA1; int regA2; int regA3;
				const int* pA = pfA;
				const int* pB0 = pfB;
				const int* pB1 = pfB + Bc;
				const int* pB2 = pfB + 2 * Bc;
				const int* pB3 = pfB + 3 * Bc;
				int* pC;

				std::size_t N = Ac - (Ac & 3);
				std::size_t M = Bc - (Bc & 7);
				std::size_t jump = 3 * Bc;

				std::size_t j, k;

				__m256i vregA0; __m256i vregA1; __m256i vregA2; __m256i vregA3; __m256i vregC;

				// major upper part of B
				for (k = N; k > 0; k -= 4)
				{
					pC = pfC;

					vregA0 = _mm256_set1_epi32(*pA);
					vregA1 = _mm256_set1_epi32(*(pA + 1));
					vregA2 = _mm256_set1_epi32(*(pA + 2));
					vregA3 = _mm256_set1_epi32(*(pA + 3));

					for (j = M; j > 0; j -= 8)
					{
						vregC = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC));
						vregC = _mm256_sub_epi32(vregC, _mm256_mullo_epi32(vregA0, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB0)))); pB0 += 8;
						vregC = _mm256_sub_epi32(vregC, _mm256_mullo_epi32(vregA1, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB1)))); pB1 += 8;
						vregC = _mm256_sub_epi32(vregC, _mm256_mullo_epi32(vregA2, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB2)))); pB2 += 8;
						vregC = _mm256_sub_epi32(vregC, _mm256_mullo_epi32(vregA3, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB3)))); pB3 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ -= (regA0 * (*pB0++) + regA1 * (*pB1++))
							+ (regA2 * (*pB2++) + regA3 * (*pB3++));
					}
					pB0 += jump;
					pB1 += jump;
					pB2 += jump;
					pB3 += jump;
				}

				// bottom fringe of B
				switch (Ac & 3)
				{

				case 0:
					break;

				case 1:
					pC = pfC;
					vregA0 = _mm256_set1_epi32(*pA);

					for (j = M; j > 0; j -= 8)
					{
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), _mm256_sub_epi32(_mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)),
							_mm256_mullo_epi32(vregA0, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB0)))));
						pB0 += 8; pC += 8;
					}
					regA0 = *pA;
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ -= regA0 * (*pB0++);
					}
					break;

				case 2:
					pC = pfC;
					vregA0 = _mm256_set1_epi32(*pA);
					vregA1 = _mm256_set1_epi32(*(pA + 1));

					for (j = M; j > 0; j -= 8)
					{
						vregC = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC));
						vregC = _mm256_sub_epi32(vregC, _mm256_mullo_epi32(vregA0, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB0)))); pB0 += 8;
						vregC = _mm256_sub_epi32(vregC, _mm256_mullo_epi32(vregA1, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB1)))); pB1 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ -= regA0 * (*pB0++) + regA1 * (*pB1++);
					}
					break;

				case 3:
					pC = pfC;
					vregA0 = _mm256_set1_epi32(*pA);
					vregA1 = _mm256_set1_epi32(*(pA + 1));
					vregA2 = _mm256_set1_epi32(*(pA + 2));

					for (j = M; j > 0; j -= 8)
					{
						vregC = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC));
						vregC = _mm256_sub_epi32(vregC, _mm256_mullo_epi32(vregA0, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB0)))); pB0 += 8;
						vregC = _mm256_sub_epi32(vregC, _mm256_mullo_epi32(vregA1, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB1)))); pB1 += 8;
						vregC = _mm256_sub_epi32(vregC, _mm256_mullo_epi32(vregA2, _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB2)))); pB2 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ -= (regA0 * (*pB0++) + regA1 * (*pB1++)) + regA2 * (*pB2++);
					}
					break;
				}
				return;
			}

			else if (Ac == 1)
			{
				int regA0; int regA1; int regA2; int regA3; int regB;
				const int* pA = pfA;
				const int* pB;
				int* pC0 = pfC;
				int* pC1 = pfC + Bc;
				int* pC2 = pfC + 2 * Bc;
				int* pC3 = pfC + 3 * Bc;

				std::size_t N = Ar - (Ar & 3);
				std::size_t M = Bc - (Bc & 7);
				std::size_t jump = 3 * Bc;

				std::size_t i, j;

				__m256i vregA0; __m256i vregA1; __m256i vregA2; __m256i vregA3; __m256i vregB;

				// major upper part of B
				for (i = N; i > 0; i -= 4)
				{
					pB = pfB;
					vregA0 = _mm256_set1_epi32(*pA);
					vregA1 = _mm256_set1_epi32(*(pA + 1));
					vregA2 = _mm256_set1_epi32(*(pA + 2));
					vregA3 = _mm256_set1_epi32(*(pA + 3));
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC0), _mm256_sub_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC0)), _mm256_mullo_epi32(vregA0, vregB))); pC0 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC1), _mm256_sub_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC1)), _mm256_mullo_epi32(vregA1, vregB))); pC1 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC2), _mm256_sub_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC2)), _mm256_mullo_epi32(vregA2, vregB))); pC2 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC3), _mm256_sub_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC3)), _mm256_mullo_epi32(vregA3, vregB))); pC3 += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ -= regA0 * regB;
						*pC1++ -= regA1 * regB;
						*pC2++ -= regA2 * regB;
						*pC3++ -= regA3 * regB;
					}
					pC0 += jump;
					pC1 += jump;
					pC2 += jump;
					pC3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					pB = pfB;
					vregA0 = _mm256_set1_epi32(*pA);
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC0), _mm256_sub_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC0)), _mm256_mullo_epi32(vregA0, vregB))); pC0 += 8;
					}
					regA0 = *pA;
					for (j = Bc - M; j > 0; j--)
					{
						*pC0++ -= regA0 * (*pB++);
					}
					break;

				case 2:
					pB = pfB;
					vregA0 = _mm256_set1_epi32(*pA);
					vregA1 = _mm256_set1_epi32(*(pA + 1));
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC0), _mm256_sub_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC0)), _mm256_mullo_epi32(vregA0, vregB))); pC0 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC1), _mm256_sub_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC1)), _mm256_mullo_epi32(vregA1, vregB))); pC1 += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ -= regA0 * regB;
						*pC1++ -= regA1 * regB;
					}
					break;

				case 3:
					pB = pfB;
					vregA0 = _mm256_set1_epi32(*pA);
					vregA1 = _mm256_set1_epi32(*pA + 1);
					vregA2 = _mm256_set1_epi32(*pA + 2);
					for (j = M; j > 0; j -= 8)
					{
						vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC0), _mm256_sub_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC0)), _mm256_mullo_epi32(vregA0, vregB))); pC0 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC1), _mm256_sub_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC1)), _mm256_mullo_epi32(vregA1, vregB))); pC1 += 8;
						_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC2), _mm256_sub_epi32(_mm256_loadu_si256(
							reinterpret_cast<__m256i*>(pC2)), _mm256_mullo_epi32(vregA2, vregB))); pC2 += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ -= regA0 * regB;
						*pC1++ -= regA1 * regB;
						*pC2++ -= regA2 * regB;
					}
					break;
				}
				return;
			}

			// if A and B are full matrices
			else
			{
				const int* pB; int* pC;

				const int* pA0; const int* pA1; const int* pA2; const int* pA3;
				const int* pA4; const int* pA5; const int* pA6; const int* pA7;

				int temp[8];

				std::size_t M = Ar - (Ar & 7);
				std::size_t N = Bc - (Bc & 7);
				std::size_t r = (Bc & 7) * sizeof(int);

				std::size_t i, j, k, iOut, jOut, kOut, imax, jmax, kmax;

				__m256i vacc0; __m256i vacc1; __m256i vacc2; __m256i vacc3;
				__m256i vacc4; __m256i vacc5; __m256i vacc6; __m256i vacc7;
				__m256i vregB;

				// major upper part of C
				for (iOut = 0; iOut < Ar; iOut += WELP_MATRIX_AVX_ps_mm_Ti)
				{
					imax = (iOut + WELP_MATRIX_AVX_ps_mm_Ti < Ar) ? iOut + WELP_MATRIX_AVX_ps_mm_Ti : M;
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (i = iOut; i < imax; i += 8)
							{
								for (j = jOut; j < jmax; j += 8)
								{
									pB = (pfB + j) + (Bc * kOut);
									pC = (pfC + j) + (Bc * i);

									pA0 = (pfA + kOut) + (Ac * i); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
									pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac; pA7 = pA0 + 7 * Ac;

									vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
									vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
									vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc)); vacc5 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc));
									vacc6 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 6 * Bc)); vacc7 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 7 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
									for (k = kOut; k < kmax; k++)
									{
										vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
										vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
										vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
										vacc2 = _mm256_sub_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
										vacc3 = _mm256_sub_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
										vacc4 = _mm256_sub_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
										vacc5 = _mm256_sub_epi32(vacc5, _mm256_mullo_epi32(_mm256_set1_epi32(*pA5++), vregB));
										vacc6 = _mm256_sub_epi32(vacc6, _mm256_mullo_epi32(_mm256_set1_epi32(*pA6++), vregB));
										vacc7 = _mm256_sub_epi32(vacc7, _mm256_mullo_epi32(_mm256_set1_epi32(*pA7++), vregB));
									}

									_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc), vacc2); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc), vacc3);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc), vacc4); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc), vacc5);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 6 * Bc), vacc6); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 7 * Bc), vacc7);
								}
								if ((jmax == N) && (r != 0))
								{
									pB = (pfB + N) + (Bc * kOut);
									pC = (pfC + N) + (Bc * i);

									pA0 = (pfA + kOut) + (Ac * i); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
									pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac; pA7 = pA0 + 7 * Ac;

									vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
									vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
									vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc)); vacc5 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc));
									vacc6 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 6 * Bc)); vacc7 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 7 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
									for (k = kOut; k < kmax; k++)
									{
										vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
										vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
										vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
										vacc2 = _mm256_sub_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
										vacc3 = _mm256_sub_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
										vacc4 = _mm256_sub_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
										vacc5 = _mm256_sub_epi32(vacc5, _mm256_mullo_epi32(_mm256_set1_epi32(*pA5++), vregB));
										vacc6 = _mm256_sub_epi32(vacc6, _mm256_mullo_epi32(_mm256_set1_epi32(*pA6++), vregB));
										vacc7 = _mm256_sub_epi32(vacc7, _mm256_mullo_epi32(_mm256_set1_epi32(*pA7++), vregB));
									}

									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc6); std::memcpy(pC + 6 * Bc, static_cast<int*>(temp), r);
									_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc7); std::memcpy(pC + 7 * Bc, static_cast<int*>(temp), r);
								}
							}
						}
					}
				}

				// bottom fringe of C
				switch (Ar & 7)
				{

				case 0:
					break;

				case 1:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M);

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M);

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
							}
						}
					}
					break;

				case 2:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
							}
						}
					}
					break;

				case 3:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_sub_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc), vacc2);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_sub_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<int*>(temp), r);
							}
						}
					}
					break;

				case 4:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_sub_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_sub_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc), vacc2); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc), vacc3);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_sub_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_sub_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<int*>(temp), r);
							}
						}
					}
					break;

				case 5:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
								vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_sub_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_sub_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
									vacc4 = _mm256_sub_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc), vacc2); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc), vacc3);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc), vacc4);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
								vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_sub_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_sub_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
									vacc4 = _mm256_sub_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<int*>(temp), r);
							}
						}
					}
					break;

				case 6:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
								vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc)); vacc5 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_sub_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_sub_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
									vacc4 = _mm256_sub_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
									vacc5 = _mm256_sub_epi32(vacc5, _mm256_mullo_epi32(_mm256_set1_epi32(*pA5++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc), vacc2); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc), vacc3);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc), vacc4); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc), vacc5);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
								vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc)); vacc5 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_sub_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_sub_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
									vacc4 = _mm256_sub_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
									vacc5 = _mm256_sub_epi32(vacc5, _mm256_mullo_epi32(_mm256_set1_epi32(*pA5++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<int*>(temp), r);
							}
						}
					}
					break;

				case 7:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_ps_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_ps_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_ps_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_ps_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_ps_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_ps_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 8)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
								vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc)); vacc5 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc));
								vacc6 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 6 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_sub_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_sub_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
									vacc4 = _mm256_sub_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
									vacc5 = _mm256_sub_epi32(vacc5, _mm256_mullo_epi32(_mm256_set1_epi32(*pA5++), vregB));
									vacc6 = _mm256_sub_epi32(vacc6, _mm256_mullo_epi32(_mm256_set1_epi32(*pA6++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC), vacc0); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + Bc), vacc1);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc), vacc2); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc), vacc3);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc), vacc4); _mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc), vacc5);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(pC + 6 * Bc), vacc6);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac;

								vacc0 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC)); vacc1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + Bc));
								vacc2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 2 * Bc)); vacc3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 3 * Bc));
								vacc4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 4 * Bc)); vacc5 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 5 * Bc));
								vacc6 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pC + 6 * Bc));
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pB)); pB += Bc;
									vacc0 = _mm256_sub_epi32(vacc0, _mm256_mullo_epi32(_mm256_set1_epi32(*pA0++), vregB));
									vacc1 = _mm256_sub_epi32(vacc1, _mm256_mullo_epi32(_mm256_set1_epi32(*pA1++), vregB));
									vacc2 = _mm256_sub_epi32(vacc2, _mm256_mullo_epi32(_mm256_set1_epi32(*pA2++), vregB));
									vacc3 = _mm256_sub_epi32(vacc3, _mm256_mullo_epi32(_mm256_set1_epi32(*pA3++), vregB));
									vacc4 = _mm256_sub_epi32(vacc4, _mm256_mullo_epi32(_mm256_set1_epi32(*pA4++), vregB));
									vacc5 = _mm256_sub_epi32(vacc5, _mm256_mullo_epi32(_mm256_set1_epi32(*pA5++), vregB));
									vacc6 = _mm256_sub_epi32(vacc6, _mm256_mullo_epi32(_mm256_set1_epi32(*pA6++), vregB));
								}

								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc0); std::memcpy(pC, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<int*>(temp), r);
								_mm256_storeu_si256(reinterpret_cast<__m256i*>(temp), vacc6); std::memcpy(pC + 6 * Bc, static_cast<int*>(temp), r);
							}
						}
					}
					break;
				}
			}
		}
	}
#endif // WELP_MATRIX_AVX_EXT


#ifdef WELP_MATRIX_AVX_EXT
	namespace matrix_subroutines
	{
		inline void fill(double* const pfC, const double x, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			double* pC = pfC;

			__m256d vs = _mm256_broadcast_sd(&x);

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				_mm256_storeu_pd(pC, vs);
				pC += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = x;
				break;

			case 2:
				*pC = x;
				*(pC + 1) = x;
				break;

			case 3:
				*pC = x;
				*(pC + 1) = x;
				*(pC + 2) = x;
				break;
			}
		}
		inline double dot(const double* const pfA, const double* const pfB, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			const double* pA = pfA; const double* pB = pfB;

			__m256d v = _mm256_setzero_pd();
			union { __m256d v; double arr[4]; } varr;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				v = _mm256_fmadd_pd(_mm256_loadu_pd(pA), _mm256_loadu_pd(pB), v);
				pA += 4; pB += 4;
			}
			varr.v = _mm256_hadd_pd(v, v);
			double acc = varr.arr[0] + varr.arr[2];

			switch (r)
			{

			case 0:
				break;

			case 1:
				acc += (*pA) * (*pB);
				break;

			case 2:
				acc += (*pA) * (*pB) + (*(pA + 1)) * (*(pB + 1));
				break;

			case 3:
				acc += (*pA) * (*pB) + (*(pA + 1)) * (*(pB + 1))
					+ (*(pA + 2)) * (*(pB + 2));
				break;
			}

			return acc;
		}
		inline double norm2(const double* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			const double* pA = pfA;

			__m256d v = _mm256_setzero_pd();
			union { __m256d v; double arr[4]; } varr;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				v = _mm256_fmadd_pd(_mm256_loadu_pd(pA), _mm256_loadu_pd(pA), v);
				pA += 4;
			}
			varr.v = _mm256_hadd_pd(v, v);
			double acc = varr.arr[0] + varr.arr[2];

			switch (r)
			{

			case 0:
				break;

			case 1:
				acc += (*pA) * (*pA);
				break;

			case 2:
				acc += (*pA) * (*pA) + (*(pA + 1)) * (*(pA + 1));
				break;

			case 3:
				acc += (*pA) * (*pA) + (*(pA + 1)) * (*(pA + 1))
					+ (*(pA + 2)) * (*(pA + 2));
				break;
			}

			return acc;
		}
		inline void _m(double* const pfC, const double* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			double* pC = pfC; const double* pA = pfA;

			__m256d vs = _mm256_setzero_pd();

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				_mm256_storeu_pd(pC, _mm256_sub_pd(vs, _mm256_loadu_pd(pA)));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = -(*pA);
				break;

			case 2:
				*pC = -(*pA);
				*(pC + 1) = -(*(pA + 1));
				break;

			case 3:
				*pC = -(*pA);
				*(pC + 1) = -(*(pA + 1));
				*(pC + 2) = -(*(pA + 2));
				break;
			}
		}
		inline void ps(double* const pfC, const double x, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			double* pC = pfC;

			__m256d vs = _mm256_broadcast_sd(&x);

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				_mm256_storeu_pd(pC, _mm256_add_pd(vs, _mm256_loadu_pd(pC)));
				pC += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC += x;
				break;

			case 2:
				*pC += x;
				*(pC + 1) += x;
				break;

			case 3:
				*pC += x;
				*(pC + 1) += x;
				*(pC + 2) += x;
				break;
			}
		}
		inline void spm(double* const pfC, const double x, const double* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			double* pC = pfC; const double* pA = pfA;

			__m256d vs = _mm256_broadcast_sd(&x);

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				_mm256_storeu_pd(pC, _mm256_add_pd(vs, _mm256_loadu_pd(pA)));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = x + (*pA);
				break;

			case 2:
				*pC += x + (*pA);
				*(pC + 1) = x + (*(pA + 1));
				break;

			case 3:
				*pC = x + (*pA);
				*(pC + 1) = x + (*(pA + 1));
				*(pC + 2) = x + (*(pA + 2));
				break;
			}
		}
		inline void sp_m(double* const pfC, const double x, const double* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			double* pC = pfC; const double* pA = pfA;

			__m256d vs = _mm256_broadcast_sd(&x);

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				_mm256_storeu_pd(pC, _mm256_sub_pd(vs, _mm256_loadu_pd(pA)));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = x - (*pA);
				break;

			case 2:
				*pC += x - (*pA);
				*(pC + 1) = x - (*(pA + 1));
				break;

			case 3:
				*pC = x - (*pA);
				*(pC + 1) = x - (*(pA + 1));
				*(pC + 2) = x - (*(pA + 2));
				break;
			}
		}
		inline void pm(double* const pfC, const double* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			double* pC = pfC; const double* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				_mm256_storeu_pd(pC, _mm256_add_pd(_mm256_loadu_pd(pC),
					_mm256_loadu_pd(pA)));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC += (*pA);
				break;

			case 2:
				*pC += (*pA);
				*(pC + 1) += (*(pA + 1));
				break;

			case 3:
				*pC += (*pA);
				*(pC + 1) += (*(pA + 1));
				*(pC + 2) += (*(pA + 2));
				break;
			}
		}
		inline void p_m(double* const pfC, const double* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			double* pC = pfC; const double* pA = pfA;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				_mm256_storeu_pd(pC, _mm256_sub_pd(_mm256_loadu_pd(pC),
					_mm256_loadu_pd(pA)));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC -= (*pA);
				break;

			case 2:
				*pC -= (*pA);
				*(pC + 1) -= (*(pA + 1));
				break;

			case 3:
				*pC -= (*pA);
				*(pC + 1) -= (*(pA + 1));
				*(pC + 2) -= (*(pA + 2));
				break;
			}
		}
		inline void mpm(double* const pfC, const double* const pfA, const double* const pfB, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			double* pC = pfC; const double* pA = pfA; const double* pB = pfB;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				_mm256_storeu_pd(pC, _mm256_add_pd(_mm256_loadu_pd(pA),
					_mm256_loadu_pd(pB)));
				pC += 4; pA += 4; pB += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = (*pA) + (*pB);
				break;

			case 2:
				*pC = (*pA) + (*pB);
				*(pC + 1) = (*(pA + 1)) + (*(pB + 1));
				break;

			case 3:
				*pC = (*pA) + (*pB);
				*(pC + 1) = (*(pA + 1)) + (*(pB + 1));
				*(pC + 2) = (*(pA + 2)) + (*(pB + 2));
				break;
			}
		}
		inline void mp_m(double* const pfC, const  double* const pfA, const double* const pfB, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			double* pC = pfC; const double* pA = pfA; const double* pB = pfB;

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				_mm256_storeu_pd(pC, _mm256_sub_pd(_mm256_loadu_pd(pA),
					_mm256_loadu_pd(pB)));
				pC += 4; pA += 4; pB += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = (*pA) - (*pB);
				break;

			case 2:
				*pC = (*pA) - (*pB);
				*(pC + 1) = (*(pA + 1)) - (*(pB + 1));
				break;

			case 3:
				*pC = (*pA) - (*pB);
				*(pC + 1) = (*(pA + 1)) - (*(pB + 1));
				*(pC + 2) = (*(pA + 2)) - (*(pB + 2));
				break;
			}
		}
		inline void xs(double* const pfC, const double x, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			double* pC = pfC;

			__m256d vs = _mm256_broadcast_sd(&x);

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				_mm256_storeu_pd(pC, _mm256_mul_pd(vs, _mm256_loadu_pd(pC)));
				pC += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC *= x;
				break;

			case 2:
				*pC *= x;
				*(pC + 1) *= x;
				break;

			case 3:
				*pC *= x;
				*(pC + 1) *= x;
				*(pC + 2) *= x;
				break;
			}
		}
		inline void sxm(double* const pfC, const double x, const double* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			double* pC = pfC; const double* pA = pfA;

			__m256d vs = _mm256_broadcast_sd(&x);

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				_mm256_storeu_pd(pC, _mm256_mul_pd(vs, _mm256_loadu_pd(pA)));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC = x * (*pA);
				break;

			case 2:
				*pC = x * (*pA);
				*(pC + 1) = x * (*(pA + 1));
				break;

			case 3:
				*pC = x * (*pA);
				*(pC + 1) = x * (*(pA + 1));
				*(pC + 2) = x * (*(pA + 2));
				break;
			}
		}
		inline void psxm(double* const pfC, const double x, const double* const pfA, const std::size_t n) noexcept
		{
			std::size_t r = n & 3;
			double* pC = pfC; const double* pA = pfA;

			__m256d vs = _mm256_broadcast_sd(&x);

			for (std::size_t k = n - r; k > 0; k -= 4)
			{
				_mm256_storeu_pd(pC, _mm256_fmadd_pd(vs, _mm256_loadu_pd(pA), _mm256_loadu_pd(pC)));
				pC += 4; pA += 4;
			}

			switch (r)
			{

			case 0:
				break;

			case 1:
				*pC += x * (*pA);
				break;

			case 2:
				*pC += x * (*pA);
				*(pC + 1) += x * (*(pA + 1));
				break;

			case 3:
				*pC += x * (*pA);
				*(pC + 1) += x * (*(pA + 1));
				*(pC + 2) += x * (*(pA + 2));
				break;
			}
		}
		void pmxm(double* const pfC, const double* const pfA, const double* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept
		{
			if (Bc == 1)
			{
				double acc0; double acc1; double acc2; double acc3; double regB;
				const double* pA0 = pfA;
				const double* pA1 = pfA + Ac;
				const double* pA2 = pfA + 2 * Ac;
				const double* pA3 = pfA + 3 * Ac;
				const double* pB; double* pC = pfC;
				std::size_t N = Ar - (Ar & 3);
				std::size_t M = Ac - (Ac & 3);
				std::size_t jump = 3 * Ac;

				std::size_t i, k;

				__m256d vacc0; __m256d vacc1; __m256d vacc2; __m256d vacc3; __m256d vB;
				union { __m256d v; double arr[4]; } varr;

				// major upper part of C
				for (i = N; i > 0; i -= 4)
				{
					acc0 = 0.0; acc1 = 0.0; acc2 = 0.0; acc3 = 0.0;
					vacc0 = _mm256_setzero_pd();
					vacc1 = _mm256_setzero_pd();
					vacc2 = _mm256_setzero_pd();
					vacc3 = _mm256_setzero_pd();
					pB = pfB;

					for (k = M; k > 0; k -= 4)
					{
						vB = _mm256_loadu_pd(pB); pB += 4;
						vacc0 = _mm256_fmadd_pd(_mm256_loadu_pd(pA0), vB, vacc0); pA0 += 4;
						vacc1 = _mm256_fmadd_pd(_mm256_loadu_pd(pA1), vB, vacc1); pA1 += 4;
						vacc2 = _mm256_fmadd_pd(_mm256_loadu_pd(pA2), vB, vacc2); pA2 += 4;
						vacc3 = _mm256_fmadd_pd(_mm256_loadu_pd(pA3), vB, vacc3); pA3 += 4;
					}
					for (k = Ac & 3; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
						acc3 += (*pA3++) * regB;
					}
					varr.v = _mm256_hadd_pd(vacc0, vacc0);
					*pC += acc0 + varr.arr[0] + varr.arr[2];
					varr.v = _mm256_hadd_pd(vacc1, vacc1);
					*(pC + 1) += acc1 + varr.arr[0] + varr.arr[2];
					varr.v = _mm256_hadd_pd(vacc2, vacc2);
					*(pC + 2) += acc2 + varr.arr[0] + varr.arr[2];
					varr.v = _mm256_hadd_pd(vacc3, vacc3);
					*(pC + 3) += acc3 + varr.arr[0] + varr.arr[2];
					pC += 4;

					pA0 += jump;
					pA1 += jump;
					pA2 += jump;
					pA3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					acc0 = 0.0;
					vacc0 = _mm256_setzero_pd();
					pB = pfB;

					for (k = M; k > 0; k -= 4)
					{
						vacc0 = _mm256_fmadd_pd(_mm256_loadu_pd(pA0), _mm256_loadu_pd(pB), vacc0);
						pA0 += 4; pB += 4;
					}
					for (k = Ac & 3; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
					}
					varr.v = _mm256_hadd_pd(vacc0, vacc0);
					*pC += acc0 + varr.arr[0] + varr.arr[2];
					break;

				case 2:
					acc0 = 0.0; acc1 = 0.0;
					vacc0 = _mm256_setzero_pd();
					vacc1 = _mm256_setzero_pd();
					pB = pfB;

					for (k = M; k > 0; k -= 4)
					{
						vB = _mm256_loadu_pd(pB); pB += 4;
						vacc0 = _mm256_fmadd_pd(_mm256_loadu_pd(pA0), vB, vacc0); pA0 += 4;
						vacc1 = _mm256_fmadd_pd(_mm256_loadu_pd(pA1), vB, vacc1); pA1 += 4;
					}
					for (k = Ac & 3; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
					}
					varr.v = _mm256_hadd_pd(vacc0, vacc0);
					*pC += acc0 + varr.arr[0] + varr.arr[2];
					varr.v = _mm256_hadd_pd(vacc1, vacc1);
					*(pC + 1) += acc1 + varr.arr[0] + varr.arr[2];
					break;

				case 3:
					acc0 = 0.0; acc1 = 0.0; acc2 = 0.0;
					vacc0 = _mm256_setzero_pd();
					vacc1 = _mm256_setzero_pd();
					vacc2 = _mm256_setzero_pd();
					pB = pfB;

					for (k = M; k > 0; k -= 4)
					{
						vB = _mm256_loadu_pd(pB); pB += 4;
						vacc0 = _mm256_fmadd_pd(_mm256_loadu_pd(pA0), vB, vacc0); pA0 += 4;
						vacc1 = _mm256_fmadd_pd(_mm256_loadu_pd(pA1), vB, vacc1); pA1 += 4;
						vacc2 = _mm256_fmadd_pd(_mm256_loadu_pd(pA2), vB, vacc2); pA2 += 4;
					}
					for (k = Ac & 3; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
					}
					varr.v = _mm256_hadd_pd(vacc0, vacc0);
					*pC += acc0 + varr.arr[0] + varr.arr[2];
					varr.v = _mm256_hadd_pd(vacc1, vacc1);
					*(pC + 1) += acc1 + varr.arr[0] + varr.arr[2];
					varr.v = _mm256_hadd_pd(vacc2, vacc2);
					*(pC + 2) += acc2 + varr.arr[0] + varr.arr[2];
					break;
				}
				return;
			}

			else if (Ar == 1)
			{
				double regA0; double regA1; double regA2; double regA3;
				const double* pA = pfA;
				const double* pB0 = pfB;
				const double* pB1 = pfB + Bc;
				const double* pB2 = pfB + 2 * Bc;
				const double* pB3 = pfB + 3 * Bc;
				double* pC;

				std::size_t N = Ac - (Ac & 3);
				std::size_t M = Bc - (Bc & 3);
				std::size_t jump = 3 * Bc;

				std::size_t j, k;

				__m256d vregA0; __m256d vregA1; __m256d vregA2; __m256d vregA3; __m256d vregC;

				// major upper part of B
				for (k = N; k > 0; k -= 4)
				{
					pC = pfC;

					vregA0 = _mm256_broadcast_sd(pA);
					vregA1 = _mm256_broadcast_sd(pA + 1);
					vregA2 = _mm256_broadcast_sd(pA + 2);
					vregA3 = _mm256_broadcast_sd(pA + 3);

					for (j = M; j > 0; j -= 4)
					{
						vregC = _mm256_loadu_pd(pC);
						vregC = _mm256_fmadd_pd(vregA0, _mm256_loadu_pd(pB0), vregC); pB0 += 4;
						vregC = _mm256_fmadd_pd(vregA1, _mm256_loadu_pd(pB1), vregC); pB1 += 4;
						vregC = _mm256_fmadd_pd(vregA2, _mm256_loadu_pd(pB2), vregC); pB2 += 4;
						vregC = _mm256_fmadd_pd(vregA3, _mm256_loadu_pd(pB3), vregC); pB3 += 4;
						_mm256_storeu_pd(pC, vregC); pC += 4;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ += (regA0 * (*pB0++) + regA1 * (*pB1++))
							+ (regA2 * (*pB2++) + regA3 * (*pB3++));
					}
					pB0 += jump;
					pB1 += jump;
					pB2 += jump;
					pB3 += jump;
				}

				// bottom fringe of B
				switch (Ac & 3)
				{

				case 0:
					break;

				case 1:
					pC = pfC;
					vregA0 = _mm256_broadcast_sd(pA);

					for (j = M; j > 0; j -= 4)
					{
						_mm256_storeu_pd(pC, _mm256_fmadd_pd(vregA0,
							_mm256_loadu_pd(pB0), _mm256_loadu_pd(pC)));
						pB0 += 4; pC += 4;
					}
					regA0 = *pA;
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ += regA0 * (*pB0++);
					}
					break;

				case 2:
					pC = pfC;
					vregA0 = _mm256_broadcast_sd(pA);
					vregA1 = _mm256_broadcast_sd(pA + 1);

					for (j = M; j > 0; j -= 4)
					{
						vregC = _mm256_loadu_pd(pC);
						vregC = _mm256_fmadd_pd(vregA0, _mm256_loadu_pd(pB0), vregC); pB0 += 4;
						vregC = _mm256_fmadd_pd(vregA1, _mm256_loadu_pd(pB1), vregC); pB1 += 4;
						_mm256_storeu_pd(pC, vregC); pC += 4;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ += regA0 * (*pB0++) + regA1 * (*pB1++);
					}
					break;

				case 3:
					pC = pfC;
					vregA0 = _mm256_broadcast_sd(pA);
					vregA1 = _mm256_broadcast_sd(pA + 1);
					vregA2 = _mm256_broadcast_sd(pA + 2);

					for (j = M; j > 0; j -= 4)
					{
						vregC = _mm256_loadu_pd(pC);
						vregC = _mm256_fmadd_pd(vregA0, _mm256_loadu_pd(pB0), vregC); pB0 += 4;
						vregC = _mm256_fmadd_pd(vregA1, _mm256_loadu_pd(pB1), vregC); pB1 += 4;
						vregC = _mm256_fmadd_pd(vregA2, _mm256_loadu_pd(pB2), vregC); pB2 += 4;
						_mm256_storeu_pd(pC, vregC); pC += 4;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ += (regA0 * (*pB0++) + regA1 * (*pB1++)) + regA2 * (*pB2++);
					}
					break;
				}
				return;
			}

			else if (Ac == 1)
			{
				double regA0; double regA1; double regA2; double regA3; double regB;
				const double* pA = pfA;
				const double* pB;
				double* pC0 = pfC;
				double* pC1 = pfC + Bc;
				double* pC2 = pfC + 2 * Bc;
				double* pC3 = pfC + 3 * Bc;

				std::size_t N = Ar - (Ar & 3);
				std::size_t M = Bc - (Bc & 3);
				std::size_t jump = 3 * Bc;

				std::size_t i, j;

				__m256d vregA0; __m256d vregA1; __m256d vregA2; __m256d vregA3; __m256d vregB;

				// major upper part of B
				for (i = N; i > 0; i -= 4)
				{
					pB = pfB;
					vregA0 = _mm256_broadcast_sd(pA);
					vregA1 = _mm256_broadcast_sd(pA + 1);
					vregA2 = _mm256_broadcast_sd(pA + 2);
					vregA3 = _mm256_broadcast_sd(pA + 3);
					for (j = M; j > 0; j -= 4)
					{
						vregB = _mm256_loadu_pd(pB); pB += 4;
						_mm256_storeu_pd(pC0, _mm256_fmadd_pd(vregA0, vregB, _mm256_loadu_pd(pC0))); pC0 += 4;
						_mm256_storeu_pd(pC1, _mm256_fmadd_pd(vregA1, vregB, _mm256_loadu_pd(pC1))); pC1 += 4;
						_mm256_storeu_pd(pC2, _mm256_fmadd_pd(vregA2, vregB, _mm256_loadu_pd(pC2))); pC2 += 4;
						_mm256_storeu_pd(pC3, _mm256_fmadd_pd(vregA3, vregB, _mm256_loadu_pd(pC3))); pC3 += 4;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ += regA0 * regB;
						*pC1++ += regA1 * regB;
						*pC2++ += regA2 * regB;
						*pC3++ += regA3 * regB;
					}
					pC0 += jump;
					pC1 += jump;
					pC2 += jump;
					pC3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					pB = pfB;
					vregA0 = _mm256_broadcast_sd(pA);
					for (j = M; j > 0; j -= 4)
					{
						_mm256_storeu_pd(pC0, _mm256_fmadd_pd(vregA0, _mm256_loadu_pd(pB), _mm256_loadu_pd(pC0)));
						pB += 4; pC0 += 4;
					}
					regA0 = *pA;
					for (j = Bc - M; j > 0; j--)
					{
						*pC0++ += regA0 * (*pB++);
					}
					break;

				case 2:
					pB = pfB;
					vregA0 = _mm256_broadcast_sd(pA);
					vregA1 = _mm256_broadcast_sd(pA + 1);
					for (j = M; j > 0; j -= 4)
					{
						vregB = _mm256_loadu_pd(pB); pB += 4;
						_mm256_storeu_pd(pC0, _mm256_fmadd_pd(vregA0, vregB, _mm256_loadu_pd(pC0))); pC0 += 4;
						_mm256_storeu_pd(pC1, _mm256_fmadd_pd(vregA1, vregB, _mm256_loadu_pd(pC1))); pC1 += 4;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ += regA0 * regB;
						*pC1++ += regA1 * regB;
					}
					break;

				case 3:
					pB = pfB;
					vregA0 = _mm256_broadcast_sd(pA);
					vregA1 = _mm256_broadcast_sd(pA + 1);
					vregA2 = _mm256_broadcast_sd(pA + 2);
					for (j = M; j > 0; j -= 4)
					{
						vregB = _mm256_loadu_pd(pB); pB += 4;
						_mm256_storeu_pd(pC0, _mm256_fmadd_pd(vregA0, vregB, _mm256_loadu_pd(pC0))); pC0 += 4;
						_mm256_storeu_pd(pC1, _mm256_fmadd_pd(vregA1, vregB, _mm256_loadu_pd(pC1))); pC1 += 4;
						_mm256_storeu_pd(pC2, _mm256_fmadd_pd(vregA2, vregB, _mm256_loadu_pd(pC2))); pC2 += 4;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ += regA0 * regB;
						*pC1++ += regA1 * regB;
						*pC2++ += regA2 * regB;
					}
					break;
				}
				return;
			}

			// if A and B are full matrices
			else
			{
				const double* pB; double* pC;

				const double* pA0; const double* pA1; const double* pA2; const double* pA3;
				const double* pA4; const double* pA5; const double* pA6; const double* pA7;

				double temp[4];

				std::size_t M = Ar - (Ar & 7);
				std::size_t N = Bc - (Bc & 3);
				std::size_t r = (Bc & 3) * sizeof(double);

				std::size_t i, j, k, iOut, jOut, kOut, imax, jmax, kmax;

				__m256d vacc0; __m256d vacc1; __m256d vacc2; __m256d vacc3;
				__m256d vacc4; __m256d vacc5; __m256d vacc6; __m256d vacc7;
				__m256d vregB;

				// major upper part of C
				for (iOut = 0; iOut < Ar; iOut += WELP_MATRIX_AVX_pd_mm_Ti)
				{
					imax = (iOut + WELP_MATRIX_AVX_pd_mm_Ti < Ar) ? iOut + WELP_MATRIX_AVX_pd_mm_Ti : M;
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (i = iOut; i < imax; i += 8)
							{
								for (j = jOut; j < jmax; j += 4)
								{
									pB = (pfB + j) + (Bc * kOut);
									pC = (pfC + j) + (Bc * i);

									pA0 = (pfA + kOut) + (Ac * i); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
									pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac; pA7 = pA0 + 7 * Ac;

									vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
									vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
									vacc4 = _mm256_loadu_pd(pC + 4 * Bc); vacc5 = _mm256_loadu_pd(pC + 5 * Bc);
									vacc6 = _mm256_loadu_pd(pC + 6 * Bc); vacc7 = _mm256_loadu_pd(pC + 7 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
									for (k = kOut; k < kmax; k++)
									{
										vregB = _mm256_loadu_pd(pB); pB += Bc;
										vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
										vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
										vacc2 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
										vacc3 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
										vacc4 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
										vacc5 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA5++), vregB, vacc5);
										vacc6 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA6++), vregB, vacc6);
										vacc7 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA7++), vregB, vacc7);
									}

									_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
									_mm256_storeu_pd(pC + 2 * Bc, vacc2); _mm256_storeu_pd(pC + 3 * Bc, vacc3);
									_mm256_storeu_pd(pC + 4 * Bc, vacc4); _mm256_storeu_pd(pC + 5 * Bc, vacc5);
									_mm256_storeu_pd(pC + 6 * Bc, vacc6); _mm256_storeu_pd(pC + 7 * Bc, vacc7);
								}
								if ((jmax == N) && (r != 0))
								{
									pB = (pfB + N) + (Bc * kOut);
									pC = (pfC + N) + (Bc * i);

									pA0 = (pfA + kOut) + (Ac * i); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
									pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac; pA7 = pA0 + 7 * Ac;

									vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
									vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
									vacc4 = _mm256_loadu_pd(pC + 4 * Bc); vacc5 = _mm256_loadu_pd(pC + 5 * Bc);
									vacc6 = _mm256_loadu_pd(pC + 6 * Bc); vacc7 = _mm256_loadu_pd(pC + 7 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
									for (k = kOut; k < kmax; k++)
									{
										vregB = _mm256_loadu_pd(pB); pB += Bc;
										vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
										vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
										vacc2 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
										vacc3 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
										vacc4 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
										vacc5 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA5++), vregB, vacc5);
										vacc6 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA6++), vregB, vacc6);
										vacc7 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA7++), vregB, vacc7);
									}

									_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc6); std::memcpy(pC + 6 * Bc, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc7); std::memcpy(pC + 7 * Bc, static_cast<double*>(temp), r);
								}
							}
						}
					}
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;

						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M);

								vacc0 = _mm256_loadu_pd(pC);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
								}

								_mm256_storeu_pd(pC, vacc0);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M);

								vacc0 = _mm256_loadu_pd(pC);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
								}

								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
							}
						}
					}
					break;

				case 2:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
								}

								_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
								}

								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
							}
						}
					}
					break;

				case 3:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
								}

								_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
								_mm256_storeu_pd(pC + 2 * Bc, vacc2);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
								}

								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<double*>(temp), r);
							}
						}
					}
					break;

				case 4:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
								}

								_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
								_mm256_storeu_pd(pC + 2 * Bc, vacc2); _mm256_storeu_pd(pC + 3 * Bc, vacc3);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
								}

								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<double*>(temp), r);
							}
						}
					}
					break;

				case 5:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
								vacc4 = _mm256_loadu_pd(pC + 4 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
									vacc4 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
								}

								_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
								_mm256_storeu_pd(pC + 2 * Bc, vacc2); _mm256_storeu_pd(pC + 3 * Bc, vacc3);
								_mm256_storeu_pd(pC + 4 * Bc, vacc4);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
								vacc4 = _mm256_loadu_pd(pC + 4 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
									vacc4 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
								}

								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<double*>(temp), r);
							}
						}
					}
					break;

				case 6:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
								vacc4 = _mm256_loadu_pd(pC + 4 * Bc); vacc5 = _mm256_loadu_pd(pC + 5 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
									vacc4 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
									vacc5 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA5++), vregB, vacc5);
								}

								_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
								_mm256_storeu_pd(pC + 2 * Bc, vacc2); _mm256_storeu_pd(pC + 3 * Bc, vacc3);
								_mm256_storeu_pd(pC + 4 * Bc, vacc4); _mm256_storeu_pd(pC + 5 * Bc, vacc5);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
								vacc4 = _mm256_loadu_pd(pC + 4 * Bc); vacc5 = _mm256_loadu_pd(pC + 5 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
									vacc4 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
									vacc5 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA5++), vregB, vacc5);
								}

								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<double*>(temp), r);
							}
						}
					}
					break;

				case 7:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
								vacc4 = _mm256_loadu_pd(pC + 4 * Bc); vacc5 = _mm256_loadu_pd(pC + 5 * Bc);
								vacc6 = _mm256_loadu_pd(pC + 6 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
									vacc4 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
									vacc5 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA5++), vregB, vacc5);
									vacc6 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA6++), vregB, vacc6);
								}

								_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
								_mm256_storeu_pd(pC + 2 * Bc, vacc2); _mm256_storeu_pd(pC + 3 * Bc, vacc3);
								_mm256_storeu_pd(pC + 4 * Bc, vacc4); _mm256_storeu_pd(pC + 5 * Bc, vacc5);
								_mm256_storeu_pd(pC + 6 * Bc, vacc6);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
								vacc4 = _mm256_loadu_pd(pC + 4 * Bc); vacc5 = _mm256_loadu_pd(pC + 5 * Bc);
								vacc6 = _mm256_loadu_pd(pC + 6 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
									vacc4 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
									vacc5 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA5++), vregB, vacc5);
									vacc6 = _mm256_fmadd_pd(_mm256_broadcast_sd(pA6++), vregB, vacc6);
								}

								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc6); std::memcpy(pC + 6 * Bc, static_cast<double*>(temp), r);
							}
						}
					}
					break;
				}
			}
		}
		void p_mxm(double* const pfC, const double* const pfA, const  double* const pfB, const std::size_t Ar, const std::size_t Bc, const std::size_t Ac) noexcept
		{
			if (Bc == 1)
			{
				double acc0; double acc1; double acc2; double acc3; double regB;
				const double* pA0 = pfA;
				const double* pA1 = pfA + Ac;
				const double* pA2 = pfA + 2 * Ac;
				const double* pA3 = pfA + 3 * Ac;
				const double* pB; double* pC = pfC;
				std::size_t N = Ar - (Ar & 3);
				std::size_t M = Ac - (Ac & 3);
				std::size_t jump = 3 * Ac;

				std::size_t i, k;

				__m256d vacc0; __m256d vacc1; __m256d vacc2; __m256d vacc3; __m256d vB;
				union { __m256d v; double arr[4]; } varr;

				// major upper part of C
				for (i = 0; i < N; i += 4)
				{
					acc0 = 0.0; acc1 = 0.0; acc2 = 0.0; acc3 = 0.0;
					vacc0 = _mm256_setzero_pd();
					vacc1 = _mm256_setzero_pd();
					vacc2 = _mm256_setzero_pd();
					vacc3 = _mm256_setzero_pd();
					pB = pfB;

					for (k = M; k > 0; k -= 4)
					{
						vB = _mm256_loadu_pd(pB); pB += 4;
						vacc0 = _mm256_fmadd_pd(_mm256_loadu_pd(pA0), vB, vacc0); pA0 += 4;
						vacc1 = _mm256_fmadd_pd(_mm256_loadu_pd(pA1), vB, vacc1); pA1 += 4;
						vacc2 = _mm256_fmadd_pd(_mm256_loadu_pd(pA2), vB, vacc2); pA2 += 4;
						vacc3 = _mm256_fmadd_pd(_mm256_loadu_pd(pA3), vB, vacc3); pA3 += 4;
					}
					for (k = Ac & 3; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
						acc3 += (*pA3++) * regB;
					}
					varr.v = _mm256_hadd_pd(vacc0, vacc0);
					*pC -= acc0 + varr.arr[0] + varr.arr[2];
					varr.v = _mm256_hadd_pd(vacc1, vacc1);
					*(pC + 1) -= acc1 + varr.arr[0] + varr.arr[2];
					varr.v = _mm256_hadd_pd(vacc2, vacc2);
					*(pC + 2) -= acc2 + varr.arr[0] + varr.arr[2];
					varr.v = _mm256_hadd_pd(vacc3, vacc3);
					*(pC + 3) -= acc3 + varr.arr[0] + varr.arr[2];
					pC += 4;

					pA0 += jump;
					pA1 += jump;
					pA2 += jump;
					pA3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					acc0 = 0.0;
					vacc0 = _mm256_setzero_pd();
					pA0 = pfA + (Ac * N);
					pB = pfB;

					for (k = M; k > 0; k -= 4)
					{
						vacc0 = _mm256_fmadd_pd(_mm256_loadu_pd(pA0), _mm256_loadu_pd(pB), vacc0);
						pA0 += 4; pB += 4;
					}
					for (k = Ac & 3; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
					}
					varr.v = _mm256_hadd_pd(vacc0, vacc0);
					*pC -= acc0 + varr.arr[0] + varr.arr[2];
					break;

				case 2:
					acc0 = 0.0; acc1 = 0.0;
					vacc0 = _mm256_setzero_pd();
					vacc1 = _mm256_setzero_pd();
					pA0 = pfA + (Ac * i);
					pA1 = pA0 + Ac;
					pB = pfB;

					for (k = M; k > 0; k -= 4)
					{
						vB = _mm256_loadu_pd(pB); pB += 4;
						vacc0 = _mm256_fmadd_pd(_mm256_loadu_pd(pA0), vB, vacc0); pA0 += 4;
						vacc1 = _mm256_fmadd_pd(_mm256_loadu_pd(pA1), vB, vacc1); pA1 += 4;
					}
					for (k = Ac & 3; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
					}
					varr.v = _mm256_hadd_pd(vacc0, vacc0);
					*pC -= acc0 + varr.arr[0] + varr.arr[2];
					varr.v = _mm256_hadd_pd(vacc1, vacc1);
					*(pC + 1) -= acc1 + varr.arr[0] + varr.arr[2];
					break;

				case 3:
					acc0 = 0.0; acc1 = 0.0; acc2 = 0.0;
					vacc0 = _mm256_setzero_pd();
					vacc1 = _mm256_setzero_pd();
					vacc2 = _mm256_setzero_pd();
					pA0 = pfA + (Ac * i);
					pA1 = pA0 + Ac;
					pA2 = pA0 + 2 * Ac;
					pB = pfB;

					for (k = M; k > 0; k -= 4)
					{
						vB = _mm256_loadu_pd(pB); pB += 4;
						vacc0 = _mm256_fmadd_pd(_mm256_loadu_pd(pA0), vB, vacc0); pA0 += 4;
						vacc1 = _mm256_fmadd_pd(_mm256_loadu_pd(pA1), vB, vacc1); pA1 += 4;
						vacc2 = _mm256_fmadd_pd(_mm256_loadu_pd(pA2), vB, vacc2); pA2 += 4;
					}
					for (k = Ac & 3; k > 0; k--)
					{
						regB = *pB++;
						acc0 += (*pA0++) * regB;
						acc1 += (*pA1++) * regB;
						acc2 += (*pA2++) * regB;
					}
					varr.v = _mm256_hadd_pd(vacc0, vacc0);
					*pC -= acc0 + varr.arr[0] + varr.arr[2];
					varr.v = _mm256_hadd_pd(vacc1, vacc1);
					*(pC + 1) -= acc1 + varr.arr[0] + varr.arr[2];
					varr.v = _mm256_hadd_pd(vacc2, vacc2);
					*(pC + 2) -= acc2 + varr.arr[0] + varr.arr[2];
					break;
				}
				return;
			}

			else if (Ar == 1)
			{
				double regA0; double regA1; double regA2; double regA3;
				const double* pA = pfA;
				const double* pB0 = pfB;
				const double* pB1 = pfB + Bc;
				const double* pB2 = pfB + 2 * Bc;
				const double* pB3 = pfB + 3 * Bc;
				double* pC;

				std::size_t N = Ac - (Ac & 3);
				std::size_t M = Bc - (Bc & 3);
				std::size_t jump = 3 * Bc;

				std::size_t j, k;

				__m256d vregA0; __m256d vregA1; __m256d vregA2; __m256d vregA3; __m256d vregC;

				// major upper part of B
				for (k = 0; k < N; k += 4)
				{
					pC = pfC;

					vregA0 = _mm256_broadcast_sd(pA);
					vregA1 = _mm256_broadcast_sd(pA + 1);
					vregA2 = _mm256_broadcast_sd(pA + 2);
					vregA3 = _mm256_broadcast_sd(pA + 3);

					for (j = M; j > 0; j -= 4)
					{
						vregC = _mm256_loadu_pd(pC);
						vregC = _mm256_fnmadd_pd(vregA0, _mm256_loadu_pd(pB0), vregC); pB0 += 4;
						vregC = _mm256_fnmadd_pd(vregA1, _mm256_loadu_pd(pB1), vregC); pB1 += 4;
						vregC = _mm256_fnmadd_pd(vregA2, _mm256_loadu_pd(pB2), vregC); pB2 += 4;
						vregC = _mm256_fnmadd_pd(vregA3, _mm256_loadu_pd(pB3), vregC); pB3 += 4;
						_mm256_storeu_pd(pC, vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ -= (regA0 * (*pB0++) + regA1 * (*pB1++))
							+ (regA2 * (*pB2++) + regA3 * (*pB3++));
					}
					pB0 += jump;
					pB1 += jump;
					pB2 += jump;
					pB3 += jump;
				}

				// bottom fringe of B
				switch (Ac & 3)
				{

				case 0:
					break;

				case 1:
					pC = pfC;
					vregA0 = _mm256_broadcast_sd(pA);

					for (j = M; j > 0; j -= 4)
					{
						_mm256_storeu_pd(pC, _mm256_fnmadd_pd(vregA0,
							_mm256_loadu_pd(pB0), _mm256_loadu_pd(pC)));
						pB0 += 4; pC += 4;
					}
					regA0 = *pA;
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ -= regA0 * (*pB0++);
					}
					break;

				case 2:
					pC = pfC;
					vregA0 = _mm256_broadcast_sd(pA);
					vregA1 = _mm256_broadcast_sd(pA + 1);

					for (j = M; j > 0; j -= 4)
					{
						vregC = _mm256_loadu_pd(pC);
						vregC = _mm256_fnmadd_pd(vregA0, _mm256_loadu_pd(pB0), vregC); pB0 += 4;
						vregC = _mm256_fnmadd_pd(vregA1, _mm256_loadu_pd(pB1), vregC); pB1 += 4;
						_mm256_storeu_pd(pC, vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ -= regA0 * (*pB0++) + regA1 * (*pB1++);
					}
					break;

				case 3:
					pC = pfC;
					vregA0 = _mm256_broadcast_sd(pA);
					vregA1 = _mm256_broadcast_sd(pA + 1);
					vregA2 = _mm256_broadcast_sd(pA + 2);

					for (j = M; j > 0; j -= 4)
					{
						vregC = _mm256_loadu_pd(pC);
						vregC = _mm256_fnmadd_pd(vregA0, _mm256_loadu_pd(pB0), vregC); pB0 += 4;
						vregC = _mm256_fnmadd_pd(vregA1, _mm256_loadu_pd(pB1), vregC); pB1 += 4;
						vregC = _mm256_fnmadd_pd(vregA2, _mm256_loadu_pd(pB2), vregC); pB2 += 4;
						_mm256_storeu_pd(pC, vregC); pC += 8;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					for (j = Bc - M; j > 0; j--)
					{
						*pC++ -= (regA0 * (*pB0++) + regA1 * (*pB1++)) + regA2 * (*pB2++);
					}
					break;
				}
				return;
			}

			else if (Ac == 1)
			{
				double regA0; double regA1; double regA2; double regA3; double regB;
				const double* pA = pfA;
				const double* pB;
				double* pC0 = pfC;
				double* pC1 = pfC + Bc;
				double* pC2 = pfC + 2 * Bc;
				double* pC3 = pfC + 3 * Bc;

				std::size_t N = Ar - (Ar & 3);
				std::size_t M = Bc - (Bc & 3);
				std::size_t jump = 3 * Bc;

				std::size_t i, j;

				__m256d vregA0; __m256d vregA1; __m256d vregA2; __m256d vregA3; __m256d vregB;

				// major upper part of B
				for (i = N; i > 0; i -= 4)
				{
					pB = pfB;
					vregA0 = _mm256_broadcast_sd(pA);
					vregA1 = _mm256_broadcast_sd(pA + 1);
					vregA2 = _mm256_broadcast_sd(pA + 2);
					vregA3 = _mm256_broadcast_sd(pA + 3);
					for (j = M; j > 0; j -= 4)
					{
						vregB = _mm256_loadu_pd(pB); pB += 4;
						_mm256_storeu_pd(pC0, _mm256_fnmadd_pd(vregA0, vregB, _mm256_loadu_pd(pC0))); pC0 += 4;
						_mm256_storeu_pd(pC1, _mm256_fnmadd_pd(vregA1, vregB, _mm256_loadu_pd(pC1))); pC1 += 4;
						_mm256_storeu_pd(pC2, _mm256_fnmadd_pd(vregA2, vregB, _mm256_loadu_pd(pC2))); pC2 += 4;
						_mm256_storeu_pd(pC3, _mm256_fnmadd_pd(vregA3, vregB, _mm256_loadu_pd(pC3))); pC3 += 4;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					regA3 = *(pA + 3);
					pA += 4;
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ -= regA0 * regB;
						*pC1++ -= regA1 * regB;
						*pC2++ -= regA2 * regB;
						*pC3++ -= regA3 * regB;
					}
					pC0 += jump;
					pC1 += jump;
					pC2 += jump;
					pC3 += jump;
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					pB = pfB;
					vregA0 = _mm256_broadcast_sd(pA);
					for (j = M; j > 0; j -= 4)
					{
						_mm256_storeu_pd(pC0, _mm256_fnmadd_pd(vregA0, _mm256_loadu_pd(pB), _mm256_loadu_pd(pC0)));
						pB += 4; pC0 += 4;
					}
					regA0 = *pA;
					for (j = Bc - M; j > 0; j--)
					{
						*pC0++ -= regA0 * (*pB++);
					}
					break;

				case 2:
					pB = pfB;
					vregA0 = _mm256_broadcast_sd(pA);
					vregA1 = _mm256_broadcast_sd(pA + 1);
					for (j = M; j > 0; j -= 4)
					{
						vregB = _mm256_loadu_pd(pB); pB += 4;
						_mm256_storeu_pd(pC0, _mm256_fnmadd_pd(vregA0, vregB, _mm256_loadu_pd(pC0))); pC0 += 4;
						_mm256_storeu_pd(pC1, _mm256_fnmadd_pd(vregA1, vregB, _mm256_loadu_pd(pC1))); pC1 += 4;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ -= regA0 * regB;
						*pC1++ -= regA1 * regB;
					}
					break;

				case 3:
					pB = pfB;
					vregA0 = _mm256_broadcast_sd(pA);
					vregA1 = _mm256_broadcast_sd(pA + 1);
					vregA2 = _mm256_broadcast_sd(pA + 2);
					for (j = M; j > 0; j -= 4)
					{
						vregB = _mm256_loadu_pd(pB); pB += 4;
						_mm256_storeu_pd(pC0, _mm256_fnmadd_pd(vregA0, vregB, _mm256_loadu_pd(pC0))); pC0 += 4;
						_mm256_storeu_pd(pC1, _mm256_fnmadd_pd(vregA1, vregB, _mm256_loadu_pd(pC1))); pC1 += 4;
						_mm256_storeu_pd(pC2, _mm256_fnmadd_pd(vregA2, vregB, _mm256_loadu_pd(pC2))); pC2 += 4;
					}
					regA0 = *pA;
					regA1 = *(pA + 1);
					regA2 = *(pA + 2);
					for (j = Bc - M; j > 0; j--)
					{
						regB = *pB++;
						*pC0++ -= regA0 * regB;
						*pC1++ -= regA1 * regB;
						*pC2++ -= regA2 * regB;
					}
					break;
				}
				return;
			}

			// if A and B are full matrices
			else
			{
				const double* pB; double* pC;

				const double* pA0; const double* pA1; const double* pA2; const double* pA3;
				const double* pA4; const double* pA5; const double* pA6; const double* pA7;

				double temp[4];

				std::size_t M = Ar - (Ar & 7);
				std::size_t N = Bc - (Bc & 3);
				std::size_t r = (Bc & 3) * sizeof(double);

				std::size_t i, j, k, iOut, jOut, kOut, imax, jmax, kmax;

				__m256d vacc0; __m256d vacc1; __m256d vacc2; __m256d vacc3;
				__m256d vacc4; __m256d vacc5; __m256d vacc6; __m256d vacc7;
				__m256d vregB;

				// major upper part of C
				for (iOut = 0; iOut < Ar; iOut += WELP_MATRIX_AVX_pd_mm_Ti)
				{
					imax = (iOut + WELP_MATRIX_AVX_pd_mm_Ti < Ar) ? iOut + WELP_MATRIX_AVX_pd_mm_Ti : M;
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (i = iOut; i < imax; i += 8)
							{
								for (j = jOut; j < jmax; j += 4)
								{
									pB = (pfB + j) + (Bc * kOut);
									pC = (pfC + j) + (Bc * i);

									pA0 = (pfA + kOut) + (Ac * i); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
									pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac; pA7 = pA0 + 7 * Ac;

									vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
									vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
									vacc4 = _mm256_loadu_pd(pC + 4 * Bc); vacc5 = _mm256_loadu_pd(pC + 5 * Bc);
									vacc6 = _mm256_loadu_pd(pC + 6 * Bc); vacc7 = _mm256_loadu_pd(pC + 7 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
									for (k = kOut; k < kmax; k++)
									{
										vregB = _mm256_loadu_pd(pB); pB += Bc;
										vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
										vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
										vacc2 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
										vacc3 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
										vacc4 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
										vacc5 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA5++), vregB, vacc5);
										vacc6 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA6++), vregB, vacc6);
										vacc7 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA7++), vregB, vacc7);
									}
									_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
									_mm256_storeu_pd(pC + 2 * Bc, vacc2); _mm256_storeu_pd(pC + 3 * Bc, vacc3);
									_mm256_storeu_pd(pC + 4 * Bc, vacc4); _mm256_storeu_pd(pC + 5 * Bc, vacc5);
									_mm256_storeu_pd(pC + 6 * Bc, vacc6); _mm256_storeu_pd(pC + 7 * Bc, vacc7);
								}
								if ((jmax == N) && (r != 0))
								{
									pB = (pfB + N) + (Bc * kOut);
									pC = (pfC + N) + (Bc * i);

									pA0 = (pfA + kOut) + (Ac * i); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
									pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac; pA7 = pA0 + 7 * Ac;

									vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
									vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
									vacc4 = _mm256_loadu_pd(pC + 4 * Bc); vacc5 = _mm256_loadu_pd(pC + 5 * Bc);
									vacc6 = _mm256_loadu_pd(pC + 6 * Bc); vacc7 = _mm256_loadu_pd(pC + 7 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
									for (k = kOut; k < kmax; k++)
									{
										vregB = _mm256_loadu_pd(pB); pB += Bc;
										vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
										vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
										vacc2 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
										vacc3 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
										vacc4 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
										vacc5 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA5++), vregB, vacc5);
										vacc6 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA6++), vregB, vacc6);
										vacc7 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA7++), vregB, vacc7);
									}
									_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc6); std::memcpy(pC + 6 * Bc, static_cast<double*>(temp), r);
									_mm256_storeu_pd(static_cast<double*>(temp), vacc7); std::memcpy(pC + 7 * Bc, static_cast<double*>(temp), r);
								}
							}
						}
					}
				}

				// bottom fringe of C
				switch (Ar & 3)
				{

				case 0:
					break;

				case 1:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;

						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M);

								vacc0 = _mm256_loadu_pd(pC);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
								}
								_mm256_storeu_pd(pC, vacc0);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M);

								vacc0 = _mm256_loadu_pd(pC);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
								}
								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
							}
						}
					}
					break;

				case 2:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
								}
								_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
								}
								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
							}
						}
					}
					break;

				case 3:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
								}
								_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
								_mm256_storeu_pd(pC + 2 * Bc, vacc2);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
								}
								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<double*>(temp), r);
							}
						}
					}
					break;

				case 4:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
								}
								_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
								_mm256_storeu_pd(pC + 2 * Bc, vacc2); _mm256_storeu_pd(pC + 3 * Bc, vacc3);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
								}
								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<double*>(temp), r);
							}
						}
					}
					break;

				case 5:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
								vacc4 = _mm256_loadu_pd(pC + 4 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
									vacc4 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
								}
								_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
								_mm256_storeu_pd(pC + 2 * Bc, vacc2); _mm256_storeu_pd(pC + 3 * Bc, vacc3);
								_mm256_storeu_pd(pC + 4 * Bc, vacc4);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
								vacc4 = _mm256_loadu_pd(pC + 4 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
									vacc4 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
								}
								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<double*>(temp), r);
							}
						}
					}
					break;

				case 6:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
								vacc4 = _mm256_loadu_pd(pC + 4 * Bc); vacc5 = _mm256_loadu_pd(pC + 5 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
									vacc4 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
									vacc5 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA5++), vregB, vacc5);
								}
								_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
								_mm256_storeu_pd(pC + 2 * Bc, vacc2); _mm256_storeu_pd(pC + 3 * Bc, vacc3);
								_mm256_storeu_pd(pC + 4 * Bc, vacc4); _mm256_storeu_pd(pC + 5 * Bc, vacc5);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
								vacc4 = _mm256_loadu_pd(pC + 4 * Bc); vacc5 = _mm256_loadu_pd(pC + 5 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
									vacc4 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
									vacc5 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA5++), vregB, vacc5);
								}
								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<double*>(temp), r);
							}
						}
					}
					break;

				case 7:
					for (jOut = 0; jOut < N || jOut == 0; jOut += WELP_MATRIX_AVX_pd_mm_Tj)
					{
						jmax = (jOut + WELP_MATRIX_AVX_pd_mm_Tj < Bc) ? jOut + WELP_MATRIX_AVX_pd_mm_Tj : N;
						for (kOut = 0; kOut < Ac; kOut += WELP_MATRIX_AVX_pd_mm_Tk)
						{
							kmax = (kOut + WELP_MATRIX_AVX_pd_mm_Tk < Ac) ? kOut + WELP_MATRIX_AVX_pd_mm_Tk : Ac;
							for (j = jOut; j < jmax; j += 4)
							{
								pB = (pfB + j) + (Bc * kOut);
								pC = (pfC + j) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
								vacc4 = _mm256_loadu_pd(pC + 4 * Bc); vacc5 = _mm256_loadu_pd(pC + 5 * Bc);
								vacc6 = _mm256_loadu_pd(pC + 6 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
									vacc4 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
									vacc5 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA5++), vregB, vacc5);
									vacc6 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA6++), vregB, vacc6);
								}
								_mm256_storeu_pd(pC, vacc0); _mm256_storeu_pd(pC + Bc, vacc1);
								_mm256_storeu_pd(pC + 2 * Bc, vacc2); _mm256_storeu_pd(pC + 3 * Bc, vacc3);
								_mm256_storeu_pd(pC + 4 * Bc, vacc4); _mm256_storeu_pd(pC + 5 * Bc, vacc5);
								_mm256_storeu_pd(pC + 6 * Bc, vacc6);
							}
							if ((jmax == N) && (r != 0))
							{
								pB = (pfB + N) + (Bc * kOut);
								pC = (pfC + N) + (Bc * M);

								pA0 = (pfA + kOut) + (Ac * M); pA1 = pA0 + Ac; pA2 = pA0 + 2 * Ac; pA3 = pA0 + 3 * Ac;
								pA4 = pA0 + 4 * Ac; pA5 = pA0 + 5 * Ac; pA6 = pA0 + 6 * Ac;

								vacc0 = _mm256_loadu_pd(pC); vacc1 = _mm256_loadu_pd(pC + Bc);
								vacc2 = _mm256_loadu_pd(pC + 2 * Bc); vacc3 = _mm256_loadu_pd(pC + 3 * Bc);
								vacc4 = _mm256_loadu_pd(pC + 4 * Bc); vacc5 = _mm256_loadu_pd(pC + 5 * Bc);
								vacc6 = _mm256_loadu_pd(pC + 6 * Bc);
#ifdef __clang__
#pragma unroll 8
#endif // __clang__
#if defined __GNUC__ && !defined __clang__
#pragma GCC unroll 8
#endif // defined __GNUC__ && !defined __clang__
								for (k = kOut; k < kmax; k++)
								{
									vregB = _mm256_loadu_pd(pB); pB += Bc;
									vacc0 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA0++), vregB, vacc0);
									vacc1 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA1++), vregB, vacc1);
									vacc2 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA2++), vregB, vacc2);
									vacc3 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA3++), vregB, vacc3);
									vacc4 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA4++), vregB, vacc4);
									vacc5 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA5++), vregB, vacc5);
									vacc6 = _mm256_fnmadd_pd(_mm256_broadcast_sd(pA6++), vregB, vacc6);
								}
								_mm256_storeu_pd(static_cast<double*>(temp), vacc0); std::memcpy(pC, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc1); std::memcpy(pC + Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc2); std::memcpy(pC + 2 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc3); std::memcpy(pC + 3 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc4); std::memcpy(pC + 4 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc5); std::memcpy(pC + 5 * Bc, static_cast<double*>(temp), r);
								_mm256_storeu_pd(static_cast<double*>(temp), vacc6); std::memcpy(pC + 6 * Bc, static_cast<double*>(temp), r);
							}
						}
					}
					break;
				}
			}
		}
		void elim_gauss(double* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t slice) noexcept
		{
			double* p; double* q0; double* q1; double* q2; double* q3;
			double reg0; double reg1; double reg2; double reg3; double reg;
			double maxelem; double temp; bool pivot;
			std::size_t iOut = Ar - (Ar % slice);
			if (iOut != Ar) { iOut += slice; }
			std::size_t ur; std::size_t M; std::size_t jump;
			std::size_t i, ii, j, jj;

			__m256d vreg0; __m256d vreg1; __m256d vreg2; __m256d vreg3; __m256d vreg;

			while (iOut > 0)
			{
				iOut -= slice; j = 0;
				for (i = iOut; i < Ar - 1; i++)
				{
					if (j < Ac)
					{
						ur = (slice < Ar - 1 - i) ? slice : Ar - 1 - i;

						p = (pfA + j) + (Ac * i);
						q0 = p + Ac;
						q1 = p + Ac;
						maxelem = std::abs(*p);
						pivot = false;
						for (jj = 0; jj < ur; jj++)
						{
							temp = std::abs(*q1);
							if (temp > maxelem)
							{
								pivot = true;
								maxelem = temp;
								q0 = q1;
							}
							q1 += Ac;
						}

						M = Ac - j;

						if (pivot)
						{
							for (jj = M - (M & 3); jj > 0; jj -= 4)
							{
								vreg = _mm256_loadu_pd(p);
								_mm256_storeu_pd(p, _mm256_loadu_pd(q0));
								_mm256_storeu_pd(q0, vreg);
								p += 4; q0 += 4;
							}
							for (jj = M & 3; jj > 0; jj--)
							{
								temp = *p;
								*p = *q0;
								*q0 = temp;
								p++; q0++;
							}
						}

						p = (pfA + j) + (Ac * i);

						if (*p != 0.0)
						{
							temp = 1.0 / *p;
							q0 = p + Ac;
							q1 = p + 2 * Ac;
							q2 = p + 3 * Ac;
							q3 = p + 4 * Ac;
							jump = j + 3 * Ac;
							for (ii = ur - (ur & 3); ii > 0; ii -= 4)
							{
								p = (pfA + j) + (Ac * i);
								reg0 = temp * (*q0);
								reg1 = temp * (*q1);
								reg2 = temp * (*q2);
								reg3 = temp * (*q3);
								vreg0 = _mm256_broadcast_sd(&reg0);
								vreg1 = _mm256_broadcast_sd(&reg1);
								vreg2 = _mm256_broadcast_sd(&reg2);
								vreg3 = _mm256_broadcast_sd(&reg3);
								for (jj = M - (M & 3); jj > 0; jj -= 4)
								{
									vreg = _mm256_loadu_pd(p); p += 4;
									_mm256_storeu_pd(q0, _mm256_fnmadd_pd(vreg0, vreg, _mm256_loadu_pd(q0))); q0 += 4;
									_mm256_storeu_pd(q1, _mm256_fnmadd_pd(vreg1, vreg, _mm256_loadu_pd(q1))); q1 += 4;
									_mm256_storeu_pd(q2, _mm256_fnmadd_pd(vreg2, vreg, _mm256_loadu_pd(q2))); q2 += 4;
									_mm256_storeu_pd(q3, _mm256_fnmadd_pd(vreg3, vreg, _mm256_loadu_pd(q3))); q3 += 4;
								}
								for (jj = M & 3; jj > 0; jj--)
								{
									reg = *p++;
									*q0++ -= reg0 * reg;
									*q1++ -= reg1 * reg;
									*q2++ -= reg2 * reg;
									*q3++ -= reg3 * reg;
								}
								q0 += jump;
								q1 += jump;
								q2 += jump;
								q3 += jump;
							}

							switch (ur & 3)
							{

							case 0:
								break;

							case 1:
								p = (pfA + j) + (Ac * i);
								reg0 = temp * (*q0);
								vreg0 = _mm256_broadcast_sd(&reg0);
								for (jj = M - (M & 3); jj > 0; jj -= 4)
								{
									_mm256_storeu_pd(q0, _mm256_fnmadd_pd(vreg0,
										_mm256_loadu_pd(p), _mm256_loadu_pd(q0)));
									p += 4; q0 += 4;
								}
								for (jj = M & 3; jj > 0; jj--)
								{
									*q0++ -= reg0 * (*p++);
								}
								break;

							case 2:
								p = (pfA + j) + (Ac * i);
								reg0 = temp * (*q0);
								reg1 = temp * (*q1);
								vreg0 = _mm256_broadcast_sd(&reg0);
								vreg1 = _mm256_broadcast_sd(&reg1);
								for (jj = M - (M & 3); jj > 0; jj -= 4)
								{
									vreg = _mm256_loadu_pd(p); p += 4;
									_mm256_storeu_pd(q0, _mm256_fnmadd_pd(vreg0, vreg, _mm256_loadu_pd(q0))); q0 += 4;
									_mm256_storeu_pd(q1, _mm256_fnmadd_pd(vreg1, vreg, _mm256_loadu_pd(q1))); q1 += 4;
								}
								for (jj = M & 3; jj > 0; jj--)
								{
									reg = *p++;
									*q0++ -= reg0 * reg;
									*q1++ -= reg1 * reg;
								}
								break;

							case 3:
								p = (pfA + j) + (Ac * i);
								reg0 = temp * (*q0);
								reg1 = temp * (*q1);
								reg2 = temp * (*q2);
								vreg0 = _mm256_broadcast_sd(&reg0);
								vreg1 = _mm256_broadcast_sd(&reg1);
								vreg2 = _mm256_broadcast_sd(&reg2);
								for (jj = M - (M & 3); jj > 0; jj -= 4)
								{
									vreg = _mm256_loadu_pd(p); p += 4;
									_mm256_storeu_pd(q0, _mm256_fnmadd_pd(vreg0, vreg, _mm256_loadu_pd(q0))); q0 += 4;
									_mm256_storeu_pd(q1, _mm256_fnmadd_pd(vreg1, vreg, _mm256_loadu_pd(q1))); q1 += 4;
									_mm256_storeu_pd(q2, _mm256_fnmadd_pd(vreg2, vreg, _mm256_loadu_pd(q2))); q2 += 4;
								}
								for (jj = M & 3; jj > 0; jj--)
								{
									reg = *p++;
									*q0++ -= reg0 * reg;
									*q1++ -= reg1 * reg;
									*q2++ -= reg2 * reg;
								}
								break;
							}
						}
						j++;
					}
					else { break; }
				}
			}

			std::size_t ndiag = (Ar < Ac) ? Ar : Ac;
			p = pfA + Ac;
			for (i = 1; i < ndiag; i++)
			{
				std::memset(p, 0, i * sizeof(double));
				p += Ac;
			}
			if (Ar > Ac)
			{
				std::memset(p, 0, Ac * (Ar - Ac) * sizeof(double));
			}
		}
		void elim_householder(double* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t Nc,
			double* const pfu, double* const pfv, const std::size_t slice) noexcept
		{
			double reg0; double reg1; double reg2; double reg3; double duu; double regv;
			double* pA0; double* pA1; double* pA2; double* pA3; double* pu; double* pv;
			std::size_t iOut = Ar - (Ar % slice);
			if (iOut != Ar) { iOut += slice; }
			std::size_t M; std::size_t jump;
			std::size_t i, j, ii, jj, ur;

			__m256d vreg; __m256d vreg0; __m256d vreg1; __m256d vreg2; __m256d vreg3;
			union { __m256d v; double arr[4]; } varr;

			while (iOut > 0)
			{
				iOut -= slice; j = 0;
				for (i = iOut; i < Ar - 1; i++)
				{
					if (j < Nc)
					{
						ur = (slice + 1 < Ar - i) ? slice + 1 : Ar - i;

						pA0 = (pfA + j) + (Ac * i);
						pu = pfu;
						for (ii = ur; ii > 0; ii--)
						{
							*pu++ = *pA0; pA0 += Ac;
						}

						duu = 0.0;
						pu = pfu;
						vreg0 = _mm256_setzero_pd();
						for (ii = ur - (ur & 3); ii > 0; ii -= 4)
						{
							vreg0 = _mm256_fmadd_pd(_mm256_loadu_pd(pu), _mm256_loadu_pd(pu), vreg0);
							pu += 4;
						}
						varr.v = _mm256_hadd_pd(vreg0, vreg0);
						duu = varr.arr[0] + varr.arr[2];
						for (ii = ur & 3; ii > 0; ii--)
						{
							duu += (*pu) * (*pu); pu++;
						}

						pu = pfu;
						if (*pu > 0.0) { *pu += std::sqrt(duu); }
						else { *pu -= std::sqrt(duu); }

						duu = 0.0;
						vreg0 = _mm256_setzero_pd();
						for (ii = ur - (ur & 3); ii > 0; ii -= 4)
						{
							vreg0 = _mm256_fmadd_pd(_mm256_loadu_pd(pu), _mm256_loadu_pd(pu), vreg0);
							pu += 4;
						}
						varr.v = _mm256_hadd_pd(vreg0, vreg0);
						duu = varr.arr[0] + varr.arr[2];
						for (ii = ur & 3; ii > 0; ii--)
						{
							duu += (*pu) * (*pu); pu++;
						}

						if (duu != 0.0)
						{
							duu = (-2.0 / duu);
							M = Ac - j;

							std::memset(pfv, 0, Ac * sizeof(double));
							pA0 = (pfA + j) + (Ac * i);
							pA1 = pA0 + Ac;
							pA2 = pA0 + 2 * Ac;
							pA3 = pA0 + 3 * Ac;
							pu = pfu;
							jump = j + 3 * Ac;
							for (ii = ur - (ur & 3); ii > 0; ii -= 4)
							{
								pv = pfv;
								vreg0 = _mm256_broadcast_sd(pu);
								vreg1 = _mm256_broadcast_sd(pu + 1);
								vreg2 = _mm256_broadcast_sd(pu + 2);
								vreg3 = _mm256_broadcast_sd(pu + 3);
								for (jj = M - (M & 3); jj > 0; jj -= 4)
								{
									vreg = _mm256_loadu_pd(pv);
									vreg = _mm256_fmadd_pd(vreg0, _mm256_loadu_pd(pA0), vreg); pA0 += 4;
									vreg = _mm256_fmadd_pd(vreg1, _mm256_loadu_pd(pA1), vreg); pA1 += 4;
									vreg = _mm256_fmadd_pd(vreg2, _mm256_loadu_pd(pA2), vreg); pA2 += 4;
									vreg = _mm256_fmadd_pd(vreg3, _mm256_loadu_pd(pA3), vreg); pA3 += 4;
									_mm256_storeu_pd(pv, vreg); pv += 4;
								}
								reg0 = *pu;
								reg1 = *(pu + 1);
								reg2 = *(pu + 2);
								reg3 = *(pu + 3);
								pu += 4;
								for (jj = M & 3; jj > 0; jj--)
								{
									*pv++ += (reg0 * (*pA0++) + reg1 * (*pA1++))
										+ (reg2 * (*pA2++) + reg3 * (*pA3++));
								}
								pA0 += jump;
								pA1 += jump;
								pA2 += jump;
								pA3 += jump;
							}

							switch (ur & 3)
							{

							case 0:
								break;

							case 1:
								pv = pfv;
								vreg0 = _mm256_broadcast_sd(pu);
								for (jj = M - (M & 3); jj > 0; jj -= 4)
								{
									_mm256_storeu_pd(pv, _mm256_fmadd_pd(vreg0,
										_mm256_loadu_pd(pA0), _mm256_loadu_pd(pv)));
									pA0 += 4; pv += 4;
								}
								reg0 = *pu++;
								for (jj = M & 3; jj > 0; jj--)
								{
									*pv++ += reg0 * (*pA0++);
								}
								break;

							case 2:
								pv = pfv;
								vreg0 = _mm256_broadcast_sd(pu);
								vreg1 = _mm256_broadcast_sd(pu + 1);
								for (jj = M - (M & 3); jj > 0; jj -= 4)
								{
									vreg = _mm256_loadu_pd(pv);
									vreg = _mm256_fmadd_pd(vreg0, _mm256_loadu_pd(pA0), vreg); pA0 += 4;
									vreg = _mm256_fmadd_pd(vreg1, _mm256_loadu_pd(pA1), vreg); pA1 += 4;
									_mm256_storeu_pd(pv, vreg); pv += 4;
								}
								reg0 = *pu;
								reg1 = *(pu + 1);
								for (jj = M & 3; jj > 0; jj--)
								{
									*pv++ += (reg0 * (*pA0++) + reg1 * (*pA1++));
								}
								break;

							case 3:
								pv = pfv;
								vreg0 = _mm256_broadcast_sd(pu);
								vreg1 = _mm256_broadcast_sd(pu + 1);
								vreg2 = _mm256_broadcast_sd(pu + 2);
								for (jj = M - (M & 3); jj > 0; jj -= 4)
								{
									vreg = _mm256_loadu_pd(pv);
									vreg = _mm256_fmadd_pd(vreg0, _mm256_loadu_pd(pA0), vreg); pA0 += 4;
									vreg = _mm256_fmadd_pd(vreg1, _mm256_loadu_pd(pA1), vreg); pA1 += 4;
									vreg = _mm256_fmadd_pd(vreg2, _mm256_loadu_pd(pA2), vreg); pA2 += 4;
									_mm256_storeu_pd(pv, vreg); pv += 4;
								}
								reg0 = *pu;
								reg1 = *(pu + 1);
								reg2 = *(pu + 2);
								for (jj = M & 3; jj > 0; jj--)
								{
									*pv++ += (reg0 * (*pA0++) + reg1 * (*pA1++))
										+ (reg2 * (*pA2++));
								}
								break;
							}

							pv = pfv;
							vreg0 = _mm256_broadcast_sd(&duu);
							for (jj = M - (M & 3); jj > 0; jj -= 4)
							{
								_mm256_storeu_pd(pv, _mm256_mul_pd(vreg0, _mm256_loadu_pd(pv)));
								pv += 4;
							}
							for (jj = M & 3; jj > 0; jj--)
							{
								*pv++ *= duu;
							}

							pA0 = (pfA + j) + (Ac * i);
							pA1 = pA0 + Ac;
							pA2 = pA0 + 2 * Ac;
							pA3 = pA0 + 3 * Ac;
							pu = pfu;
							jump = j + 3 * Ac;
							for (ii = ur - (ur & 3); ii > 0; ii -= 4)
							{
								pv = pfv;
								vreg0 = _mm256_broadcast_sd(pu);
								vreg1 = _mm256_broadcast_sd(pu + 1);
								vreg2 = _mm256_broadcast_sd(pu + 2);
								vreg3 = _mm256_broadcast_sd(pu + 3);
								for (jj = M - (M & 3); jj > 0; jj -= 4)
								{
									vreg = _mm256_loadu_pd(pv); pv += 4;
									_mm256_storeu_pd(pA0, _mm256_fmadd_pd(vreg0, vreg, _mm256_loadu_pd(pA0))); pA0 += 4;
									_mm256_storeu_pd(pA1, _mm256_fmadd_pd(vreg1, vreg, _mm256_loadu_pd(pA1))); pA1 += 4;
									_mm256_storeu_pd(pA2, _mm256_fmadd_pd(vreg2, vreg, _mm256_loadu_pd(pA2))); pA2 += 4;
									_mm256_storeu_pd(pA3, _mm256_fmadd_pd(vreg3, vreg, _mm256_loadu_pd(pA3))); pA3 += 4;
								}
								reg0 = *pu;
								reg1 = *(pu + 1);
								reg2 = *(pu + 2);
								reg3 = *(pu + 3);
								pu += 4;
								for (jj = M & 3; jj > 0; jj--)
								{
									regv = *pv++;
									*pA0++ += reg0 * regv;
									*pA1++ += reg1 * regv;
									*pA2++ += reg2 * regv;
									*pA3++ += reg3 * regv;
								}
								pA0 += jump;
								pA1 += jump;
								pA2 += jump;
								pA3 += jump;
							}

							switch (ur & 3)
							{

							case 0:
								break;

							case 1:
								pv = pfv;
								vreg0 = _mm256_broadcast_sd(pu);
								for (jj = M - (M & 3); jj > 0; jj -= 4)
								{
									_mm256_storeu_pd(pA0, _mm256_fmadd_pd(vreg0,
										_mm256_loadu_pd(pv), _mm256_loadu_pd(pA0)));
									pA0 += 4; pv += 4;
								}
								reg0 = *pu;
								for (jj = M & 3; jj > 0; jj--)
								{
									*pA0++ += reg0 * (*pv++);
								}
								break;

							case 2:
								pv = pfv;
								vreg0 = _mm256_broadcast_sd(pu);
								vreg1 = _mm256_broadcast_sd(pu + 1);
								for (jj = M - (M & 3); jj > 0; jj -= 4)
								{
									vreg = _mm256_loadu_pd(pv); pv += 4;
									_mm256_storeu_pd(pA0, _mm256_fmadd_pd(vreg0, vreg, _mm256_loadu_pd(pA0))); pA0 += 4;
									_mm256_storeu_pd(pA1, _mm256_fmadd_pd(vreg1, vreg, _mm256_loadu_pd(pA1))); pA1 += 4;
								}
								reg0 = *pu;
								reg1 = *(pu + 1);
								for (jj = M & 3; jj > 0; jj--)
								{
									regv = *pv++;
									*pA0++ += reg0 * regv;
									*pA1++ += reg1 * regv;
								}
								break;

							case 3:
								pv = pfv;
								vreg0 = _mm256_broadcast_sd(pu);
								vreg1 = _mm256_broadcast_sd(pu + 1);
								vreg2 = _mm256_broadcast_sd(pu + 2);
								for (jj = M - (M & 3); jj > 0; jj -= 4)
								{
									vreg = _mm256_loadu_pd(pv); pv += 4;
									_mm256_storeu_pd(pA0, _mm256_fmadd_pd(vreg0, vreg, _mm256_loadu_pd(pA0))); pA0 += 4;
									_mm256_storeu_pd(pA1, _mm256_fmadd_pd(vreg1, vreg, _mm256_loadu_pd(pA1))); pA1 += 4;
									_mm256_storeu_pd(pA2, _mm256_fmadd_pd(vreg2, vreg, _mm256_loadu_pd(pA2))); pA2 += 4;
								}
								reg0 = *pu;
								reg1 = *(pu + 1);
								reg2 = *(pu + 2);
								for (jj = M & 3; jj > 0; jj--)
								{
									regv = *pv++;
									*pA0++ += reg0 * regv;
									*pA1++ += reg1 * regv;
									*pA2++ += reg2 * regv;
								}
								break;
							}
						}
						j++;
					}
					else { break; }
				}
			}

			std::size_t ndiag = (Ar < Nc) ? Ar : Nc;
			pA0 = pfA + Ac;
			for (i = 1; i < ndiag; i++)
			{
				std::memset(pA0, 0, i * sizeof(double));
				pA0 += Ac;
			}
			if (Ar > Nc)
			{
				for (i = Ar - Nc; i > 0; i--)
				{
					std::memset(pA0, 0, ndiag * sizeof(double));
					pA0 += Ac;
				}
			}
		}
		void elim_givens(double* const pfA, const std::size_t Ar, const std::size_t Ac, const std::size_t Nc, const std::size_t slice) noexcept
		{
			double C; double S; double temp0; double temp1;
			double* p; double* q;
			std::size_t N;
			std::size_t i, j, iOut, jOut, jmax, jj;

			__m256d vp; __m256d vq; __m256d vC; __m256d vS;

			for (jOut = 0; jOut < Nc; jOut += slice)
			{
				jmax = (jOut + slice < Nc) ? jOut + slice : Nc;
				for (iOut = Ar - 1; iOut > jOut; iOut--)
				{
					for (j = jOut; j < jmax; j++)
					{
						i = iOut + (j - jOut);

						if (i < Ar)
						{
							q = (pfA + j) + (Ac * i);
							if (*q != 0.0)
							{
								p = q - Ac;

								C = *p;
								S = *q;
								temp0 = 1.0 / std::sqrt(C * C + S * S);
								C *= temp0;
								S *= temp0;

								temp1 = *q;
								*q = 0.0;
								*p = C * (*p) + S * temp1;
								q++; p++;
								N = Ac - (j + 1);
								vC = _mm256_broadcast_sd(&C);
								vS = _mm256_broadcast_sd(&S);

								for (jj = N - (N & 3); jj > 0; jj -= 4)
								{
									vp = _mm256_loadu_pd(p); vq = _mm256_loadu_pd(q);
									_mm256_storeu_pd(q, _mm256_fnmadd_pd(vS, vp, _mm256_mul_pd(vC, vq)));
									_mm256_storeu_pd(p, _mm256_fmadd_pd(vC, vp, _mm256_mul_pd(vS, vq)));
									q += 4; p += 4;
								}
								for (jj = N & 3; jj > 0; jj--)
								{
									temp0 = *p; temp1 = *q;
									*p++ = C * temp0 + S * temp1;
									*q++ = C * temp1 - S * temp0;
								}
							}
						}
					}
				}
			}
		}
		void trisolve(double* const pfX, const double* const pfU, const std::size_t Ur, const std::size_t Xc, const std::size_t slice) noexcept
		{
			std::size_t Uc = Ur + Xc;

			if (Xc == 1)
			{
				double acc;
				const double* pU; double* pX1;
				std::size_t k;
				std::size_t M;

				__m256d vreg;
				union { __m256d v; double arr[4]; } varr;

				for (std::size_t i = Ur - 1; i + 1 > 0; i--)
				{
					pU = (pfU + (i + 1)) + (Uc * i);
					pX1 = pfX + (i + 1);
					M = Ur - (i + 1);
					acc = 0.0;
					vreg = _mm256_setzero_pd();

					for (k = M - (M & 3); k > 0; k -= 4)
					{
						vreg = _mm256_fnmadd_pd(_mm256_loadu_pd(pU), _mm256_loadu_pd(pX1), vreg);
						pU += 4; pX1 += 4;
					}
					varr.v = _mm256_hadd_pd(vreg, vreg);
					acc = varr.arr[0] + varr.arr[2];
					for (k = M & 3; k > 0; k--)
					{
						acc -= (*pU++) * (*pX1++);
					}

					acc += *pU;
					*(pfX + i) = acc / *((pfU + i) + (Uc * i));
				}
				return;
			}

			else
			{
				double temp;
				std::memset(pfX, 0, Ur * Xc * sizeof(double));
				const double* pU; double* pX1; double* pX2;
				std::size_t M = Xc - (Xc & 3);
				std::size_t i, j, ii, iOut, imin, ip;

				__m256d vtemp;

				for (iOut = Ur - 1; iOut < Ur; iOut -= slice)
				{
					imin = (iOut - slice < Ur) ? iOut - slice + 1 : 0;
					for (ip = iOut + 1; ip > imin; ip--)
					{
						i = ip - 1;
						pX1 = pfX + (Xc * i);
						pU = (pfU + Ur) + (Uc * i);
						for (j = M; j > 0; j -= 4)
						{
							_mm256_storeu_pd(pX1, _mm256_add_pd(_mm256_loadu_pd(pX1),
								_mm256_loadu_pd(pU)));
							pU += 4; pX1 += 4;
						}
						for (j = Xc - M; j > 0; j--)
						{
							*pX1++ += *pU++;
						}

						pX1 = pfX + (Xc * i);
						temp = 1.0 / *((pfU + i) + (Uc * i));
						vtemp = _mm256_broadcast_sd(&temp);
						for (j = M; j > 0; j -= 4)
						{
							_mm256_storeu_pd(pX1, _mm256_mul_pd(_mm256_loadu_pd(pX1), vtemp));
							pX1 += 4;
						}
						for (j = Xc - M; j > 0; j--)
						{
							(*pX1++) *= temp;
						}

						for (ii = imin; ii < i; ii++)
						{
							pX1 = pfX + (Xc * i);
							pX2 = pfX + (Xc * ii);
							temp = *((pfU + i) + (Uc * ii));
							vtemp = _mm256_broadcast_sd(&temp);
							for (j = M; j > 0; j -= 4)
							{
								_mm256_storeu_pd(pX2, _mm256_fnmadd_pd(vtemp,
									_mm256_loadu_pd(pX1), _mm256_loadu_pd(pX2)));
								pX1 += 4; pX2 += 4;
							}
							for (j = Xc - M; j > 0; j--)
							{
								*pX2++ -= temp * (*pX1++);
							}
						}
					}

					for (ii = 0; ii < imin; ii++)
					{
						for (i = imin; i < iOut + 1; i++)
						{
							pX1 = pfX + (Xc * i);
							pX2 = pfX + (Xc * ii);
							temp = *((pfU + i) + (Uc * ii));
							vtemp = _mm256_broadcast_sd(&temp);
							for (j = M; j > 0; j -= 4)
							{
								_mm256_storeu_pd(pX2, _mm256_fnmadd_pd(vtemp,
									_mm256_loadu_pd(pX1), _mm256_loadu_pd(pX2)));
								pX1 += 4; pX2 += 4;
							}
							for (j = Xc - M; j > 0; j--)
							{
								*pX2++ -= temp * (*pX1++);
							}
						}
					}
				}
			}
		}
	}
#endif // WELP_MATRIX_AVX_EXT
}


////// CONTAINER //////

template <typename Ty, class _Allocator> inline void welp::_matrix_container<Ty, _Allocator>::construct_all()
{
	if (typeid(Ty) == typeid(float) || typeid(Ty) == typeid(double) || typeid(Ty) == typeid(int))
	{
		return;
	}
	else
	{
		Ty* p = data_ptr;
		std::size_t r = storage_capacity & 3;
		std::size_t k;

		for (k = storage_capacity - r; k > 0; k -= 4)
		{
			new (p) Ty();
			new (p + 1) Ty();
			new (p + 2) Ty();
			new (p + 3) Ty();
			p += 4;
		}

		switch (r)
		{

		case 0:
			break;

		case 1:
			new (p) Ty();
			break;

		case 2:
			new (p) Ty();
			new (p + 1) Ty();
			break;

		case 3:
			new (p) Ty();
			new (p + 1) Ty();
			new (p + 2) Ty();
			break;
		}
	}
}
template <typename Ty, class _Allocator> inline void welp::_matrix_container<Ty, _Allocator>::construct_all(Ty* ptr, std::size_t instances)
{
	if (typeid(Ty) == typeid(float) || typeid(Ty) == typeid(double) || typeid(Ty) == typeid(int))
	{
		return;
	}
	else
	{
		Ty* p = ptr;
		std::size_t r = instances & 3;
		std::size_t k;

		for (k = instances - r; k > 0; k -= 4)
		{
			new (p) Ty();
			new (p + 1) Ty();
			new (p + 2) Ty();
			new (p + 3) Ty();
			p += 4;
		}

		switch (r)
		{

		case 0:
			break;

		case 1:
			new (p) Ty();
			break;

		case 2:
			new (p) Ty();
			new (p + 1) Ty();
			break;

		case 3:
			new (p) Ty();
			new (p + 1) Ty();
			new (p + 2) Ty();
			break;
		}
	}
}
template <typename Ty, class _Allocator> inline void welp::_matrix_container<Ty, _Allocator>::destroy_all() noexcept
{
	if (typeid(Ty) == typeid(float) || typeid(Ty) == typeid(double) || typeid(Ty) == typeid(int))
	{
		return;
	}
	else
	{
		Ty* p = data_ptr + (storage_capacity - 1);
		std::size_t r = storage_capacity & 3;
		std::size_t k;

		for (k = storage_capacity - r; k > 0; k -= 4)
		{
			p->~Ty();
			(p - 1)->~Ty();
			(p - 2)->~Ty();
			(p - 3)->~Ty();
			p -= 4;
		}

		switch (r)
		{

		case 0:
			break;

		case 1:
			p->~Ty();
			break;

		case 2:
			p->~Ty();
			(p - 1)->~Ty();
			break;

		case 3:
			p->~Ty();
			(p - 1)->~Ty();
			(p - 2)->~Ty();
			break;
		}
	}
}
template <typename Ty, class _Allocator> inline void welp::_matrix_container<Ty, _Allocator>::destroy_all(Ty* ptr, std::size_t instances) noexcept
{
	if (typeid(Ty) == typeid(float) || typeid(Ty) == typeid(double) || typeid(Ty) == typeid(int))
	{
		return;
	}
	else
	{
		Ty* p = ptr + (instances - 1);
		std::size_t r = instances & 3;
		std::size_t k;

		for (k = instances - r; k > 0; k -= 4)
		{
			p->~Ty();
			(p - 1)->~Ty();
			(p - 2)->~Ty();
			(p - 3)->~Ty();
			p -= 4;
		}

		switch (r)
		{

		case 0:
			break;

		case 1:
			p->~Ty();
			break;

		case 2:
			p->~Ty();
			(p - 1)->~Ty();
			break;

		case 3:
			p->~Ty();
			(p - 1)->~Ty();
			(p - 2)->~Ty();
			break;
		}
	}
}
template <typename Ty, class _Allocator> inline void welp::_matrix_container<Ty, _Allocator>::copy_all
(Ty* copy_to_ptr, Ty* copy_from_ptr, std::size_t instances)
{
	if (typeid(Ty) == typeid(float) || typeid(Ty) == typeid(double) || typeid(Ty) == typeid(int))
	{
		std::memcpy(copy_to_ptr, copy_from_ptr, instances * sizeof(Ty));
		return;
	}
	else
	{
		Ty* p = copy_to_ptr; Ty* q = copy_from_ptr;
		std::size_t r = instances & 3;
		std::size_t k;

		for (k = instances - r; k > 0; k -= 4)
		{
			*p = *q;
			*(p + 1) = *(q + 1);
			*(p + 2) = *(q + 2);
			*(p + 3) = *(q + 3);
			p += 4; q += 4;
		}

		switch (r)
		{

		case 0:
			break;

		case 1:
			*p = *q;
			break;

		case 2:
			*p = *q;
			*(p + 1) = *(q + 1);
			break;

		case 3:
			*p = *q;
			*(p + 1) = *(q + 1);
			*(p + 2) = *(q + 2);
			break;
		}
	}
}
template <typename Ty, class _Allocator> inline void welp::_matrix_container<Ty, _Allocator>::move_all
(Ty* copy_to_ptr, Ty* copy_from_ptr, std::size_t instances)
{
	if (typeid(Ty) == typeid(float) || typeid(Ty) == typeid(double) || typeid(Ty) == typeid(int))
	{
		std::memcpy(copy_to_ptr, copy_from_ptr, instances * sizeof(Ty));
		return;
	}
	else
	{
		Ty* p = copy_to_ptr; Ty* q = copy_from_ptr;
		std::size_t r = instances & 3;
		std::size_t k;

		for (k = instances - r; k > 0; k -= 4)
		{
			*p = std::move(*q);
			*(p + 1) = std::move(*(q + 1));
			*(p + 2) = std::move(*(q + 2));
			*(p + 3) = std::move(*(q + 3));
			p += 4; q += 4;
		}

		switch (r)
		{

		case 0:
			break;

		case 1:
			*p = std::move(*q);
			break;

		case 2:
			*p = std::move(*q);
			*(p + 1) = std::move(*(q + 1));
			break;

		case 3:
			*p = std::move(*q);
			*(p + 1) = std::move(*(q + 1));
			*(p + 2) = std::move(*(q + 2));
			break;
		}
	}
}


// constructors, assign, copy, destructor

template <typename Ty, class _Allocator> welp::_matrix_container<Ty, _Allocator>::_matrix_container() noexcept : data_ptr(nullptr), end_ptr(nullptr), rows(0), cols(0), storage_capacity(0) {}
template <typename Ty, class _Allocator> welp::_matrix_container<Ty, _Allocator>::_matrix_container(std::size_t new_capacity) : rows(0), cols(0), storage_capacity(new_capacity)
{
	if (storage_capacity != 0)
	{
		data_ptr = this->allocate(new_capacity);
		end_ptr = data_ptr + storage_capacity;
		this->construct_all();
	}
	else
	{
		data_ptr = nullptr;
		end_ptr = nullptr;
	}
}
template <typename Ty, class _Allocator> welp::_matrix_container<Ty, _Allocator>::_matrix_container(std::size_t new_r, std::size_t new_c) : rows(new_r), cols(new_c), storage_capacity(new_r* new_c)
{
	if (storage_capacity != 0)
	{
		data_ptr = this->allocate(storage_capacity);
		end_ptr = data_ptr + storage_capacity;
		this->construct_all();
	}
	else
	{
		data_ptr = nullptr;
		end_ptr = nullptr;
	}
}
template <typename Ty, class _Allocator> welp::_matrix_container<Ty, _Allocator>::_matrix_container(std::size_t new_capacity, std::size_t new_r, std::size_t new_c) : rows(new_r), cols(new_c), storage_capacity(new_capacity)
{
	if (storage_capacity != 0)
	{
		data_ptr = this->allocate(new_capacity);
		end_ptr = data_ptr + (new_r * new_c);
		this->construct_all();
	}
	else
	{
		data_ptr = nullptr;
		end_ptr = nullptr;
	}
}

template <typename Ty, class _Allocator> welp::_matrix_container<Ty, _Allocator>::_matrix_container(const welp::_matrix_container<Ty, _Allocator>& rhs_container)
{
	// if rhs_container is NOT allocated
	if (rhs_container.data_ptr == nullptr)
	{
		data_ptr = nullptr;
		end_ptr = nullptr;
		rows = 0;
		cols = 0;
		storage_capacity = 0;
		return;
	}

	// if rhs_container IS allocated
	else
	{
		rows = rhs_container.rows;
		cols = rhs_container.cols;
		storage_capacity = rows * cols;
		data_ptr = this->allocate(storage_capacity);
		end_ptr = data_ptr + storage_capacity;

		this->construct_all();
		this->copy_all(data_ptr, rhs_container.data_ptr, storage_capacity);
	}
}
template <typename Ty, class _Allocator> welp::_matrix_container<Ty, _Allocator>& welp::_matrix_container<Ty, _Allocator>::operator=(const welp::_matrix_container<Ty, _Allocator>& rhs_container)
{
	// rhs_container IS empty
	if (rhs_container.data_ptr == nullptr)
	{
		// *this IS not empty
		if (data_ptr != nullptr)
		{
			this->destroy_all();
			this->deallocate(data_ptr, storage_capacity);

			data_ptr = nullptr;
			end_ptr = nullptr;
			rows = 0;
			cols = 0;
			storage_capacity = 0;
			return *this;
		}
		else
		{
			return *this;
		}
	}

	// rhs_container is NOT empty
	else
	{
		// *this is empty
		if (data_ptr == nullptr)
		{
			rows = rhs_container.rows;
			cols = rhs_container.cols;
			storage_capacity = rows * cols;

			data_ptr = this->allocate(storage_capacity);
			end_ptr = data_ptr + storage_capacity;

			this->construct_all();
			this->copy_all(data_ptr, rhs_container.data_ptr, storage_capacity);
			return *this;
		}

		// *this is NOT empty
		else
		{
			std::size_t needed_capacity_value = rhs_container.rows * rhs_container.cols;
			// *this has SUFFICIENT capacity
			if (needed_capacity_value < storage_capacity)
			{
				this->copy_all(data_ptr, rhs_container.data_ptr, needed_capacity_value);

				rows = rhs_container.rows;
				cols = rhs_container.cols;
				return *this;
			}
			// *this has INSUFFICIENT capacity
			else
			{
				this->destroy_all();
				this->deallocate(data_ptr, storage_capacity);

				data_ptr = this->allocate(needed_capacity_value);
				end_ptr = data_ptr + needed_capacity_value;
				rows = rhs_container.rows;
				cols = rhs_container.cols;
				storage_capacity = needed_capacity_value;

				this->construct_all();
				this->copy_all(data_ptr, rhs_container.data_ptr, needed_capacity_value);
				return *this;
			}
		}
	}
}
template <typename Ty, class _Allocator> welp::_matrix_container<Ty, _Allocator>::_matrix_container(_matrix_container<Ty, _Allocator>&& rhs_container) noexcept
{
	// moving
	data_ptr = std::move(rhs_container.data_ptr);
	end_ptr = std::move(rhs_container.end_ptr);
	rows = std::move(rhs_container.rows);
	cols = std::move(rhs_container.cols);
	storage_capacity = std::move(rhs_container.storage_capacity);

	// emptying rhs_container
	rhs_container.data_ptr = nullptr;
	rhs_container.end_ptr = nullptr;
	rhs_container.rows = 0;
	rhs_container.cols = 0;
	rhs_container.storage_capacity = 0;
}
template <typename Ty, class _Allocator> welp::_matrix_container<Ty, _Allocator>& welp::_matrix_container<Ty, _Allocator>::operator=(welp::_matrix_container<Ty, _Allocator>&& rhs_container) noexcept
{
	// if rhs_container IS allocated
	if (rhs_container.data_ptr != nullptr)
	{
		// freeing resources if *this IS allocated
		if (data_ptr != nullptr)
		{
			this->destroy_all();
			this->deallocate(data_ptr, storage_capacity);
		}

		// moving
		data_ptr = std::move(rhs_container.data_ptr);
		end_ptr = std::move(rhs_container.end_ptr);
		rows = std::move(rhs_container.rows);
		cols = std::move(rhs_container.cols);
		storage_capacity = std::move(rhs_container.storage_capacity);

		// emptying rhs_container
		rhs_container.data_ptr = nullptr;
		rhs_container.end_ptr = nullptr;
		rhs_container.rows = 0;
		rhs_container.cols = 0;
		rhs_container.storage_capacity = 0;
		return *this;
	}

	// if rhs_container is NOT allocated
	else
	{
		// if rhs_container IS allocated
		if (data_ptr != nullptr)
		{
			this->destroy_all();
			this->deallocate(data_ptr, storage_capacity);
			data_ptr = nullptr;
			end_ptr = nullptr;
			rows = 0;
			cols = 0;
			storage_capacity = 0;
		}
		return *this;
	}
}

template <typename Ty, class _Allocator> welp::_matrix_container<Ty, _Allocator>::~_matrix_container() noexcept
{
	if (data_ptr != nullptr)
	{
		this->destroy_all();
		this->deallocate(data_ptr, storage_capacity);
	}
}


// accessors
template <typename Ty, class _Allocator> inline const Ty& welp::_matrix_container<Ty, _Allocator>::operator[](std::size_t offset) const noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(offset < rows* cols);
#endif // WELP_MATRIX_DEBUG_MODE
	return *(data_ptr + offset);
}
template <typename Ty, class _Allocator> inline Ty& welp::_matrix_container<Ty, _Allocator>::operator[](std::size_t offset) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(offset < rows* cols);
#endif // WELP_MATRIX_DEBUG_MODE
	return *(data_ptr + offset);
}
template <typename Ty, class _Allocator> inline const Ty& welp::_matrix_container<Ty, _Allocator>::operator()(std::size_t row_index, std::size_t col_index) const noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(row_index < rows);
	assert(col_index < cols);
#endif // WELP_MATRIX_DEBUG_MODE
	return *((data_ptr + col_index) + (cols * row_index));
}
template <typename Ty, class _Allocator> inline Ty& welp::_matrix_container<Ty, _Allocator>::operator()(std::size_t row_index, std::size_t col_index) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(row_index < rows);
	assert(col_index < cols);
#endif // WELP_MATRIX_DEBUG_MODE
	return *((data_ptr + col_index) + (cols * row_index));
}

// memory and dimension management
template <typename Ty, class _Allocator> void welp::_matrix_container<Ty, _Allocator>::reserve(std::size_t new_capacity_value)
{
	// if new capacity is smaller than size
	if (new_capacity_value < rows * cols)
	{
		new_capacity_value = rows * cols;
	}
	// if new capacity is different than previous
	if (new_capacity_value != storage_capacity)
	{
		if (new_capacity_value != 0)
		{
			_matrix_temp temp(new_capacity_value); // temp storage
			this->construct_all(temp.data_ptr, new_capacity_value);
			this->move_all(temp.data_ptr, data_ptr, rows * cols); // copy to temp
			this->destroy_all();
			this->deallocate(data_ptr, storage_capacity); // deallocate old storage

			// move
			data_ptr = temp.data_ptr;
			end_ptr = data_ptr + rows * cols;
			storage_capacity = new_capacity_value;

			// discard temp
			temp.data_ptr = nullptr;
			return;
		}
		else
		{
			if (data_ptr != nullptr)
			{
				this->destroy_all();
				this->deallocate(data_ptr, storage_capacity);

				data_ptr = nullptr;
				end_ptr = nullptr;
				rows = 0;
				cols = 0;
				storage_capacity = 0;
			}
		}
	}
}
template <typename Ty, class _Allocator> void welp::_matrix_container<Ty, _Allocator>::reserve(std::size_t new_r, std::size_t new_c)
{
	std::size_t new_capacity_value = new_r * new_c; // new capacity

	// if new capacity is smaller than size
	if (new_capacity_value < rows * cols)
	{
		new_capacity_value = rows * cols;
	}
	// if new capacity is different than previous
	if (new_capacity_value != rows * cols)
	{
		if (new_capacity_value != 0)
		{
			_matrix_temp temp(new_capacity_value); // temp storage
			this->construct_all(temp.data_ptr, new_capacity_value);
			this->move_all(temp.data_ptr, data_ptr, rows * cols); // copy to temp
			this->destroy_all();
			this->deallocate(data_ptr, storage_capacity); // deallocate old storage

			// move
			data_ptr = temp.data_ptr;
			end_ptr = data_ptr + rows * cols;
			storage_capacity = new_capacity_value;

			// discard temp
			temp.data_ptr = nullptr;
			return;
		}
		else
		{
			if (data_ptr != nullptr)
			{
				this->destroy_all();
				this->deallocate(data_ptr, storage_capacity);

				data_ptr = nullptr;
				end_ptr = nullptr;
				rows = 0;
				cols = 0;
				storage_capacity = 0;
			}
		}
	}
}
template <typename Ty, class _Allocator> void welp::_matrix_container<Ty, _Allocator>::resize(std::size_t new_r, std::size_t new_c)
{
	std::size_t new_size = new_r * new_c;
	// if previous capacity IS sufficient
	if (new_size <= storage_capacity)
	{
		rows = new_r;
		cols = new_c;
		end_ptr = data_ptr + new_size;
		return;
	}
	// if previous capacity is NOT sufficient
	else
	{
		_matrix_temp temp(new_size); // temp storage
		this->construct_all(temp.data_ptr, new_size);
		this->move_all(temp.data_ptr, data_ptr, rows * cols); // copy to temp
		this->destroy_all();
		this->deallocate(data_ptr, storage_capacity); // deallocate old storage

		// move
		data_ptr = temp.data_ptr;
		end_ptr = data_ptr + new_size;
		storage_capacity = new_size;
		rows = new_r;
		cols = new_c;

		// discard temp
		temp.data_ptr = nullptr;
	}
}
template <typename Ty, class _Allocator> void welp::_matrix_container<Ty, _Allocator>::shrink_to_fit()
{
	std::size_t new_capacity_value = rows * cols;

	// if new capacity is different than previous
	if (new_capacity_value < storage_capacity)
	{
		if (new_capacity_value != 0)
		{
			_matrix_temp temp(new_capacity_value); // temp storage
			this->construct_all(temp.data_ptr, new_capacity_value);
			this->move_all(temp.data_ptr, data_ptr, rows * cols);
			this->destroy_all();
			this->deallocate(data_ptr, storage_capacity); // deallocate old storage

			// move
			data_ptr = temp.data_ptr;
			end_ptr = data_ptr + new_capacity_value;
			storage_capacity = new_capacity_value;

			// discard temp
			temp.data_ptr = nullptr;
		}
		else
		{
			if (data_ptr != nullptr)
			{
				this->destroy_all();
				this->deallocate(data_ptr, storage_capacity);

				data_ptr = nullptr;
				end_ptr = nullptr;
				rows = 0;
				cols = 0;
				storage_capacity = 0;
			}
		}
	}
}
template <typename Ty, class _Allocator> void welp::_matrix_container<Ty, _Allocator>::clear() noexcept
{
	if (data_ptr != nullptr)
	{
		this->destroy_all();
		this->deallocate(data_ptr, storage_capacity);
	}

	data_ptr = nullptr;
	end_ptr = nullptr;
	rows = 0;
	cols = 0;
	storage_capacity = 0;
}


////// MATRIX CLASS //////

// constructors and destructors

template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>::matrix(std::size_t new_capacity) : welp::_matrix_container<Ty, _Allocator>(new_capacity) {}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>::matrix(std::size_t new_r, std::size_t new_c) : welp::_matrix_container<Ty, _Allocator>(new_r, new_c) {}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>::matrix(std::size_t new_r, std::size_t new_c, Ty x) : welp::_matrix_container<Ty, _Allocator>(new_r, new_c)
{
	welp::matrix_subroutines::fill(this->data(), x, this->size());
}
#ifdef WELP_MATRIX_INCLUDE_INITLIST
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>::matrix(std::size_t new_r, std::size_t new_c, std::initializer_list<Ty> L) : welp::_matrix_container<Ty, _Allocator>(new_r, new_c)
{
	std::copy(L.begin(), L.end(), this->data());
}
#endif // WELP_MATRIX_INCLUDE_INITLIST
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>::~matrix() = default;

// set
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::cpy(const Ty* const p)
{
	welp::matrix_subroutines::cpy(this->data(), p, this->size());
	return *this;
}
template <typename Ty, class _Allocator> template <class iter> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::cpy(iter p)
{
	Ty* q = this->data();
	for (std::size_t k = this->size(); k > 0; k--)
	{
		*q = *p; ++q; ++p;
	}
	return *this;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::fill(Ty x)
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::fill(this->data(), x, this->size());
	return *this;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::diag(Ty x)
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::diag(this->data(), x, this->r(), this->c(), 0, 0);
	return *this;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::diag(Ty x, std::size_t i0, std::size_t j0)
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(i0 < this->r());
	assert(j0 < this->c());
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::diag(this->data(), x, this->r(), this->c(), i0, j0);
	return *this;
}

// submatrix operations
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> welp::matrix<Ty, _Allocator>::get_row(std::size_t i0) const
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(i0 < this->r());
#endif // WELP_MATRIX_DEBUG_MODE

	welp::matrix<Ty, _Allocator> C(1, this->c());
	welp::matrix_subroutines::get_row(C.data(), this->data(), i0, this->c());
	return C;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> welp::matrix<Ty, _Allocator>::get_col(std::size_t j0) const
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(j0 < this->c());
#endif // WELP_MATRIX_DEBUG_MODE

	welp::matrix<Ty, _Allocator> C(this->r(), 1);
	welp::matrix_subroutines::get_col(C.data(), this->data(), j0, this->r(), this->c());
	return C;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> welp::matrix<Ty, _Allocator>::get_blk(std::size_t i0, std::size_t j0, std::size_t Cr, std::size_t Cc) const
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(i0 < this->r());
	assert(j0 < this->c());
	assert(i0 + Cr <= this->r());
	assert(j0 + Cc <= this->c());

#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix<Ty, _Allocator> C(Cr, Cc);
	welp::matrix_subroutines::get_blk(C.data(), this->data(), i0, j0, Cr, Cc, this->c());
	return C;
}
template <typename Ty, class _Allocator> template <class _Allocator_C> void welp::matrix<Ty, _Allocator>::get_into(std::size_t i0, std::size_t j0, welp::matrix<Ty, _Allocator_C> & C) const
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(C.data() != nullptr);
	assert(i0 < this->r());
	assert(j0 < this->c());
	assert(C.r() <= i0 + this->r());
	assert(C.c() <= j0 + this->c());

#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::get_blk(C.data(), this->data(), i0, j0, C.r(), C.c(), this->c());
}
template <typename Ty, class _Allocator> template <class _Allocator_A> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::insert(std::size_t i0, std::size_t j0, const welp::matrix<Ty, _Allocator_A> & A)
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(A.data() != nullptr);
	assert(i0 < this->r());
	assert(j0 < this->c());
	assert(i0 + A.r() <= this->r());
	assert(j0 + A.c() <= this->c());
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::insert(this->data(), A.data(), i0, j0, this->c(), A.r(), A.c());
	return *this;
}

// linear algebra operations
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::adj()
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE

	if ((this->c() == 1) || (this->r() == 1))
	{
		this->resize(this->c(), this->r());
		return *this;
	}
	else if (this->r() == this->c())
	{
		welp::matrix_subroutines::adj_sqm(this->data(), this->r());
		return *this;
	}
	else
	{
		welp::matrix<Ty, _Allocator> A = *this;
		welp::matrix_subroutines::adj(this->data(), A.data(), A.r(), A.c());
		this->resize(this->c(), this->r());
		return *this;
	}
}
template <typename Ty, class _Allocator> template <class _Allocator_A> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::adj(const welp::matrix<Ty, _Allocator_A> & A)
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE

	this->resize(A.c(), A.r());
	if ((this->c() == 1) || (this->r() == 1))
	{
		welp::matrix_subroutines::cpy(this->data(), A.data(), this->size());
	}
	else
	{
		welp::matrix_subroutines::adj(this->data(), A.data(), A.r(), A.c());
	}
	return *this;
}

template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::operator+=(Ty x) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE

	welp::matrix_subroutines::ps(this->data(), x, this->size());
	return *this;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::operator-=(Ty x) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE

	welp::matrix_subroutines::ps(this->data(), -x, this->size());
	return *this;
}
template <typename Ty, class _Allocator> template <class _Allocator_A> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::operator+=(const welp::matrix<Ty, _Allocator_A> & A) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(A.data() != nullptr);
	assert(this->r() == A.r());
	assert(this->c() == A.c());
#endif // WELP_MATRIX_DEBUG_MODE

	welp::matrix_subroutines::pm(this->data(), A.data(), this->size());
	return *this;
}
template <typename Ty, class _Allocator> template <class _Allocator_A> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::operator-=(const welp::matrix<Ty, _Allocator_A> & A) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(A.data() != nullptr);
	assert(this->r() == A.r());
	assert(this->c() == A.c());
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::p_m(this->data(), A.data(), this->size());
	return *this;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::operator*=(Ty x) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::xs(this->data(), x, this->size());
	return *this;
}
template <typename Ty, class _Allocator> template <class _Allocator_A> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::sxm(Ty x, const welp::matrix<Ty, _Allocator_A> & A) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(A.data() != nullptr);
	assert(this->r() == A.r());
	assert(this->c() == A.c());
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::sxm(this->data(), x, A.data(), this->size());
	return *this;
}
template <typename Ty, class _Allocator> template <class _Allocator_A> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::psxm(Ty x, const welp::matrix<Ty, _Allocator_A> & A) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(A.data() != nullptr);
	assert(this->r() == A.r());
	assert(this->c() == A.c());
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::psxm(this->data(), x, A.data(), this->size());
	return *this;
}
template <typename Ty, class _Allocator> template <class _Allocator_A> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::p_sxm(Ty x, const welp::matrix<Ty, _Allocator_A> & A) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(A.data() != nullptr);
	assert(this->r() == A.r());
	assert(this->c() == A.c());
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::psxm(this->data(), -x, A.data(), this->size());
	return *this;
}
template <typename Ty, class _Allocator> template <class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::pmxm(const welp::matrix<Ty, _Allocator_A> & A, const welp::matrix<Ty, _Allocator_B> & B) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(A.data() != nullptr);
	assert(B.data() != nullptr);
	assert(this->r() == A.r());
	assert(this->c() == B.c());
	assert(A.c() == B.r());
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::pmxm(this->data(), A.data(), B.data(), A.r(), B.c(), A.c());
	return *this;
}
template <typename Ty, class _Allocator> template <class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::p_mxm(const welp::matrix<Ty, _Allocator_A> & A, const welp::matrix<Ty, _Allocator_B> & B) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(A.data() != nullptr);
	assert(B.data() != nullptr);
	assert(this->r() == A.r());
	assert(this->c() == B.c());
	assert(A.c() == B.r());
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::p_mxm(this->data(), A.data(), B.data(), A.r(), B.c(), A.c());
	return *this;
}
template <typename Ty, class _Allocator> template <class _Allocator_v, class _Allocator_w> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::pvxv(const welp::matrix<Ty, _Allocator_v> & v, const welp::matrix<Ty, _Allocator_w> & w) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(v.data() != nullptr);
	assert(w.data() != nullptr);
	assert(this->r() == v.size());
	assert(this->c() == w.size());
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::pmxm(this->data(), v.data(), w.data(), this->r(), this->c(), 1);
	return *this;
}
template <typename Ty, class _Allocator> template <class _Allocator_v, class _Allocator_w> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::p_vxv(const welp::matrix<Ty, _Allocator_v> & v, const welp::matrix<Ty, _Allocator_w> & w) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(v.data() != nullptr);
	assert(w.data() != nullptr);
	assert(this->r() == v.size());
	assert(this->c() == w.size());
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::p_mxm(this->data(), v.data(), w.data(), this->r(), this->c(), 1);
	return *this;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::pdiag(Ty x) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::pdiag(this->data(), x, this->r(), this->c(), 0, 0);
	return *this;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::p_diag(Ty x) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::pdiag(this->data(), -x, this->r(), this->c(), 0, 0);
	return *this;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::pdiag(Ty x, std::size_t i0, std::size_t j0) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(i0 < this->r());
	assert(j0 < this->c());
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::pdiag(this->data(), x, this->r(), this->c(), i0, j0);
	return *this;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::p_diag(Ty x, std::size_t i0, std::size_t j0) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(i0 < this->r());
	assert(j0 < this->c());
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix_subroutines::pdiag(this->data(), -x, this->r(), this->c(), i0, j0);
	return *this;
}

template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::elim_gauss() noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
	std::size_t slice;
#ifdef WELP_MATRIX_AVX_EXT
	if (typeid(Ty) == typeid(float))
	{
		slice = WELP_MATRIX_AVX_ps_elim_T / this->c();
		if (slice < 5) { slice = 5; }
	}
	else if (typeid(Ty) == typeid(double))
	{
		slice = WELP_MATRIX_AVX_pd_elim_T / this->c();
		if (slice < 5) { slice = 5; }
	}
	else { slice = 0; }
#else
	slice = 0;
#endif
	welp::matrix_subroutines::elim_gauss(this->data(), this->r(), this->c(), slice);
	return *this;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::elim_householder(std::size_t Nc)
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(Nc <= this->r());
	assert(Nc <= this->c());
#endif // WELP_MATRIX_DEBUG_MODE
	std::size_t slice;
#ifdef WELP_MATRIX_AVX_EXT
	if (typeid(Ty) == typeid(float))
	{
		slice = WELP_MATRIX_AVX_ps_elim_T / this->c();
		if (slice < 5) { slice = 5; }
	}
	else if (typeid(Ty) == typeid(double))
	{
		slice = WELP_MATRIX_AVX_pd_elim_T / this->c();
		if (slice < 5) { slice = 5; }
	}
	else { slice = 0; }
#else
	slice = 0;
#endif
	welp::matrix<Ty, _Allocator> uv(this->r() + this->c());
	welp::matrix_subroutines::elim_householder(this->data(), this->r(), this->c(), Nc,
		uv.data(), uv.data() + this->r(), slice);
	return *this;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::elim_givens(std::size_t Nc) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(Nc <= this->r());
	assert(Nc <= this->c());
#endif // WELP_MATRIX_DEBUG_MODE
	std::size_t slice;
#ifdef WELP_MATRIX_AVX_EXT
	if (typeid(Ty) == typeid(float))
	{
		slice = WELP_MATRIX_AVX_ps_elim_T / this->c();
		if (slice < 4) { slice = 4; }
	}
	else if (typeid(Ty) == typeid(double))
	{
		slice = WELP_MATRIX_AVX_pd_elim_T / this->c();
		if (slice < 4) { slice = 4; }
	}
	else { slice = 0; }
#else
	slice = 0;
#endif
	welp::matrix_subroutines::elim_givens(this->data(), this->r(), this->c(), Nc, slice);
	return *this;
}
template <typename Ty, class _Allocator> template <class _Allocator_U> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::trisolve(const welp::matrix<Ty, _Allocator_U> & U) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(U.data() != nullptr);
	assert(this->r() + this->c() == U.c());
	assert(this->r() <= U.r());
#endif // WELP_MATRIX_DEBUG_MODE
#ifdef WELP_MATRIX_AVX_EXT
	if (typeid(Ty) == typeid(float))
	{
		welp::matrix_subroutines::trisolve(this->data(), U.data(), this->r(), this->c(), WELP_MATRIX_AVX_ps_trisolve_Ti);
	}
	else if (typeid(Ty) == typeid(double))
	{
		welp::matrix_subroutines::trisolve(this->data(), U.data(), this->r(), this->c(), WELP_MATRIX_AVX_pd_trisolve_Ti);
	}
	else { welp::matrix_subroutines::trisolve(this->data(), U.data(), this->r(), this->c(), 0); }
#else
	welp::matrix_subroutines::trisolve(this->data(), U.data(), this->r(), this->c(), 0);
#endif
	return *this;
}

template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::sort(std::size_t col)
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix<std::size_t> num0(this->r());
	welp::matrix<std::size_t> num1(this->r());
	std::size_t Bc = (this->c() < 2) ? 2 : this->c();
	welp::matrix<Ty, _Allocator> B(this->r() * Bc);
	welp::matrix_subroutines::sort(this->data(), this->r(), this->c(), col, B.data(), num0.data(), num1.data());
	return *this;
}
template <typename Ty, class _Allocator> template <class Predicate> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::sort(std::size_t col, Predicate less_Predicate)
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
	welp::matrix<std::size_t> num0(this->r());
	welp::matrix<std::size_t> num1(this->r());
	std::size_t Bc = (this->c() < 2) ? 2 : this->c();
	welp::matrix<Ty, _Allocator> B(this->r() * Bc);
	welp::matrix_subroutines::sort(this->data(), this->r(), this->c(), col, B.data(), num0.data(), num1.data(), less_Predicate);
	return *this;
}
template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::sort(std::size_t col, std::size_t row_begin, std::size_t row_end)
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(row_end < this->r());
	assert(row_begin < row_end);
#endif // WELP_MATRIX_DEBUG_MODE
	std::size_t Br = row_end - row_begin;
	welp::matrix<std::size_t> num0(Br);
	welp::matrix<std::size_t> num1(Br);
	std::size_t Bc = (this->c() < 2) ? 2 : this->c();
	welp::matrix<Ty, _Allocator> B(Br * Bc);
	welp::matrix_subroutines::sort(this->data() + this->c() * row_begin, Br, this->c(), col,
		B.data(), num0.data(), num1.data());
	return *this;
}
template <typename Ty, class _Allocator> template <class Predicate> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::sort(std::size_t col, std::size_t row_begin, std::size_t row_end, Predicate Pr)
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(this->data() != nullptr);
	assert(row_end < this->r());
	assert(row_begin < row_end);
#endif // WELP_MATRIX_DEBUG_MODE
	std::size_t Br = row_end - row_begin;
	welp::matrix<std::size_t> num0(Br);
	welp::matrix<std::size_t> num1(Br);
	std::size_t Bc = (this->c() < 2) ? 2 : this->c();
	welp::matrix<Ty, _Allocator> B(Br * Bc);
	welp::matrix_subroutines::sort(this->data() + this->c() * row_begin, Br, this->c(), col,
		B.data(), num0.data(), num1.data(), Pr);
	return *this;
}

#ifdef WELP_MATRIX_INCLUDE_IOSTREAM
template <typename Ty, class _Allocator> template <std::size_t cmax, std::size_t rblock, std::size_t cwidth> const welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::say() const
{
	if (this->data() == nullptr) { std::cout << ">>>         matrix is not allocated" << std::endl; }
	else if (this->size() == 0) { std::cout << ">>>         matrix is empty" << std::endl; }
	else { welp::matrix_subroutines::say_rm(this->data(), this->r(), this->c(), cmax, rblock, cwidth); }
	return *this;
}
template <typename Ty, class _Allocator> template <std::size_t cmax, std::size_t rblock, std::size_t cwidth, typename msg_Ty> const welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::say
(const msg_Ty & msg) const
{
	std::cout << "[ " << msg << " ]\n";
	return this->say();
}
template <typename Ty, class _Allocator> template <std::size_t cmax, std::size_t rblock, std::size_t cwidth, typename msg_Ty1, typename msg_Ty2> const welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::say
(const msg_Ty1 & msg1, const msg_Ty2 & msg2) const
{
	std::cout << "[ " << msg1 << " " << msg2 << " ]\n";
	return this->say();
}
template <typename Ty, class _Allocator> template <std::size_t cmax, std::size_t rblock, std::size_t cwidth, typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> const welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::say
(const msg_Ty1 & msg1, const msg_Ty2 & msg2, const msg_Ty3 & msg3) const
{
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n";
	return this->say();
}
template <typename Ty, class _Allocator> template <std::size_t cmax, std::size_t rblock, std::size_t cwidth, typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> const welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::say
(const msg_Ty1 & msg1, const msg_Ty2 & msg2, const msg_Ty3 & msg3, const msg_Ty4 & msg4) const
{
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n";
	return this->say();
}
template <typename Ty, class _Allocator> template <std::size_t cmax, std::size_t rblock, std::size_t cwidth> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::say()
{
	if (this->data() == nullptr) { std::cout << ">>>         matrix is not allocated" << std::endl; }
	else if (this->size() == 0) { std::cout << ">>>         matrix is empty" << std::endl; }
	else { welp::matrix_subroutines::say_rm(this->data(), this->r(), this->c(), cmax, rblock, cwidth); }
	return *this;
}
template <typename Ty, class _Allocator> template <std::size_t cmax, std::size_t rblock, std::size_t cwidth, typename msg_Ty> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::say
(const msg_Ty & msg)
{
	std::cout << "[ " << msg << " ]\n";
	return this->say();
}
template <typename Ty, class _Allocator> template <std::size_t cmax, std::size_t rblock, std::size_t cwidth, typename msg_Ty1, typename msg_Ty2> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::say
(const msg_Ty1 & msg1, const msg_Ty2 & msg2)
{
	std::cout << "[ " << msg1 << " " << msg2 << " ]\n";
	return this->say();
}
template <typename Ty, class _Allocator> template <std::size_t cmax, std::size_t rblock, std::size_t cwidth, typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::say
(const msg_Ty1 & msg1, const msg_Ty2 & msg2, const msg_Ty3 & msg3)
{
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n";
	return this->say();
}
template <typename Ty, class _Allocator> template <std::size_t cmax, std::size_t rblock, std::size_t cwidth, typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> welp::matrix<Ty, _Allocator>& welp::matrix<Ty, _Allocator>::say
(const msg_Ty1 & msg1, const msg_Ty2 & msg2, const msg_Ty3 & msg3, const msg_Ty4 & msg4)
{
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n";
	return this->say();
}
#endif // WELP_MATRIX_INCLUDE_IOSTREAM

template <std::size_t length, typename Ty, class _Allocator>
inline const welp::const_matrix_ptr<Ty, _Allocator>& welp::matrix_stream<length, Ty, _Allocator>::operator[](std::size_t offset) const noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(offset < length);
#endif // WELP_MATRIX_DEBUG_MODE
	return arr[offset];
}
template <std::size_t length, typename Ty, class _Allocator>
inline welp::const_matrix_ptr<Ty, _Allocator>& welp::matrix_stream<length, Ty, _Allocator>::operator[](std::size_t offset) noexcept
{
#ifdef WELP_MATRIX_DEBUG_MODE
	assert(offset < length);
#endif // WELP_MATRIX_DEBUG_MODE
	return arr[offset];
}
template <std::size_t length, typename Ty, class _Allocator>
inline welp::matrix_stream<length, Ty, _Allocator>& welp::matrix_stream<length, Ty, _Allocator>::operator<<(const welp::matrix<Ty, _Allocator> & A) noexcept
{
	arr[this->count++] = &A;
	return *this;
}
template <std::size_t length, typename Ty, class _Allocator>
inline welp::matrix_stream<length, Ty, _Allocator>& welp::matrix_stream<length, Ty, _Allocator>::operator<<(std::size_t repeat) noexcept
{
	if (count != 0 && repeat > 1)
	{
		welp::const_matrix_ptr<Ty, _Allocator> ptr = this->arr[this->count - 1];
		for (std::size_t k = repeat - 1; k > 0; k--)
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(count < length);
#endif // WELP_MATRIX_DEBUG_MODE
		{
			arr[this->count++] = ptr;
		}
	}
	return *this;
}

namespace welp
{
	// operators

	template <typename Ty, class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator_B> operator+(
		const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(B.data() != nullptr);
		assert(A.r() == B.r());
		assert(A.c() == B.c());
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<Ty, _Allocator_B> C(A.r(), A.c());
		welp::matrix_subroutines::mpm(C.data(), A.data(), B.data(), A.size());
		return C;
	}

	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> operator-(const welp::matrix<Ty, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<Ty, _Allocator> C(A.r(), A.c());
		welp::matrix_subroutines::_m(C.data(), A.data(), A.size());
		return C;
	}

	template <typename Ty, class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator_B> operator-(
		const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(B.data() != nullptr);
		assert(A.r() == B.r());
		assert(A.c() == B.c());
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<Ty, _Allocator_B> C(A.r(), A.c());
		welp::matrix_subroutines::mp_m(C.data(), A.data(), B.data(), A.size());
		return C;
	}

	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> operator*(Ty x, const welp::matrix<Ty, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<Ty, _Allocator> C(A.r(), A.c());
		welp::matrix_subroutines::sxm(C.data(), x, A.data(), A.size());
		return C;
	}

	template <typename Ty, class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator_B> operator*(
		const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(B.data() != nullptr);
		assert(A.c() == B.r());
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<Ty, _Allocator_B> C(A.r(), B.c(), static_cast<Ty>(0));
		welp::matrix_subroutines::pmxm(C.data(), A.data(), B.data(), A.r(), B.c(), A.c());
		return C;
	}

	template <typename Ty, class _Allocator, typename Tn> welp::matrix<Ty, _Allocator> pow(const welp::matrix<Ty, _Allocator>& A, Tn n)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(A.c() == A.r());
#endif // WELP_MATRIX_DEBUG_MODE
		std::size_t m = A.r();
		welp::matrix<Ty, _Allocator> C(m, m);
		welp::matrix<Ty, _Allocator> pool0(m, m);
		welp::matrix<Ty, _Allocator> pool1(m, m);
		welp::matrix_subroutines::pow(C.data(), A.data(), n, m, pool0.data(), pool1.data());
		return C;
	}

	// comparisons

	template <typename Ty, class _Allocator_A, class _Allocator_B> bool operator==(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(B.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		if (A.r() != B.r() || A.c() != B.c()) { return false; }
		else
		{
			return welp::matrix_subroutines::compare_equal_mm(A.data(), B.data(), A.size());
		}
	}
	template <typename Ty, class _Allocator_A, class _Allocator_B> bool operator!=(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(B.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		if (A.r() != B.r() || A.c() != B.c()) { return false; }
		else
		{
			return !welp::matrix_subroutines::compare_equal_mm(A.data(), B.data(), A.size());
		}
	}
	template <typename Ty, class _Allocator_A, class _Allocator_B> bool operator<(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(B.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		if (A.r() != B.r() || A.c() != B.c()) { return false; }
		else
		{
			return welp::matrix_subroutines::compare_strictly_smaller_mm(A.data(), B.data(), A.size());
		}
	}
	template <typename Ty, class _Allocator_A, class _Allocator_B> bool operator>(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(B.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		if (A.r() != B.r() || A.c() != B.c()) { return false; }
		else
		{
			return welp::matrix_subroutines::compare_strictly_greater_mm(A.data(), B.data(), A.size());
		}
	}
	template <typename Ty, class _Allocator_A, class _Allocator_B> bool operator<=(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(B.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		if (A.r() != B.r() || A.c() != B.c()) { return false; }
		else
		{
			return welp::matrix_subroutines::compare_smaller_mm(A.data(), B.data(), A.size());
		}
	}
	template <typename Ty, class _Allocator_A, class _Allocator_B> bool operator>=(const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(B.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		if (A.r() != B.r() || A.c() != B.c()) { return false; }
		else
		{
			return welp::matrix_subroutines::compare_greater_mm(A.data(), B.data(), A.size());
		}
	}
	template <typename Ty1, typename Ty2, class relation_function, class _Allocator_A, class _Allocator_B> bool relation(
		relation_function _relation, const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(B.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		if (A.r() != B.r() || A.c() != B.c()) { return false; }
		else
		{
			return welp::matrix_subroutines::compare_mm(A.data(), B.data(), A.size(), _relation);
		}
	}

	template <typename Ty, class _Allocator> bool operator==(const welp::matrix<Ty, _Allocator>& A, const Ty& x) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::compare_equal_ms(A.data(), x, A.size());
	}
	template <typename Ty, class _Allocator> bool operator!=(const welp::matrix<Ty, _Allocator>& A, const Ty& x) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return !welp::matrix_subroutines::compare_equal_ms(A.data(), x, A.size());
	}
	template <typename Ty, class _Allocator> bool operator<(const welp::matrix<Ty, _Allocator>& A, const Ty& x) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::compare_strictly_smaller_ms(A.data(), x, A.size());
	}
	template <typename Ty, class _Allocator> bool operator>(const welp::matrix<Ty, _Allocator>& A, const Ty& x) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::compare_strictly_greater_ms(A.data(), x, A.size());
	}
	template <typename Ty, class _Allocator> bool operator<=(const welp::matrix<Ty, _Allocator>& A, const Ty& x) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::compare_smaller_ms(A.data(), x, A.size());
	}
	template <typename Ty, class _Allocator> bool operator>=(const welp::matrix<Ty, _Allocator>& A, const Ty& x) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::compare_greater_ms(A.data(), x, A.size());
	}

	template <typename Ty, class _Allocator> bool operator==(const Ty& x, const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::compare_equal_ms(A.data(), x, A.size());
	}
	template <typename Ty, class _Allocator> bool operator!=(const Ty& x, const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return !welp::matrix_subroutines::compare_equal_ms(A.data(), x, A.size());
	}
	template <typename Ty, class _Allocator> bool operator<(const Ty& x, const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::compare_strictly_greater_ms(A.data(), x, A.size());
	}
	template <typename Ty, class _Allocator> bool operator>(const Ty& x, const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::compare_strictly_smaller_ms(A.data(), x, A.size());
	}
	template <typename Ty, class _Allocator> bool operator<=(const Ty& x, const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::compare_greater_ms(A.data(), x, A.size());
	}
	template <typename Ty, class _Allocator> bool operator>=(const Ty& x, const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::compare_smaller_ms(A.data(), x, A.size());
	}
	template <typename Ty, class relation_function, class _Allocator> bool relation(relation_function _relation, const welp::matrix<Ty, _Allocator>& A, const Ty& x)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::compare_ms(A.data(), x, A.size(), _relation);
	}

	template <typename Ty, class _Allocator> bool well_defined(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty* pA = A.data();
		for (std::size_t k = A.size(); k > 0; k--)
		{
			if (std::isnan(*pA)) { return false; }
			pA++;
		}
		return true;
	}

	template <std::size_t length, typename Ty, class _Allocator>
	inline welp::matrix_stream<length, Ty, _Allocator> operator<<(const welp::matrix<Ty, _Allocator>& A, const welp::matrix<Ty, _Allocator>& B)
	{
		welp::matrix_stream<length, Ty, _Allocator> S;
		S[0] = &A;
		S[1] = &B;
		S.count = 2;
		return S;
	}
	template <std::size_t length, typename Ty, class _Allocator>
	inline welp::matrix_stream<length, Ty, _Allocator> operator<<(const welp::matrix<Ty, _Allocator>& A, std::size_t n)
	{
		welp::matrix_stream<length, Ty, _Allocator> S;
		for (std::size_t k = 0; k < n; k++)
		{
			S[k] = &A;
		}
		S.count = n;
		return S;
	}
}

////// display //////

#ifdef WELP_MATRIX_INCLUDE_IOSTREAM
template <typename Ty, class _Allocator> std::ostream& operator<<(std::ostream & out, const welp::matrix<Ty, _Allocator> & A)
{
	A.say(); return out;
}
#endif // WELP_MATRIX_INCLUDE_IOSTREAM


////// commons //////

namespace welp
{
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> id(std::size_t N, Ty x)
	{
		welp::matrix<Ty, _Allocator> C(N, N, static_cast<Ty>(0));
		C.diag(x);
		return C;
	}
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> random_matrix(std::size_t M, std::size_t N, Ty a, Ty b)
	{
		welp::matrix<Ty, _Allocator> C(M, N);
		Ty x = (b - a) / static_cast<Ty>(RAND_MAX);
		Ty* pC = C.data();
		for (std::size_t k = C.size(); k > 0; k--) { *pC++ = a + x * std::rand(); }
		return C;
	}
	template <class _Allocator = WELP_MATRIX_DEFAULT_ALLOCATOR<int>> welp::matrix<int, _Allocator> random_matrix(std::size_t M, std::size_t N, int a, int b)
	{
		welp::matrix<int, _Allocator> C(M, N);
		int x = b - a + 1;
		int* pC = C.data();
		for (std::size_t k = C.size(); k > 0; k--) { *pC++ = a + std::rand() % x; }
		return C;
	}
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> linspace_length(Ty x, Ty length, Ty y)
	{
		welp::matrix<Ty, _Allocator> C(static_cast<std::size_t>(std::abs((y - x) / length)) + 1, 1);
		Ty* pC = C.data();
		Ty N = C.size();
		length = std::abs(length);
		if (x < y)
		{
			for (Ty k = 0; k < N; k++) { *pC++ = x + k * length; }
		}
		else
		{
			for (Ty k = 0; k < N; k++) { *pC++ = x - k * length; }
		}
		return C;
	}
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> linspace_number(Ty x, std::size_t number, Ty y)
	{
		welp::matrix<Ty, _Allocator> C(number + 1, 1);
		Ty length = (y - x) / static_cast<Ty>(number);
		Ty* pC = C.data();
		Ty N = C.size();
		for (Ty k = 0; k < N; k++) { *pC++ = x + k * length; }
		return C;
	}

	template <typename Ty, class _Allocator> Ty min(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty* pA = A.data();
		Ty x = *pA;
		for (std::size_t k = A.size(); k > 0; k--)
		{
			if (*pA < x) { x = *pA; }
			pA++;
		}
		return x;
	}
	template <typename Ty, class _Allocator> Ty max(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty* pA = A.data();
		Ty x = *pA;
		for (std::size_t k = A.size(); k > 0; k--)
		{
			if (*pA > x) { x = *pA; }
			pA++;
		}
		return x;
	}
	template <typename Ty, class _Allocator> Ty sum(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty* pA = A.data();
		Ty acc = static_cast<Ty>(0);
		for (std::size_t k = A.size(); k > 0; k--)
		{
			acc += *pA++;
		}
		return acc;
	}

	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> abs(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<Ty, _Allocator> C(A.r(), A.c());
		const Ty* pC = C.data(); Ty* pA = A.data();
		for (std::size_t k = A.size(); k > 0; k--)
		{
			*pC++ = std::abs(*pA++);
		}
		return C;
	}
	template <typename Ty, class _Allocator> Ty min_abs(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty* pA = A.data();
		Ty x = std::abs(*pA);
		Ty temp;
		for (std::size_t k = A.size(); k > 0; k--)
		{
			temp = std::abs(*pA);
			if (temp < x) { x = temp; }
			pA++;
		}
		return x;
	}
	template <typename Ty, class _Allocator> Ty max_abs(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty* pA = A.data();
		Ty x = std::abs(*pA);
		Ty temp;
		for (std::size_t k = A.size(); k > 0; k--)
		{
			temp = std::abs(*pA);
			if (temp > x) { x = temp; }
			pA++;
		}
		return x;
	}
	template <typename Ty, class _Allocator> Ty sum_abs(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty* pA = A.data();
		Ty acc = static_cast<Ty>(0);
		for (std::size_t k = A.size(); k > 0; k--)
		{
			acc += std::abs(*pA++);
		}
		return acc;
	}

	template <typename Ty, class _Allocator> std::size_t arg_min(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty* pA = A.data();
		Ty x = *pA;
		std::size_t n = A.size();
		std::size_t arg = 0;
		for (std::size_t k = 0; k < n; k++)
		{
			if (*pA < x) { x = *pA; arg = k; }
			pA++;
		}
		return arg;
	}
	template <typename Ty, class _Allocator> std::size_t arg_max(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty* pA = A.data();
		Ty x = *pA;
		std::size_t n = A.size();
		std::size_t arg = 0;
		for (std::size_t k = 0; k < n; k++)
		{
			if (*pA > x) { x = *pA; arg = k; }
			pA++;
		}
		return arg;
	}
	template <typename Ty, class _Allocator> std::size_t arg_min_abs(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty* pA = A.data();
		Ty x = std::abs(*pA);
		Ty temp;
		std::size_t n = A.size();
		std::size_t arg = 0;
		for (std::size_t k = 0; k < n; k++)
		{
			temp = std::abs(*pA);
			if (temp < x) { x = temp; arg = k; }
			pA++;
		}
		return arg;
	}
	template <typename Ty, class _Allocator> std::size_t arg_max_abs(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty* pA = A.data();
		Ty x = std::abs(*pA);
		Ty temp;
		std::size_t n = A.size();
		std::size_t arg = 0;
		for (std::size_t k = 0; k < n; k++)
		{
			temp = std::abs(*pA);
			if (temp > x) { x = temp; arg = k; }
			pA++;
		}
		return arg;
	}

	template <typename Ty, class _Allocator> Ty trace(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(A.r() == A.c());
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty* pA = A.data();
		Ty acc = static_cast<Ty>(0);
		std::size_t N = A.r() + 1;
		for (std::size_t k = A.r(); k > 0; k--)
		{
			acc += *pA;
			pA += N;
		}
		return acc;
	}

	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> adj(const welp::matrix<Ty, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		if ((A.c() == 1) || (A.r() == 1))
		{
			welp::matrix<Ty, _Allocator> C = A;
			C.resize(C.c(), C.r());
			return C;
		}
		else
		{
			welp::matrix<Ty, _Allocator> C(A.c(), A.r());
			welp::matrix_subroutines::adj(C.data(), A.data(), A.r(), A.c());
			return C;
		}
	}
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> flip_lr(const welp::matrix<Ty, _Allocator>& A)
	{
		welp::matrix<Ty, _Allocator> C(A.r(), A.c());
		const Ty* pA = A.data() + C.c() - 1; Ty* pC = *(Ty**)(&C);
		std::size_t Cc2 = 2 * C.c();
		std::size_t j;
		for (std::size_t i = C.r(); i > 0; i--)
		{
			for (j = C.c(); j > 0; j--) { *pC++ = *pA--; }
			pA += Cc2;
		}
		return C;
	}
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> flip_ud(const welp::matrix<Ty, _Allocator>& A)
	{
		welp::matrix<Ty, _Allocator> C(A.r(), A.c());
		const Ty* pA = A.data(); Ty* pC = C.data() + C.c() * (C.r() - 1);
		std::size_t Cc = C.c();
		for (std::size_t i = C.r(); i > 0; i--)
		{
			welp::matrix_subroutines::cpy(pC, pA, Cc);
			pA += Cc; pC -= Cc;
		}
		return C;
	}

	template <typename Ty, std::size_t length, class _Allocator> welp::matrix<Ty, _Allocator> blk_hor(const welp::matrix_stream<length, Ty, _Allocator>& blk)
	{
		std::size_t Cr = blk[0]->r();
		std::size_t Cc = 0;
		for (std::size_t n = 0; n < blk.count; n++)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(blk[n]->data() != nullptr);
			assert(blk[n]->r() == Cr);
#endif // WELP_MATRIX_DEBUG_MODE
			Cc += blk[n]->c();
		}
		welp::matrix<Ty, _Allocator> C(Cr, Cc);
		Cc = 0;
		for (std::size_t n = 0; n < blk.count; n++)
		{
			C.insert(0, Cc, *blk[n]);
			Cc += blk[n]->c();
		}
		return C;
	}
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> blk_hor(const welp::matrix<Ty, _Allocator>& A)
	{
		welp::matrix<Ty, _Allocator> C = A;
		return C;
	}

	template <typename Ty, std::size_t length, class _Allocator> welp::matrix<Ty, _Allocator> blk_vert(const welp::matrix_stream<length, Ty, _Allocator>& blk)
	{
		std::size_t Cr = 0;
		std::size_t Cc = blk[0]->c();
		for (std::size_t n = 0; n < blk.count; n++)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(blk[n]->data() != nullptr);
			assert(blk[n]->c() == Cc);
#endif // WELP_MATRIX_DEBUG_MODE
			Cr += blk[n]->r();
		}
		welp::matrix<Ty, _Allocator> C(Cr, Cc);
		Ty* pC = C.data();
		for (std::size_t n = 0; n < blk.count; n++)
		{
			welp::matrix_subroutines::cpy(pC, blk[n]->data(), blk[n]->size());
			pC += blk[n]->size();
		}
		return C;
	}
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> blk_vert(const welp::matrix<Ty, _Allocator>& A)
	{
		welp::matrix<Ty, _Allocator> C = A;
		return C;
	}

	template <typename Ty, std::size_t length, class _Allocator> welp::matrix<Ty, _Allocator> blk_diag(const welp::matrix_stream<length, Ty, _Allocator>& blk)
	{
		std::size_t Cr = 0; std::size_t Cc = 0;
		for (std::size_t n = 0; n < blk.count; n++)
		{
			Cr += blk[n]->r();
			Cc += blk[n]->c();
		}
		welp::matrix<Ty, _Allocator> C(Cr, Cc, Ty());
		Cr = 0; Cc = 0;
		for (std::size_t n = 0; n < blk.count; n++)
		{
			C.insert(Cr, Cc, *blk[n]);
			Cr += blk[n]->r();
			Cc += blk[n]->c();
		}
		return C;
	}
	template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> blk_diag(const welp::matrix<Ty, _Allocator>& A)
	{
		welp::matrix<Ty, _Allocator> C = A;
		return C;
	}
}


////// dot product, norm //////

namespace welp
{
	template <typename Ty, class _Allocator_A, class _Allocator_B> Ty dot(
		const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(B.data() != nullptr);
		assert(A.size() == B.size());
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::dot(A.data(), B.data(), A.size());
	}
	template <typename Ty, class _Allocator> Ty norm(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return std::sqrt(welp::matrix_subroutines::norm2(A.data(), A.size()));
	}
	template <typename Ty, class _Allocator> Ty norm2(const welp::matrix<Ty, _Allocator>& A) noexcept
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::norm2(A.data(), A.size());
	}
}


////// solve, inverse, determinant //////

namespace welp
{
	namespace solve
	{
		template <typename Ty, class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator_B> gauss(
			const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(B.data() != nullptr);
			assert(A.r() == B.r());
#endif // WELP_MATRIX_DEBUG_MODE
			welp::matrix<Ty, _Allocator_B> X(B.r(), B.c());
			welp::matrix<Ty, _Allocator_A> U(B.r(), A.c() + B.c());
			U.insert(0, 0, A);
			U.insert(0, A.c(), B);
			U.elim_gauss();
			return X.trisolve(U);
		}

		template <typename Ty, class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator_B> householder(
			const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(B.data() != nullptr);
			assert(A.r() == B.r());
#endif // WELP_MATRIX_DEBUG_MODE
			if (A.r() >= A.c())
			{
				welp::matrix<Ty, _Allocator_B> X(A.c(), B.c());
				welp::matrix<Ty, _Allocator_A> U(A.r(), A.c() + B.c());
				U.insert(0, 0, A);
				U.insert(0, A.c(), B);
				U.elim_householder(A.c());
				return X.trisolve(U);
			}
			else
			{
				welp::matrix<Ty, _Allocator_B> X = welp::adj(A);
				welp::matrix<Ty, _Allocator_A> U(X.r(), X.r() + X.c(), static_cast<Ty>(0));
				U.insert(0, 0, X);
				U.diag(static_cast<Ty>(1), 0, X.c());
				U.elim_householder(X.c());
				X.resize(X.c(), X.r());
				return U.adj(X.trisolve(U)) * B;
			}
		}

		template <typename Ty, class _Allocator_A, class _Allocator_B> welp::matrix<Ty, _Allocator_B> givens(
			const welp::matrix<Ty, _Allocator_A>& A, const welp::matrix<Ty, _Allocator_B>& B)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(B.data() != nullptr);
			assert(A.r() == B.r());
#endif // WELP_MATRIX_DEBUG_MODE
			if (A.r() >= A.c())
			{
				welp::matrix<Ty, _Allocator_B> X(A.c(), B.c());
				welp::matrix<Ty, _Allocator_A> U(A.r(), A.c() + B.c());
				U.insert(0, 0, A);
				U.insert(0, A.c(), B);
				U.elim_givens(A.c());
				return X.trisolve(U);
			}
			else
			{
				welp::matrix<Ty, _Allocator_B> X = welp::adj(A);
				welp::matrix<Ty, _Allocator_A> U(X.r(), X.r() + X.c(), static_cast<Ty>(0));
				U.insert(0, 0, X);
				U.diag(static_cast<Ty>(1), 0, X.c());
				U.elim_givens(X.c());
				X.resize(X.c(), X.r());
				return U.adj(X.trisolve(U)) * B;
			}
		}

		template <typename Ty, class function_f, class function_J, class _Allocator> welp::matrix<Ty, _Allocator> newton(const function_f& f,
			const function_J& J, const welp::matrix<Ty, _Allocator>& x0, const welp::matrix<Ty, _Allocator>& b, int max_iter, Ty tol)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(x0.data() != nullptr);
			assert(b.data() != nullptr);
			assert(x0.r() == J(x0).c());
			assert(x0.c() == 1);
			assert(b.r() == J(x0).r());
			assert(b.c() == 1);
#endif // WELP_MATRIX_DEBUG_MODE
			welp::matrix<Ty, _Allocator> x = x0;
			welp::matrix<Ty, _Allocator> y = std::move(f(x) -= b);
			tol *= tol;

			while ((welp::norm2(y) > tol) && (max_iter > 0))
			{
				x -= welp::solve::givens(J(x), y);
				y = std::move(f(x) -= b);
				max_iter--;
			}
			return x;
		}

		template <typename Ty, class function_f, class _Allocator> void broyden_update(const function_f& f, welp::matrix<Ty, _Allocator>& Jinv,
			welp::matrix<Ty, _Allocator>& x, const welp::matrix<Ty, _Allocator>& b, int max_iter, Ty tol)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(Jinv.data() != nullptr);
			assert(x.data() != nullptr);
			assert(b.data() != nullptr);
			assert(Jinv.r() == Jinv.c());
			assert(x.r() == Jinv.r());
			assert(x.c() == 1);
			assert(b.r() == Jinv.r());
			assert(b.c() == 1);
#endif // WELP_MATRIX_DEBUG_MODE
			welp::matrix<Ty, _Allocator> s(x.r(), 1);
			welp::matrix<Ty, _Allocator> stemp(x.r(), 1);
			welp::matrix<Ty, _Allocator> Jinvtemp = Jinv;
			welp::matrix<Ty, _Allocator> y = f(x) -= b;
			tol *= tol;

			while ((welp::norm2(y) > tol) && (max_iter > 0))
			{
				s.fill(static_cast<Ty>(0));
				s.pmxm(Jinv, y);
				y += b; y -= f(x -= s);
				stemp.cpy(s.data());
				stemp.p_mxm(Jinv, y);
				stemp *= (static_cast<Ty>(1)) / welp::dot(Jinvtemp * y, s);
				s.adj();
				Jinv.pvxv(stemp, s * Jinvtemp);
				s.adj();
				Jinvtemp.cpy(Jinv.data());
				y = f(x) -= b;
				max_iter--;
			}
		}

		template <typename Ty, class function_f, class _Allocator> inline welp::matrix<Ty, _Allocator> broyden(
			const function_f& f, const welp::matrix<Ty, _Allocator>& Jx0inv, const welp::matrix<Ty, _Allocator>& x0,
			const welp::matrix<Ty, _Allocator>& b, int max_iter, Ty tol)
		{
			welp::matrix<Ty, _Allocator> x = x0;
			welp::matrix<Ty, _Allocator> Jinv = Jx0inv;
			welp::solve::broyden_update(f, Jinv, x, b, max_iter, tol);
			return x;
		}
	}

	namespace inv
	{
		template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> gauss(const welp::matrix<Ty, _Allocator>& A)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(A.r() == A.c());
#endif // WELP_MATRIX_DEBUG_MODE
			welp::matrix<Ty, _Allocator> X(A.r(), A.r());
			welp::matrix<Ty, _Allocator> U(A.r(), A.r() + A.r(), static_cast<Ty>(0));
			U.insert(0, 0, A);
			U.diag(static_cast<Ty>(1), 0, A.r());
			U.elim_gauss();
			return X.trisolve(U);
		}
		template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> householder(const welp::matrix<Ty, _Allocator>& A)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
			if (A.r() >= A.c())
			{
				welp::matrix<Ty, _Allocator> X(A.c(), A.r());
				welp::matrix<Ty, _Allocator> U(A.r(), A.r() + A.c(), static_cast<Ty>(0));
				U.insert(0, 0, A);
				U.diag(static_cast<Ty>(1), 0, A.c());
				U.elim_householder(A.c());
				return X.trisolve(U);
			}
			else
			{
				welp::matrix<Ty, _Allocator> X = welp::adj(A);
				welp::matrix<Ty, _Allocator> U(X.r(), X.r() + X.c(), static_cast<Ty>(0));
				U.insert(0, 0, X);
				U.diag(static_cast<Ty>(1), 0, X.c());
				U.elim_householder(X.c());
				X.resize(X.c(), X.r());
				return X.trisolve(U).adj();
			}
		}
		template <typename Ty, class _Allocator> welp::matrix<Ty, _Allocator> givens(const welp::matrix<Ty, _Allocator>& A)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
			if (A.r() >= A.c())
			{
				welp::matrix<Ty, _Allocator> X(A.c(), A.r());
				welp::matrix<Ty, _Allocator> U(A.r(), A.r() + A.c(), static_cast<Ty>(0));
				U.insert(0, 0, A);
				U.diag(static_cast<Ty>(1), 0, A.c());
				U.elim_givens(A.c());
				return X.trisolve(U);
			}
			else
			{
				welp::matrix<Ty, _Allocator> X = welp::adj(A);
				welp::matrix<Ty, _Allocator> U(X.r(), X.r() + X.c(), static_cast<Ty>(0));
				U.insert(0, 0, X);
				U.diag(static_cast<Ty>(1), 0, X.c());
				U.elim_givens(X.c());
				X.resize(X.c(), X.r());
				return X.trisolve(U).adj();
			}
		}
	}
	template <typename Ty, class _Allocator> Ty det(const welp::matrix<Ty, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(A.r() == A.c());
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<Ty, _Allocator> B = A;
		B.elim_givens(A.r() - 1);
		Ty d = static_cast<Ty>(1);
		Ty* p = B.data();
		std::size_t N = A.c() + 1;
		for (std::size_t k = A.c(); k > 0; k--)
		{
			d *= (*p);
			p += N;
		}
		return d;
	}
	template <typename Ty, class _Allocator> Ty diagonal_ratio(const welp::matrix<Ty, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		return welp::matrix_subroutines::diagonal_ratio(A.data(), A.r(), A.c());
	}
}


////// optimization //////

namespace welp
{
	namespace opti
	{
		template <typename Ty, class function_obj, class function_grd, class _Allocator> void quasi_convex_update(
			const function_obj& obj, const function_grd& grad, welp::matrix<Ty, _Allocator>& x,
			Ty alpha, Ty beta, int max_iter, Ty tol, bool track_forward)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(x.data() != nullptr);
			assert(grad(x).c() == 1);
			assert(x.r() == grad(x).r());
			assert(x.c() == 1);
#endif // WELP_MATRIX_DEBUG_MODE
			welp::matrix<Ty, _Allocator> d = grad(x);
			Ty t; Ty objx;
			Ty betainv = (static_cast<Ty>(1)) / beta;
			Ty vel = welp::norm2(d);
			tol = tol * tol;

			while ((vel > tol) && (max_iter > static_cast<Ty>(0)))
			{
				t = -static_cast<Ty>(1);
				objx = obj(x);
				x.psxm(t, d);

				while (!(obj(x) < objx + alpha * t * vel))
				{
					x.psxm((beta - static_cast<Ty>(1)) * t, d);
					t *= beta; // backtrack
				}

				if (track_forward && (!(t > -static_cast<Ty>(1))))
				{
					x.psxm((betainv - static_cast<Ty>(1)) * t, d);
					t *= betainv; // track forward
					while (obj(x) < objx + alpha * t * vel)
					{
						x.psxm((betainv - static_cast<Ty>(1)) * t, d);
						t *= betainv; // track forward
					}
					x.psxm((beta - static_cast<Ty>(1)) * t, d);
				}

				d = grad(x);
				vel = welp::norm2(d);
				max_iter--;
			}
		}

		template <typename Ty, class function_obj, class function_grd, class _Allocator> inline welp::matrix<Ty, _Allocator> quasi_convex(
			const function_obj& obj, const function_grd& grad, const welp::matrix<Ty, _Allocator>& x0,
			Ty alpha, Ty beta, int max_iter, Ty tol, bool track_forward)
		{
			welp::matrix<Ty, _Allocator> x = x0;
			welp::opti::quasi_convex_update(obj, grad, x, alpha, beta, max_iter, tol, track_forward);
			return x;
		}

		template <typename Ty, class function_obj, class function_grd, class _Allocator> void strict_convex_update(
			const function_obj& obj, const function_grd& grad, welp::matrix<Ty, _Allocator>& x, welp::matrix<Ty, _Allocator>& Binv,
			Ty alpha, Ty beta, int max_iter, Ty tol)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(x.data() != nullptr);
			assert(grad(x).c() == 1);
			assert(x.r() == grd(x).r());
			assert(x.c() == 1);
			assert(Binv.r() == x.r());
			assert(Binv.c() == x.r());
#endif // WELP_MATRIX_DEBUG_MODE
			welp::matrix<Ty, _Allocator> d = grad(x);

			Ty t; Ty objx; Ty ysinv; welp::matrix<Ty, _Allocator> y;

			welp::matrix<Ty, _Allocator> Binvtemp = Binv;
			welp::matrix<Ty, _Allocator> s = Binv * d;
			Ty vel = welp::dot(s, d);
			tol = tol * tol;

			while ((vel > tol) && (max_iter > static_cast<Ty>(0)))
			{
				t = -static_cast<Ty>(1);
				objx = obj(x);
				y = grad(x);
				x.psxm(t, s);

				while (!(obj(x) < objx + alpha * t * vel))
				{
					x.psxm((beta - static_cast<Ty>(1)) * t, s);
					t *= beta; // backtrack
				}

				y -= grad(x);
				s *= (-t);
				ysinv = (static_cast<Ty>(1)) / welp::dot(y, s);
				Binv.pvxv((ysinv + ysinv * ysinv * welp::dot(Binvtemp * y, y)) * s, s);
				s *= ysinv;
				Binv.p_vxv(Binvtemp * y, s);
				y.adj();
				Binv.p_vxv(s, y * Binvtemp);
				y.adj();
				Binvtemp.cpy(Binv.data());
				d = grad(x);
				s.fill(static_cast<Ty>(0)).pmxm(Binv, d);
				vel = welp::dot(s, d);
				max_iter--;
			}
		}

		template <typename Ty, class function_obj, class function_grd, class _Allocator> inline welp::matrix<Ty, _Allocator> strict_convex(
			const function_obj& obj, const function_grd& grad, const welp::matrix<Ty, _Allocator>& x0, const welp::matrix<Ty, _Allocator>& Bx0inv,
			Ty alpha, Ty beta, int max_iter, Ty tol)
		{
			welp::matrix<Ty, _Allocator> x = x0;
			welp::matrix<Ty, _Allocator> Binv = Bx0inv;
			welp::opti::strict_convex_update(obj, grad, x, Binv, alpha, beta, max_iter, tol);
			return x;
		}
	}
}


////// gradient and jacobian //////

namespace welp
{
	template <typename Ty, class function_f, class _Allocator> inline welp::matrix<Ty, _Allocator> gradient(
		function_f f, const welp::matrix<Ty, _Allocator>& x, Ty h)
	{
		welp::matrix<Ty, _Allocator> gradfx(x.r(), 1);
		welp::matrix<Ty, _Allocator> xd = x;
		Ty hinvd2 = static_cast<Ty>(0.5f) / h;
		Ty temp; Ty grad_plus; Ty grad_minus;
		Ty* pxd = xd.data(); Ty* pgradfx = gradfx.data();
		for (size_t i = xd.r(); i > 0; i--)
		{
			temp = *pxd;
			*pxd += h;
			grad_plus = f(xd);
			*pxd = temp - h;
			grad_minus = f(xd);
			*pgradfx++ = (grad_plus - grad_minus) * hinvd2;
			pxd++;
		}
		return gradfx;
	}

	template <typename Ty, class function_f, class _Allocator> inline welp::matrix<Ty, _Allocator> jacobian(
		function_f f, const welp::matrix<Ty, _Allocator>& x, Ty h)
	{
		welp::matrix<Ty, _Allocator> J(f(x).r(), x.r(), 0);
		welp::matrix<Ty, _Allocator> xd = x;
		welp::matrix<Ty, _Allocator> gradfi;
		Ty hinvd2 = static_cast<Ty>(0.5f) / h; Ty temp;
		Ty* pxd = xd.data();
		for (size_t i = 0; i < xd.r(); i++)
		{
			temp = *pxd;
			*pxd += h;
			gradfi = f(xd);
			*pxd = temp - h;
			gradfi -= f(xd);
			*pxd = temp;
			gradfi *= hinvd2;
			J.insert(0, i, gradfi);
			pxd++;
		}
		return J;
	}
}


////// element-wise and row-wise functional operations //////

namespace welp
{
	template <typename Ty2, class _return_Allocator, typename Ty1, class _Allocator>
	welp::matrix<Ty2, _return_Allocator> eval(Ty2(*f)(Ty1), const welp::matrix<Ty1, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<Ty2, _return_Allocator> C(A.r(), A.c());
		const Ty1* pA = A.data(); Ty2* pC = C.data();
		for (std::size_t k = A.size(); k > 0; k--) { *pC++ = f(*pA++); }
		return C;
	}

	template <typename Ty2, class _return_Allocator, typename Ty1, class _Allocator>
	welp::matrix<Ty2, _return_Allocator> eval(Ty2(*f)(welp::matrix<Ty1, _Allocator>), const welp::matrix<Ty1, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<Ty2, _return_Allocator> C(A.r(), 1);
		std::size_t Ac = A.c();
		welp::matrix<Ty1> v(1, Ac);
		const Ty1* pA = A.data();
		Ty2* pC = C.data();
		Ty1* pv = v.data();

		for (std::size_t i = A.r(); i > 0; i--)
		{
			welp::matrix_subroutines::cpy(pv, pA, Ac);
			*pC++ = f(v);
			pA += Ac;
		}
		return C;
	}

	template <typename Ty2, class _return_Allocator, typename Ty1, class _Allocator, class _temp_Allocator> welp::matrix<Ty2, _return_Allocator> eval(
		welp::matrix<Ty2, _temp_Allocator>(*f)(Ty1), const welp::matrix<Ty1, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty1* pA = A.data();
		welp::matrix<Ty2, _return_Allocator> C = f(*pA++);
		std::size_t Cc = C.size();
		std::size_t Asize = A.size();
		C.resize(Asize, Cc);
		Ty1* pC = C.data() + Cc;

		for (std::size_t i = 1; i < Asize; i++)
		{
			welp::matrix_subroutines::cpy(pC, (f(*pA++)).data(), Cc);
			pC += Cc;
		}
		return C;
	}

	template <typename Ty2, class _return_Allocator, typename Ty1, class _Allocator, class _temp_Allocator> welp::matrix<Ty2, _return_Allocator> eval(
		welp::matrix<Ty2, _temp_Allocator>(*f)(welp::matrix<Ty1, _Allocator>), const welp::matrix<Ty1, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		std::size_t Ac = A.c();
		welp::matrix<Ty1, _Allocator> v(1, Ac);
		const Ty1* pA = A.data();
		Ty1* pv = v.data();
		welp::matrix_subroutines::cpy(pv, pA, Ac);
		pA += Ac;
		welp::matrix<Ty2, _return_Allocator> C = f(v);
		std::size_t Cc = C.size();
		std::size_t Ar = A.r();
		C.resize(Ar, Cc);
		Ty1* pC = C.data() + Cc;

		for (std::size_t i = 1; i < Ar; i++)
		{
			welp::matrix_subroutines::cpy(pv, pA, Ac);
			welp::matrix_subroutines::cpy(pC, (f(v)).data(), Cc);
			pA += Ac; pC += Cc;
		}
		return C;
	}

#ifdef WELP_MATRIX_INCLUDE_FUNCTIONAL
	template <typename Ty2, class _return_Allocator, typename Ty1, class _Allocator>
	welp::matrix<Ty2, _return_Allocator> eval(const std::function<Ty2(Ty1)>& f, const welp::matrix<Ty1, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<Ty2, _return_Allocator> C(A.r(), A.c());
		const Ty1* pA = A.data(); Ty2* pC = C.data();
		for (std::size_t k = A.size(); k > 0; k--) { *pC++ = f(*pA++); }
		return C;
	}

	template <typename Ty2, class _return_Allocator, typename Ty1, class _Allocator>
	welp::matrix<Ty2, _return_Allocator> eval(const std::function<Ty2(welp::matrix<Ty1, _Allocator>)>& f, const welp::matrix<Ty1, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<Ty2, _return_Allocator> C(A.r(), 1);
		std::size_t Ac = A.c();
		welp::matrix<Ty1> v(1, Ac);
		const Ty1* pA = A.data();
		Ty2* pC = C.data();
		Ty1* pv = v.data();

		for (std::size_t i = A.r(); i > 0; i--)
		{
			welp::matrix_subroutines::cpy(pv, pA, Ac);
			*pC++ = f(v);
			pA += Ac;
		}
		return C;
	}

	template <typename Ty2, class _return_Allocator, typename Ty1, class _Allocator, class _temp_Allocator> welp::matrix<Ty2, _return_Allocator> eval(
		const std::function<welp::matrix<Ty2, _temp_Allocator>(Ty1)>& f, const welp::matrix<Ty1, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		const Ty1* pA = A.data();
		welp::matrix<Ty2, _return_Allocator> C = f(*pA++);
		std::size_t Cc = C.size();
		std::size_t Asize = A.size();
		C.resize(Asize, Cc);
		Ty1* pC = C.data() + Cc;

		for (std::size_t i = 1; i < Asize; i++)
		{
			welp::matrix_subroutines::cpy(pC, (f(*pA++)).data(), Cc);
			pC += Cc;
		}
		return C;
	}

	template <typename Ty2, class _return_Allocator, typename Ty1, class _Allocator, class _temp_Allocator> welp::matrix<Ty2, _return_Allocator> eval(
		const std::function<welp::matrix<Ty2, _temp_Allocator>(welp::matrix<Ty1, _Allocator>)>& f, const welp::matrix<Ty1, _Allocator>& A)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		std::size_t Ac = A.c();
		welp::matrix<Ty1, _Allocator> v(1, Ac);
		const Ty1* pA = A.data();
		Ty1* pv = v.data();
		welp::matrix_subroutines::cpy(pv, pA, Ac);
		pA += Ac;
		welp::matrix<Ty2, _return_Allocator> C = f(v);
		std::size_t Cc = C.size();
		std::size_t Ar = A.r();
		C.resize(Ar, Cc);
		Ty1* pC = C.data() + Cc;

		for (std::size_t i = 1; i < Ar; i++)
		{
			welp::matrix_subroutines::cpy(pv, pA, Ac);
			welp::matrix_subroutines::cpy(pC, (f(v)).data(), Cc);
			pA += Ac; pC += Cc;
		}
		return C;
	}
#endif // WELP_MATRIX_INCLUDE_FUNCTIONAL

	namespace elem
	{
		template <typename Ty, class Predicate, class _Allocator> std::size_t find_first(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, std::size_t elem_begin, std::size_t elem_end)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(elem_begin <= elem_end);
			assert(elem_end <= A.size());
#endif // WELP_MATRIX_DEBUG_MODE
			const Ty* pA = A.data() + elem_begin;
			std::size_t cnt = elem_begin;

			for (std::size_t i = elem_end - elem_begin; i > 0; i--)
			{
				if (Pr(*pA++)) { return cnt; }
				cnt++;
			}
			return cnt;
		}

		template <typename Ty, class Predicate, class _Allocator> std::size_t find_first(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
			return welp::elem::find_first(Pr, A, 0, A.size());
		}

		template <typename Ty, class Predicate, class _Allocator> std::size_t find_last(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, std::size_t elem_begin, std::size_t elem_end)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(elem_begin <= elem_end);
			assert(elem_end <= A.size());
#endif // WELP_MATRIX_DEBUG_MODE
			const Ty* pA = A.data() + elem_end - 1;
			std::size_t cnt = elem_end;

			for (std::size_t i = elem_end - elem_begin; i > 0; i--)
			{
				cnt--;
				if (Pr(*pA--)) { return cnt; }
			}
			return A.size();
		}

		template <typename Ty, class Predicate, class _Allocator> std::size_t find_last(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
			return welp::elem::find_last(Pr, A, 0, A.size());
		}

		template <class _return_Allocator, typename Ty, class Predicate, class _Allocator>
		welp::matrix<std::size_t, _return_Allocator> find_all(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A,
			std::size_t elem_begin, std::size_t elem_end, bool shrink_to_fit)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(elem_begin <= elem_end);
			assert(elem_end <= A.size());
#endif // WELP_MATRIX_DEBUG_MODE
			std::size_t erange = elem_end - elem_begin;
			welp::matrix<std::size_t, _return_Allocator> C(erange, 1);
			std::size_t Cc = A.c();
			const Ty* pA = A.data() + elem_begin;
			std::size_t* pC = C.data();
			std::size_t Cr = 0;
			std::size_t cnt = elem_begin;

			for (std::size_t i = erange; i > 0; i--)
			{
				if (Pr(*pA++))
				{
					*pC++ = cnt;
					Cr++;
				}
				cnt++;
			}
			C.resize(Cr, 1);
			if (shrink_to_fit) { C.shrink_to_fit(); }
			return C;
		}

		template <class _return_Allocator, typename Ty, class Predicate, class _Allocator>
		welp::matrix<std::size_t, _return_Allocator> find_all(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, bool shrink_to_fit)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
			return welp::elem::find_all<_return_Allocator>(Pr, A, 0, A.size(), shrink_to_fit);
		}

		template <typename Ty, class Predicate, class _Allocator> welp::matrix<Ty, _Allocator> select(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, std::size_t elem_begin, std::size_t elem_end, bool shrink_to_fit)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(elem_begin <= elem_end);
			assert(elem_end <= A.size());
#endif // WELP_MATRIX_DEBUG_MODE
			std::size_t erange = elem_end - elem_begin;
			welp::matrix<Ty, _Allocator> C(erange, 1);
			const Ty* pA = A.data() + elem_begin;
			Ty* pC = C.data();
			std::size_t Cr = 0;

			for (std::size_t i = erange; i > 0; i--)
			{
				if (Pr(*pA))
				{
					*pC++ += *pA;
					Cr++;
				}
				pA++;
			}
			C.resize(Cr, 1);
			if (shrink_to_fit) { C.shrink_to_fit(); }
			return C;
		}

		template <typename Ty, class Predicate, class _Allocator> welp::matrix<Ty, _Allocator> select(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, bool shrink_to_fit)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
			return welp::elem::select(Pr, A, 0, A.size(), shrink_to_fit);
		}

		template <typename Ty, class Predicate, class _Allocator> std::size_t count(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, std::size_t elem_begin, std::size_t elem_end)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(elem_begin <= elem_end);
			assert(elem_end <= A.size());
#endif // WELP_MATRIX_DEBUG_MODE
			const Ty* pA = A.data() + elem_begin;
			std::size_t cnt = 0;

			for (std::size_t i = elem_end - elem_begin; i > 0; i--)
			{
				if (Pr(*pA++)) { cnt++; }
			}
			return cnt;
		}

		template <typename Ty, class Predicate, class _Allocator> std::size_t count(
			const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
			return welp::elem::count(Pr, A, 0, A.size());
		}
	}

	namespace row
	{
		template <typename Ty, class Predicate, class _Allocator>
		std::size_t find_first(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A,
			std::size_t row_begin, std::size_t row_end)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(row_begin <= row_end);
			assert(row_end <= A.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::size_t Ac = A.c();
			welp::matrix<Ty, _Allocator> v(1, Ac);
			const Ty* pA = A.data() + Ac * row_begin;
			std::size_t cnt = row_begin;

			for (std::size_t i = row_end - row_begin; i > 0; i--)
			{
				v.cpy(pA);
				pA += Ac;
				if (Pr(v)) { return cnt; }
				cnt++;
			}
			return cnt;
		}

		template <typename Ty, class Predicate, class _Allocator>
		std::size_t find_first(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
			return welp::row::find_first(Pr, A, 0, A.r());
		}

		template <typename Ty, class Predicate, class _Allocator>
		std::size_t find_last(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A,
			std::size_t row_begin, std::size_t row_end)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(row_begin <= row_end);
			assert(row_end <= A.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::size_t Ac = A.c();
			welp::matrix<Ty, _Allocator> v(1, Ac);
			const Ty* pA = A.data() + Ac * (row_end - 1);
			std::size_t cnt = row_end;

			for (std::size_t i = row_end - row_begin; i > 0; i--)
			{
				cnt--;
				v.cpy(pA);
				pA += Ac;
				if (Pr(v)) { return cnt; }
			}
			return A.r();
		}

		template <typename Ty, class Predicate, class _Allocator>
		std::size_t find_last(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
			return welp::row::find_last(Pr, A, 0, A.r());
		}

		template <class _return_Allocator, typename Ty, class Predicate, class _Allocator>
		welp::matrix<std::size_t, _return_Allocator> find_all(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A,
			std::size_t row_begin, std::size_t row_end, bool shrink_to_fit)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(row_begin <= row_end);
			assert(row_end <= A.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::size_t rrange = row_end - row_begin;
			welp::matrix<std::size_t, _return_Allocator> C(rrange, 1);
			std::size_t Cc = A.c();
			welp::matrix<Ty, _Allocator> v(1, Cc);
			const Ty* pA = A.data() + Cc * row_begin;
			std::size_t* pC = C.data();
			std::size_t Cr = 0;
			std::size_t cnt = row_begin;

			for (std::size_t i = rrange; i > 0; i--)
			{
				v.cpy(pA);
				pA += Cc;
				if (Pr(v))
				{
					*pC++ = cnt;
					Cr++;
				}
				cnt++;
			}
			C.resize(Cr, 1);
			if (shrink_to_fit) { C.shrink_to_fit(); }
			return C;
		}

		template <class _return_Allocator, typename Ty, class Predicate, class _Allocator>
		welp::matrix<std::size_t, _return_Allocator> find_all(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, bool shrink_to_fit)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
			return welp::row::find_all<_return_Allocator>(Pr, A, 0, A.r(), shrink_to_fit);
		}

		template <typename Ty, class Predicate, class _Allocator>
		welp::matrix<Ty, _Allocator> select(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A,
			std::size_t row_begin, std::size_t row_end, bool shrink_to_fit)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(row_begin <= row_end);
			assert(row_end <= A.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::size_t rrange = row_end - row_begin;
			welp::matrix<Ty, _Allocator> C(rrange, A.c());
			std::size_t Cc = A.c();
			welp::matrix<Ty, _Allocator> v(1, Cc);
			const Ty* pA = A.data() + Cc * row_begin;
			Ty* pC = C.data();
			std::size_t Cr = 0;

			for (std::size_t i = rrange; i > 0; i--)
			{
				v.cpy(pA);
				pA += Cc;
				if (Pr(v))
				{
					welp::matrix_subroutines::cpy(pC, v.data(), Cc);
					pC += Cc;
					Cr++;
				}
			}
			C.resize(Cr, Cc);
			if (shrink_to_fit) { C.shrink_to_fit(); }
			return C;
		}

		template <typename Ty, class Predicate, class _Allocator>
		welp::matrix<Ty, _Allocator> select(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A, bool shrink_to_fit)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
			return welp::row::select(Pr, A, 0, A.r(), shrink_to_fit);
		}

		template <typename Ty, class Predicate, class _Allocator>
		std::size_t count(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A,
			std::size_t row_begin, std::size_t row_end)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
			assert(row_begin <= row_end);
			assert(row_end <= A.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::size_t Ac = A.c();
			welp::matrix<Ty, _Allocator> v(1, Ac);
			const Ty* pA = A.data() + Ac * row_begin;
			std::size_t cnt = 0;

			for (std::size_t i = row_end - row_begin; i > 0; i--)
			{
				v.cpy(pA);
				pA += Ac;
				if (Pr(v)) { cnt++; }
			}
			return cnt;
		}

		template <typename Ty, class Predicate, class _Allocator>
		std::size_t count(const Predicate& Pr, const welp::matrix<Ty, _Allocator>& A)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
			return welp::row::count(Pr, A, 0, A.r());
		}
	}
}


////// textual matrices //////

#ifdef WELP_MATRIX_INCLUDE_STRING
namespace welp
{
	template <class _Allocator, class _string_Allocator>
	welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> parse(
		const welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator>& A,
		std::size_t rows_max, std::size_t rows_skipped, char delim)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(A.c() == 1);
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> C;
		std::ifstream loadmat;
		const char* pdata; const char* pbegin; const char* pend;

		const std::basic_string<char, std::char_traits<char>, _string_Allocator>* pA = A.data() + rows_skipped;

		if (rows_max > A.r() - rows_skipped) { rows_max = A.r() - rows_skipped; }

		C.resize(rows_max, static_cast<std::size_t>(std::count(pA->begin(), pA->end(), delim) + 1));
		std::basic_string<char, std::char_traits<char>, _string_Allocator>* pC = C.data();
		std::size_t Cr = 0;

		pdata = pA->data();
		std::size_t j = A.r() - rows_skipped;
		std::size_t k;

		while ((Cr < rows_max) && (j > 0))
		{
			if (std::count(pA->begin(), pA->end(), delim) + 1 - C.c() == 0)
			{
				pdata = pA->data();
				pbegin = pdata;
				pend = pdata;
				for (k = pA->size(); k > 0; k--)
				{
					if (*pend != delim) { pend++; }
					else
					{
						*pC++ = pA->substr(static_cast<std::size_t>(pbegin - pdata),
							static_cast<std::size_t>(pend - pbegin));
						pend++; pbegin = pend;
					}
				}
				*pC++ = pA->substr(static_cast<std::size_t>(pbegin - pdata),
					static_cast<std::size_t>(pend - pbegin));
				pbegin = pend;
				Cr++;
			}
			pA++;
			j--;
		}
		loadmat.close();
		C.resize(Cr, C.c());
		C.shrink_to_fit();
		return C;
	}

	template <typename Ty, class _return_Allocator, class _Allocator, class _string_Allocator, class function_f>
	welp::matrix<Ty, _return_Allocator> parse(
		const welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator>& A,
		std::size_t rows_max, std::size_t rows_skipped, char delim, function_f f)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
		assert(A.c() == 1);
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<Ty, _return_Allocator> C;
		std::ifstream loadmat;
		const char* pdata; const char* pbegin; const char* pend;

		const std::basic_string<char, std::char_traits<char>, _string_Allocator>* pA = A.data() + rows_skipped;

		if (rows_max > A.r() - rows_skipped) { rows_max = A.r() - rows_skipped; }

		C.resize(rows_max, static_cast<std::size_t>(std::count(pA->begin(), pA->end(), delim) + 1));
		Ty* pC = C.data();
		std::size_t Cr = 0;

		pdata = pA->data();
		std::size_t j = A.r() - rows_skipped;
		std::size_t k;

		while ((Cr < rows_max) && (j > 0))
		{
			if (std::count(pA->begin(), pA->end(), delim) + 1 - C.c() == 0)
			{
				pdata = pA->data();
				pbegin = pdata;
				pend = pdata;
				for (k = pA->size(); k > 0; k--)
				{
					if (*pend != delim) { pend++; }
					else
					{
						*pC++ = f(pA->substr(static_cast<std::size_t>(pbegin - pdata),
							static_cast<std::size_t>(pend - pbegin)));
						pend++; pbegin = pend;
					}
				}
				*pC++ = f(pA->substr(static_cast<std::size_t>(pbegin - pdata),
					static_cast<std::size_t>(pend - pbegin)));
				pbegin = pend;
				Cr++;
			}
			pA++;
			j--;
		}
		loadmat.close();
		C.resize(Cr, C.c());
		C.shrink_to_fit();
		return C;
	}

	template <typename Ty, class _Allocator, class _string_Allocator>
	welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> concatenate(
		const welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator>& A, Ty delim)
	{
#ifdef WELP_MATRIX_DEBUG_MODE
		assert(A.data() != nullptr);
#endif // WELP_MATRIX_DEBUG_MODE
		welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> C(A.r(), 1, "");
		const std::basic_string<char, std::char_traits<char>, _string_Allocator>* pA = A.data();
		std::basic_string<char, std::char_traits<char>, _string_Allocator>* pC = C.data();
		std::size_t Ac = A.c(); std::size_t strsize;
		std::size_t j;

		for (std::size_t i = A.r(); i > 0; i--)
		{
			strsize = 0;
			for (j = Ac; j > 0; j--)
			{
				strsize += pA->size();
				pA++;
			}
			strsize += (Ac - 1);
			pC->reserve(strsize);

			pA -= Ac;

			for (j = Ac - 1; j > 0; j--)
			{
				*pC += *pA++;
				*pC += delim;
			}
			*pC += *pA++;
			pC++;
		}
		return C;
	}

	template <typename Ty, std::size_t length, class _Allocator, class _string_Allocator>
	welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> concatenate(
		const welp::matrix_stream<length, std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> S, Ty delim)
	{
		std::size_t Cr = 0;
		for (std::size_t n = 0; n < S.count; n++)
		{
			Cr += S[n]->r();
		}

		welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> C(Cr, 1, "");
		std::basic_string<char, std::char_traits<char>, _string_Allocator>* pC = C.data();
		std::size_t j;

		for (std::size_t n = 0; n < S.count; n++)
		{
			const std::basic_string<char, std::char_traits<char>, _string_Allocator>* pA = S[n]->data();
			std::size_t Ac = S[n]->c(); std::size_t strsize;

			for (std::size_t i = S[n]->r(); i > 0; i--)
			{
				strsize = 0;
				for (j = Ac; j > 0; j--)
				{
					strsize += pA->size();
					pA++;
				}
				strsize += (Ac - 1);
				pC->reserve(strsize);

				pA -= Ac;

				for (j = Ac - 1; j > 0; j--)
				{
					*pC += *pA++;
					*pC += delim;
				}
				*pC += *pA++;
				pC++;
			}
		}
		return C;
	}

#ifdef WELP_MATRIX_INCLUDE_FSTREAM
	namespace file
	{
		template <class _string_Allocator, class _Allocator>
		welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> load(
			const std::string& filename, std::size_t rows_max, std::size_t rows_skipped)
		{
			welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> C(rows_max, 1);
			std::ifstream loadmat;
			loadmat.open(filename);
			loadmat.peek();

			if (loadmat.good())
			{
				std::basic_string<char, std::char_traits<char>, _string_Allocator> line;
				std::basic_string<char, std::char_traits<char>, _string_Allocator>* pC = C.data();
				for (std::size_t k = rows_skipped; k > 0; k--)
				{
					std::getline(loadmat, line);
				}
				std::size_t Cr = 0;
				while ((Cr < rows_max) && loadmat.good())
				{
					std::getline(loadmat, line);
					*pC++ = std::move(line);
					Cr++;
				}
				C.resize(Cr, 1);
			}
			else { C.clear(); }
			loadmat.close();
			return C;
		}

#ifdef WELP_MATRIX_INCLUDE_ALGORITHM
		template <class _string_Allocator, class _Allocator>
		welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> load(
			const std::string& filename, std::size_t rows_max, std::size_t rows_skipped, char delim)
		{
			welp::matrix<std::basic_string<char, std::char_traits<char>, _string_Allocator>, _Allocator> C;
			std::ifstream loadmat;
			loadmat.open(filename);
			loadmat.peek();
			const char* pdata; const char* pbegin; const char* pend;

			if (loadmat.good())
			{
				std::basic_string<char, std::char_traits<char>, _string_Allocator> line;
				std::size_t k;
				for (k = rows_skipped; k > 0; k--)
				{
					std::getline(loadmat, line);
				}

				std::getline(loadmat, line);
				C.resize(rows_max, static_cast<std::size_t>(std::count(line.begin(), line.end(), delim) + 1));
				std::basic_string<char, std::char_traits<char>, _string_Allocator>* pC = C.data();
				std::size_t Cr = 0;

				pdata = line.data();
				pbegin = pdata;
				pend = pdata;
				for (k = line.size(); k > 0; k--)
				{
					if (*pend != delim) { pend++; }
					else
					{
						*pC++ = line.substr(static_cast<std::size_t>(pbegin - pdata),
							static_cast<std::size_t>(pend - pbegin));
						pend++; pbegin = pend;
					}
				}
				*pC++ = line.substr(static_cast<std::size_t>(pbegin - pdata),
					static_cast<std::size_t>(pend - pbegin));
				pbegin = pend;
				Cr++;

				while ((Cr < rows_max) && loadmat.good())
				{
					std::getline(loadmat, line);
					if (std::count(line.begin(), line.end(), delim) + 1 - C.c() == 0)
					{
						pdata = line.data();
						pbegin = pdata;
						pend = pdata;
						for (k = line.size(); k > 0; k--)
						{
							if (*pend != delim) { pend++; }
							else
							{
								*pC++ = line.substr(static_cast<std::size_t>(pbegin - pdata),
									static_cast<std::size_t>(pend - pbegin));
								pend++; pbegin = pend;
							}
						}
						*pC++ = line.substr(static_cast<std::size_t>(pbegin - pdata),
							static_cast<std::size_t>(pend - pbegin));
						pbegin = pend;
						Cr++;
					}
				}
				C.resize(Cr, C.c());
			}
			else { C.clear(); }
			loadmat.close();
			return C;
		}

		template <typename Ty, class _string_Allocator, class _Allocator, class function_f>
		welp::matrix<Ty, _Allocator> load(const std::string& filename, std::size_t rows_max, std::size_t rows_skipped, char delim, function_f f)
		{
			welp::matrix<Ty, _Allocator> C;
			std::ifstream loadmat;
			loadmat.open(filename);
			loadmat.peek();
			const char* pdata; const char* pbegin; const char* pend;

			if (loadmat.good())
			{
				std::basic_string<char, std::char_traits<char>, _string_Allocator> line;
				std::size_t k;
				for (k = rows_skipped; k > 0; k--)
				{
					std::getline(loadmat, line);
				}

				std::getline(loadmat, line);
				C.resize(rows_max, static_cast<std::size_t>(std::count(line.begin(), line.end(), delim) + 1));
				Ty* pC = C.data();
				std::size_t Cr = 0;

				pdata = line.data();
				pbegin = pdata;
				pend = pdata;
				for (k = line.size(); k > 0; k--)
				{
					if (*pend != delim) { pend++; }
					else
					{
						*pC++ = f(line.substr(static_cast<std::size_t>(pbegin - pdata),
							static_cast<std::size_t>(pend - pbegin)));
						pend++; pbegin = pend;
					}
				}
				*pC++ = f(line.substr(static_cast<std::size_t>(pbegin - pdata),
					static_cast<std::size_t>(pend - pbegin)));
				pbegin = pend;
				Cr++;

				while ((Cr < rows_max) && loadmat.good())
				{
					std::getline(loadmat, line);
					if (std::count(line.begin(), line.end(), delim) + 1 - C.c() == 0)
					{
						pdata = line.data();
						pbegin = pdata;
						pend = pdata;
						for (k = line.size(); k > 0; k--)
						{
							if (*pend != delim) { pend++; }
							else
							{
								*pC++ = f(line.substr(static_cast<std::size_t>(pbegin - pdata),
									static_cast<std::size_t>(pend - pbegin)));
								pend++; pbegin = pend;
							}
						}
						*pC++ = f(line.substr(static_cast<std::size_t>(pbegin - pdata),
							static_cast<std::size_t>(pend - pbegin)));
						pbegin = pend;
						Cr++;
					}
				}
				C.resize(Cr, C.c());
			}
			else { C.clear(); }
			loadmat.close();
			return C;
		}
#endif // WELP_MATRIX_INCLUDE_ALGORITHM

		template <typename Ty1, typename Ty2, class _Allocator_A> void write(
			const std::string& filename, const welp::matrix<Ty1, _Allocator_A>& A, Ty2 delim)
		{
			std::ofstream writemat;
			writemat.open(filename);
			if (writemat.good())
			{
				std::size_t n = A.size();
				const Ty1* pA = A.data();
				std::size_t Ac = A.c();
				std::size_t k = 1;
				for (std::size_t j = 0; j < n; j++)
				{
					writemat << *pA++;
					if (k == Ac) { writemat << "\n"; k = 1; }
					else { writemat << delim; k++; }
				}
			}
			writemat.close();
		}

		template <typename Ty1, typename Ty2, typename Ty3, class _Allocator_A, class _Allocator_B> void write(
			const std::string& filename, const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B,
			const Ty3& delim)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.r() == B.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::ofstream writemat;
			writemat.open(filename);
			if (writemat.good())
			{
				const Ty1* pA = A.data();
				const Ty2* pB = B.data();
				std::size_t Ar = A.r();
				std::size_t Ac = A.c();
				std::size_t Bc = B.c();
				std::size_t j;
				std::size_t k = 1;
				for (std::size_t i = 0; i < Ar; i++)
				{
					for (j = Ac; j > 0; j--)
					{
						writemat << *pA++ << delim;
					}
					for (j = Bc; j > 0; j--)
					{
						writemat << *pB++;
						if (k == Bc) { writemat << "\n"; k = 1; }
						else { writemat << delim; k++; }
					}
				}
			}
			writemat.close();
		}

		template <typename Ty1, typename Ty2, typename Ty3, typename Ty4, class _Allocator_A, class _Allocator_B, class _Allocator_C>
		void write(const std::string& filename, const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B,
			const welp::matrix<Ty3, _Allocator_C>& C, const Ty4& delim)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.r() == B.r());
			assert(A.r() == C.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::ofstream writemat;
			writemat.open(filename);
			if (writemat.good())
			{
				const Ty1* pA = A.data();
				const Ty2* pB = B.data();
				const Ty3* pC = C.data();
				std::size_t Ar = A.r();
				std::size_t Ac = A.c();
				std::size_t Bc = B.c();
				std::size_t Cc = C.c();
				std::size_t j;
				std::size_t k = 1;
				for (std::size_t i = 0; i < Ar; i++)
				{
					for (j = Ac; j > 0; j--)
					{
						writemat << *pA++ << delim;
					}
					for (j = Bc; j > 0; j--)
					{
						writemat << *pB++ << delim;
					}
					for (j = Cc; j > 0; j--)
					{
						writemat << *pC++;
						if (k == Cc) { writemat << "\n"; k = 1; }
						else { writemat << delim; k++; }
					}
				}
			}
			writemat.close();
		}

		template <typename Ty1, typename Ty2, typename Ty3, typename Ty4, typename Ty5,
			class _Allocator_A, class _Allocator_B, class _Allocator_C, class _Allocator_D> void write(const std::string& filename,
				const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B,
				const welp::matrix<Ty3, _Allocator_C>& C, const welp::matrix<Ty4, _Allocator_D>& D, const Ty5& delim)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.r() == B.r());
			assert(A.r() == C.r());
			assert(A.r() == D.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::ofstream writemat;
			writemat.open(filename);
			if (writemat.good())
			{
				const Ty1* pA = A.data();
				const Ty2* pB = B.data();
				const Ty3* pC = C.data();
				const Ty4* pD = D.data();
				std::size_t Ar = A.r();
				std::size_t Ac = A.c();
				std::size_t Bc = B.c();
				std::size_t Cc = C.c();
				std::size_t Dc = D.c();
				std::size_t j;
				std::size_t k = 1;
				for (std::size_t i = 0; i < Ar; i++)
				{
					for (j = Ac; j > 0; j--)
					{
						writemat << *pA++ << delim;
					}
					for (j = Bc; j > 0; j--)
					{
						writemat << *pB++ << delim;
					}
					for (j = Cc; j > 0; j--)
					{
						writemat << *pC++ << delim;
					}
					for (j = Dc; j > 0; j--)
					{
						writemat << *pD++;
						if (k == Dc) { writemat << "\n"; k = 1; }
						else { writemat << delim; k++; }
					}
				}
			}
			writemat.close();
		}

		template <typename Ty1, typename Ty2, typename Ty3, typename Ty4, typename Ty5, typename Ty6,
			class _Allocator_A, class _Allocator_B, class _Allocator_C, class _Allocator_D, class _Allocator_E> void write(const std::string& filename,
				const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B, const welp::matrix<Ty3, _Allocator_C>& C,
				const welp::matrix<Ty4, _Allocator_D>& D, const welp::matrix<Ty5, _Allocator_E>& E, const Ty6& delim)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.r() == B.r());
			assert(A.r() == C.r());
			assert(A.r() == D.r());
			assert(A.r() == E.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::ofstream writemat;
			writemat.open(filename);
			if (writemat.good())
			{
				const Ty1* pA = A.data();
				const Ty2* pB = B.data();
				const Ty3* pC = C.data();
				const Ty4* pD = D.data();
				const Ty4* pE = E.data();
				std::size_t Ar = A.r();
				std::size_t Ac = A.c();
				std::size_t Bc = B.c();
				std::size_t Cc = C.c();
				std::size_t Dc = D.c();
				std::size_t Ec = E.c();
				std::size_t j;
				std::size_t k = 1;
				for (std::size_t i = 0; i < Ar; i++)
				{
					for (j = Ac; j > 0; j--)
					{
						writemat << *pA++ << delim;
					}
					for (j = Bc; j > 0; j--)
					{
						writemat << *pB++ << delim;
					}
					for (j = Cc; j > 0; j--)
					{
						writemat << *pC++ << delim;
					}
					for (j = Dc; j > 0; j--)
					{
						writemat << *pD++ << delim;
					}
					for (j = Ec; j > 0; j--)
					{
						writemat << *pE++;
						if (k == Ec) { writemat << "\n"; k = 1; }
						else { writemat << delim; k++; }
					}
				}
			}
			writemat.close();
		}

		template <typename Ty1, typename Ty2, class _Allocator_A> void append(
			const std::string& filename, const welp::matrix<Ty1, _Allocator_A>& A, const Ty2& delim)
		{
			std::ofstream writemat;
			writemat.open(filename, std::ios::app);
			if (writemat.good())
			{
				std::size_t n = A.size();
				const Ty1* pA = A.data();
				std::size_t Ac = A.c();
				std::size_t k = 1;
				for (std::size_t j = 0; j < n; j++)
				{
					writemat << *pA++;
					if (k == Ac) { writemat << "\n"; k = 1; }
					else { writemat << delim; k++; }
				}
			}
			writemat.close();
		}

		template <typename Ty1, typename Ty2, typename Ty3, class _Allocator_A, class _Allocator_B> void append(
			const std::string& filename, const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B,
			const Ty3& delim)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.r() == B.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::ofstream writemat;
			writemat.open(filename, std::ios::app);
			if (writemat.good())
			{
				const Ty1* pA = A.data();
				const Ty2* pB = B.data();
				std::size_t Ar = A.r();
				std::size_t Ac = A.c();
				std::size_t Bc = B.c();
				std::size_t j;
				std::size_t k = 1;
				for (std::size_t i = 0; i < Ar; i++)
				{
					for (j = Ac; j > 0; j--)
					{
						writemat << *pA++ << delim;
					}
					for (j = Bc; j > 0; j--)
					{
						writemat << *pB++;
						if (k == Bc) { writemat << "\n"; k = 1; }
						else { writemat << delim; k++; }
					}
				}
			}
			writemat.close();
		}

		template <typename Ty1, typename Ty2, typename Ty3, typename Ty4, class _Allocator_A, class _Allocator_B, class _Allocator_C>
		void append(const std::string& filename, const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B,
			const welp::matrix<Ty3, _Allocator_C>& C, const Ty4& delim)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.r() == B.r());
			assert(A.r() == C.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::ofstream writemat;
			writemat.open(filename, std::ios::app);
			if (writemat.good())
			{
				const Ty1* pA = A.data();
				const Ty2* pB = B.data();
				const Ty3* pC = C.data();
				std::size_t Ar = A.r();
				std::size_t Ac = A.c();
				std::size_t Bc = B.c();
				std::size_t Cc = C.c();
				std::size_t j;
				std::size_t k = 1;
				for (std::size_t i = 0; i < Ar; i++)
				{
					for (j = Ac; j > 0; j--)
					{
						writemat << *pA++ << delim;
					}
					for (j = Bc; j > 0; j--)
					{
						writemat << *pB++ << delim;
					}
					for (j = Cc; j > 0; j--)
					{
						writemat << *pC++;
						if (k == Cc) { writemat << "\n"; k = 1; }
						else { writemat << delim; k++; }
					}
				}
			}
			writemat.close();
		}

		template <typename Ty1, typename Ty2, typename Ty3, typename Ty4, typename Ty5,
			class _Allocator_A, class _Allocator_B, class _Allocator_C, class _Allocator_D> void append(const std::string& filename,
				const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B,
				const welp::matrix<Ty3, _Allocator_C>& C, const welp::matrix<Ty4, _Allocator_D>& D, const Ty5& delim)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.r() == B.r());
			assert(A.r() == C.r());
			assert(A.r() == D.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::ofstream writemat;
			writemat.open(filename, std::ios::app);
			if (writemat.good())
			{
				const Ty1* pA = A.data();
				const Ty2* pB = B.data();
				const Ty3* pC = C.data();
				const Ty4* pD = D.data();
				std::size_t Ar = A.r();
				std::size_t Ac = A.c();
				std::size_t Bc = B.c();
				std::size_t Cc = C.c();
				std::size_t Dc = D.c();
				std::size_t j;
				std::size_t k = 1;
				for (std::size_t i = 0; i < Ar; i++)
				{
					for (j = Ac; j > 0; j--)
					{
						writemat << *pA++ << delim;
					}
					for (j = Bc; j > 0; j--)
					{
						writemat << *pB++ << delim;
					}
					for (j = Cc; j > 0; j--)
					{
						writemat << *pC++ << delim;
					}
					for (j = Dc; j > 0; j--)
					{
						writemat << *pD++;
						if (k == Dc) { writemat << "\n"; k = 1; }
						else { writemat << delim; k++; }
					}
				}
			}
			writemat.close();
		}

		template <typename Ty1, typename Ty2, typename Ty3, typename Ty4, typename Ty5, typename Ty6,
			class _Allocator_A, class _Allocator_B, class _Allocator_C, class _Allocator_D, class _Allocator_E> void append(const std::string& filename,
				const welp::matrix<Ty1, _Allocator_A>& A, const welp::matrix<Ty2, _Allocator_B>& B, const welp::matrix<Ty3, _Allocator_C>& C,
				const welp::matrix<Ty4, _Allocator_D>& D, const welp::matrix<Ty4, _Allocator_E>& E, const Ty6& delim)
		{
#ifdef WELP_MATRIX_DEBUG_MODE
			assert(A.r() == B.r());
			assert(A.r() == C.r());
			assert(A.r() == D.r());
			assert(A.r() == E.r());
#endif // WELP_MATRIX_DEBUG_MODE
			std::ofstream writemat;
			writemat.open(filename, std::ios::app);
			if (writemat.good())
			{
				const Ty1* pA = A.data();
				const Ty2* pB = B.data();
				const Ty3* pC = C.data();
				const Ty4* pD = D.data();
				const Ty5* pE = E.data();
				std::size_t Ar = A.r();
				std::size_t Ac = A.c();
				std::size_t Bc = B.c();
				std::size_t Cc = C.c();
				std::size_t Dc = D.c();
				std::size_t Ec = E.c();
				std::size_t j;
				std::size_t k = 1;
				for (std::size_t i = 0; i < Ar; i++)
				{
					for (j = Ac; j > 0; j--)
					{
						writemat << *pA++ << delim;
					}
					for (j = Bc; j > 0; j--)
					{
						writemat << *pB++ << delim;
					}
					for (j = Cc; j > 0; j--)
					{
						writemat << *pC++ << delim;
					}
					for (j = Dc; j > 0; j--)
					{
						writemat << *pD++ << delim;
					}
					for (j = Ec; j > 0; j--)
					{
						writemat << *pE++;
						if (k == Ec) { writemat << "\n"; k = 1; }
						else { writemat << delim; k++; }
					}
				}
			}
			writemat.close();
		}

		std::string file_path(const std::string& str)
		{
			std::string::const_iterator p = str.end(); --p;
			while ((*p != '\\') && (*p != '/')) { --p; } ++p;
			return std::string(str.begin(), p);
		}
		std::string file_name(const std::string& str)
		{
			std::string::const_iterator p = str.end(); --p;
			while ((*p != '\\') && (*p != '/')) { --p; } ++p;
			return std::string(p, str.end());
		}
		std::string file_name_without_type(const std::string& str)
		{
			std::string::const_iterator p = str.end(); --p;
			while ((*p != '\\') && (*p != '/')) { --p; } ++p;
			std::string::const_iterator q = str.end(); --q;
			while (*q != '.') { --q; };
			return std::string(p, q);
		}
		std::string file_type(const std::string& str)
		{
			std::string::const_iterator p = str.end(); --p;
			while (*p != '.') { --p; } ++p;
			return std::string(p, str.end());
		}
	}
#endif // WELP_MATRIX_INCLUDE_FSTREAM
}
#endif // WELP_MATRIX_INCLUDE_STRING


#undef WELP_MATRIX_DEFAULT_ALLOCATOR
#undef WELP_MATRIX_DEFAULT_STREAM_LENGTH

#undef WELP_MATRIX_AVX_ps_mm_Ti
#undef WELP_MATRIX_AVX_ps_mm_Tj
#undef WELP_MATRIX_AVX_ps_mm_Tk

#undef WELP_MATRIX_AVX_pd_mm_Ti
#undef WELP_MATRIX_AVX_pd_mm_Tj
#undef WELP_MATRIX_AVX_pd_mm_Tk

#undef WELP_MATRIX_AVX_ps_elim_T
#undef WELP_MATRIX_AVX_pd_elim_T

#undef WELP_MATRIX_AVX_ps_trisolve_Ti
#undef WELP_MATRIX_AVX_pd_trisolve_Ti


#endif // WELP_MATRIX_HPP


// welp_matrix.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.

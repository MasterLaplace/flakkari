/**************************************************************************
 * Flakkari Library v0.3.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Packed.hpp
 * @brief Packed header. Contains PACKED macros.
 *        (LPL_PACKED_START, LPL_PACKED_END, PACKED)
 *
 * @details LPL_PACKED_START and LPL_PACKED_END macros are used to pack structs:
 *        PACKED_<_> macros are used to pack structs:
 *          LPL_PACKED_START struct _ {}; LPL_PACKED_END
 *        PACKED macros are used to pack structs:
 *          struct _ {} PACKED;
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.3.0
 * @date 2023-01-10
 **************************************************************************/

#ifndef PACKED_HPP_
#define PACKED_HPP_

#ifdef LPL_PACKED_START
#    undef LPL_PACKED_START
#endif

#ifdef LPL_PACKED_END
#    undef LPL_PACKED_END
#endif

#ifdef _MSC_VER
#    define LPL_PACKED_START __pragma(pack(push, 1))
#    define LPL_PACKED_END   __pragma(pack(pop))
#else
#    define LPL_PACKED_START _Pragma("pack(1)")
#    define LPL_PACKED_END   _Pragma("pack()")
#endif

#endif /* !PACKED_HPP_ */

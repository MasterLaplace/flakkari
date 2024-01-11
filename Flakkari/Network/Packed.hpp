/**************************************************************************
 * Flakkari Library v0.2.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Packed.hpp
 * @brief Packed header. Contains PACKED macros.
 *        (PACKED_START, PACKED_END, PACKED)
 *
 * @details PACKED_START and PACKED_END macros are used to pack structs:
 *        PACKED_<_> macros are used to pack structs:
 *          PACKED_START struct _ {}; PACKED_END
 *        PACKED macros are used to pack structs:
 *          struct _ {} PACKED;
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.2.0
 * @date 2023-01-10
 **************************************************************************/


#ifndef PACKED_HPP_
    #define PACKED_HPP_

#ifdef _MSC_VER
#define PACKED_START __pragma(pack(push, 1))
#define PACKED_END __pragma(pack(pop))
#else
#define PACKED_START _Pragma("pack(1)")
#define PACKED_END _Pragma("pack()")
#endif

#if __GNUC__
#define __PACKED __attribute__((packed))

#define PACKED(name, body)      \
do {                            \
    struct name body __PACKED;  \
} while (0)

#else

#define PACKED(name, body)  \
do {                        \
    PACKED_START            \
    struct name             \
    body;                   \
    PACKED_END              \
} while (0)

#endif

#endif /* !PACKED_HPP_ */

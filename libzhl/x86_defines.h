#pragma once

#include <type_traits>
#include <variant>

namespace x86 {
    enum class Registers8 {
        AL,
        CL,
        DL,
        BL,
        AH,
        CH,
        DH,
        BH
    };

    enum class Registers16 {
        AX,
        CX,
        DX,
        BX,
        SP,
        BP,
        SI,
        DI
    };

    enum class Registers32 {
        EAX,
        ECX,
        EDX,
        EBX,
        ESP,
        EBP,
        ESI,
        EDI
    };

    enum class SSERegisters64 {
        MM0,
        MM1,
        MM2,
        MM3,
        MM4,
        MM5,
        MM6,
        MM7
    };

    enum class SSERegisters128 {
        XMM0,
        XMM1,
        XMM2,
        XMM3,
        XMM4,
        XMM5,
        XMM6,
        XMM7
    };

    typedef std::variant<Registers8, Registers16, Registers32, SSERegisters64,
        SSERegisters128> x86Register;

    template<typename T, typename... Args>
    struct is_meta_member_of : public std::false_type {};

    template<typename T, typename... Args>
    struct is_meta_member_of<T, T, Args...> : public std::true_type {};

    template<typename T, typename U, typename... Args>
    struct is_meta_member_of<T, U, Args...> {
        constexpr static bool value = is_meta_member_of<T, Args...>::value;
    };

    template<typename T, typename... Args>
    constexpr bool is_member_of_v = is_meta_member_of<T, Args...>::value;

    template<typename T, typename... Args>
    constexpr bool is_member_of_v<T, std::variant<Args...>> = is_member_of_v<T, Args...>;

    static_assert(is_member_of_v<Registers8, x86Register>);
    static_assert(is_member_of_v<Registers16, x86Register>);
    static_assert(is_member_of_v<Registers32, x86Register>);
    static_assert(is_member_of_v<SSERegisters64, x86Register>);
    static_assert(is_member_of_v<SSERegisters128, x86Register>);
    static_assert(!is_member_of_v<float, x86Register>);

    template<typename T>
    uint8_t Register(T t) {
        static_assert(is_member_of_v<T, x86Register>);
        return static_cast<uint8_t>(t);
    }

    enum class Opcodes8 : uint8_t {
        PUSH_RM         = 0xFF, // /6
        CALL_NEAR_REL   = 0xE8,
        JMP_ABSOLUTE    = 0xE9,
        PUSH_IMM8       = 0x6A,
        PUSH_IMM32      = 0x68,
        LEA             = 0x8D,
        PUSH_REG        = 0x50, // +r
        MOV_REG8_TO_RM  = 0x88, // /r
        MOV_REG32_TO_RM = 0x89, // /r
        MOV_MEM8_TO_RM  = 0x8A, // /r
        MOV_MEM32_TO_RM = 0x8B, // /r
        MOV_IMM8        = 0xB0, // +r
        MOV_IMM32       = 0xB8, // +r
        MOV_IMM8_TO_RM  = 0xC6, // /0
        MOV_IMM32_TO_RM = 0xC7, // /0
    };

    enum class RMs : uint8_t {
        PUSH_RM         = 0x6,
        MOV_IMM8_TO_RM  = 0x0,
        MOV_IMM32_TO_RM = 0x0,
    };

    inline uint8_t Opcode8(Opcodes8 op) {
        return static_cast<uint8_t>(op);
    }

    inline uint8_t RM(RMs rm) {
        return static_cast<uint8_t>(rm);
    }
}
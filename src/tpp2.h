/* Copyright (c) 2017-2026 Griefer@Work                                       *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgement (see the following) in the product     *
 *    documentation is required:                                              *
 *    Portions Copyright (c) 2017-2026 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifdef TPP_DEFS

/* Emulate TPP2 user-defs */

#define KWD(id, string)                     TPP_KWD(id, string)
#define KWD_FLAGS(id, flags)                TPP_KWD_FLAGS(id, flags)
#define WGROUP(wgroup_id, names, default)   TPP_WGROUP(wgroup_id, names, default)
#ifdef tpp_va_arg
#define WARNING(name, groups, default)      /* nothing */
#define WARNING_MESSAGE(name, expr)         TPP_WARNING_EX(name, ~, ~, ~, format)
#else /* tpp_va_arg */
#define WARNING(name, groups, default)      TPP_WARNING_EX(name, TPP2_VA_NARGS(groups) groups, 1(name), default, ~)
#define WARNING_MESSAGE(name, expr)         /* nothing */
#endif /* !tpp_va_arg */
#define WARNING_NAMESPACE(name, start)      /* nothing (sorry; these no longer exist in TPP3; use warning numbers instead!) */
#define MACRO(keyword_id, if)               TPP_MACRO(keyword_id, if)
#define BUILTIN_MACRO(keyword_id, value)    TPP_BUILTIN_MACRO(keyword_id, value)
#define RT_BUILTIN_MACRO(keyword_id, value) TPP_BUILTIN_MACRO(keyword_id, value)
#define BUILTIN_FUNCTION(name, argc, expr)  /* nothing (sorry; these no longer exist in TPP3) */
#define EXTENSION(id, name, default)        TPP_EXTENSION(id, name, default)


#define DEF_K(name)          KWD(KWD_##name, #name)
#define DEF_M(name)          KWD(KWD_##name, #name) MACRO(KWD_##name, 1)
#define DEF_M_IF(name, expr) KWD(KWD_##name, #name) MACRO(KWD_##name, expr)
#define DEF_WARNING(name, groups, default, expr) \
	WARNING(name, groups, default)               \
	WARNING_MESSAGE(name, expr)

#define HAS_BUILTIN_IF(name, if)   KWD(KWD_##name, #name) KWD_FLAGS(KWD_##name, (if) ? TPP_KEYWORD_FLAG_HAS_BUILTIN : 0u)
#define HAS_FEATURE_IF(name, if)   KWD(KWD_##name, #name) KWD_FLAGS(KWD_##name, (if) ? TPP_KEYWORD_FLAG_HAS_FEATURE : 0u)
#define HAS_EXTENSION_IF(name, if) KWD(KWD_##name, #name) KWD_FLAGS(KWD_##name, (if) ? TPP_KEYWORD_FLAG_HAS_EXTENSION : 0u)

#define PREDEFINED_KWDMACRO_IF(name, str, if, value)                 KWD(name, str) MACRO(name, if) BUILTIN_MACRO(name, value)
#define PREDEFINED_KWDMACRO(name, str, value)                        KWD(name, str) MACRO(name, 1) BUILTIN_MACRO(name, value)
#define PREDEFINED_MACRO_IF(name, if, value)                         PREDEFINED_KWDMACRO_IF(KWD_##name, #name, if, value)
#define PREDEFINED_MACRO(name, value)                                PREDEFINED_KWDMACRO(KWD_##name, #name, value)
#define PREDEFINED_RT_KWDMACRO_IF(name, str, if, value)              KWD(name, str) MACRO(name, if) RT_BUILTIN_MACRO(name, value)
#define PREDEFINED_RT_KWDMACRO(name, str, value)                     KWD(name, str) MACRO(name, 1) RT_BUILTIN_MACRO(name, value)
#define PREDEFINED_RT_MACRO_IF(name, if, value)                      PREDEFINED_RT_KWDMACRO_IF(KWD_##name, #name, if, value)
#define PREDEFINED_RT_MACRO(name, value)                             PREDEFINED_RT_KWDMACRO(KWD_##name, #name, value)
#define PREDEFINED_KWDFUNCTION_IF(name, str, if, argc, expr)         /* nothing (no longer supported) */
#define PREDEFINED_KWDFUNCTION(name, str, argc, expr)                /* nothing (no longer supported) */
#define PREDEFINED_FUNCTION_IF(name, if, argc, expr)                 /* nothing (no longer supported) */
#define PREDEFINED_FUNCTION(name, argc, expr)                        /* nothing (no longer supported) */
#define PREDEFINED_BUILTIN_KWDFUNCTION_IF(name, str, if, argc, expr) /* nothing (no longer supported) */
#define PREDEFINED_BUILTIN_KWDFUNCTION(name, str, argc, expr)        /* nothing (no longer supported) */
#define PREDEFINED_BUILTIN_FUNCTION_IF(name, if, argc, expr)         /* nothing (no longer supported) */
#define PREDEFINED_BUILTIN_FUNCTION(name, argc, expr)                /* nothing (no longer supported) */


/* Misc utility macros... */
#define token      (*tpp_lexer_gettoken(tpp_current_lexer()))
#define tok        tpp_lexer_gettok(tpp_current_lexer())
#define yield      tpp_lexer_yield_blocking(tpp_current_lexer())
#define WARN(...)  tpp_lexer_warnf(tpp_current_lexer(), __VA_ARGS__)
#define HAS(ext)   tpp_lexer_getextension(tpp_current_lexer(), ext)
#define CURRENT    (*tpp_current_lexer())
#undef TPPLexer_Current
#define TPPLexer_Current tpp_current_lexer()
#ifdef tpp_current_va_arg
#define Q(x)       "%[" x "%]"
#define WARNF(...) tpp_do(tpp_lexer_printf_warning(tpp_current_lexer(), file, pos, pos_lcinfo, printer, printer_arg, __VA_ARGS__))
#define TOK_S      "%[%.*s%]" /* Use "%Pt" instead! */
#define TOK_A      (unsigned int)tpp_lexer_gettokenlen(tpp_current_lexer()), tpp_lexer_gettokenstart(tpp_current_lexer())
#define KWDNAME()  tpp_keyword_getkwdcstr(tpp_lexer_gettokenkwd(tpp_current_lexer()))
#define FILENAME() tpp_file_getfilename(ARG(tpp_file *))
#define ARG(T)     tpp_current_va_arg(T)
#endif /* tpp_current_va_arg */


/************************************************************************/
/************************************************************************/
/************************************************************************/
/*                                                                      */
/* Predefined CPU/Platform macros (was a dumb idea since mirrors host)  */
/*                                                                      */
/************************************************************************/
/************************************************************************/
/************************************************************************/
#if !TPP_CONFIG_MINMACRO
#if TPP_CONFIG_MINGCCFUNC < 2
EXTENSION(EXT_CPU_MACROS, "define-cpu-macros", 1)
EXTENSION(EXT_SYSTEM_MACROS, "define-system-macros", 1)
EXTENSION(EXT_UTILITY_MACROS, "define-utility-macros", 1)
#endif /* TPP_CONFIG_MINGCCFUNC < 2 */

#define TPP_PRIVATE_PP_CAT2(a, b)    a##b
#define TPP_PRIVATE_PP_CAT3(a, b, c) a##b##c
#define TPP_PP_CAT2(a, b)            TPP_PRIVATE_PP_CAT2(a, b)
#define TPP_PP_CAT3(a, b, c)         TPP_PRIVATE_PP_CAT3(a, b, c)

/* CPU-specific predefined macros. */
#if defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
PREDEFINED_MACRO_IF(__alpha__, HAS(EXT_CPU_MACROS), "1")
#endif

#if (defined(__amd64__) || defined(__amd64) ||   \
     defined(__x86_64__) || defined(__x86_64) || \
     defined(_M_X64) || defined(_M_AMD64))
PREDEFINED_MACRO_IF(__x86_64__, HAS(EXT_CPU_MACROS), "1")
#endif

#if defined(__TARGET_ARCH_ARM)
#define TPP_ARM_VERSION __TARGET_ARCH_ARM
#elif defined(_M_ARM)
#define TPP_ARM_VERSION _M_ARM
#elif defined(__ARM_ARCH_8__)
#define TPP_ARM_VERSION 8
#elif (defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) ||  \
       defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || \
       defined(__ARM_ARCH_7S__))
#define TPP_ARM_VERSION 7
#elif (defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) ||  \
       defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || \
       defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__))
#define TPP_ARM_VERSION 6
#elif (defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5E__) ||   \
       defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5TE__) || \
       defined(__ARM_ARCH_5TEJ__))
#define TPP_ARM_VERSION 5
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
#define TPP_ARM_VERSION 4
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
#define TPP_ARM_VERSION 3
#elif defined(__ARM_ARCH_2__)
#define TPP_ARM_VERSION 2
#elif defined(__arm__) || defined(__arm) || defined(_ARM) || defined(__ARM_ARCH_1__)
#define TPP_ARM_VERSION 1
#endif
#ifdef TPP_ARM_VERSION
PREDEFINED_MACRO_IF(__arm__, HAS(EXT_CPU_MACROS), TPP_PP_STR(TPP_ARM_VERSION))
#if TPP_ARM_VERSION == 8
PREDEFINED_MACRO_IF(__ARM_ARCH_8__, HAS(EXT_CPU_MACROS), "1")
#elif TPP_ARM_VERSION == 7
PREDEFINED_MACRO_IF(__ARM_ARCH_7__, HAS(EXT_CPU_MACROS), "1")
#elif TPP_ARM_VERSION == 6
#ifdef __ARM_ARCH_6T2__
PREDEFINED_MACRO_IF(__ARM_ARCH_6T2__, HAS(EXT_CPU_MACROS), "1")
#else
PREDEFINED_MACRO_IF(__ARM_ARCH_6__, HAS(EXT_CPU_MACROS), "1")
#endif
#elif TPP_ARM_VERSION == 5
#if defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__)
PREDEFINED_MACRO_IF(__ARM_ARCH_5T__, HAS(EXT_CPU_MACROS), "1")
#else
PREDEFINED_MACRO_IF(__ARM_ARCH_5__, HAS(EXT_CPU_MACROS), "1")
#endif
#elif TPP_ARM_VERSION == 4
PREDEFINED_MACRO_IF(__ARM_ARCH_4T__, HAS(EXT_CPU_MACROS), "1")
#elif TPP_ARM_VERSION == 3
PREDEFINED_MACRO_IF(__ARM_ARCH_3__, HAS(EXT_CPU_MACROS), "1")
#elif TPP_ARM_VERSION == 2
PREDEFINED_MACRO_IF(__ARM_ARCH_2__, HAS(EXT_CPU_MACROS), "1")
#else
PREDEFINED_MACRO_IF(__ARM_ARCH_1__, HAS(EXT_CPU_MACROS), "1")
#endif
#if defined(__TARGET_ARCH_THUMB)
PREDEFINED_MACRO_IF(__thumb__, HAS(EXT_CPU_MACROS), TPP_PP_STR(__TARGET_ARCH_THUMB))
#elif defined(_M_ARMT)
PREDEFINED_MACRO_IF(__thumb__, HAS(EXT_CPU_MACROS), TPP_PP_STR(_M_ARM))
#elif defined(__thumb__)
PREDEFINED_MACRO_IF(__thumb__, HAS(EXT_CPU_MACROS), TPP_PP_STR(TPP_ARM_VERSION))
#endif
#undef TPP_ARM_VERSION
#endif

#ifdef __aarch64__
PREDEFINED_MACRO_IF(__aarch64__, HAS(EXT_CPU_MACROS), "1")
#endif /* __aarch64__ */

#if defined(__bfin) || defined(__BFIN__)
PREDEFINED_MACRO_IF(__bfin, HAS(EXT_CPU_MACROS), "1")
#endif /* __bfin || __BFIN__ */

#ifdef __C67__
PREDEFINED_MACRO_IF(__C67__, HAS(EXT_CPU_MACROS), "1")
#endif /* __C67__ */

#ifdef __convex__
PREDEFINED_MACRO_IF(__convex__, HAS(EXT_CPU_MACROS), "1")
#ifdef __convex_c1__
PREDEFINED_MACRO_IF(__convex_c1__, HAS(EXT_CPU_MACROS), "1")
#endif /* __convex_c1__ */
#ifdef __convex_c2__
PREDEFINED_MACRO_IF(__convex_c2__, HAS(EXT_CPU_MACROS), "1")
#endif /* __convex_c2__ */
#ifdef __convex_c32__
PREDEFINED_MACRO_IF(__convex_c32__, HAS(EXT_CPU_MACROS), "1")
#endif /* __convex_c32__ */
#ifdef __convex_c34__
PREDEFINED_MACRO_IF(__convex_c34__, HAS(EXT_CPU_MACROS), "1")
#endif /* __convex_c34__ */
#ifdef __convex_c38__
PREDEFINED_MACRO_IF(__convex_c38__, HAS(EXT_CPU_MACROS), "1")
#endif /* __convex_c38__ */
#endif /* __convex__ */

#ifdef __epiphany__
PREDEFINED_MACRO_IF(__epiphany__, HAS(EXT_CPU_MACROS), "1")
#endif /* __epiphany__ */

#if defined(__hppa__) || defined(__HPPA__) || defined(__hppa)
PREDEFINED_MACRO_IF(__hppa__, HAS(EXT_CPU_MACROS), "1")
#endif /* __hppa__ || __HPPA__ || __hppa */

#if defined(__I86__)
#define TPP_I386_VERSION (__I86__ * 100)
#elif defined(_M_IX86)
#define TPP_I386_VERSION _M_IX86
#elif defined(__i686__) || defined(__i686) || defined(i686)
#define TPP_I386_VERSION 600
#elif defined(__i586__) || defined(__i586) || defined(i586)
#define TPP_I386_VERSION 500
#elif defined(__i486__) || defined(__i486) || defined(i486)
#define TPP_I386_VERSION 400
#elif (defined(__i386__) || defined(__i386) || defined(i386) || \
       defined(__X86__) || defined(_X86_) ||                    \
       defined(__THW_INTEL__) || defined(__INTEL__))
#define TPP_I386_VERSION 300
#endif
#ifdef TPP_I386_VERSION
#if TPP_I386_VERSION >= 600
PREDEFINED_MACRO_IF(__i686__, HAS(EXT_CPU_MACROS), "1")
#endif
#if TPP_I386_VERSION >= 500
PREDEFINED_MACRO_IF(__i586__, HAS(EXT_CPU_MACROS), "1")
#endif
#if TPP_I386_VERSION >= 400
PREDEFINED_MACRO_IF(__i486__, HAS(EXT_CPU_MACROS), "1")
#endif
#if TPP_I386_VERSION >= 300
PREDEFINED_MACRO_IF(__i386__, HAS(EXT_CPU_MACROS), "1")
#endif
#undef TPP_I386_VERSION
#endif /* TPP_I386_VERSION */

#if (defined(__ia64__) || defined(_IA64) || defined(__IA64__) || \
     defined(__ia64) || defined(_M_IA64) || defined(__itanium__))
PREDEFINED_MACRO_IF(__ia64__, HAS(EXT_CPU_MACROS), "1")
#endif /* __ia64__ || _IA64 || __IA64__ || __ia64 || _M_IA64 || __itanium__ */

#if defined(__mc68060__) || defined(__mc68060) || defined(mc68060)
#define TPP_M64K_VERSION 68060
#elif defined(__mc68040__) || defined(__mc68040) || defined(mc68040)
#define TPP_M64K_VERSION 68040
#elif defined(__mc68030__) || defined(__mc68030) || defined(mc68030)
#define TPP_M64K_VERSION 68030
#elif defined(__mc68020__) || defined(__mc68020) || defined(mc68020)
#define TPP_M64K_VERSION 68020
#elif defined(__mc68010__) || defined(__mc68010) || defined(mc68010)
#define TPP_M64K_VERSION 68010
#elif (defined(__mc68000__) || defined(__mc68000) || defined(mc68000) || \
       defined(__MC68000__) || defined(M68000) || defined(__MC68K__))
#define TPP_M64K_VERSION 68000
#endif
#ifdef TPP_M64K_VERSION
PREDEFINED_MACRO_IF(__m68k__, HAS(EXT_CPU_MACROS), TPP_PP_STR(TPP_M64K_VERSION))
PREDEFINED_KWD_MACRO_IF(TPP_PP_CAT3(KWD___mc, TPP_M64K_VERSION, __),
                        "__mc" TPP_PP_STR(TPP_M64K_VERSION) "__",
                        HAS(EXT_CPU_MACROS), "1")
#undef TPP_M64K_VERSION
#endif /* TPP_M64K_VERSION */

#if defined(__mips)
#define TPP_MIPS_VERSION __mips
#elif defined(__MIPS_ISA4__)
#define TPP_MIPS_VERSION 4
#elif defined(__MIPS_ISA3__)
#define TPP_MIPS_VERSION 3
#elif defined(__MIPS_ISA2__)
#define TPP_MIPS_VERSION 2
#elif defined(_MIPS_ISA)
#if defined(_MIPS_ISA_MIPS4) && _MIPS_ISA == _MIPS_ISA_MIPS4
#define TPP_MIPS_VERSION 4
#elif defined(_MIPS_ISA_MIPS3) && _MIPS_ISA == _MIPS_ISA_MIPS3
#define TPP_MIPS_VERSION 3
#elif defined(_MIPS_ISA_MIPS2) && _MIPS_ISA == _MIPS_ISA_MIPS2
#define TPP_MIPS_VERSION 2
#else
#define TPP_MIPS_VERSION 1
#endif
#elif defined(__mips__)
#define TPP_MIPS_VERSION __mips__
#elif defined(__MIPS__)
#define TPP_MIPS_VERSION 1
#endif

#ifdef TPP_MIPS_VERSION
PREDEFINED_MACRO_IF(__mips__, HAS(EXT_CPU_MACROS), TPP_PP_STR(TPP_MIPS_VERSION))
PREDEFINED_MACRO_IF(__mips, HAS(EXT_CPU_MACROS), TPP_PP_STR(TPP_MIPS_VERSION))
#undef TPP_MIPS_VERSION
#endif /* TPP_MIPS_VERSION */

#ifdef _M_PPC
#define TPP_POWERPC_VERSION _M_PPC
#elif defined(__ppc604__)
#define TPP_POWERPC_VERSION 604
#elif defined(__ppc603__)
#define TPP_POWERPC_VERSION 603
#elif defined(__ppc601__)
#define TPP_POWERPC_VERSION 601
#elif (defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) ||      \
       defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) ||            \
       defined(__PPC__) || defined(__PPC64__) || defined(_ARCH_PPC) ||              \
       defined(_ARCH_PPC64) || defined(__PPCGECKO__) || defined(__PPCBROADWAY__) || \
       defined(_XENON) || defined(__ppc))
#if defined(_ARCH_620)
#define TPP_POWERPC_VERSION 620
#elif defined(_ARCH_604)
#define TPP_POWERPC_VERSION 604
#elif defined(_ARCH_603)
#define TPP_POWERPC_VERSION 603
#elif defined(_ARCH_601)
#define TPP_POWERPC_VERSION 601
#elif defined(_ARCH_450)
#define TPP_POWERPC_VERSION 450
#else
#define TPP_POWERPC_VERSION 440
#endif
#endif

#ifdef TPP_POWERPC_VERSION
PREDEFINED_MACRO_IF(__powerpc__, HAS(EXT_CPU_MACROS),
                    TPP_PP_STR(TPP_POWERPC_VERSION))
PREDEFINED_KWDMACRO_IF(TPP_PP_CAT3(KWD___ppc, TPP_POWERPC_VERSION, __),
                       "__ppc" TPP_PP_STR(TPP_POWERPC_VERSION) "__",
                       HAS(EXT_CPU_MACROS), "1")
#undef TPP_POWERPC_VERSION
#endif /* TPP_POWERPC_VERSION */

#ifdef pyr
PREDEFINED_MACRO_IF(pyr, HAS(EXT_CPU_MACROS), TPP_PP_STR(pyr))
#endif /* pyr */


#if defined(__sparc_v9__) || defined(__sparcv9)
#define TPP_SPARC_VERSION 9
#elif defined(__sparc_v8__) || defined(__sparcv8)
#define TPP_SPARC_VERSION 8
#elif defined(__sparc__) || defined(__sparc)
#define TPP_SPARC_VERSION 1
#endif

#ifdef TPP_SPARC_VERSION
PREDEFINED_MACRO_IF(__sparc__, HAS(EXT_CPU_MACROS), TPP_PP_STR(TPP_SPARC_VERSION))
#if TPP_SPARC_VERSION == 9
PREDEFINED_MACRO_IF(__sparc_v9__, HAS(EXT_CPU_MACROS), "1")
#elif TPP_SPARC_VERSION == 8
PREDEFINED_MACRO_IF(__sparc_v8__, HAS(EXT_CPU_MACROS), "1")
#endif
#undef TPP_SPARC_VERSION
#endif /* TPP_SPARC_VERSION */

#if defined(__SH5__) || defined(__sh5__)
#define TPP_SUPERH_VERSION 5
#elif defined(__SH4__) || defined(__sh4__)
#define TPP_SUPERH_VERSION 4
#elif defined(__SH3__) || defined(__sh3__)
#define TPP_SUPERH_VERSION 3
#elif defined(__SH2__) || defined(__sh2__)
#define TPP_SUPERH_VERSION 2
#elif defined(__SH1__) || defined(__sh1__) || defined(__sh__)
#define TPP_SUPERH_VERSION 1
#endif

#ifdef TPP_SUPERH_VERSION
PREDEFINED_MACRO_IF(__sh__, HAS(EXT_CPU_MACROS), TPP_PP_STR(TPP_SUPERH_VERSION))
PREDEFINED_KWDMACRO_IF(TPP_PP_CAT3(KWD___sh, TPP_SUPERH_VERSION, __),
                       "__sh" TPP_PP_STR(TPP_SUPERH_VERSION) "__",
                       HAS(EXT_CPU_MACROS), "1")
#undef TPP_SUPERH_VERSION
#endif /* TPP_SUPERH_VERSION */

#if defined(__s390x__) || defined(__zarch__) || defined(__SYSC_ZARCH__)
PREDEFINED_MACRO_IF(__s390x__, HAS(EXT_CPU_MACROS), "1")
#endif /* __s390x__ || __zarch__ || __SYSC_ZARCH__ */
#if defined(__370__) || defined(__THW_370__) || defined(__s390__)
PREDEFINED_MACRO_IF(__370__, HAS(EXT_CPU_MACROS), "1")
#endif /* __370__ || __THW_370__ || __s390__ */

/* System-specific predefined macros. */
#ifdef __APPLE__
PREDEFINED_MACRO_IF(__APPLE__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__APPLE__))
#endif /* __APPLE__ */
#ifdef __MACH__
PREDEFINED_MACRO_IF(__MACH__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__MACH__))
#endif /* __MACH__ */
#ifdef __ANDROID__
PREDEFINED_MACRO_IF(__ANDROID__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__ANDROID__))
#elif defined(__ANDROID)
PREDEFINED_MACRO_IF(__ANDROID__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__ANDROID))
#elif defined(__android__)
PREDEFINED_MACRO_IF(__ANDROID__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__android__))
#elif defined(__android)
PREDEFINED_MACRO_IF(__ANDROID__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__android))
#endif
#ifdef __amigaos__
PREDEFINED_MACRO_IF(__amigaos__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__amigaos__))
#elif defined(AMIGA)
PREDEFINED_MACRO_IF(__amigaos__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(AMIGA))
#endif
#if defined(__bg__) || defined(__bgq__) || defined(__THW_BLUEGENE__) || defined(__TOS_BGQ__)
PREDEFINED_MACRO_IF(__bg__, HAS(EXT_SYSTEM_MACROS), "1")
#endif /* __bg__ || __bgq__ || __THW_BLUEGENE__ || __TOS_BGQ__ */
#ifdef __bgq__
PREDEFINED_MACRO_IF(__bgq__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__bgq__))
#elif defined(__TOS_BGQ__)
PREDEFINED_MACRO_IF(__bgq__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__TOS_BGQ__))
#endif
#ifdef __FreeBSD__
PREDEFINED_MACRO_IF(__FreeBSD__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__FreeBSD__))
#endif /* __FreeBSD__ */
#ifdef __NetBSD__
PREDEFINED_MACRO_IF(__NetBSD__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__NetBSD__))
#endif /* __NetBSD__ */
#ifdef __OpenBSD__
PREDEFINED_MACRO_IF(__OpenBSD__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__OpenBSD__))
#endif /* __OpenBSD__ */
#ifdef __bsdi__
PREDEFINED_MACRO_IF(__bsdi__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__bsdi__))
#endif /* __bsdi__ */
#ifdef __CYGWIN__
#define TPP_PLATFORM_NOT_WINDOWS
PREDEFINED_MACRO_IF(__CYGWIN__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__CYGWIN__))
#elif defined(__MINGW32__)
#define TPP_PLATFORM_NOT_WINDOWS
PREDEFINED_MACRO_IF(__MINGW32__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__MINGW32__))
PREDEFINED_MACRO_IF(__CYGWIN__, HAS(EXT_SYSTEM_MACROS), "1") /* Prefer cygwin! */
#endif
#ifdef DGUX
PREDEFINED_MACRO_IF(__dgux__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(DGUX))
#elif defined(__DGUX__)
PREDEFINED_MACRO_IF(__dgux__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__DGUX__))
#elif defined(__dgux__)
PREDEFINED_MACRO_IF(__dgux__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__dgux__))
#endif
#ifdef __DragonFly__
PREDEFINED_MACRO_IF(__DragonFly__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__DragonFly__))
#endif /* __DragonFly__ */
#ifdef __ECOS
PREDEFINED_MACRO_IF(__ECOS, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__ECOS))
#endif /* __ECOS */
#ifdef __EMX__
PREDEFINED_MACRO_IF(__EMX__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__EMX__))
#endif /* __EMX__ */
#ifdef __hiuxmpp
PREDEFINED_MACRO_IF(__hiuxmpp, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__hiuxmpp))
#endif /* __hiuxmpp */
#ifdef _hpux
PREDEFINED_MACRO_IF(__hpux, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(_hpux))
#elif defined(hpux)
PREDEFINED_MACRO_IF(__hpux, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(hpux))
#elif defined(__hpux)
PREDEFINED_MACRO_IF(__hpux, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__hpux))
#endif
#ifdef sgi
PREDEFINED_MACRO_IF(__sgi, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(sgi))
#elif defined(__sgi)
PREDEFINED_MACRO_IF(__sgi, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__sgi))
#endif
#if defined(__linux__) || defined(__linux) || defined(linux)
PREDEFINED_MACRO_IF(__linux__, HAS(EXT_SYSTEM_MACROS), "1")
#endif /* __linux__ || __linux || linux */
#ifdef __Lynx__
PREDEFINED_MACRO_IF(__Lynx__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__Lynx__))
#endif /* __Lynx__ */
#ifdef __OS9000
PREDEFINED_MACRO_IF(__OS9000, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__OS9000))
#elif defined(_OSK)
PREDEFINED_MACRO_IF(__OS9000, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(_OSK))
#endif
#ifdef __minix
PREDEFINED_MACRO_IF(__minix, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__minix))
#endif /* __minix */
#ifdef __MORPHOS__
PREDEFINED_MACRO_IF(__MORPHOS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__MORPHOS__))
#endif /* __MORPHOS__ */
#ifdef __mpeix
PREDEFINED_MACRO_IF(__mpeix, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__mpeix))
#elif defined(mpeix)
PREDEFINED_MACRO_IF(__mpeix, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(mpeix))
#endif
#ifdef __MSDOS__
PREDEFINED_MACRO_IF(__MSDOS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__MSDOS__))
#elif defined(MSDOS)
PREDEFINED_MACRO_IF(__MSDOS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(MSDOS))
#elif defined(_MSDOS)
PREDEFINED_MACRO_IF(__MSDOS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(_MSDOS))
#elif defined(__DOS__)
PREDEFINED_MACRO_IF(__MSDOS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__DOS__))
#endif
#ifdef __TANDEM
PREDEFINED_MACRO_IF(__TANDEM, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__TANDEM))
#endif /* __TANDEM */
#ifdef __MACOS__
PREDEFINED_MACRO_IF(__MACOS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__MACOS__))
#endif /* __MACOS__ */
#ifdef __MACOSX__
PREDEFINED_MACRO_IF(__MACOSX__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__MACOSX__))
#endif /* __MACOSX__ */
#ifdef __nucleus__
PREDEFINED_MACRO_IF(__nucleus__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__nucleus__))
#endif /* __nucleus__ */
#ifdef OS2
PREDEFINED_MACRO_IF(__OS2__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(OS2))
#elif defined(_OS2)
PREDEFINED_MACRO_IF(__OS2__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(_OS2))
#elif defined(__OS2__)
PREDEFINED_MACRO_IF(__OS2__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__OS2__))
#elif defined(__TOS_OS2__)
PREDEFINED_MACRO_IF(__OS2__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__TOS_OS2__))
#endif
#ifdef __palmos__
PREDEFINED_MACRO_IF(__palmos__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__palmos__))
#endif /* __palmos__ */
#ifdef EPLAN9
PREDEFINED_MACRO_IF(EPLAN9, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(EPLAN9))
#endif /* EPLAN9 */
#ifdef __QNX__
PREDEFINED_MACRO_IF(__QNX__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__QNX__))
#endif /* __QNX__ */
#ifdef __QNXNTO__
PREDEFINED_MACRO_IF(__QNXNTO__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__QNXNTO__))
#endif /* __QNXNTO__ */
#ifdef sinux
PREDEFINED_MACRO_IF(sinux, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(sinux))
#endif /* sinux */
#ifdef sun
PREDEFINED_MACRO_IF(__sun, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(sun))
#elif defined(__sun)
PREDEFINED_MACRO_IF(__sun, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__sun))
#endif
#ifdef __VOS__
PREDEFINED_MACRO_IF(__VOS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__VOS__))
#endif /* __VOS__ */
#if (defined(__sysv__) || defined(__SVR4) || \
     defined(__svr4__) || defined(_SYSTYPE_SVR4))
PREDEFINED_MACRO_IF(__sysv__, HAS(EXT_SYSTEM_MACROS), "1")
PREDEFINED_MACRO_IF(__svr4__, HAS(EXT_SYSTEM_MACROS), "1")
#endif
#ifdef __SYLLABLE__
PREDEFINED_MACRO_IF(__SYLLABLE__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__SYLLABLE__))
#endif /* __SYLLABLE__ */
#ifdef __SYMBIAN32__
PREDEFINED_MACRO_IF(__SYMBIAN32__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__SYMBIAN32__))
#endif /* __SYMBIAN32__ */
#ifdef __osf__
PREDEFINED_MACRO_IF(__osf__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__osf__))
#elif defined(__osf)
PREDEFINED_MACRO_IF(__osf__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__osf))
#endif
#ifdef __ultrix__
PREDEFINED_MACRO_IF(__ultrix__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__ultrix__))
#elif defined(__ultrix)
PREDEFINED_MACRO_IF(__ultrix__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__ultrix))
#elif defined(ultrix)
PREDEFINED_MACRO_IF(__ultrix__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(ultrix))
#elif ((defined(__unix__) || defined(__unix) || defined(unix)) && \
       (defined(__vax__) || defined(__vax) || defined(vax)))
PREDEFINED_MACRO_IF(__ultrix__, HAS(EXT_SYSTEM_MACROS), "1")
#endif
#ifdef _UNICOS
PREDEFINED_MACRO_IF(_UNICOS, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(_UNICOS))
#endif /* _UNICOS */
#ifdef __crayx1
PREDEFINED_MACRO_IF(__crayx1, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__crayx1))
#elif defined(_CRAY)
PREDEFINED_MACRO_IF(__crayx1, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(_CRAY))
#endif
#if defined(__unix__)
PREDEFINED_MACRO_IF(__unix__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__unix__))
#elif defined(__unix)
PREDEFINED_MACRO_IF(__unix__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__unix))
#elif defined(unix)
PREDEFINED_MACRO_IF(__unix__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(unix))
#elif (defined(__ANDROID__) || defined(__ANDROID) ||               \
       defined(__android__) || defined(__android) ||               \
       defined(__linux__) || defined(__linux) || defined(linux) || \
       defined(__MACOS__) || defined(__MACOSX__) || defined(__POSIX__))
PREDEFINED_MACRO_IF(__unix__, HAS(EXT_SYSTEM_MACROS), "1")
#endif
#ifdef __VMS
PREDEFINED_MACRO_IF(__VMS, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__VMS))
#elif defined(VMS)
PREDEFINED_MACRO_IF(__VMS, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(VMS))
#endif
#ifdef __VMS_VER
PREDEFINED_MACRO_IF(__VMS_VER, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__VMS_VER))
#endif /* __VMS_VER */
#ifdef __vxworks
PREDEFINED_MACRO_IF(__vxworks, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__vxworks))
#elif defined(__VXWORKS__)
PREDEFINED_MACRO_IF(__vxworks, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__VXWORKS__))
#endif
#ifndef TPP_PLATFORM_NOT_WINDOWS
#ifdef __WINDOWS__
#define TPP_PLATFORM_WINDOWS
PREDEFINED_MACRO_IF(__WINDOWS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__WINDOWS__))
#elif (defined(_WIN16) || defined(WIN16) || \
       defined(_WIN32) || defined(WIN32) || \
       defined(_WIN64) || defined(WIN64) || \
       defined(__WIN32__) || defined(__TOS_WIN__))
#define TPP_PLATFORM_WINDOWS
PREDEFINED_MACRO_IF(__WINDOWS__, HAS(EXT_SYSTEM_MACROS), "1")
#elif defined(_WIN32_WCE) || defined(WIN32_WCE)
PREDEFINED_MACRO_IF(__WINDOWS__, HAS(EXT_SYSTEM_MACROS), "1")
#endif
#ifdef _WIN32
PREDEFINED_MACRO_IF(_WIN32, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(_WIN32))
#elif defined(WIN32)
PREDEFINED_MACRO_IF(_WIN32, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(WIN32))
#elif defined(__WIN32__)
PREDEFINED_MACRO_IF(_WIN32, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__WIN32__))
#elif defined(TPP_PLATFORM_WINDOWS) && __SIZEOF_POINTER__ >= 4
PREDEFINED_MACRO_IF(_WIN32, HAS(EXT_SYSTEM_MACROS), "1")
#endif
#ifdef _WIN64
PREDEFINED_MACRO_IF(_WIN64, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(_WIN64))
#elif defined(WIN64)
PREDEFINED_MACRO_IF(_WIN64, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(WIN64))
#elif defined(TPP_PLATFORM_WINDOWS) && __SIZEOF_POINTER__ == 8
PREDEFINED_MACRO_IF(_WIN64, HAS(EXT_SYSTEM_MACROS), "1")
#endif
#ifdef _WIN16
PREDEFINED_MACRO_IF(_WIN16, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(_WIN16))
#elif defined(WIN16)
PREDEFINED_MACRO_IF(_WIN16, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(WIN16))
#elif defined(TPP_PLATFORM_WINDOWS) && __SIZEOF_POINTER__ == 2
PREDEFINED_MACRO_IF(_WIN16, HAS(EXT_SYSTEM_MACROS), "1")
#endif
#ifdef _WIN32_WCE
PREDEFINED_MACRO_IF(_WIN32_WCE, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(_WIN32_WCE))
#elif defined(WIN32_WCE)
PREDEFINED_MACRO_IF(_WIN32_WCE, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(WIN32_WCE))
#endif
#ifdef TPP_PLATFORM_WINDOWS
#undef TPP_PLATFORM_WINDOWS
#endif /* TPP_PLATFORM_WINDOWS */
#endif /* !TPP_PLATFORM_NOT_WINDOWS */
#ifdef __MVS__
PREDEFINED_MACRO_IF(__MVS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__MVS__))
#ifdef __HOS_MVS__
PREDEFINED_MACRO_IF(__HOS_MVS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__HOS_MVS__))
#else
PREDEFINED_MACRO_IF(__HOS_MVS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__MVS__))
#endif
#elif defined(__HOS_MVS__)
PREDEFINED_MACRO_IF(__MVS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__HOS_MVS__))
PREDEFINED_MACRO_IF(__HOS_MVS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__HOS_MVS__))
#endif
#ifdef __TOS_MVS__
PREDEFINED_MACRO_IF(__TOS_MVS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__TOS_MVS__))
#endif         /* __TOS_MVS__ */
#ifdef __KOS__ /* ~smirks~ */
PREDEFINED_MACRO_IF(__KOS__, HAS(EXT_SYSTEM_MACROS), TPP_PP_STR(__KOS__))
#endif /* __KOS__ */


/* Utility predefined macros. */
#ifndef TPP_UTILITY_MACROS_DEFINED
#define TPP_UTILITY_MACROS_DEFINED
#ifndef TPP_MSCV_OR_GCC
#ifdef _MSC_VER
#define TPP_MSCV_OR_GCC(msvc, gcc) msvc
#else /* _MSC_VER */
#define TPP_MSCV_OR_GCC(msvc, gcc) gcc
#endif /* !_MSC_VER */
#endif /* !TPP_MSCV_OR_GCC */
#ifndef __INT8_TYPE__
#define __INT8_TYPE__ TPP_MSCV_OR_GCC(__int8, signed char)
#endif /* !__INT8_TYPE__ */
#ifndef __UINT8_TYPE__
#define __UINT8_TYPE__ unsigned TPP_MSCV_OR_GCC(__int8, char)
#endif /* !__UINT8_TYPE__ */
#ifndef __INT16_TYPE__
#define __INT16_TYPE__ TPP_MSCV_OR_GCC(__int16, short)
#endif /* !__INT16_TYPE__ */
#ifndef __UINT16_TYPE__
#define __UINT16_TYPE__ unsigned TPP_MSCV_OR_GCC(__int16, short)
#endif /* !__UINT16_TYPE__ */
#ifndef __INT32_TYPE__
#define __INT32_TYPE__ TPP_MSCV_OR_GCC(__int32, int)
#endif /* !__INT32_TYPE__ */
#ifndef __UINT32_TYPE__
#define __UINT32_TYPE__ unsigned TPP_MSCV_OR_GCC(__int32, int)
#endif /* !__UINT32_TYPE__ */
#ifndef __INT64_TYPE__
#define __INT64_TYPE__ TPP_MSCV_OR_GCC(__int64, long long)
#endif /* !__INT64_TYPE__ */
#ifndef __UINT64_TYPE__
#define __UINT64_TYPE__ unsigned TPP_MSCV_OR_GCC(__int64, long long)
#endif /* !__UINT64_TYPE__ */
#define TPP_MUL8_1      8
#define TPP_MUL8_2      16
#define TPP_MUL8_4      32
#define TPP_MUL8_8      64
#define TPP_MUL8_16     128
#define TPP_MUL8(x)     TPP_PRIVATE_PP_CAT2(TPP_MUL8_, x)
#define TPP_INTTYPE_1   __INT8_TYPE__
#define TPP_INTTYPE_2   __INT16_TYPE__
#define TPP_INTTYPE_4   __INT32_TYPE__
#define TPP_INTTYPE_8   __INT64_TYPE__
#define TPP_UINTTYPE_1  __UINT8_TYPE__
#define TPP_UINTTYPE_2  __UINT16_TYPE__
#define TPP_UINTTYPE_4  __UINT32_TYPE__
#define TPP_UINTTYPE_8  __UINT64_TYPE__
#define TPP_INTTYPE(n)  TPP_PRIVATE_PP_CAT2(TPP_INTTYPE_, n)
#define TPP_UINTTYPE(n) TPP_PRIVATE_PP_CAT2(TPP_UINTTYPE_, n)
#ifndef __SIZEOF_INT__
#define __SIZEOF_INT__ 4
#endif /* !__SIZEOF_INT__ */
#ifndef __SIZEOF_LONG__
#if defined(_WIN16) || defined(WIN16) || \
defined(_WIN32) || defined(WIN32) ||     \
defined(_WIN64) || defined(WIN64)
#define __SIZEOF_LONG__ 4
#else
#define __SIZEOF_LONG__ __SIZEOF_POINTER__
#endif
#endif
#ifndef __SIZEOF_LONG_LONG__
#define __SIZEOF_LONG_LONG__ 8
#endif /* __SIZEOF_LONG_LONG__ */
#ifndef __SIZEOF_SHORT__
#define __SIZEOF_SHORT__ 2
#endif /* __SIZEOF_SHORT__ */
#ifndef __SIZEOF_FLOAT__
#define __SIZEOF_FLOAT__ 4
#endif /* __SIZEOF_FLOAT__ */
#ifndef __SIZEOF_DOUBLE__
#define __SIZEOF_DOUBLE__ 8
#endif /* __SIZEOF_DOUBLE__ */
#ifndef __SIZEOF_LONG_DOUBLE__
#ifdef _MSC_VER
#define __SIZEOF_LONG_DOUBLE__ 8
#elif defined(__C67__) || defined(__i386__) || \
defined(__i386) || defined(i386)
#define __SIZEOF_LONG_DOUBLE__ 12
#elif defined(__X86_64__)
#define __SIZEOF_LONG_DOUBLE__ 16
#else
#define __SIZEOF_LONG_DOUBLE__ 8
#endif
#endif /* !__SIZEOF_LONG_DOUBLE__ */
#ifndef __SIZEOF_SIZE_T__
#define __SIZEOF_SIZE_T__ __SIZEOF_POINTER__
#endif /* !__SIZEOF_SIZE_T__ */
#ifndef __SIZEOF_WCHAR_T__
#if defined(_WIN16) || defined(WIN16) || \
defined(_WIN32) || defined(WIN32) ||     \
defined(_WIN64) || defined(WIN64)
#define __SIZEOF_WCHAR_T__ 2
#else
#define __SIZEOF_WCHAR_T__ 4
#endif
#endif /* !__SIZEOF_WCHAR_T__ */
#ifndef __SIZEOF_WINT_T__
#define __SIZEOF_WINT_T__ __SIZEOF_INT__
#endif /* !__SIZEOF_WINT_T__ */
#ifndef __SIZEOF_PTRDIFF_T__
#define __SIZEOF_PTRDIFF_T__ __SIZEOF_POINTER__
#endif /* !__SIZEOF_PTRDIFF_T__ */
#ifndef __SCHAR_WIDTH__
#define __SCHAR_WIDTH__ 8
#endif /* !__SCHAR_WIDTH__ */
#ifndef __SHRT_WIDTH__
#define __SHRT_WIDTH__ TPP_MUL8(__SIZEOF_SHORT__)
#endif /* !__SHRT_WIDTH__ */
#ifndef __INT_WIDTH__
#define __INT_WIDTH__ TPP_MUL8(__SIZEOF_INT__)
#endif /* !__INT_WIDTH__ */
#ifndef __LONG_WIDTH__
#define __LONG_WIDTH__ TPP_MUL8(__SIZEOF_LONG__)
#endif /* !__LONG_WIDTH__ */
#ifndef __LONG_LONG_WIDTH__
#define __LONG_LONG_WIDTH__ TPP_MUL8(__SIZEOF_LONG_LONG__)
#endif /* !__LONG_LONG_WIDTH__ */
#ifndef __PTRDIFF_WIDTH__
#define __PTRDIFF_WIDTH__ TPP_MUL8(__SIZEOF_PTRDIFF_T__)
#endif /* !__PTRDIFF_WIDTH__ */
#ifndef __SIG_ATOMIC_WIDTH__
#define __SIG_ATOMIC_WIDTH__ TPP_MUL8(__SIZEOF_INT__)
#endif /* !__SIG_ATOMIC_WIDTH__ */
#ifndef __SIZE_WIDTH__
#define __SIZE_WIDTH__ TPP_MUL8(__SIZEOF_SIZE_T__)
#endif /* !__SIZE_WIDTH__ */
#ifndef __WCHAR_WIDTH__
#define __WCHAR_WIDTH__ TPP_MUL8(__SIZEOF_WCHAR_T__)
#endif /* !__WCHAR_WIDTH__ */
#ifndef __WINT_WIDTH__
#define __WINT_WIDTH__ TPP_MUL8(__SIZEOF_WINT_T__)
#endif /* !__WINT_WIDTH__ */
#ifndef __INT_LEAST8_WIDTH__
#define __INT_LEAST8_WIDTH__ 8
#endif /* !__INT_LEAST8_WIDTH__ */
#ifndef __INT_LEAST16_WIDTH__
#define __INT_LEAST16_WIDTH__ 16
#endif /* !__INT_LEAST16_WIDTH__ */
#ifndef __INT_LEAST32_WIDTH__
#define __INT_LEAST32_WIDTH__ 32
#endif /* !__INT_LEAST32_WIDTH__ */
#ifndef __INT_LEAST64_WIDTH__
#define __INT_LEAST64_WIDTH__ 64
#endif /* !__INT_LEAST64_WIDTH__ */
#ifndef __INT_FAST8_WIDTH__
#define __INT_FAST8_WIDTH__ 8
#endif /* !__INT_FAST8_WIDTH__ */
#ifndef __INT_FAST16_WIDTH__
#define __INT_FAST16_WIDTH__ 16
#endif /* !__INT_FAST16_WIDTH__ */
#ifndef __INT_FAST32_WIDTH__
#define __INT_FAST32_WIDTH__ 32
#endif /* !__INT_FAST32_WIDTH__ */
#ifndef __INT_FAST64_WIDTH__
#define __INT_FAST64_WIDTH__ 64
#endif /* !__INT_FAST64_WIDTH__ */
#ifndef __INTPTR_WIDTH__
#define __INTPTR_WIDTH__ TPP_MUL8(__SIZEOF_POINTER__)
#endif /* !__INTPTR_WIDTH__ */
#ifndef __INTMAX_WIDTH__
#define __INTMAX_WIDTH__ 64
#endif /* !__INTMAX_WIDTH__ */

#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ TPP_UINTTYPE(__SIZEOF_POINTER__)
#endif /* !__SIZE_TYPE__ */
#ifndef __PTRDIFF_TYPE__
#define __PTRDIFF_TYPE__ TPP_INTTYPE(__SIZEOF_POINTER__)
#endif /* !__PTRDIFF_TYPE__ */
#ifndef __WCHAR_TYPE__
#define __WCHAR_TYPE__ wchar_t
#endif /* !__WCHAR_TYPE__ */
#ifndef __WINT_TYPE__
#define __WINT_TYPE__ int
#endif /* !__WINT_TYPE__ */
#ifndef __INTMAX_TYPE__
#define __INTMAX_TYPE__ __INT64_TYPE__
#endif /* !__INTMAX_TYPE__ */
#ifndef __UINTMAX_TYPE__
#define __UINTMAX_TYPE__ __UINT64_TYPE__
#endif /* !__UINTMAX_TYPE__ */
#ifndef __SIG_ATOMIC_TYPE__
#define __SIG_ATOMIC_TYPE__ int
#endif /* !__SIG_ATOMIC_TYPE__ */
#ifndef __INT_LEAST8_TYPE__
#define __INT_LEAST8_TYPE__ __INT8_TYPE__
#endif /* !__INT_LEAST8_TYPE__ */
#ifndef __INT_LEAST16_TYPE__
#define __INT_LEAST16_TYPE__ __INT16_TYPE__
#endif /* !__INT_LEAST16_TYPE__ */
#ifndef __INT_LEAST32_TYPE__
#define __INT_LEAST32_TYPE__ __INT32_TYPE__
#endif /* !__INT_LEAST32_TYPE__ */
#ifndef __INT_LEAST64_TYPE__
#define __INT_LEAST64_TYPE__ __INT64_TYPE__
#endif /* !__INT_LEAST64_TYPE__ */
#ifndef __UINT_LEAST8_TYPE__
#define __UINT_LEAST8_TYPE__ __UINT8_TYPE__
#endif /* !__UINT_LEAST8_TYPE__ */
#ifndef __UINT_LEAST16_TYPE__
#define __UINT_LEAST16_TYPE__ __UINT16_TYPE__
#endif /* !__UINT_LEAST16_TYPE__ */
#ifndef __UINT_LEAST32_TYPE__
#define __UINT_LEAST32_TYPE__ __UINT32_TYPE__
#endif /* !__UINT_LEAST32_TYPE__ */
#ifndef __UINT_LEAST64_TYPE__
#define __UINT_LEAST64_TYPE__ __UINT64_TYPE__
#endif /* !__UINT_LEAST64_TYPE__ */
#ifndef __INT_FAST8_TYPE__
#define __INT_FAST8_TYPE__ __INT8_TYPE__
#endif /* !__INT_FAST8_TYPE__ */
#ifndef __INT_FAST16_TYPE__
#define __INT_FAST16_TYPE__ __INT16_TYPE__
#endif /* !__INT_FAST16_TYPE__ */
#ifndef __INT_FAST32_TYPE__
#define __INT_FAST32_TYPE__ __INT32_TYPE__
#endif /* !__INT_FAST32_TYPE__ */
#ifndef __INT_FAST64_TYPE__
#define __INT_FAST64_TYPE__ __INT64_TYPE__
#endif /* !__INT_FAST64_TYPE__ */
#ifndef __UINT_FAST8_TYPE__
#define __UINT_FAST8_TYPE__ __UINT8_TYPE__
#endif /* !__UINT_FAST8_TYPE__ */
#ifndef __UINT_FAST16_TYPE__
#define __UINT_FAST16_TYPE__ __UINT16_TYPE__
#endif /* !__UINT_FAST16_TYPE__ */
#ifndef __UINT_FAST32_TYPE__
#define __UINT_FAST32_TYPE__ __UINT32_TYPE__
#endif /* !__UINT_FAST32_TYPE__ */
#ifndef __UINT_FAST64_TYPE__
#define __UINT_FAST64_TYPE__ __UINT64_TYPE__
#endif /* !__UINT_FAST64_TYPE__ */
#ifndef __INTPTR_TYPE__
#define __INTPTR_TYPE__ __PTRDIFF_TYPE__
#endif /* !__INTPTR_TYPE__ */
#ifndef __UINTPTR_TYPE__
#define __UINTPTR_TYPE__ __SIZE_TYPE__
#endif /* !__UINTPTR_TYPE__ */
#endif /* !TPP_UTILITY_MACROS_DEFINED */

#if ((defined(__LP64__) || defined(_LP64)) || \
     (__SIZEOF_INT__ == 4 && __SIZEOF_LONG__ == 8 && __SIZEOF_POINTER__ == 8))
PREDEFINED_MACRO_IF(__LP64__, HAS(EXT_SYSTEM_MACROS), "1")
#endif

PREDEFINED_MACRO_IF(__BYTE_ORDER__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(TPP_BYTEORDER))
#ifdef __FLOAT_WORD_ORDER__
PREDEFINED_MACRO_IF(__FLOAT_WORD_ORDER__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__FLOAT_WORD_ORDER__))
#else  /* __FLOAT_WORD_ORDER__ */
PREDEFINED_MACRO_IF(__FLOAT_WORD_ORDER__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(TPP_BYTEORDER))
#endif /* !__FLOAT_WORD_ORDER__ */
PREDEFINED_MACRO_IF(__ORDER_LITTLE_ENDIAN__, HAS(EXT_UTILITY_MACROS), "1234")
PREDEFINED_MACRO_IF(__ORDER_BIG_ENDIAN__, HAS(EXT_UTILITY_MACROS), "4321")
PREDEFINED_MACRO_IF(__ORDER_PDP_ENDIAN__, HAS(EXT_UTILITY_MACROS), "3412")

PREDEFINED_MACRO_IF(__SIZEOF_INT__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZEOF_INT__))
PREDEFINED_MACRO_IF(__SIZEOF_LONG__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZEOF_LONG__))
PREDEFINED_MACRO_IF(__SIZEOF_LONG_LONG__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZEOF_LONG_LONG__))
PREDEFINED_MACRO_IF(__SIZEOF_SHORT__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZEOF_SHORT__))
PREDEFINED_MACRO_IF(__SIZEOF_POINTER__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZEOF_POINTER__))
PREDEFINED_MACRO_IF(__SIZEOF_FLOAT__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZEOF_FLOAT__))
PREDEFINED_MACRO_IF(__SIZEOF_DOUBLE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZEOF_DOUBLE__))
PREDEFINED_MACRO_IF(__SIZEOF_LONG_DOUBLE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZEOF_LONG_DOUBLE__))
PREDEFINED_MACRO_IF(__SIZEOF_SIZE_T__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZEOF_SIZE_T__))
PREDEFINED_MACRO_IF(__SIZEOF_WCHAR_T__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZEOF_WCHAR_T__))
PREDEFINED_MACRO_IF(__SIZEOF_WINT_T__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZEOF_WINT_T__))
PREDEFINED_MACRO_IF(__SIZEOF_PTRDIFF_T__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZEOF_PTRDIFF_T__))

PREDEFINED_MACRO_IF(__SCHAR_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SCHAR_WIDTH__))
PREDEFINED_MACRO_IF(__SHRT_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SHRT_WIDTH__))
PREDEFINED_MACRO_IF(__INT_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_WIDTH__))
PREDEFINED_MACRO_IF(__LONG_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__LONG_WIDTH__))
PREDEFINED_MACRO_IF(__LONG_LONG_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__LONG_LONG_WIDTH__))
PREDEFINED_MACRO_IF(__PTRDIFF_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__PTRDIFF_WIDTH__))
PREDEFINED_MACRO_IF(__SIG_ATOMIC_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIG_ATOMIC_WIDTH__))
PREDEFINED_MACRO_IF(__SIZE_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZE_WIDTH__))
PREDEFINED_MACRO_IF(__WCHAR_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__WCHAR_WIDTH__))
PREDEFINED_MACRO_IF(__WINT_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__WINT_WIDTH__))
PREDEFINED_MACRO_IF(__INT_LEAST8_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_LEAST8_WIDTH__))
PREDEFINED_MACRO_IF(__INT_LEAST16_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_LEAST16_WIDTH__))
PREDEFINED_MACRO_IF(__INT_LEAST32_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_LEAST32_WIDTH__))
PREDEFINED_MACRO_IF(__INT_LEAST64_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_LEAST64_WIDTH__))
PREDEFINED_MACRO_IF(__INT_FAST8_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_FAST8_WIDTH__))
PREDEFINED_MACRO_IF(__INT_FAST16_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_FAST16_WIDTH__))
PREDEFINED_MACRO_IF(__INT_FAST32_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_FAST32_WIDTH__))
PREDEFINED_MACRO_IF(__INT_FAST64_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_FAST64_WIDTH__))
PREDEFINED_MACRO_IF(__INTPTR_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INTPTR_WIDTH__))
PREDEFINED_MACRO_IF(__INTMAX_WIDTH__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INTMAX_WIDTH__))

PREDEFINED_MACRO_IF(__SIZE_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIZE_TYPE__))
PREDEFINED_MACRO_IF(__PTRDIFF_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__PTRDIFF_TYPE__))
PREDEFINED_MACRO_IF(__WCHAR_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__WCHAR_TYPE__))
PREDEFINED_MACRO_IF(__WINT_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__WINT_TYPE__))
PREDEFINED_MACRO_IF(__INTMAX_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INTMAX_TYPE__))
PREDEFINED_MACRO_IF(__UINTMAX_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINTMAX_TYPE__))
PREDEFINED_MACRO_IF(__SIG_ATOMIC_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__SIG_ATOMIC_TYPE__))
PREDEFINED_MACRO_IF(__INT8_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT8_TYPE__))
PREDEFINED_MACRO_IF(__INT16_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT16_TYPE__))
PREDEFINED_MACRO_IF(__INT32_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT32_TYPE__))
PREDEFINED_MACRO_IF(__INT64_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT64_TYPE__))
PREDEFINED_MACRO_IF(__UINT8_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINT8_TYPE__))
PREDEFINED_MACRO_IF(__UINT16_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINT16_TYPE__))
PREDEFINED_MACRO_IF(__UINT32_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINT32_TYPE__))
PREDEFINED_MACRO_IF(__UINT64_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINT64_TYPE__))
PREDEFINED_MACRO_IF(__INT_LEAST8_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_LEAST8_TYPE__))
PREDEFINED_MACRO_IF(__INT_LEAST16_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_LEAST16_TYPE__))
PREDEFINED_MACRO_IF(__INT_LEAST32_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_LEAST32_TYPE__))
PREDEFINED_MACRO_IF(__INT_LEAST64_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_LEAST64_TYPE__))
PREDEFINED_MACRO_IF(__UINT_LEAST8_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINT_LEAST8_TYPE__))
PREDEFINED_MACRO_IF(__UINT_LEAST16_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINT_LEAST16_TYPE__))
PREDEFINED_MACRO_IF(__UINT_LEAST32_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINT_LEAST32_TYPE__))
PREDEFINED_MACRO_IF(__UINT_LEAST64_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINT_LEAST64_TYPE__))
PREDEFINED_MACRO_IF(__INT_FAST8_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_FAST8_TYPE__))
PREDEFINED_MACRO_IF(__INT_FAST16_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_FAST16_TYPE__))
PREDEFINED_MACRO_IF(__INT_FAST32_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_FAST32_TYPE__))
PREDEFINED_MACRO_IF(__INT_FAST64_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INT_FAST64_TYPE__))
PREDEFINED_MACRO_IF(__UINT_FAST8_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINT_FAST8_TYPE__))
PREDEFINED_MACRO_IF(__UINT_FAST16_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINT_FAST16_TYPE__))
PREDEFINED_MACRO_IF(__UINT_FAST32_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINT_FAST32_TYPE__))
PREDEFINED_MACRO_IF(__UINT_FAST64_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINT_FAST64_TYPE__))
PREDEFINED_MACRO_IF(__INTPTR_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__INTPTR_TYPE__))
PREDEFINED_MACRO_IF(__UINTPTR_TYPE__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__UINTPTR_TYPE__))

PREDEFINED_MACRO_IF(__CHAR_BIT__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(TPP_CHAR_BIT))
PREDEFINED_MACRO_IF(__SCHAR_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(SCHAR_MAX))
PREDEFINED_MACRO_IF(__WCHAR_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(WCHAR_MAX))
PREDEFINED_MACRO_IF(__SHRT_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(SHRT_MAX))
PREDEFINED_MACRO_IF(__INT_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT_MAX))
PREDEFINED_MACRO_IF(__LONG_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(LONG_MAX))
#ifdef __LONG_LONG_MAX__
PREDEFINED_MACRO_IF(__LONG_LONG_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(__LONG_LONG_MAX__))
#elif defined(LONG_LONG_MAX)
PREDEFINED_MACRO_IF(__LONG_LONG_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(LONG_LONG_MAX))
#else
PREDEFINED_MACRO_IF(__LONG_LONG_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(LLONG_MAX))
#endif
PREDEFINED_MACRO_IF(__WINT_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(WINT_MAX))
PREDEFINED_MACRO_IF(__SIZE_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(SIZE_MAX))
PREDEFINED_MACRO_IF(__PTRDIFF_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(PTRDIFF_MAX))
PREDEFINED_MACRO_IF(__INTMAX_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INTMAX_MAX))
PREDEFINED_MACRO_IF(__UINTMAX_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINTMAX_MAX))
PREDEFINED_MACRO_IF(__SIG_ATOMIC_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(SIG_ATOMIC_MAX))
PREDEFINED_MACRO_IF(__INT8_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT8_MAX))
PREDEFINED_MACRO_IF(__INT16_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT16_MAX))
PREDEFINED_MACRO_IF(__INT32_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT32_MAX))
PREDEFINED_MACRO_IF(__INT64_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT64_MAX))
PREDEFINED_MACRO_IF(__UINT8_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINT8_MAX))
PREDEFINED_MACRO_IF(__UINT16_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINT16_MAX))
PREDEFINED_MACRO_IF(__UINT32_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINT32_MAX))
PREDEFINED_MACRO_IF(__UINT64_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINT64_MAX))
PREDEFINED_MACRO_IF(__INT_LEAST8_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT_LEAST8_MAX))
PREDEFINED_MACRO_IF(__INT_LEAST16_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT_LEAST16_MAX))
PREDEFINED_MACRO_IF(__INT_LEAST32_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT_LEAST32_MAX))
PREDEFINED_MACRO_IF(__INT_LEAST64_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT_LEAST64_MAX))
PREDEFINED_MACRO_IF(__UINT_LEAST8_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINT_LEAST8_MAX))
PREDEFINED_MACRO_IF(__UINT_LEAST16_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINT_LEAST16_MAX))
PREDEFINED_MACRO_IF(__UINT_LEAST32_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINT_LEAST32_MAX))
PREDEFINED_MACRO_IF(__UINT_LEAST64_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINT_LEAST64_MAX))
PREDEFINED_MACRO_IF(__INT_FAST8_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT_FAST8_MAX))
PREDEFINED_MACRO_IF(__INT_FAST16_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT_FAST16_MAX))
PREDEFINED_MACRO_IF(__INT_FAST32_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT_FAST32_MAX))
PREDEFINED_MACRO_IF(__INT_FAST64_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INT_FAST64_MAX))
PREDEFINED_MACRO_IF(__UINT_FAST8_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINT_FAST8_MAX))
PREDEFINED_MACRO_IF(__UINT_FAST16_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINT_FAST16_MAX))
PREDEFINED_MACRO_IF(__UINT_FAST32_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINT_FAST32_MAX))
PREDEFINED_MACRO_IF(__UINT_FAST64_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINT_FAST64_MAX))
PREDEFINED_MACRO_IF(__INTPTR_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(INTPTR_MAX))
PREDEFINED_MACRO_IF(__UINTPTR_MAX__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(UINTPTR_MAX))
PREDEFINED_MACRO_IF(__WCHAR_MIN__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(WCHAR_MIN))
PREDEFINED_MACRO_IF(__WINT_MIN__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(WINT_MIN))
PREDEFINED_MACRO_IF(__SIG_ATOMIC_MIN__, HAS(EXT_UTILITY_MACROS), TPP_PP_STR(SIG_ATOMIC_MIN))

/*
DEF_M_IF(__INT8_C, HAS(EXT_UTILITY_MACROS))
DEF_M_IF(__INT16_C, HAS(EXT_UTILITY_MACROS))
DEF_M_IF(__INT32_C, HAS(EXT_UTILITY_MACROS))
DEF_M_IF(__INT64_C, HAS(EXT_UTILITY_MACROS))
DEF_M_IF(__UINT8_C, HAS(EXT_UTILITY_MACROS))
DEF_M_IF(__UINT16_C, HAS(EXT_UTILITY_MACROS))
DEF_M_IF(__UINT32_C, HAS(EXT_UTILITY_MACROS))
DEF_M_IF(__UINT64_C, HAS(EXT_UTILITY_MACROS))
DEF_M_IF(__INTMAX_C, HAS(EXT_UTILITY_MACROS))
DEF_M_IF(__UINTMAX_C, HAS(EXT_UTILITY_MACROS))
PREDEFINED_MACRO_IF(__CHAR_UNSIGNED__, HAS(EXT_UTILITY_MACROS) && (TPPLexer_Current->l_flags & TPPLEXER_FLAG_CHAR_UNSIGNED), "1")
PREDEFINED_MACRO_IF(__WCHAR_UNSIGNED__, HAS(EXT_UTILITY_MACROS), "1")
*/

#undef TPP_PP_CAT3
#undef TPP_PP_CAT2
#undef TPP_PRIVATE_PP_CAT3
#undef TPP_PRIVATE_PP_CAT2
#endif /* !TPP_CONFIG_MINMACRO */
/************************************************************************/
/************************************************************************/
/************************************************************************/




/* Allow the user to provide additional definitions */
#ifdef TPP_USERDEFS
#include TPP_USERDEFS
#endif /* TPP_USERDEFS */


/* Misc utility macros... */
#undef TPPLexer_Current
#define TPPLexer_Current TPP2_DEFAULT_Lexer_Current
#undef HAS
#undef token
#undef tok
#undef yield
#undef WARN
#undef HAS
#undef CURRENT
#ifdef tpp_va_arg
#undef Q
#undef WARNF
#undef TOK_S
#undef TOK_A
#undef KWDNAME
#undef FILENAME
#undef ARG
#endif /* tpp_va_arg */


#undef TPP_PP_STR
#undef TPP_PP_STR2
#undef PREDEFINED_BUILTIN_FUNCTION
#undef PREDEFINED_BUILTIN_FUNCTION_IF
#undef PREDEFINED_BUILTIN_KWDFUNCTION
#undef PREDEFINED_BUILTIN_KWDFUNCTION_IF
#undef PREDEFINED_FUNCTION
#undef PREDEFINED_FUNCTION_IF
#undef PREDEFINED_KWDFUNCTION
#undef PREDEFINED_KWDFUNCTION_IF
#undef PREDEFINED_RT_MACRO
#undef PREDEFINED_RT_MACRO_IF
#undef PREDEFINED_RT_KWDMACRO
#undef PREDEFINED_RT_KWDMACRO_IF
#undef PREDEFINED_MACRO
#undef PREDEFINED_MACRO_IF
#undef PREDEFINED_KWDMACRO
#undef PREDEFINED_KWDMACRO_IF
#undef HAS_EXTENSION_IF
#undef HAS_FEATURE_IF
#undef HAS_BUILTIN_IF
#undef DEF_WARNING
#undef DEF_M_IF
#undef DEF_M
#undef DEF_K

#undef EXTENSION
#undef WARNING_MESSAGE
#undef BUILTIN_FUNCTION
#undef RT_BUILTIN_MACRO
#undef BUILTIN_MACRO
#undef MACRO
#undef WARNING_NAMESPACE
#undef WARNING
#undef WGROUP
#undef KWD_FLAGS
#undef KWD

#else /* TPP_DEFS */
#ifndef GUARD_TPP2_H
#define GUARD_TPP2_H 1

/*
 * Compatibility wrapper header for tpp2
 *
 * This obviously won't catch everything (and won't be some kind of "magic bullet"
 * that'll enable you to use tpp3 when coming from tpp2 without needing to make any
 * changes to your code), but it should make transitioning al *little* easier.
 */

/* Enable symbol aliases in the global namespace (TPP2 did this under "#define TPP(x) x") */
#ifndef TPP2_HAVE_GLOBAL_NAMESPACE
#define TPP2_HAVE_GLOBAL_NAMESPACE 1
#endif /* !TPP2_HAVE_GLOBAL_NAMESPACE */


/************************************************************************/
/* MIGRATION NOTES (use CTRL+F to search for TPP2 identifiers)          */
/************************************************************************/

/* TPP_CONFIG_ONELEXER, TPP_CONFIG_FASTSTARTUP_KEYWORD_FLAGS
 *  - TPP3 now only supports the multi-lexer configuration (TPP_CONFIG_ONELEXER=3)
 *    where the lexer is passed as an argument to various APIs. This has been done
 *    intentionally in order to allow multi-threaded API usage, where every thread
 *    has its own "tpp_lexer", which are all able to operate individually.
 *  - For global/one-lexer configurations, this compatibility header uses a new
 *    configuration macro "TPP2_LEXER", which is used in place for the current
 *    lexer. When transitioning to TPP3, this can be used as a stop-gap measure.
 */

/* TPP_CONFIG_MINMACRO, TPP_CONFIG_GCCFUNC, TPP_CONFIG_MINGCCFUNC
 *  - TPP3 no longer includes "builtin" function support, nor does it try to pre-
 *    define CPU-specific macros. As such, these config options (which used to
 *    configure which macros/builtins are available) are no longer needed.
 *  - Adding CPU-specific should be done by the API user in TPP3, though this header
 *    does continue to provide them (only if: #ifndef TPP2_NO_AUTOCONFIGURE_TPP3_DEFS)
 */

/* TPP_UNESCAPE_MAXCHAR
 *  - TPP3 operates entirely on unicode/utf-8. As such, you can no longer configure
 *    the width of your designed USC-character type.
 *  - To decode multi-char characters, you must supply your own utf-8 decoder when
 *    calling "tpp_lexer_parsestring_ex()", which you can then use to transform
 *    input into whichever encoding you wish to use (though I recommend you just
 *    stick with utf-8 if possible)
 */

/* TPP_UNESCAPE_ENDIAN, TPP_BYTEORDER
 *  - TPP3 is written to be agnostic to CPU endian at runtime
 *  - The only place TPP2 (and TPP3) need the endian is for decoding UTF-16
 */

/* TPP_CONFIG_EXPORT
 *  - TPP3 has a completely different API, so it wouldn't make sense to
 *    support a legacy config macro that enables the dllexport-style export
 *    of that API
 *  - Instead, if you want to export TPP3, you can override the following:
 *    - TPP_DECL
 *    - TPP_IMPL
 *    - TPP_CONST_DECL
 *    - TPP_CONST_IMPL
 *    - TPP_INTERN_DECL
 *    - TPP_INTERN_IMPL
 */

/* EXT_CLANG_FEATURES: "-fhas-feature-macros"
 *  - This extension has been split into its individual components:
 *    -> TPP_EXT_CLANG_MACRO___has_attribute:          "-fclang-__has_attribute"
 *    -> TPP_EXT_CLANG_MACRO___has_builtin:            "-fclang-__has_builtin"
 *    -> TPP_EXT_CLANG_MACRO___has_cpp_attribute:      "-fclang-__has_cpp_attribute"
 *    -> TPP_EXT_CLANG_MACRO___has_declspec_attribute: "-fclang-__has_declspec_attribute"
 *    -> TPP_EXT_CLANG_MACRO___has_extension:          "-fclang-__has_extension"
 *    -> TPP_EXT_CLANG_MACRO___has_feature:            "-fclang-__has_feature"
 *    -> TPP_EXT_CLANG_MACRO___has_c_attribute:        "-fclang-__has_c_attribute"
 *    -> TPP_EXT_MACRO___is_identifier:                "-f__is_identifier"
 *    -> TPP_EXT_MACRO___is_deprecated:                "-f__is_deprecated"
 *    -> TPP_EXT_MACRO___is_poisoned:                  "-f__is_poisoned"
 */

/* EXT_HAS_INCLUDE: "has-include-macros"
 *  - This extension has been split into its individual components:
 *    -> TPP_EXT_MACRO___has_include:                  "-f__has_include"
 *    -> TPP_EXT_MACRO___has_include_next:             "-f__has_include_next"
 */

/* EXT_EXTENDED_IDENTS: "extended-identifiers"
 *  - TPP3 no longer includes ANSI support. Instead, TPP3 supports proper unicode
 *    and utf-8. As such, there is no extension to enable/disable ANSI identifiers.
 *    Instead, you can pre-define macros like "tpp_unicode_issymstrt()" to supply
 *    TPP with a unicode character traits database, which it will happly use.
 *  - Decoding input files from (certain) codecs into utf-8 is done automatically
 *  - TPP unicode support is enabled with "TPP_HAVE_UNICODE"
 *  - For compatibility with TPP2's (flaky) attempts at decoding (e.g.) utf-16
 *    into utf-8, this compatibility header force-enables unicode support, even
 *    though that (somewhat) conflicts with TPP2's ANSI support.
 */

/* TPPLEXER_TOKEN_NONE, TPPLEXER_TOKEN_DEFAULT:
 *  - TPP3 allows recognized tokens to be configured on a per-token basis.
 *  - Every recognized token can also be configured to be runtime configurable
 *  - When runtime configurable, tokens can be enabled/disabled using "tpp_lexer_setfeat"
 */

/* TPPLEXER_TOKEN_EQUALBINOP:
 *  - TPP3 still includes support for tokens like "=+", however unlike
 *    in TPP2, TPP3 defines separate token IDs for all mirrored variants.
 *  - As such, you will need to re-write your code as:
 *    ```diff
 *    -case TPP_TOK_SHL_EQUAL:
 *    +case TPP_TOK_LANGLE_LANGLE_EQUAL:
 *    +case TPP_TOK_EQUAL_LANGLE_LANGLE:
 *    ```
 */

/* EXT_DOLLAR_IS_ALPHA: "-fdollars-in-identifiers"
 * TPP_CONFIG_EXTENSION_DOLLAR_IS_ALPHA_DEFAULT
 * TPP_CONFIG_EXTENSION_DOLLAR_IS_ALPHA
 *  - TPP3 has removed this feature as an extension switch.
 *    Instead, it is a lexer "feature" that can be configured
 *    (possibly at runtime) via "TPP_HAVE_TPP_TOK_DOLLAR"
 *  - When "TPP_HAVE_TPP_TOK_DOLLAR" is enabled, '$' is treated
 *    as its own, 1-char token.
 *  - When "TPP_HAVE_TPP_TOK_DOLLAR" is disabled, '$' is treated
 *    like any other tpp_ascii_issymcont() byte, meaning it is
 *    treated as part of a keyword / identifier
 *  - HINT: #define TPP_HAVE_TPP_TOK_DOLLAR (-1)  // Runtime-configurable; default=true
 *    HINT: #define TPP_HAVE_TPP_TOK_DOLLAR (-2)  // Runtime-configurable; default=false
 */

/* EXT_CANONICAL_HEADERS: "-fcanonical-system-headers"
 * TPP_CONFIG_EXTENSION_CANONICAL_HEADERS_DEFAULT
 * TPP_CONFIG_EXTENSION_CANONICAL_HEADERS
 *  - Filename normalization is no longer something that can be configured
 *    or affected using conventional lexer features / extensions. This is
 *    because the act of normalizing a filename / opening a file is now
 *    something that needs to happen at the time a lexer is initialized
 *  - Since TPP3 requires you to specify the file to-be loaded initially
 *    when initializing your lexer, that also means that there is no point
 *    in time where filename normalization could be overwritten/configured
 *  - You can however still affect how TPP3 normalizes filenames (TPP_FS_*):
 *    - TPP_FS_HAVE_DRIVES
 *    - TPP_FS_HAVE_ICASE
 *    - TPP_FS_SEP
 *    - TPP_FS_ALTSEP
 *    - TPP_FS_ISSEP
 *    - TPP_FS_ISABS
 *    Additionally, you may take a look at `tpp_fs_normalize()'
 */

/* TOK_CHAR, TPP_TOK_CHAR, TOK_STRING, TPP_TOK_STRING:
 *  - TPP3 has individual tokens for every type of string, whereas
 *    TPP2 used to have only 2 token types describing string and
 *    char tokens
 *  - Because of this, you should migrate code as follows:
 *    ```diff
 *    - case TPP_TOK_CHAR:
 *    - case TPP_TOK_STRING:
 *    +     if (tok == TPP_TOK_CHAR || tok == TPP_TOK_STRING) {
 *    + TPP_CASE_TPP_TOK_STRING
 *    +     if (TPP_TOK_ISSTRING(tok)) {
 *    ```
 */

/* TOK_COMMENT, TPP_TOK_COMMENT:
 *  - TPP3 has individual tokens for every type of comment
 *  - It also differentiates between comments that include a trailing
 *    line-feed, and comments that end in-line
 *  - Because of this, you should migrate code as follows:
 *    ```diff
 *    - case TPP_TOK_COMMENT:
 *    -     if (tok == TPP_TOK_COMMENT) {
 *    + TPP_CASE_TPP_TOK_COMMENT
 *    +     if (TPP_TOK_ISCOMMENT(tok)) {
 *    ```
 */

/* TOK_ERR, TPP_CONFIG_SET_API_ERROR, TPPLEXER_FLAG_ERROR,
 * TPP_CONFIG_SET_API_ERROR_BADALLOC, TPP_CONFIG_INLINE_SETERR:
 *  - TPP3 defines individual token IDs for different errors that can
 *    happen during lexing, whereas TPP2 only used to define a singular
 *    token ID to represent errors.
 *  - When migrating, you should adjust your error-checking code to
 *    deal with all possible types of lexing errors:
 *    ```diff
 *    - case TOK_ERR:
 *    -     if (tok == TOK_ERR) {
 *    + TPP_CASE_TPP_TOK_ERR
 *    +     if (TPP_TOK_ISERR(tok)) {
 *    ```
 *
 * Note however that you are advised to deal with the meaning of actual errors:
 *  - TPP_TOK_ENOMEM, TPP_TOK_EIO, TPP_TOK_EWARNPRINT:
 *    These errors are not caused by TPP itself and indicate a problem
 *    with the underlying operating system. In all likelihood, you can
 *    probably just propagate these errors
 *  - TPP_TOK_ELEXERROR:
 *    This error is what comes closest to TPP2's "TPPLexer_SetErr()"
 *    macro. This error is returned when an error/fatal-level message
 *    was emitted in a way where the configuration of lexer warnings
 *    indicates that compilation must be aborted.
 *  - TPP_TOK_EWOULDBLOCK:
 *    You will only see this error if you made use of "TPP_CONFIG_NONBLOCKING_IO"
 *    (aka. "TPP_HAVE_FILE_NONBLOCK" in TPP3). This is a temporary error
 *    that means that the next token cannot be read *right now* because
 *    reading from the underlying I/O file would block.
 *    -> You will not see this error when building with "-DTPP_HAVE_FILE_NONBLOCK=0"
 *    -> You will not see this error when not using the "TPP_FILE_IOFLAGS_NONBLOCK" flag
 */

/* TPP_CONFIG_EXTENSION_MULTICHAR_CONST_DEFAULT,
 * TPP_CONFIG_EXTENSION_MULTICHAR_CONST, EXT_MULTICHAR_CONST, "-fmultichar-constants":
 *  - TPP3 allows multi-char literals by default. Instead of having an extension to
 *    enable/disable support for this, there is now a warning "-Wmultichar" that is
 *    emitted by default when multi-char constants are used.
 *  - see: TPP_HAVE_TPP_W_MULTICHAR_LITERAL
 */

/* >> int TPPFile_Copyname(struct TPPFile *self);
 * >> struct TPPFile *TPPFile_CopyForInclude(struct TPPFile *self);
 * >> struct TPPFile *TPPFile_NewDefine(void);
 * >> int TPPLexer_ExpandFunctionMacro_(struct TPPLexer *self, struct TPPFile *macro);
 * >> int TPPLexer_ExpandFunctionMacro(struct TPPFile *macro);
 *  - These functions should have never been exposed by the TPP2 API.
 *    If you've been using them, you'll have to decide how to migrate
 *  - TPP3 uses a completely different model when it comes to how files
 *    are managed and pushed on the #include-stack: namely, the current
 *    file is *inlined* within the lexer, allowing for faster access,
 *    and allowing a simple lexer to be constructed without the need
 *    for *any* heap allocations */

/* >> struct TPPFile *TPPFile_Open(char const *filename);
 * >> struct TPPFile *TPPFile_OpenStream(TPP_stream_t stream, char const *name);
 *  - TPP3 uses a completely different model when it comes to how files
 *    are managed and pushed on the #include-stack
 *  - These functions don't exist in the same manner anymore
 *  - To migrate these functions, see the following:
 *                          First file                  Additional file
 *    - TPPFile_Open:       tpp_lexer_initfile_open()   tpp_lexer_pushfile_open()
 *    - TPPFile_OpenStream: tpp_lexer_initfile_io_ex()  tpp_lexer_pushfile_io_ex()
 */

/* >> char *TPP_Unescape_(tpp_lexer *self, char *buf, char const *data, size_t size);
 * >> char *TPP_Unescape(char *buf, char const *data, size_t size);
 * >> char *TPP_UnescapeRaw(char *buf, char const *data, size_t size);
 * >> size_t TPP_SizeofUnescape_(tpp_lexer *self, char const *data, size_t size);
 * >> size_t TPP_SizeofUnescape(char const *data, size_t size);
 * >> size_t TPP_SizeofUnescapeRaw(char const *data, size_t size);
 *  - Since TPP3 supports many different string formats (some of which don't even
 *    support \-escape sequences), these function no longer exist.
 *  - Instead, TPP3 has a function "tpp_lexer_decodestring()" can can be used to
 *    decode the character data of *any* type of string token. However, unlike
 *    the TPP2 api, "tpp_lexer_decodestring()" expects the currently loaded token
 *    to be string-like (TPP_TOK_ISSTRING()), rather than taking string token data
 *    via "data" + "size" arguments
 *  - Migrate use of these APIs as follows:
 *    ```diff
 *    - size_t size = TPP_SizeofUnescape(TPPLexer_Current->l_token.t_begin,
 *    -                                  (size_t)(TPPLexer_Current->l_token.t_end -
 *    -                                           TPPLexer_Current->l_token.t_begin));
 *    - char *buf = (char *)malloc(size);
 *    - if (!buf) return NULL;
 *    - TPP_Unescape(buf, TPPLexer_Current->l_token.t_begin,
 *    -              (size_t)(TPPLexer_Current->l_token.t_end -
 *    -                       TPPLexer_Current->l_token.t_begin));
 *    - ...
 *    - free(buf);
 *    + tpp_string_builder builder;
 *    + tpp_string_builder_init(&builder);
 *    + if (tpp_lexer_decodestring(TPPLexer_Current, &tpp_string_builder_print, &tpp_string_builder_print, &builder) < 0)
 *    +     return NULL;
 *    + TPP_REF tpp_string *string = tpp_string_builder_pack(&builder);
 *    + char *buf = (char *)tpp_string_str(string);
 *    + tpp_size size = tpp_string_len(string);
 *    + ...
 *    + tpp_string_decref(string);
 *    ```
 */

/* TPP_CONFIG_LOCKED_KEYWORDS, TPP_KEYWORDFLAG_LOCKED,
 * W_CANT_DEFINE_LOCKED_KEYWORD, W_CANT_UNDEF_LOCKED_KEYWORD:
 *  - Locked keywords are no longer supported by TPP3.
 *  - Instead, TPP3 simply doesn't allow users to re-define builtin macros,
 *    but there is no runtime configurable flag (that can't even be set by
 *    a builtin #pragma, meaning it's entire purpose is to be set explicitly)
 */

/* TPP_KEYWORDFLAG_NO_UNDERSCORES:
 *  - No longer supported in TPP3. All feature-test macros allow for optional
 *    leading/trailing _-characters, and this functionality cannot be disabled
 *    for some specific feature macro (because: why would you want to?)
 */

/* TPPLEXER_FLAG_WILLRESTORE,
 * >> struct TPPLexerFilePosition { ... };
 * >> struct TPPLexerPosition { ... };
 * >> int TPPLexer_SavePosition_(struct TPPLexer *lexer, struct TPPLexerPosition *self);
 * >> void TPPLexer_LoadPosition_(struct TPPLexer *lexer, struct TPPLexerPosition *self);
 * >> int TPPLexer_SavePosition(struct TPPLexerPosition *self);
 * >> void TPPLexer_LoadPosition(struct TPPLexerPosition *self);
 * >> int TPPLexer_ParsePragma_(struct TPPLexer *lexer);
 * >> int TPPLexer_ParseBuiltinPragma_(struct TPPLexer *lexer);
 * >> int TPPLexer_ParsePragma(void);
 * >> int TPPLexer_ParseBuiltinPragma(void);
 *  - These APIs are no longer supported like this in TPP3
 *  - Instead, many individual lexer states can be pushed/popped, including
 *    in a way that makes (different types of) rollback possible:
 *    - tpp_lexer_yieldraw_at()
 *    - tpp_lexer_seek_start()
 *    - tpp_lexer_manualpopfile_start()
 *    - tpp_lexer_tryskip_raw()
 *    - TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK
 */

/* TPPLEXER_FLAG_DIRECTIVE_NOOWN_LF, TPPLEXER_FLAG_COMMENT_NOOWN_LF:
 *  - These flags are no longer supported in TPP3 (in TPP3, line-like comment tokens
 *    *always* include the trailing line-feed (if any) as part of the comment itself)
 *  - If line-feeds have special meaning in your programming language, I recommend
 *    you migrate your code as follows:
 *    ```diff
 *    - case TPP_TOK_LF:
 *    -    if (tok == TPP_TOK_LF)
 *    + case TPP_TOK_LF:
 *    + TPP_CASE_TPP_TOK_COMMENT_LINE
 *    +    if (TPP_TOK_ISLF_OR_COMMENT(tok))
 *    ```
 */

/* TPPLEXER_FLAG_INCLUDESTRING:
 * - No longer supported; in TPP3, #include-strings are parsed without the use
 *   of "tpp_lexer_yieldraw()" (with macro expansion also handled manually)
 *   meaning that there is no need to have some special flag to alter the
 *   parsing of "string" literals for the sake of complying with #include-strings.
 */

/* TPPLEXER_FLAG_EXTENDFILE:
 * - In TPP3, this flag no longer exists: files are allowed to unload any chunk
 *   data that is considered to be unused (and all data that is still considered
 *   to be used will never be unloaded). For this purpose, you have multiple ways
 *   of telling "tpp_file" that you want certain ranges of memory to be kept loaded:
 *   - tpp_file_pushkeep() + tpp_file_setkeep():
 *     Probably the most useful method: allows you to set a position within a file
 *     from which point forth no data can be unloaded.
 *   - tpp_lexer_yieldraw_at():
 *     Not as powerful as tpp_file_setkeep(), but still useful none-the-less, this
 *     method of reading tokens uses your own, custom file-pointer, whilst keeping
 *     the file's *actual* pointer unchanged. As such (since the file's actual
 *     pointer isn't advanced), no file data will be unloaded, however more file
 *     data may be read (whilst keeping your pointer up-to-date) in order to read
 *     the next token resp.
 *   - tpp_string_incref(tpp_file_getchunk()):
 *     If you need to retain access to data that is already loaded into memory,
 *     you can simply increment the file's chunk's reference counter. When TPP3
 *     needs to load more file data into memory, it will no re-use the file's old
 *     chunk, but will instead allocate a new chunk, allowing you to keep using
 *     data form the original chunk without even needing to relocate pointers.
 * - Implementation notes:
 *   Whenever tpp_file_expandchunk() is called, all file data between the current
 *   file's current chunk's start, and the file's position (tpp_file_getpos()) [or
 *   the file's keep-pointer (tpp_file_getkeep())] may be unloaded, such that only
 *   data file data after that position is retained in the file's new chunk (before
 *   being followed by whatever additional data was read from disk)
 */

/* TPPLEXER_FLAG_NO_LEGACY_GUARDS:
 * - In TPP3, detection of #ifndef-style #include-guards cannot be disabled
 *   at runtime, though you can disable for some specific file at the time
 *   when that file is pushed by setting "TPP_FILE_IOFLAGS_NOGUARD".
 * - TPP3 does however let you compile-time disable this feature by building
 *   with "#define TPP_HAVE_IFNDEF_INCLUDE_GUARDS 0"
 */

/* TPPLEXER_FLAG_NO_ENCODING:
 * - TPP3 doesn't let you global-disable unicode support (or for that matter:
 *   disable automatic decoding/detection of input file codecs). However, you
 *   *can* disable codec detection of a text file by changing its encoding
 *   before the first token is read from it:
 *   - TPP_FILE_ENCODING_ASCII:      What "TPPLEXER_FLAG_NO_ENCODING" would have done
 *                                   Treat time data as ASCII and don't detect codec
 *   - TPP_FILE_ENCODING_FORCE_UTF8: Treat time data as UTF-8
 *   - TPP_FILE_ENCODING_UTF16_LE:   Treat time data as UTF-16 (LE)
 *   - TPP_FILE_ENCODING_UTF16_BE:   Treat time data as UTF-16 (BE)
 *   - TPP_FILE_ENCODING_UTF32_LE:   Treat time data as UTF-32 (LE)
 *   - TPP_FILE_ENCODING_UTF32_BE:   Treat time data as UTF-32 (BE)
 */

/* TPPLEXER_FLAG_REEMIT_UNKNOWN_PRAGMA:
 * - TPP3 does no longer support re-emission of unknown #pragma-directives
 * - Instead, you should define you own, custom #pragma-hook (TODO: API for
 *   this) that will be called whenever TPP encounters an unknown pragma.
 */

/* TPPLEXER_FLAG_CHAR_UNSIGNED:
 * - This flag no longer has any meaning. In TPP2, this flag used to control
 *   if calls to "TPP_Atoi()" with TPP_TOK_CHAR-tokens would return with or
 *   without the "TPP_ATOI_UNSIGNED" flag set.
 * - In TPP3, parsing or 'TPP_TOK_CHAR' is delegated to a separate function
 *   "tpp_lexer_parsecharacter_literal()" that expects to be called with a
 *   string-like token. Additionally, the C standard specifies that 'x'-like
 *   character literals have "int" typing, meaning they are never signed.
 */

/* TPPLEXER_FLAG_EOF_ON_PAREN, TPPLexer::l_eof_paren:
 * - No longer needed by TPP3, which handles detection of (...)-pairs by
 *   doing an initial argument-list scan via "tpp_lexer_seekpp_rparen()",
 *   and then re-injecting data via "tpp_file_subtext_setchunk_fromarg()"
 */

/* TPPLEXER_FLAG_RANDOM_INITIALIZED:
 * - TPP3 implements __TPP_RANDOM on a per-lexer basis, thus producing reproducible
 *   results that are based on actual input data, rather than the current time.
 */

/* >Expression evaluation is no longer used everywhere<:
 * - TPP2 used to accept code like:
 *   >> #pragma warning("foo-Wmy-warning"[4:])
 *   This worked because TPP2 used to accept expressions in many more places
 *   than are strictly necessary, primarily since its API design meant that
 *   doing so was simpler than the alternative
 * - Due to its much more modular design, TPP3 no longer does this (instead,
 *   it directly decodes string tokens when it expects to find strings, rather
 *   than parse an expression first, and operate on the resulting string only
 *   after having already done so.
 *   If TPP2 behavior would be preserved in such cases, this would cause lots
 *   of features to implicitly require "tpp_lexer_parseexpr()" (which is quote
 *   the big function that should only be required for "__TPP_EVAL" and "#if")
 * - If your code actually relied on this behavior, the recommendation is to
 *   migrate it as following:
 *   >> #pragma warning(__TPP_EVAL("foo-Wmy-warning"[4:]))
 *   By wrapping the offending expression with "__TPP_EVAL", the construct
 *   will continue to work in TPP2, whilst behaving identically in TPP3, with
 *   the added bonus of making the requirement of expression evaluation more
 *   clear.
 */
/************************************************************************/


#ifndef TPP2_VA_NARGS
#ifndef __has_extension
#define __has_extension(x) 0
#endif /* !__has_extension */

#ifdef __has_known_extension
#if __has_known_extension("va-nargs-in-macros")
#   if !__has_extension("va-nargs-in-macros")
#      pragma extension("va-nargs-in-macros")
#   endif /* !__has_extension("va-nargs-in-macros") */
#   define TPP2_VA_NARGS(...) __VA_NARGS__
#elif __has_known_extension("va-comma-in-macros")
#   if !__has_extension("va-comma-in-macros")
#      pragma extension("va-comma-in-macros")
#   endif /* !__has_extension("va-comma-in-macros") */
#   define TPP2_PP_PRIVATE__VA_NARGS(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,N,...) N
#   define TPP2_VA_NARGS(...) TPP2_PP_PRIVATE__VA_NARGS(__VA_ARGS__ __VA_COMMA__ 59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#elif __has_known_extension("glue-comma-in-macros")
#   if !__has_extension("glue-comma-in-macros")
#      pragma extension("glue-comma-in-macros")
#   endif /* !__has_extension("glue-comma-in-macros") */
#   define TPP2_PP_PRIVATE__VA_NARGS(x,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,N,...) N
#   define TPP2_VA_NARGS(...) TPP2_PP_PRIVATE__VA_NARGS(~,##__VA_ARGS__,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#endif /* ... */
#endif /* __has_known_extension */

#ifndef TPP2_VA_NARGS
#if defined(_MSC_VER) || defined(__INTELLISENSE__)
/* MSVC-Specific (Capable of detecting empty __VA_ARGS__ with ~,__VA_ARGS__) */
#   define TPP2_PP_PRIVATE__VA_NARGS_EXPAND_0(...) __VA_ARGS__
#   define TPP2_PP_PRIVATE__VA_NARGS_EXPAND_1(...) TPP2_PP_PRIVATE__VA_NARGS_EXPAND_0(__VA_ARGS__)
#   define TPP2_PP_PRIVATE__VA_NARGS_I(x,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,N,...) N
#   define TPP2_PP_PRIVATE__VA_NARGS_X(...) (~,__VA_ARGS__,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#   define TPP2_VA_NARGS(...) TPP2_PP_PRIVATE__VA_NARGS_EXPAND_1(TPP2_PP_PRIVATE__VA_NARGS_I TPP2_PP_PRIVATE__VA_NARGS_X(__VA_ARGS__))
#elif __has_extension(tpp_va_comma)
/* TPP-Specific (Capable of detecting empty __VA_ARGS__ with __VA_COMMA__) */
#   define TPP2_PP_PRIVATE__VA_NARGS(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,N,...) N
#   define TPP2_VA_NARGS(...) TPP2_PP_PRIVATE__VA_NARGS(__VA_ARGS__ __VA_COMMA__ 59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#elif defined(__GNUC__) || (defined(__TPP_VERSION__) && __TPP_VERSION__ >= 103)
/* GCC-Specific (Capable of detecting empty __VA_ARGS__ with ,##__VA_ARGS__) */
#   define TPP2_PP_PRIVATE__VA_NARGS(x,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,N,...) N
#   define TPP2_VA_NARGS(...) TPP2_PP_PRIVATE__VA_NARGS(~,##__VA_ARGS__,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#else /* ... */
/* Check if __VA_OPT__ is supported, and if it is, use it for a standard, 0-arg-capable implementation.
 * Note that we can check for `__VA_OPT__' without having to rely on any feature-test macros, since its
 * behavior is well-defined even on preprocessors that don't support it! */
#define TPP2_PP_PRIVATE__TEST_VA_OPT2(a, b, ...) b
#define TPP2_PP_PRIVATE__TEST_VA_OPT(...) TPP2_PP_PRIVATE__TEST_VA_OPT2(__VA_OPT__(,) 1, 0)
#if TPP2_PP_PRIVATE__TEST_VA_OPT(~)
/* STD-Implementation (Capable of detecting empty __VA_ARGS__ with __VA_OPT__) */
#   define TPP2_PP_PRIVATE__VA_NARGS(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,N,...) N
#   define TPP2_VA_NARGS(...) TPP2_PP_PRIVATE__VA_NARGS(__VA_ARGS__ __VA_OPT__(,) 59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#else /* TPP2_PP_PRIVATE__TEST_VA_OPT(~) */
/* STD-Implementation (Not Capable of detecting empty __VA_ARGS__) */
#   define TPP2_PP_PRIVATE__VA_NARGS_EXPAND_0(...) __VA_ARGS__
#   define TPP2_PP_PRIVATE__VA_NARGS2(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,N,...) N
#   define TPP2_PP_PRIVATE__VA_NARGS(...) TPP2_PP_PRIVATE__VA_NARGS2(__VA_ARGS__,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1)
#   define TPP2_VA_NARGS(...) TPP2_PP_PRIVATE__VA_NARGS_EXPAND_0(TPP2_PP_PRIVATE__VA_NARGS(__VA_ARGS__))
#endif /* !TPP2_PP_PRIVATE__TEST_VA_OPT(~) */
#undef TPP2_PP_PRIVATE__TEST_VA_OPT2
#undef TPP2_PP_PRIVATE__TEST_VA_OPT
#endif /* !... */
#endif /* !TPP2_VA_NARGS */
#endif /* !TPP2_VA_NARGS */


#ifndef TPP_CONFIG_MINMACRO
#define TPP_CONFIG_MINMACRO 0
#endif /* !TPP_CONFIG_MINMACRO */
#undef TPP_CONFIG_GCCFUNC
#define TPP_CONFIG_GCCFUNC 0 /* Default used to be "1", but builtin functions are no longer supported by TPP3 */
#undef TPP_CONFIG_MINGCCFUNC
#define TPP_CONFIG_MINGCCFUNC 2 /* Default used to be "0", but builtin functions are no longer supported by TPP3 */



#ifndef TPP2_NO_AUTOCONFIGURE_TPP3 /* Define this to prevent this header from configuring TPP3 */
/************************************************************************/
/* ALTER CONFIGURATION                                                  */
/************************************************************************/

/* TPP2 configuration macros */

#ifndef TPP_CONFIG_FEATURE_TRIGRAPHS_DEFAULT
#define TPP_CONFIG_FEATURE_TRIGRAPHS_DEFAULT 0
#endif /* !TPP_CONFIG_FEATURE_TRIGRAPHS_DEFAULT */
#ifndef TPP_CONFIG_FEATURE_DIGRAPHS_DEFAULT
#define TPP_CONFIG_FEATURE_DIGRAPHS_DEFAULT 1
#endif /* !TPP_CONFIG_FEATURE_DIGRAPHS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_GCC_VA_ARGS_DEFAULT
#define TPP_CONFIG_EXTENSION_GCC_VA_ARGS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_GCC_VA_ARGS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_GCC_VA_COMMA_DEFAULT
#define TPP_CONFIG_EXTENSION_GCC_VA_COMMA_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_GCC_VA_COMMA_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_GCC_IFELSE_DEFAULT
#define TPP_CONFIG_EXTENSION_GCC_IFELSE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_GCC_IFELSE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_VA_COMMA_DEFAULT
#define TPP_CONFIG_EXTENSION_VA_COMMA_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_VA_COMMA_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_VA_OPT_DEFAULT
#define TPP_CONFIG_EXTENSION_VA_OPT_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_VA_OPT_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_VA_NARGS_DEFAULT
#define TPP_CONFIG_EXTENSION_VA_NARGS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_VA_NARGS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_VA_ARGS_DEFAULT
#define TPP_CONFIG_EXTENSION_VA_ARGS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_VA_ARGS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_STR_E_DEFAULT
#define TPP_CONFIG_EXTENSION_STR_E_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_STR_E_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_ALTMAC_DEFAULT
#define TPP_CONFIG_EXTENSION_ALTMAC_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_ALTMAC_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_RECMAC_DEFAULT
#define TPP_CONFIG_EXTENSION_RECMAC_DEFAULT 0
#endif /* !TPP_CONFIG_EXTENSION_RECMAC_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_ARGSPACE_DEFAULT
#define TPP_CONFIG_EXTENSION_ARGSPACE_DEFAULT 0
#endif /* !TPP_CONFIG_EXTENSION_ARGSPACE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_BININTEGRAL_DEFAULT
#define TPP_CONFIG_EXTENSION_BININTEGRAL_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_BININTEGRAL_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_MSVC_PRAGMA_DEFAULT
#define TPP_CONFIG_EXTENSION_MSVC_PRAGMA_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_MSVC_PRAGMA_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_STRINGOPS_DEFAULT
#define TPP_CONFIG_EXTENSION_STRINGOPS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_STRINGOPS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_HASH_AT_DEFAULT
#define TPP_CONFIG_EXTENSION_HASH_AT_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_HASH_AT_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_HASH_XCLAIM_DEFAULT
#define TPP_CONFIG_EXTENSION_HASH_XCLAIM_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_HASH_XCLAIM_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_WARNING_DEFAULT
#define TPP_CONFIG_EXTENSION_WARNING_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_WARNING_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_SHEBANG_DEFAULT
#define TPP_CONFIG_EXTENSION_SHEBANG_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_SHEBANG_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_INCLUDE_NEXT_DEFAULT
#define TPP_CONFIG_EXTENSION_INCLUDE_NEXT_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_INCLUDE_NEXT_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_IMPORT_DEFAULT
#define TPP_CONFIG_EXTENSION_IMPORT_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_IMPORT_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_IDENT_SCCS_DEFAULT
#define TPP_CONFIG_EXTENSION_IDENT_SCCS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_IDENT_SCCS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_BASEFILE_DEFAULT
#define TPP_CONFIG_EXTENSION_BASEFILE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_BASEFILE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_INCLUDE_LEVEL_DEFAULT
#define TPP_CONFIG_EXTENSION_INCLUDE_LEVEL_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_INCLUDE_LEVEL_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_COUNTER_DEFAULT
#define TPP_CONFIG_EXTENSION_COUNTER_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_COUNTER_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_CLANG_FEATURES_DEFAULT
#define TPP_CONFIG_EXTENSION_CLANG_FEATURES_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_CLANG_FEATURES_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_HAS_INCLUDE_DEFAULT
#define TPP_CONFIG_EXTENSION_HAS_INCLUDE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_HAS_INCLUDE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_LXOR_DEFAULT
#define TPP_CONFIG_EXTENSION_LXOR_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_LXOR_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_DATEUTILS_DEFAULT
#define TPP_CONFIG_EXTENSION_DATEUTILS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_DATEUTILS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TIMEUTILS_DEFAULT
#define TPP_CONFIG_EXTENSION_TIMEUTILS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TIMEUTILS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TIMESTAMP_DEFAULT
#define TPP_CONFIG_EXTENSION_TIMESTAMP_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TIMESTAMP_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_COLUMN_DEFAULT
#define TPP_CONFIG_EXTENSION_COLUMN_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_COLUMN_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_EVAL_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_EVAL_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_EVAL_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_UNIQUE_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_UNIQUE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_UNIQUE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_LOAD_FILE_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_LOAD_FILE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_LOAD_FILE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_COUNTER_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_COUNTER_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_COUNTER_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_RANDOM_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_RANDOM_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_RANDOM_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_PACK_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_STR_PACK_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_PACK_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_SIZE_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_STR_SIZE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_SIZE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_IDENTIFIER_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_IDENTIFIER_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_IDENTIFIER_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_ASSERTIONS_DEFAULT
#define TPP_CONFIG_EXTENSION_ASSERTIONS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_ASSERTIONS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_CANONICAL_HEADERS_DEFAULT
#define TPP_CONFIG_EXTENSION_CANONICAL_HEADERS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_CANONICAL_HEADERS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES_DEFAULT
#define TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_MSVC_FIXED_INT_DEFAULT
#define TPP_CONFIG_EXTENSION_MSVC_FIXED_INT_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_MSVC_FIXED_INT_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED_DEFAULT
#define TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR_DEFAULT
#define TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_EXTENDED_IDENTS_DEFAULT
#define TPP_CONFIG_EXTENSION_EXTENDED_IDENTS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_EXTENDED_IDENTS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO_DEFAULT
#define TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO_DEFAULT 0
#endif /* !TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO_DEFAULT */

#ifndef TPP_CONFIG_FEATURE_TRIGRAPHS
#define TPP_CONFIG_FEATURE_TRIGRAPHS TPP_CONF_MAKEEXT(TPP_CONFIG_FEATURE_TRIGRAPHS_DEFAULT)
#endif /* !TPP_CONFIG_FEATURE_TRIGRAPHS */
#ifndef TPP_CONFIG_FEATURE_DIGRAPHS
#define TPP_CONFIG_FEATURE_DIGRAPHS TPP_CONF_MAKEEXT(TPP_CONFIG_FEATURE_DIGRAPHS_DEFAULT)
#endif /* !TPP_CONFIG_FEATURE_DIGRAPHS */
#ifndef TPP_CONFIG_EXTENSION_GCC_VA_ARGS
#define TPP_CONFIG_EXTENSION_GCC_VA_ARGS TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_GCC_VA_ARGS_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_GCC_VA_ARGS */
#ifndef TPP_CONFIG_EXTENSION_GCC_VA_COMMA
#define TPP_CONFIG_EXTENSION_GCC_VA_COMMA TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_GCC_VA_COMMA_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_GCC_VA_COMMA */
#ifndef TPP_CONFIG_EXTENSION_GCC_IFELSE
#define TPP_CONFIG_EXTENSION_GCC_IFELSE TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_GCC_IFELSE_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_GCC_IFELSE */
#ifndef TPP_CONFIG_EXTENSION_VA_COMMA
#define TPP_CONFIG_EXTENSION_VA_COMMA TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_VA_COMMA_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_VA_COMMA */
#ifndef TPP_CONFIG_EXTENSION_VA_OPT
#define TPP_CONFIG_EXTENSION_VA_OPT TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_VA_OPT_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_VA_OPT */
#ifndef TPP_CONFIG_EXTENSION_VA_NARGS
#define TPP_CONFIG_EXTENSION_VA_NARGS TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_VA_NARGS_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_VA_NARGS */
#ifndef TPP_CONFIG_EXTENSION_VA_ARGS
#define TPP_CONFIG_EXTENSION_VA_ARGS TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_VA_ARGS_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_VA_ARGS */
#ifndef TPP_CONFIG_EXTENSION_STR_E
#define TPP_CONFIG_EXTENSION_STR_E TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_STR_E_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_STR_E */
#ifndef TPP_CONFIG_EXTENSION_ALTMAC
#define TPP_CONFIG_EXTENSION_ALTMAC TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_ALTMAC_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_ALTMAC */
#ifndef TPP_CONFIG_EXTENSION_RECMAC
#define TPP_CONFIG_EXTENSION_RECMAC TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_RECMAC_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_RECMAC */
#ifndef TPP_CONFIG_EXTENSION_ARGSPACE
#define TPP_CONFIG_EXTENSION_ARGSPACE TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_ARGSPACE_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_ARGSPACE */
#ifndef TPP_CONFIG_EXTENSION_BININTEGRAL
#define TPP_CONFIG_EXTENSION_BININTEGRAL TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_BININTEGRAL_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_BININTEGRAL */
#ifndef TPP_CONFIG_EXTENSION_MSVC_PRAGMA
#define TPP_CONFIG_EXTENSION_MSVC_PRAGMA TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_MSVC_PRAGMA_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_MSVC_PRAGMA */
#ifndef TPP_CONFIG_EXTENSION_STRINGOPS
#define TPP_CONFIG_EXTENSION_STRINGOPS TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_STRINGOPS_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_STRINGOPS */
#ifndef TPP_CONFIG_EXTENSION_HASH_AT
#define TPP_CONFIG_EXTENSION_HASH_AT TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_HASH_AT_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_HASH_AT */
#ifndef TPP_CONFIG_EXTENSION_HASH_XCLAIM
#define TPP_CONFIG_EXTENSION_HASH_XCLAIM TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_HASH_XCLAIM_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_HASH_XCLAIM */
#ifndef TPP_CONFIG_EXTENSION_WARNING
#define TPP_CONFIG_EXTENSION_WARNING TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_WARNING_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_WARNING */
#ifndef TPP_CONFIG_EXTENSION_SHEBANG
#define TPP_CONFIG_EXTENSION_SHEBANG TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_SHEBANG_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_SHEBANG */
#ifndef TPP_CONFIG_EXTENSION_INCLUDE_NEXT
#define TPP_CONFIG_EXTENSION_INCLUDE_NEXT TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_INCLUDE_NEXT_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_INCLUDE_NEXT */
#ifndef TPP_CONFIG_EXTENSION_IMPORT
#define TPP_CONFIG_EXTENSION_IMPORT TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_IMPORT_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_IMPORT */
#ifndef TPP_CONFIG_EXTENSION_IDENT_SCCS
#define TPP_CONFIG_EXTENSION_IDENT_SCCS TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_IDENT_SCCS_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_IDENT_SCCS */
#ifndef TPP_CONFIG_EXTENSION_BASEFILE
#define TPP_CONFIG_EXTENSION_BASEFILE TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_BASEFILE_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_BASEFILE */
#ifndef TPP_CONFIG_EXTENSION_INCLUDE_LEVEL
#define TPP_CONFIG_EXTENSION_INCLUDE_LEVEL TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_INCLUDE_LEVEL_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_INCLUDE_LEVEL */
#ifndef TPP_CONFIG_EXTENSION_COUNTER
#define TPP_CONFIG_EXTENSION_COUNTER TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_COUNTER_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_COUNTER */
#ifndef TPP_CONFIG_EXTENSION_CLANG_FEATURES
#define TPP_CONFIG_EXTENSION_CLANG_FEATURES TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_CLANG_FEATURES_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_CLANG_FEATURES */
#ifndef TPP_CONFIG_EXTENSION_HAS_INCLUDE
#define TPP_CONFIG_EXTENSION_HAS_INCLUDE TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_HAS_INCLUDE_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_HAS_INCLUDE */
#ifndef TPP_CONFIG_EXTENSION_LXOR
#define TPP_CONFIG_EXTENSION_LXOR TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_LXOR_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_LXOR */
#ifndef TPP_CONFIG_EXTENSION_DATEUTILS
#define TPP_CONFIG_EXTENSION_DATEUTILS TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_DATEUTILS_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_DATEUTILS */
#ifndef TPP_CONFIG_EXTENSION_TIMEUTILS
#define TPP_CONFIG_EXTENSION_TIMEUTILS TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TIMEUTILS_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TIMEUTILS */
#ifndef TPP_CONFIG_EXTENSION_TIMESTAMP
#define TPP_CONFIG_EXTENSION_TIMESTAMP TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TIMESTAMP_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TIMESTAMP */
#ifndef TPP_CONFIG_EXTENSION_COLUMN
#define TPP_CONFIG_EXTENSION_COLUMN TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_COLUMN_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_COLUMN */
#ifndef TPP_CONFIG_EXTENSION_TPP_EVAL
#define TPP_CONFIG_EXTENSION_TPP_EVAL TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TPP_EVAL_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TPP_EVAL */
#ifndef TPP_CONFIG_EXTENSION_TPP_UNIQUE
#define TPP_CONFIG_EXTENSION_TPP_UNIQUE TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TPP_UNIQUE_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TPP_UNIQUE */
#ifndef TPP_CONFIG_EXTENSION_TPP_LOAD_FILE
#define TPP_CONFIG_EXTENSION_TPP_LOAD_FILE TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TPP_LOAD_FILE_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TPP_LOAD_FILE */
#ifndef TPP_CONFIG_EXTENSION_TPP_COUNTER
#define TPP_CONFIG_EXTENSION_TPP_COUNTER TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TPP_COUNTER_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TPP_COUNTER */
#ifndef TPP_CONFIG_EXTENSION_TPP_RANDOM
#define TPP_CONFIG_EXTENSION_TPP_RANDOM TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TPP_RANDOM_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TPP_RANDOM */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE
#define TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR
#define TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_SIZE
#define TPP_CONFIG_EXTENSION_TPP_STR_SIZE TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TPP_STR_SIZE_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_SIZE */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_PACK
#define TPP_CONFIG_EXTENSION_TPP_STR_PACK TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TPP_STR_PACK_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_PACK */
#ifndef TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS
#define TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS */
#ifndef TPP_CONFIG_EXTENSION_TPP_IDENTIFIER
#define TPP_CONFIG_EXTENSION_TPP_IDENTIFIER TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TPP_IDENTIFIER_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TPP_IDENTIFIER */
#ifndef TPP_CONFIG_EXTENSION_ASSERTIONS
#define TPP_CONFIG_EXTENSION_ASSERTIONS TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_ASSERTIONS_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_ASSERTIONS */
#ifndef TPP_CONFIG_EXTENSION_CANONICAL_HEADERS
#define TPP_CONFIG_EXTENSION_CANONICAL_HEADERS TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_CANONICAL_HEADERS_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_CANONICAL_HEADERS */
#ifndef TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES
#define TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES */
#ifndef TPP_CONFIG_EXTENSION_MSVC_FIXED_INT
#define TPP_CONFIG_EXTENSION_MSVC_FIXED_INT TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_MSVC_FIXED_INT_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_MSVC_FIXED_INT */
#ifndef TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED
#define TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED */
#ifndef TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR
#define TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR */
#ifndef TPP_CONFIG_EXTENSION_EXTENDED_IDENTS
#define TPP_CONFIG_EXTENSION_EXTENDED_IDENTS TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_EXTENDED_IDENTS_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_EXTENDED_IDENTS */
#ifndef TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO
#define TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO TPP_CONF_MAKEEXT(TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO_DEFAULT)
#endif /* !TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO */


#ifdef TPP_CONFIG_DEBUG
#define TPP_DEBUG TPP_CONFIG_DEBUG
#endif /* TPP_CONFIG_DEBUG */


/* Inherit legacy tabsize configuration */
#ifndef TPPLEXER_DEFAULT_TABSIZE
#if (defined(_WIN16) || defined(WIN16) || \
     defined(_WIN32) || defined(WIN32) || \
     defined(_WIN64) || defined(WIN64) || \
     defined(__WIN32__) || defined(__TOS_WIN__))
#define TPPLEXER_DEFAULT_TABSIZE 4 /* Default tab size (used for `__COLUMN__' and in error messages). */
#else /* Windows... */
#define TPPLEXER_DEFAULT_TABSIZE 8 /* Default tab size (used for `__COLUMN__' and in error messages). */
#endif /* Unix... */
#endif /* !TPPLEXER_DEFAULT_TABSIZE */
#undef TPP_TABSIZE
#define TPP_TABSIZE TPPLEXER_DEFAULT_TABSIZE

#if 0 /* TODO */
//#ifndef TPPLEXER_DEFAULT_LIMIT_MREC
//#define TPPLEXER_DEFAULT_LIMIT_MREC 512 /* Even when generated text differs from previous version, don't allow more self-recursion per macro than this. */
//#endif /* !TPPLEXER_DEFAULT_LIMIT_MREC */
//#ifndef TPPLEXER_DEFAULT_LIMIT_INCL
//#define TPPLEXER_DEFAULT_LIMIT_INCL 64  /* User attempts to #include a file more often that file will fail with an error message. */
//#endif /* !TPPLEXER_DEFAULT_LIMIT_INCL */
#endif

/* Inherit legacy error limit configuration */
#ifndef TPPLEXER_DEFAULT_LIMIT_ECNT
#define TPPLEXER_DEFAULT_LIMIT_ECNT 16
#endif /* !TPPLEXER_DEFAULT_LIMIT_ECNT */
#undef TPP_ERROR_LIMIT
#define TPP_ERROR_LIMIT TPPLEXER_DEFAULT_LIMIT_ECNT

/* Inherit legacy non-blocking I/O configuration */
#undef TPP_HAVE_FILE_NONBLOCK
#ifdef TPP_CONFIG_NONBLOCKING_IO
#define TPP_HAVE_FILE_NONBLOCK (-1)
#else /* TPP_CONFIG_NONBLOCKING_IO */
#define TPP_HAVE_FILE_NONBLOCK 0
#endif /* !TPP_CONFIG_NONBLOCKING_IO */

#define TPP_PROFILE TPP_PROFILE_MINIMAL /* Disable anything not otherwise explicitly configured */

#define TPP_HAVE_TPP2_COMPAT           1 /* Enable some extra TPP2 compatibility tweaks */
#define TPP_BUILDING                   1 /* Not actually true, but needed to prevent internals from being escaped (TPP3 doesn't expose internals by default anymore) */
#define TPP_HAVE_UNICODE               1 /* Always enable unicode support */
#define TPP_HAVE_STRERROR              0
#define TPP_HAVE_STRTOKENID            0
#define TPP_HAVE_KEYWORD_USERDATA      1 /* To emulate "kr_user" */
#define TPP_HAVE_KEYWORD_ASSTRING      0 /* Not needed for anything */
#define TPP_HAVE_EXTENSIONS            1
#define TPP_HAVE_EXTENSIONS_PUSH_POP   1
#define TPP_HAVE_WARNINGS              1
#define TPP_HAVE_WARNINGS_PUSH_POP     1
#define TPP_HAVE_WARNING_NUMBERS       1 /* Enable this, even though TPP2 had different warning numbers... */
#define TPP_HAVE_WARNING_ERROR         1
#define TPP_HAVE_WARNING_SUPPRESS      1
#define TPP_HAVE_WARNING_DEFAULT       1
#define TPP_HAVE_FILE_NOCLOSE          0 /* TTP2 didn't have this */
#define TPP_HAVE_FILE_NOKWD            0 /* TTP2 didn't have this */
#define TPP_COMMON_HAVE_TPP_TOK        0 /* We want to configure tokens individually */

#ifdef TPP_CONFIG_NO_PRECACHE_TEXTLINES
#define TPP_HAVE_FILE_LC_CACHE 0
#else /* TPP_CONFIG_NO_PRECACHE_TEXTLINES */
#define TPP_HAVE_FILE_LC_CACHE 1
#endif /* !TPP_CONFIG_NO_PRECACHE_TEXTLINES */


#ifdef TPP_CONFIG_RAW_STRING_LITERALS
#define TPP2_HAVE_RAW_STRING_LITERALS 1
#else /* TPP_CONFIG_RAW_STRING_LITERALS */
#define TPP2_HAVE_RAW_STRING_LITERALS 0
#endif /* !TPP_CONFIG_RAW_STRING_LITERALS */

#define TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL \
	TPP2_HAVE_RAW_STRING_LITERALS /* Specifically for deemon, TPP2 had (really badly integrated) deemon-style r"raw string" support */
#define TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL \
	TPP2_HAVE_RAW_STRING_LITERALS /* *ditto* */

#define TPP_HAVE_TPP_TOK_LF                         TPP_CONF_FEAT0 /* Configurable, default=false (TPP2 used to configure this via "TPPLEXER_FLAG_WANTLF"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_LF)") */
#define TPP_HAVE_TPP_TOK_SPACE                      TPP_CONF_FEAT0 /* Configurable, default=false (TPP2 used to configure this via "TPPLEXER_FLAG_WANTSPACE"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_SPACE)") */
#define TPP_HAVE_TPP_TOK_COMMENT                    TPP_CONF_FEAT0 /* Configurable, default=false (TPP2 used to configure this via "TPPLEXER_FLAG_WANTCOMMENTS"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_COMMENT)") */
#define TPP_HAVE_TPP_TOK_CXX_COMMENT                TPP_CONF_FEAT1 /* Configurable, default=true  (TPP2 used to configure this via "TPPLEXER_TOKEN_CPP_COMMENT"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_CXX_COMMENT)") */
#define TPP_HAVE_TPP_TOK_C_COMMENT                  TPP_CONF_FEAT1 /* Configurable, default=true  (TPP2 used to configure this via "TPPLEXER_TOKEN_C_COMMENT"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_C_COMMENT)") */
#define TPP_HAVE_TPP_TOK_PASCAL_COMMENT             0              /* TPP2 only recognized C/C++-like comments */
#define TPP_HAVE_TPP_TOK_SHELL_COMMENT              TPP_CONF_FEAT0 /* Configurable, default=false (TPP2 used to configure this via "TPPLEXER_FLAG_ASM_COMMENTS"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_SHELL_COMMENT)") */
#define TPP_HAVE_TPP_TOK_ASM_COMMENT                0              /* TPP2 only recognized C/C++-like comments */
#define TPP_HAVE_TPP_TOK_SQL_COMMENT                0              /* TPP2 only recognized C/C++-like comments */
#define TPP_HAVE_TPP_TOK_DOLLAR                     TPP_CONF_FEAT0 /* "$" Configurable, default=false (TPP2 used to configure this via "TPPLEXER_TOKEN_DOLLAR"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_DOLLAR)") */
#define TPP_HAVE_TPP_TOK_INT                        1              /* ... */
#define TPP_HAVE_TPP_TOK_FLOAT                      1              /* ... */
#define TPP_HAVE_TPP_TOK_STRING                     1              /* TPP2 only supported C character/string literals */
#define TPP_HAVE_TPP_TOK_CHAR                       1              /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL     0              /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL    0              /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL    0              /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL   0              /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL   0              /* *ditto* */
#define TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL       0              /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL       0              /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL      0              /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL      0              /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL     0              /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL     0              /* *ditto* */
#define TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL         0              /* *ditto* */
#define TPP_HAVE_STRING_ALLOW_MULTILINE             TPP_CONF_FEAT1 /* Configurable, default=true (TPP2 used to configure this via "TPPLEXER_FLAG_TERMINATE_STRING_LF"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_STRING_ALLOW_MULTILINE)") */
#define TPP_HAVE_STRING_WARN_MULTILINE              0              /* TPP2 offered no such warning */
#define TPP_HAVE_STRING_AUTO_CONCAT                 1              /* TPP2 had this always-enabled */
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE              1              /* "<<" */
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE              1              /* ">>" */
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL                1              /* "==" */
#define TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL              1              /* "!=" */
#define TPP_HAVE_TPP_TOK_RANGLE_EQUAL               1              /* ">=" */
#define TPP_HAVE_TPP_TOK_LANGLE_EQUAL               1              /* "<=" */
#define TPP_HAVE_TPP_TOK_DOT_DOT_DOT                1              /* "..." */
#define TPP_HAVE_TPP_TOK_PLUS_EQUAL                 1              /* "+=" */
#define TPP_HAVE_TPP_TOK_MINUS_EQUAL                1              /* "-=" */
#define TPP_HAVE_TPP_TOK_STAR_EQUAL                 1              /* "*=" */
#define TPP_HAVE_TPP_TOK_SLASH_EQUAL                1              /* "/=" */
#define TPP_HAVE_TPP_TOK_PERCENT_EQUAL              1              /* "%=" */
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL        1              /* "<<=" */
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL        1              /* ">>=" */
#define TPP_HAVE_TPP_TOK_AMP_EQUAL                  1              /* "&=" */
#define TPP_HAVE_TPP_TOK_PIPE_EQUAL                 1              /* "|=" */
#define TPP_HAVE_TPP_TOK_HAT_EQUAL                  1              /* "^=" */
#define TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL            TPP_CONF_FEAT1 /* "**="  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_STARSTAR"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL)") */
#define TPP_HAVE_TPP_TOK_AT_EQUAL                   TPP_CONF_FEAT1 /* "@="   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ATEQUAL"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_AT_EQUAL)") */
#define TPP_HAVE_TPP_TOK_POUND_POUND                1              /* "##" */
#define TPP_HAVE_TPP_TOK_AMP_AMP                    1              /* "&&" */
#define TPP_HAVE_TPP_TOK_PIPE_PIPE                  1              /* "||" */
#define TPP_HAVE_TPP_TOK_HAT_HAT                    TPP_CONF_FEAT1 /* "^^"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ROOFROOF"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_HAT_HAT)") */
#define TPP_HAVE_TPP_TOK_PLUS_PLUS                  1              /* "++" */
#define TPP_HAVE_TPP_TOK_MINUS_MINUS                1              /* "--" */
#define TPP_HAVE_TPP_TOK_STAR_STAR                  TPP_CONF_FEAT1 /* "**"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_STARSTAR"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_STAR_STAR)") */
#define TPP_HAVE_TPP_TOK_TILDE_TILDE                TPP_CONF_FEAT1 /* "~~"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_TILDETILDE"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_TILDE_TILDE)") */
#define TPP_HAVE_TPP_TOK_TILDE_EQUAL                1              /* "~=" */
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE               TPP_CONF_FEAT1 /* "->"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ARROW"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_MINUS_RANGLE)") */
#define TPP_HAVE_TPP_TOK_COLON_EQUAL                TPP_CONF_FEAT1 /* ":="   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_COLONASSIGN" / "TPPLEXER_TOKEN_COLLONASSIGN"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_COLON_EQUAL)") */
#define TPP_HAVE_TPP_TOK_COLON_COLON                TPP_CONF_FEAT1 /* "::"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_COLONCOLON" / "TPPLEXER_TOKEN_COLLONCOLLON"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_COLON_COLON)") */
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR          TPP_CONF_FEAT1 /* "->*"  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ARROWSTAR"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR)") */
#define TPP_HAVE_TPP_TOK_DOT_STAR                   TPP_CONF_FEAT1 /* ".*"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_DOTSTAR"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_DOT_STAR)") */
#define TPP_HAVE_TPP_TOK_DOT_DOT                    TPP_CONF_FEAT1 /* ".."   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_DOTDOT"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_DOT_DOT)") */
#define TPP_HAVE_TPP_TOK_LANGLE_RANGLE              TPP_CONF_FEAT1 /* "<>"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_LOGT"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_LANGLE_RANGLE)") */
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE       TPP_CONF_FEAT1 /* "<<<"  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ANGLE3"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE)") */
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE       TPP_CONF_FEAT1 /* ">>>"  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ANGLE3"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE)") */
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL TPP_CONF_FEAT1 /* "<<<=" Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ANGLE3_EQUAL"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)") */
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL TPP_CONF_FEAT1 /* ">>>=" Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ANGLE3_EQUAL"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)") */
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL          TPP_CONF_FEAT1 /* "==="  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUAL3"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL)") */
#define TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL        TPP_CONF_FEAT1 /* "!=="  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUAL3"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL)") */
#define TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM            0              /* "!!"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_QMARK_QMARK                TPP_CONF_FEAT1 /* "??"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_QMARK_QMARK"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_SLASH_SLASH                0              /* "//"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL          0              /* "//="  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_QMARK_EQUAL                0              /* "?="   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_RANGLE_LANGLE              0              /* "><"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_PLUS                 TPP_CONF_FEAT1 /* "=+"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_PLUS)") */
#define TPP_HAVE_TPP_TOK_EQUAL_MINUS                TPP_CONF_FEAT1 /* "=-"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_MINUS)") */
#define TPP_HAVE_TPP_TOK_EQUAL_STAR                 TPP_CONF_FEAT1 /* "=*"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_STAR)") */
#define TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR            TPP_CONF_FEAT1 /* "=**"  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP" + "TPPLEXER_TOKEN_STARSTAR"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_STAR_STAR)") */
#define TPP_HAVE_TPP_TOK_EQUAL_SLASH                TPP_CONF_FEAT1 /* "=/"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_SLASH)") */
#define TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH          0              /* "=//"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_PERCENT              TPP_CONF_FEAT1 /* "=%"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_PERCENT)") */
#define TPP_HAVE_TPP_TOK_EQUAL_AMP                  TPP_CONF_FEAT1 /* "=&"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_AMP)") */
#define TPP_HAVE_TPP_TOK_EQUAL_PIPE                 TPP_CONF_FEAT1 /* "=|"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_PIPE)") */
#define TPP_HAVE_TPP_TOK_EQUAL_HAT                  TPP_CONF_FEAT1 /* "=^"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_HAT)") */
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE               0              /* "=<"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE        TPP_CONF_FEAT1 /* "=<<"  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE)") */
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE TPP_CONF_FEAT1 /* "=<<<" Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP" + "TPPLEXER_TOKEN_ANGLE3_EQUAL"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE)") */
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE               0              /* "=>"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE        TPP_CONF_FEAT1 /* "=>>"  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE)") */
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE TPP_CONF_FEAT1 /* "=>>>" Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP" + "TPPLEXER_TOKEN_ANGLE3_EQUAL"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE)") */
#define TPP_HAVE_TPP_TOK_EQUAL_AT                   TPP_CONF_FEAT1 /* "=@"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP" + "TPPLEXER_TOKEN_ATEQUAL"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_AT)") */
#define TPP_HAVE_TPP_TOK_EQUAL_TILDE                0              /* "=~"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_COLON                0              /* "=:"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM              0              /* "=!"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM        0              /* "==!"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_QMARK                0              /* "=?"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_LANGLE_MINUS               0              /* "<-"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS          0              /* "*<-"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_STAR_DOT                   0              /* "*."   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_MINUS_LANGLE               0              /* "-<"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_RANGLE_MINUS               0              /* ">-"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE        0              /* "<=>"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE        0              /* "<->"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE        0              /* ">=<"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE        0              /* ">-<"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE        0              /* "<=<"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE        0              /* "<-<"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE        0              /* ">=>"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE        0              /* ">->"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_PERCENT_PERCENT            0              /* "%%"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL      0              /* "%%="  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT      0              /* "=%%"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_AT_AT                      0              /* "@@"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_AT_AT_EQUAL                0              /* "@@="  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_AT_AT                0              /* "=@@"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE        0              /* "->>"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE 0              /* "->>>" Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE        0              /* "-<<"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE 0              /* "-<<<" Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS        0              /* ">>-"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS 0              /* ">>>-" Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS        0              /* "<<-"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS 0              /* "<<<-" Completely unknown to TPP2 */

/* Features... */
#define TPP_HAVE_BSE                                  1 /* TPP2 didn't even let you disable this */
#define TPP_HAVE_BSE_WHITESPACE                       0 /* TPP2 didn't support whitespace between \ and LF */
#define TPP_HAVE_ESCAPE_IN_IDENTIFIERS                0 /* TPP2 didn't support \uXXXX or \UXXXXXXXX characters in identifiers */
#define TPP_HAVE_TRIGRAPHS                            TPP_CONFIG_FEATURE_TRIGRAPHS
#define TPP_HAVE_DIGRAPHS                             TPP_CONFIG_FEATURE_DIGRAPHS
#define TPP_HAVE_ESCAPE_E_IN_STRINGS                  TPP_CONFIG_EXTENSION_STR_E             /* Support for "\e" (for U+001B) escape sequences */
#define TPP_HAVE_ESCAPE_S_IN_STRINGS                  0                                      /* Support for "\s" (for U+0020) escape sequences */
#define TPP_HAVE_CPP_DIRECTIVES                       1                                      /* Specifies if *any* CPP directives are supported */
#define TPP_HAVE_CPP_MACROS                           1                                      /* Support for C-style macros */
#define TPP_HAVE_CPP_EXCLAIM                          TPP_CONFIG_EXTENSION_SHEBANG           /* Support for: #!foobar  (comments) */
#define TPP_HAVE_CPP_BLANK                            1                                      /* Support for: #  (blank line) */
#define TPP_HAVE_CPP_DIGIT_LINE                       1                                      /* Support for: # 42 ...  (similar to #line) */
#define TPP_HAVE_CPP_LINE                             1                                      /* Support for: #line ... */
#define TPP_HAVE_CPP_INCLUDE                          1                                      /* Support for: #include */
#define TPP_HAVE_CPP_INCLUDE_NEXT                     TPP_CONFIG_EXTENSION_INCLUDE_NEXT      /* Support for: #include_next */
#define TPP_HAVE_CPP_IMPORT                           TPP_CONFIG_EXTENSION_IMPORT            /* Support for: #import */
#define TPP_HAVE_CPP_IF_ELSE_ENDIF                    1                                      /* Support for: #if, #ifdef, #ifndef, #elif, #elifdef, #elifndef, #else, #endif */
#define TPP_HAVE_CPP_DEFINE                           1                                      /* Support for: #define, #undef */
#define TPP_HAVE_CPP_ASSERT                           TPP_CONFIG_EXTENSION_ASSERTIONS        /* Support for: #assert, #unassert */
#define TPP_HAVE_CPP_ERROR                            1                                      /* Support for: #error */
#define TPP_HAVE_CPP_WARNING                          TPP_CONFIG_EXTENSION_WARNING           /* Support for: #warning */
#define TPP_HAVE_CPP_IDENT_SCCS                       TPP_CONFIG_EXTENSION_IDENT_SCCS        /* Support for: #ident, #sccs */
#define TPP_HAVE_CPP_PRAGMA                           1                                      /* Support for: #pragma */
#define TPP_HAVE_CPP_EMBED                            0                                      /* Support for: #embed */
#define TPP_HAVE_MACRO__Pragma                        1                                      /* Support for: _Pragma("foo") */
#define TPP_HAVE_MACRO___pragma                       TPP_CONFIG_EXTENSION_MSVC_PRAGMA       /* Support for: __pragma(foo) */
#define TPP_HAVE_CLANG_MACRO___has_attribute          TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_attribute */
#define TPP_HAVE_CLANG_MACRO___has_builtin            TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_builtin */
#define TPP_HAVE_CLANG_MACRO___has_cpp_attribute      TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_cpp_attribute */
#define TPP_HAVE_CLANG_MACRO___has_declspec_attribute TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_declspec_attribute */
#define TPP_HAVE_CLANG_MACRO___has_extension          TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_extension */
#define TPP_HAVE_CLANG_MACRO___has_feature            TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_feature */
#define TPP_HAVE_CLANG_MACRO___has_c_attribute        TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_c_attribute */
#define TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES        TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES  /* When enabled, clang's __has_feature() also expands to "1" when __has_extension() would. */
#define TPP_HAVE_MACRO___is_identifier                TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __is_identifier */
#define TPP_HAVE_MACRO___is_deprecated                TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for TPP's __is_deprecated */
#define TPP_HAVE_MACRO___is_poisoned                  TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for TPP's __is_poisoned */
#define TPP_HAVE_MACRO___has_extension                TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for TPP's __has_extension */
#define TPP_HAVE_MACRO___has_known_extension          TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for TPP's __has_known_extension */
#define TPP_HAVE_MACRO___has_warning                  TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for TPP's __has_warning */
#define TPP_HAVE_MACRO___has_known_warning            TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for TPP's __has_known_warning */
#define TPP_HAVE_MACRO___has_include                  TPP_CONFIG_EXTENSION_HAS_INCLUDE       /* Support for clang __has_include */
#define TPP_HAVE_MACRO___has_include_next             TPP_CONFIG_EXTENSION_HAS_INCLUDE       /* Support for clang __has_include_next */
#define TPP_HAVE_MACRO___has_embed                    0                                      /* Support for clang __has_embed */
#define TPP_HAVE_MACRO___FILE__                       1                                      /* __FILE__ */
#define TPP_HAVE_MACRO___LINE__                       1                                      /* __LINE__ */
#define TPP_HAVE_MACRO___TIME__                       1                                      /* __TIME__ */
#define TPP_HAVE_MACRO___DATE__                       1                                      /* __DATE__ */
#define TPP_HAVE_MACRO___COLUMN__                     TPP_CONFIG_EXTENSION_COLUMN            /* __COLUMN__ */
#define TPP_HAVE_MACRO___BASE_FILE__                  TPP_CONFIG_EXTENSION_BASEFILE          /* __BASE_FILE__ */
#define TPP_HAVE_MACRO___FILE_NAME__                  0                                      /* __FILE_NAME__ */
#define TPP_HAVE_MACRO___INCLUDE_LEVEL__              TPP_CONFIG_EXTENSION_INCLUDE_LEVEL     /* __INCLUDE_LEVEL__ */
#define TPP_HAVE_MACRO___INCLUDE_DEPTH__              TPP_CONFIG_EXTENSION_INCLUDE_LEVEL     /* __INCLUDE_DEPTH__ */
#define TPP_HAVE_MACRO___COUNTER__                    TPP_CONFIG_EXTENSION_COUNTER           /* __COUNTER__ */
#define TPP_HAVE_MACRO___TIMESTAMP__                  TPP_CONFIG_EXTENSION_TIMESTAMP         /* __TIMESTAMP__ */
#define TPP_HAVE_NUMERIC_DATE_MACROS                  TPP_CONFIG_EXTENSION_DATEUTILS         /* __DATE_DAY__, __DATE_WDAY__, __DATE_YDAY__, __DATE_MONTH__, __DATE_YEAR__ */
#define TPP_HAVE_NUMERIC_TIME_MACROS                  TPP_CONFIG_EXTENSION_TIMEUTILS         /* __TIME_SEC__, __TIME_MIN__, __TIME_HOUR__ */
#define TPP_HAVE_MACRO___TPP_EVAL                     TPP_CONFIG_EXTENSION_TPP_EVAL          /* __TPP_EVAL */
#define TPP_HAVE_MACRO___TPP_UNIQUE                   TPP_CONFIG_EXTENSION_TPP_UNIQUE        /* __TPP_UNIQUE */
#define TPP_HAVE_MACRO___TPP_LOAD_FILE                TPP_CONFIG_EXTENSION_TPP_LOAD_FILE     /* __TPP_LOAD_FILE */
#define TPP_HAVE_MACRO___TPP_COUNTER                  TPP_CONFIG_EXTENSION_TPP_COUNTER       /* __TPP_COUNTER */
#define TPP_HAVE_MACRO___TPP_RANDOM                   TPP_CONFIG_EXTENSION_TPP_RANDOM        /* __TPP_RANDOM */
#define TPP_HAVE_MACRO___TPP_STR_DECOMPILE            TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE /* __TPP_STR_DECOMPILE */
#define TPP_HAVE_MACRO___TPP_STR_PACK                 TPP_CONFIG_EXTENSION_TPP_STR_PACK      /* __TPP_STR_PACK */
#define TPP_HAVE_MACRO___TPP_STR_SUBSTR               TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR    /* __TPP_STR_SUBSTR */
#define TPP_HAVE_MACRO___TPP_STR_SIZE                 TPP_CONFIG_EXTENSION_TPP_STR_SIZE      /* __TPP_STR_SIZE */
#define TPP_HAVE_MACRO___TPP_COUNT_TOKENS             TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS  /* __TPP_COUNT_TOKENS */
#define TPP_HAVE_MACRO___TPP_IDENTIFIER               TPP_CONFIG_EXTENSION_TPP_IDENTIFIER    /* __TPP_IDENTIFIER */
#define TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS        TPP_CONFIG_EXTENSION_ALTMAC            /* Support for: #define point<T> ... */
#define TPP_HAVE_MACRO_ARGUMENT_WHITESPACE            TPP_CONFIG_EXTENSION_ARGSPACE          /* Support for retaining whitespace around macro arguments */
#define TPP_HAVE_MACRO_RECURSION                      TPP_CONFIG_EXTENSION_RECMAC            /* Support for: #pragma extension("-fmacro-recursion") */
#define TPP_HAVE_TRADITIONAL_MACROS                   TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO /* Support for traditional macro expansion */
#define TPP_HAVE_NAMED_VARARGS_IN_MACROS              TPP_CONFIG_EXTENSION_GCC_VA_ARGS       /* Support for: #define printf(format, args...) args */
#define TPP_HAVE_VA_ARGS_IN_MACROS                    TPP_CONFIG_EXTENSION_VA_ARGS           /* Support for: #define printf(format, ...) __VA_ARGS__ */
#define TPP_HAVE_VA_COMMA_IN_MACROS                   TPP_CONFIG_EXTENSION_VA_COMMA          /* Support for: #define printf(format, ...) fprintf(stderr, format __VA_COMMA__ __VA_ARGS__) */
#define TPP_HAVE_VA_OPT_IN_MACROS                     TPP_CONFIG_EXTENSION_VA_OPT            /* Support for: #define printf(format, ...) fprintf(stderr, format __VA_OPT__(,) __VA_ARGS__) */
#define TPP_HAVE_VA_NARGS_IN_MACROS                   TPP_CONFIG_EXTENSION_VA_NARGS          /* Support for: #define min(...) min_##__VA_NARGS__(__VA_ARGS__) */
#define TPP_HAVE_VA_GLUE_COMMA_IN_MACROS              TPP_CONFIG_EXTENSION_GCC_VA_COMMA      /* Support for: #define printf(format, ...) fprintf(stderr, format,##__VA_ARGS__) */
#define TPP_HAVE_STRINGIZE_MACRO_ARGUMENT             1                                      /* Support for: #define str(x) #x */
#define TPP_HAVE_CHARIZE_MACRO_ARGUMENT               TPP_CONFIG_EXTENSION_HASH_AT           /* Support for: #define chr(x) #@x */
#define TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT           TPP_CONFIG_EXTENSION_HASH_XCLAIM       /* Support for: #define noexpand(x) #!x */
#define TPP_HAVE_GLUE_MACRO_ARGUMENT                  1                                      /* Support for: #define cat(a, b) a##b */

/* #pragma directives */
#define TPP_HAVE_PRAGMA_PUSH_MACRO                1 /* Support for: #pragma push_macro() / #pragma pop_macro() */
#define TPP_HAVE_PRAGMA_ONCE                      1 /* Support for: #pragma once */
#define TPP_HAVE_PRAGMA_DEPRECATED                1 /* Support for: #pragma deprecated("foo") */
#define TPP_HAVE_PRAGMA_EXTENSION                 1 /* Support for: #pragma extension(...) */
#define TPP_HAVE_PRAGMA_WARNING                   1 /* Support for: #pragma warning(...) */
#define TPP_HAVE_PRAGMA_MESSAGE                   1 /* Support for: #pragma message("...") */
#define TPP_HAVE_PRAGMA_ERROR                     1 /* Support for: #pragma error("...") */
#define TPP_HAVE_PRAGMA_REGION                    1 /* Support for: #pragma region + #pragma endregion */
#define TPP_HAVE_PRAGMA_TPP_EXEC                  1 /* Support for: #pragma tpp_exec("...") */
#define TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS     1 /* Support for: #pragma tpp_set_keyword_flags("foo", 0x7f) */
#define TPP_HAVE_PRAGMA_GCC_POISON                1 /* Support for: #pragma GCC poison foo */
#define TPP_HAVE_PRAGMA_GCC_WARNING               0 /* TPP2 didn't know this one */
#define TPP_HAVE_PRAGMA_GCC_ERROR                 0 /* TPP2 didn't know this one */
#define TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER         1 /* Support for: #pragma GCC system_header */
#define TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC            1 /* Support for: #pragma GCC diagnostic */
#define TPP_HAVE_PRAGMA_GCC_DEPENDENCY            1 /* Support for: #pragma GCC dependency */
#define TPP_HAVE_PRAGMA_TPP_WARNING               1 /* Support for: #pragma TPP warning(...)  (same as TPP_HAVE_PRAGMA_WARNING) */
#define TPP_HAVE_PRAGMA_TPP_EXTENSION             1 /* Support for: #pragma TPP extension(...)  (same as TPP_HAVE_PRAGMA_EXTENSION) */
#define TPP_HAVE_PRAGMA_TPP_TPP_EXEC              1 /* Support for: #pragma TPP tpp_exec(...)  (same as TPP_HAVE_PRAGMA_TPP_EXEC) */
#define TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS 1 /* Support for: #pragma TPP tpp_set_keyword_flags("foo", 0x7f)  (same as TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS) */
#define TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH          1 /* Support for: #pragma TPP include_path(...) */

/* Lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_DEFINED                1                                      /* Enable support for "defined(MACRO)" in builtin lexer expressions */
#define TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR         TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED /* Enable special handling in "#define foo(x) defined(x)" such that "x" is not expanded */
#define TPP_HAVE_BUILTIN_EXPR_STRINGS                TPP_CONFIG_EXTENSION_STRINGOPS         /* Enable support for string operations in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_FLOATS                 1                                      /* Enable support for floats in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT    TPP_CONFIG_EXTENSION_GCC_IFELSE        /* Enable support for "foo ?: bar" in builtin lexer expressions (same as "foo ? foo : bar") */
#define TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR    /* Enable support for "if (foo) bar else baz" in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR            TPP_CONFIG_EXTENSION_LXOR              /* Enable support for "^^" in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS        TPP_CONFIG_EXTENSION_BININTEGRAL       /* Enable support for "0b" literals in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS         0                                      /* Enable support for "0o" literals in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS   1                                      /* Enable support for "u", "l", "ul", "ll", "ull" integer suffixes in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS TPP_CONFIG_EXTENSION_MSVC_FIXED_INT    /* Enable support for "i8", "i16", "i32", "i64", "ui8", "ui16", "ui32", "ui64" integer suffixes in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS     1                                      /* Treat 'a' as an integer, rather than as a string (in C, this is always the case) */

/* Include-path-related features */
#define TPP_HAVE_INCLUDE_PATH_QUOTE             0 /* TPP2 only had a system-include-path list */
#define TPP_HAVE_INCLUDE_PATH_AFTER             0 /* TPP2 only had a system-include-path list */
#define TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE 1 /* TPP2 used to do this unconditionally */

/* Force extensions to use the names they'd been using in TPP2 */
#define TPP_EXTNAME_TRIGRAPHS                           "trigraphs"
#define TPP_EXTNAME_DIGRAPHS                            "digraphs"
#define TPP_EXTNAME_NAMED_VARARGS_IN_MACROS             "named-varargs-in-macros"
#define TPP_EXTNAME_VA_GLUE_COMMA_IN_MACROS             "glue-comma-in-macros"
#define TPP_EXTNAME_VA_COMMA_IN_MACROS                  "va-comma-in-macros"
#define TPP_EXTNAME_VA_OPT_IN_MACROS                    "va-opt-in-macros"
#define TPP_EXTNAME_VA_NARGS_IN_MACROS                  "va-nargs-in-macros"
#define TPP_EXTNAME_VA_ARGS_IN_MACROS                   "va-args-in-macros"
#define TPP_EXTNAME_ESCAPE_E_IN_STRINGS                 "escape-e-in-strings"
#define TPP_EXTNAME_ALTERNATIVE_MACRO_PARENTHESIS       "alternative-macro-parenthesis"
#define TPP_EXTNAME_MACRO_RECURSION                     "macro-recursion"
#define TPP_EXTNAME_MACRO_ARGUMENT_WHITESPACE           "macro-argument-whitespace"
#define TPP_EXTNAME_MACRO___pragma                      "msvc-pragma-support" /* TPP3 renamed "-fmsvc-pragma-support" -> "-f__pragma" */
#define TPP_EXTNAME_CHARIZE_MACRO_ARGUMENT              "charize-macro-argument"
#define TPP_EXTNAME_DONT_EXPAND_MACRO_ARGUMENT          "dont-expand-macro-argument"
#define TPP_EXTNAME_CPP_WARNING                         "warning-directives"
#define TPP_EXTNAME_CPP_EXCLAIM                         "shebang-directives"
#define TPP_EXTNAME_CPP_INCLUDE_NEXT                    "include-next-directives"
#define TPP_EXTNAME_CPP_IMPORT                          "import-directives"
#define TPP_EXTNAME_CPP_IDENT_SCCS                      "ident-directives"
#define TPP_EXTNAME_MACRO___BASE_FILE__                 "basefile-macro"
#define TPP_EXTNAME_MACRO___INCLUDE_LEVEL__             "include-level-macro"
#define TPP_EXTNAME_MACRO___COUNTER__                   "counter-macro"
#define TPP_EXTNAME_MACRO___TIMESTAMP__                 "timestamp-macro"
#define TPP_EXTNAME_MACRO___COLUMN__                    "column-macro"
#define TPP_EXTNAME_NUMERIC_DATE_MACROS                 "numeric-date-macros"
#define TPP_EXTNAME_NUMERIC_TIME_MACROS                 "numeric-time-macros"
#define TPP_EXTNAME_MACRO___TPP_EVAL                    "tpp-eval-macro"
#define TPP_EXTNAME_MACRO___TPP_UNIQUE                  "tpp-unique-macro"
#define TPP_EXTNAME_MACRO___TPP_LOAD_FILE               "tpp-load-file-macro"
#define TPP_EXTNAME_MACRO___TPP_COUNTER                 "tpp-counter-macro"
#define TPP_EXTNAME_MACRO___TPP_RANDOM                  "tpp-random-macro"
#define TPP_EXTNAME_MACRO___TPP_STR_DECOMPILE           "tpp-str-decompile-macro"
#define TPP_EXTNAME_MACRO___TPP_STR_SUBSTR              "tpp-str-substr-macro"
#define TPP_EXTNAME_MACRO___TPP_STR_PACK                "tpp-str-pack-macro"
#define TPP_EXTNAME_MACRO___TPP_STR_SIZE                "tpp-str-size-macro"
#define TPP_EXTNAME_MACRO___TPP_COUNT_TOKENS            "tpp-count-tokens-macro"
#define TPP_EXTNAME_MACRO___TPP_IDENTIFIER              "tpp-identifier-macro"
#define TPP_EXTNAME_CPP_ASSERT                          "assertions"
#define TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR         "dont-expand-defined"
#define TPP_EXTNAME_TRADITIONAL_MACROS                  "traditional-macro"
#define TPP_EXTNAME_CLANG_EXTENSIONS_ARE_FEATURES       "extensions-are-features" /* TPP3 renamed "-fextensions-are-features" -> "-fclang-extensions-are-features" */
#define TPP_EXTNAME_BUILTIN_EXPR_STRINGS                "strings-in-expressions"
#define TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT    "if-else-optional-true"
#define TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS "ifelse-in-expressions"
#define TPP_EXTNAME_BUILTIN_EXPR_LOGICAL_XOR            "logical-xor-in-expressions"
#define TPP_EXTNAME_BUILTIN_EXPR_BINARY_LITERALS        "binary-literals"
#define TPP_EXTNAME_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS "fixed-length-integrals"
/************************************************************************/





/************************************************************************/
/* CUSTOM HOOKS                                                         */
/************************************************************************/
#ifdef TPP_CONFIG_USERSTREAMS
#define tpp_io_handle         TPP_USERSTREAM_TYPE
#define tpp_io_handle_INVALID TPP_USERSTREAM_INVALID
#define tpp_io_open(filename) TPP_USERSTREAM_FOPEN(filename)
#define tpp_io_close(file)  TPP_USERSTREAM_FCLOSE(file)
#if TPP_HAVE_FILE_NONBLOCK
#define tpp_io_read(file, buf, bufsize, nonblock)                   \
	((nonblock) ? TPP_USERSTREAM_FREAD_NONBLOCK(file, buf, bufsize) \
	            : TPP_USERSTREAM_FREAD(file, buf, bufsize))
#else /* TPP_HAVE_FILE_NONBLOCK */
#define tpp_io_read(file, buf, bufsize) \
	TPP_USERSTREAM_FREAD(file, buf, bufsize)
#endif /* !TPP_HAVE_FILE_NONBLOCK */
#endif /* TPP_CONFIG_USERSTREAMS */
/************************************************************************/





/************************************************************************/
/* Configure the user's custom "defs.h" file                            */
/************************************************************************/
#ifndef TPP2_NO_AUTOCONFIGURE_TPP3_DEFS
#ifndef __INTELLISENSE__
#ifndef TPP_CONFIG_USERDEFS_FILENAME
#define TPP_CONFIG_USERDEFS_FILENAME "tpp2.h"
#endif /* !TPP_CONFIG_USERDEFS_FILENAME */
#endif /* !__INTELLISENSE__ */
#endif /* !TPP2_NO_AUTOCONFIGURE_TPP3_DEFS */
/************************************************************************/





/************************************************************************/
/* PULL IN HEADER                                                       */
/************************************************************************/
#endif /* !TPP2_NO_AUTOCONFIGURE_TPP3 */
#include "tpp-amalgamation.h"
/************************************************************************/





/************************************************************************/
/* PROVIDE ALIASES                                                      */
/************************************************************************/

/*[[[deemon
import * from deemon;

function alias(tpp2Name, tpp3Name, onlyIfDefined = true, condition: string = "") {
	local nsTpp2Name = "TPP_" + tpp2Name;
	if (!condition)
		condition = onlyIfDefined ? f"defined({tpp3Name})" : "1";
	local simpleCondition = condition;
	if (condition != "1") {
		try {
			simpleCondition = condition.rescanf(r"defined\((\w+)\)")...;
		} catch (...) {
			simpleCondition = condition;
		}
	}
	if (nsTpp2Name != tpp3Name || condition == "1") {
		if (condition != "1") {
			if (simpleCondition != condition) {
				print("#ifdef ", simpleCondition);
			} else {
				print("#if ", condition);
			}
		}
		if (nsTpp2Name != tpp3Name)
			print("#define ", nsTpp2Name, " ", tpp3Name);
		print("#if TPP2_HAVE_GLOBAL_NAMESPACE");
		print("#define ", tpp2Name, " ", tpp3Name);
		print("#endif /" "* TPP2_HAVE_GLOBAL_NAMESPACE *" "/");
		if (condition != "1")
			print("#endif /" "* ", simpleCondition, " *" "/");
	} else {
		print("#if TPP2_HAVE_GLOBAL_NAMESPACE && ", condition);
		print("#define ", tpp2Name, " ", tpp3Name);
		print("#endif /" "* TPP2_HAVE_GLOBAL_NAMESPACE && ", simpleCondition, " *" "/");
	}
}

// Token IDs
alias("TOK_EOF", "TPP_TOK_EOF", onlyIfDefined: false);

alias("TOK_CHAR",      "TPP_TOK_CHAR",      condition: "TPP_HAVE_TPP_TOK_CHAR");
alias("TOK_STRING",    "TPP_TOK_STRING",    condition: "TPP_HAVE_TPP_TOK_STRING");
alias("TOK_INT",       "TPP_TOK_INT",       condition: "TPP_HAVE_TPP_TOK_INT");
alias("TOK_FLOAT",     "TPP_TOK_FLOAT",     condition: "TPP_HAVE_TPP_TOK_FLOAT");
alias("TOK_LF",        "TPP_TOK_LF",        onlyIfDefined: false);
alias("TOK_SPACE",     "TPP_TOK_SPACE",     onlyIfDefined: false);
alias("TOK_ADD",       "TPP_TOK_PLUS",      onlyIfDefined: false);
alias("TOK_AND",       "TPP_TOK_AMP",       onlyIfDefined: false);
alias("TOK_ASSIGN",    "TPP_TOK_EQUAL",     onlyIfDefined: false);
alias("TOK_AT",        "TPP_TOK_AT",        onlyIfDefined: false);
alias("TOK_BACKSLASH", "TPP_TOK_BACKSLASH", onlyIfDefined: false);
alias("TOK_COLON",     "TPP_TOK_COLON",     onlyIfDefined: false);
alias("TOK_COMMA",     "TPP_TOK_COMMA",     onlyIfDefined: false);
alias("TOK_DIV",       "TPP_TOK_SLASH",     onlyIfDefined: false);
alias("TOK_DOT",       "TPP_TOK_DOT",       onlyIfDefined: false);
alias("TOK_HASH",      "TPP_TOK_POUND",     onlyIfDefined: false);
alias("TOK_LANGLE",    "TPP_TOK_LANGLE",    onlyIfDefined: false);
alias("TOK_RANGLE",    "TPP_TOK_RANGLE",    onlyIfDefined: false);
alias("TOK_LBRACKET",  "TPP_TOK_LBRACKET",  onlyIfDefined: false);
alias("TOK_RBRACKET",  "TPP_TOK_RBRACKET",  onlyIfDefined: false);
alias("TOK_LBRACE",    "TPP_TOK_LBRACE",    onlyIfDefined: false);
alias("TOK_RBRACE",    "TPP_TOK_RBRACE",    onlyIfDefined: false);
alias("TOK_LPAREN",    "TPP_TOK_LPAREN",    onlyIfDefined: false);
alias("TOK_RPAREN",    "TPP_TOK_RPAREN",    onlyIfDefined: false);
alias("TOK_MOD",       "TPP_TOK_PERCENT",   onlyIfDefined: false);
alias("TOK_MUL",       "TPP_TOK_STAR",      onlyIfDefined: false);
alias("TOK_NOT",       "TPP_TOK_EXCLAIM",   onlyIfDefined: false);
alias("TOK_OR",        "TPP_TOK_PIPE",      onlyIfDefined: false);
alias("TOK_QUESTION",  "TPP_TOK_QMARK",     onlyIfDefined: false);
alias("TOK_SEMICOLON", "TPP_TOK_SEMICOLON", onlyIfDefined: false);
alias("TOK_SUB",       "TPP_TOK_MINUS",     onlyIfDefined: false);
alias("TOK_TILDE",     "TPP_TOK_TILDE",     onlyIfDefined: false);
alias("TOK_XOR",       "TPP_TOK_HAT",       onlyIfDefined: false);

alias("TOK_SHL", "TPP_TOK_LANGLE_LANGLE", condition: "TPP_HAVE_TPP_TOK_LANGLE_LANGLE");
alias("TOK_SHR", "TPP_TOK_RANGLE_RANGLE", condition: "TPP_HAVE_TPP_TOK_RANGLE_RANGLE");

// Custom handling required here because "TPP_TOK_EQUAL" is a single-character token in TPP3
print("#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_EQUAL_EQUAL");
print("#define TOK_EQUAL TPP_TOK_EQUAL_EQUAL");
print("#endif /" "* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_EQUAL_EQUAL *" "/");

alias("TOK_NOT_EQUAL", "TPP_TOK_EXCLAIM_EQUAL", condition: "TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL");
alias("TOK_GREATER_EQUAL", "TPP_TOK_RANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_RANGLE_EQUAL");
alias("TOK_LOWER_EQUAL", "TPP_TOK_LANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_LANGLE_EQUAL");
alias("TOK_DOTS", "TPP_TOK_DOT_DOT_DOT", condition: "TPP_HAVE_TPP_TOK_DOT_DOT_DOT");
alias("TOK_ADD_EQUAL", "TPP_TOK_PLUS_EQUAL", condition: "TPP_HAVE_TPP_TOK_PLUS_EQUAL");
alias("TOK_SUB_EQUAL", "TPP_TOK_MINUS_EQUAL", condition: "TPP_HAVE_TPP_TOK_MINUS_EQUAL");
alias("TOK_MUL_EQUAL", "TPP_TOK_STAR_EQUAL", condition: "TPP_HAVE_TPP_TOK_STAR_EQUAL");
alias("TOK_DIV_EQUAL", "TPP_TOK_SLASH_EQUAL", condition: "TPP_HAVE_TPP_TOK_SLASH_EQUAL");
alias("TOK_MOD_EQUAL", "TPP_TOK_PERCENT_EQUAL", condition: "TPP_HAVE_TPP_TOK_PERCENT_EQUAL");
alias("TOK_SHL_EQUAL", "TPP_TOK_LANGLE_LANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL");
alias("TOK_SHR_EQUAL", "TPP_TOK_RANGLE_RANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL");
alias("TOK_AND_EQUAL", "TPP_TOK_AMP_EQUAL", condition: "TPP_HAVE_TPP_TOK_AMP_EQUAL");
alias("TOK_OR_EQUAL", "TPP_TOK_PIPE_EQUAL", condition: "TPP_HAVE_TPP_TOK_PIPE_EQUAL");
alias("TOK_XOR_EQUAL", "TPP_TOK_HAT_EQUAL", condition: "TPP_HAVE_TPP_TOK_HAT_EQUAL");
alias("TOK_POW_EQUAL", "TPP_TOK_STAR_STAR_EQUAL", condition: "TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL");
alias("TOK_AT_EQUAL", "TPP_TOK_AT_EQUAL", condition: "TPP_HAVE_TPP_TOK_AT_EQUAL");
alias("TOK_GLUE", "TPP_TOK_POUND_POUND", condition: "TPP_HAVE_TPP_TOK_POUND_POUND");
alias("TOK_LAND", "TPP_TOK_AMP_AMP", condition: "TPP_HAVE_TPP_TOK_AMP_AMP");
alias("TOK_LOR", "TPP_TOK_PIPE_PIPE", condition: "TPP_HAVE_TPP_TOK_PIPE_PIPE");
alias("TOK_LXOR", "TPP_TOK_HAT_HAT", condition: "TPP_HAVE_TPP_TOK_HAT_HAT");
alias("TOK_INC", "TPP_TOK_PLUS_PLUS", condition: "TPP_HAVE_TPP_TOK_PLUS_PLUS");
alias("TOK_DEC", "TPP_TOK_MINUS_MINUS", condition: "TPP_HAVE_TPP_TOK_MINUS_MINUS");
alias("TOK_POW", "TPP_TOK_STAR_STAR", condition: "TPP_HAVE_TPP_TOK_STAR_STAR");
alias("TOK_TILDE_TILDE", "TPP_TOK_TILDE_TILDE", condition: "TPP_HAVE_TPP_TOK_TILDE_TILDE");
alias("TOK_ARROW", "TPP_TOK_MINUS_RANGLE", condition: "TPP_HAVE_TPP_TOK_MINUS_RANGLE");
alias("TOK_COLON_EQUAL", "TPP_TOK_COLON_EQUAL", condition: "TPP_HAVE_TPP_TOK_COLON_EQUAL");
alias("TOK_NAMESPACE", "TPP_TOK_COLON_COLON", condition: "TPP_HAVE_TPP_TOK_COLON_COLON");
alias("TOK_ARROW_STAR", "TPP_TOK_MINUS_RANGLE_STAR", condition: "TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR");
alias("TOK_DOT_STAR", "TPP_TOK_DOT_STAR", condition: "TPP_HAVE_TPP_TOK_DOT_STAR");
alias("TOK_DOTDOT", "TPP_TOK_DOT_DOT", condition: "TPP_HAVE_TPP_TOK_DOT_DOT");
alias("TOK_LOGT", "TPP_TOK_LANGLE_RANGLE", condition: "TPP_HAVE_TPP_TOK_LANGLE_RANGLE");
alias("TOK_LANGLE3", "TPP_TOK_LANGLE_LANGLE_LANGLE", condition: "TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE");
alias("TOK_RANGLE3", "TPP_TOK_RANGLE_RANGLE_RANGLE", condition: "TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE");
alias("TOK_LANGLE3_EQUAL", "TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL");
alias("TOK_RANGLE3_EQUAL", "TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL");
alias("TOK_EQUAL3", "TPP_TOK_EQUAL_EQUAL_EQUAL", condition: "TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL");
alias("TOK_NOT_EQUAL3", "TPP_TOK_EXCLAIM_EQUAL_EQUAL", condition: "TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL");
alias("TOK_QMARK_QMARK", "TPP_TOK_QMARK_QMARK", condition: "TPP_HAVE_TPP_TOK_QMARK_QMARK");
alias("TOK_KEYWORD_BEGIN", "TPP_TOK_KEYWORD_BEGIN", onlyIfDefined: false);
alias("TOK_TWOCHAR_END", "TPP_TOK_MULTICHAR_END", onlyIfDefined: false);

// Name aliases 
alias("TOK_POS",           "TPP_TOK_PLUS",      onlyIfDefined: false);
alias("TOK_NEG",           "TPP_TOK_MINUS",     onlyIfDefined: false);
alias("TOK_LOWER",         "TPP_TOK_LANGLE",    onlyIfDefined: false);
alias("TOK_GREATER",       "TPP_TOK_RANGLE",    onlyIfDefined: false);
alias("TOK_COLON_COLON",   "TPP_TOK_COLON_COLON", condition: "TPP_HAVE_TPP_TOK_COLON_COLON");
alias("TOK_LOWER_GREATER", "TPP_TOK_LANGLE_RANGLE", condition: "TPP_HAVE_TPP_TOK_LANGLE_RANGLE");
alias("TOK_LANGLE_RANGLE", "TPP_TOK_LANGLE_RANGLE", condition: "TPP_HAVE_TPP_TOK_LANGLE_RANGLE");
alias("TOK_LANGLE1",       "TPP_TOK_LANGLE",    onlyIfDefined: false);
alias("TOK_LANGLE2",       "TPP_TOK_LANGLE_LANGLE", condition: "TPP_HAVE_TPP_TOK_LANGLE_LANGLE");
alias("TOK_LANGLE_EQUAL",  "TPP_TOK_LANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_LANGLE_EQUAL");
alias("TOK_LANGLE1_EQUAL", "TPP_TOK_LANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_LANGLE_EQUAL");
alias("TOK_LANGLE2_EQUAL", "TPP_TOK_LANGLE_LANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL");
alias("TOK_RANGLE1",       "TPP_TOK_RANGLE",    onlyIfDefined: false);
alias("TOK_RANGLE2",       "TPP_TOK_RANGLE_RANGLE", condition: "TPP_HAVE_TPP_TOK_RANGLE_RANGLE");
alias("TOK_RANGLE_EQUAL",  "TPP_TOK_RANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_RANGLE_EQUAL");
alias("TOK_RANGLE1_EQUAL", "TPP_TOK_RANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_RANGLE_EQUAL");
alias("TOK_RANGLE2_EQUAL", "TPP_TOK_RANGLE_RANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL");

// Deprecated typos 
alias("TOK_COLLON",        "TPP_TOK_COLON",     onlyIfDefined: false);
alias("TOK_COLLON_EQUAL",  "TPP_TOK_COLON_EQUAL", condition: "TPP_HAVE_TPP_TOK_COLON_EQUAL");
alias("TOK_COLLON_COLLON", "TPP_TOK_COLON_COLON", condition: "TPP_HAVE_TPP_TOK_COLON_COLON");

// Keywords
alias("KWD_if", "TPP_KWD_if");
alias("KWD_ifdef", "TPP_KWD_ifdef");
alias("KWD_ifndef", "TPP_KWD_ifndef");
alias("KWD_elif", "TPP_KWD_elif");
alias("KWD_else", "TPP_KWD_else");
alias("KWD_endif", "TPP_KWD_endif");
alias("KWD_define", "TPP_KWD_define");
alias("KWD_defined", "TPP_KWD_defined");
alias("KWD_undef", "TPP_KWD_undef");
alias("KWD_include", "TPP_KWD_include");
alias("KWD_include_next", "TPP_KWD_include_next");
alias("KWD_import", "TPP_KWD_import");
alias("KWD_line", "TPP_KWD_line");
alias("KWD_error", "TPP_KWD_error");
alias("KWD_warning", "TPP_KWD_warning");
alias("KWD_ident", "TPP_KWD_ident");
alias("KWD_sccs", "TPP_KWD_sccs");
alias("KWD_assert", "TPP_KWD_assert");
alias("KWD_unassert", "TPP_KWD_unassert");
alias("KWD_pragma", "TPP_KWD_pragma");
alias("KWD__Pragma", "TPP_KWD__Pragma");
alias("KWD___pragma", "TPP_KWD___pragma");
alias("KWD___FILE__", "TPP_KWD___FILE__");
alias("KWD___LINE__", "TPP_KWD___LINE__");
alias("KWD___TIME__", "TPP_KWD___TIME__");
alias("KWD___DATE__", "TPP_KWD___DATE__");
alias("KWD___BASE_FILE__", "TPP_KWD___BASE_FILE__");
alias("KWD___INCLUDE_LEVEL__", "TPP_KWD___INCLUDE_LEVEL__");
alias("KWD___INCLUDE_DEPTH__", "TPP_KWD___INCLUDE_DEPTH__");
alias("KWD___COUNTER__", "TPP_KWD___COUNTER__");
alias("KWD___TIMESTAMP__", "TPP_KWD___TIMESTAMP__");
alias("KWD___COLUMN__", "TPP_KWD___COLUMN__");
alias("KWD___is_identifier", "TPP_KWD___is_identifier");
alias("KWD___is_deprecated", "TPP_KWD___is_deprecated");
alias("KWD___is_poisoned", "TPP_KWD___is_poisoned");
alias("KWD___has_attribute", "TPP_KWD___has_attribute");
alias("KWD___has_builtin", "TPP_KWD___has_builtin");
alias("KWD___has_cpp_attribute", "TPP_KWD___has_cpp_attribute");
alias("KWD___has_declspec_attribute", "TPP_KWD___has_declspec_attribute");
alias("KWD___has_feature", "TPP_KWD___has_feature");
alias("KWD___has_extension", "TPP_KWD___has_extension");
alias("KWD___has_warning", "TPP_KWD___has_warning");
alias("KWD___has_known_extension", "TPP_KWD___has_known_extension");
alias("KWD___has_known_warning", "TPP_KWD___has_known_warning");
alias("KWD___has_include", "TPP_KWD___has_include");
alias("KWD___has_include_next", "TPP_KWD___has_include_next");
alias("KWD___VA_ARGS__", "TPP_KWD___VA_ARGS__");
alias("KWD___VA_COMMA__", "TPP_KWD___VA_COMMA__");
alias("KWD___VA_NARGS__", "TPP_KWD___VA_NARGS__");
alias("KWD___VA_OPT__", "TPP_KWD___VA_OPT__");
alias("KWD___TPP_EVAL", "TPP_KWD___TPP_EVAL");
alias("KWD___TPP_LOAD_FILE", "TPP_KWD___TPP_LOAD_FILE");
alias("KWD___TPP_COUNTER", "TPP_KWD___TPP_COUNTER");
alias("KWD___TPP_RANDOM", "TPP_KWD___TPP_RANDOM");
alias("KWD___TPP_STR_DECOMPILE", "TPP_KWD___TPP_STR_DECOMPILE");
alias("KWD___TPP_STR_AT", "TPP_KWD___TPP_STR_AT");
alias("KWD___TPP_STR_SUBSTR", "TPP_KWD___TPP_STR_SUBSTR");
alias("KWD___TPP_STR_PACK", "TPP_KWD___TPP_STR_PACK");
alias("KWD___TPP_STR_SIZE", "TPP_KWD___TPP_STR_SIZE");
alias("KWD___TPP_UNIQUE", "TPP_KWD___TPP_UNIQUE");
alias("KWD___TPP_COUNT_TOKENS", "TPP_KWD___TPP_COUNT_TOKENS");
alias("KWD___TPP_IDENTIFIER", "TPP_KWD___TPP_IDENTIFIER");
alias("KWD___DATE_DAY__", "TPP_KWD___DATE_DAY__");
alias("KWD___DATE_WDAY__", "TPP_KWD___DATE_WDAY__");
alias("KWD___DATE_YDAY__", "TPP_KWD___DATE_YDAY__");
alias("KWD___DATE_MONTH__", "TPP_KWD___DATE_MONTH__");
alias("KWD___DATE_YEAR__", "TPP_KWD___DATE_YEAR__");
alias("KWD___TIME_SEC__", "TPP_KWD___TIME_SEC__");
alias("KWD___TIME_MIN__", "TPP_KWD___TIME_MIN__");
alias("KWD___TIME_HOUR__", "TPP_KWD___TIME_HOUR__");
alias("KWD_tpp_dollar_is_alpha", "TPP_KWD_tpp_dollar_is_alpha");
alias("KWD_tpp_va_args", "TPP_KWD_tpp_va_args");
alias("KWD_tpp_named_va_args", "TPP_KWD_tpp_named_va_args");
alias("KWD_tpp_va_comma", "TPP_KWD_tpp_va_comma");
alias("KWD_tpp_msvc_integer_suffix", "TPP_KWD_tpp_msvc_integer_suffix");
alias("KWD_tpp_charize_operator", "TPP_KWD_tpp_charize_operator");
alias("KWD_tpp_trigraphs", "TPP_KWD_tpp_trigraphs");
alias("KWD_tpp_digraphs", "TPP_KWD_tpp_digraphs");
alias("KWD_tpp_reemit_unknown_pragmas", "TPP_KWD_tpp_reemit_unknown_pragmas");
alias("KWD_tpp_pragma_push_macro", "TPP_KWD_tpp_pragma_push_macro");
alias("KWD_tpp_pragma_pop_macro", "TPP_KWD_tpp_pragma_pop_macro");
alias("KWD_tpp_pragma_region", "TPP_KWD_tpp_pragma_region");
alias("KWD_tpp_pragma_endregion", "TPP_KWD_tpp_pragma_endregion");
alias("KWD_tpp_pragma_warning", "TPP_KWD_tpp_pragma_warning");
alias("KWD_tpp_pragma_message", "TPP_KWD_tpp_pragma_message");
alias("KWD_tpp_pragma_error", "TPP_KWD_tpp_pragma_error");
alias("KWD_tpp_pragma_once", "TPP_KWD_tpp_pragma_once");
alias("KWD_tpp_pragma_tpp_exec", "TPP_KWD_tpp_pragma_tpp_exec");
alias("KWD_tpp_pragma_deprecated", "TPP_KWD_tpp_pragma_deprecated");
alias("KWD_tpp_pragma_tpp_set_keyword_flags", "TPP_KWD_tpp_pragma_tpp_set_keyword_flags");
alias("KWD_tpp_directive_include_next", "TPP_KWD_tpp_directive_include_next");
alias("KWD_tpp_directive_import", "TPP_KWD_tpp_directive_import");
alias("KWD_tpp_directive_warning", "TPP_KWD_tpp_directive_warning");
alias("KWD_tpp_lxor", "TPP_KWD_tpp_lxor");
alias("KWD_tpp_token_tilde_tilde", "TPP_KWD_tpp_token_tilde_tilde");
alias("KWD_tpp_token_pow", "TPP_KWD_tpp_token_pow");
alias("KWD_tpp_token_lxor", "TPP_KWD_tpp_token_lxor");
alias("KWD_tpp_token_arrow", "TPP_KWD_tpp_token_arrow");
alias("KWD_tpp_token_colon_assign", "TPP_KWD_tpp_token_colon_assign");
alias("KWD_tpp_token_colon_colon", "TPP_KWD_tpp_token_colon_colon");
alias("KWD_tpp_macro_calling_conventions", "TPP_KWD_tpp_macro_calling_conventions");
alias("KWD_tpp_strict_whitespace", "TPP_KWD_tpp_strict_whitespace");
alias("KWD_tpp_strict_integer_overflow", "TPP_KWD_tpp_strict_integer_overflow");
alias("KWD_tpp_support_ansi_characters", "TPP_KWD_tpp_support_ansi_characters");
alias("KWD_tpp_emit_lf_after_directive", "TPP_KWD_tpp_emit_lf_after_directive");
alias("KWD_tpp_if_cond_expression", "TPP_KWD_tpp_if_cond_expression");
alias("KWD_tpp_debug", "TPP_KWD_tpp_debug");
alias("KWD___TPP_VERSION__", "TPP_KWD___TPP_VERSION__", onlyIfDefined: false);

// Extensions
alias("EXT_TRIGRAPHS", "TPP_EXT_TRIGRAPHS");
alias("EXT_DIGRAPHS", "TPP_EXT_DIGRAPHS");
alias("EXT_GCC_VA_ARGS", "TPP_EXT_NAMED_VARARGS_IN_MACROS");
alias("EXT_GCC_VA_COMMA", "TPP_EXT_VA_GLUE_COMMA_IN_MACROS");
alias("EXT_VA_COMMA", "TPP_EXT_VA_COMMA_IN_MACROS");
alias("EXT_VA_OPT", "TPP_EXT_VA_OPT_IN_MACROS");
alias("EXT_VA_NARGS", "TPP_EXT_VA_NARGS_IN_MACROS");
alias("EXT_VA_ARGS", "TPP_EXT_VA_ARGS_IN_MACROS");
alias("EXT_STR_E", "TPP_EXT_ESCAPE_E_IN_STRINGS");
alias("EXT_ALTMAC", "TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS");
alias("EXT_RECMAC", "TPP_EXT_MACRO_RECURSION");
alias("EXT_ARGSPACE", "TPP_EXT_MACRO_ARGUMENT_WHITESPACE");
alias("EXT_MSVC_PRAGMA", "TPP_EXT_MACRO___pragma");
alias("EXT_HASH_AT", "TPP_EXT_CHARIZE_MACRO_ARGUMENT");
alias("EXT_HASH_XCLAIM", "TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT");
alias("EXT_WARNING", "TPP_EXT_CPP_WARNING");
alias("EXT_SHEBANG", "TPP_EXT_CPP_EXCLAIM");
alias("EXT_INCLUDE_NEXT", "TPP_EXT_CPP_INCLUDE_NEXT");
alias("EXT_IMPORT", "TPP_EXT_CPP_IMPORT");
alias("EXT_IDENT_SCCS", "TPP_EXT_CPP_IDENT_SCCS");
alias("EXT_BASEFILE", "TPP_EXT_MACRO___BASE_FILE__");
alias("EXT_INCLUDE_LEVEL", "TPP_EXT_MACRO___INCLUDE_LEVEL__");
alias("EXT_COUNTER", "TPP_EXT_MACRO___COUNTER__");
alias("EXT_TIMESTAMP", "TPP_EXT_MACRO___TIMESTAMP__");
alias("EXT_COLUMN", "TPP_EXT_MACRO___COLUMN__");
alias("EXT_DATEUTILS", "TPP_EXT_NUMERIC_DATE_MACROS");
alias("EXT_TIMEUTILS", "TPP_EXT_NUMERIC_TIME_MACROS");
alias("EXT_TPP_EVAL", "TPP_EXT_MACRO_TPP_EVAL");
alias("EXT_TPP_UNIQUE", "TPP_EXT_MACRO_TPP_UNIQUE");
alias("EXT_TPP_LOAD_FILE", "TPP_EXT_MACRO_TPP_LOAD_FILE");
alias("EXT_TPP_COUNTER", "TPP_EXT_MACRO_TPP_COUNTER");
alias("EXT_TPP_RANDOM", "TPP_EXT_MACRO_TPP_RANDOM");
alias("EXT_TPP_STR_DECOMPILE", "TPP_EXT_MACRO_TPP_STR_DECOMPILE");
alias("EXT_TPP_STR_SUBSTR", "TPP_EXT_MACRO_TPP_STR_SUBSTR");
alias("EXT_TPP_STR_PACK", "TPP_EXT_MACRO_TPP_STR_PACK");
alias("EXT_TPP_STR_SIZE", "TPP_EXT_MACRO_TPP_STR_SIZE");
alias("EXT_TPP_COUNT_TOKENS", "TPP_EXT_MACRO_TPP_COUNT_TOKENS");
alias("EXT_TPP_IDENTIFIER", "TPP_EXT_MACRO_TPP_IDENTIFIER");
alias("EXT_ASSERTIONS", "TPP_EXT_CPP_ASSERT");
alias("EXT_NO_EXPAND_DEFINED", "TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR");
alias("EXT_TRADITIONAL_MACRO", "TPP_EXT_TRADITIONAL_MACROS");
alias("EXT_EXT_ARE_FEATURES", "TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES");
alias("EXT_STRINGOPS", "TPP_EXT_BUILTIN_EXPR_STRINGS");
alias("EXT_GCC_IFELSE", "TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT");
alias("EXT_IFELSE_IN_EXPR", "TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS");
alias("EXT_LXOR", "TPP_EXT_BUILTIN_EXPR_LOGICAL_XOR");
alias("EXT_BININTEGRAL", "TPP_EXT_BUILTIN_EXPR_BINARY_LITERALS");
alias("EXT_MSVC_FIXED_INT", "TPP_EXT_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS");

// Warning groups
alias("WG_COMMENT",  "TPP_WG_COMMENT");
alias("WG_COMMENTS", "TPP_WG_COMMENT");
alias("WG_MACROS", "TPP_WG_MACROS");
alias("WG_SYNTAX", "TPP_WG_SYNTAX");
alias("WG_VALUE", "TPP_WG_VALUE");
alias("WG_USER", "TPP_WG_USER");
alias("WG_UNDEF", "TPP_WG_UNDEF");
alias("WG_TRIGRAPHS", "TPP_WG_TRIGRAPHS");
alias("WG_EXPANSION_TO_DEFINED", "TPP_WG_EXPANSION_TO_DEFINED");
alias("WG_DEPRECATED", "TPP_WG_DEPRECATED");
alias("WG_ENVIRON", "TPP_WG_ENVIRON");

//TODO:alias("WG_USAGE", "TPP_WG_USAGE");
//TODO:alias("WG_BOOLVALUE", "TPP_WG_BOOLVALUE");
//TODO:alias("WG_LIMIT", "TPP_WG_LIMIT");
//TODO:alias("WG_QUALITY", "TPP_WG_QUALITY");
//TODO:alias("WG_DEPENDENCY", "TPP_WG_DEPENDENCY");

// Warnings
alias("W_EXPECTED_KEYWORD_AFTER_DEFINE", "TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE");
alias("W_EXPECTED_KEYWORD_AFTER_UNDEF", "TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE");
alias("W_EXPECTED_KEYWORD_AFTER_IFDEF", "TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE");
alias("W_UNKNOWN_PREPROCESSOR_DIRECTIVE", "TPP_W_UNKNOWN_DIRECTIVE");
alias("W_ERROR", "TPP_W_ERROR");
alias("W_WARNING", "TPP_W_WARNING");
alias("W_EXPECTED_MACRO_ARGUMENT_NAME", "TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST");
alias("W_EXPECTED_ARGEND_AFTER_VARARGS", "TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST");
alias("W_EXPECTED_COMMA_OR_ARGEND", "TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST");
alias("W_TOO_MANY_MACRO_ARGUMENTS", "TPP_W_TOO_MANY_ARGUMENTS");
alias("W_EOF_IN_MACRO_ARGUMENT_LIST", "TPP_W_EOF_IN_ARGUMENT_LIST");
alias("W_REDEFINING_MACRO", "TPP_W_REDEFINE_MACRO");
alias("W_CANT_UNDEF_BUILTIN_MACRO", "TPP_W_CANNOT_UNDEF_BUILTIN_MACRO");
alias("W_EXPECTED_STRING_AFTER_PUSHMACRO", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_TPP_STRD", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_LPAREN", "TPP_W_UNEXPECTED_TOKEN");
alias("W_EXPECTED_RPAREN", "TPP_W_UNEXPECTED_TOKEN");
alias("W_DEPRECATED_IDENTIFIER", "TPP_W_DEPRECATED_KEYWORD");
alias("W_ARGUMENT_NAMED_ALREADY_TAKEN", "TPP_W_DUPLICATE_MACRO_PARAMETER_NAME");
alias("W_SLASHSTAR_INSIDE_OF_COMMENT", "TPP_W_SLASHSTAR_INSIDE_OF_COMMENT");
alias("W_LINE_COMMENT_CONTINUED", "TPP_W_LINE_COMMENT_CONTINUED");
alias("W_NOT_ENGOUH_MACRO_ARGUMENTS", "TPP_W_TOO_FEW_ARGUMENTS");
alias("W_STRING_TERMINATED_BY_LINEFEED", "TPP_W_STRING_TERMINATED_BY_LINEFEED");
alias("W_STRING_TERMINATED_BY_EOF", "TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF");
alias("W_COMMENT_TERMINATED_BY_EOF", "TPP_W_COMMENT_TERMINATED_BY_EOF");
alias("W_ENCOUNTERED_TRIGRAPH", "TPP_W_ENCOUNTERED_TRIGRAPH");
alias("W_EXPECTED_STRING_IN_EXPRESSION", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_MESSAGE", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_DEPRECATED", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_TPP_EXEC", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_TPP_SETF", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_TPP_STRAT", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_TPP_CNTTOK", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_PRGERROR", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_EXTENSION", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_TPP_INCPTH", "TPP_W_EXPECTED_STRING");
alias("W_DEFINED_IN_MACRO_BODY", "TPP_W_EXPANSION_TO_DEFINED");
alias("W_EXPECTED_STRING_AFTER_IDENT", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_GCC_DIAG", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_LPAREN_AFTER_VA_OPT", "TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT");
alias("W_EXPECTED_RPAREN_AFTER_VA_OPT", "TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT");
alias("W_IF_WITHOUT_ENDIF", "TPP_W_EOF_BEFORE_ENDIF");
alias("W_ENDIF_WITHOUT_IF", "TPP_W_ENDIF_WITHOUT_IF");
alias("W_ELSE_WITHOUT_IF", "TPP_W_ELIF_OR_ELSE_WITHOUT_IF");
alias("W_ELSE_AFTER_ELSE", "TPP_W_ELIF_OR_ELSE_AFTER_ELSE");
alias("W_ELIF_WITHOUT_IF", "TPP_W_ELIF_OR_ELSE_WITHOUT_IF");
alias("W_ELIF_AFTER_ELSE", "TPP_W_ELIF_OR_ELSE_AFTER_ELSE");
alias("W_EXPECTED_KWDLPAR_AFTER_DEFINED", "TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED");
alias("W_EXPECTED_KEYWORD_AFTER_DEFINED", "TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED");
alias("W_EXPECTED_RPAREN_AFTER_DEFINED", "TPP_W_UNEXPECTED_TOKEN");
alias("W_EXPECTED_COLON_AFTER_QUESTION", "TPP_W_UNEXPECTED_TOKEN");
alias("W_EXPECTED_COLLON_AFTER_QUESTION", "TPP_W_UNEXPECTED_TOKEN");
alias("W_INVALID_INTEGER", "TPP_W_INVALID_INTEGER");
alias("W_EXPECTED_RPAREN_IN_EXPRESSION", "TPP_W_UNEXPECTED_TOKEN");
alias("W_UNKNOWN_TOKEN_IN_EXPR_IS_ZERO", "TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION");
alias("W_EXPECTED_STRING_AFTER_PRAGMA", "TPP_W_EXPECTED_STRING");
alias("W_DIVIDE_BY_ZERO", "TPP_W_DIVIDE_BY_ZERO");
alias("W_CHARACTER_TOO_LONG", "TPP_W_MULTICHAR_LITERAL");
alias("W_MULTICHAR_NOT_ALLOWED", "TPP_W_MULTICHAR_LITERAL");
alias("W_EXPECTED_RBRACKET_IN_EXPRESSION", "TPP_W_UNEXPECTED_TOKEN");
alias("W_EXPECTED_COMMA", "TPP_W_UNEXPECTED_TOKEN");
alias("W_INTEGRAL_OVERFLOW", "TPP_W_INVALID_INTEGER");
alias("W_EXPECTED_ELSE_IN_EXPRESSION", "TPP_W_UNEXPECTED_TOKEN");
alias("W_EXPECTED_INCLUDE_STRING", "TPP_W_EXPECTED_INCLUDE_STRING");
alias("W_FILE_NOT_FOUND", "TPP_W_NO_SUCH_FILE");
alias("W_EXPECTED_STRING_AFTER_LINE", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_COLON_AFTER_WARNING", "TPP_W_UNEXPECTED_TOKEN");
alias("W_EXPECTED_COLLON_AFTER_WARNING", "TPP_W_UNEXPECTED_TOKEN");
alias("W_UNKNOWN_EXTENSION", "TPP_W_UNKNOWN_EXTENSION");
alias("W_CANT_POP_EXTENSIONS", "TPP_W_CANNOT_POP_EXTENSIONS");
]]]*/
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_EOF TPP_TOK_EOF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_CHAR
#define TOK_CHAR TPP_TOK_CHAR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_CHAR */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_STRING
#define TOK_STRING TPP_TOK_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_STRING */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_INT
#define TOK_INT TPP_TOK_INT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_INT */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_FLOAT
#define TOK_FLOAT TPP_TOK_FLOAT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_FLOAT */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LF TPP_TOK_LF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SPACE TPP_TOK_SPACE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_ADD TPP_TOK_PLUS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_ADD TPP_TOK_PLUS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_AND TPP_TOK_AMP
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_AND TPP_TOK_AMP
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_ASSIGN TPP_TOK_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_ASSIGN TPP_TOK_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_AT TPP_TOK_AT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_BACKSLASH TPP_TOK_BACKSLASH
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_COLON TPP_TOK_COLON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_COMMA TPP_TOK_COMMA
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_DIV TPP_TOK_SLASH
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_DIV TPP_TOK_SLASH
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_DOT TPP_TOK_DOT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_HASH TPP_TOK_POUND
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_HASH TPP_TOK_POUND
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE TPP_TOK_LANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE TPP_TOK_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LBRACKET TPP_TOK_LBRACKET
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RBRACKET TPP_TOK_RBRACKET
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LBRACE TPP_TOK_LBRACE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RBRACE TPP_TOK_RBRACE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LPAREN TPP_TOK_LPAREN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RPAREN TPP_TOK_RPAREN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_MOD TPP_TOK_PERCENT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_MOD TPP_TOK_PERCENT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_MUL TPP_TOK_STAR
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_MUL TPP_TOK_STAR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_NOT TPP_TOK_EXCLAIM
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_NOT TPP_TOK_EXCLAIM
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_OR TPP_TOK_PIPE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_OR TPP_TOK_PIPE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_QUESTION TPP_TOK_QMARK
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_QUESTION TPP_TOK_QMARK
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SEMICOLON TPP_TOK_SEMICOLON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_SUB TPP_TOK_MINUS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SUB TPP_TOK_MINUS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_TILDE TPP_TOK_TILDE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_XOR TPP_TOK_HAT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_XOR TPP_TOK_HAT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
#define TPP_TOK_SHL TPP_TOK_LANGLE_LANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SHL TPP_TOK_LANGLE_LANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
#define TPP_TOK_SHR TPP_TOK_RANGLE_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SHR TPP_TOK_RANGLE_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_EQUAL_EQUAL
#define TOK_EQUAL TPP_TOK_EQUAL_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
#define TPP_TOK_NOT_EQUAL TPP_TOK_EXCLAIM_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_NOT_EQUAL TPP_TOK_EXCLAIM_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#define TPP_TOK_GREATER_EQUAL TPP_TOK_RANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_GREATER_EQUAL TPP_TOK_RANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#define TPP_TOK_LOWER_EQUAL TPP_TOK_LANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LOWER_EQUAL TPP_TOK_LANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
#define TPP_TOK_DOTS TPP_TOK_DOT_DOT_DOT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_DOTS TPP_TOK_DOT_DOT_DOT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
#define TPP_TOK_ADD_EQUAL TPP_TOK_PLUS_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_ADD_EQUAL TPP_TOK_PLUS_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
#define TPP_TOK_SUB_EQUAL TPP_TOK_MINUS_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SUB_EQUAL TPP_TOK_MINUS_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
#define TPP_TOK_MUL_EQUAL TPP_TOK_STAR_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_MUL_EQUAL TPP_TOK_STAR_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL
#define TPP_TOK_DIV_EQUAL TPP_TOK_SLASH_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_DIV_EQUAL TPP_TOK_SLASH_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
#define TPP_TOK_MOD_EQUAL TPP_TOK_PERCENT_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_MOD_EQUAL TPP_TOK_PERCENT_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
#define TPP_TOK_SHL_EQUAL TPP_TOK_LANGLE_LANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SHL_EQUAL TPP_TOK_LANGLE_LANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
#define TPP_TOK_SHR_EQUAL TPP_TOK_RANGLE_RANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SHR_EQUAL TPP_TOK_RANGLE_RANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
#define TPP_TOK_AND_EQUAL TPP_TOK_AMP_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_AND_EQUAL TPP_TOK_AMP_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
#define TPP_TOK_OR_EQUAL TPP_TOK_PIPE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_OR_EQUAL TPP_TOK_PIPE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
#define TPP_TOK_XOR_EQUAL TPP_TOK_HAT_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_XOR_EQUAL TPP_TOK_HAT_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
#define TPP_TOK_POW_EQUAL TPP_TOK_STAR_STAR_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_POW_EQUAL TPP_TOK_STAR_STAR_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_AT_EQUAL
#define TOK_AT_EQUAL TPP_TOK_AT_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_POUND_POUND
#define TPP_TOK_GLUE TPP_TOK_POUND_POUND
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_GLUE TPP_TOK_POUND_POUND
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_AMP_AMP
#define TPP_TOK_LAND TPP_TOK_AMP_AMP
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LAND TPP_TOK_AMP_AMP
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
#define TPP_TOK_LOR TPP_TOK_PIPE_PIPE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LOR TPP_TOK_PIPE_PIPE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_HAT_HAT
#define TPP_TOK_LXOR TPP_TOK_HAT_HAT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LXOR TPP_TOK_HAT_HAT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
#define TPP_TOK_INC TPP_TOK_PLUS_PLUS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_INC TPP_TOK_PLUS_PLUS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
#define TPP_TOK_DEC TPP_TOK_MINUS_MINUS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_DEC TPP_TOK_MINUS_MINUS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_STAR
#define TPP_TOK_POW TPP_TOK_STAR_STAR
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_POW TPP_TOK_STAR_STAR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_TILDE_TILDE
#define TOK_TILDE_TILDE TPP_TOK_TILDE_TILDE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
#define TPP_TOK_ARROW TPP_TOK_MINUS_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_ARROW TPP_TOK_MINUS_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_COLON_EQUAL
#define TOK_COLON_EQUAL TPP_TOK_COLON_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
#define TPP_TOK_NAMESPACE TPP_TOK_COLON_COLON
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_NAMESPACE TPP_TOK_COLON_COLON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
#define TPP_TOK_ARROW_STAR TPP_TOK_MINUS_RANGLE_STAR
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_ARROW_STAR TPP_TOK_MINUS_RANGLE_STAR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_DOT_STAR
#define TOK_DOT_STAR TPP_TOK_DOT_STAR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_DOT_DOT
#define TPP_TOK_DOTDOT TPP_TOK_DOT_DOT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_DOTDOT TPP_TOK_DOT_DOT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#define TPP_TOK_LOGT TPP_TOK_LANGLE_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LOGT TPP_TOK_LANGLE_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
#define TPP_TOK_LANGLE3 TPP_TOK_LANGLE_LANGLE_LANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE3 TPP_TOK_LANGLE_LANGLE_LANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
#define TPP_TOK_RANGLE3 TPP_TOK_RANGLE_RANGLE_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE3 TPP_TOK_RANGLE_RANGLE_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#define TPP_TOK_LANGLE3_EQUAL TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE3_EQUAL TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#define TPP_TOK_RANGLE3_EQUAL TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE3_EQUAL TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
#define TPP_TOK_EQUAL3 TPP_TOK_EQUAL_EQUAL_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_EQUAL3 TPP_TOK_EQUAL_EQUAL_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
#define TPP_TOK_NOT_EQUAL3 TPP_TOK_EXCLAIM_EQUAL_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_NOT_EQUAL3 TPP_TOK_EXCLAIM_EQUAL_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_QMARK_QMARK
#define TOK_QMARK_QMARK TPP_TOK_QMARK_QMARK
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_KEYWORD_BEGIN TPP_TOK_KEYWORD_BEGIN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_TWOCHAR_END TPP_TOK_MULTICHAR_END
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_TWOCHAR_END TPP_TOK_MULTICHAR_END
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_POS TPP_TOK_PLUS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_POS TPP_TOK_PLUS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_NEG TPP_TOK_MINUS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_NEG TPP_TOK_MINUS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_LOWER TPP_TOK_LANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LOWER TPP_TOK_LANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_GREATER TPP_TOK_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_GREATER TPP_TOK_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_COLON_COLON
#define TOK_COLON_COLON TPP_TOK_COLON_COLON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#define TPP_TOK_LOWER_GREATER TPP_TOK_LANGLE_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LOWER_GREATER TPP_TOK_LANGLE_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#define TOK_LANGLE_RANGLE TPP_TOK_LANGLE_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#define TPP_TOK_LANGLE1 TPP_TOK_LANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE1 TPP_TOK_LANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
#define TPP_TOK_LANGLE2 TPP_TOK_LANGLE_LANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE2 TPP_TOK_LANGLE_LANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#define TOK_LANGLE_EQUAL TPP_TOK_LANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#define TPP_TOK_LANGLE1_EQUAL TPP_TOK_LANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE1_EQUAL TPP_TOK_LANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
#define TPP_TOK_LANGLE2_EQUAL TPP_TOK_LANGLE_LANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE2_EQUAL TPP_TOK_LANGLE_LANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#define TPP_TOK_RANGLE1 TPP_TOK_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE1 TPP_TOK_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
#define TPP_TOK_RANGLE2 TPP_TOK_RANGLE_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE2 TPP_TOK_RANGLE_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#define TOK_RANGLE_EQUAL TPP_TOK_RANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#define TPP_TOK_RANGLE1_EQUAL TPP_TOK_RANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE1_EQUAL TPP_TOK_RANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
#define TPP_TOK_RANGLE2_EQUAL TPP_TOK_RANGLE_RANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE2_EQUAL TPP_TOK_RANGLE_RANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#define TPP_TOK_COLLON TPP_TOK_COLON
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_COLLON TPP_TOK_COLON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
#define TPP_TOK_COLLON_EQUAL TPP_TOK_COLON_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_COLLON_EQUAL TPP_TOK_COLON_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
#define TPP_TOK_COLLON_COLLON TPP_TOK_COLON_COLON
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_COLLON_COLLON TPP_TOK_COLON_COLON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_if)
#define KWD_if TPP_KWD_if
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_if */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_ifdef)
#define KWD_ifdef TPP_KWD_ifdef
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_ifdef */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_ifndef)
#define KWD_ifndef TPP_KWD_ifndef
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_ifndef */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_elif)
#define KWD_elif TPP_KWD_elif
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_elif */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_else)
#define KWD_else TPP_KWD_else
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_else */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_endif)
#define KWD_endif TPP_KWD_endif
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_endif */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_define)
#define KWD_define TPP_KWD_define
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_define */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_defined)
#define KWD_defined TPP_KWD_defined
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_defined */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_undef)
#define KWD_undef TPP_KWD_undef
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_undef */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_include)
#define KWD_include TPP_KWD_include
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_include */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_include_next)
#define KWD_include_next TPP_KWD_include_next
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_include_next */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_import)
#define KWD_import TPP_KWD_import
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_import */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_line)
#define KWD_line TPP_KWD_line
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_line */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_error)
#define KWD_error TPP_KWD_error
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_error */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_warning)
#define KWD_warning TPP_KWD_warning
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_warning */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_ident)
#define KWD_ident TPP_KWD_ident
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_ident */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_sccs)
#define KWD_sccs TPP_KWD_sccs
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_sccs */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_assert)
#define KWD_assert TPP_KWD_assert
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_assert */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_unassert)
#define KWD_unassert TPP_KWD_unassert
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_unassert */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_pragma)
#define KWD_pragma TPP_KWD_pragma
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_pragma */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD__Pragma)
#define KWD__Pragma TPP_KWD__Pragma
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD__Pragma */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___pragma)
#define KWD___pragma TPP_KWD___pragma
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___pragma */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___FILE__)
#define KWD___FILE__ TPP_KWD___FILE__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___FILE__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___LINE__)
#define KWD___LINE__ TPP_KWD___LINE__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___LINE__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TIME__)
#define KWD___TIME__ TPP_KWD___TIME__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TIME__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___DATE__)
#define KWD___DATE__ TPP_KWD___DATE__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___DATE__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___BASE_FILE__)
#define KWD___BASE_FILE__ TPP_KWD___BASE_FILE__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___BASE_FILE__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___INCLUDE_LEVEL__)
#define KWD___INCLUDE_LEVEL__ TPP_KWD___INCLUDE_LEVEL__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___INCLUDE_LEVEL__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___INCLUDE_DEPTH__)
#define KWD___INCLUDE_DEPTH__ TPP_KWD___INCLUDE_DEPTH__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___INCLUDE_DEPTH__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___COUNTER__)
#define KWD___COUNTER__ TPP_KWD___COUNTER__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___COUNTER__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TIMESTAMP__)
#define KWD___TIMESTAMP__ TPP_KWD___TIMESTAMP__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TIMESTAMP__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___COLUMN__)
#define KWD___COLUMN__ TPP_KWD___COLUMN__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___COLUMN__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___is_identifier)
#define KWD___is_identifier TPP_KWD___is_identifier
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___is_identifier */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___is_deprecated)
#define KWD___is_deprecated TPP_KWD___is_deprecated
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___is_deprecated */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___is_poisoned)
#define KWD___is_poisoned TPP_KWD___is_poisoned
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___is_poisoned */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_attribute)
#define KWD___has_attribute TPP_KWD___has_attribute
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_attribute */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_builtin)
#define KWD___has_builtin TPP_KWD___has_builtin
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_builtin */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_cpp_attribute)
#define KWD___has_cpp_attribute TPP_KWD___has_cpp_attribute
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_cpp_attribute */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_declspec_attribute)
#define KWD___has_declspec_attribute TPP_KWD___has_declspec_attribute
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_declspec_attribute */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_feature)
#define KWD___has_feature TPP_KWD___has_feature
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_feature */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_extension)
#define KWD___has_extension TPP_KWD___has_extension
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_extension */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_warning)
#define KWD___has_warning TPP_KWD___has_warning
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_warning */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_known_extension)
#define KWD___has_known_extension TPP_KWD___has_known_extension
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_known_extension */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_known_warning)
#define KWD___has_known_warning TPP_KWD___has_known_warning
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_known_warning */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_include)
#define KWD___has_include TPP_KWD___has_include
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_include */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_include_next)
#define KWD___has_include_next TPP_KWD___has_include_next
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_include_next */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___VA_ARGS__)
#define KWD___VA_ARGS__ TPP_KWD___VA_ARGS__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___VA_ARGS__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___VA_COMMA__)
#define KWD___VA_COMMA__ TPP_KWD___VA_COMMA__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___VA_COMMA__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___VA_NARGS__)
#define KWD___VA_NARGS__ TPP_KWD___VA_NARGS__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___VA_NARGS__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___VA_OPT__)
#define KWD___VA_OPT__ TPP_KWD___VA_OPT__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___VA_OPT__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_EVAL)
#define KWD___TPP_EVAL TPP_KWD___TPP_EVAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_EVAL */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_LOAD_FILE)
#define KWD___TPP_LOAD_FILE TPP_KWD___TPP_LOAD_FILE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_LOAD_FILE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_COUNTER)
#define KWD___TPP_COUNTER TPP_KWD___TPP_COUNTER
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_COUNTER */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_RANDOM)
#define KWD___TPP_RANDOM TPP_KWD___TPP_RANDOM
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_RANDOM */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_STR_DECOMPILE)
#define KWD___TPP_STR_DECOMPILE TPP_KWD___TPP_STR_DECOMPILE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_STR_DECOMPILE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_STR_AT)
#define KWD___TPP_STR_AT TPP_KWD___TPP_STR_AT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_STR_AT */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_STR_SUBSTR)
#define KWD___TPP_STR_SUBSTR TPP_KWD___TPP_STR_SUBSTR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_STR_SUBSTR */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_STR_PACK)
#define KWD___TPP_STR_PACK TPP_KWD___TPP_STR_PACK
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_STR_PACK */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_STR_SIZE)
#define KWD___TPP_STR_SIZE TPP_KWD___TPP_STR_SIZE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_STR_SIZE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_UNIQUE)
#define KWD___TPP_UNIQUE TPP_KWD___TPP_UNIQUE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_UNIQUE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_COUNT_TOKENS)
#define KWD___TPP_COUNT_TOKENS TPP_KWD___TPP_COUNT_TOKENS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_COUNT_TOKENS */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_IDENTIFIER)
#define KWD___TPP_IDENTIFIER TPP_KWD___TPP_IDENTIFIER
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_IDENTIFIER */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___DATE_DAY__)
#define KWD___DATE_DAY__ TPP_KWD___DATE_DAY__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___DATE_DAY__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___DATE_WDAY__)
#define KWD___DATE_WDAY__ TPP_KWD___DATE_WDAY__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___DATE_WDAY__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___DATE_YDAY__)
#define KWD___DATE_YDAY__ TPP_KWD___DATE_YDAY__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___DATE_YDAY__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___DATE_MONTH__)
#define KWD___DATE_MONTH__ TPP_KWD___DATE_MONTH__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___DATE_MONTH__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___DATE_YEAR__)
#define KWD___DATE_YEAR__ TPP_KWD___DATE_YEAR__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___DATE_YEAR__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TIME_SEC__)
#define KWD___TIME_SEC__ TPP_KWD___TIME_SEC__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TIME_SEC__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TIME_MIN__)
#define KWD___TIME_MIN__ TPP_KWD___TIME_MIN__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TIME_MIN__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TIME_HOUR__)
#define KWD___TIME_HOUR__ TPP_KWD___TIME_HOUR__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TIME_HOUR__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_dollar_is_alpha)
#define KWD_tpp_dollar_is_alpha TPP_KWD_tpp_dollar_is_alpha
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_dollar_is_alpha */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_va_args)
#define KWD_tpp_va_args TPP_KWD_tpp_va_args
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_va_args */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_named_va_args)
#define KWD_tpp_named_va_args TPP_KWD_tpp_named_va_args
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_named_va_args */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_va_comma)
#define KWD_tpp_va_comma TPP_KWD_tpp_va_comma
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_va_comma */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_msvc_integer_suffix)
#define KWD_tpp_msvc_integer_suffix TPP_KWD_tpp_msvc_integer_suffix
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_msvc_integer_suffix */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_charize_operator)
#define KWD_tpp_charize_operator TPP_KWD_tpp_charize_operator
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_charize_operator */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_trigraphs)
#define KWD_tpp_trigraphs TPP_KWD_tpp_trigraphs
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_trigraphs */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_digraphs)
#define KWD_tpp_digraphs TPP_KWD_tpp_digraphs
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_digraphs */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_reemit_unknown_pragmas)
#define KWD_tpp_reemit_unknown_pragmas TPP_KWD_tpp_reemit_unknown_pragmas
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_reemit_unknown_pragmas */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_push_macro)
#define KWD_tpp_pragma_push_macro TPP_KWD_tpp_pragma_push_macro
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_push_macro */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_pop_macro)
#define KWD_tpp_pragma_pop_macro TPP_KWD_tpp_pragma_pop_macro
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_pop_macro */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_region)
#define KWD_tpp_pragma_region TPP_KWD_tpp_pragma_region
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_region */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_endregion)
#define KWD_tpp_pragma_endregion TPP_KWD_tpp_pragma_endregion
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_endregion */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_warning)
#define KWD_tpp_pragma_warning TPP_KWD_tpp_pragma_warning
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_warning */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_message)
#define KWD_tpp_pragma_message TPP_KWD_tpp_pragma_message
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_message */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_error)
#define KWD_tpp_pragma_error TPP_KWD_tpp_pragma_error
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_error */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_once)
#define KWD_tpp_pragma_once TPP_KWD_tpp_pragma_once
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_once */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_tpp_exec)
#define KWD_tpp_pragma_tpp_exec TPP_KWD_tpp_pragma_tpp_exec
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_tpp_exec */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_deprecated)
#define KWD_tpp_pragma_deprecated TPP_KWD_tpp_pragma_deprecated
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_deprecated */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_tpp_set_keyword_flags)
#define KWD_tpp_pragma_tpp_set_keyword_flags TPP_KWD_tpp_pragma_tpp_set_keyword_flags
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_tpp_set_keyword_flags */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_directive_include_next)
#define KWD_tpp_directive_include_next TPP_KWD_tpp_directive_include_next
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_directive_include_next */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_directive_import)
#define KWD_tpp_directive_import TPP_KWD_tpp_directive_import
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_directive_import */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_directive_warning)
#define KWD_tpp_directive_warning TPP_KWD_tpp_directive_warning
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_directive_warning */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_lxor)
#define KWD_tpp_lxor TPP_KWD_tpp_lxor
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_lxor */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_token_tilde_tilde)
#define KWD_tpp_token_tilde_tilde TPP_KWD_tpp_token_tilde_tilde
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_token_tilde_tilde */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_token_pow)
#define KWD_tpp_token_pow TPP_KWD_tpp_token_pow
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_token_pow */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_token_lxor)
#define KWD_tpp_token_lxor TPP_KWD_tpp_token_lxor
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_token_lxor */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_token_arrow)
#define KWD_tpp_token_arrow TPP_KWD_tpp_token_arrow
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_token_arrow */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_token_colon_assign)
#define KWD_tpp_token_colon_assign TPP_KWD_tpp_token_colon_assign
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_token_colon_assign */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_token_colon_colon)
#define KWD_tpp_token_colon_colon TPP_KWD_tpp_token_colon_colon
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_token_colon_colon */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_macro_calling_conventions)
#define KWD_tpp_macro_calling_conventions TPP_KWD_tpp_macro_calling_conventions
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_macro_calling_conventions */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_strict_whitespace)
#define KWD_tpp_strict_whitespace TPP_KWD_tpp_strict_whitespace
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_strict_whitespace */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_strict_integer_overflow)
#define KWD_tpp_strict_integer_overflow TPP_KWD_tpp_strict_integer_overflow
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_strict_integer_overflow */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_support_ansi_characters)
#define KWD_tpp_support_ansi_characters TPP_KWD_tpp_support_ansi_characters
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_support_ansi_characters */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_emit_lf_after_directive)
#define KWD_tpp_emit_lf_after_directive TPP_KWD_tpp_emit_lf_after_directive
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_emit_lf_after_directive */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_if_cond_expression)
#define KWD_tpp_if_cond_expression TPP_KWD_tpp_if_cond_expression
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_if_cond_expression */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_debug)
#define KWD_tpp_debug TPP_KWD_tpp_debug
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_debug */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define KWD___TPP_VERSION__ TPP_KWD___TPP_VERSION__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_EXT_TRIGRAPHS)
#define EXT_TRIGRAPHS TPP_EXT_TRIGRAPHS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_EXT_TRIGRAPHS */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_EXT_DIGRAPHS)
#define EXT_DIGRAPHS TPP_EXT_DIGRAPHS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_EXT_DIGRAPHS */
#ifdef TPP_EXT_NAMED_VARARGS_IN_MACROS
#define TPP_EXT_GCC_VA_ARGS TPP_EXT_NAMED_VARARGS_IN_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_GCC_VA_ARGS TPP_EXT_NAMED_VARARGS_IN_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_NAMED_VARARGS_IN_MACROS */
#ifdef TPP_EXT_VA_GLUE_COMMA_IN_MACROS
#define TPP_EXT_GCC_VA_COMMA TPP_EXT_VA_GLUE_COMMA_IN_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_GCC_VA_COMMA TPP_EXT_VA_GLUE_COMMA_IN_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_VA_GLUE_COMMA_IN_MACROS */
#ifdef TPP_EXT_VA_COMMA_IN_MACROS
#define TPP_EXT_VA_COMMA TPP_EXT_VA_COMMA_IN_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_VA_COMMA TPP_EXT_VA_COMMA_IN_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_VA_COMMA_IN_MACROS */
#ifdef TPP_EXT_VA_OPT_IN_MACROS
#define TPP_EXT_VA_OPT TPP_EXT_VA_OPT_IN_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_VA_OPT TPP_EXT_VA_OPT_IN_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_VA_OPT_IN_MACROS */
#ifdef TPP_EXT_VA_NARGS_IN_MACROS
#define TPP_EXT_VA_NARGS TPP_EXT_VA_NARGS_IN_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_VA_NARGS TPP_EXT_VA_NARGS_IN_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_VA_NARGS_IN_MACROS */
#ifdef TPP_EXT_VA_ARGS_IN_MACROS
#define TPP_EXT_VA_ARGS TPP_EXT_VA_ARGS_IN_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_VA_ARGS TPP_EXT_VA_ARGS_IN_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_VA_ARGS_IN_MACROS */
#ifdef TPP_EXT_ESCAPE_E_IN_STRINGS
#define TPP_EXT_STR_E TPP_EXT_ESCAPE_E_IN_STRINGS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_STR_E TPP_EXT_ESCAPE_E_IN_STRINGS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_ESCAPE_E_IN_STRINGS */
#ifdef TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS
#define TPP_EXT_ALTMAC TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_ALTMAC TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS */
#ifdef TPP_EXT_MACRO_RECURSION
#define TPP_EXT_RECMAC TPP_EXT_MACRO_RECURSION
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_RECMAC TPP_EXT_MACRO_RECURSION
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_RECURSION */
#ifdef TPP_EXT_MACRO_ARGUMENT_WHITESPACE
#define TPP_EXT_ARGSPACE TPP_EXT_MACRO_ARGUMENT_WHITESPACE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_ARGSPACE TPP_EXT_MACRO_ARGUMENT_WHITESPACE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_ARGUMENT_WHITESPACE */
#ifdef TPP_EXT_MACRO___pragma
#define TPP_EXT_MSVC_PRAGMA TPP_EXT_MACRO___pragma
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_MSVC_PRAGMA TPP_EXT_MACRO___pragma
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO___pragma */
#ifdef TPP_EXT_CHARIZE_MACRO_ARGUMENT
#define TPP_EXT_HASH_AT TPP_EXT_CHARIZE_MACRO_ARGUMENT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_HASH_AT TPP_EXT_CHARIZE_MACRO_ARGUMENT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CHARIZE_MACRO_ARGUMENT */
#ifdef TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT
#define TPP_EXT_HASH_XCLAIM TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_HASH_XCLAIM TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT */
#ifdef TPP_EXT_CPP_WARNING
#define TPP_EXT_WARNING TPP_EXT_CPP_WARNING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_WARNING TPP_EXT_CPP_WARNING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CPP_WARNING */
#ifdef TPP_EXT_CPP_EXCLAIM
#define TPP_EXT_SHEBANG TPP_EXT_CPP_EXCLAIM
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_SHEBANG TPP_EXT_CPP_EXCLAIM
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CPP_EXCLAIM */
#ifdef TPP_EXT_CPP_INCLUDE_NEXT
#define TPP_EXT_INCLUDE_NEXT TPP_EXT_CPP_INCLUDE_NEXT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_INCLUDE_NEXT TPP_EXT_CPP_INCLUDE_NEXT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CPP_INCLUDE_NEXT */
#ifdef TPP_EXT_CPP_IMPORT
#define TPP_EXT_IMPORT TPP_EXT_CPP_IMPORT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_IMPORT TPP_EXT_CPP_IMPORT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CPP_IMPORT */
#ifdef TPP_EXT_CPP_IDENT_SCCS
#define TPP_EXT_IDENT_SCCS TPP_EXT_CPP_IDENT_SCCS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_IDENT_SCCS TPP_EXT_CPP_IDENT_SCCS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CPP_IDENT_SCCS */
#ifdef TPP_EXT_MACRO___BASE_FILE__
#define TPP_EXT_BASEFILE TPP_EXT_MACRO___BASE_FILE__
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_BASEFILE TPP_EXT_MACRO___BASE_FILE__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO___BASE_FILE__ */
#ifdef TPP_EXT_MACRO___INCLUDE_LEVEL__
#define TPP_EXT_INCLUDE_LEVEL TPP_EXT_MACRO___INCLUDE_LEVEL__
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_INCLUDE_LEVEL TPP_EXT_MACRO___INCLUDE_LEVEL__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO___INCLUDE_LEVEL__ */
#ifdef TPP_EXT_MACRO___COUNTER__
#define TPP_EXT_COUNTER TPP_EXT_MACRO___COUNTER__
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_COUNTER TPP_EXT_MACRO___COUNTER__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO___COUNTER__ */
#ifdef TPP_EXT_MACRO___TIMESTAMP__
#define TPP_EXT_TIMESTAMP TPP_EXT_MACRO___TIMESTAMP__
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TIMESTAMP TPP_EXT_MACRO___TIMESTAMP__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO___TIMESTAMP__ */
#ifdef TPP_EXT_MACRO___COLUMN__
#define TPP_EXT_COLUMN TPP_EXT_MACRO___COLUMN__
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_COLUMN TPP_EXT_MACRO___COLUMN__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO___COLUMN__ */
#ifdef TPP_EXT_NUMERIC_DATE_MACROS
#define TPP_EXT_DATEUTILS TPP_EXT_NUMERIC_DATE_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_DATEUTILS TPP_EXT_NUMERIC_DATE_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_NUMERIC_DATE_MACROS */
#ifdef TPP_EXT_NUMERIC_TIME_MACROS
#define TPP_EXT_TIMEUTILS TPP_EXT_NUMERIC_TIME_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TIMEUTILS TPP_EXT_NUMERIC_TIME_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_NUMERIC_TIME_MACROS */
#ifdef TPP_EXT_MACRO_TPP_EVAL
#define TPP_EXT_TPP_EVAL TPP_EXT_MACRO_TPP_EVAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_EVAL TPP_EXT_MACRO_TPP_EVAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_EVAL */
#ifdef TPP_EXT_MACRO_TPP_UNIQUE
#define TPP_EXT_TPP_UNIQUE TPP_EXT_MACRO_TPP_UNIQUE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_UNIQUE TPP_EXT_MACRO_TPP_UNIQUE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_UNIQUE */
#ifdef TPP_EXT_MACRO_TPP_LOAD_FILE
#define TPP_EXT_TPP_LOAD_FILE TPP_EXT_MACRO_TPP_LOAD_FILE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_LOAD_FILE TPP_EXT_MACRO_TPP_LOAD_FILE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_LOAD_FILE */
#ifdef TPP_EXT_MACRO_TPP_COUNTER
#define TPP_EXT_TPP_COUNTER TPP_EXT_MACRO_TPP_COUNTER
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_COUNTER TPP_EXT_MACRO_TPP_COUNTER
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_COUNTER */
#ifdef TPP_EXT_MACRO_TPP_RANDOM
#define TPP_EXT_TPP_RANDOM TPP_EXT_MACRO_TPP_RANDOM
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_RANDOM TPP_EXT_MACRO_TPP_RANDOM
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_RANDOM */
#ifdef TPP_EXT_MACRO_TPP_STR_DECOMPILE
#define TPP_EXT_TPP_STR_DECOMPILE TPP_EXT_MACRO_TPP_STR_DECOMPILE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_STR_DECOMPILE TPP_EXT_MACRO_TPP_STR_DECOMPILE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_STR_DECOMPILE */
#ifdef TPP_EXT_MACRO_TPP_STR_SUBSTR
#define TPP_EXT_TPP_STR_SUBSTR TPP_EXT_MACRO_TPP_STR_SUBSTR
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_STR_SUBSTR TPP_EXT_MACRO_TPP_STR_SUBSTR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_STR_SUBSTR */
#ifdef TPP_EXT_MACRO_TPP_STR_PACK
#define TPP_EXT_TPP_STR_PACK TPP_EXT_MACRO_TPP_STR_PACK
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_STR_PACK TPP_EXT_MACRO_TPP_STR_PACK
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_STR_PACK */
#ifdef TPP_EXT_MACRO_TPP_STR_SIZE
#define TPP_EXT_TPP_STR_SIZE TPP_EXT_MACRO_TPP_STR_SIZE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_STR_SIZE TPP_EXT_MACRO_TPP_STR_SIZE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_STR_SIZE */
#ifdef TPP_EXT_MACRO_TPP_COUNT_TOKENS
#define TPP_EXT_TPP_COUNT_TOKENS TPP_EXT_MACRO_TPP_COUNT_TOKENS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_COUNT_TOKENS TPP_EXT_MACRO_TPP_COUNT_TOKENS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_COUNT_TOKENS */
#ifdef TPP_EXT_MACRO_TPP_IDENTIFIER
#define TPP_EXT_TPP_IDENTIFIER TPP_EXT_MACRO_TPP_IDENTIFIER
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_IDENTIFIER TPP_EXT_MACRO_TPP_IDENTIFIER
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_IDENTIFIER */
#ifdef TPP_EXT_CPP_ASSERT
#define TPP_EXT_ASSERTIONS TPP_EXT_CPP_ASSERT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_ASSERTIONS TPP_EXT_CPP_ASSERT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CPP_ASSERT */
#ifdef TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR
#define TPP_EXT_NO_EXPAND_DEFINED TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_NO_EXPAND_DEFINED TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR */
#ifdef TPP_EXT_TRADITIONAL_MACROS
#define TPP_EXT_TRADITIONAL_MACRO TPP_EXT_TRADITIONAL_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TRADITIONAL_MACRO TPP_EXT_TRADITIONAL_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_TRADITIONAL_MACROS */
#ifdef TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES
#define TPP_EXT_EXT_ARE_FEATURES TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_EXT_ARE_FEATURES TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES */
#ifdef TPP_EXT_BUILTIN_EXPR_STRINGS
#define TPP_EXT_STRINGOPS TPP_EXT_BUILTIN_EXPR_STRINGS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_STRINGOPS TPP_EXT_BUILTIN_EXPR_STRINGS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_BUILTIN_EXPR_STRINGS */
#ifdef TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT
#define TPP_EXT_GCC_IFELSE TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_GCC_IFELSE TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT */
#ifdef TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
#define TPP_EXT_IFELSE_IN_EXPR TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_IFELSE_IN_EXPR TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS */
#ifdef TPP_EXT_BUILTIN_EXPR_LOGICAL_XOR
#define TPP_EXT_LXOR TPP_EXT_BUILTIN_EXPR_LOGICAL_XOR
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_LXOR TPP_EXT_BUILTIN_EXPR_LOGICAL_XOR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_BUILTIN_EXPR_LOGICAL_XOR */
#ifdef TPP_EXT_BUILTIN_EXPR_BINARY_LITERALS
#define TPP_EXT_BININTEGRAL TPP_EXT_BUILTIN_EXPR_BINARY_LITERALS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_BININTEGRAL TPP_EXT_BUILTIN_EXPR_BINARY_LITERALS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_BUILTIN_EXPR_BINARY_LITERALS */
#ifdef TPP_EXT_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
#define TPP_EXT_MSVC_FIXED_INT TPP_EXT_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_MSVC_FIXED_INT TPP_EXT_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_COMMENT)
#define WG_COMMENT TPP_WG_COMMENT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_COMMENT */
#ifdef TPP_WG_COMMENT
#define TPP_WG_COMMENTS TPP_WG_COMMENT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define WG_COMMENTS TPP_WG_COMMENT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_WG_COMMENT */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_MACROS)
#define WG_MACROS TPP_WG_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_MACROS */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_SYNTAX)
#define WG_SYNTAX TPP_WG_SYNTAX
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_SYNTAX */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_VALUE)
#define WG_VALUE TPP_WG_VALUE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_VALUE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_USER)
#define WG_USER TPP_WG_USER
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_USER */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_UNDEF)
#define WG_UNDEF TPP_WG_UNDEF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_UNDEF */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_TRIGRAPHS)
#define WG_TRIGRAPHS TPP_WG_TRIGRAPHS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_TRIGRAPHS */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_EXPANSION_TO_DEFINED)
#define WG_EXPANSION_TO_DEFINED TPP_WG_EXPANSION_TO_DEFINED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_EXPANSION_TO_DEFINED */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_DEPRECATED)
#define WG_DEPRECATED TPP_WG_DEPRECATED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_DEPRECATED */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_ENVIRON)
#define WG_ENVIRON TPP_WG_ENVIRON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_ENVIRON */
#ifdef TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#define TPP_W_EXPECTED_KEYWORD_AFTER_DEFINE TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_KEYWORD_AFTER_DEFINE TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
#ifdef TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#define TPP_W_EXPECTED_KEYWORD_AFTER_UNDEF TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_KEYWORD_AFTER_UNDEF TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
#ifdef TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#define TPP_W_EXPECTED_KEYWORD_AFTER_IFDEF TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_KEYWORD_AFTER_IFDEF TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
#ifdef TPP_W_UNKNOWN_DIRECTIVE
#define TPP_W_UNKNOWN_PREPROCESSOR_DIRECTIVE TPP_W_UNKNOWN_DIRECTIVE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_UNKNOWN_PREPROCESSOR_DIRECTIVE TPP_W_UNKNOWN_DIRECTIVE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNKNOWN_DIRECTIVE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_ERROR)
#define W_ERROR TPP_W_ERROR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_ERROR */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_WARNING)
#define W_WARNING TPP_W_WARNING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_WARNING */
#ifdef TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#define TPP_W_EXPECTED_MACRO_ARGUMENT_NAME TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_MACRO_ARGUMENT_NAME TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
#ifdef TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#define TPP_W_EXPECTED_ARGEND_AFTER_VARARGS TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_ARGEND_AFTER_VARARGS TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
#ifdef TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#define TPP_W_EXPECTED_COMMA_OR_ARGEND TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_COMMA_OR_ARGEND TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
#ifdef TPP_W_TOO_MANY_ARGUMENTS
#define TPP_W_TOO_MANY_MACRO_ARGUMENTS TPP_W_TOO_MANY_ARGUMENTS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_TOO_MANY_MACRO_ARGUMENTS TPP_W_TOO_MANY_ARGUMENTS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_TOO_MANY_ARGUMENTS */
#ifdef TPP_W_EOF_IN_ARGUMENT_LIST
#define TPP_W_EOF_IN_MACRO_ARGUMENT_LIST TPP_W_EOF_IN_ARGUMENT_LIST
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EOF_IN_MACRO_ARGUMENT_LIST TPP_W_EOF_IN_ARGUMENT_LIST
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EOF_IN_ARGUMENT_LIST */
#ifdef TPP_W_REDEFINE_MACRO
#define TPP_W_REDEFINING_MACRO TPP_W_REDEFINE_MACRO
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_REDEFINING_MACRO TPP_W_REDEFINE_MACRO
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_REDEFINE_MACRO */
#ifdef TPP_W_CANNOT_UNDEF_BUILTIN_MACRO
#define TPP_W_CANT_UNDEF_BUILTIN_MACRO TPP_W_CANNOT_UNDEF_BUILTIN_MACRO
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_CANT_UNDEF_BUILTIN_MACRO TPP_W_CANNOT_UNDEF_BUILTIN_MACRO
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_CANNOT_UNDEF_BUILTIN_MACRO */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_PUSHMACRO TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_PUSHMACRO TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_TPP_STRD TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_TPP_STRD TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_LPAREN TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_LPAREN TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_RPAREN TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_RPAREN TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#ifdef TPP_W_DEPRECATED_KEYWORD
#define TPP_W_DEPRECATED_IDENTIFIER TPP_W_DEPRECATED_KEYWORD
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_DEPRECATED_IDENTIFIER TPP_W_DEPRECATED_KEYWORD
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_DEPRECATED_KEYWORD */
#ifdef TPP_W_DUPLICATE_MACRO_PARAMETER_NAME
#define TPP_W_ARGUMENT_NAMED_ALREADY_TAKEN TPP_W_DUPLICATE_MACRO_PARAMETER_NAME
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_ARGUMENT_NAMED_ALREADY_TAKEN TPP_W_DUPLICATE_MACRO_PARAMETER_NAME
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_DUPLICATE_MACRO_PARAMETER_NAME */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_SLASHSTAR_INSIDE_OF_COMMENT)
#define W_SLASHSTAR_INSIDE_OF_COMMENT TPP_W_SLASHSTAR_INSIDE_OF_COMMENT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_SLASHSTAR_INSIDE_OF_COMMENT */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_LINE_COMMENT_CONTINUED)
#define W_LINE_COMMENT_CONTINUED TPP_W_LINE_COMMENT_CONTINUED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_LINE_COMMENT_CONTINUED */
#ifdef TPP_W_TOO_FEW_ARGUMENTS
#define TPP_W_NOT_ENGOUH_MACRO_ARGUMENTS TPP_W_TOO_FEW_ARGUMENTS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_NOT_ENGOUH_MACRO_ARGUMENTS TPP_W_TOO_FEW_ARGUMENTS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_TOO_FEW_ARGUMENTS */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_STRING_TERMINATED_BY_LINEFEED)
#define W_STRING_TERMINATED_BY_LINEFEED TPP_W_STRING_TERMINATED_BY_LINEFEED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_STRING_TERMINATED_BY_LINEFEED */
#ifdef TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
#define TPP_W_STRING_TERMINATED_BY_EOF TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_STRING_TERMINATED_BY_EOF TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_COMMENT_TERMINATED_BY_EOF)
#define W_COMMENT_TERMINATED_BY_EOF TPP_W_COMMENT_TERMINATED_BY_EOF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_COMMENT_TERMINATED_BY_EOF */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_ENCOUNTERED_TRIGRAPH)
#define W_ENCOUNTERED_TRIGRAPH TPP_W_ENCOUNTERED_TRIGRAPH
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_ENCOUNTERED_TRIGRAPH */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_IN_EXPRESSION TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_IN_EXPRESSION TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_MESSAGE TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_MESSAGE TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_DEPRECATED TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_DEPRECATED TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_TPP_EXEC TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_TPP_EXEC TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_TPP_SETF TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_TPP_SETF TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_TPP_STRAT TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_TPP_STRAT TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_TPP_CNTTOK TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_TPP_CNTTOK TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_PRGERROR TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_PRGERROR TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_EXTENSION TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_EXTENSION TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_TPP_INCPTH TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_TPP_INCPTH TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPANSION_TO_DEFINED
#define TPP_W_DEFINED_IN_MACRO_BODY TPP_W_EXPANSION_TO_DEFINED
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_DEFINED_IN_MACRO_BODY TPP_W_EXPANSION_TO_DEFINED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPANSION_TO_DEFINED */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_IDENT TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_IDENT TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_GCC_DIAG TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_GCC_DIAG TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT)
#define W_EXPECTED_LPAREN_AFTER_VA_OPT TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT)
#define W_EXPECTED_RPAREN_AFTER_VA_OPT TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT */
#ifdef TPP_W_EOF_BEFORE_ENDIF
#define TPP_W_IF_WITHOUT_ENDIF TPP_W_EOF_BEFORE_ENDIF
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_IF_WITHOUT_ENDIF TPP_W_EOF_BEFORE_ENDIF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EOF_BEFORE_ENDIF */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_ENDIF_WITHOUT_IF)
#define W_ENDIF_WITHOUT_IF TPP_W_ENDIF_WITHOUT_IF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_ENDIF_WITHOUT_IF */
#ifdef TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#define TPP_W_ELSE_WITHOUT_IF TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_ELSE_WITHOUT_IF TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_ELIF_OR_ELSE_WITHOUT_IF */
#ifdef TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#define TPP_W_ELSE_AFTER_ELSE TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_ELSE_AFTER_ELSE TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_ELIF_OR_ELSE_AFTER_ELSE */
#ifdef TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#define TPP_W_ELIF_WITHOUT_IF TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_ELIF_WITHOUT_IF TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_ELIF_OR_ELSE_WITHOUT_IF */
#ifdef TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#define TPP_W_ELIF_AFTER_ELSE TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_ELIF_AFTER_ELSE TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_ELIF_OR_ELSE_AFTER_ELSE */
#ifdef TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED
#define TPP_W_EXPECTED_KWDLPAR_AFTER_DEFINED TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_KWDLPAR_AFTER_DEFINED TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED */
#ifdef TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED
#define TPP_W_EXPECTED_KEYWORD_AFTER_DEFINED TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_KEYWORD_AFTER_DEFINED TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_RPAREN_AFTER_DEFINED TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_RPAREN_AFTER_DEFINED TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_COLON_AFTER_QUESTION TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_COLON_AFTER_QUESTION TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_COLLON_AFTER_QUESTION TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_COLLON_AFTER_QUESTION TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_INVALID_INTEGER)
#define W_INVALID_INTEGER TPP_W_INVALID_INTEGER
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_INVALID_INTEGER */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_RPAREN_IN_EXPRESSION TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_RPAREN_IN_EXPRESSION TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#ifdef TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION
#define TPP_W_UNKNOWN_TOKEN_IN_EXPR_IS_ZERO TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_UNKNOWN_TOKEN_IN_EXPR_IS_ZERO TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_PRAGMA TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_PRAGMA TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_DIVIDE_BY_ZERO)
#define W_DIVIDE_BY_ZERO TPP_W_DIVIDE_BY_ZERO
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_DIVIDE_BY_ZERO */
#ifdef TPP_W_MULTICHAR_LITERAL
#define TPP_W_CHARACTER_TOO_LONG TPP_W_MULTICHAR_LITERAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_CHARACTER_TOO_LONG TPP_W_MULTICHAR_LITERAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_MULTICHAR_LITERAL */
#ifdef TPP_W_MULTICHAR_LITERAL
#define TPP_W_MULTICHAR_NOT_ALLOWED TPP_W_MULTICHAR_LITERAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_MULTICHAR_NOT_ALLOWED TPP_W_MULTICHAR_LITERAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_MULTICHAR_LITERAL */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_RBRACKET_IN_EXPRESSION TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_RBRACKET_IN_EXPRESSION TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_COMMA TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_COMMA TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#ifdef TPP_W_INVALID_INTEGER
#define TPP_W_INTEGRAL_OVERFLOW TPP_W_INVALID_INTEGER
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_INTEGRAL_OVERFLOW TPP_W_INVALID_INTEGER
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_INVALID_INTEGER */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_ELSE_IN_EXPRESSION TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_ELSE_IN_EXPRESSION TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_EXPECTED_INCLUDE_STRING)
#define W_EXPECTED_INCLUDE_STRING TPP_W_EXPECTED_INCLUDE_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_EXPECTED_INCLUDE_STRING */
#ifdef TPP_W_NO_SUCH_FILE
#define TPP_W_FILE_NOT_FOUND TPP_W_NO_SUCH_FILE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_FILE_NOT_FOUND TPP_W_NO_SUCH_FILE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_NO_SUCH_FILE */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_LINE TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_LINE TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_COLON_AFTER_WARNING TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_COLON_AFTER_WARNING TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_COLLON_AFTER_WARNING TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_COLLON_AFTER_WARNING TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_UNKNOWN_EXTENSION)
#define W_UNKNOWN_EXTENSION TPP_W_UNKNOWN_EXTENSION
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_UNKNOWN_EXTENSION */
#ifdef TPP_W_CANNOT_POP_EXTENSIONS
#define TPP_W_CANT_POP_EXTENSIONS TPP_W_CANNOT_POP_EXTENSIONS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_CANT_POP_EXTENSIONS TPP_W_CANNOT_POP_EXTENSIONS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_CANNOT_POP_EXTENSIONS */
/*[[[end]]]*/

//TODO: /* #define TPP_CONFIG_CALLBACK_WARNING          x // int x(int wnum, ...) { ... } -- A user-replacement for `TPPLexer_Warn' */
//TODO: /* #define TPP_CONFIG_CALLBACK_PARSE_PRAGMA     x // int TPPCALL x(void) { ... } */
//TODO: /* #define TPP_CONFIG_CALLBACK_PARSE_PRAGMA_GCC x // int TPPCALL x(void) { ... } */
//TODO: /* #define TPP_CONFIG_CALLBACK_INS_COMMENT      x // int TPPCALL x(struct TPPString *tpp_restrict comment) { ... } */
//TODO: /* #define TPP_CONFIG_CALLBACK_NEW_TEXTFILE     x // int TPPCALL x(struct TPPFile *tpp_restrict file, int is_system_header) { ... } */
//TODO: /* #define TPP_CONFIG_CALLBACK_UNKNOWN_FILE     x // struct TPPFile *TPPCALL x(int mode, char *tpp_restrict filename, size_t filename_size, struct TPPKeyword **pkeyword_entry) */
//TODO: /* #define TPP_CONFIG_NO_CALLBACK_PARSE_PRAGMA     1 */
//TODO: /* #define TPP_CONFIG_NO_CALLBACK_PARSE_PRAGMA_GCC 1 */
//TODO: /* #define TPP_CONFIG_NO_CALLBACK_INS_COMMENT      1 */
//TODO: /* #define TPP_CONFIG_NO_CALLBACK_NEW_TEXTFILE     1 */
//TODO: /* #define TPP_CONFIG_NO_CALLBACK_UNKNOWN_FILE     1 */


//TODO:/*36*/ DEF_WARNING(W_MACRO_NOT_DEFINED, (WG_MACROS), WSTATE_DISABLED, WARNF("Macro " Q("%s") " is not defined", KWDNAME()))                                /* [struct TPPKeyword *] OLD(TPPWarn_MacroDoesntExist). */
//TODO:/*50*/ DEF_WARNING(W_INVALID_WARNING, (WG_VALUE), WSTATE_ERROR, {
//TODO:	/* [struct TPPConst *] OLD(TPPWarn_InvalidWarning). */
//TODO:	struct TPPConst *c = ARG(struct TPPConst *);
//TODO:	if (c->c_kind == TPP_CONST_STRING) {
//TODO:		char const *wname = c->c_data.c_string->s_text;
//TODO:		if (*wname == '-')
//TODO:			++wname;
//TODO:		if (*wname == 'W')
//TODO:			++wname;
//TODO:		if (!memcmp(wname, "no-", 3))
//TODO:			wname += 3;
//TODO:		WARNF("Invalid warning " Q("%s") " (Did you mean " Q("%s") ")", wname, find_most_likely_warning(wname));
//TODO:	} else if (c->c_kind == TPP_CONST_FLOAT) {
//TODO:		WARNF("Invalid warning " Q("%f"), (double)c->c_data.c_float);
//TODO:	} else if (c->c_kind == TPP_CONST_INTEGRAL) {
//TODO:		WARNF("Invalid warning " Q("%ld"), (long)c->c_data.c_int);
//TODO:	} else {
//TODO:		WARNF("Invalid warning");
//TODO:	}
//TODO:})
//TODO:/*51*/ DEF_WARNING(W_CANT_POP_WARNINGS, (WG_VALUE), WSTATE_ERROR, WARNF("Can't pop warnings"))                                                                       /* OLD(TPPWarn_CantPopWarnings). */
//TODO:/*61*/ DEF_WARNING(W_NONPARTABLE_FILENAME_CASING, (WG_ENVIRON), WSTATE_WARN, {
//TODO:	/* [char const *,char const *,size_t,char const *]. (path,wrong_begin,wrong_size,corrent_begin)
//TODO:	 *  OLD(TPPWarn_InvalidPathCasing). */
//TODO:	char *temp;
//TODO:	char *temp2;
//TODO:	size_t temp3;
//TODO:	temp  = ARG(char *);
//TODO:	temp2 = ARG(char *);
//TODO:	temp3 = ARG(size_t);
//TODO:	WARNF("Non-portable casing in " Q("%s") ": " Q("%.*s") " should be " Q("%s") " instead",
//TODO:	      temp, (int)temp3, temp2, ARG(char *));
//TODO:})
//TODO:/*64*/ DEF_WARNING(W_SPECIAL_ARGUMENT_NAME, (WG_MACROS), WSTATE_WARN, WARNF("Special keyword " Q("%s") " used as argument name", KWDNAME())) /* [struct TPPKeyword *] OLD(TPPWarn_VaArgsUsedAsMacroParameter). */
//TODO:/*68*/ WARNING(W_EXPECTED_BOOL, (WG_BOOLVALUE, WG_VALUE), WSTATE_DISABLED)            /* [struct TPPConst *] OLD(TPPWarn_ExpectedBoolExpression). */
//TODO:/*69*/ WARNING(W_EXPECTED_BOOL_UNARY, (WG_BOOLVALUE, WG_VALUE), WSTATE_DISABLED)      /* [struct TPPConst *] OLD(TPPWarn_ExpectedBoolExpressionNot). */
//TODO:/*70*/ WARNING(W_EXPECTED_BOOL_BINARY_LHS, (WG_BOOLVALUE, WG_VALUE), WSTATE_DISABLED) /* [struct TPPConst *] OLD(TPPWarn_ExpectedBoolExpressionLhsOP). */
//TODO:/*71*/ WARNING(W_EXPECTED_BOOL_BINARY_RHS, (WG_BOOLVALUE, WG_VALUE), WSTATE_DISABLED) /* [struct TPPConst *] OLD(TPPWarn_ExpectedBoolExpressionRhsOP). */

/* Warnings added by the new TPP2. */
//TODO:DEF_WARNING(W_VA_KEYWORD_IN_REGULAR_MACRO, (WG_MACROS), WSTATE_WARN, WARNF("Variadic keyword " Q("%s") " used in regular macro", KWDNAME()))                    /* [struct TPPKeyword *]. */
//TODO:DEF_WARNING(W_KEYWORD_MACRO_ALREADY_ONSTACK, (WG_MACROS), WSTATE_DISABLED, WARNF("Keyword-style macro " Q("%s") " is already being expanded", FILENAME()))      /* [struct TPPFile *]. */
//TODO:DEF_WARNING(W_FUNCTION_MACRO_ALREADY_ONSTACK, (WG_MACROS), WSTATE_DISABLED, WARNF("Function-style macro " Q("%s") " is expanded to the same text", FILENAME())) /* [struct TPPFile *]. */
//TODO:DEF_WARNING(W_INDEX_OUT_OF_BOUNDS, (WG_VALUE), WSTATE_DISABLED, { struct TPPString *s = ARG(struct TPPString *); WARNF("Index %ld is out-of-bounds of 0..%lu", (unsigned long)s->s_size, (unsigned long)ARG(ptrdiff_t)); })                                                                                            /* [struct TPPString *,ptrdiff_t]. */
//TODO:DEF_WARNING(W_MACRO_RECURSION_LIMIT_EXCEEDED, (WG_LIMIT), WSTATE_ERROR, WARNF("Macro recursion limit exceeded when expanding " Q("%s") " (Consider passing " Q("-fno-macro-recursion") ")", FILENAME())) /* [struct TPPFile *]. */
//TODO:DEF_WARNING(W_INCLUDE_RECURSION_LIMIT_EXCEEDED, (WG_LIMIT), WSTATE_ERROR, WARNF("Include recursion limit exceeded when including " Q("%s"), FILENAME()))                                                 /* [struct TPPFile *]. */
//TODO:DEF_WARNING(W_IDENT_SCCS_IGNORED, (WG_USAGE), WSTATE_WARN, WARNF("#ident/sccs with " Q("%s") " is ignored", CONST_STR())) /* [struct TPPConst *]. */
//TODO:DEF_WARNING(W_EXPECTED_KEYWORD_AFTER_ASSERT, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected keyword after #assert, but got " TOK_S, TOK_A))                                        /* . */
//TODO:DEF_WARNING(W_EXPECTED_KEYWORD_AFTER_PREDICATE, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected keyword after predicate " Q("%s") " in #assert, but got " TOK_S, KWDNAME(), TOK_A)) /* [struct TPPKeyword *]. */
//TODO:DEF_WARNING(W_EXPECTED_KEYWORD_AFTER_EXPR_HASH, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected keyword after # in expression, but got " TOK_S, TOK_A)) /* . */
//TODO:DEF_WARNING(W_EXPECTED_KEYWORD_AFTER_EXPR_PRED, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected keyword after predicate " Q("%s") " in expression, but got " TOK_S, KWDNAME(), TOK_A)) /* . */
//TODO:DEF_WARNING(W_UNKNOWN_ASSERTION, (WG_VALUE), WSTATE_DISABLED, { char const *temp = KWDNAME(); WARNF("Assertion " Q("%s") " does not contain a predicate " Q("%s"), temp, KWDNAME()); }) /* [struct TPPKeyword *,struct TPPKeyword *]. */
//TODO:DEF_WARNING(W_CANT_POP_INCLUDE_PATH, (WG_VALUE), WSTATE_ERROR, WARNF("Can't pop #include paths"))                                                                               /* . */
//TODO:DEF_WARNING(W_CONSIDER_PAREN_AROUND_LAND, (WG_QUALITY), WSTATE_WARN, WARNF("Consider adding parenthesis around " Q("&&") " to prevent confusion with " Q("||")))                /* . */
//TODO:DEF_WARNING(W_INTEGRAL_CLAMPED, (WG_VALUE), WSTATE_WARN, WARNF("Integral constant clamped to fit")) /* [tint_t,tint_t]. */
//TODO:DEF_WARNING(W_UNKNOWN_INCLUDE_PATH, (WG_VALUE), WSTATE_WARN, { char *temp = ARG(char *); WARNF("Unknown system #include-path " Q("%.*s"), (int)ARG(size_t), temp); })                                   /* [char const *,size_t]. */
//TODO:DEF_WARNING(W_INCLUDE_PATH_ALREADY_EXISTS, (WG_VALUE), WSTATE_WARN, { char *temp = ARG(char *); WARNF("System #include-path " Q("%.*s") " already exists", (int)ARG(size_t), temp); })                            /* [char const *,size_t]. */
//TODO:DEF_WARNING(W_STATEMENT_IN_EXPRESSION, (WG_USAGE, WG_SYNTAX), WSTATE_WARN, WARNF("GCC-style statement " TOK_S " in expression is not understood", TOK_A))                                                  /* . */
//TODO:DEF_WARNING(W_TYPECAST_IN_EXPRESSION, (WG_USAGE, WG_SYNTAX), WSTATE_WARN, WARNF("C-style type cast " TOK_S " in expression is not understood (Consider using bit-masks to narrow integral types)", TOK_A)) /* . */
//TODO:DEF_WARNING(W_EXPECTED_RPAREN_AFTER_CAST, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected " Q(")") " after casting type, but got " TOK_S, TOK_A))                                                               /* . */
//TODO:DEF_WARNING(W_EXPECTED_RBRACE_AFTER_STATEMENT, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected " Q("}") " after statement, but got " TOK_S, TOK_A))                                                             /* . */
//TODO:DEF_WARNING(W_EXPECTED_WARNING_NAMEORID, (WG_SYNTAX, WG_VALUE), WSTATE_ERROR, WARNF("Expected warning name or id, but got " Q("%s"), CONST_STR()))                                                         /* [struct TPPConst *]. */
//TODO:DEF_WARNING(W_INVALID_FLOAT_SUFFIX, (WG_SYNTAX), WSTATE_ERROR, { char *temp = ARG(char *); WARNF("Invalid floating point suffix " Q("%.*s"), (int)ARG(size_t), temp); }) /* [char const *,size_t] */
//TODO:DEF_WARNING(W_DEPENDENCY_CHANGED, (WG_DEPENDENCY), WSTATE_ERROR, {
//TODO:	char *depnam         = ARG(char *);
//TODO:	char *srcnam         = ARG(char *);
//TODO:	char *reason         = ARG(char *);
//TODO:	size_t reason_length = ARG(size_t);
//TODO:	WARNF("Dependency " Q("%s") " changed after " Q("%s"), depnam, srcnam);
//TODO:	if (reason_length)
//TODO:		WARNF(" (" Q("%.*s") ")", (unsigned int)reason_length, reason);
//TODO:})




/* API Compatibility */

#if 0 /* TODO: Not directly portable (write migration notes for this stuff) */
//struct TPPTextFile {
//	/* [owned((:f_name) = true]
//	 * HINT: `:f_name' usually is the string passed to the
//	 *        system for opening a stream for this file.
//	 *        Though this doesn't necessarily have to hold up,
//	 *        as pipe text files don't need to follow this rule. */
//	/*ref*/struct TPPFile   *f_cacheentry;  /* [0..1] Used when the copy of a file is loaded onto the #include-stack (points to the original version of the file)
//	                                         * NOTE: When non-NULL, always a textfile and this pointer also owns a reference to the associated textfile's `f_cacheinc' field. */
//	/*ref*/struct TPPString *f_usedname;    /* [0..1] When non-NULL, an override to the used filename (as set by `#line') */
//	TPP(line_t)              f_lineoff;     /* Offset of `f_begin' from the original start of the file in lines. */
//	TPP(stream_t)            f_stream;      /* Stream handle for reading more data. */
//	TPP(stream_t)            f_ownedstream; /* Usually equal to `f_stream', the stream that should be closed when it's EOF is reached (set to `TPP_STREAM_INVALID' if TPP shouldn't close the stream). */
//	/* NOTE: `:f_end' may not be equal to the end of `:f_text'.
//	 *        The difference between the two should be prefixed to the next chunk. */
//	struct TPPKeyword       *f_guard;       /* [0..1] Name of the #include guard for this file, or NULL if unknown. */
//	size_t                   f_cacheinc;    /* Used to track how often a given file is loaded onto the #include-stack.
//	                                         * (Only used for cached entires themself; aka. when `f_cacheentry == NULL'). */
//	size_t                   f_rdata;       /* (In bytes) The amount of data already read from the stream. */
//	char                     f_prefixdel;   /* The original character at `:f_end' that was overwritten with a `\0'. */
//#define TPP_TEXTFILE_FLAG_NONE         0x00 /* No special flags. */
//#define TPP_TEXTFILE_FLAG_NOGUARD      0x01 /* Set after a secondary #ifdef block was detected at the top level of this file:
//                                             * >> // File: "myfile.h"
//                                             * >> #ifndef foo // Potential include-guard?
//                                             * >> #endif
//                                             * >> #ifndef bar // This #ifndef will set `TPP_TEXTFILE_FLAG_NOGUARD'
//                                             * >> #endif */
//#define TPP_TEXTFILE_FLAG_SYSHEADER    0x02 /* This file is a system-header and all non-error warnings are suppressed. */
//#ifdef TPP_CONFIG_NONBLOCKING_IO
//#define TPP_TEXTFILE_FLAG_NONBLOCK     0x40 /* Do not block when reading file data.
//                                             * NOTE: Still block if waiting for the end of a non-escaped
//                                             *       linefeed, of the end of a comment, or string. */
//#endif /* TPP_CONFIG_NONBLOCKING_IO */
//#define TPP_TEXTFILE_FLAG_INTERNAL     0x80 /* This file is internal, meaning it shouldn't ~really~ represent a line/col number. */
//	unsigned char            f_flags;       /* A set of `TPP_TEXTFILE_FLAG_*' */
//	TPP(encoding_t)          f_encoding;    /* Encoding determined to-be used by this file. */
//	char                     f_padding[1];  /* Padding data... */
//	struct TPPKeyword       *f_newguard;    /* [0..1] The keyword of the #ifndef block that was determined to be located at
//	                                         *        the start of the file. When the file is popped from the #include-stack,
//	                                         *        this is non-NULL and `TPP_TEXTFILE_FLAG_NOGUARD' isn't set, this keyword
//	                                         *        will be copied into the `f_guard' field if not already set. */
//#ifndef TPP_CONFIG_NO_PRECACHE_TEXTLINES
//	char const              *f_lfpos;       /* [?..?] Cached line-feed counter pointer.
//	                                         * This pointer is only valid if it points
//	                                         * into the file's current text-block. */
//	size_t                   f_lfcnt;       /* [valid_if(is_valid(f_lfpos))] Number of line-feeds
//	                                         * that can be found between the file's start and `f_lfpos'. */
//#endif /* TPP_CONFIG_NO_PRECACHE_TEXTLINES */
//#ifdef TPP_USERTEXTDATA
//	TPP_USERTEXTDATA /* Optional user-defined data memory (when present, initialized to ZERO) */
//#endif /* TPP_USERTEXTDATA */
//};
//

///* Without the ident/sccs extension, disable the insert-comment callback. */
//#if (defined(TPP_CONFIG_EXTENSION_IDENT_SCCS) && !TPP_CONFIG_EXTENSION_IDENT_SCCS)
//#undef TPP_CONFIG_NO_CALLBACK_INS_COMMENT
//#undef TPP_CONFIG_CALLBACK_INS_COMMENT
//#undef TPP_CONFIG_DYN_CALLBACK_INS_COMMENT
//#define TPP_CONFIG_NO_CALLBACK_INS_COMMENT 1
//#endif
//
//
///* Figure out which callbacks should be dynamically linked at runtime. */
//#undef TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA
//#undef TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA_GCC
//#undef TPP_CONFIG_DYN_CALLBACK_INS_COMMENT
//#undef TPP_CONFIG_DYN_CALLBACK_NEW_TEXTFILE
//#undef TPP_CONFIG_DYN_CALLBACK_UNKNOWN_FILE
//#if (!defined(TPP_CONFIG_NO_CALLBACK_PARSE_PRAGMA) && \
//     !defined(TPP_CONFIG_CALLBACK_PARSE_PRAGMA))
//#define TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA 1
//#endif /* ... */
//#if (!defined(TPP_CONFIG_NO_CALLBACK_PARSE_PRAGMA_GCC) && \
//     !defined(TPP_CONFIG_CALLBACK_PARSE_PRAGMA_GCC))
//#define TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA_GCC 1
//#endif /* ... */
//#if (!defined(TPP_CONFIG_NO_CALLBACK_INS_COMMENT) && \
//     !defined(TPP_CONFIG_CALLBACK_INS_COMMENT))
//#define TPP_CONFIG_DYN_CALLBACK_INS_COMMENT 1
//#endif /* ... */
//#if (!defined(TPP_CONFIG_NO_CALLBACK_NEW_TEXTFILE) && \
//     !defined(TPP_CONFIG_CALLBACK_NEW_TEXTFILE))
//#define TPP_CONFIG_DYN_CALLBACK_NEW_TEXTFILE 1
//#endif /* ... */
//#if (!defined(TPP_CONFIG_NO_CALLBACK_UNKNOWN_FILE) && \
//     !defined(TPP_CONFIG_CALLBACK_UNKNOWN_FILE))
//#define TPP_CONFIG_DYN_CALLBACK_UNKNOWN_FILE 1
//#endif /* ... */
//#if (defined(TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA) ||     \
//     defined(TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA_GCC) || \
//     defined(TPP_CONFIG_DYN_CALLBACK_INS_COMMENT) ||      \
//     defined(TPP_CONFIG_DYN_CALLBACK_NEW_TEXTFILE) ||     \
//     defined(TPP_CONFIG_DYN_CALLBACK_UNKNOWN_FILE))
//#define TPP_CONFIG_DYN_CALLBACKS 1
//#endif /* ... */
//
//
//#ifdef TPP_CONFIG_DYN_CALLBACKS
//struct TPPCallbacks {
//	/* Optional user-hooks for implementing special preprocessor behavior.
//	 * NOTE: Any function pointer in here may be specified as NULL. */
//#ifdef TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA
//	/* Handle an unknown pragma.
//	 *  - The lexer currently points to the pragma's first token
//	 *    and is configured not to continue yielding tokens once
//	 *    the pragma's effective end is reached, as well as
//	 *    to ignore comment, space and LF tokens:
//	 *    >> #pragma foo bar   // [foo][bar][EOF]
//	 *    >> _Pragma("baz(2)") // [baz][(][2][)][EOF]
//	 *    >> __pragma(x*y)     // [x][*][y][EOF]
//	 * @return: 0: Unknown/errorous pragma.
//	 * @return: 1: Successfully parsed the given pragma. */
//	int (TPPCALL *c_parse_pragma)(void);
//#endif /* TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA */
//
//#ifdef TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA_GCC
//	/* Same as `c_parse_pragma', but invoked for unknown GCC-namespace pragmas
//	 * >> #pragma GCC visibility(push)
//	 *                ^^^^^^^^^^ Invoked on this token */
//	int (TPPCALL *c_parse_pragma_gcc)(void);
//#endif /* TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA_GCC */
//
//#ifdef TPP_CONFIG_DYN_CALLBACK_INS_COMMENT
//	/* Insert the given text into the ".comment" section of the current object file.
//	 * @return: 0: Error occurred (Set a lexer error if not already set)
//	 * @return: 1: Successfully inserted the given text. */
//	int (TPPCALL *c_ins_comment)(struct TPPString *tpp_restrict comment);
//#endif /* TPP_CONFIG_DYN_CALLBACK_INS_COMMENT */
//
//#ifdef TPP_CONFIG_DYN_CALLBACK_NEW_TEXTFILE
//	/* Event-callback invoked when a textfile is included the first time.
//	 * >> Very useful for generating dependency trees.
//	 * NOTE: This function will only ever be called once
//	 *       for any given file within the same lexer.
//	 * @return: 0: Error occurred (Set a lexer error if not already set)
//	 * @return: 1: Continue parsing (same as not filling in this member). */
//	int (TPPCALL *c_new_textfile)(struct TPPFile *tpp_restrict file, int is_system_header);
//#endif /* TPP_CONFIG_DYN_CALLBACK_NEW_TEXTFILE */
//
//#if TPP_CONFIG_DYN_CALLBACK_UNKNOWN_FILE
//	/* Called when a given filename could not be found, allowing this
//	 * function to attempt more voodoo-magic in an attempt to locate it.
//	 * @param: mode:                    Set of `TPPLEXER_OPENFILE_FLAG_*' that are being used to open the file.
//	 * @return: NULL:                   Still failed to find the file (unless a lexer error was set, only emit a warning)
//	 * @return: * :                     The file we now managed to successfully open.
//	 * @return: TPP_UNKNOWN_FILE_RETRY: Instruct the file loader to try again (without invoking this function on that try)
//	 * WARNING: This callback is responsible to caching the file in a keyword entry!
//	 */
//	struct TPPFile *(TPPCALL *c_unknown_file)(int mode, char *tpp_restrict filename,
//	                                          size_t filename_size,
//	                                          struct TPPKeyword **pkeyword_entry);
//#endif /* TPP_CONFIG_DYN_CALLBACK_UNKNOWN_FILE */
//};
//#endif /* TPP_CONFIG_DYN_CALLBACKS */
//
//#ifndef TPP_CONFIG_NO_CALLBACK_UNKNOWN_FILE
///* Return value by `c_unknown_file' to indicate that the open should be re-attempted. */
//#define TPP_UNKNOWN_FILE_RETRY ((struct TPPFile *)-1)
//#endif /* !TPP_CONFIG_NO_CALLBACK_UNKNOWN_FILE */

#endif

#define TPP_SYMARRAY_SIZE TPP_FLEX_ARRAY

#ifndef TPP_CONFIG_ONELEXER
/* Globally provide only one lexer (faster, but more restrictive).
 * 0: Have a global "struct TPPLexer *TPPLexer_Current"
 * 1: Have a global "struct TPPLexer TPPLexer_Global"
 * 2: Have a global "struct TPPLexer TPPLexer_Global" that can be backed-up/restored using "memcpy"
 * 3: Pass the current lexer to functions via arguments.*/
#define TPP_CONFIG_ONELEXER 1
#endif /* !TPP_CONFIG_ONELEXER */

#if TPP_CONFIG_ONELEXER == 3
#define TPP_LEXER_PARAM  tpp_lexer *tpp_restrict _current
#define TPP_LEXER_PARAM_ tpp_lexer *tpp_restrict _current,
#define TPP_LEXER__PARAM , tpp_lexer *tpp_restrict _current
#define TPP_LEXER_ARG    _current
#define TPP_LEXER_ARG_   _current,
#define TPP_LEXER__ARG   , _current
#ifndef TPP2_LEXER
#define TPP2_LEXER       _current
#endif /* !TPP2_LEXER */
#else /* TPP_CONFIG_ONELEXER == 3 */
#define TPP_LEXER_PARAM  void
#define TPP_LEXER_PARAM_ /* nothing */
#define TPP_LEXER__PARAM /* nothing */
#define TPP_LEXER_ARG    /* nothing */
#define TPP_LEXER_ARG_   /* nothing */
#define TPP_LEXER__ARG   /* nothing */
#endif /* TPP_CONFIG_ONELEXER != 3 */

/* Used in places where TPP2 never returned an error, but TPP3 can */
#ifndef TPP2_FATAL
#if 1
#include <stdlib.h>
#define TPP2_FATAL(fallback) (abort(), fallback)
#else
#define TPP2_FATAL(fallback) (fallback)
#endif
#endif /* !TPP2_FATAL */

TPP_DECL_BEGIN

#ifndef TPPLexer_Current
#ifdef TPP2_LEXER
#define TPP2_DEFAULT_Lexer_Current TPP2_LEXER
#elif TPP_CONFIG_ONELEXER == 3
#define TPP2_DEFAULT_Lexer_Current _current
#elif TPP_CONFIG_ONELEXER != 0
#define TPP2_DEFAULT_Lexer_Current (&TPPLexer_Global)
extern tpp_lexer TPPLexer_Global;
#else /* ... */
/* [1..1] The currently selected lexer
 * >> When NULL, only certain parts of TPP can work without problems. */
extern tpp_lexer *TPPLexer_Current;
#define TPP2_DEFAULT_Lexer_Current TPPLexer_Current
#endif /* !... */
#define TPPLexer_Current TPP2_DEFAULT_Lexer_Current
#endif /* !TPPLexer_Current */

#ifndef TPP2_LEXER
#define TPP2_LEXER TPPLexer_Current
#endif /* !TPP2_LEXER */





#define TPP_ISOK(id)                   (!TPP_TOK_ISERR(id))
#define TPP_ISKEYWORD(id)              TPP_TOK_ISKEYWORD(id)
#define TPP_ISUSERKEYWORD(id)          TPP_TOK_ISUSERKEYWORD(id)
#define TPP_ISBUILTINMACRO_(lexer, id) tpp_lexer_getkeyworddefined(lexer, tpp_lexer_kwds_getkeyword_byid(self, id))
#define TPP_ISBUILTINMACRO(id)         TPP_ISBUILTINMACRO_(TPP2_LEXER, id)

#define TPP_stream_t   tpp_io_handle
#define TPP_tok_t      tpp_token_id
#define TPP_hash_t     tpp_hash
#define TPP_encoding_t tpp_file_encoding
#define TPP_wgroup_t   tpp_warning_group_id
#define TPP_col_t      tpp_column
#define TPP_line_t     tpp_line
#define TPP_refcnt_t   tpp_refcnt
#define TPP_printer_t  tpp_formatprinter
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define stream_t   tpp_io_handle
#define tok_t      tpp_token_id
#define hash_t     tpp_hash
#define encoding_t tpp_file_encoding
#define wgroup_t   tpp_warning_group_id
#define col_t      tpp_column
#define line_t     tpp_line
#define refcnt_t   tpp_refcnt
#define printer_t  tpp_formatprinter
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */

#define TPP_ENCODING_UTF8     TPP_FILE_ENCODING_UTF8 /* Use TPP_FILE_ENCODING_ISUTF8() */
#define TPP_ENCODING_UTF16_BE TPP_FILE_ENCODING_UTF16_BE
#define TPP_ENCODING_UTF16_LE TPP_FILE_ENCODING_UTF16_LE
#define TPP_ENCODING_UTF32_BE TPP_FILE_ENCODING_UTF32_BE
#define TPP_ENCODING_UTF32_LE TPP_FILE_ENCODING_UTF32_LE

#define TPPKeyword tpp_keyword
#define TPPString  tpp_string
#define s_refcnt   ts_refcnt /* Use tpp_string_isshared() */
#define s_size     ts_len    /* Use tpp_string_len() */
#define s_text     ts_str    /* Use tpp_string_str() */

#define TPPString_TEXT(x)   ((char *)tpp_string_str(x))
#define TPPString_SIZE(x)   tpp_string_len(x)
#define TPPString_Shared(x) tpp_string_isshared(x)
#define TPPString_Free(x)   tpp_string_destroy(x)
#define TPPString_Incref(x) tpp_string_incref(x)
#define TPPString_Decref(x) tpp_string_decref(x)
/*#define TPPString_Cat(lhs, rhs) tpp_string_cat(lhs, rhs)*/
/*#define TPPString_New(text, size) tpp_string_new(text, size)*/
#define TPPString_NewSized(size) tpp_string_malloc(size)
#define TPPString_NewEmpty()     tpp_string_newempty()

#define TPP_funop_t   tpp_macro_opcode
#define TPP_tint_t    tpp_intmax
#define TPP_tuint_t   tpp_uintmax
#define TPP_tfloat_t  tpp_float
#define TPP_arginfo_t tpp_macro_argument
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define funop_t   tpp_macro_opcode
#define tint_t    tpp_intmax
#define tuint_t   tpp_uintmax
#define tfloat_t  tpp_float
#define arginfo_t tpp_macro_argument
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */

#define ai_id tma_id /* Use tpp_macro_getfuncargtok() */
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT
#define ai_ins tma_ins /* Don't access */
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT */
#define ai_ins_exp tma_ins_exp /* Don't access */
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
#define ai_ins_str tma_ins_str /* Don't access */
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
#undef ai_name      /* Use tpp_keyword_getkwdcstr(tpp_lexer_kwds_getkeyword_byid(lexer, tpp_macro_getfuncargtok(macro, i))) */
#undef ai_namesize  /* Use tpp_keyword_getkwdlen(tpp_lexer_kwds_getkeyword_byid(lexer, tpp_macro_getfuncargtok(macro, i))) */

#define TPPLCInfo tpp_lcinfo
#define lc_line   lci_line /* Use tpp_lcinfo_getline() */
#define lc_col    lci_col  /* Use tpp_lcinfo_getcol() */

#define TPPMacroFile          tpp_macro
#define TPPFile               tpp_file
#define TPPFILE_KIND_TEXT     TPP_FILE_KIND_IO
#define TPPFILE_KIND_EXPLICIT TPP_FILE_KIND_TEXT
#define TPPFILE_KIND_MACRO    TPP_FILE_KIND_MACRO
#define f_kind                TPP_INTERNAL(tf_kind)  /* Use tpp_file_getkind() */
#define f_prev                TPP_INTERNAL(tf_tprev) /* Don't access */ /* Or maybe "tf_prev"... */
#define f_name                TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(tff_name) /* Use tpp_file_getrealfilename() */
#define f_text                TPP_INTERNAL(tf_chunk) /* Use tpp_file_getchunk() */
#define f_begin               TPP_INTERNAL(tf_chunk)->TPP_INTERNAL(ts_str) /* Shouldn't be used */
#define f_end                 TPP_INTERNAL(tf_end)   /* Use tpp_file_getend() */
#define f_pos                 TPP_INTERNAL(tf_pos)   /* Use tpp_file_getpos() */

#define TPPFile_LCAt(self, info, text_pointer) \
	(void)(*(info) = tpp_file_getlcinfo(self, (tpp_char const *)(text_pointer)))
#define TPPFile_LineAt(self, text_pointer) \
	tpp_lcinfo_getline(tpp_file_getlcinfo(self, (tpp_char const *)(text_pointer)))
#define TPPFile_ColumnAt(self, text_pointer) \
	tpp_lcinfo_getcol(tpp_file_getlcinfo(self, (tpp_char const *)(text_pointer)))
#define TPPFile_Filename(self, opt_filename_length)                                            \
	((opt_filename_length)                                                                     \
	 ? (void)(*(tpp_size *)(opt_filename_length) = tpp_strlen(tpp_file_getfilename(self))) \
	 : (void)0,                                                                                \
	 tpp_file_getfilename(self))
#define TPPFile_RealFilename(self, opt_filename_length)                                    \
	((opt_filename_length)                                                                 \
	 ? (void)(*(tpp_size *)(opt_filename_length) = tpp_strlen(tpp_file_getrealfilename(self))) \
	 : (void)0,                                                                            \
	 tpp_file_getrealfilename(self))


#define TPP_Itos(buf, i) tpp_itoa(buf, i)
TPP_INLINE tpp_size TPPCALL TPP_SizeofItos(tpp_intmax i) {
	char buf[TPP_ITOA_MAXLEN];
	return (tpp_size)((buf + TPP_ITOA_MAXLEN) - tpp_itoa(buf, i));
}

#define TPP_Ftos(buf, f) ((buf) + tpp_ftoa(buf, f))
TPP_INLINE tpp_size TPPCALL TPP_SizeofFtos(tpp_float f) {
	char buf[TPP_FTOA_MAXLEN];
	return tpp_ftoa(buf, f);
}


TPP_INLINE TPP_FORMATPRINTER_DEFINE(_TPP_Escape_buffer_cb, arg, text, num_bytes) {
	if (arg) {
		char **p_buf = (char **)arg;
		tpp_memcpy(*p_buf, text, num_bytes);
		*p_buf += num_bytes;
	}
	return (tpp_ssize)num_bytes;
}

#define TPP_Escape_(lexer, buf, data, size)  TPP_Escape(buf, data, size)
#define TPP_SizeofEscape_(lexer, data, size) TPP_SizeofEscape(data, size)
#define TPP_SizeofEscape(data, size) \
	((tpp_size)tpp_token_encodestring(&_TPP_Escape_buffer_cb, NULL, data, size))
TPP_INLINE char *TPPCALL
TPP_Escape(char *tpp_restrict buf, char const *tpp_restrict data, size_t size) {
	tpp_token_encodestring(&_TPP_Escape_buffer_cb, &buf, data, size);
	return buf;
}


#define TPP_Hashof(data, size) tpp_hashof((tpp_char const *)(data), size)

#define TPP_wstate_t tpp_warning_state
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define wstate_t        tpp_warning_state
#define WSTATE_DISABLED TPP_WSTATE_DISABLED
#define WSTATE_FATAL    TPP_WSTATE_FATAL
#define WSTATE_WARN     TPP_WSTATE_WARN
#define WSTATE_ERROR    TPP_WSTATE_ERROR
#define WSTATE_SUPPRESS TPP_WSTATE_SUPPRESS
#define WSTATE_DEFAULT  TPP_WSTATE_DEFAULT
#define WSTATE_DISABLE  TPP_WSTATE_DISABLED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_WSTATE_DISABLE TPP_WSTATE_DISABLED

#define TPP_WSTATE_ISENABLED(s) tpp_warning_state_willemit(s)

#if TPP2_HAVE_GLOBAL_NAMESPACE
#define WG_COUNT TPP_WG_COUNT
#define W_COUNT  TPP_W_COUNT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */

#define TPPWarningStateEx      tpp_warning_suppress_item
#define wse_wid                TPP_INTERNAL(twsi_ctx_id)  /* Don't access */
#define wse_suppress           TPP_INTERNAL(twsi_count)   /* Don't access */
#define wse_oldstate           TPP_INTERNAL(twsi_restore) /* Don't access */
#define TPP_WARNING_BITS       2
#define TPP_WARNING_TOTAL      TPP_WC_COUNT
#define TPP_WARNING_BITSETSIZE (((TPP_WARNING_TOTAL * TPP_WARNING_BITS + 7)) / 8)
#define TPPWarningState        tpp_warnings
#define ws_state               TPP_INTERNAL(tw_state).TPP_INTERNAL(tws_bitset)      /* Don't access */
#define ws_extendeda           TPP_INTERNAL(tw_suppressions).TPP_INTERNAL(tws_ctxc) /* Don't access */
#define ws_extendedv           TPP_INTERNAL(tw_suppressions).TPP_INTERNAL(tws_ctxv) /* Don't access */
#define ws_prev                TPP_INTERNAL(tw_prev)                                /* Don't access */

#define TPPLexer_PushWarnings_(lexer) (tpp_lexer_pushwarnings(lexer), 1)
#define TPPLexer_PopWarnings_(lexer)  (tpp_lexer_canpopwarnings(lexer) && (tpp_lexer_pushwarnings(lexer), 1))
#define TPPLexer_PushWarnings()       TPPLexer_PushWarnings_(TPP2_LEXER)
#define TPPLexer_PopWarnings()        TPPLexer_PopWarnings_(TPP2_LEXER)


/* Set the state of a given warning number.
 * NOTE: If the given state is `WSTATE_SUPPRESS', ONE(1)
 *       will be added to the suppress recursion counter.
 * @return: 0: Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
 * @return: 1: Successfully set the given warning number.
 * @return: 2: The given warning number is unknown. */
#define TPPLexer_SetWarning(wnum, state)      TPPLexer_SetWarning_(TPP2_LEXER, wnum, state)
#define TPPLexer_SetWarningGroup(wgrp, state) TPPLexer_SetWarningGroup_(TPP2_LEXER, wgrp, state)
#define TPPLexer_GetWarning(wnum)             TPPLexer_GetWarning_(TPP2_LEXER, wnum)
#define TPPLexer_GetWarningGroup(wgrp)        TPPLexer_GetWarningGroup_(TPP2_LEXER, wgrp)
TPP_INLINE int TPPCALL TPPLexer_SetWarning_(tpp_lexer *self, int wnum, TPP_wstate_t state) {
	tpp_errno error;
	tpp_warning_context_id ctx = tpp_warning_context_id_ofwarning((tpp_warning_id)wnum);
	if ((unsigned int)ctx >= (unsigned int)TPP_WC_COUNT)
		return 2;
	error = tpp_lexer_setwarningctx(self, ctx, state);
	return error == TPP_EOK ? 0 : 1;
}
TPP_INLINE int TPPCALL TPPLexer_SetWarningGroup_(tpp_lexer *self, int wgrp, TPP_wstate_t state) {
	tpp_errno error;
	tpp_warning_context_id ctx = tpp_warning_context_id_ofgroup((tpp_warning_group_id)wgrp);
	if ((unsigned int)ctx >= (unsigned int)TPP_WC_COUNT)
		return 2;
	error = tpp_lexer_setwarningctx(self, ctx, state);
	return error == TPP_EOK ? 0 : 1;
}
TPP_INLINE TPP_wstate_t TPPCALL TPPLexer_GetWarning_(tpp_lexer *self, int wnum) {
	tpp_warning_context_id ctx = tpp_warning_context_id_ofwarning((tpp_warning_id)wnum);
	if ((unsigned int)ctx >= (unsigned int)TPP_WC_COUNT)
		return TPP_WSTATE_DISABLED;
	return tpp_lexer_getwarningctx(self, ctx);
}
TPP_INLINE TPP_wstate_t TPPCALL TPPLexer_GetWarningGroup_(tpp_lexer *self, int wgrp) {
	tpp_warning_context_id ctx = tpp_warning_context_id_ofgroup((tpp_warning_group_id)wgrp);
	if ((unsigned int)ctx >= (unsigned int)TPP_WC_COUNT)
		return TPP_WSTATE_DISABLED;
	return tpp_lexer_getwarningctx(self, ctx);
}

/* Similar to `TPPLexer_SetWarning', but set the state of all warnings from a given group.
 * NOTES:
 *   - Groups work independent of warning ids, meaning you can even
 *     specify `WSTATE_SUPPRESS' as state, with the next warning
 *     part of that group occurring simply consuming that suppression.
 *   - If you disable an entire warning group, no warning apart of it will be emit.
 *   - If a warning is invoked, that is both part of an error and a warning/disabled
 *     group it will always tend to do as little damage as possible:
 *     >> suppress >= disabled >= warning >= error
 *     With that in mind, both the warning itself, as well as all of its groups
 *     must be configured as `WSTATE_FATAL' for the warning to actually result
 *     in an error.
 * @return: 0: Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
 * @return: 1: Successfully set the given warning number.
 * @return: 2: The given group name is unknown. */
TPP_INLINE int TPPCALL
TPPLexer_SetWarnings_(tpp_lexer *self, char const *tpp_restrict group, TPP_wstate_t state) {
	tpp_warning_group_id wgrp = tpp_warning_group_byname(group);
	return TPPLexer_SetWarningGroup_(self, (int)wgrp, state);
}
TPP_INLINE TPP_wstate_t TPPCALL
TPPLexer_GetWarnings_(tpp_lexer *self, char const *tpp_restrict group) {
	tpp_warning_group_id wgrp = tpp_warning_group_byname(group);
	return TPPLexer_GetWarningGroup_(self, (int)wgrp);
}
#define TPPLexer_SetWarnings(group, state) TPPLexer_SetWarnings_(TPP2_LEXER, group, state)
#define TPPLexer_GetWarnings(group)        TPPLexer_GetWarnings_(TPP2_LEXER, group)

#define TPP_WARNINGMODE_FATAL  ((int)TPP_WSTATE_FATAL)
#define TPP_WARNINGMODE_ERROR  ((int)TPP_WSTATE_ERROR)
#define TPP_WARNINGMODE_WARN   ((int)TPP_WSTATE_WARN)
#define TPP_WARNINGMODE_IGNORE ((int)TPP_WSTATE_DISABLED)

/* Invoke a given warning number, returning one of `TPP_WARNINGMODE_*'.
 * NOTE: Unknown warnings will always result in `TPP_WARNINGMODE_WARN' being returned. */
#define TPPLexer_InvokeWarning(wnum) TPPLexer_InvokeWarning_(TPP2_LEXER, wnum)
TPP_INLINE int TPPCALL TPPLexer_InvokeWarning_(tpp_lexer *self, int wnum) {
	tpp_errno error;
	tpp_warning_invokeinfo info;
	if ((unsigned int)wnum >= (unsigned int)TPP_W_COUNT)
		return TPP_WARNINGMODE_IGNORE;
	error = tpp_lexer_invokewarning(self, (tpp_warning_id)wnum, &info);
	if (TPP_ISERR(error))
		return TPP_WARNINGMODE_FATAL; /* Wrong behavior... */
	return (int)info.twii_state;
}



#define TPPIfdefStackSlot tpp_ifdef_stack_entry
/*#define TPP_IFDEFMODE_FALSE 0 * Status doesn't exist in TPP3 anymore */
#define TPP_IFDEFMODE_TRUE  TPP_IFDEF_MODE_IFDEF /* The block is enabled. */
#define TPP_IFDEFMODE_ELSE  TPP_IFDEF_MODE_ELSE  /* The block follows an #else. */
#define iss_mode            TPP_INTERNAL(tidse_mode)
#define iss_line            TPP_INTERNAL(tidse_updated).TPP_INTERNAL(lci_line)
#define iss_file            tpp3_ifdef_stack_is_per_file_so_there_is_no_file_property
#define TPPIfdefStack       tpp_ifdef_stack
#define is_slotc            TPP_INTERNAL(tids_cnt) /* Don't access */
#define is_slota            TPP_INTERNAL(tids_alc) /* Don't access */
#define is_slotv            TPP_INTERNAL(tids_vec) /* Don't access */

#define TPP_EXTENSIONS_BITSETSIZE (TPP_EXT_COUNT ? (TPP_EXT_COUNT + 7) / 8 : 1)
#define TPPExtState               tpp_extensions
#define es_prev                   TPP_INTERNAL(te_prev)                           /* Don't access */
#define es_bitset                 TPP_INTERNAL(te_state).TPP_INTERNAL(tes_bitset) /* Don't access */

/* Check if a given extension `ext' is currently enabled.
 * @return: 0: The extension is disabled.
 * @return: !0: The extension is enabled. */
#define TPPLexer_HasExtension_(_current, ext) tpp_lexer_getextension(_current, expt)
#define TPPLexer_HasExtension(ext)            TPPLexer_HasExtension_(TPP2_LEXER, ext)

/* Set the state of a given extension `ext'. */
#define TPPLexer_EnableExtension_(_current, ext)  tpp_lexer_enableextension(_current, ext)
#define TPPLexer_DisableExtension_(_current, ext) tpp_lexer_disableextension(_current, ext)
#define TPPLexer_EnableExtension(ext)             TPPLexer_EnableExtension_(TPP2_LEXER, ext)
#define TPPLexer_DisableExtension(ext)            TPPLexer_DisableExtension_(TPP2_LEXER, ext)


/* CAUTION: This function works differently:
 * - TPPFILE_NEXTCHUNK_FLAG_EXTEND: Extension is now works such that the file may
 *                                  only discard bytes that have already been read,
 *                                  and are not covered by "tpp_file_setkeep"
 * - TPPFILE_NEXTCHUNK_FLAG_BINARY: Not available anymore (if you want to do binary
 *                                  read, you must directly use "tpp_io_read()")
 * - TPPFILE_NEXTCHUNK_FLAG_NOBLCK: Not available anymore: non-blocking is controlled on
 *                                  a per-file basis (s.a. "TPP_FILE_IOFLAGS_NONBLOCK")
 */
#define TPPFILE_NEXTCHUNK_FLAG_NONE   0 /* No special behavior modification. */
#undef TPPFILE_NEXTCHUNK_FLAG_EXTEND
#undef TPPFILE_NEXTCHUNK_FLAG_BINARY
#undef TPPFILE_NEXTCHUNK_FLAG_NOBLCK
TPP_INLINE int TPPCALL TPPFile_NextChunk_impl(tpp_file *tpp_restrict self) {
	tpp_errno error = tpp_file_expandchunk(self);
#if TPP_HAVE_FILE_NONBLOCK
	if (error == TPP_EWOULDBLOCK)
		return 0;
#endif /* TPP_HAVE_FILE_NONBLOCK */
	if (TPP_ISERR(error)) {
#ifdef TPP_CONFIG_SET_API_ERROR
		return -1; /* TPP_ENOMEM or TPP_EIO */
#else /* TPP_CONFIG_SET_API_ERROR */
		return 0; /* TPP_ENOMEM or TPP_EIO */
#endif /* !TPP_CONFIG_SET_API_ERROR */
	}
	return 1;
}


#define TPPIncludeList tpp_include_paths
#define il_prev  TPP_INTERNAL(tip_prev)                                /* Use tpp_lexer_canpopincludes() */
#define il_pathc TPP_INTERNAL(tip_system_list).TPP_INTERNAL(tipl_size) /* Use tpp_lexer_includes_numsystem() */
#define il_pathv TPP_INTERNAL(tip_system_list).TPP_INTERNAL(tipl_list) /* Use tpp_lexer_includes_getsystem() */

/* Push/Pop the current system #include-path list.
 * @return: 0: [TPPLexer_PushInclude] Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
 * @return: 0: [TPPLexer_PopInclude] No older #include-path list was available to restore.
 * @return: 1: Successfully pushed/popped the system #include-path list. */
#define TPPLexer_PushInclude_(self) (tpp_lexer_pushincludes(self), 1)
#define TPPLexer_PopInclude_(self)  (tpp_lexer_canpopincludes(self) ? (tpp_lexer_popincludes(self), 1) : 0)
#define TPPLexer_PushInclude()      TPPLexer_PushInclude_(TPP2_LEXER)
#define TPPLexer_PopInclude()       TPPLexer_PopInclude_(TPP2_LEXER)

/* Add/delete the given path from the list of system #include paths.
 * WARNING: This function will modify the given path.
 * WARNING: Be careful with absolute vs. relative paths!
 *          TPP can not tell that they're the same and
 *          `#pragma once' might break as a consequence!
 *       >> As a solution, _always_ use either absolute
 *          or relative paths for the same file/path.
 *          (This also goes for #include directives)
 * @return: 0: [TPPLexer_AddIncludePath] Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
 * @return: 1: [TPPLexer_AddIncludePath] The given path was successfully added.
 * @return: 2: [TPPLexer_AddIncludePath] The given path had already been added before.
 * @return: 0: [TPPLexer_DelIncludePath] The given path was not found.
 * @return: 1: [TPPLexer_DelIncludePath] The given path was successfully removed. */
#define TPPLexer_AddIncludePath_(self, path, pathsize) (tpp_lexer_includes_addsystem(self, path, pathsize) == TPP_ENOMEM ? 0 : 1)
#define TPPLexer_DelIncludePath_(self, path, pathsize) (tpp_lexer_includes_delsystem(self, path, pathsize) == TPP_ENOMEM ? 0 : 1)
#define TPPLexer_AddIncludePath(path, pathsize)        TPPLexer_AddIncludePath_(TPP2_LEXER, path, pathsize)
#define TPPLexer_DelIncludePath(path, pathsize)        TPPLexer_DelIncludePath_(TPP2_LEXER, path, pathsize)

#if 0 /* TODO */
//#ifndef TPP_CONFIG_NO_ASSERTIONS
//struct TPPAssertion {
//	struct TPPAssertion *as_next; /* [0..1][owned] Next assertion. */
//	struct TPPKeyword   *as_kwd;  /* [1..1][const] Keyword associated with this assertion. */
//};
//struct TPPAssertions {
//	/* s.a.: `https://gcc.gnu.org/onlinedocs/cpp/Obsolete-Features.html' */
//	size_t                as_assc; /* Amount of defined assertions. */
//	size_t                as_assa; /* Allocated amount of assertions. */
//	struct TPPAssertion **as_assv; /* [0..1][owned][0..as_alloc][owned] Hash-map of existing assertions. */
//};
//#endif /* !TPP_CONFIG_NO_ASSERTIONS */
#endif



#define TPP_KEYWORDFLAG_NONE TPP_KEYWORD_FLAG_NORMAL
#undef TPP_KEYWORDFLAG_BUILTINMACRO   /* Must be checked for using `tpp_macro_getbuiltin()' */
#undef TPP_KEYWORDFLAG_NO_UNDERSCORES /* Must be checked for using `tpp_macro_getbuiltin()' */
#define TPP_KEYWORDFLAG_IMPORTED               TPP_KEYWORD_FLAG_HDR_IMPORTED
#define TPP_KEYWORDFLAG_HAS_ATTRIBUTE          TPP_KEYWORD_FLAG_HAS_ATTRIBUTE
#define TPP_KEYWORDFLAG_HAS_BUILTIN            TPP_KEYWORD_FLAG_HAS_BUILTIN
#define TPP_KEYWORDFLAG_HAS_CPP_ATTRIBUTE      TPP_KEYWORD_FLAG_HAS_CPP_ATTRIBUTE
#define TPP_KEYWORDFLAG_HAS_DECLSPEC_ATTRIBUTE TPP_KEYWORD_FLAG_HAS_DECLSPEC_ATTRIBUTE
#define TPP_KEYWORDFLAG_HAS_EXTENSION          TPP_KEYWORD_FLAG_HAS_EXTENSION
#define TPP_KEYWORDFLAG_HAS_FEATURE            TPP_KEYWORD_FLAG_HAS_FEATURE
#define TPP_KEYWORDFLAG_IS_DEPRECATED          TPP_KEYWORD_FLAG_IS_DEPRECATED
#define TPP_KEYWORDFLAG_IS_POISONED            TPP_KEYWORD_FLAG_IS_POISONED
#undef TPP_KEYWORDFLAG_HAS_TPP_BUILTIN /* TPP builtins (and consequently "__has_tpp_builtin") are no longer supported */
#define TPP_KEYWORDFLAG_USERMASK TPP_KEYWORD_FLAG_USERMASK

#define TPPRareKeyword tpp_keyword_misc
#undef kr_file     /* Replaced with "tkm_file_guard" (which has a slightly different meaning) */
#undef kr_oldmacro /* Replaced with "tkm_macro_pushstack" (which has a slightly different meaning) */
#undef kr_defmacro /* Builtin macros are now handled by `tpp_macro_getbuiltin()' */
#define kr_flags   tkm_flags /* Use tpp_lexer_getkeywordflags() */
#define kr_counter tkm_builtin_counter /* Don't access */
#if 0 /* TODO */
//#ifndef TPP_CONFIG_NO_ASSERTIONS
//	struct TPPAssertions      kr_asserts;  /* Assertions (aka. #assert/#unassert associated with this keyword) */
//#endif /* !TPP_CONFIG_NO_ASSERTIONS */
#endif
#define kr_user tkm_userdata_ptr /* Use tpp_keyword_getuserdata() / tpp_keyword_setuserdata() / tpp_keyword_misc_getuserdata() / tpp_keyword_misc_setuserdata() */


#define TPPKeyword tpp_keyword
#define k_next     TPP_INTERNAL(tk_next)  /* Don't access */
#define k_macro    TPP_INTERNAL(tk_macro) /* Use tpp_keyword_getmacro() */
#define k_rare     TPP_INTERNAL(tk_misc)  /* Use tpp_keyword_getmisc() / tpp_keyword_requiremisc() */
#define k_id       TPP_INTERNAL(tk_id)    /* Use tpp_keyword_getid() */
#define k_size     TPP_INTERNAL(tk_len)   /* Use tpp_keyword_getkwdlen() */
#define k_hash     TPP_INTERNAL(tk_hash)  /* Use tpp_keyword_getkwdhash() */
#define k_name     TPP_INTERNAL(tk_kwd)   /* Use tpp_keyword_getkwdcstr() */

#define TPPKeyword_ISDEFINED(self) ((self)->k_macro != NULL || TPP_ISBUILTINMACRO((self)->k_id))
#define TPPKeyword_MAKERARE(self)  (tpp_keyword_requiremisc(self) != NULL)
#define TPPKeyword_EQUALS(self, constant_string) \
	tpp_keyword_equals_cstr(self, constant_string)

/* Returns the effective keyword flags of `self'.
 * @return: A set of `TPP_KEYWORDFLAG_*' */
#define TPPKeyword_GetFlags(self, check_without_underscores) \
	TPPKeyword_GetFlags_(TPP_LEXER_ARG_ self, check_without_underscores)
TPP_INLINE tpp_keyword_flags TPPCALL
TPPKeyword_GetFlags_(tpp_lexer *lexer,
                     tpp_keyword const *tpp_restrict self,
                     int check_without_underscores) {
	tpp_keyword_flags result = tpp_lexer_getkeywordflags(lexer, self);
	if (check_without_underscores) {
		tpp_char const *without_underscore_start = tpp_keyword_getkwd(self);
		tpp_size without_underscore_len          = tpp_keyword_getkwdlen(self);
		while (without_underscore_len && *without_underscore_start == '_')
			++without_underscore_start, --without_underscore_len;
		while (without_underscore_len && without_underscore_start[without_underscore_len - 1] == '_')
			--without_underscore_len;
		if (without_underscore_len < tpp_keyword_getkwdlen(self)) {
			tpp_hash hash = tpp_hashof(without_underscore_start, without_underscore_len);
			tpp_keyword const *without_underscore = tpp_lexer_kwds_getkeyword(lexer, without_underscore_start,
			                                                                  without_underscore_len, hash);
			if (without_underscore)
				result |= tpp_lexer_getkeywordflags(lexer, without_underscore);
		}
	}
	return result;
}


#define TPPKeywordMap tpp_keywords
#define km_entryc     TPP_INTERNAL(tks_kwdc)
#define km_bucketc    TPP_INTERNAL(tks_bckm)+1 /* Not really the same (km_bucketc == tks_bckm+1), but close enough... */
#define km_bucketv    TPP_INTERNAL(tks_bckv)
#define TPPKeywordMap_SHOULDHASH(self) \
	((self)->km_entryc >= (self)->km_bucketc * 2)


#define TPPToken tpp_token
#define t_id TPP_INTERNAL(tt_id)
#undef t_num  /* Not supported by TPP3 */
#undef t_file /* In TPP3, the lexer always re-uses the same file structure as the current file.
               * (when a new file is #include-ed, the old one is saved in the #include-stack)
               * To access the current file, use `tpp_lexer_getfile()' */
#define t_begin TPP_INTERNAL(tt_start)
#define t_end   TPP_INTERNAL(tt_end)
#define t_kwd   TPP_INTERNAL(tt_kwd)

/* Returns the top-level source locations (in-macro & everything)
 * NOTE: These are _not_ what you're probably looking for.
 *       You probably expect these to act like __FILE__ and __LINE__,
 *       but instead they will show the true source locations where the
 *       current token originates from (e.g.: from a macro definition.)
 *    >> To get information about the source file you must walk the
 *       current token's t_file->f_prev->... chain until you reach
 *       a text file. Then, taking that file, use it and its f_pos
 *       pointers to figure out what you actually want to know. */
#define TPPLexer_TRUE_FILE(plength) TPPFile_Filename(tpp_lexer_getfile(TPP2_LEXER), plength)
#define TPPLexer_TRUE_LC(info)      TPPFile_LCAt(tpp_lexer_getfile(TPP2_LEXER), info, tpp_lexer_gettokenstart(TPP2_LEXER))
#define TPPLexer_TRUE_LINE()        TPPFile_LineAt(tpp_lexer_getfile(TPP2_LEXER), tpp_lexer_gettokenstart(TPP2_LEXER))
#define TPPLexer_TRUE_COLUMN()      TPPFile_ColumnAt(tpp_lexer_getfile(TPP2_LEXER), tpp_lexer_gettokenstart(TPP2_LEXER))


/* Returns the top-most text file associated with the current lexer.
 * NOTE: These functions never returns NULL. */
#define TPPLexer_Textfile_(lexer) tpp_file_getlcfile(tpp_lexer_getfile(lexer))
#define TPPLexer_Basefile_(lexer) tpp_file_getbasefile(tpp_lexer_getfile(lexer))
#define TPPLexer_Textfile()       TPPLexer_Textfile_(TPP_LEXER_ARG)
#define TPPLexer_Basefile()       TPPLexer_Basefile_(TPP_LEXER_ARG)

#define TPPLexer_FILE(plength)     TPPFile_Filename(TPPLexer_Textfile(), plength)
#define TPPLexer_BASEFILE(plength) TPPFile_Filename(TPPLexer_Basefile(), plength)
#define TPPLexer_LC(info)          TPPLexer_LC_(TPP_LEXER_ARG_ info)
#define TPPLexer_LINE()            TPPLexer_LINE_(TPP_LEXER_ARG)
#define TPPLexer_COLUMN()          TPPLexer_COLUMN_(TPP_LEXER_ARG)

TPP_INLINE void TPPCALL TPPLexer_LC_(tpp_lexer *self, tpp_lcinfo *tpp_restrict info) {
	struct TPPFile *f = TPPLexer_Textfile_(self);
	TPPFile_LCAt(f, info, f->f_pos);
}
TPP_INLINE tpp_line TPPCALL TPPLexer_LINE_(tpp_lexer *self) {
	struct TPPFile *f = TPPLexer_Textfile_(self);
	return TPPFile_LineAt(f, f->f_pos);
}
TPP_INLINE tpp_column TPPCALL TPPLexer_COLUMN_(tpp_lexer *self) {
	struct TPPFile *f = TPPLexer_Textfile_(self);
	return TPPFile_ColumnAt(f, f->f_pos);
}



/* Lexer state flags. */
#define TPPLEXER_FLAG_NONE    TPP_LEXER_STATE_FLAG_NORMAL
#define TPPLEXER_FLAG_DEFAULT TPP_LEXER_STATE_FLAG_NORMAL
#undef TPPLEXER_FLAG_WANTCOMMENTS          /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_COMMENT) */
#undef TPPLEXER_FLAG_WANTSPACE             /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_SPACE) */
#undef TPPLEXER_FLAG_WANTLF                /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_LF) */
#undef TPPLEXER_FLAG_NO_SEEK_ON_EOB        /* Use tpp_file_pusheof() or tpp_file_subtext_push() */
#undef TPPLEXER_FLAG_NO_POP_ON_EOF         /* Use tpp_file_autopopfile_pushoff() */
#undef TPPLEXER_FLAG_KEEP_MACRO_WHITESPACE /* Use "TPP_EXT_MACRO_ARGUMENT_WHITESPACE" (global) or "TPP_MACRO_FLAG_KEEPARGSPC" (individual macro) */
#undef TPPLEXER_FLAG_NONBLOCKING           /* Use "TPP_FILE_IOFLAGS_NONBLOCK" (per-file) */
#undef TPPLEXER_FLAG_TERMINATE_STRING_LF   /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_STRING_ALLOW_MULTILINE) (inverted meaning) */
#undef TPPLEXER_FLAG_NO_DIRECTIVES         /* Use tpp_lexer_setfeat(TPP_FEAT_CPP_DIRECTIVES) */
#undef TPPLEXER_FLAG_NO_MACROS             /* Use tpp_lexer_setfeat(TPP_FEAT_CPP_MACROS) */
#undef TPPLEXER_FLAG_NO_BUILTIN_MACROS     /* Use tpp_lexer_setfeat(TPP_FEAT_CPP_BUILTIN_MACROS) */
#undef TPPLEXER_FLAG_ASM_COMMENTS          /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_SHELL_COMMENT) */
#undef TPPLEXER_FLAG_DIRECTIVE_NOOWN_LF    /* No longer supported; in TPP3, preprocessor directives *always* include the trailing line-feed */
#undef TPPLEXER_FLAG_COMMENT_NOOWN_LF      /* No longer supported; in TPP3, line-comments *always* include the trailing line-feed */
#if 0 /* TODO */
//#define TPPLEXER_FLAG_MESSAGE_LOCATION       0x00004000 /* Print the file+line location in messages from `#pragma message'. */
//#define TPPLEXER_FLAG_MESSAGE_NOLINEFEED     0x00008000 /* Don't print a linefeed following the user-provided message in `#pragma message'. */
#endif
#undef TPPLEXER_FLAG_INCLUDESTRING         /* No longer supported; in TPP3, #include-strings parsing bypasses regular tokenization done by "tpp_lexer_yieldraw()" */
#undef TPPLEXER_FLAG_EXTENDFILE            /* Use tpp_file_pushkeep() + tpp_file_setkeep() to keep certain file data loaded into memory */
#undef TPPLEXER_FLAG_NO_LEGACY_GUARDS      /* No longer supported; in TPP3, this can only be compile-time configured via "TPP_HAVE_IFNDEF_INCLUDE_GUARDS" */
#if 0 /* TODO */
//#define TPPLEXER_FLAG_WERROR                 0x00080000 /* All warnings are turned into errors (NOTE: less powerful than `TPPLEXER_FLAG_WSYSTEMHEADERS'). */
//#define TPPLEXER_FLAG_WSYSTEMHEADERS         0x00100000 /* Still emit warnings in system headers (alongside errors). */
//#define TPPLEXER_FLAG_NO_DEPRECATED          0x00200000 /* Don't warn about deprecated or poisoned keywords. */
//#define TPPLEXER_FLAG_MSVC_MESSAGEFORMAT     0x00400000 /* Use msvc's file+line format `%s(%d,%d) : ' instead of GCC's `%s:%d:%d: '. */
#endif
#define TPPLEXER_FLAG_NO_WARNINGS TPP_LEXER_STATE_FLAG_NOWARNINGS
#undef TPPLEXER_FLAG_NO_ENCODING           /* No longer supported; encoding detection can be disabled on a per-file basis, though */
#undef TPPLEXER_FLAG_REEMIT_UNKNOWN_PRAGMA /* No longer supported; define your own hook that gets called for unknown pragmas */
#undef TPPLEXER_FLAG_CHAR_UNSIGNED         /* No longer relevant since APIs were split */
#undef TPPLEXER_FLAG_EOF_ON_PAREN          /* No longer supported; paren-tracking is now done by "tpp_lexer_seekpp_rparen()" + "tpp_file_subtext_setchunk_fromarg()" */
#undef TPPLEXER_FLAG_WILLRESTORE           /* No longer supported; TPP3 has no global restore-lexer-position API */
#undef TPPLEXER_FLAG_RANDOM_INITIALIZED    /* No longer needed; TPP3 implements __TPP_RANDOM() differently */
#undef TPPLEXER_FLAG_ERROR                 /* No longer relevant: TPP3 handles errors explicitly via return values */
#undef TPPLEXER_FLAG_MERGEMASK             /* No longer relevant: TPP3's state flags are no longer used like that */
#undef TPPLEXER_FLAG_MERGEMASK             /* No longer relevant: TPP3's state flags are no longer used like that */
#undef TPPLEXER_FLAG_PRAGMA_KEEPMASK       /* No longer relevant: TPP3's state flags are no longer used like that */


/* Recognized extension token flags. */
#undef TPPLEXER_TOKEN_NONE         /* No longer exists in TPP3 (though for default tokens, though you could do `tpp_lexer_resetfeatures()' to reset token emissions to their default) */
#undef TPPLEXER_TOKEN_DEFAULT      /* No longer exists in TPP3 (though for default tokens, though you could do `tpp_lexer_resetfeatures()' to reset token emissions to their default) */
#undef TPPLEXER_TOKEN_TILDETILDE   /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_TILDE_TILDE) */
#undef TPPLEXER_TOKEN_ROOFROOF     /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_HAT_HAT) */
#undef TPPLEXER_TOKEN_COLONCOLON   /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_COLON_COLON) */
#undef TPPLEXER_TOKEN_COLLONCOLLON /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_COLON_COLON) */
#undef TPPLEXER_TOKEN_COLONASSIGN  /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_COLON_EQUAL) */
#undef TPPLEXER_TOKEN_COLLONASSIGN /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_COLON_EQUAL) */
#undef TPPLEXER_TOKEN_STARSTAR     /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL) + tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_STAR_STAR) */
#undef TPPLEXER_TOKEN_ARROW        /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_MINUS_RANGLE) */
#undef TPPLEXER_TOKEN_ARROWSTAR    /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR) */
#undef TPPLEXER_TOKEN_DOTSTAR      /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_DOT_STAR) */
#undef TPPLEXER_TOKEN_DOTDOT       /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_DOT_DOT) */
#undef TPPLEXER_TOKEN_ATEQUAL      /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_AT_EQUAL) */
#undef TPPLEXER_TOKEN_C_COMMENT    /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_C_COMMENT) */
#undef TPPLEXER_TOKEN_CPP_COMMENT  /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_CXX_COMMENT) */
#undef TPPLEXER_TOKEN_ANGLE3       /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE) + tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE) */
#undef TPPLEXER_TOKEN_ANGLE3_EQUAL /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL) + tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL) */
#undef TPPLEXER_TOKEN_LOGT         /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_LANGLE_RANGLE) */
#undef TPPLEXER_TOKEN_EQUALBINOP   /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_PLUS), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_MINUS), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_STAR), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_STAR_STAR), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_SLASH), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_PERCENT), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_AMP), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_PIPE), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_HAT), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE), tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_AT) */
#undef TPPLEXER_TOKEN_EQUAL3       /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL) + tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL) */
#undef TPPLEXER_TOKEN_QMARK_QMARK  /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK) */
#undef TPPLEXER_TOKEN_DOLLAR       /* Use tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_DOLLAR) */
#undef TPPLEXER_TOKEN_LANG_C
#undef TPPLEXER_TOKEN_LANG_ASM
#undef TPPLEXER_TOKEN_LANG_CPP
#undef TPPLEXER_TOKEN_LANG_JAVA
#undef TPPLEXER_TOKEN_LANG_DEEMON


#define TPPLexer tpp_lexer
#define l_token      TPP_INTERNAL(tl_core).TPP_INTERNAL(tlc_tok) /* Use tpp_lexer_gettoken() */
#undef l_eob_file    /* No longer exists because "TPPLEXER_FLAG_NO_SEEK_ON_EOB" no longer exists */
#undef l_eof_file    /* No longer exists; use "tpp_file_autopopfile_pushoff()" */
#define l_flags      TPP_INTERNAL(tl_state) /* Use "tpp_lexer_*_pushon()" instead */
#undef l_extokens    /* No longer exists: recognized tokens are controlled via "tpp_lexer_setfeat()" */
#define l_extensions TPP_INTERNAL(tl_exts) /* Use tpp_lexer_getextension() / tpp_lexer_pushextensions() / ... */
#define l_keywords   TPP_INTERNAL(tl_kwds) /* Use tpp_lexer_kwds_*() */
#define l_syspaths   TPP_INTERNAL(tl_include_paths) /* Use tpp_lexer_includes_*() */
#if 0 /* TODO */
//	size_t                l_limit_mrec; /* Limit for how often a macro may recursively expand into itself. */
//	size_t                l_limit_incl; /* Limit for how often the same text file may exist on the #include stack. */
#endif
#undef l_eof_paren   /* No longer supported; see `TPPLEXER_FLAG_EOF_ON_PAREN' */
#if 0 /* TODO */
//	size_t                l_warncount;  /* Amount of warnings that were invoked (including those that were dismissed). */
//#ifdef TPP_CONFIG_DYN_CALLBACKS
//	struct TPPCallbacks   l_callbacks;  /* User-defined lexer callbacks. */
//#endif /* TPP_CONFIG_DYN_CALLBACKS */
#endif
#define l_errorcount TPP_INTERNAL(tl_error_count)
#define l_maxerrors  TPP_INTERNAL(tl_error_limit)
#undef l_tabsize     /* This is now one of the very few *global* configs in TPP3: use tpp_gettabsize() / tpp_settabsize() */
#define l_ifdef      TPP_INTERNAL(tl_core).TPP_INTERNAL(tlc_input).TPP_INTERNAL(tli_file).TPP_INTERNAL(tf_ifdef) /* WARNING: ifdef-stacks are configured on a per-file basis in TPP3! */
#define l_warnings   TPP_INTERNAL(tl_warn)
#undef l_noerror     /* No longer relevant since errors are now handled via return values */
#define l_counter    TPP_INTERNAL(tl_builtin_counter)

#define TPPLexer_Init(self) \
	(tpp_lexer_init(self),  \
	 tpp_lexer_initfile_text_ascii(self, NULL, NULL, NULL, 0, TPP_LCINFO_INVALID), 1)
#define TPPLexer_Quit(self) \
	(tpp_lexer_finifile(self), tpp_lexer_fini(self))

/* Clear the current ifdef-stack and warn about each entry.
 * @return: 1: Everything was ok, or no critical warning happened.
 * @return: 0: A critical warning happened. */
#define TPPLexer_ClearIfdefStack() TPPLexer_ClearIfdefStack_(TPP2_LEXER)
TPP_INLINE int TPPCALL TPPLexer_ClearIfdefStack_(tpp_lexer *self) {
	tpp_ifdef_stack *ifdef_stack;
	tpp_errno error = tpp_lexer_warn_nonempty_ifdef(self);
	if (TPP_ISERR(error))
		return 0;
	ifdef_stack = tpp_file_getifdef(tpp_lexer_getfile(self));
	tpp_ifdef_stack_clear(ifdef_stack);
	return 1;
}

#if 0 /* TODO */
//#define TPPLEXER_RESET_NONE       0x00000000
//#define TPPLEXER_RESET_INCLUDE    0x00000001 /* Reset the #include/#ifdef-stack and set the current token to EOF.
//                                              * NOTE: Also resets the `l_eob_file' and `l_eof_file' special
//                                              *       file pointers, as well as setting `l_noerror' to EOF
//                                              *       and `l_warncount' and `l_errorcount' to ZERO(0). */
//#define TPPLEXER_RESET_ESTATE     0x00000002 /* Reset the current extensions state to mirror the default. */
//#define TPPLEXER_RESET_ESTACK     0x00000004 /* Clear all previously pushed extension states. */
//#define TPPLEXER_RESET_WSTATE     0x00000008 /* Reset the current warning state to mirror the default. */
//#define TPPLEXER_RESET_WSTACK     0x00000010 /* Clear all previously pushed warning states. */
//#define TPPLEXER_RESET_SYSPATHS   0x00000020 /* Clears all system #include-paths. */
//#define TPPLEXER_RESET_MACRO      0x00000040 /* Reset user-defined macros.
//                                              * The original definitions of runtime builtin macros are restored,
//                                              * unless the `TPPLEXER_RESET_NORESTOREMACROS' flag is set. */
//#define TPPLEXER_RESET_ASSERT     0x00000080 /* Reset user-defined assertions. */
//#define TPPLEXER_RESET_KWDFLAGS   0x00000100 /* Reset user-defined keyword flags. */
//#define TPPLEXER_RESET_COUNTER    0x00000200 /* Reset __COUNTER__ and __TPP_COUNTER for all keywords. */
//#define TPPLEXER_RESET_FONCE      0x00000400 /* Clear all `#pragma once' descriptors. */
//#define TPPLEXER_RESET_KEYWORDS   0x00000800 /* Clear all keywords, but keep all predefined.
//                                              * NOTE: When set, this flag implies `TPPLEXER_RESET_MACRO',
//                                              *       `TPPLEXER_RESET_ASSERT', `TPPLEXER_RESET_KWDFLAGS',
//                                              *       `TPPLEXER_RESET_COUNTER' and `TPPLEXER_RESET_FONCE'.
//                                              * NOTE: It also implies `TPPLEXER_RESET_NORESTOREMACROS' */
//#define TPPLEXER_RESET_NORESTOREMACROS 0x00001000 /* When used with `TPPLEXER_RESET_MACRO': Don't restore builtin macro definitions. */
//#define TPPLEXER_RESET_EXTENSIONS (TPPLEXER_RESET_ESTATE|TPPLEXER_RESET_ESTACK)
//#define TPPLEXER_RESET_WARNINGS   (TPPLEXER_RESET_WSTATE|TPPLEXER_RESET_WSTACK)

///* Reset certain parts of the lexer.
// * NOTE: This function can be called when `TPPLexer_Current' is NULL, or not initialized.
// * @param: flags: Set of `TPPLEXER_RESET_*' */
//TPPFUN void TPPCALL TPPLexer_Reset(struct TPPLexer *tpp_restrict self, uint32_t flags);
#endif

/* Push/Pop the current extension state.
 * @return: 0: [TPPLexer_PushExtensions] Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
 * @return: 0: [TPPLexer_PopExtensions] No older extension state was available to restore.
 * @return: 1: Successfully pushed/popped active extensions. */
#define TPPLexer_PushExtensions_(self) (tpp_lexer_pushextensions(self), 1)
#define TPPLexer_PushExtensions() TPPLexer_PushExtensions_(TPP2_LEXER)
#define TPPLexer_PopExtensions()  TPPLexer_PopExtensions_(TPP2_LEXER)
TPP_INLINE int TPPCALL TPPLexer_PopExtensions_(tpp_lexer *self) {
	if (!tpp_lexer_canpopextensions(self))
		return 0;
	tpp_lexer_popextensions(self);
	return 1;
}

/* Set the state of a given extension `name'.
 * Extension names attempt to follow gcc names of the same extension.
 * The name of an extension can be found above.
 * @return: 0: Unknown extension.
 * @return: 1: Successfully configured the given extension. */
#define TPPLexer_SetExtension(name, enable) TPPLexer_SetExtension_(TPP2_LEXER, name, enable)
TPP_INLINE int TPPCALL
TPPLexer_SetExtension_(tpp_lexer *self, char const *tpp_restrict name, int enable) {
	tpp_errno error;
	tpp_extension_id id = tpp_extension_byname(name);
	if ((unsigned int)id >= (unsigned int)TPP_EXT_COUNT)
		return 0;
	error = tpp_lexer_setextension(self, id, enable);
	if (TPP_ISERR(error))
		return TPP2_FATAL(-1); /* TPP2 could not trigger OOM here, but we can... */
	return 1;
}

/* Returns the state of a given extension.
 * @return: -1: Unknown extension.
 * @return:  0: Disabled extension.
 * @return:  1: Enabled extension. */
#define TPPLexer_GetExtension(name) TPPLexer_GetExtension_(TPP2_LEXER, name)
TPP_INLINE int TPPCALL
TPPLexer_GetExtension_(tpp_lexer *self, char const *tpp_restrict name) {
	tpp_extension_id id = tpp_extension_byname(name);
	if ((unsigned int)id >= (unsigned int)TPP_EXT_COUNT)
		return -1;
	return tpp_lexer_getextension(self, id) ? 1 : 0;
}


#if 0 /* TODO */
///* Searches the cache and opens a new file if not found.
// * WARNING: If the caller intends to push the file onto the #include-stack,
// *          additional steps must be taken when the file was already
// *          located on the stack (in which case another stream must be opened,
// *          and a file that is not cached must be pushed onto the #include-stack).
// * WARNING: This function may modify the given `filename..filename_size+1' area of memory.
// * @param: pkeyword_entry: When non-NULL, the keyword entry associated with the filename is stored here.
// * @return: * :   A pointer to the already-chached file (WARNING: This is not a reference)
// * @return: NULL: File not found. */
//TPPFUN struct TPPFile *TPPCALL
//TPPLexer_OpenFile_(TPP_LEXER_PARAM_
//                   int mode, char *tpp_restrict filename, size_t filename_size,
//                   struct TPPKeyword **pkeyword_entry);
//#define TPPLexer_OpenFile(mode, filename, filename_size, pkeyword_entry) \
//	TPPLexer_OpenFile_(TPP_LEXER_ARG_ mode, filename, filename_size, pkeyword_entry)
//#define TPPLEXER_OPENFILE_MODE_NORMAL     0x00 /* Normal open (simply pass the given filename to TPPFile_Open, but still sanitize and cache the filename) */
//#define TPPLEXER_OPENFILE_MODE_RELATIVE   0x01 /* #include "foo.h" (Search for the file relative to the path of every text file on the #include-stack in reverse. - If this fails, search in system folders). */
//#define TPPLEXER_OPENFILE_MODE_SYSTEM     0x02 /* #include <stdlib.h> (Search through system folders usually specified with `-I' on the commandline). */
//#define TPPLEXER_OPENFILE_FLAG_NEXT       0x04 /* FLAG: Only open a file not already part of the #include-stack
//                                                * WARNING: May not be used for `TPPLEXER_OPENFILE_MODE_NORMAL'! */
//#define TPPLEXER_OPENFILE_FLAG_NOCASEWARN 0x08 /* FLAG: Don't warn about filename casing on windows. */
//#ifndef TPP_CONFIG_NO_CALLBACK_UNKNOWN_FILE
//#define TPPLEXER_OPENFILE_FLAG_NOCALLBACK 0x10 /* FLAG: Don't invoke the unknown-file callback when set. */
//#endif /* !TPP_CONFIG_NO_CALLBACK_UNKNOWN_FILE */
//#define TPPLEXER_OPENFILE_FLAG_CONSTNAME  0x20 /* FLAG: The given `filename' may not be modified, but is guarantied to be '\0'-terminated. */
//
///* Push a given file into the #include-stack of the current lexer.
// * NOTE: These functions never fail and return void.
// * HINT: Call `TPPLexer_PushFileInherited' if you want the lexer to inherit the file.
// * WARNING: The file argument may be evaluated more than once! */
//#define TPPLexer_PushFileInherited(f) TPPLexer_PushFileInherited_(TPPLexer_Current, f)
//#define TPPLexer_PushFile(f)          TPPLexer_PushFile_(TPPLexer_Current, f)
//#define TPPLexer_PushFileInherited_(_current, f)   \
//	(void)((f)->f_prev = _current->l_token.t_file, \
//	       _current->l_token.t_file = (f))
//#define TPPLexer_PushFile_(_current, f) \
//	(TPPFile_Incref(f), TPPLexer_PushFileInherited_(_current, f))
#endif

/* Returns the currently active #include-file.
 * WARNING: The file returned here will *always* be the "current" file!
 *          This is because in TPP3, the file currently being read from
 *          is inlined into the lexer, and pushing/popping files saved/
 *          restores/overrides that same file. */
#define TPPLexer_GetFile_(self) tpp_lexer_getfile(self)
#define TPPLexer_GetFile()      tpp_lexer_getfile(TPP2_LEXER)

/* Pop the current file off of the #include-stack. */
#define TPPLexer_PopFile_(self) tpp_lexer_popfile(self)
#define TPPLexer_PopFile()      TPPLexer_PopFile_(TPP2_LEXER)

#define TPPLexer_LookupKeyword_(self, name, namelen, create_missing)                              \
	((create_missing) ? tpp_lexer_kwds_newkeyword(self, name, namelen, tpp_hashof(name, namelen)) \
	                  : tpp_lexer_kwds_getkeyword(self, name, namelen, tpp_hashof(name, namelen)))
#define TPPLexer_LookupEscapedKeyword_(self, name, namelen, create_missing)                           \
	((create_missing) ? tpp_lexer_kwds_newkeyword_bse(self, name, namelen, tpp_hashof(name, namelen)) \
	                  : tpp_lexer_kwds_getkeyword_bse(self, name, namelen, tpp_hashof(name, namelen)))
#define TPPLexer_LookupKeyword(name, namelen, create_missing) \
	TPPLexer_LookupKeyword_(TPP2_LEXER, name, namelen, create_missing)
#define TPPLexer_LookupEscapedKeyword(name, namelen, create_missing) \
	TPPLexer_LookupEscapedKeyword_(TPP2_LEXER, name, namelen, create_missing)

/* Looks up a keyword, given its ID */
#define TPPLexer_LookupKeywordID_(self, id) tpp_lexer_kwds_getkeyword_byid(self, id)
#define TPPLexer_LookupKeywordID(id)        tpp_lexer_kwds_getkeyword_byid(TPP2_LEXER, id)

#if 0 /* TODO */
///* Define a regular, keyword-style macro `name' as `value'.
// * @param: flags: A set of `TPPLEXER_DEFINE_FLAG_*'
// * @return: 0: Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
// * @return: 1: Successfully defined the given macro.
// * @return: 2: A macro named `name' was already defined, and was overwritten. */
//TPPFUN int TPPCALL
//TPPLexer_Define_(TPP_LEXER_PARAM_
//                 char const *tpp_restrict name, size_t name_size,
//                 char const *tpp_restrict value, size_t value_size,
//                 uint32_t flags);
//#define TPPLexer_Define(name, name_size, value, value_size, flags) \
//	TPPLexer_Define_(TPP_LEXER_ARG_ name, name_size, value, value_size, flags)
//#define TPPLEXER_DEFINE_FLAG_NONE    0x00000000
//#define TPPLEXER_DEFINE_FLAG_BUILTIN TPP_KEYWORDFLAG_BUILTINMACRO /* Define the macro as builtin, meaning the definition
//                                                                   * set by `value' will be restored when `TPPLexer_Reset()'
//                                                                   * is called with `TPPLEXER_RESET_MACRO'. */
//
///* Undefine the macro associated with a given name.
// * @return: 0: No macro was associated with the given name.
// * @return: 1: Successfully undefined a macro. */
//TPPFUN int TPPCALL TPPLexer_Undef_(TPP_LEXER_PARAM_ char const *tpp_restrict name, size_t name_size);
//#define TPPLexer_Undef(name, name_size) TPPLexer_Undef_(TPP_LEXER_ARG_ name, name_size)
//
//#ifndef TPP_CONFIG_NO_ASSERTIONS
///* Add/Delete a given assertion for a given predicate.
// * @param: answer: [TPPLexer_DelAssert] When NULL, clear all assertions.
// * @return: 0: [TPPLexer_AddAssert] Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
// * @return: 0: [TPPLexer_DelAssert] Unknown/no answer(s)
// * @return: 1: Successfully added/deleted any assertion(s) */
//TPPFUN int TPPCALL
//TPPLexer_AddAssert_(TPP_LEXER_PARAM_
//                    char const *tpp_restrict predicate, size_t predicate_size,
//                    char const *tpp_restrict answer, size_t answer_size);
//TPPFUN int TPPCALL
//TPPLexer_DelAssert_(TPP_LEXER_PARAM_
//                    char const *tpp_restrict predicate, size_t predicate_size,
//                    char const *answer, size_t answer_size);
//#define TPPLexer_AddAssert(predicate, predicate_size, answer, answer_size) \
//	TPPLexer_AddAssert_(TPP_LEXER_ARG_ predicate, predicate_size, answer, answer_size)
//#define TPPLexer_DelAssert(predicate, predicate_size, answer, answer_size) \
//	TPPLexer_DelAssert_(TPP_LEXER_ARG_ predicate, predicate_size, answer, answer_size)
//#endif /* !TPP_CONFIG_NO_ASSERTIONS */
#endif

/* Similar to `TPPLexer_Yield' and used to implement it, but
 * doesn't expand macros or execute preprocessor directives. */
#define TPPLexer_YieldRaw_(self) tpp_lexer_yieldraw_blocking(self)
#define TPPLexer_YieldRaw()      tpp_lexer_yieldraw_blocking(TPP2_LEXER)

/* Similar to `TPPLexer_Yield' and used to
 * implement it, but doesn't expand macros. */
#define TPPLexer_YieldPP_(self) tpp_lexer_yieldpp_blocking(self)
#define TPPLexer_YieldPP()      tpp_lexer_yieldpp_blocking(TPP2_LEXER)

/* Advance the selected lexer by one token and return the id of the new one.
 * HINT: Returns ZERO(0) if EOF was reached. */
#define TPPLexer_Yield_(self) tpp_lexer_yield_blocking(self)
#define TPPLexer_Yield()      tpp_lexer_yield_blocking(TPP2_LEXER)

#if TPP_HAVE_FILE_NONBLOCK
/* In TPP3, non-blocking is enabled by default (if enabled for the current file),
 * but can be "disabled" by simply handling "TPP_EWOULDBLOCK" when that error
 * happens. */
#define TPPLexer_YieldRawNB_(self) tpp_lexer_yieldraw(self)
#define TPPLexer_YieldRawNB()      tpp_lexer_yieldraw(TPP2_LEXER)
#define TPPLexer_YieldPPNB_(self)  tpp_lexer_yieldpp(self)
#define TPPLexer_YieldPPNB()       tpp_lexer_yieldpp(TPP2_LEXER)
#define TPPLexer_YieldNB_(self)    tpp_lexer_yield(self)
#define TPPLexer_YieldNB()         tpp_lexer_yield(TPP2_LEXER)
#endif /* TPP_HAVE_FILE_NONBLOCK */


/* Return non-ZERO if the current token is the first of the current input line.
 * Return ZERO otherwise. */
#define TPPLexer_AtStartOfLine_(self) (!((self)->TPP_INTERNAL(tl_state) & TPP_LEXER_STATE_FLAG_NODIRECTIVES))
#define TPPLexer_AtStartOfLine()      TPPLexer_AtStartOfLine_(TPP2_LEXER)

/* Emit a given warning.
 * @return: 0: The warning was critical (TPPLexer_SetErr() was called and you should try to abort)
 * @return: 1: The warning was ignored, suppressed or simply non-fatal. */
#define TPPLexer_Warn_(self, ...) (!TPP_ISERR(tpp_lexer_warnf(self, __VA_ARGS__)))
#define TPPLexer_Warn(...)        (!TPP_ISERR(tpp_lexer_warnf(TPP2_LEXER, __VA_ARGS__)))

/* None of these are available since TPP3 passes errors through return values */
#undef TPPLexer_SetErr
#undef TPPLexer_SetErr_
#undef TPPLexer_UnsetErr
#undef TPPLexer_UnsetErr_
#undef TPPLexer_SetErr_inline
#undef TPPLexer_SetErr_inline_
#undef TPPLexer_UnsetErr_inline
#undef TPPLexer_UnsetErr_inline_


#define TPPConst tpp_expr_value
#define TPP_CONST_INTEGRAL _TPP_EXPR_VALUE_KIND_INT    /* Use tpp_expr_value_isint() */
#define TPP_CONST_FLOAT    _TPP_EXPR_VALUE_KIND_FLOAT  /* Use tpp_expr_value_isfloat() */
#define TPP_CONST_STRING   _TPP_EXPR_VALUE_KIND_STRING /* Use tpp_expr_value_isstring() */
#define c_kind             TPP_INTERNAL(xv_kind)       /* Use tpp_expr_value_is*() */
#define c_data             TPP_INTERNAL(xv_data)       /* Use tpp_expr_value_as*() */
#define c_int              TPP_INTERNAL(xd_int)        /* Use tpp_expr_value_asint() */
#define c_float            TPP_INTERNAL(xd_float)      /* Use tpp_expr_value_asfloat() */
#define c_string           TPP_INTERNAL(xd_string)     /* Use tpp_expr_value_asstringref() */

#define TPPConst_IsTrue(self) TPPConst_IsTrue_(TPP2_LEXER, self)
TPP_INLINE bool TPPConst_IsTrue_(tpp_lexer *lexer, tpp_expr_value *self) {
	bool result;
	if (tpp_expr_value_asbool(lexer, self, &result))
		return TPP2_FATAL(false);
	return result;
}

TPP_INLINE bool TPPConst_IsBool(tpp_expr_value *self) {
	tpp_intmax value;
	tpp_errno error;
	if (!tpp_expr_value_isint(self))
		return false;
	error = tpp_expr_value_asint(self, &value);
	if (TPP_ISERR(error))
		return TPP2_FATAL(false);
	return value == 0 || value == 1;
}

TPP_INLINE tpp_intmax TPPConst_AsInt(tpp_expr_value *self) {
	tpp_errno error;
	tpp_intmax value;
	if (tpp_expr_value_isint(self)) {
		error = tpp_expr_value_asint(self, &value);
	} else if (tpp_expr_value_isfloat(self)) {
		tpp_float float_value;
		error = tpp_expr_value_asint(self, &float_value);
		value = (tpp_intmax)float_value;
	} else if (tpp_expr_value_isstring(self)) {
		TPP_REF tpp_string *string_value;
		error = tpp_expr_value_asstringref(self, &string_value);
		if (TPP_ISERR(error))
			return TPP2_FATAL(0);
		value = tpp_string_len(string_value) ? 1 : 0;
		tpp_string_decref(string_value);
	} else {
		return TPP2_FATAL(0);
	}
	if (TPP_ISERR(error))
		return TPP2_FATAL(0);
	return value;
}

TPP_INLINE tpp_float TPPConst_AsFloat(tpp_expr_value *self) {
	tpp_errno error;
	tpp_float value;
	if (tpp_expr_value_isint(self)) {
		tpp_intmax int_value;
		error = tpp_expr_value_asint(self, &int_value);
		value = (tpp_float)int_value;
	} else if (tpp_expr_value_isfloat(self)) {
		error = tpp_expr_value_asfloat(self, &value);
	} else if (tpp_expr_value_isstring(self)) {
		TPP_REF tpp_string *string_value;
		error = tpp_expr_value_asstringref(self, &string_value);
		if (TPP_ISERR(error))
			return TPP2_FATAL(0.0);
		value = tpp_string_len(string_value) ? 1.0 : 0.0;
		tpp_string_decref(string_value);
	} else {
		return TPP2_FATAL(0.0);
	}
	if (TPP_ISERR(error))
		return TPP2_FATAL(0.0);
	return value;
}

TPP_INLINE void TPPConst_InitCopy(tpp_expr_value *self, tpp_expr_value *right) {
	tpp_errno error = tpp_expr_value_copy(self, right);
	if (TPP_ISERR(error))
		(void)TPP2_FATAL(0);
}

#define TPPConst_ToBool(self) TPPConst_ToBool_(TPP2_LEXER, self)
TPP_INLINE void TPPConst_ToBool_(tpp_lexer *lexer, tpp_expr_value *self) {
	bool result;
	tpp_errno error = tpp_expr_value_asbool(lexer, self, &result);
	if (TPP_ISERR(error))
		result = TPP2_FATAL(false);
	tpp_expr_value_fini(self);
	error = tpp_expr_value_init_bool(self, result);
	if (TPP_ISERR(error))
		(void)TPP2_FATAL(0);
}

TPP_INLINE void TPPConst_ToInt(tpp_expr_value *self) {
	if (tpp_expr_value_isfloat(self)) {
		tpp_float value;
		tpp_errno error = tpp_expr_value_asfloat(self, &value);
		if (TPP_ISERR(error))
			value = TPP2_FATAL(0.0);
		tpp_expr_value_fini(self);
		error = tpp_expr_value_init_int(self, (tpp_intmax)value);
		if (TPP_ISERR(error))
			(void)TPP2_FATAL(0);
	} else if (tpp_expr_value_isstring(self)) {
		TPP_REF tpp_string *string_value;
		tpp_errno error = tpp_expr_value_asstringref(self, &string_value);
		if (TPP_ISERR(error))
			string_value = TPP2_FATAL(tpp_string_newempty());
		tpp_expr_value_fini(self);
		error = tpp_expr_value_init_int(self, tpp_string_len(string_value) ? 1 : 0);
		tpp_string_decref(string_value);
		if (TPP_ISERR(error))
			(void)TPP2_FATAL(0);
	} else if (!tpp_expr_value_isint(self)) {
		(void)TPP2_FATAL(0);
	}
}

TPP_INLINE void TPPConst_ZERO(tpp_expr_value *self) {
	tpp_errno error = tpp_expr_value_init_int(self, 0);
	if (TPP_ISERR(error))
		(void)TPP2_FATAL(0);
}

#define TPPConst_Quit(self) tpp_expr_value_fini(self)

/* Convert a given preprocessor constant into a string:
 * >> The returned string can be used to create a file
 *    that represents the constant's value as loaded
 *    by the `__TPP_EVAL' extension.
 * NOTE: If `self' is a string, it will be escaped.
 * @return: NULL: Not enough available memory. */
#define TPPConst_ToString_(lexer, self) TPPConst_ToString(self)
TPP_INLINE /*ref*/ tpp_string *TPPCALL
TPPConst_ToString(tpp_expr_value const *tpp_restrict self) {
	tpp_ssize status;
	tpp_string_builder builder;
	tpp_string_builder_init(&builder);
	status = tpp_expr_value_printrepr((tpp_expr_value *)self,
	                                  &tpp_string_builder_print,
	                                  &builder);
	if (status < 0) {
		tpp_string_builder_fini(&builder);
		return NULL;
	}
	return tpp_string_builder_pack(&builder);
}

/* Evaluate a constant expression as found after `#if' or in `__TPP_EVAL(...)'
 * NOTE: If `result' is NULL, the expression's is
 *       parsed, yet warnings will not be emit.
 * NOTE: Expects the current token to point to the first one part of the expression.
 *       Upon exit, that token will point to the first one past the expression.
 * NOTE: Evaluation is compatible with standard c rules, but
 *       `,' operators are not parsed at the highest level.
 * @return: 1: Successfully parsed an expression
 * @return: 0: An error occurred. */
#define TPPLexer_Eval(result) TPPLexer_Eval_(TPP2_LEXER, result)
TPP_INLINE int TPPCALL TPPLexer_Eval_(tpp_lexer *self, tpp_expr_value *result) {
	tpp_errno error;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file_setpos(file, tpp_lexer_gettokenstart(self)); /* Rewind so current token gets yielded again */
	error = tpp_lexer_parseexpr(self, result);
	return TPP_ISERR(error) ? 0 : 1;
}

/* Parse an evaluate a string from the current lexer.
 * NOTE: This functions expects the current token to be a string token
 *       and will continue parsing and concat-ing strings until the
 *       next non-string token.
 * @return: * :   A reference to the unescaped string that was parsed.
 * @return: NULL: A lexer error occurred (TPPLexer_SetErr() was set; (TPP_CONFIG_SET_API_ERROR)). */
#define TPPLexer_ParseString() TPPLexer_ParseString_(TPP2_LEXER)
TPP_INLINE TPP_REF tpp_string *TPPCALL TPPLexer_ParseString_(tpp_lexer *self) {
	TPP_REF tpp_string *result;
	tpp_errno error = tpp_lexer_parsestring(self, &result, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
	if (TPP_ISERR(error))
		result = NULL;
	return result;
}


#define TPP_ATOI_ERR           0x00 /* NOTE: Never used with any flags (indicates failure). */
#define TPP_ATOI_OK            0x01 /* Always set on success. */
#define TPP_ATOI_UNSIGNED      0x02 /* Unless set, the integral is signed. */
#define TPP_ATOI_TYPE_MASK     0xf0 /* Mask of the integral's typing (NOTE: The function already clamped the resulting value with this type's range). */
#define TPP_ATOI_TYPE_INT      0x00 /* `int' (default typing without suffix/for chars). */
#define TPP_ATOI_TYPE_LONG     0x10 /* `long'. */
#define TPP_ATOI_TYPE_LONGLONG 0x20 /* `long long'. */
#define TPP_ATOI_TYPE_INT8     0x30 /* `__int8' (msvc-extension). */
#define TPP_ATOI_TYPE_INT16    0x40 /* `__int16' (msvc-extension). */
#define TPP_ATOI_TYPE_INT32    0x50 /* `__int32' (msvc-extension). */
#define TPP_ATOI_TYPE_INT64    0x60 /* `__int64' (msvc-extension). */

/* Transform the current token (which must either be `TOK_INT' or `TOK_CHAR')
 * into an integral value, storing that value in `*pint' and returning
 * a set of `TPP_ATOI_*' flags, indicating typing and success.
 * NOTE: This function does _NOT_ yield the current token once finished.
 *       If intended, the caller is responsible for advancing it upon success.
 * @return: TPP_ATOI_ERR: Emiting a warning caused the lexer to error out (TPPLexer_SetErr() was set).
 * @return: * :           A set of `TPP_ATOI_*' (see below) */
#define TPP_Atoi(pint) TPP_Atoi_(TPP_LEXER_ARG_ pint)
TPP_INLINE int TPPCALL TPP_Atoi_(tpp_lexer *self, tpp_intmax *tpp_restrict pint) {
	tpp_errno error;
	tpp_token_id tok = tpp_lexer_gettok(self);
	tpp_integer_suffix_kind kind;
	if (TPP_TOK_ISSTRING(tok)) {
		/* Parse strings into character literals */
		error = tpp_lexer_parsecharacter_literal(self, pint, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
		if (TPP_ISERR(error))
			return TPP_ATOI_ERR;
		/* Rewind so token after character literal gets yielded again */
		tpp_file_setpos(tpp_lexer_getfile(self), tpp_lexer_gettokenstart(self));
//		return TPP_ATOI_OK | TPP_ATOI_TYPE_INT | TPP_ATOI_UNSIGNED; /* Returned by TPP2 dependent on "TPPLEXER_FLAG_CHAR_UNSIGNED" */
		return TPP_ATOI_OK | TPP_ATOI_TYPE_INT;
	}
	error = tpp_lexer_decodeint_ex(self, pint, &kind);
	if (TPP_ISERR(error))
		return TPP_ATOI_ERR;
	switch (kind) {
	case TPP_INTEGER_SUFFIX_KIND_INT:
		return TPP_ATOI_OK | TPP_ATOI_TYPE_INT;
#if TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS
	case TPP_INTEGER_SUFFIX_KIND_UNSIGNED:
		return TPP_ATOI_OK | TPP_ATOI_TYPE_INT | TPP_ATOI_UNSIGNED;
	case TPP_INTEGER_SUFFIX_KIND_LONG:
		return TPP_ATOI_OK | TPP_ATOI_TYPE_LONG;
	case TPP_INTEGER_SUFFIX_KIND_UNSIGNED_LONG:
		return TPP_ATOI_OK | TPP_ATOI_UNSIGNED | TPP_ATOI_TYPE_LONG;
	case TPP_INTEGER_SUFFIX_KIND_LONG_LONG:
		return TPP_ATOI_OK | TPP_ATOI_TYPE_LONGLONG;
	case TPP_INTEGER_SUFFIX_KIND_UNSIGNED_LONG_LONG:
		return TPP_ATOI_OK | TPP_ATOI_UNSIGNED | TPP_ATOI_TYPE_LONGLONG;
#endif /* TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS */
#if TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
	case TPP_INTEGER_SUFFIX_KIND_INT8:
		return TPP_ATOI_OK | TPP_ATOI_TYPE_INT8;
	case TPP_INTEGER_SUFFIX_KIND_INT16:
		return TPP_ATOI_OK | TPP_ATOI_TYPE_INT16;
	case TPP_INTEGER_SUFFIX_KIND_INT32:
		return TPP_ATOI_OK | TPP_ATOI_TYPE_INT32;
	case TPP_INTEGER_SUFFIX_KIND_INT64:
		return TPP_ATOI_OK | TPP_ATOI_TYPE_INT64;
	case TPP_INTEGER_SUFFIX_KIND_UINT8:
		return TPP_ATOI_OK | TPP_ATOI_UNSIGNED | TPP_ATOI_TYPE_INT8;
	case TPP_INTEGER_SUFFIX_KIND_UINT16:
		return TPP_ATOI_OK | TPP_ATOI_UNSIGNED | TPP_ATOI_TYPE_INT16;
	case TPP_INTEGER_SUFFIX_KIND_UINT32:
		return TPP_ATOI_OK | TPP_ATOI_UNSIGNED | TPP_ATOI_TYPE_INT32;
	case TPP_INTEGER_SUFFIX_KIND_UINT64:
		return TPP_ATOI_OK | TPP_ATOI_UNSIGNED | TPP_ATOI_TYPE_INT64;
#endif /* TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS */
	default: tpp_unreachable();
	}
	tpp_unreachable();
}

#if 0 /* TODO */
///* Transform the current token (which must be `TOK_FLOAT') into a
// * floating point value, storing that value in `*pfloat' and returning
// * a set of `TPP_ATOF_*' flags, indicating typing and success.
// * NOTE: This function does _NOT_ yield the current token once finished.
// *       If intended, the caller is responsible for advancing it upon success.
// * @return: TPP_ATOF_ERR: Emiting a warning caused the lexer to error out (TPPLexer_SetErr() was set).
// * @return: * :           A set of `TPP_ATOF_*' (see below) */
//TPPFUN int TPPCALL TPP_Atof_(TPP_LEXER_PARAM_ TPP(tfloat_t) *tpp_restrict pfloat);
//#define TPP_Atof(pfloat) TPP_Atof_(TPP_LEXER_ARG_ pfloat)
//#define TPP_ATOF_ERR             0x00 /* NOTE: Never used with any flags (indicates failure). */
//#define TPP_ATOF_OK              0x01 /* Always set on success. */
//#define TPP_ATOF_TYPE_MASK       0xf0 /* Mask of the float's typing. */
//#define TPP_ATOF_TYPE_DOUBLE     0x00 /* `double' (default typing without suffix). */
//#define TPP_ATOF_TYPE_FLOAT      0x10 /* `float' (float-suffix `f') */
//#define TPP_ATOF_TYPE_LONGDOUBLE 0x20 /* `long double' (long-double-suffix `L'). */
#endif


#if 0 /* TODO */
///* Prints the text contained within the current token, automatically
// * skipping escaped linefeeds and converting di/trigraphs.
// * NOTE: `TPP_PrintComment' behaves similar, but will
// *        instead handle any kind of comment token,
// *        printing the comment text within.
// * @return: >= 0: Sum of all return values from `printer'.
// * @return: <  0: The first negative value returned by `printer' */
//TPPFUN ptrdiff_t TPPCALL TPP_PrintToken_(TPP_LEXER_PARAM_ TPP(printer_t) printer, void *closure);
//TPPFUN ptrdiff_t TPPCALL TPP_PrintComment_(TPP_LEXER_PARAM_ TPP(printer_t) printer, void *closure);
//#define TPP_PrintToken(printer, closure)   TPP_PrintToken_(TPP_LEXER_ARG_ printer, closure)
//#define TPP_PrintComment(printer, closure) TPP_PrintComment_(TPP_LEXER_ARG_ printer, closure)
#endif


/* Helper macros to initialize/finalize the global TPP context.
 * NOTE: These macros can (obviously) be called when
 *      `TPPLexer_Current' is NULL, or not initialized. */
#if TPP_CONFIG_ONELEXER == 3
/* You have to allocate+TPPLexer_Init()+TPPLexer_Quit() your own lexer(s) */
#elif TPP_CONFIG_ONELEXER
#define TPP_INITIALIZE() TPPLexer_Init(&TPPLexer_Global)
#define TPP_FINALIZE()   TPPLexer_Quit(&TPPLexer_Global)
#else /* TPP_CONFIG_ONELEXER */
#define TPP_INITIALIZE()                                                        \
	(TPPLexer_Current = (struct TPPLexer *)tpp_malloc(sizeof(struct TPPLexer)), \
	 TPPLexer_Current ? (TPPLexer_Init(TPPLexer_Current) ? 1 : (tpp_free(TPPLexer_Current), 0)) : 0)
#define TPP_FINALIZE() (TPPLexer_Quit(TPPLexer_Current), tpp_free(TPPLexer_Current))
#endif /* !TPP_CONFIG_ONELEXER */

TPP_DECL_END

#endif /* !GUARD_TPP2_H */
#endif /* !TPP_DEFS */

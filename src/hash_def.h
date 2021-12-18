#ifndef __HASH_DEF_64
#define __HASH_DEF_64

#ifdef __KEEP_ENTRY_NAMES
#define ENTRY_NAME(x) (x)
#else
#define ENTRY_NAME(x) (NULL)
#endif

// Define __SHOW_MISSING_HASH in conf.h to display name <-> hash relationship

#define NOP_HASH                       (hash_t) 0xB889592
#define PRINT_TOS_HASH                 (hash_t) 0x2B5D3
#define DUP_HASH                       (hash_t) 0xB886BCE
#define OVER_HASH                      (hash_t) 0x17C9BF101
#define SWAP_HASH                      (hash_t) 0x17C9E2640
#define DROP_HASH                      (hash_t) 0x17C95D91A
#define ROT_HASH                       (hash_t) 0xB88A69A
#define MINUS_ROT_HASH                 (hash_t) 0x17C77B047
#define NIP_HASH                       (hash_t) 0xB8894CC
#define ADD_HASH                       (hash_t) 0x2B5D0
#define SUB_HASH                       (hash_t) 0x2B5D2
#define MUL_HASH                       (hash_t) 0x2B5CF
#define EQUAL_HASH                     (hash_t) 0x2B5E2
#define LESS_HASH                      (hash_t) 0x2B5E1
#define BYE_HASH                       (hash_t) 0xB8863C5
#define WORDS_HASH                     (hash_t) 0x3110A73474
#define EMIT_HASH                      (hash_t) 0x17C964F74
#define KEY_HASH                       (hash_t) 0xB88878E
#define LOAD_HASH                      (hash_t) 0x2B5E5
#define STORE_HASH                     (hash_t) 0x2B5C6
#define CLOAD_HASH                     (hash_t) 0x597748
#define CSTORE__HASH                   (hash_t) 0x597729
#define CELL_HASH                      (hash_t) 0x17C951505
#define HERE_HASH                      (hash_t) 0x17C97D3A9
#define LATEST_HASH                    (hash_t) 0x6530AE8CA72
#define I_LATEST_HASH                  (hash_t) 0x1AE713EDBA0DE8
#define COMPILE_LITERAL_HASH           (hash_t) 0x597281
#define GET_ENTRY_CODE_HASH            (hash_t) 0x310F3D0EBE
#define EXECUTE_HASH                   (hash_t) 0xD0B281C09678
#define DOT_S_HASH                     (hash_t) 0x5970A6
#define RETURN_HASH                    (hash_t) 0x2B5E0
#define WHEN_HASH                      (hash_t) 0x17CA01877
#define UNLESS_HASH                    (hash_t) 0x65320CE139F
#define CHOOSE_HASH                    (hash_t) 0x652F66674E6
#define R_PUSH_HASH                    (hash_t) 0x597295
#define R_POP_HASH                     (hash_t) 0x597515
#define R_FETCH_HASH                   (hash_t) 0x597517

// Lib
#define BASE_STORE_HASH                (hash_t) 0x310F2388A1
#define BASE_LOAD_HASH                 (hash_t) 0x310F2388C0
#define ENTRY_IS_HASH                  (hash_t) 0x1AE6FFDC1AE3E2
#define ENTRY_HIDE_HASH                (hash_t) 0x7270A6674E62BAC0
#define ROOM_HASH                      (hash_t) 0x17C9D79A2

// Registers
#define REG_A_LOAD_HASH                (hash_t) 0x5972E6
#define REG_A_STORE_HASH               (hash_t) 0x5972C7
#define REG_A_ADD_STORE_HASH           (hash_t) 0xB87CD12
#define REG_A_ADD_ADD_HASH             (hash_t) 0xB87CD1C
#define REG_A_SUB_SUB_HASH             (hash_t) 0xB87CD60
#define REG_A_TO_R_HASH                (hash_t) 0xB87CFB6
#define REG_R_TO_A_HASH                (hash_t) 0xB8817F6

#define REG_B_LOAD_HASH                (hash_t) 0x597307
#define REG_B_STORE_HASH               (hash_t) 0x5972E8
#define REG_B_ADD_STORE_HASH           (hash_t) 0xB87D153
#define REG_B_ADD_ADD_HASH             (hash_t) 0xB87D15D
#define REG_B_SUB_SUB_HASH             (hash_t) 0xB87D1A1
#define REG_B_TO_R_HASH                (hash_t) 0xB87D3F7
#define REG_R_TO_B_HASH                (hash_t) 0xB8817F7

#define REG_C_LOAD_HASH                (hash_t) 0x597328
#define REG_C_STORE_HASH               (hash_t) 0x597309
#define REG_C_ADD_STORE_HASH           (hash_t) 0xB87D594
#define REG_C_ADD_ADD_HASH             (hash_t) 0xB87D59E
#define REG_C_SUB_SUB_HASH             (hash_t) 0xB87D5E2
#define REG_C_TO_R_HASH                (hash_t) 0xB87D838
#define REG_R_TO_C_HASH                (hash_t) 0xB8817F8

#define REG_I_LOAD_HASH                (hash_t) 0x5973EE
#define REG_I_STORE_HASH               (hash_t) 0x5973CF
#define REG_I_ADD_STORE_HASH           (hash_t) 0xB87EF1A
#define REG_I_ADD_ADD_HASH             (hash_t) 0xB87EF24
#define REG_I_SUB_SUB_HASH             (hash_t) 0xB87EF68
#define REG_I_TO_R_HASH                (hash_t) 0xB87F1BE
#define REG_R_TO_I_HASH                (hash_t) 0xB8817FE

#define REG_J_LOAD_HASH                (hash_t) 0x59740F
#define REG_J_STORE_HASH               (hash_t) 0x5973F0
#define REG_J_ADD_STORE_HASH           (hash_t) 0xB87F35B
#define REG_J_ADD_ADD_HASH             (hash_t) 0xB87F365
#define REG_J_SUB_SUB_HASH             (hash_t) 0xB87F3A9
#define REG_J_TO_R_HASH                (hash_t) 0xB87F5FF
#define REG_R_TO_J_HASH                (hash_t) 0xB8817FF

// OS
#define SYSTEM_HASH                    (hash_t) 0x6531CEEE48A
#define C_SUBALLOC_HASH                (hash_t) 0xD0B138959C40
#define C_SUBFREE_HASH                 (hash_t) 0x652F235EAB7

// Dict
#define SEE_HASH                       (hash_t) 0xB88A982
#define DISASSEMBLE_HASH               (hash_t) 0xC07F2D5B2CDA32F1
#define FULLROOM_HASH                  (hash_t) 0x1AE70BD01BC455
#define ENTRY__PATCH_HASH              (hash_t) 0xC08573511B46B7F6

// IO
#define ECHO_STORE_HASH                (hash_t) 0x310F5ABD65
#define FILE_SUBSIZE_HASH              (hash_t) 0x377C80EAB4BB74D
#define FILE_LOAD_HASH                 (hash_t) 0x17C9A2D85
#define FILE_SAVE_HASH                 (hash_t) 0x17C9DCB54
#define INCLUDED_HASH                  (hash_t) 0x1AE727650CA10D

// Parser
#define PARSE_HASH                     (hash_t) 0x311020DD80
#define CLEAR_TIB_HASH                 (hash_t) 0x377C453F329FBF8

// Time
#define TIME_HASH                      (hash_t) 0x17C9E7894
#define UTIME_HASH                     (hash_t) 0x3110859C29
#define SLEEP_HASH                     (hash_t) 0x31105CF61E
#define MSSLEEP_HASH                   (hash_t) 0xD0B4DEEAB1DE

// Math
#define DIV_HASH                       (hash_t) 0x2B5D4
#define MOD_HASH                       (hash_t) 0xB889145
#define DIVMOD_HASH                    (hash_t) 0x17C78B3B4
#define MUL_DIV_HASH                   (hash_t) 0x596FDE
#define LSHIFT_HASH                    (hash_t) 0x6530C27FE4F
#define RSHIFT_HASH                    (hash_t) 0x6531A26F215
#define SUP_HASH                       (hash_t) 0x2B5E3
#define INF_EQUAL_HASH                 (hash_t) 0x59723E
#define SUP_EQUAL_HASH                 (hash_t) 0x597280
#define INFSUP_HASH                    (hash_t) 0x59723F
#define OR_HASH                        (hash_t) 0x597906
#define AND_HASH                       (hash_t) 0xB885E18
#define MIN_HASH                       (hash_t) 0xB889089
#define MAX_HASH                       (hash_t) 0xB888F8B
#define RAND_HASH                      (hash_t) 0x17C9D3DEA
#define SRAND_HASH                     (hash_t) 0x311060307D
#define RANDOM_HASH                    (hash_t) 0x65318E46F26

// Float
#define FLOAT_HASH                     (hash_t) 0x310F71E19B
#define F_DOTS_HASH                    (hash_t) 0xB886B2C
#define F_DOT_HASH                     (hash_t) 0x597799
#define FDROP_HASH                     (hash_t) 0x310F6D8D20
#define FDUP_HASH                      (hash_t) 0x17C96B714
#define FSWAP_HASH                     (hash_t) 0x310F75DA46
#define FOVER_HASH                     (hash_t) 0x310F73A507
#define FROT_HASH                      (hash_t) 0x17C96F1E0
#define F_SUBROT_HASH                  (hash_t) 0x310F4F644D
#define FNIP_HASH                      (hash_t) 0x17C96E012
#define F_ADD_HASH                     (hash_t) 0x597796
#define F_SUB_HASH                     (hash_t) 0x597798
#define F_MUL_HASH                     (hash_t) 0x597795
#define F_DIV_HASH                     (hash_t) 0x59779A
#define F_TO__HASH                     (hash_t) 0x5977A9
#define F_FROM_HASH                    (hash_t) 0x5977A7
#define F_LOAD_HASH                    (hash_t) 0x5977AB
#define F_STORE_HASH                   (hash_t) 0x59778C
#define F_INLINE_HASH                  (hash_t) 0xB886D07

// MPZ
#define MPZ_HASH                       (hash_t) 0xB88917C
#define M_DOTS_HASH                    (hash_t) 0xB8888F3
#define M_DOT_HASH                     (hash_t) 0x597880
#define MDROP_HASH                     (hash_t) 0x310FEC38A7
#define MDUP_HASH                      (hash_t) 0x17C9A8DBB
#define MSWAP_HASH                     (hash_t) 0x310FF485CD
#define MOVER_HASH                     (hash_t) 0x310FF2508E
#define MROT_HASH                      (hash_t) 0x17C9AC887
#define M_SUBROT_HASH                  (hash_t) 0x310FCE0FD4
#define MNIP_HASH                      (hash_t) 0x17C9AB6B9
#define M_ADD_HASH                     (hash_t) 0x59787D
#define M_SUB_HASH                     (hash_t) 0x59787F
#define M_MUL_HASH                     (hash_t) 0x59787C
#define M_DIV_HASH                     (hash_t) 0x597881
#define M_TO__HASH                     (hash_t) 0x597890
#define M_FROM_HASH                    (hash_t) 0x59788E
#define M_EQUAL_HASH                   (hash_t) 0x59788F
#define M_LOAD_HASH                    (hash_t) 0x597892
#define M_STORE_HASH                   (hash_t) 0x597873
#define M_INLINE_HASH                  (hash_t) 0xB888ACE

// Threads
#define THREAD__RUN_HASH               (hash_t) 0x727317CCDA8200A1
#define THREAD__JOIN_SUBALL_HASH       (hash_t) 0x76D38AD3FF9B8E02
#define THREAD__JOIN_HASH              (hash_t) 0xC0D611682ABD97FC
#define THREAD__KILL_HASH              (hash_t) 0xC0D611682ABE0B38
#define THREAD__LOCK_HASH              (hash_t) 0xC0D611682ABEAFF5
#define THREAD__UNLOCK_HASH            (hash_t) 0x4EA00C1DEA1BE3F8

#endif /* __HASH_DEF_64 */

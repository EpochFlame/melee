#include <dolphin/types.h>
#include <dolphin/base/PPCArch.h>
#include <dolphin/os/OSContext.h>
#include <dolphin/os/OSInterrupt.h>
#include <dolphin/os/OSThread.h>
#include <dolphin/os/OSTime.h>

typedef struct OSAlarm OSAlarm;

typedef void (*OSAlarmHandler)(struct OSAlarm* alarm, OSContext* context);

struct OSAlarm {
    OSAlarmHandler  handler;
    u32             tag;
    OSTime          fire;
    OSAlarm*        prev;
    OSAlarm*        next;
    OSTime          period;
    OSTime          start;
};

static OSThreadQueue AlarmQueue;
extern unk_t __OSSetExceptionHandler();
extern unk_t __div2i();
extern OSTime __OSGetSystemTime();

typedef u8 __OSException;
typedef void (*__OSExceptionHandler)(__OSException exception, OSContext* context);
extern void DecrementerExceptionHandler(__OSException exception, OSContext* context);

extern __OSExceptionHandler __OSGetExceptionHandler(__OSException);

void OSInitAlarm(void)
{
    if (__OSGetExceptionHandler(8) != DecrementerExceptionHandler) {
        AlarmQueue.tail = 0;
        AlarmQueue.head = 0;
        __OSSetExceptionHandler(8, DecrementerExceptionHandler);
    }
}

void OSCreateAlarm(s32* arg0)
{
    *arg0 = 0;
}

#pragma push
asm unk_t InsertAlarm()
{ // clang-format off
    nofralloc
/* 80343778 00340358  7C 08 02 A6 */	mflr r0
/* 8034377C 0034035C  90 01 00 04 */	stw r0, 4(r1)
/* 80343780 00340360  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 80343784 00340364  BF 21 00 24 */	stmw r25, 0x24(r1)
/* 80343788 00340368  3B 80 00 00 */	li r28, 0
/* 8034378C 0034036C  7C 7D 1B 78 */	mr r29, r3
/* 80343790 00340370  6F 84 80 00 */	xoris r4, r28, 0x8000
/* 80343794 00340374  3B 25 00 00 */	addi r25, r5, 0
/* 80343798 00340378  3B C6 00 00 */	addi r30, r6, 0
/* 8034379C 0034037C  3B E7 00 00 */	addi r31, r7, 0
/* 803437A0 00340380  80 03 00 18 */	lwz r0, 0x18(r3)
/* 803437A4 00340384  81 03 00 1C */	lwz r8, 0x1c(r3)
/* 803437A8 00340388  6C 03 80 00 */	xoris r3, r0, 0x8000
/* 803437AC 0034038C  7C 08 E0 10 */	subfc r0, r8, r28
/* 803437B0 00340390  7C 63 21 10 */	subfe r3, r3, r4
/* 803437B4 00340394  7C 64 21 10 */	subfe r3, r4, r4
/* 803437B8 00340398  7C 63 00 D1 */	neg. r3, r3
/* 803437BC 0034039C  41 82 00 7C */	beq lbl_80343838
/* 803437C0 003403A0  48 00 8C 51 */	bl __OSGetSystemTime
/* 803437C4 003403A4  80 FD 00 20 */	lwz r7, 0x20(r29)
/* 803437C8 003403A8  6C 65 80 00 */	xoris r5, r3, 0x8000
/* 803437CC 003403AC  81 1D 00 24 */	lwz r8, 0x24(r29)
/* 803437D0 003403B0  6C E6 80 00 */	xoris r6, r7, 0x8000
/* 803437D4 003403B4  7C 04 40 10 */	subfc r0, r4, r8
/* 803437D8 003403B8  7C A5 31 10 */	subfe r5, r5, r6
/* 803437DC 003403BC  7C A6 31 10 */	subfe r5, r6, r6
/* 803437E0 003403C0  7C A5 00 D1 */	neg. r5, r5
/* 803437E4 003403C4  3B C8 00 00 */	addi r30, r8, 0
/* 803437E8 003403C8  3B 27 00 00 */	addi r25, r7, 0
/* 803437EC 003403CC  41 82 00 4C */	beq lbl_80343838
/* 803437F0 003403D0  83 7D 00 18 */	lwz r27, 0x18(r29)
/* 803437F4 003403D4  7C 88 20 10 */	subfc r4, r8, r4
/* 803437F8 003403D8  83 5D 00 1C */	lwz r26, 0x1c(r29)
/* 803437FC 003403DC  7C 67 19 10 */	subfe r3, r7, r3
/* 80343800 003403E0  38 BB 00 00 */	addi r5, r27, 0
/* 80343804 003403E4  38 DA 00 00 */	addi r6, r26, 0
/* 80343808 003403E8  4B FD F2 01 */	bl __div2i
/* 8034380C 003403EC  38 00 00 01 */	li r0, 1
/* 80343810 003403F0  7C A4 00 14 */	addc r5, r4, r0
/* 80343814 003403F4  7C 9B 29 D6 */	mullw r4, r27, r5
/* 80343818 003403F8  7C 1A 28 16 */	mulhwu r0, r26, r5
/* 8034381C 003403FC  7C 63 E1 14 */	adde r3, r3, r28
/* 80343820 00340400  7C 84 02 14 */	add r4, r4, r0
/* 80343824 00340404  7C 1A 19 D6 */	mullw r0, r26, r3
/* 80343828 00340408  7C 7A 29 D6 */	mullw r3, r26, r5
/* 8034382C 0034040C  7C 04 02 14 */	add r0, r4, r0
/* 80343830 00340410  7F DE 18 14 */	addc r30, r30, r3
/* 80343834 00340414  7F 39 01 14 */	adde r25, r25, r0
lbl_80343838:
/* 80343838 00340418  93 FD 00 00 */	stw r31, 0(r29)
/* 8034383C 0034041C  6F 24 80 00 */	xoris r4, r25, 0x8000
/* 80343840 00340420  93 DD 00 0C */	stw r30, 0xc(r29)
/* 80343844 00340424  93 3D 00 08 */	stw r25, 8(r29)
/* 80343848 00340428  80 CD BC B8 */	lwz r6, AlarmQueue(r13)
/* 8034384C 0034042C  48 00 00 C4 */	b lbl_80343910
lbl_80343850:
/* 80343850 00340430  80 06 00 08 */	lwz r0, 8(r6)
/* 80343854 00340434  80 A6 00 0C */	lwz r5, 0xc(r6)
/* 80343858 00340438  6C 03 80 00 */	xoris r3, r0, 0x8000
/* 8034385C 0034043C  7C 05 F0 10 */	subfc r0, r5, r30
/* 80343860 00340440  7C 63 21 10 */	subfe r3, r3, r4
/* 80343864 00340444  7C 64 21 10 */	subfe r3, r4, r4
/* 80343868 00340448  7C 63 00 D1 */	neg. r3, r3
/* 8034386C 0034044C  41 82 00 A0 */	beq lbl_8034390C
/* 80343870 00340450  80 06 00 10 */	lwz r0, 0x10(r6)
/* 80343874 00340454  90 1D 00 10 */	stw r0, 0x10(r29)
/* 80343878 00340458  93 A6 00 10 */	stw r29, 0x10(r6)
/* 8034387C 0034045C  90 DD 00 14 */	stw r6, 0x14(r29)
/* 80343880 00340460  80 7D 00 10 */	lwz r3, 0x10(r29)
/* 80343884 00340464  28 03 00 00 */	cmplwi r3, 0
/* 80343888 00340468  41 82 00 0C */	beq lbl_80343894
/* 8034388C 0034046C  93 A3 00 14 */	stw r29, 0x14(r3)
/* 80343890 00340470  48 00 01 24 */	b lbl_803439B4
lbl_80343894:
/* 80343894 00340474  93 AD BC B8 */	stw r29, AlarmQueue(r13)
/* 80343898 00340478  48 00 8B 79 */	bl __OSGetSystemTime
/* 8034389C 0034047C  80 DD 00 0C */	lwz r6, 0xc(r29)
/* 803438A0 00340480  38 E0 00 00 */	li r7, 0
/* 803438A4 00340484  80 1D 00 08 */	lwz r0, 8(r29)
/* 803438A8 00340488  6C E5 80 00 */	xoris r5, r7, 0x8000
/* 803438AC 0034048C  7D 04 30 10 */	subfc r8, r4, r6
/* 803438B0 00340490  7C 03 01 10 */	subfe r0, r3, r0
/* 803438B4 00340494  6C 06 80 00 */	xoris r6, r0, 0x8000
/* 803438B8 00340498  7C 07 40 10 */	subfc r0, r7, r8
/* 803438BC 0034049C  7C A5 31 10 */	subfe r5, r5, r6
/* 803438C0 003404A0  7C A6 31 10 */	subfe r5, r6, r6
/* 803438C4 003404A4  7C A5 00 D1 */	neg. r5, r5
/* 803438C8 003404A8  41 82 00 10 */	beq lbl_803438D8
/* 803438CC 003404AC  38 60 00 00 */	li r3, 0
/* 803438D0 003404B0  4B FF 25 B5 */	bl PPCMtdec
/* 803438D4 003404B4  48 00 00 E0 */	b lbl_803439B4
lbl_803438D8:
/* 803438D8 003404B8  3C 80 80 00 */	lis r4, 0x8000
/* 803438DC 003404BC  6C E3 80 00 */	xoris r3, r7, 0x8000
/* 803438E0 003404C0  7C 04 40 10 */	subfc r0, r4, r8
/* 803438E4 003404C4  7C 63 31 10 */	subfe r3, r3, r6
/* 803438E8 003404C8  7C 66 31 10 */	subfe r3, r6, r6
/* 803438EC 003404CC  7C 63 00 D1 */	neg. r3, r3
/* 803438F0 003404D0  41 82 00 10 */	beq lbl_80343900
/* 803438F4 003404D4  7D 03 43 78 */	mr r3, r8
/* 803438F8 003404D8  4B FF 25 8D */	bl PPCMtdec
/* 803438FC 003404DC  48 00 00 B8 */	b lbl_803439B4
lbl_80343900:
/* 80343900 003404E0  38 64 FF FF */	addi r3, r4, -1
/* 80343904 003404E4  4B FF 25 81 */	bl PPCMtdec
/* 80343908 003404E8  48 00 00 AC */	b lbl_803439B4
lbl_8034390C:
/* 8034390C 003404EC  80 C6 00 14 */	lwz r6, 0x14(r6)
lbl_80343910:
/* 80343910 003404F0  28 06 00 00 */	cmplwi r6, 0
/* 80343914 003404F4  40 82 FF 3C */	bne lbl_80343850
/* 80343918 003404F8  3B C0 00 00 */	li r30, 0
/* 8034391C 003404FC  93 DD 00 14 */	stw r30, 0x14(r29)
/* 80343920 00340500  38 6D BC B8 */	addi r3, r13, AlarmQueue
/* 80343924 00340504  80 83 00 04 */	lwz r4, 4(r3)
/* 80343928 00340508  97 A3 00 04 */	stwu r29, 4(r3)
/* 8034392C 0034050C  28 04 00 00 */	cmplwi r4, 0
/* 80343930 00340510  90 9D 00 10 */	stw r4, 0x10(r29)
/* 80343934 00340514  41 82 00 0C */	beq lbl_80343940
/* 80343938 00340518  93 A4 00 14 */	stw r29, 0x14(r4)
/* 8034393C 0034051C  48 00 00 78 */	b lbl_803439B4
lbl_80343940:
/* 80343940 00340520  93 A3 00 00 */	stw r29, 0(r3)
/* 80343944 00340524  93 AD BC B8 */	stw r29, AlarmQueue(r13)
/* 80343948 00340528  48 00 8A C9 */	bl __OSGetSystemTime
/* 8034394C 0034052C  80 DD 00 0C */	lwz r6, 0xc(r29)
/* 80343950 00340530  6F C5 80 00 */	xoris r5, r30, 0x8000
/* 80343954 00340534  80 1D 00 08 */	lwz r0, 8(r29)
/* 80343958 00340538  7C E4 30 10 */	subfc r7, r4, r6
/* 8034395C 0034053C  7C 03 01 10 */	subfe r0, r3, r0
/* 80343960 00340540  6C 06 80 00 */	xoris r6, r0, 0x8000
/* 80343964 00340544  7C 1E 38 10 */	subfc r0, r30, r7
/* 80343968 00340548  7C A5 31 10 */	subfe r5, r5, r6
/* 8034396C 0034054C  7C A6 31 10 */	subfe r5, r6, r6
/* 80343970 00340550  7C A5 00 D1 */	neg. r5, r5
/* 80343974 00340554  41 82 00 10 */	beq lbl_80343984
/* 80343978 00340558  38 60 00 00 */	li r3, 0
/* 8034397C 0034055C  4B FF 25 09 */	bl PPCMtdec
/* 80343980 00340560  48 00 00 34 */	b lbl_803439B4
lbl_80343984:
/* 80343984 00340564  3C 80 80 00 */	lis r4, 0x8000
/* 80343988 00340568  6F C3 80 00 */	xoris r3, r30, 0x8000
/* 8034398C 0034056C  7C 04 38 10 */	subfc r0, r4, r7
/* 80343990 00340570  7C 63 31 10 */	subfe r3, r3, r6
/* 80343994 00340574  7C 66 31 10 */	subfe r3, r6, r6
/* 80343998 00340578  7C 63 00 D1 */	neg. r3, r3
/* 8034399C 0034057C  41 82 00 10 */	beq lbl_803439AC
/* 803439A0 00340580  7C E3 3B 78 */	mr r3, r7
/* 803439A4 00340584  4B FF 24 E1 */	bl PPCMtdec
/* 803439A8 00340588  48 00 00 0C */	b lbl_803439B4
lbl_803439AC:
/* 803439AC 0034058C  38 64 FF FF */	addi r3, r4, -1
/* 803439B0 00340590  4B FF 24 D5 */	bl PPCMtdec
lbl_803439B4:
/* 803439B4 00340594  BB 21 00 24 */	lmw r25, 0x24(r1)
/* 803439B8 00340598  80 01 00 44 */	lwz r0, 0x44(r1)
/* 803439BC 0034059C  38 21 00 40 */	addi r1, r1, 0x40
/* 803439C0 003405A0  7C 08 03 A6 */	mtlr r0
/* 803439C4 003405A4  4E 80 00 20 */	blr 
} // clang-format on
#pragma pop

void OSSetAlarm(OSAlarm* alarm, OSTime tick, OSAlarmHandler handler)
{
    u32 oldInt = OSDisableInterrupts();
    alarm->period = 0;
    InsertAlarm(alarm, __OSGetSystemTime() + tick, handler);
    OSRestoreInterrupts(oldInt);
}

extern unk_t __OSTimeToSystemTime();

#pragma push
asm unk_t func_80343A30()
{ // clang-format off
    nofralloc
/* 80343A30 00340610  7C 08 02 A6 */	mflr r0
/* 80343A34 00340614  90 01 00 04 */	stw r0, 4(r1)
/* 80343A38 00340618  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 80343A3C 0034061C  BF 41 00 28 */	stmw r26, 0x28(r1)
/* 80343A40 00340620  3B 43 00 00 */	addi r26, r3, 0
/* 80343A44 00340624  3B 85 00 00 */	addi r28, r5, 0
/* 80343A48 00340628  3B 66 00 00 */	addi r27, r6, 0
/* 80343A4C 0034062C  3B A7 00 00 */	addi r29, r7, 0
/* 80343A50 00340630  3B E8 00 00 */	addi r31, r8, 0
/* 80343A54 00340634  3B C9 00 00 */	addi r30, r9, 0
/* 80343A58 00340638  48 00 39 0D */	bl OSDisableInterrupts
/* 80343A5C 0034063C  93 FA 00 1C */	stw r31, 0x1c(r26)
/* 80343A60 00340640  7C 7F 1B 78 */	mr r31, r3
/* 80343A64 00340644  38 9B 00 00 */	addi r4, r27, 0
/* 80343A68 00340648  93 BA 00 18 */	stw r29, 0x18(r26)
/* 80343A6C 0034064C  38 7C 00 00 */	addi r3, r28, 0
/* 80343A70 00340650  48 00 8A 05 */	bl __OSTimeToSystemTime
/* 80343A74 00340654  90 9A 00 24 */	stw r4, 0x24(r26)
/* 80343A78 00340658  38 FE 00 00 */	addi r7, r30, 0
/* 80343A7C 0034065C  38 C0 00 00 */	li r6, 0
/* 80343A80 00340660  90 7A 00 20 */	stw r3, 0x20(r26)
/* 80343A84 00340664  38 7A 00 00 */	addi r3, r26, 0
/* 80343A88 00340668  38 A0 00 00 */	li r5, 0
/* 80343A8C 0034066C  4B FF FC ED */	bl InsertAlarm
/* 80343A90 00340670  7F E3 FB 78 */	mr r3, r31
/* 80343A94 00340674  48 00 38 F9 */	bl OSRestoreInterrupts
/* 80343A98 00340678  BB 41 00 28 */	lmw r26, 0x28(r1)
/* 80343A9C 0034067C  80 01 00 44 */	lwz r0, 0x44(r1)
/* 80343AA0 00340680  38 21 00 40 */	addi r1, r1, 0x40
/* 80343AA4 00340684  7C 08 03 A6 */	mtlr r0
/* 80343AA8 00340688  4E 80 00 20 */	blr 
} // clang-format on
#pragma pop

#pragma push
asm unk_t OSCancelAlarm()
{ // clang-format off
    nofralloc
/* 80343AAC 0034068C  7C 08 02 A6 */	mflr r0
/* 80343AB0 00340690  90 01 00 04 */	stw r0, 4(r1)
/* 80343AB4 00340694  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 80343AB8 00340698  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 80343ABC 0034069C  93 C1 00 18 */	stw r30, 0x18(r1)
/* 80343AC0 003406A0  7C 7E 1B 78 */	mr r30, r3
/* 80343AC4 003406A4  93 A1 00 14 */	stw r29, 0x14(r1)
/* 80343AC8 003406A8  48 00 38 9D */	bl OSDisableInterrupts
/* 80343ACC 003406AC  80 1E 00 00 */	lwz r0, 0(r30)
/* 80343AD0 003406B0  3B E3 00 00 */	addi r31, r3, 0
/* 80343AD4 003406B4  28 00 00 00 */	cmplwi r0, 0
/* 80343AD8 003406B8  40 82 00 10 */	bne lbl_80343AE8
/* 80343ADC 003406BC  7F E3 FB 78 */	mr r3, r31
/* 80343AE0 003406C0  48 00 38 AD */	bl OSRestoreInterrupts
/* 80343AE4 003406C4  48 00 00 C8 */	b lbl_80343BAC
lbl_80343AE8:
/* 80343AE8 003406C8  83 BE 00 14 */	lwz r29, 0x14(r30)
/* 80343AEC 003406CC  28 1D 00 00 */	cmplwi r29, 0
/* 80343AF0 003406D0  40 82 00 14 */	bne lbl_80343B04
/* 80343AF4 003406D4  80 1E 00 10 */	lwz r0, 0x10(r30)
/* 80343AF8 003406D8  38 6D BC B8 */	addi r3, r13, AlarmQueue
/* 80343AFC 003406DC  90 03 00 04 */	stw r0, 4(r3)
/* 80343B00 003406E0  48 00 00 0C */	b lbl_80343B0C
lbl_80343B04:
/* 80343B04 003406E4  80 1E 00 10 */	lwz r0, 0x10(r30)
/* 80343B08 003406E8  90 1D 00 10 */	stw r0, 0x10(r29)
lbl_80343B0C:
/* 80343B0C 003406EC  80 7E 00 10 */	lwz r3, 0x10(r30)
/* 80343B10 003406F0  28 03 00 00 */	cmplwi r3, 0
/* 80343B14 003406F4  41 82 00 0C */	beq lbl_80343B20
/* 80343B18 003406F8  93 A3 00 14 */	stw r29, 0x14(r3)
/* 80343B1C 003406FC  48 00 00 80 */	b lbl_80343B9C
lbl_80343B20:
/* 80343B20 00340700  28 1D 00 00 */	cmplwi r29, 0
/* 80343B24 00340704  93 AD BC B8 */	stw r29, AlarmQueue(r13)
/* 80343B28 00340708  41 82 00 74 */	beq lbl_80343B9C
/* 80343B2C 0034070C  48 00 88 E5 */	bl __OSGetSystemTime
/* 80343B30 00340710  80 DD 00 0C */	lwz r6, 0xc(r29)
/* 80343B34 00340714  38 E0 00 00 */	li r7, 0
/* 80343B38 00340718  80 1D 00 08 */	lwz r0, 8(r29)
/* 80343B3C 0034071C  6C E5 80 00 */	xoris r5, r7, 0x8000
/* 80343B40 00340720  7D 04 30 10 */	subfc r8, r4, r6
/* 80343B44 00340724  7C 03 01 10 */	subfe r0, r3, r0
/* 80343B48 00340728  6C 06 80 00 */	xoris r6, r0, 0x8000
/* 80343B4C 0034072C  7C 07 40 10 */	subfc r0, r7, r8
/* 80343B50 00340730  7C A5 31 10 */	subfe r5, r5, r6
/* 80343B54 00340734  7C A6 31 10 */	subfe r5, r6, r6
/* 80343B58 00340738  7C A5 00 D1 */	neg. r5, r5
/* 80343B5C 0034073C  41 82 00 10 */	beq lbl_80343B6C
/* 80343B60 00340740  38 60 00 00 */	li r3, 0
/* 80343B64 00340744  4B FF 23 21 */	bl PPCMtdec
/* 80343B68 00340748  48 00 00 34 */	b lbl_80343B9C
lbl_80343B6C:
/* 80343B6C 0034074C  3C 80 80 00 */	lis r4, 0x8000
/* 80343B70 00340750  6C E3 80 00 */	xoris r3, r7, 0x8000
/* 80343B74 00340754  7C 04 40 10 */	subfc r0, r4, r8
/* 80343B78 00340758  7C 63 31 10 */	subfe r3, r3, r6
/* 80343B7C 0034075C  7C 66 31 10 */	subfe r3, r6, r6
/* 80343B80 00340760  7C 63 00 D1 */	neg. r3, r3
/* 80343B84 00340764  41 82 00 10 */	beq lbl_80343B94
/* 80343B88 00340768  7D 03 43 78 */	mr r3, r8
/* 80343B8C 0034076C  4B FF 22 F9 */	bl PPCMtdec
/* 80343B90 00340770  48 00 00 0C */	b lbl_80343B9C
lbl_80343B94:
/* 80343B94 00340774  38 64 FF FF */	addi r3, r4, -1
/* 80343B98 00340778  4B FF 22 ED */	bl PPCMtdec
lbl_80343B9C:
/* 80343B9C 0034077C  38 00 00 00 */	li r0, 0
/* 80343BA0 00340780  90 1E 00 00 */	stw r0, 0(r30)
/* 80343BA4 00340784  7F E3 FB 78 */	mr r3, r31
/* 80343BA8 00340788  48 00 37 E5 */	bl OSRestoreInterrupts
lbl_80343BAC:
/* 80343BAC 0034078C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 80343BB0 00340790  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 80343BB4 00340794  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 80343BB8 00340798  7C 08 03 A6 */	mtlr r0
/* 80343BBC 0034079C  83 A1 00 14 */	lwz r29, 0x14(r1)
/* 80343BC0 003407A0  38 21 00 20 */	addi r1, r1, 0x20
/* 80343BC4 003407A4  4E 80 00 20 */	blr 
} // clang-format on
#pragma pop

#pragma push
asm unk_t DecrementerExceptionCallback()
{ // clang-format off
    nofralloc
/* 80343BC8 003407A8  7C 08 02 A6 */	mflr r0
/* 80343BCC 003407AC  90 01 00 04 */	stw r0, 4(r1)
/* 80343BD0 003407B0  94 21 FD 10 */	stwu r1, -0x2f0(r1)
/* 80343BD4 003407B4  93 E1 02 EC */	stw r31, 0x2ec(r1)
/* 80343BD8 003407B8  93 C1 02 E8 */	stw r30, 0x2e8(r1)
/* 80343BDC 003407BC  93 A1 02 E4 */	stw r29, 0x2e4(r1)
/* 80343BE0 003407C0  7C 9D 23 78 */	mr r29, r4
/* 80343BE4 003407C4  93 81 02 E0 */	stw r28, 0x2e0(r1)
/* 80343BE8 003407C8  48 00 88 29 */	bl __OSGetSystemTime
/* 80343BEC 003407CC  80 0D BC B8 */	lwz r0, AlarmQueue(r13)
/* 80343BF0 003407D0  3B 84 00 00 */	addi r28, r4, 0
/* 80343BF4 003407D4  3B C3 00 00 */	addi r30, r3, 0
/* 80343BF8 003407D8  28 00 00 00 */	cmplwi r0, 0
/* 80343BFC 003407DC  7C 1F 03 78 */	mr r31, r0
/* 80343C00 003407E0  40 82 00 0C */	bne lbl_80343C0C
/* 80343C04 003407E4  7F A3 EB 78 */	mr r3, r29
/* 80343C08 003407E8  48 00 15 6D */	bl OSLoadContext
lbl_80343C0C:
/* 80343C0C 003407EC  80 1F 00 08 */	lwz r0, 8(r31)
/* 80343C10 003407F0  6F C4 80 00 */	xoris r4, r30, 0x8000
/* 80343C14 003407F4  80 BF 00 0C */	lwz r5, 0xc(r31)
/* 80343C18 003407F8  6C 03 80 00 */	xoris r3, r0, 0x8000
/* 80343C1C 003407FC  7C 05 E0 10 */	subfc r0, r5, r28
/* 80343C20 00340800  7C 63 21 10 */	subfe r3, r3, r4
/* 80343C24 00340804  7C 64 21 10 */	subfe r3, r4, r4
/* 80343C28 00340808  7C 63 00 D1 */	neg. r3, r3
/* 80343C2C 0034080C  41 82 00 7C */	beq lbl_80343CA8
/* 80343C30 00340810  48 00 87 E1 */	bl __OSGetSystemTime
/* 80343C34 00340814  80 DF 00 0C */	lwz r6, 0xc(r31)
/* 80343C38 00340818  38 E0 00 00 */	li r7, 0
/* 80343C3C 0034081C  80 1F 00 08 */	lwz r0, 8(r31)
/* 80343C40 00340820  6C E5 80 00 */	xoris r5, r7, 0x8000
/* 80343C44 00340824  7D 04 30 10 */	subfc r8, r4, r6
/* 80343C48 00340828  7C 03 01 10 */	subfe r0, r3, r0
/* 80343C4C 0034082C  6C 06 80 00 */	xoris r6, r0, 0x8000
/* 80343C50 00340830  7C 07 40 10 */	subfc r0, r7, r8
/* 80343C54 00340834  7C A5 31 10 */	subfe r5, r5, r6
/* 80343C58 00340838  7C A6 31 10 */	subfe r5, r6, r6
/* 80343C5C 0034083C  7C A5 00 D1 */	neg. r5, r5
/* 80343C60 00340840  41 82 00 10 */	beq lbl_80343C70
/* 80343C64 00340844  38 60 00 00 */	li r3, 0
/* 80343C68 00340848  4B FF 22 1D */	bl PPCMtdec
/* 80343C6C 0034084C  48 00 00 34 */	b lbl_80343CA0
lbl_80343C70:
/* 80343C70 00340850  3C 80 80 00 */	lis r4, 0x8000
/* 80343C74 00340854  6C E3 80 00 */	xoris r3, r7, 0x8000
/* 80343C78 00340858  7C 04 40 10 */	subfc r0, r4, r8
/* 80343C7C 0034085C  7C 63 31 10 */	subfe r3, r3, r6
/* 80343C80 00340860  7C 66 31 10 */	subfe r3, r6, r6
/* 80343C84 00340864  7C 63 00 D1 */	neg. r3, r3
/* 80343C88 00340868  41 82 00 10 */	beq lbl_80343C98
/* 80343C8C 0034086C  7D 03 43 78 */	mr r3, r8
/* 80343C90 00340870  4B FF 21 F5 */	bl PPCMtdec
/* 80343C94 00340874  48 00 00 0C */	b lbl_80343CA0
lbl_80343C98:
/* 80343C98 00340878  38 64 FF FF */	addi r3, r4, -1
/* 80343C9C 0034087C  4B FF 21 E9 */	bl PPCMtdec
lbl_80343CA0:
/* 80343CA0 00340880  7F A3 EB 78 */	mr r3, r29
/* 80343CA4 00340884  48 00 14 D1 */	bl OSLoadContext
lbl_80343CA8:
/* 80343CA8 00340888  80 7F 00 14 */	lwz r3, 0x14(r31)
/* 80343CAC 0034088C  28 03 00 00 */	cmplwi r3, 0
/* 80343CB0 00340890  90 6D BC B8 */	stw r3, AlarmQueue(r13)
/* 80343CB4 00340894  40 82 00 14 */	bne lbl_80343CC8
/* 80343CB8 00340898  38 00 00 00 */	li r0, 0
/* 80343CBC 0034089C  38 6D BC B8 */	addi r3, r13, AlarmQueue
/* 80343CC0 003408A0  90 03 00 04 */	stw r0, 4(r3)
/* 80343CC4 003408A4  48 00 00 0C */	b lbl_80343CD0
lbl_80343CC8:
/* 80343CC8 003408A8  38 00 00 00 */	li r0, 0
/* 80343CCC 003408AC  90 03 00 10 */	stw r0, 0x10(r3)
lbl_80343CD0:
/* 80343CD0 003408B0  83 DF 00 00 */	lwz r30, 0(r31)
/* 80343CD4 003408B4  38 C0 00 00 */	li r6, 0
/* 80343CD8 003408B8  6C C4 80 00 */	xoris r4, r6, 0x8000
/* 80343CDC 003408BC  90 DF 00 00 */	stw r6, 0(r31)
/* 80343CE0 003408C0  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 80343CE4 003408C4  80 BF 00 1C */	lwz r5, 0x1c(r31)
/* 80343CE8 003408C8  6C 03 80 00 */	xoris r3, r0, 0x8000
/* 80343CEC 003408CC  7C 05 30 10 */	subfc r0, r5, r6
/* 80343CF0 003408D0  7C 63 21 10 */	subfe r3, r3, r4
/* 80343CF4 003408D4  7C 64 21 10 */	subfe r3, r4, r4
/* 80343CF8 003408D8  7C 63 00 D1 */	neg. r3, r3
/* 80343CFC 003408DC  41 82 00 18 */	beq lbl_80343D14
/* 80343D00 003408E0  38 7F 00 00 */	addi r3, r31, 0
/* 80343D04 003408E4  38 FE 00 00 */	addi r7, r30, 0
/* 80343D08 003408E8  38 C0 00 00 */	li r6, 0
/* 80343D0C 003408EC  38 A0 00 00 */	li r5, 0
/* 80343D10 003408F0  4B FF FA 69 */	bl InsertAlarm
lbl_80343D14:
/* 80343D14 003408F4  83 8D BC B8 */	lwz r28, AlarmQueue(r13)
/* 80343D18 003408F8  28 1C 00 00 */	cmplwi r28, 0
/* 80343D1C 003408FC  41 82 00 74 */	beq lbl_80343D90
/* 80343D20 00340900  48 00 86 F1 */	bl __OSGetSystemTime
/* 80343D24 00340904  80 DC 00 0C */	lwz r6, 0xc(r28)
/* 80343D28 00340908  38 E0 00 00 */	li r7, 0
/* 80343D2C 0034090C  80 1C 00 08 */	lwz r0, 8(r28)
/* 80343D30 00340910  6C E5 80 00 */	xoris r5, r7, 0x8000
/* 80343D34 00340914  7D 04 30 10 */	subfc r8, r4, r6
/* 80343D38 00340918  7C 03 01 10 */	subfe r0, r3, r0
/* 80343D3C 0034091C  6C 06 80 00 */	xoris r6, r0, 0x8000
/* 80343D40 00340920  7C 07 40 10 */	subfc r0, r7, r8
/* 80343D44 00340924  7C A5 31 10 */	subfe r5, r5, r6
/* 80343D48 00340928  7C A6 31 10 */	subfe r5, r6, r6
/* 80343D4C 0034092C  7C A5 00 D1 */	neg. r5, r5
/* 80343D50 00340930  41 82 00 10 */	beq lbl_80343D60
/* 80343D54 00340934  38 60 00 00 */	li r3, 0
/* 80343D58 00340938  4B FF 21 2D */	bl PPCMtdec
/* 80343D5C 0034093C  48 00 00 34 */	b lbl_80343D90
lbl_80343D60:
/* 80343D60 00340940  3C 80 80 00 */	lis r4, 0x8000
/* 80343D64 00340944  6C E3 80 00 */	xoris r3, r7, 0x8000
/* 80343D68 00340948  7C 04 40 10 */	subfc r0, r4, r8
/* 80343D6C 0034094C  7C 63 31 10 */	subfe r3, r3, r6
/* 80343D70 00340950  7C 66 31 10 */	subfe r3, r6, r6
/* 80343D74 00340954  7C 63 00 D1 */	neg. r3, r3
/* 80343D78 00340958  41 82 00 10 */	beq lbl_80343D88
/* 80343D7C 0034095C  7D 03 43 78 */	mr r3, r8
/* 80343D80 00340960  4B FF 21 05 */	bl PPCMtdec
/* 80343D84 00340964  48 00 00 0C */	b lbl_80343D90
lbl_80343D88:
/* 80343D88 00340968  38 64 FF FF */	addi r3, r4, -1
/* 80343D8C 0034096C  4B FF 20 F9 */	bl PPCMtdec
lbl_80343D90:
/* 80343D90 00340970  48 00 6F B9 */	bl OSDisableScheduler
/* 80343D94 00340974  38 61 00 18 */	addi r3, r1, 0x18
/* 80343D98 00340978  48 00 14 BD */	bl OSClearContext
/* 80343D9C 0034097C  38 61 00 18 */	addi r3, r1, 0x18
/* 80343DA0 00340980  48 00 12 ED */	bl OSSetCurrentContext
/* 80343DA4 00340984  39 9E 00 00 */	addi r12, r30, 0
/* 80343DA8 00340988  7D 88 03 A6 */	mtlr r12
/* 80343DAC 0034098C  38 7F 00 00 */	addi r3, r31, 0
/* 80343DB0 00340990  38 9D 00 00 */	addi r4, r29, 0
/* 80343DB4 00340994  4E 80 00 21 */	blrl 
/* 80343DB8 00340998  38 61 00 18 */	addi r3, r1, 0x18
/* 80343DBC 0034099C  48 00 14 99 */	bl OSClearContext
/* 80343DC0 003409A0  7F A3 EB 78 */	mr r3, r29
/* 80343DC4 003409A4  48 00 12 C9 */	bl OSSetCurrentContext
/* 80343DC8 003409A8  48 00 6F C1 */	bl OSEnableScheduler
/* 80343DCC 003409AC  48 00 74 61 */	bl __OSReschedule
/* 80343DD0 003409B0  7F A3 EB 78 */	mr r3, r29
/* 80343DD4 003409B4  48 00 13 A1 */	bl OSLoadContext
/* 80343DD8 003409B8  80 01 02 F4 */	lwz r0, 0x2f4(r1)
/* 80343DDC 003409BC  83 E1 02 EC */	lwz r31, 0x2ec(r1)
/* 80343DE0 003409C0  83 C1 02 E8 */	lwz r30, 0x2e8(r1)
/* 80343DE4 003409C4  7C 08 03 A6 */	mtlr r0
/* 80343DE8 003409C8  83 A1 02 E4 */	lwz r29, 0x2e4(r1)
/* 80343DEC 003409CC  83 81 02 E0 */	lwz r28, 0x2e0(r1)
/* 80343DF0 003409D0  38 21 02 F0 */	addi r1, r1, 0x2f0
/* 80343DF4 003409D4  4E 80 00 20 */	blr 
} // clang-format on
#pragma pop

#pragma push
asm void DecrementerExceptionHandler(__OSException exception, OSContext* context)
{ // clang-format off
    nofralloc
/* 80343DF8 003409D8  90 04 00 00 */	stw r0, 0(r4)
/* 80343DFC 003409DC  90 24 00 04 */	stw r1, 4(r4)
/* 80343E00 003409E0  90 44 00 08 */	stw r2, 8(r4)
/* 80343E04 003409E4  BC C4 00 18 */	stmw r6, 0x18(r4)
/* 80343E08 003409E8  7C 11 E2 A6 */	mfspr r0, 0x391
/* 80343E0C 003409EC  90 04 01 A8 */	stw r0, 0x1a8(r4)
/* 80343E10 003409F0  7C 12 E2 A6 */	mfspr r0, 0x392
/* 80343E14 003409F4  90 04 01 AC */	stw r0, 0x1ac(r4)
/* 80343E18 003409F8  7C 13 E2 A6 */	mfspr r0, 0x393
/* 80343E1C 003409FC  90 04 01 B0 */	stw r0, 0x1b0(r4)
/* 80343E20 00340A00  7C 14 E2 A6 */	mfspr r0, 0x394
/* 80343E24 00340A04  90 04 01 B4 */	stw r0, 0x1b4(r4)
/* 80343E28 00340A08  7C 15 E2 A6 */	mfspr r0, 0x395
/* 80343E2C 00340A0C  90 04 01 B8 */	stw r0, 0x1b8(r4)
/* 80343E30 00340A10  7C 16 E2 A6 */	mfspr r0, 0x396
/* 80343E34 00340A14  90 04 01 BC */	stw r0, 0x1bc(r4)
/* 80343E38 00340A18  7C 17 E2 A6 */	mfspr r0, 0x397
/* 80343E3C 00340A1C  90 04 01 C0 */	stw r0, 0x1c0(r4)
/* 80343E40 00340A20  4B FF FD 88 */	b DecrementerExceptionCallback
} // clang-format on
#pragma pop
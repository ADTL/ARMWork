// 
// STM32F4DISCOVERY��LED�`�J�`�J�T���v��(�ŏ��̈���p)
// 
// ChaN����̍쐬����L�`�J�v���O����(mary_1st.zip)���Q�l�ɂ���
// ��L�]���{�[�h�p�̎��s�t�@�C���𐶐��ł���悤�ɒ����쐬����
// ���̂ł��B
// �����̃w�b�_�t�@�C���́Cstm32f4_dsp_stdperiph_lib.zip��
// /STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/�ȉ���
// �f�B���N�g������R�s�[�������̂ł��B
// Libraries/CMSIS/Device/ST/Include
// Libraries/CMSIS/Include
// 
// 2012/02/05
// inc/core_cmInstr.h �ɂ��ẮCgas�̃o�O�ɂ��G���[��������邽�߂�
// �C�����C���֐���__STREXB,__STREXH,__STREXW�ł̃��W�X�^�I�[�o���b�v��
// �}�~������悤�ɃC�����C���A�Z���u���������C�����Ă��܂��B
// �C���ӏ��́Cinc/core_cmInstr.h.diff���Q�Ƃ̂��ƁB
// http://old.nabble.com/-Bug-gas-13215--New%3A-ARM-Cortex-M3-strexh-strexb-instructions-with-same-registers-generates-error-td32516436.html
// 
// �r���h���s���́Ccygwin��z�肵�Ă��܂��B
// toolchain�́CARM_Sourcery_G++(GCC 4.6.1�x�[�X)��z�肵�Ă��܂��B
// STM32 ST-LINK Utility�����g�p���ĕ]���{�[�h�Ƀ��[�h�����s���܂��B
// 
// �Q�l�T�C�g�F
// ChaN�����ELM Home page ( http://elm-chan.org/junk/32bit/report.html )
// �˂ނ�����u���O ( http://nemuisan.blog.bai.ne.jp/ )
// STmicro Home page ( http://www.st.com/stm32f4 )
// 
[build_log]
$ make clean

rm -f -r obj/* | exit 0

$ make
arm-none-eabi-gcc.exe (Sourcery CodeBench Lite 2011.09-69) 4.6.1
Copyright (C) 2011 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


src/vector.c :
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mthumb-interwork -mfpu=fpv4-sp-d16
 -mfloat-abi=softfp -fipa-sra -I. -std=gnu89 -gdwarf-2 -fno-schedule-insns2 -Os
-Wall -Wextra -Iinc  -Wp,-M,-MP,-MT,obj/vector.o src/vector.c -o obj/vector.o

src/main.c :
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mthumb-interwork -mfpu=fpv4-sp-d16
 -mfloat-abi=softfp -fipa-sra -I. -std=gnu89 -gdwarf-2 -fno-schedule-insns2 -Os
-Wall -Wextra -Iinc  -Wp,-M,-MP,-MT,obj/main.o src/main.c -o obj/main.o

Linking...
arm-none-eabi-gcc -mthumb  -mcpu=cortex-m4 -mthumb-interwork -mfpu=fpv4-sp-d16 -
mfloat-abi=softfp -fipa-sra -I. -std=gnu89 -gdwarf-2 -fno-schedule-insns2 -Os -W
all -Wextra -Iinc  -Wp,-M,-MP,-MT,obj/template.o    obj/vector.o obj/main.o --ou
tput obj/template.elf -nostartfiles -Wl,-Map=obj/template.map,--cref,--gc-sectio
ns -lc -lgcc   -lm -Tstm32f4xx.ld

arm-none-eabi-objcopy -S -O ihex obj/template.elf obj/template.hex

arm-none-eabi-objdump -h -S -C obj/template.elf > obj/template.lst

arm-none-eabi-nm -n obj/template.elf > obj/template.sym

arm-none-eabi-size -A -x obj/template.elf
obj/template.elf  :
section             size         addr
.text              0x2c4    0x8000000
.bss                 0x4   0x20000000
.comment            0x30          0x0
.ARM.attributes     0x39          0x0
.debug_aranges      0x48          0x0
.debug_info        0x866          0x0
.debug_abbrev      0x259          0x0
.debug_line        0x1e6          0x0
.debug_frame        0x94          0x0
.debug_str         0x2f7          0x0
.debug_loc          0xd3          0x0
.debug_ranges       0x48          0x0
Total             0x14c4


$ make clean

rm -f -r obj/* | exit 0

$ make ON_SRAM=YES
arm-none-eabi-gcc.exe (Sourcery CodeBench Lite 2011.09-69) 4.6.1
Copyright (C) 2011 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


src/vector.c :
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mthumb-interwork -mfpu=fpv4-sp-d16
 -mfloat-abi=softfp -fipa-sra -I. -std=gnu89 -gdwarf-2 -fno-schedule-insns2 -Os
-Wall -Wextra -Iinc -DVECTOR_ON_SRAM -Wp,-M,-MP,-MT,obj/vector.o src/vector.c -o
 obj/vector.o

src/main.c :
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mthumb-interwork -mfpu=fpv4-sp-d16
 -mfloat-abi=softfp -fipa-sra -I. -std=gnu89 -gdwarf-2 -fno-schedule-insns2 -Os
-Wall -Wextra -Iinc -DVECTOR_ON_SRAM -Wp,-M,-MP,-MT,obj/main.o src/main.c -o obj
/main.o

Linking...
arm-none-eabi-gcc -mthumb  -mcpu=cortex-m4 -mthumb-interwork -mfpu=fpv4-sp-d16 -
mfloat-abi=softfp -fipa-sra -I. -std=gnu89 -gdwarf-2 -fno-schedule-insns2 -Os -W
all -Wextra -Iinc -DVECTOR_ON_SRAM -Wp,-M,-MP,-MT,obj/template.o    obj/vector.o
 obj/main.o --output obj/template.elf -nostartfiles -Wl,-Map=obj/template.map,--
cref,--gc-sections -lc -lgcc   -lm -Tstm32f4xx_ram.ld

arm-none-eabi-objcopy -S -O ihex obj/template.elf obj/template.hex

arm-none-eabi-objdump -h -S -C obj/template.elf > obj/template.lst

arm-none-eabi-nm -n obj/template.elf > obj/template.sym

arm-none-eabi-size -A -x obj/template.elf
obj/template.elf  :
section             size         addr
.text              0x2c4   0x20000000
.bss                 0x4   0x10000000
.comment            0x30          0x0
.ARM.attributes     0x39          0x0
.debug_aranges      0x48          0x0
.debug_info        0x866          0x0
.debug_abbrev      0x259          0x0
.debug_line        0x1e6          0x0
.debug_frame        0x94          0x0
.debug_str         0x2f7          0x0
.debug_loc          0xd3          0x0
.debug_ranges       0x48          0x0
Total             0x14c4


$

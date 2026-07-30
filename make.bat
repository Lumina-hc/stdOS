@echo off
nasm -f bin boot.asm -o boot.bin
nasm -f elf32 kernel.asm -o khead.o
nasm -f elf32 kernelClang\CFunc.asm -o cfunc.o
nasm -f elf32 kernelAsm\basicFunc.asm -o basic.o
nasm -f elf32 kernelAsm\key.asm -o key.o
nasm -f elf32 kernelAsm\global.asm -o global.o
nasm -f elf32 kernelAsm\idt.asm -o idt.o

i686-elf-gcc -m32 -ffreestanding -nostdlib -c kernelClang\std.c -o std.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c kernel.c -o kernel.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c kernelClang\diskio.c -o diskio.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c kernelClang\fat16.c -o fat16.o

i686-elf-ld -T linker.ld -o kernel.elf khead.o kernel.o cfunc.o basic.o key.o global.o idt.o std.o diskio.o fat16.o
i686-elf-objcopy -O binary kernel.elf kernel.bin

if %errorlevel% neq 0 exit /b %errorlevel%

powershell -Command ^
$T=131072; $S=512; $R=64; $FC=2; $FS=128; $RE=512; $RS=($RE*32)/$S; $DS=$R+$FC*$FS+$RS; $KS=63; ^
$img=New-Object byte[] ([int]($T*$S)); ^
function sw($a,$o,$v){ $a[$o]=$v-band0xFF; $a[$o+1]=($v-shr8)-band0xFF }; ^
function sdw($a,$o,$v){ $a[$o]=$v-band0xFF; $a[$o+1]=($v-shr8)-band0xFF; $a[$o+2]=($v-shr16)-band0xFF; $a[$o+3]=($v-shr24)-band0xFF }; ^
$bpb=New-Object byte[] $S; ^
$bpb[0]=0xEB;$bpb[1]=0x3C;$bpb[2]=0x90; ^
$enc=[System.Text.Encoding]::ASCII; ^
$enc.GetBytes('STDOS   ').CopyTo($bpb,3); ^
sw $bpb 11 512; $bpb[13]=4; sw $bpb 14 $R; $bpb[16]=$FC; ^
sw $bpb 17 $RE; $bpb[21]=0xF8; sw $bpb 22 $FS; ^
sw $bpb 24 63; sw $bpb 26 16; ^
sdw $bpb 32 $T; ^
$bpb[510]=0x55;$bpb[511]=0xAA; ^
$boot=[System.IO.File]::ReadAllBytes('boot.bin'); ^
for($i=62;$i-lt[Math]::Min($boot.Length,510);$i++){ $bpb[$i]=$boot[$i] }; ^
$bpb.CopyTo($img,0); ^
$kernel=[System.IO.File]::ReadAllBytes('kernel.bin'); ^
if($kernel.Length-gt($KS*$S)){ throw 'kernel too large' }; ^
[Array]::Copy($kernel,0,$img,1*$S,$kernel.Length); ^
$fo=$R*$S; ^
sw $img ($fo+0) 0xFFF8; sw $img ($fo+2) 0xFFFF; ^
$ro=($R+$FC*$FS)*$S; ^
$root=New-Object byte[] ([int]($RS*$S)); ^
$enc.GetBytes('README   ').CopyTo($root,0); ^
$enc.GetBytes('TXT').CopyTo($root,8); ^
$root[11]=0x20; $rclu=2; sw $root 26 $rclu; ^
$rdata=$enc.GetBytes('stdOS FAT16'+"`n"); ^
sdw $root 28 $rdata.Length; ^
$root.CopyTo($img,$ro); ^
$do=$DS*$S; ^
for($i=0;$i-lt$rdata.Length;$i++){ $img[$do+($rclu-2)*$S*4+$i]=$rdata[$i] }; ^
sw $img ($fo+$rclu*2) 0xFFFF; ^
for($i=0;$i-lt$FS*$S;$i++){ $img[$fo+$FS*$S+$i]=$img[$fo+$i] }; ^
[System.IO.File]::WriteAllBytes('stdos.img',$img)

del boot.bin *.o *.elf

qemu-system-i386 -drive file=stdos.img,format=raw,if=ide

$TOTAL=131072; $SEC=512; $RES=64; $FAT_CNT=2; $FAT_SEC=128
$ROOT_ENT=512; $ROOT_SEC=($ROOT_ENT*32)/$SEC
$DATA_START=$RES+$FAT_CNT*$FAT_SEC+$ROOT_SEC
$KERNEL_SEC=63

$img=New-Object byte[] ([int]($TOTAL*$SEC))
function sw($a,$o,$v){ $a[$o]=$v-band0xFF; $a[$o+1]=($v-shr8)-band0xFF }
function sdw($a,$o,$v){ $a[$o]=$v-band0xFF; $a[$o+1]=($v-shr8)-band0xFF; $a[$o+2]=($v-shr16)-band0xFF; $a[$o+3]=($v-shr24)-band0xFF }

$bpb=New-Object byte[] $SEC
$bpb[0]=0xEB;$bpb[1]=0x3C;$bpb[2]=0x90
$enc=[System.Text.Encoding]::ASCII
$enc.GetBytes("STDOS   ").CopyTo($bpb,3)
sw $bpb 11 512; $bpb[13]=4; sw $bpb 14 $RES; $bpb[16]=$FAT_CNT
sw $bpb 17 $ROOT_ENT; $bpb[21]=0xF8; sw $bpb 22 $FAT_SEC
sw $bpb 24 63; sw $bpb 26 16
sdw $bpb 32 $TOTAL
$bpb[510]=0x55;$bpb[511]=0xAA

$boot=[System.IO.File]::ReadAllBytes("boot.bin")
for($i=62;$i-lt[Math]::Min($boot.Length,510);$i++){ $bpb[$i]=$boot[$i] }
$bpb.CopyTo($img,0)

$kernel=[System.IO.File]::ReadAllBytes("kernel.bin")
$pad=$KERNEL_SEC*$SEC
if($kernel.Length-gt$pad){ throw "kernel.bin too large ($($kernel.Length) > $pad)" }
[Array]::Copy($kernel,0,$img,1*$SEC,$kernel.Length)

$fat_off=$RES*$SEC
sw $img ($fat_off+0) 0xFFF8
sw $img ($fat_off+2) 0xFFFF

$root_off=($RES+$FAT_CNT*$FAT_SEC)*$SEC
$root=New-Object byte[] ([int]($ROOT_SEC*$SEC))
$enc.GetBytes("README   ").CopyTo($root,0)
$enc.GetBytes("TXT").CopyTo($root,8)
$root[11]=0x20
$rclu=2; sw $root 26 $rclu
$rdata = $enc.GetBytes("stdOS FAT16" + [char]10)
sdw $root 28 $rdata.Length
$root.CopyTo($img,$root_off)

$data_off=$DATA_START*$SEC
for($i=0;$i-lt$rdata.Length;$i++){ $img[$data_off+($rclu-2)*$SEC*4+$i]=$rdata[$i] }
sw $img ($fat_off+$rclu*2) 0xFFFF

for($i=0;$i-lt$FAT_SEC*$SEC;$i++){ $img[$fat_off+$FAT_SEC*$SEC+$i]=$img[$fat_off+$i] }

[System.IO.File]::WriteAllBytes("stdos.img",$img)
Write-Host "stdos.img (64MB FAT16) created"
Write-Host "Kernel: $($kernel.Length) bytes in $KERNEL_SEC sectors"
